#include "lwip/opt.h"
#if XY_PERF

#include "xy_utils.h" 
#include "xy_at_api.h" 
#if XY_PING
#include "net_utils.h"
#endif
#include "xyperf.h"
#include "oss_nv.h"
#include "ps_netif_api.h"
#include "lwip/ip.h"
#include "lwip/icmp.h"
#include "lwip/sockets.h"

osThreadId_t g_xyperf_TskHandle = NULL;

static inline void xyperf_upload_fin(int fd, uint32_t nb_packets, uint32_t end_time, uint32_t packet_size, struct sockaddr *remote_sockaddr, int rai_val)
{
	struct xyperf_udp_datagram datagram;
	int loop = 10;
	int ret;

	(void) packet_size;

    while (loop-- > 0)
    {
        /* Fill the packet header */
		datagram.id = htonl(-nb_packets);
		datagram.tv_sec = htonl(end_time / 1000);
		datagram.tv_usec = htonl((end_time % 1000) * 1000);

        ret = sendto2(fd, &datagram, sizeof(struct xyperf_udp_datagram), 0, (struct sockaddr*)remote_sockaddr, sizeof(struct sockaddr_in),0,rai_val);

        if (ret < 0)
        {
			softap_printf(USER_LOG, WARN_LOG, "xyperf Failed to send the packet (%d)\n", ret);
		}
    }
}

int xyperf_udp_ipv4_client(unsigned int duration, unsigned int packet_size, unsigned int rate_in_kbps, 
    char *remote_ip, int remote_port, unsigned int print_interval, int rai_val)
{
    uint32_t packet_duration;
    uint32_t duration_in_ms = duration * 1000;
	uint32_t delay;
	uint32_t nb_packets = 0;
	uint32_t start_time, last_print_time, end_time, loop_start_time, loop_end_time;
	int ret = 0;
    uint8_t *data = NULL;
    int fd = -1;
    struct xyperf_udp_datagram datagram;
    uint32_t total_ip_size = 0;
    struct sockaddr_in remote_sockaddr = {0};

    if (rate_in_kbps > 1024 * 1024)
    {
        //rate limitation, no more than 1Gbps
        return XY_ERR;
    }

    fd = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);

    if (fd < 0)
    {
        goto failed;
    }

    remote_sockaddr.sin_family = AF_INET;
    remote_sockaddr.sin_port = htons(remote_port);
    inet_aton(remote_ip, &remote_sockaddr.sin_addr);

    data = xy_zalloc(packet_size + sizeof(struct xyperf_udp_datagram));

    memset(data + sizeof(struct xyperf_udp_datagram), 'z', packet_size);

    //packet size + perf包头 + udp头部长度(8字节) + ipv4 ip报头长度(20字节)
    total_ip_size = packet_size + sizeof(struct xyperf_udp_datagram) + 8 + 20;

    delay = packet_duration = 1000 * total_ip_size * 8 / (rate_in_kbps * 1024); //单位ms

    softap_printf(USER_LOG, WARN_LOG, "start xyperf\n");
	softap_printf(USER_LOG, WARN_LOG, "\nxyperf ipv4 udp remote_ip:%s, remote_port:%d, data_len:%d, bandwidth:%dkbps\n", 
        remote_ip, remote_port, packet_size, rate_in_kbps);
	softap_printf(USER_LOG, WARN_LOG, "\nxyperf duration:%ds, print_interval:%d, xyperf_rai:%d\n", duration, print_interval, rai_val);

    start_time = osKernelGetTickCount();

    do
    {
        /* Timestamp */
		loop_start_time = osKernelGetTickCount();
        
        /* Fill the packet header */
		datagram.id = htonl(nb_packets);
		datagram.tv_sec = htonl(loop_start_time / 1000);
		datagram.tv_usec = htonl((loop_start_time % 1000) * 1000);
        memcpy(data, &datagram, sizeof(struct xyperf_udp_datagram));

        ret = sendto2(fd, data, packet_size + sizeof(struct xyperf_udp_datagram), 0, (struct sockaddr *)&remote_sockaddr, sizeof(struct sockaddr_in), 0, rai_val);

        if (ret < 0)
        {
			softap_printf(USER_LOG, WARN_LOG, "xyperf ERROR! Failed to send the packet (%d)\n", ret);
			goto failed;
		}
        else
        {
			nb_packets++;
		}

        /* Print log every second */
		if (loop_start_time - last_print_time > print_interval * 1000) {
			//OutputTraceMessage(1, "nb_packets=%u\tdelay=%u\tadjust=%d\n",
			//       nb_packets, delay, adjust);
			last_print_time = loop_start_time;
		}

        /*
        if (loop_start_time + delay > duration_in_ms + start_time)
        {
            break;
        }
        */

        loop_end_time = osKernelGetTickCount();
        
        /* Wait */
        if (loop_end_time - loop_start_time < delay)
        {
            osDelay(delay - (loop_end_time - loop_start_time));
        }
    } while (loop_start_time - start_time < duration_in_ms);

    end_time = osKernelGetTickCount();

	xyperf_upload_fin(fd, nb_packets, end_time, packet_size, (struct sockaddr *)&remote_sockaddr,rai_val);
    softap_printf(USER_LOG, WARN_LOG, "xyperf finished");
	send_rsp_str_to_ext("+XYPERF:finished\r\n");

    if (fd != -1)
    {
        close(fd);
    }
    if (data != NULL)
    {
        xy_free(data);
    }

    return XY_OK;

