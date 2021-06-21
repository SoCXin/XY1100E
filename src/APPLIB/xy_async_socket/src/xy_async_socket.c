
#include "xy_async_socket.h"
#include "lwip/netdb.h"
#include "lwip/api.h"
#include "lwip/tcp.h"


static struct xy_socket_list_t *xy_socket_list[MAX_SOCKET];
osMutexId_t xy_socket_mutux;
static osMessageQueueId_t skt_thd_msg_q = NULL;
static osThreadId_t skt_thd_h = NULL;

static int find_empty_socket_idx()
{
	int idx = -1;
	int num = 0;
	LOCK_XY_SOCKET();
	for(;num < MAX_SOCKET;num++)
	{
		if(xy_socket_list[num] == NULL)
		{
			idx = num;
			break;
		}
	}
	UNLOCK_XY_SOCKET();
	return idx;
}

static int find_valid_socket_idx_by_fd(int fd)
{
	int idx = -1;
	int num = 0;
	LOCK_XY_SOCKET();
	for(;num < MAX_SOCKET;num++)
	{
		if(xy_socket_list[num] != NULL && xy_socket_list[num]->skt_fd == fd)
		{
			idx = num;
			break;
		}
	}
	UNLOCK_XY_SOCKET();
	return idx;
}

int xy_socket_create(int protocal_version, int socket_type)
{
	int protocol = IPPROTO_TCP;
    int type = SOCK_STREAM;
	int domain = AF_INET;
	int fd = -1;
    
    if (SOC_TYPE_TCP == socket_type) 
	{
        protocol = IPPROTO_TCP;
        type = SOCK_STREAM;
    } 
	else if (SOC_TYPE_UDP == socket_type) 
	{
        protocol = IPPROTO_UDP;
        type = SOCK_DGRAM;
    }
	else
		xy_assert(0);

	if(protocal_version == SOC_PROT_IPV4)
		domain = AF_INET;
	else if(protocal_version == SOC_PROT_IPV6)
		domain = AF_INET6;
	else
		xy_assert(0);

	fd = socket(domain, type, protocol);
    if (fd >= 0)
    {
    	int skt_idx = find_empty_socket_idx();
		if(skt_idx == -1)
		{
			close(fd);
			fd = -1;

			goto END;
		}
    	LOCK_XY_SOCKET();
		xy_socket_list[skt_idx] = xy_malloc(sizeof(struct xy_socket_list_t));
		memset(xy_socket_list[skt_idx],0,sizeof(struct xy_socket_list_t));
		xy_socket_list[skt_idx]->skt_fd = fd;
		xy_socket_list[skt_idx]->protocol = socket_type;
		xy_socket_list[skt_idx]->af_family = domain;
		xy_socket_list[skt_idx]->status = XY_SOCKET_CREATED;
		UNLOCK_XY_SOCKET();
		
        int NBIO = 1;
        lwip_ioctl(fd, FIONBIO, &NBIO);  //set nonblocking
    }
END:
	return (fd == -1) ? SOCK_ERROR : fd;
}

int xy_socket_event_register(int skt_fd, socket_event_callback_func pcallback)
{
	int skt_idx = -1;
	if((skt_idx = find_valid_socket_idx_by_fd(skt_fd)) == -1)
		return SOCK_ERROR;
	
	LOCK_XY_SOCKET();
	xy_socket_list[skt_idx]->event_callback = pcallback;
	UNLOCK_XY_SOCKET();
	return SOCK_OK;
}

int xy_socket_bind(int skt_fd, u16_t local_port)
{
	struct sockaddr_in bind_addr = {0};
	int skt_idx = -1;
	
	if((skt_idx = find_valid_socket_idx_by_fd(skt_fd)) == -1)
		return SOCK_ERROR;

	bind_addr.sin_port = htons(local_port);
	LOCK_XY_SOCKET();
	bind_addr.sin_family = xy_socket_list[skt_idx]->af_family;
	UNLOCK_XY_SOCKET();
	int ret = bind(skt_fd, (struct sockaddr *)&bind_addr, sizeof(bind_addr));
	return (ret == 0) ? SOCK_OK : SOCK_ERROR;
}

