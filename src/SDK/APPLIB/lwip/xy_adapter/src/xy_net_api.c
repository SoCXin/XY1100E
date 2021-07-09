/**
 * @file xy_net_api.c
 * @brief 
 * @version 1.0
 * @date 2021-04-26
 * @copyright Copyright (c) 2021  芯翼信息科技有限公司
 * 
 */

#include "xy_net_api.h"
#include "xy_system.h"
#include "xy_utils.h"
#include "xy_at_api.h"
#include "at_ps_cmd.h"
#include "ps_netif_api.h"
#include "oss_nv.h"
#include "lwip/dns.h"
#include "lwip/sockets.h"

/*******************************************************************************
 *						Global function implementations						   *
 ******************************************************************************/
int xy_dns_set(ip4_addr_t *pridns, ip4_addr_t *secdns)
{
	ip_addr_t pridns_t = {0};
	ip_addr_t secdns_t = {0};
	char pridns_str[IP4ADDR_STRLEN_MAX] = {0};
	char secdns_str[IP4ADDR_STRLEN_MAX] = {0};

	if (pridns->addr != 0 && secdns->addr != 0)
	{
		memcpy(&pridns_t.u_addr.ip4, pridns, sizeof(ip4_addr_t));
		pridns_t.type = IPADDR_TYPE_V4;
		memcpy(&secdns_t.u_addr.ip4, secdns, sizeof(ip4_addr_t));
		secdns_t.type = IPADDR_TYPE_V4;

		memcpy(pridns_str, ip4addr_ntoa(pridns), sizeof(pridns_str));
		memcpy(secdns_str, ip4addr_ntoa(secdns), sizeof(secdns_str));
		memset(g_softap_fac_nv->ref_dns, 0, sizeof(g_softap_fac_nv->ref_dns));
		snprintf((char *)g_softap_fac_nv->ref_dns, sizeof(g_softap_fac_nv->ref_dns), "%s,%s", pridns_str, secdns_str);
		dns_setserver(0, &pridns_t);
		dns_setserver(1, &secdns_t);
		SAVE_FAC_PARAM(ref_dns);
		return XY_OK;
	}
	else if (pridns->addr != 0)
	{
		memcpy(&pridns_t.u_addr.ip4, pridns, sizeof(ip4_addr_t));
		pridns_t.type = IPADDR_TYPE_V4;

		memcpy(pridns_str, ip4addr_ntoa(pridns), sizeof(pridns_str));
		memset(g_softap_fac_nv->ref_dns, 0, sizeof(g_softap_fac_nv->ref_dns));
		snprintf((char *)g_softap_fac_nv->ref_dns, sizeof(g_softap_fac_nv->ref_dns), "%s", pridns_str);
		dns_setserver(0, &pridns_t);
		SAVE_FAC_PARAM(ref_dns);	
		return XY_OK;
	}
	return XY_ERR;
}

int xy_dns_set2(char *pri_dns, char *sec_dns)
{
	ip4_addr_t pridns = {0};
	ip4_addr_t secdns = {0};
	if(strlen(pri_dns) != 0 || strlen(sec_dns) != 0)
	{
		if(strcmp(pri_dns,""))
			inet_pton(AF_INET, pri_dns, &pridns);
		
		if(strcmp(sec_dns,""))
			inet_pton(AF_INET, sec_dns, &secdns);
		if(xy_dns_set(&pridns,&secdns) != XY_OK)
		{
			return XY_ERR;
		}
		return XY_OK;
	}
	return XY_ERR;
}

int xy_getIP4Addr(char *ipAddr, int addrLen)
{
	if (xy_wait_tcpip_ok(0) == XY_ERR || ipAddr == NULL || addrLen < IPADDR_LEN)
		return XY_ERR;

	if (inet_ntop(AF_INET, &netif_default->ip_addr.u_addr.ip4, ipAddr, addrLen) == NULL)
		return XY_ERR;

	return XY_OK;
}

//该接口内部禁止调用softap_printf，可调用send_debug_str_to_at_uart
int xy_get_ip4addr(unsigned int *ip4addr)
{
	if (xy_wait_tcpip_ok(1) == XY_ERR)
		return XY_ERR;
	
	*ip4addr = (unsigned int)(netif_default->ip_addr.u_addr.ip4.addr);
	return XY_OK;
}

//该接口内部禁止调用softap_printf，因为此接口可能在云保存过程中（IDLE线程中）被调用到，可调用send_debug_str_to_at_uart接口。
int xy_wait_tcpip_ok(int timeout)
{
    int timeout_compare = 0;
	if (osKernelIsRunningIdle() == osOK && timeout > 0)
    {
        timeout = 0;
    }
    while (ps_netif_is_ok() == 0)
    {
        if (g_check_valid_sim == 0)
        {
        	send_debug_str_to_at_uart("xy_wait_tcpip_ok ERR! NO SIM CARD");
            return  XY_ERR;
        }

        if (timeout == 0)
        {
        	send_debug_str_to_at_uart("xy_wait_tcpip_ok ERR! PDP not active");
            return XY_ERR;
        }
        else if (timeout == (int)(osWaitForever))
        {
            osDelay(100);
        }
        else
        {
            osDelay(100);
            timeout_compare += 100;
            if (timeout_compare >= timeout * 1000)
            {
            	send_debug_str_to_at_uart("xy_wait_tcpip_ok ERR! PDP not active");
                return XY_ERR;
            }
        }
    }
    return XY_OK;
}
