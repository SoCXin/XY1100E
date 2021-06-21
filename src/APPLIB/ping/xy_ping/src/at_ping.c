#include "xy_at_api.h"
#include "at_ping.h"
#include "at_ps_cmd.h"
#include "ping.h"
#include "lwip/sockets.h"
#include "ps_netif_api.h"
#include "xy_utils.h"

#if XY_PING

/**
 * @brief AT+NPING=<host>,<data_len>,<ping_num>,<time_out>,<interval_time>[,<rai>]
 */
int at_NPING_req(char *at_buf,char **prsp_cmd)
{
	char *host = xy_zalloc(40);
	int data_len = 0;
	int ping_num = 0;
	int time_out = 0;
	int interval_time = 0;
	int rai_val = -1;

	if (at_parse_param("%40s,%d,%d,%d,%d,%d", at_buf, host, &data_len, &ping_num, &time_out, &interval_time, &rai_val) != AT_OK || rai_val > 2 || time_out <= 0)
	{
		*prsp_cmd = AT_ERR_BUILD(ATERR_PARAM_INVALID);
		goto out;
	}
	if (!ps_netif_is_ok())
    {
        softap_printf(USER_LOG, WARN_LOG, "ps netif is not ok!");
		*prsp_cmd = AT_ERR_BUILD(ATERR_NOT_NET_CONNECT);
		goto out;
    }
	if (g_OOS_flag == 1)
	{
    	softap_printf(USER_LOG, WARN_LOG, "ps netif is OOS!");
		*prsp_cmd = AT_ERR_BUILD(ATERR_NOT_NET_CONNECT);
		goto out;
    }
	if (start_ping(AF_INET, host, data_len, ping_num, time_out, interval_time, rai_val) == XY_ERR)
	{
        *prsp_cmd = AT_ERR_BUILD(ATERR_NOT_ALLOWED);
    }

out:
    xy_free(host);
	return AT_END;
}

int at_NPINGSTOP_req(char *at_buf,char **prsp_cmd)
{
	(void) at_buf;
	(void) prsp_cmd;

    stop_ping();
	return AT_END;
}

int at_NPING6_req(char *at_buf,char **prsp_cmd)
{
	char *host = xy_zalloc(40);
	int data_len = 0;
	int ping_num = 0;
	int time_out = 0;
	int interval_time = 0;
	int rai_val = -1;

	if (at_parse_param("%40s,%d,%d,%d,%d,%d", at_buf, host, &data_len, &ping_num, &time_out, &interval_time, &rai_val) != AT_OK || rai_val > 2)
	{
		*prsp_cmd = AT_ERR_BUILD(ATERR_PARAM_INVALID);
		goto out;
	}
	if (!ps_netif_is_ok())
    {
        softap_printf(USER_LOG, WARN_LOG, "ps netif is not ok!");
		*prsp_cmd = AT_ERR_BUILD(ATERR_NOT_ALLOWED);
		goto out;
    }
	if (g_OOS_flag == 1)
	{
    	softap_printf(USER_LOG, WARN_LOG, "ps netif is OOS!");
		*prsp_cmd = AT_ERR_BUILD(ATERR_NOT_NET_CONNECT);
		goto out;
    }
	if (at_strstr(host, "ADDR") != NULL) //自测使用
	{
		char *ip_buff = xy_zalloc(40);
		*prsp_cmd = xy_zalloc(64);
		snprintf(*prsp_cmd, 64, "\r\n%s\r\n\r\nOK\r\n", inet_ntop(AF_INET6, netif_ip6_addr(netif_get_default(), 1), ip_buff, 40));
		xy_free(ip_buff);
		goto out;
	}
	if (start_ping(AF_INET6, host, data_len, ping_num, time_out, interval_time, rai_val) == XY_ERR)
	{
        *prsp_cmd = AT_ERR_BUILD(ATERR_NOT_ALLOWED);
    }

out:
	xy_free(host);
	return AT_END;
}

#endif //XY_PING
