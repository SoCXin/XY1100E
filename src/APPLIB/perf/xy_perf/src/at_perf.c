#include "at_perf.h"
#include "xyperf.h"
#include "xy_at_api.h"
#if XY_PING
#include "at_ping.h"
#endif
#include "at_ps_cmd.h"
#include "ps_netif_api.h"

#if XY_PERF

/**
 * AT+XYPERF=<host>,<port>,<net_type>,<size>,<bandwidth>,<time>,<param>
 * time:单位秒,param:固定为1
 */
int at_XYPERF_req(char *at_buf, char **prsp_cmd)
{
    char *remote_ip = NULL;
    int remote_port = 0;
    char protocol_str[4] = {0};
    int packet_size = 0;
    char bandwidth_str[16] = {0};
    int len = 0;
    int duration = 0;
    int print_interval = 0;
    char protocol_type;
    int base = 0;
    int times = 1;
    int rai_val = 0;
    int i;
    remote_ip = xy_zalloc(16);

    if (at_parse_param("%16s,%d,%4s,%d,%s,%d,%d,%d", \
        at_buf, remote_ip, &remote_port, protocol_str, &packet_size, bandwidth_str, &duration, &print_interval, &rai_val) != AT_OK)
	{
		*prsp_cmd = AT_ERR_BUILD(ATERR_PARAM_INVALID);
		goto out;
	}
    if (g_xyperf_TskHandle != NULL)
    {
        //灌包处理线程已经运行
		*prsp_cmd = AT_ERR_BUILD(ATERR_NOT_ALLOWED);
		goto out;
	}
	if (!ps_netif_is_ok())
    {
        softap_printf(USER_LOG, WARN_LOG, "xyperf ps netif is not ok!");
		*prsp_cmd = AT_ERR_BUILD(ATERR_NOT_NET_CONNECT);
        goto out;
    }

    if (strcmp(protocol_str, "udp") == 0)
    {
        protocol_type = 0;
    }
    else if (strcmp(protocol_str, "tcp") == 0)
    {
        protocol_type = 1;
    }
    else
    {
        *prsp_cmd = AT_ERR_BUILD(ATERR_PARAM_INVALID);
		goto out;
    }

	len = strlen(bandwidth_str);
    if (bandwidth_str[len - 1] < '0' || bandwidth_str[len - 1] > '9')
    {
        if (bandwidth_str[len - 1] == 'k' || bandwidth_str[len - 1] == 'K')
        {
            times = 1;
        }
        else if (bandwidth_str[len - 1] == 'm' || bandwidth_str[len - 1] == 'M')
        {
            times = 1024;
        }
        else if (bandwidth_str[len - 1] == 'g' || bandwidth_str[len - 1] == 'G')
        {
            times = 1024 * 1024;
        }
        else
        {
            *prsp_cmd = AT_ERR_BUILD(ATERR_PARAM_INVALID);
		    goto out;
        }
        for (i = 0; i < len - 1; i++)
        {
            if (bandwidth_str[i] < '0' || bandwidth_str[i] > '9')
            {
                *prsp_cmd = AT_ERR_BUILD(ATERR_PARAM_INVALID);
		        goto out;
            }
            else
            {
                base = base * 10 + bandwidth_str[i] - '0';
            }
        }
    }
    else
    {
        for (i = 0; i < len; i++)
        {
            if (bandwidth_str[i] < '0' || bandwidth_str[i] > '9')
            {
                *prsp_cmd = AT_ERR_BUILD(ATERR_PARAM_INVALID);
		        goto out;
            }
            else
            {
                base += base * 10 + bandwidth_str[i] - '0';
            }
        }
        if (base < 1024)
        {
            *prsp_cmd = AT_ERR_BUILD(ATERR_PARAM_INVALID);
		    goto out;
        }
        times = 1;
        base = base / 1024;
    }

    start_xyperf(protocol_type, remote_ip, remote_port, duration, packet_size, base * times, print_interval, rai_val);
out:
    xy_free(remote_ip);
	return AT_END;
}

#endif //XY_PERF
