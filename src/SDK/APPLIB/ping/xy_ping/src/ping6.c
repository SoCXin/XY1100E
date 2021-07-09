#include "lwip/opt.h"
#if LWIP_IPV6 && XY_PING

#include "ping.h"
#include "at_ps_cmd.h"
#include "netif_mgmt.h"
#include "xy_utils.h"
#include "net_utils.h"
#include "xy_at_api.h"

#include "lwip/netdb.h"
#include "lwip/nd6.h"
#include "lwip/priv/nd6_priv.h"
#include "lwip/prot/nd6.h"
#include "lwip/prot/icmp6.h"
#include "lwip/pbuf.h"
#include "lwip/mem.h"
#include "lwip/memp.h"
#include "lwip/ip6.h"
#include "lwip/ip6_addr.h"
#include "lwip/inet_chksum.h"
#include "lwip/netif.h"
#include "lwip/icmp6.h"
#include "lwip/mld6.h"
#include "lwip/ip.h"
#include "lwip/stats.h"
#include "lwip/dns.h"
#include "lwip/inet.h"
#include "lwip/tcpip.h"

ping_info_t ping6_info;
uint16_t echo_id = 0;

struct ping6_netif_ip6addr
{
	struct in6_addr addr;
	int ret;
};

struct pseudo_header_icmpv6
{
	struct in6_addr 	src;
	struct in6_addr 	dst;
	uint16_t		nexth;
	uint16_t 		payload_len;
};
void netif_event_callback(struct netif* netif, netif_mgmt_event event)
{
	(void) netif;

	if (event == NETIF_MGMT_EVENT_IPV6)
    {
    	g_ping_stop = 0;
        softap_printf(USER_LOG, WARN_LOG, "ping6_netif_event_callback, netif up");
    }
    else if(event == NETIF_MGMT_EVENT_DOWN)
    {
    	g_ping_stop = 1;
        softap_printf(USER_LOG, WARN_LOG, "ping6_netif_event_callback, netif down");
    }
}

int get_default_ip6addr(void *m)
{
	struct ping6_netif_ip6addr * temp = (struct ping6_netif_ip6addr * )m;

	if(netif_get_default())
	{
		temp->ret=0;
		memcpy(&temp->addr,netif_ip6_addr(netif_get_default(),1),sizeof(struct in6_addr));
	}
	else
	{
		temp->ret = -1;
	}
	return 1;
}

int get_netif_ip6addr(struct ping6_netif_ip6addr * s_addr)
{
	osSemaphoreId_t ping6_tcpip_sem = NULL;
	ping6_tcpip_sem = osSemaphoreNew(0xFFFF, 0);
	tcpip_send_msg_wait_sem((tcpip_callback_fn)get_default_ip6addr, s_addr, (sys_sem_t*)(&ping6_tcpip_sem));
	osSemaphoreDelete(ping6_tcpip_sem);
	return 0;
}

void get_dns6_server()
{
	ip_addr_t pridns6 = {0};
	ip_addr_t secdns6 = {0};
	
	if(dns46_info[0].dns6.addr[0] == 0 && dns46_info[1].dns6.addr[0] == 0)
		return;
	memcpy(&pridns6.u_addr.ip6,&dns46_info[0].dns6,sizeof(ip6_addr_t));
	pridns6.type = IPADDR_TYPE_V6;
	memcpy(&secdns6.u_addr.ip6,&dns46_info[1].dns6,sizeof(ip6_addr_t));
	secdns6.type = IPADDR_TYPE_V6;
	
	if(pridns6.u_addr.ip6.addr[0] != 0)
		dns_setserver(0, &pridns6);
	if(secdns6.u_addr.ip6.addr[0] != 0)
		dns_setserver(1, &secdns6);
}

