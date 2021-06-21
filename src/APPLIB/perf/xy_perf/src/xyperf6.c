
#include "lwip/opt.h"
#if XY_PERF&&LWIP_IPV6

#include "xy_utils.h"
#if XY_PING
#include "net_utils.h"
#endif
#include "xyperf.h"
#include "ps_netif_api.h"
#include "lwip/sockets.h"
#include "lwip/ip.h"
#include "lwip/icmp.h"

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

        ret = sendto2(fd, &datagram, sizeof(struct xyperf_udp_datagram), 0, (struct sockaddr*)remote_sockaddr, sizeof(struct sockaddr_in6),0,rai_val);
        if (ret < 0)
        {
			softap_printf(USER_LOG, WARN_LOG, "xyperf6 Failed to send the packet (%d)\n", ret);
		}
    }
}

int xyperf_udp_ipv6_client(unsigned int duration, unsigned int packet_size, unsigned int rate_in_kbps, 
    char *remote_ip, int remote_port, unsigned int print_interval,int rai_val)
{
    uint32_t packet_duration;
    uint32_t duration_in_ms = duration * 1000;
	uint32_t delay = packet_duration;
	uint32_t nb_packets = 0;
	uint32_t start_time, last_print_time, end_time, loop_start_time, loop_end_time;
	int ret = 0;
    uint8_t *data = NULL;
    int fd = -1;
    struct xyperf_udp_datagram datagram;
    uint32_t totoal_ip_size = 0;
    char *rsp_cmd = NULL;
	struct sockaddr_in6 remote_sockaddr6 = {0};

    if (rate_in_kbps > 1024 * 1024)
    {
        //rate limitation, no more than 1Gbps
        return XY_ERR;
    }

    fd = socket(AF_INET6, SOCK_DGRAM, IPPROTO_UDP);
    if (fd < 0)
    {
        goto failed;
    }

    remote_sockaddr6.sin6_family = AF_INET6;
    remote_sockaddr6.sin6_port = htons(remote_port);
    inet_pton(AF_INET6, remote_ip, &remote_sockaddr6.sin6_addr);

    data = xy_zalloc(packet_size + sizeof(struct xyperf_udp_datagram));

    memset(data + sizeof(struct xyperf_udp_datagram), 'z', packet_size);

    //packet size + perf包头 + udp头部长度(8字节) + ipv6 ip报头长度(40字节)
    totoal_ip_size = packet_size + sizeof(struct xyperf_udp_datagram) + 8 + 40;

    delay = packet_duration = 1000 * totoal_ip_size * 8 / (rate_in_kbps * 1024); // ms

    rsp_cmd = (char *)xy_zalloc(128);
    snprintf(rsp_cmd, 128, "\r\n+XYPERF6: start xyperf6\r\n");
    send_rsp_str_to_ext(rsp_cmd);

    softap_printf(USER_LOG, WARN_LOG, "start xyperf6\n");
	snprintf(rsp_cmd, 128, "\r\n+XYPERF6: udp remote_ip:%s, remote_port:%d, data_len:%d, bandwidth:%dkbps, duration:%ds, print_interval:%d, xyperf_rai:%d\r\n",
		remote_ip, remote_port, packet_size, rate_in_kbps, duration, print_interval, rai_val);
	send_rsp_str_to_ext(rsp_cmd);
	softap_printf(USER_LOG, WARN_LOG, "\nxyperf ipv6 udp remote_ip:%s, remote_port:%d, data_len:%d, bandwidth:%dkbps, duration:%ds, print_interval:%d, xyperf_rai:%d\n", 
        remote_ip, remote_port, packet_size, rate_in_kbps, duration, print_interval, rai_val);

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

        ret = sendto2(fd, data, packet_size + sizeof(struct xyperf_udp_datagram), 0, (struct sockaddr*)&remote_sockaddr6, sizeof(struct sockaddr_in6),0,rai_val);
        if (ret < 0)
        {
			softap_printf(USER_LOG, WARN_LOG, "xyperf6 ERROR! Failed to send the packet (%d)\n", ret);
			goto failed;
		}
        else
        {
			nb_packets++;
		}

        /* Print log every second */
        if (loop_start_time - last_print_time > print_interval * 1000)
        {
            last_print_time = loop_start_time;
        }

        loop_end_time = osKernelGetTickCount();
        /* Wait */
        if (loop_end_time - loop_start_time < delay)
        {
            osDelay(delay - (loop_end_time -loop_start_time));
        }
    } while (loop_start_time - start_time < duration_in_ms);

    end_time = osKernelGetTickCount();
	xyperf_upload_fin(fd, nb_packets, end_time, packet_size, (struct sockaddr *)&remote_sockaddr6,rai_val);

    memset(rsp_cmd, 0, 128);
    snprintf(rsp_cmd, 128, "\r\n+XYPERF6: finished\r\n");
    send_rsp_str_to_ext(rsp_cmd);
    softap_printf(USER_LOG, WARN_LOG, "xyperf6 finished");

    if (fd != -1)
        close(fd);
    if (data != NULL)
        xy_free(data);
    if (rsp_cmd != NULL)
        xy_free(rsp_cmd);
    return XY_OK;

failed:
    if (fd != -1)
        close(fd);
    if (data != NULL)
        xy_free(data);
    if (rsp_cmd != NULL)
        xy_free(rsp_cmd);
    return XY_ERR;
}

#endif //XY_PERF&&LWIP_IPV6