int xy_socket_connect(int skt_fd, char *remote_ip, u16_t remote_port)
{
	int skt_idx = -1;
	int ret = 0;
	struct sockaddr remote_sockaddr = {0};
	ip_addr_t addr;
	
	if((skt_idx = find_valid_socket_idx_by_fd(skt_fd)) == -1)
	{
		ret = -1;
		goto END;
	}

	if(ipaddr_aton(remote_ip, &addr) == 0)
	{
		ret = -1;
		goto END;
	}

	ip_addr_port_to_sockaddr(&addr, remote_port, &remote_sockaddr);
	ret = connect(skt_fd, &remote_sockaddr, remote_sockaddr.sa_len);
	LOCK_XY_SOCKET();
	if(ret != 0 && errno == EINPROGRESS)
	{
		ret = 0;
		xy_socket_list[skt_idx]->status = XY_SOCKET_CONNECTING;
	}
	else if(ret == 0)
		xy_socket_list[skt_idx]->status = XY_SOCKET_CONNECTED;
	UNLOCK_XY_SOCKET();
END:
	return (ret == 0) ? SOCK_OK : SOCK_ERROR;
}

int xy_socket_send(int skt_fd, void *data, int data_len)
{
	int skt_idx = -1;
	if((skt_idx = find_valid_socket_idx_by_fd(skt_fd)) == -1)
		return SOCK_ERROR;
	
	int ret = send(skt_fd, data, data_len, 0);
	if(ret > 0)
	{
		LOCK_XY_SOCKET();
		xy_socket_list[skt_idx]->send_size += ret;
		UNLOCK_XY_SOCKET();
	}
	return (ret == -1) ? SOCK_ERROR : ret;
}

int xy_socket_sendto(int skt_fd, void *data, int data_len, char *to_addr, u16_t to_port)
{
	int skt_idx = -1;
	if((skt_idx = find_valid_socket_idx_by_fd(skt_fd)) == -1)
		return SOCK_ERROR;
	
	struct sockaddr remote_sockaddr = {0};
	ip_addr_t addr;
	
	if(ipaddr_aton(to_addr, &addr) == 0)
		return SOCK_ERROR;

	ip_addr_port_to_sockaddr(&addr, to_port, &remote_sockaddr);

	int ret = sendto(skt_fd, data, data_len, 0, (struct sockaddr *)&remote_sockaddr, sizeof(struct sockaddr));
	if(ret > 0)
	{
		LOCK_XY_SOCKET();
		xy_socket_list[skt_idx]->send_size += ret;
		UNLOCK_XY_SOCKET();
	}
	return (ret == -1) ? SOCK_ERROR : ret;
}

int xy_socket_recv(int skt_fd, void* recv_buff, int recv_len)
{
	int skt_idx = -1;
	if((skt_idx = find_valid_socket_idx_by_fd(skt_fd)) == -1)
		return SOCK_ERROR;

	int ret = recv(skt_fd, recv_buff, recv_len, MSG_DONTWAIT);
	return (ret == -1) ? SOCK_ERROR : ret;
}

int xy_socket_recvfrom(int skt_fd, void *recv_buff, int recv_len, char *from_addr, u16_t *from_port)
{
	int skt_idx = -1;
	if((skt_idx = find_valid_socket_idx_by_fd(skt_fd)) == -1)
		return SOCK_ERROR;
	
	struct sockaddr remote_info = {0};
	socklen_t fromlen = sizeof(struct sockaddr);
	ip_addr_t addr;

	int ret = recvfrom(skt_fd, recv_buff, recv_len, MSG_DONTWAIT, &remote_info, &fromlen);
	if(ret > 0)
	{
		ip_addr_port_from_sockaddr(&remote_info, &addr, from_port);
		strcpy(from_addr,ipaddr_ntoa(&addr));
	}
	return (ret == -1) ? SOCK_ERROR : ret;
}

int xy_socket_close(int skt_fd)
{
	int skt_idx = -1;
	if((skt_idx = find_valid_socket_idx_by_fd(skt_fd)) == -1)
		return SOCK_ERROR;

    int ret = close(skt_fd);
    if (!ret)
    {
    	LOCK_XY_SOCKET();
		xy_socket_list[skt_idx]->status = XY_SOCKET_CLOSED;
		xy_free(xy_socket_list[skt_idx]);
		xy_socket_list[skt_idx] = NULL;
    	UNLOCK_XY_SOCKET();
    }
    return (ret == 0) ? SOCK_OK : SOCK_ERROR;
}