failed:
    if (fd != -1)
    {
        close(fd);
    }
    if (data != NULL)
    {
        xy_free(data);
    }
    return XY_ERR;
}

int packet_type_check(char *host)
{
#if LWIP_IPV6
	struct in6_addr ipv6_target;
#endif

	int iptype = -1;

#if LWIP_IPV6
	if (inet6_aton(host, &ipv6_target) == 0)
	{
		iptype = AF_INET;
	}
	else
	{
		iptype = AF_INET6;
	}
#else
#if LWIP_IPV4
	iptype = AF_INET;
#endif  //LWIP_IPV4
#endif  //LWIP_IPV6

#if !LWIP_IPV4
	if (iptype == AF_INET)
	{
		iptype = -1;
	}
#endif

#if !LWIP_IPV6
	if (iptype == AF_INET6)
	{
		iptype = -1;
	}
#endif

	return iptype;
}

void process_xyperf(xyperf_arguments_t *xyperf_arguments)
{
    uint16_t ip_type;
    if (CHECK_SDK_TYPE(OPERATION_VER))
    {
		xy_work_lock(0);
	}

    ip_type = packet_type_check(xyperf_arguments->remote_ip);
    if (ip_type != AF_INET && ip_type != AF_INET6)
    {
        goto out;
    }

    if (xyperf_arguments->protocol_type != 0)
    {
        //目前只支持udp灌包
        goto out;
    }

    if (ip_type == AF_INET)
        xyperf_udp_ipv4_client(xyperf_arguments->duration, xyperf_arguments->packet_size,
                               xyperf_arguments->rate, xyperf_arguments->remote_ip, xyperf_arguments->remote_port, xyperf_arguments->print_interval, xyperf_arguments->rai_val);
#if LWIP_IPV6
    else if (ip_type == AF_INET6)
        xyperf_udp_ipv6_client(xyperf_arguments->duration, xyperf_arguments->packet_size,
                               xyperf_arguments->rate, xyperf_arguments->remote_ip, xyperf_arguments->remote_port, xyperf_arguments->print_interval, xyperf_arguments->rai_val);
#endif

out:
    xy_free(xyperf_arguments);

    if (CHECK_SDK_TYPE(OPERATION_VER))
    {
		xy_work_unlock(0);
	}

	g_xyperf_TskHandle = NULL;
	osThreadExit();
    return;
}

int start_xyperf(char protocol_type, char *remote_ip, int remote_port, unsigned int duration, unsigned int packet_size, 
    unsigned int rate_in_kbps, unsigned int print_interval,int rai_val)
{
    xyperf_arguments_t *xyperf_arguments = NULL;
    xyperf_arguments = (xyperf_arguments_t *)xy_zalloc(sizeof(xyperf_arguments_t));

    if (g_xyperf_TskHandle != NULL)
    {
		softap_printf(USER_LOG, WARN_LOG, "warning!!!task have created!");
		return XY_ERR;
	}
	
	strcpy(xyperf_arguments->remote_ip, remote_ip);
    xyperf_arguments->remote_port = remote_port;
    xyperf_arguments->protocol_type = protocol_type;
    xyperf_arguments->duration = duration;
    xyperf_arguments->packet_size = packet_size;
    xyperf_arguments->rate = rate_in_kbps;
    xyperf_arguments->print_interval = print_interval;
	xyperf_arguments->rai_val = rai_val;

    g_xyperf_TskHandle = osThreadNew((osThreadFunc_t)(process_xyperf), xyperf_arguments, "perf_thd", 0x800, XY_OS_PRIO_NORMAL1);

    return XY_OK;
}

#endif //XY_PERF
