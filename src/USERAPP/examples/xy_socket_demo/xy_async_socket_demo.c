#if DEMO_TEST && ASYNC_SKT_ENABLE
#include "xy_async_socket.h"

static osMessageQueueId_t app_skt_client_thd_msg_q = NULL;
static osThreadId_t app_skt_client_thd_h = NULL;

enum app_tcpip_msg_id
{
	APP_TCPIP_SOCKET_CREATE = 0,
	APP_TCPIP_SOCKET_CONNECT,
	APP_TCPIP_SOCKET_SEND,
	APP_TCPIP_SOCKET_RECV,
	APP_TCPIP_SOCKET_CLOSE,
};

struct event_msg {
	int 	msg_id;
	int 	size;
	char	data[0];
};


static int send_msg_to_client_socket_thd(int msg_id, char *buff, int len)
{
	struct event_msg *msg = NULL;
	msg = xy_malloc(sizeof(struct event_msg) + len);
	msg->msg_id = msg_id;
	msg->size = len;

	if (buff != NULL)
		memcpy(msg->data, buff, len);
	osMessageQueuePut(app_skt_client_thd_msg_q, &msg, 0, osWaitForever);
	return 0;
}

static int app_socket_client_callback(void *pevent)
{
	struct sock_event_type *event = (struct sock_event_type *)pevent;
	int skt_fd = event->skt_fd;
	switch(event->ev_id)
	{
		case XY_SOCKET_CONNECT_RSP:
			xy_printf("connect success!!");
			int num = 0;
			for(;num<3;num++)
				send_msg_to_client_socket_thd(APP_TCPIP_SOCKET_SEND, (char *)&skt_fd, sizeof(int));
		break;
		case XY_SOCKET_SEND_RSP:
			xy_printf("send success and acked size:%d, sent size:%d",xy_socket_get_acked_size(skt_fd),xy_socket_get_sent_size(skt_fd));
		break;
		case XY_SOCKET_RECV_DATA_IND:
		{
			char *recv_data = xy_zalloc(event->data_len+1);
			xy_socket_recv(skt_fd,recv_data,event->data_len);
			xy_printf("recv downlink data:%s", recv_data);
			xy_free(recv_data);
			send_msg_to_client_socket_thd(APP_TCPIP_SOCKET_CLOSE, (char *)&skt_fd, sizeof(int));
		}
		break;
		case XY_SOCKET_CLOSE_IND:
		case XY_SOCKET_ERR_IND:
			send_msg_to_client_socket_thd(APP_TCPIP_SOCKET_CLOSE, (char *)&skt_fd, sizeof(int));
		break;
		default:
		break;
	}
	return 0;
}

static int socket_create_req_proc()
{
	int fd = xy_socket_create(SOC_PROT_IPV4, SOC_TYPE_TCP);
	if(fd == SOCK_ERROR)
	{
		xy_printf("socket create failed!!");
		return 0;
	}
	if(xy_socket_bind(fd, 0) == SOCK_ERROR)
	{
		xy_printf("socket bind failed!!");
		xy_socket_close(fd);
		return 0;
	}
	xy_printf("socket fd:%d",fd);
	if(fd >= 0)
	{
		xy_socket_event_register(fd,app_socket_client_callback);
		send_msg_to_client_socket_thd(APP_TCPIP_SOCKET_CONNECT, (char *)&fd, sizeof(int));
	}
	return 0;
}

static int socket_connect_req_proc(int skt_id)
{
	char remote_ip[16] = "139.224.131.190";
	u16_t remote_port = 10001;
	
	xy_printf("socket begin connect fd:%d",skt_id);
	int ret = xy_socket_connect(skt_id,remote_ip,remote_port);
	if(ret == SOCK_ERROR)
		xy_socket_close(skt_id);
	
	xy_printf("socket end connect");
	return 0;
}

static int socket_send_req_proc(int skt_id)
{
	char data[100] = "Hello World";
	if(xy_socket_send(skt_id,data,strlen(data)) == SOCK_ERROR)
		xy_socket_close(skt_id);
	return 0;
}

static void client_socket_proc_thread()
{
	struct event_msg *msg = NULL;
	while(1) 
	{
		osMessageQueueGet(app_skt_client_thd_msg_q, (void *)(&msg), NULL, osWaitForever);
		if(msg == NULL)
			continue;
		switch(msg->msg_id)
		{
			case APP_TCPIP_SOCKET_CREATE:
				socket_create_req_proc();
			break;
			case APP_TCPIP_SOCKET_CONNECT:
				socket_connect_req_proc(*(int *)(msg->data));
			break;
			case APP_TCPIP_SOCKET_SEND:
				socket_send_req_proc(*(int *)(msg->data));
			break;
			case APP_TCPIP_SOCKET_CLOSE:
				xy_socket_close(*(int *)(msg->data));
			break;
			default:
			break;
		}

		xy_free(msg);
	}

}


void app_tcpip_client_init()
{
	if(app_skt_client_thd_msg_q == NULL)
		app_skt_client_thd_msg_q = osMessageQueueNew(10, sizeof(void *));
	if(app_skt_client_thd_h == NULL)
		app_skt_client_thd_h = osThreadNew((osThreadFunc_t)(client_socket_proc_thread), NULL, "app_skt_client_thd", 0x800, XY_OS_PRIO_NORMAL1);
	send_msg_to_client_socket_thd(APP_TCPIP_SOCKET_CREATE, NULL, 0);
}
#endif //DEMO_TEST && ASYNC_SKT_ENABLE