int xy_get_ip_by_domain_name(char *domain, char *ip_addr, int ip_len)
{
	struct hostent *p_hostent = NULL;
	
	p_hostent = gethostbyname(domain);
	if (p_hostent == NULL || ip_len < 16)
	{
		return SOCK_ERROR;
	}
	inet_ntop(p_hostent->h_addrtype, p_hostent->h_addr_list[0], ip_addr, ip_len);
	return SOCK_OK;
}

int xy_socket_get_acked_size(int skt_fd)
{
    int acked_size = -1;	
	int skt_idx = -1;
	if((skt_idx = find_valid_socket_idx_by_fd(skt_fd)) == -1)
		return SOCK_ERROR;
    
    LOCK_XY_SOCKET();
	acked_size = xy_socket_list[skt_idx]->acked_size;
	UNLOCK_XY_SOCKET();
    return acked_size;
}

int xy_socket_get_sent_size(int skt_fd)
{
    int send_size = -1;	
	int skt_idx = -1;
	if((skt_idx = find_valid_socket_idx_by_fd(skt_fd)) == -1)
		return SOCK_ERROR;
    
    LOCK_XY_SOCKET();
	send_size = xy_socket_list[skt_idx]->send_size;
	UNLOCK_XY_SOCKET();
    return send_size;
}

int xy_socket_get_status(int skt_fd)
{
    int status = -1;	
	int skt_idx = -1;
	if((skt_idx = find_valid_socket_idx_by_fd(skt_fd)) == -1)
		return XY_SOCKET_NONE;
    
    LOCK_XY_SOCKET();
	status = xy_socket_list[skt_idx]->status;
	UNLOCK_XY_SOCKET();
    return status;
}

int ip_addr_port_to_sockaddr(ip_addr_t         *src_addr, u16_t port, struct sockaddr *dst_sa)
{
	
	if (IP_IS_V6_VAL(*src_addr)) {
#if LWIP_IPV6
		struct sockaddr_in6 *sa6 = (struct sockaddr_in6 *)dst_sa;
		/* set up sockaddr */
		inet6_addr_from_ip6addr(&sa6->sin6_addr, ip_2_ip6(src_addr));
		sa6->sin6_port = lwip_htons(port);
		sa6->sin6_family = AF_INET6;
		sa6->sin6_len = sizeof(struct sockaddr_in6);
#endif
	  } else {
#if LWIP_IPV4
		struct sockaddr_in *sa4 = (struct sockaddr_in *)dst_sa;
		/* set up sockaddr */
		inet_addr_from_ip4addr(&sa4->sin_addr, ip_2_ip4(src_addr));
		sa4->sin_port = lwip_htons(port);
		sa4->sin_family = AF_INET;
		sa4->sin_len = sizeof(struct sockaddr_in);
#endif
	 }
	 return 0;
}

int ip_addr_port_from_sockaddr(struct sockaddr *src_sa, ip_addr_t          *dst_addr, u16_t *port)
{
	if ((src_sa->sa_family) == AF_INET6) {
#if LWIP_IPV6
	  inet6_addr_to_ip6addr(ip_2_ip6(dst_addr), &(((struct sockaddr_in6 *)src_sa)->sin6_addr));
	  IP_SET_TYPE_VAL(*dst_addr, IPADDR_TYPE_V6);
	  *port = lwip_ntohs((u16_t)(((struct sockaddr_in6 *)src_sa)->sin6_port));
#endif
    } else {
#if LWIP_IPV4
	  inet_addr_to_ip4addr(ip_2_ip4(dst_addr), &(((struct sockaddr_in *)src_sa)->sin_addr));
	  IP_SET_TYPE_VAL(*dst_addr, IPADDR_TYPE_V4);
	  *port = lwip_ntohs((u16_t)(((struct sockaddr_in *)src_sa)->sin_port));
#endif
    }
	return 0;
}

static void skt_proc_thread()
{
	struct skt_event_msg *msg = NULL;
	while(1) 
	{
		osMessageQueueGet(skt_thd_msg_q, (void *)(&msg), NULL, osWaitForever);
		if(msg == NULL)
			continue;
		switch(msg->msg_id)
		{
			case EVENT_ID_PROC:
			{
				int skt_idx = -1;
				struct sock_event_type *p_event = (struct sock_event_type *)(msg->data);
				if((skt_idx = find_valid_socket_idx_by_fd(p_event->skt_fd)) == -1)
					goto END;
				LOCK_XY_SOCKET();
				socket_event_callback_func cb_func = xy_socket_list[skt_idx]->event_callback;
				UNLOCK_XY_SOCKET();
				cb_func((void *)p_event);
			}
			break;
			default:
				break;
		}
END:
		xy_free(msg);
	}

}

