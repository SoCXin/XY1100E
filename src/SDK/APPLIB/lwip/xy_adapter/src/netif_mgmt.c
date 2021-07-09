/*******************************************************************************
 *							 Include header files							   *
 ******************************************************************************/
#include "netif_mgmt.h"
#include "at_global_def.h"
#include "at_ps_cmd.h"
#include "at_ctl.h"
#include "xy_utils.h"
#include "xy_system.h"
#include "xy_ps_ext_api.h"
#include "ps_netif_api.h"
#include "inter_core_msg.h"

/*******************************************************************************
 *						  Global variable definitions						   *
 ******************************************************************************/
osMutexId_t g_netif_mgmt_mutex = NULL;
netif_mgmt_event_callback_node_t *netif_mgmt_event_callback_list = NULL;

/*******************************************************************************
 *						Global function implementations						   *
 ******************************************************************************/
//m3 core start up will execute,should notice kernel may not running!!!
int netif_mgmt_add_event_callback_to_list(netif_mgmt_event_callback_node_t *netif_mgmt_event_callback_node)
{
	if (osKernelGetState() != osKernelInactive)
    	osMutexAcquire(g_netif_mgmt_mutex, osWaitForever);	
    if (netif_mgmt_event_callback_list == NULL)
    {
        netif_mgmt_event_callback_list = netif_mgmt_event_callback_node;
    }
    else
    {
        netif_mgmt_event_callback_node_t *tmp = netif_mgmt_event_callback_list;
        while(tmp->next != NULL)
            tmp = tmp->next;
        tmp->next = netif_mgmt_event_callback_node;
    }
	if (osKernelGetState() != osKernelInactive)
    	osMutexRelease(g_netif_mgmt_mutex);

    return 0;
}

int netif_mgmt_init_event_callback(netif_mgmt_event_callback_t *event_callback, netif_event_callback_func func, netif_mgmt_event event)
{
    if (event_callback == NULL)
    {
        xy_assert(0);
    }
    memset(event_callback, 0, sizeof(netif_mgmt_event_callback_t));
    event_callback->callback = func;
    event_callback->netif_mgmt_event_bitmap |= 1 << (int)event;
    return 1;
}

int netif_mgmt_add_event_callback(netif_mgmt_event_callback_t *event_callback)
{
    if (event_callback == NULL)
    {
        xy_assert(0);
    }
    netif_mgmt_event_callback_node_t *netif_mgmt_event_callback_node = NULL;
    netif_mgmt_event_callback_node = xy_zalloc(sizeof(netif_mgmt_event_callback_node_t));
    netif_mgmt_event_callback_node->event_callback = event_callback;

    netif_mgmt_add_event_callback_to_list(netif_mgmt_event_callback_node);
    return 0;
}

int netif_mgmt_del_event_callback(netif_mgmt_event_callback_t *event_callback)
{
    netif_mgmt_event_callback_node_t *prev = NULL;
    netif_mgmt_event_callback_node_t *cur = NULL;
	netif_mgmt_event_callback_node_t *temp = NULL;

    if(netif_mgmt_event_callback_list == NULL)
        return -1;
    
    osMutexAcquire(g_netif_mgmt_mutex, osWaitForever);
    if (netif_mgmt_event_callback_list->event_callback == event_callback)
    {
		temp = netif_mgmt_event_callback_list;
		netif_mgmt_event_callback_list = netif_mgmt_event_callback_list->next;
		xy_free(temp);
    }
    else
    {
        prev = netif_mgmt_event_callback_list;
        cur =  netif_mgmt_event_callback_list->next;
        while (cur != NULL)
        {
            if (cur->event_callback == event_callback)
            {
				temp = cur;
				prev->next = cur->next;
				xy_free(temp);
                break;
            }
            prev = cur;
            cur = cur->next;
        }
    }

    osMutexRelease(g_netif_mgmt_mutex);
    return 0;
}

void netif_mgmt_status_callback_proc(struct netif *netif)
{
    netif_mgmt_event_callback_node_t *cur = NULL;
    xy_netif_info_t netif_info = {0};

    //OutputTraceMessage(1, "netif_mgmt_status_callback_proc, in");
    osMutexAcquire(g_netif_mgmt_mutex, osWaitForever);
    cur = netif_mgmt_event_callback_list;
    if(netif->flags & NETIF_FLAG_UP)
    {
		netif_mgmt_event event = NETIF_MGMT_EVENT_UP;
        netif_info.state = XY_NETIF_UP;
#if LWIP_IPV6
		if(netif_ip6_addr_state(netif, 1) == IP6_ADDR_PREFERRED)
		{
			event= NETIF_MGMT_EVENT_IPV6;
		}
#endif
		while (cur != NULL)
		{
			//OutputTraceMessage(1, "netif_mgmt_status_callback_proc, 1");
			if ((cur->event_callback->netif_mgmt_event_bitmap & (1 << (int)event)) && 
			(strcmp(cur->event_callback->netif_name, "") == 0 || strcmp(cur->event_callback->netif_name, netif->name) == 0))
			{
			    cur->event_callback->callback(netif, event);
			}
			cur = cur->next;
		}
	}
	else
	{
        netif_info.state = XY_NETIF_DOWN;
	    while (cur != NULL)
	    {
	        //OutputTraceMessage(1, "netif_mgmt_status_callback_proc, 2");
	        if ((cur->event_callback->netif_mgmt_event_bitmap & (1 << (int)NETIF_MGMT_EVENT_DOWN)) && 
	            (strcmp(cur->event_callback->netif_name, "") == 0 || strcmp(cur->event_callback->netif_name, netif->name) == 0))
	        {
	            cur->event_callback->callback(netif, NETIF_MGMT_EVENT_DOWN);
	        }
	        cur = cur->next;
	    }
	}
    notify_Ps_Event(XY_PS_EVENT_NETIF_STATE, &netif_info, sizeof(xy_netif_info_t));
    osMutexRelease(g_netif_mgmt_mutex);
    //OutputTraceMessage(1, "netif_mgmt_status_callback_proc, out");
}

void set_netif_mgmt_status_callback(struct netif *netif)
{
    netif_set_status_callback(netif, netif_mgmt_status_callback_proc);
}

void netif_mgmt_init()
{
	g_out_OOS_sem = osSemaphoreNew(1, 0);
    g_netif_mgmt_mutex = osMutexNew(osMutexPrioInherit);
}