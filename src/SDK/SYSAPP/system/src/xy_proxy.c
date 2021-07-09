/*******************************************************************************
 *							 Include header files							   *
 ******************************************************************************/
#include "xy_proxy.h"
#include "at_ctl.h"
#include "inter_core_msg.h"
#include "ipcmsg.h"
#include "ps_netif_api.h"
#include "xy_utils.h"
#include "at_global_def.h"
#if TELECOM_VER
#include "cdp_backup.h"
#endif
#if MOBILE_VER
#include "onenet_backup_proc.h"
#endif

#ifdef XY_HTTP
#include "at_http.h"
#endif
/*******************************************************************************
 *                             Macro definitions                               *
 ******************************************************************************/
#if IS_DSP_CORE
#define XY_PROXY_THREAD_PRIO osPriorityBelowNormal1
#else //M3 CORE
#define XY_PROXY_THREAD_PRIO osPriorityNormal2
#endif //IS_DSP_CORE

#define XY_PROXY_THREAD_STACKSIZE 0xC00


/*******************************************************************************
 *						   Global variable definitions				           *
 ******************************************************************************/
osMessageQueueId_t xy_proxy_msg_q = NULL; //proxy msg queue
osMutexId_t xy_proxy_mutex = NULL;
/*******************************************************************************
 *						Global function implementations						   *
 ******************************************************************************/
void xy_proxy_ctl(void)
{
	xy_proxy_msg_t *msg = NULL;
	while(1) 
	{
		osMessageQueueGet(xy_proxy_msg_q, (void *)(&msg), NULL, osWaitForever);
		switch(msg->msg_id)
		{
			case PROXY_MSG_AT_RECV:
				proc_at_proxy_req((xy_proxy_msg_t *)msg);
				break;
			case PROXY_MSG_PS_PDP_ACT:
			{
				ps_netif_activate((struct pdp_active_info *)(msg->data));
				NETDOG_AT_STATICS(dbg_pdp_act_succ++);
			}
				break;
			//always,not permit user  to deactivate PDP,if receive PS CERV DEACT,do PDP act by dead cycle
			case PROXY_MSG_PS_PDP_DEACT:
			{
				ps_netif_deactivate(*((unsigned char *)msg->data), IPV4_TYPE);
				NETDOG_AT_STATICS(dbg_pdp_deact_num++);
				break;
			}
			case PROXY_MSG_INTER_CORE_WAIT_FREE:
			{
				shm_msg_write(msg->data, 4, ICM_FREE);			
				break;
			}
#if TELECOM_VER
			case PROXY_MSG_RESUME_CDP_EXCEPTION:
			{
	            if (!cdp_handle_exist() && g_cdp_timeout_restart_TskHandle == NULL)
	            {
	                g_cdp_timeout_restart_TskHandle = osThreadNew((osThreadFunc_t)(cdp_timeout_restart_process), NULL, "cdp_timeout_restart", 0x400, XY_OS_PRIO_NORMAL1);
	            }
				break;
			}
			case PROXY_MSG_RESUME_CDP_UPDATE:
			{
                if (g_cdp_rtc_resume_TskHandle == NULL)
                {
                    g_cdp_rtc_resume_TskHandle = osThreadNew((osThreadFunc_t)(cdp_rtc_resume_update_process), NULL, "cdp_keelive_update", 0x400, XY_OS_PRIO_NORMAL1);
                }

				break;
			}
#endif
#if MOBILE_VER
			case PROXY_MSG_RESUME_ONENET_UPDATE:
			{
	            if (g_onenet_rtc_resume_TskHandle == NULL)
	            {
	                g_onenet_rtc_resume_TskHandle = osThreadNew((osThreadFunc_t)(onenet_rtc_resume_process), NULL, "onenet_keelive_update", 0x400, XY_OS_PRIO_NORMAL1);
	            }
                break;
			}
#endif

#ifdef XY_HTTP
			case PROXY_MSG_HTTP_RESPOND:
			{
				proxy_http_parse((Http_Para_t *)(*((int *)msg->data)));
				break;
			}
#endif
			default:
				break;
		}
		xy_free(msg);
	}
}

int send_msg_2_proxy(int msg_id, void *buff, int len)
{
	osMutexAcquire(xy_proxy_mutex, osWaitForever);
	xy_assert(xy_proxy_msg_q != NULL);
	xy_proxy_msg_t *msg = NULL;
	
	msg = xy_malloc(sizeof(xy_proxy_msg_t) + len);
	msg->msg_id = msg_id;
	
	msg->size = len;

	if (buff != NULL)
		memcpy(msg->data, buff, len);
	osMessageQueuePut(xy_proxy_msg_q, &msg, 0, osWaitForever);
	osMutexRelease(xy_proxy_mutex);
	return XY_OK;
}

void xy_proxy_init(void)
{
	xy_proxy_msg_q = osMessageQueueNew(30, sizeof(void *));
	xy_proxy_mutex = osMutexNew(osMutexPrioInherit);

	osThreadNew((osThreadFunc_t)(xy_proxy_ctl), NULL, XY_PROXY_THREAD_NAME, XY_PROXY_THREAD_STACKSIZE, XY_PROXY_THREAD_PRIO);
}