static int send_msg_2_skt_thd(int msg_id, void *buff,int len)
{
	struct skt_event_msg *msg = NULL;
	
	msg = xy_malloc(sizeof(struct skt_event_msg) + len);
	msg->msg_id = msg_id;
	msg->size = len;

	if (buff != NULL)
		memcpy(msg->data, buff, len);

	LOCK_XY_SOCKET();
	if(skt_thd_msg_q == NULL)
		skt_thd_msg_q = osMessageQueueNew(2, sizeof(void *));
	if(skt_thd_h == NULL)
		skt_thd_h = osThreadNew((osThreadFunc_t)(skt_proc_thread), NULL, "skt_thd", 0x400, XY_OS_PRIO_NORMAL1);
	
	osMessageQueuePut(skt_thd_msg_q, &msg, 0, osWaitForever);
	UNLOCK_XY_SOCKET();
	return 0;
}

static int xy_post_event_to_app(int event_id, int skt_fd, int size, s8_t err)
{
	int skt_idx = -1;
	if((skt_idx = find_valid_socket_idx_by_fd(skt_fd)) == -1)
		return -1;

	LOCK_XY_SOCKET();
	socket_event_callback_func cb_func = xy_socket_list[skt_idx]->event_callback;
	UNLOCK_XY_SOCKET();
	if(cb_func != NULL)
	{
		struct sock_event_type event = {0};
		event.ev_id = event_id;
		event.skt_fd = skt_fd;
		event.data_len = size;
		event.err = err;
#if EVENT_PROC_SYNCH_OPERATION
		cb_func((void *)&event);
#else
		send_msg_2_skt_thd(EVENT_ID_PROC, &event, sizeof(struct sock_event_type));
#endif
	}
	return 0;
}

int xy_async_socket_api_event(void *arg, int event_id, void *pbuf, u16_t size, s8_t err)
{
	struct netconn *conn = (struct netconn *)arg;
	int skt_idx = -1;

	if(conn == NULL || (skt_idx = find_valid_socket_idx_by_fd(conn->socket)) == -1)
		return err;
	
	xy_printf("event_id:%d,socket id:%d, size:%d, err:%d",event_id,conn->socket,size,err);
    switch (event_id)
    {
    case LWIP_EVENT_SENT:
		LOCK_XY_SOCKET();
		xy_socket_list[skt_idx]->acked_size += size;
		UNLOCK_XY_SOCKET();
        xy_post_event_to_app(XY_SOCKET_SEND_RSP, conn->socket, size, ERR_OK);
        break;
    case LWIP_EVENT_RECV:
        if (pbuf == NULL)
        {
        	LOCK_XY_SOCKET();
            xy_socket_list[skt_idx]->status = XY_SOCKET_CLOSING;
			UNLOCK_XY_SOCKET();
            xy_post_event_to_app(XY_SOCKET_CLOSE_IND, conn->socket, 0, ERR_OK);
        }
        else
        {
        	int availsize = 0;
    		lwip_ioctl(conn->socket, FIONREAD, &availsize);
			xy_printf("recv availsize:%d",availsize);
			if(availsize > 0)
            	xy_post_event_to_app(XY_SOCKET_RECV_DATA_IND, conn->socket, availsize, ERR_OK);
        }
        break;
    case LWIP_EVENT_CONNECTED:
		LOCK_XY_SOCKET();
        xy_socket_list[skt_idx]->status = XY_SOCKET_CONNECTED;
		UNLOCK_XY_SOCKET();
        xy_post_event_to_app(XY_SOCKET_CONNECT_RSP, conn->socket, 0, ERR_OK);
        break;
    case LWIP_EVENT_ERR:
        xy_post_event_to_app(XY_SOCKET_ERR_IND, conn->socket, 0, err);
        break;
    default:
        break;
    }

    return err;
}

void xy_async_socket_resource_init()
{
	xy_socket_mutux = osMutexNew(osMutexPrioInherit);

}