int result_dns = -1;
//host_ip, data_len, ping_num, time_out, interval_time
int _ping_ipv6(char *host_ip, u16_t data_len, u8_t timeout, unsigned int ping_num, int interval_time,int rai_val)
{
	int datalen=data_len;
	int ret=0;
	int sock = -1;
	struct sockaddr_in6 sa6 = {0};
	struct sockaddr_in6 peer_sa6 = {0};
	int peer_addrlen = sizeof(struct sockaddr_in6);
	int max_fd;
	fd_set readfds, exceptfds;
	struct timeval tv;
	uint8_t seq_no=1;
	struct icmp6_echo_hdr *echo_hdr=NULL;
	struct icmp6_echo_hdr *reply_hdr=NULL;
	struct pseudo_header_icmpv6 *pseudo_hdr=NULL;
	struct ping6_netif_ip6addr netif_ip6addr;
	int totallen=0;
	char*buff = NULL;
	char *data = NULL;
	char * recv_data = NULL;
	char *rsp_cmd = NULL;
	netif_mgmt_event_callback_t* ipv6_setup_event=NULL;
	netif_mgmt_event_callback_t* netif_down_event=NULL;
	uint32_t start_time, end_time, rtt, total_rtt = 0;
	struct addrinfo *result = NULL;
	struct addrinfo hint = {0};
	char ip_buff[40] = {0};

	memset(&ping6_info, 0, sizeof(ping_info_t));
	g_ping_stop = 0;

	ipv6_setup_event = xy_zalloc(sizeof(netif_mgmt_event_callback_t));
	netif_mgmt_init_event_callback(ipv6_setup_event, netif_event_callback, NETIF_MGMT_EVENT_IPV6);
	netif_mgmt_add_event_callback(ipv6_setup_event);
	
	netif_down_event = xy_zalloc(sizeof(netif_mgmt_event_callback_t));
	netif_mgmt_init_event_callback(netif_down_event, netif_event_callback, NETIF_MGMT_EVENT_DOWN);
	netif_mgmt_add_event_callback(netif_down_event);

	rsp_cmd = (char*)xy_zalloc(128);

	//inet_pton(AF_INET6,host_ip,&sa6.sin6_addr);
	get_dns6_server();
	hint.ai_family = AF_INET6;
	hint.ai_socktype = SOCK_DGRAM;
	hint.ai_flags = 0;
	if ((result_dns = getaddrinfo(host_ip, "10", &hint, &result)) != 0) {
    	snprintf(rsp_cmd, 128, "\r\nget ipv6 dst addr failed and end nping6\r\n");
    	send_rsp_str_to_ext(rsp_cmd);
		softap_printf(USER_LOG, WARN_LOG, "get ipv6 dst addr failed and result_dns:%d\n",result_dns);
		goto out;
	}
	memcpy(&sa6.sin6_addr, &(((struct sockaddr_in6 *)(result->ai_addr))->sin6_addr), sizeof(struct in6_addr));
	freeaddrinfo(result);
	softap_printf(USER_LOG, WARN_LOG, "get dst addrinfo success: %s\n", inet_ntop(AF_INET6,&sa6.sin6_addr,ip_buff,sizeof(ip_buff)));
	sock=socket(AF_INET6,SOCK_RAW,IPPROTO_ICMPV6);
	if(sock < 0)
		xy_assert(0);
	
	sa6.sin6_family = AF_INET6;
	sa6.sin6_len = sizeof(struct sockaddr_in6);
	sa6.sin6_port = lwip_htons((u16_t)0);

	//tv.tv_sec = SOCK_RECV_TIMEOUT;
	tv.tv_sec = timeout;
	tv.tv_usec = 0;

	totallen = datalen+sizeof(struct icmp6_echo_hdr)+sizeof(struct pseudo_header_icmpv6);
	buff = xy_zalloc(totallen);
	
	recv_data = xy_zalloc(IP6_HLEN+datalen+sizeof(struct icmp6_echo_hdr));

	pseudo_hdr=(struct pseudo_header_icmpv6 *)buff;
	pseudo_hdr->nexth=net_tool_htons((uint16_t)IP6_NEXTH_ICMP6);
	pseudo_hdr->payload_len=net_tool_htons(datalen+sizeof(struct icmp6_echo_hdr));
	memcpy(&pseudo_hdr->dst,&sa6.sin6_addr,sizeof(struct in6_addr));

	echo_id++;
	echo_hdr=(struct icmp6_echo_hdr*)(buff+sizeof(struct pseudo_header_icmpv6 ));
	echo_hdr->type=ICMP6_TYPE_EREQ;
	echo_hdr->code=0;
	echo_hdr->id=net_tool_htons(echo_id);

	data=buff+sizeof(struct pseudo_header_icmpv6 )+sizeof(struct icmp6_echo_hdr);
	

	while(1)
	{
		get_netif_ip6addr(&netif_ip6addr);  //get ipv6 addr through slaac

		if(-1 == netif_ip6addr.ret || 0 == netif_ip6addr.addr.un.u32_addr[0])
		{
 			softap_printf(USER_LOG, WARN_LOG, "get ipv6 src addr failed\n");
			memset(rsp_cmd,0,128);
    		snprintf(rsp_cmd, 128, "\r\nget ipv6 src addr failed and end nping6\r\n");
    		send_rsp_str_to_ext(rsp_cmd);
			goto out;
		}
		else
		{						
			memcpy(&pseudo_hdr->src,&netif_ip6addr.addr,sizeof(struct in6_addr));
			break;
		}
	}

	softap_printf(USER_LOG, WARN_LOG, "start send ping6\n");
	softap_printf(USER_LOG, WARN_LOG, "dst addr:%s, datalen:%d, timeout:%d, interval_time:%d", ip_buff, data_len, timeout, interval_time);

	memset(rsp_cmd,0,128);
    snprintf(rsp_cmd, 128, "\r\n+NPING6: start send ping6\r\n");
    send_rsp_str_to_ext(rsp_cmd);
	memset(rsp_cmd,0,128);
    snprintf(rsp_cmd, 128, "\r\nping dst addr:%s, ipv6 datalen:%d, timeout:%d, interval_time:%d\r\n", ip_buff, data_len, timeout, interval_time);
    send_rsp_str_to_ext(rsp_cmd);

	
	while(ping_num-- && !g_ping_stop)
	{
		echo_hdr->seqno=net_tool_htons((uint16_t)seq_no);
		echo_hdr->chksum=0;
		memset(data,seq_no,datalen);
		//checksum = net_tool_htons(net_tool_standard_chksum(buff, totallen));

		start_time = osKernelGetTickCount();
		ret = sendto2(sock,echo_hdr,datalen+sizeof(struct icmp6_echo_hdr),0,(const struct sockaddr *)&sa6,sizeof(sa6),0,rai_val);
		if(ret <= 0)
		{
			softap_printf(USER_LOG, WARN_LOG, "\r\nsendto failed and again!!!\r\n");
			memset(rsp_cmd,0,128);
			snprintf(rsp_cmd, 128, "\r\nsendto failed and again!!!\r\n");
			send_rsp_str_to_ext(rsp_cmd);
			osDelay(500);
			continue;
		}
		softap_printf(USER_LOG, WARN_LOG, "seq_no:%d",seq_no);
		seq_no++;
		ping6_info.ping_send_num++;

		max_fd=sock;
		FD_ZERO(&readfds); 
		FD_ZERO(&exceptfds);
		FD_SET(sock, &readfds);
		FD_SET(sock, &exceptfds);
		
		ret = select(max_fd+1,&readfds,NULL,&exceptfds,&tv);

		if(ret<0)
		{
			xy_assert(0);
		}
		else if(0 == ret)
		{
			softap_printf(USER_LOG, WARN_LOG, "%d select timeout\n",seq_no);
			memset(rsp_cmd,0,128);
			snprintf(rsp_cmd, 128, "\r\nPing6 timeout\r\n");
			send_rsp_str_to_ext(rsp_cmd);
		}
		else
		{
			if(FD_ISSET(sock,&exceptfds))
			{
				xy_assert(0);
			}
			if(FD_ISSET(sock,&readfds))
			{
				ret = recvfrom(sock,recv_data,IP6_HLEN+datalen+sizeof(struct icmp6_echo_hdr),0,(struct sockaddr *)&peer_sa6,(socklen_t *)&peer_addrlen);
				softap_printf(USER_LOG, WARN_LOG, "ret=%d, port=%d\n",ret,net_tool_htons(peer_sa6.sin6_port));
				if(ret>0 && IPPROTO_ICMPV6 == net_tool_htons(peer_sa6.sin6_port))
				{
					struct ip6_hdr *ip6_addr_header = (struct ip6_hdr *)recv_data;
					
					reply_hdr=(struct icmp6_echo_hdr *)(recv_data+IP6_HLEN);
					softap_printf(USER_LOG, WARN_LOG, "reply_hdr->type=%d\n", reply_hdr->type);
					if(ICMP6_TYPE_EREP == reply_hdr->type)
					{
						ping6_info.ping_reply_num++;
						softap_printf(USER_LOG, WARN_LOG, "reply_hdr->id:%d",reply_hdr->id);
						end_time = osKernelGetTickCount();
	                    rtt = end_time - start_time;
	                    if (ping6_info.longest_rtt == 0)
	                    {
	                        ping6_info.longest_rtt = rtt;
	                    }
	                    else if ((uint32_t)(ping6_info.longest_rtt) < rtt)
	                    {
	                        ping6_info.longest_rtt = rtt;
	                    }

	                    if (ping6_info.shortest_rtt == 0)
	                    {
	                        ping6_info.shortest_rtt = rtt;
	                    }
	                    else if ((uint32_t)(ping6_info.shortest_rtt) > rtt)
	                    {
	                        ping6_info.shortest_rtt = rtt;
	                    }
	                    total_rtt += rtt;
	                    ping6_info.time_average = total_rtt / ping6_info.ping_reply_num;
	                    softap_printf(USER_LOG, WARN_LOG, "reply from %s, %d bytes %d ms\n", ip_buff, data_len, rtt);
						memset(rsp_cmd,0,128);
	                    snprintf(rsp_cmd, 128, "\r\nreply from %s, %d bytes %d ms\r\n", ip_buff, data_len, rtt);
	                    send_rsp_str_to_ext(rsp_cmd);
						
						softap_printf(USER_LOG, WARN_LOG, "reply from %s, %d bytes, %d ms, TTL=%d\n", host_ip, data_len, rtt, ip6_addr_header->_hoplim);
						if (end_time -start_time < (uint32_t)(interval_time * 1000))
	        			{
	            			osDelay(interval_time * 1000 - (end_time -start_time));
	       				}
						continue;
					}
				}
				
				softap_printf(USER_LOG, WARN_LOG, "recv failed and timeout!!!\n");
				end_time = osKernelGetTickCount();
				if (end_time -start_time < timeout * 1000)
    			{
        			osDelay(timeout * 1000 - (end_time -start_time));
   				}
				memset(rsp_cmd,0,128);
				snprintf(rsp_cmd, 128, "\r\nPing6 timeout\r\n");
				send_rsp_str_to_ext(rsp_cmd);
			}
		}
	}

	memset(rsp_cmd,0,128);
	softap_printf(USER_LOG, WARN_LOG, "ping_send_num:%d,ping_reply_num:%d,longest_rtt:%d,shortest_rtt:%d,time_average:%d", 
        ping6_info.ping_send_num, ping6_info.ping_reply_num, ping6_info.longest_rtt, ping6_info.shortest_rtt, ping6_info.time_average);
    snprintf(rsp_cmd, 128, "\r\nstatistics: ping num:%d, reply:%d, longest_rtt:%dms, shortest_rtt:%dms, average_time:%dms\r\n", 
        ping6_info.ping_send_num, ping6_info.ping_reply_num, ping6_info.longest_rtt, ping6_info.shortest_rtt, ping6_info.time_average);
    send_rsp_str_to_ext(rsp_cmd);
out:
	if (sock != -1)
        close(sock);
	if(netif_down_event != NULL)
	{
		netif_mgmt_del_event_callback(netif_down_event);
        xy_free(netif_down_event);
	}
	if(ipv6_setup_event != NULL)
	{
		netif_mgmt_del_event_callback(ipv6_setup_event);
        xy_free(ipv6_setup_event);
	}
	if(buff != NULL)
		xy_free(buff);
	if(recv_data != NULL)
		xy_free(recv_data);
	if(rsp_cmd != NULL)
		xy_free(rsp_cmd);

	return ret;
}
#endif

