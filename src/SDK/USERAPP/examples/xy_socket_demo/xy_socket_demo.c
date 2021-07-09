/**
 * @file xy_standard_socket_demo.c
 * @brief 使用标准socket套接字客户端接口实现的demo，即同步方式的socket
 * @version 1.0
 * @date 2021-02-23
 * @copyright Copyright (c) 2021  芯翼信息科技有限公司
 */

#if DEMO_TEST

/*******************************************************************************
 *							 Include header files							   *
 ******************************************************************************/
#include "xy_api.h"

/*******************************************************************************
 *                             Macro definitions                               *
 ******************************************************************************/
#define SOCKET_DEMO_NUM_MAX 		2				//用户可自定义支持的socket数目
#define SOCKET_DEMO_REMOTE_PORT		10014  			//用户自行配置服务器端口号
#define SOCKET_DEMO_REMOTE_IPADDR 	"139.224.112.6"	//用户自行配置服务器地址
#define SOCKET_DEMO_UDP_TYPE 		0
#define SOCKET_DEMO_TCP_TYPE 		1

/*******************************************************************************
 *                             Type definitions                                *
 ******************************************************************************/
typedef struct socket_demo_ctx
{
	char sock_fd; //socket fd
	char quit;	  //socket数据接收线程退出标识
	int recv_len; //接收的数据总大小
} socket_demo_ctx_t;

/*******************************************************************************
 *                       Local variable declarations                           *
 ******************************************************************************/
/**
 * 接收线程收到下行包最后的数据，释放该信号量，通知socket主线程退出
 * 此信号量仅为demo线程退出演示使用，用户可以自行设定socket线程退出条件
 */
osSemaphoreId_t recv_lastdata_sem = NULL;
osThreadId_t socket_demo_thread = NULL;
osThreadId_t socket_demo_recv_thread = NULL;
socket_demo_ctx_t sock_context_demo[SOCKET_DEMO_NUM_MAX] = {0};

/*******************************************************************************
 *						Local function implementations						   *
 ******************************************************************************/
/**
 * @brief  创建一个socket, 并执行bind和connect操作
 * @param  remote_ip [IN]
 * @param  remote_port [IN]
 * @param  ipproto_type	[IN] 0 udp, 1 tcp @see SOCKET_DEMO_UDP_TYPE/SOCKET_DEMO_TCP_TYPE
 * @param  socket_fd [OUT]
 * @return 失败返回XY_ERR,成功返回XY_OK
 */
int create_conn_socket(char *remote_ip, int remote_port, int ipproto_type, int *socket_fd)
{
	int fd = -1;
	if(ipproto_type == SOCKET_DEMO_UDP_TYPE)
		fd = socket(AF_INET/*srv.family*/, SOCK_DGRAM, IPPROTO_UDP);
	else if(ipproto_type == SOCKET_DEMO_TCP_TYPE)
		fd = socket(AF_INET/*srv.family*/, SOCK_STREAM, IPPROTO_TCP);
	else
		xy_assert(0);
	
	if (fd == -1)
	{
		xy_printf("[socket_demo]socket create errno:%d\r\n", errno);
		return XY_ERR;
	}
		
	struct sockaddr_in bind_addr = {0};
	bind_addr.sin_family = AF_INET;
	bind_addr.sin_port = htons(0);
	if (bind(fd, (struct sockaddr *)&bind_addr, sizeof(bind_addr)) == -1) 
	{
		xy_printf("[socket_demo]bind errno:%d\r\n", errno);
		close(fd);
		return XY_ERR;
	}
	
	//设置接收超时时间
	struct timeval tv;
	tv.tv_sec = 5;
	tv.tv_usec = 0;
	setsockopt(fd, SOL_SOCKET, SO_RCVTIMEO, &tv, sizeof(struct timeval));

	//获取远端服务器地址
	char remote_port_s[7] = {0};
	struct addrinfo *result = NULL;
	struct addrinfo hint = {0};
	hint.ai_family = AF_INET;
	hint.ai_socktype = SOCK_DGRAM;
	
	snprintf(remote_port_s, sizeof(remote_port_s) - 1, "%d", remote_port);
	if (getaddrinfo(remote_ip, remote_port_s, &hint, &result) != 0) 
	{
		xy_printf("[socket_demo]getaddr errno:%d\r\n", errno);
		close(fd);
		return XY_ERR;
	}

	if (connect(fd, result->ai_addr, result->ai_addrlen) == -1) 
	{
	    freeaddrinfo(result);
		xy_printf("[socket_demo]connect errno:%d\r\n", errno);
		close(fd);
		return XY_ERR;
	}
	
	freeaddrinfo(result);
	*socket_fd = fd;
	return XY_OK;
}

int socket_demo_send_data(int socket_fd, char *data)
{
	int ret = XY_OK;

	if (send(socket_fd, data, 4, 0) <= 0)
	{
		xy_printf("[socket_demo]data send fail,errno:%d\r\n", errno);
		ret = XY_ERR;
	}
	else
	{
		xy_printf("[socket_demo]data send success");
	}

	send_debug_str_to_at_uart("SEND_END\r\n");
	return ret;
}

void socket_demo_recv_func()
{
	int ret,i;
	int max_fd;
	fd_set readfds, exceptfds;
	struct timeval tv;

    tv.tv_sec = SOCK_RECV_TIMEOUT;
    tv.tv_usec = 0;
	
	while(1) 
	{		
		FD_ZERO(&readfds); 
		FD_ZERO(&exceptfds);
		max_fd = -1;

		for (i = 0; i < SOCKET_DEMO_NUM_MAX; i++) 
		{
			if(sock_context_demo[i].quit == 1 && sock_context_demo[i].sock_fd >= 0)
			{
				close(sock_context_demo[i].sock_fd);
				xy_printf("[socket_demo]socket:%d closed", sock_context_demo[i].sock_fd);
				sock_context_demo[i].sock_fd = -1;	
				sock_context_demo[i].quit = 0;
				continue;
			}
			else if (sock_context_demo[i].sock_fd >= 0 ) 
			{
				if (max_fd < sock_context_demo[i].sock_fd)
				{
					max_fd = sock_context_demo[i].sock_fd;
				}
				FD_SET(sock_context_demo[i].sock_fd, &readfds);
				FD_SET(sock_context_demo[i].sock_fd, &exceptfds);
			}
		}

		if (max_fd < 0) 
		{
            goto DEMO_RECV_OUT;
		}

		ret = select(max_fd + 1, &readfds, NULL, &exceptfds, &tv);
		if (ret < 0)
		{ 
			if (errno == EINTR)
			{
				continue;
			}
            else if(errno == EBADF)
            {
                //one or more sockets may be closed asynchronously by AT+NSOCL
                xy_assert(0);
                continue;
            }
			else
            {
                continue;
            }
		}
        else if (ret == 0)
        {
            //we assume select timeouted here
            continue;
        }
		
		for (i = 0; i < SOCKET_DEMO_NUM_MAX; i++) 
		{
            if (sock_context_demo[i].sock_fd == -1)
                continue;

			if(sock_context_demo[i].sock_fd >= 0 && FD_ISSET(sock_context_demo[i].sock_fd,&exceptfds))
			{
				xy_printf("[socket_demo]except exist,force to close socket");
				close(sock_context_demo[i].sock_fd);
				sock_context_demo[i].sock_fd = -1;
				continue;
			}
			if(sock_context_demo[i].sock_fd >= 0 && !FD_ISSET(sock_context_demo[i].sock_fd,&readfds)) 
			{
				continue;
			}

			int len = 0;
			int read_len = 0;
			char *buf = NULL;

			ioctl(sock_context_demo[i].sock_fd, FIONREAD, &len);  //get the size of received data 
			xy_printf("[socket_demo]recv len:%d", len);
			
			if (len == 0) 
			{ 
				xy_printf("[socket_demo]rcv 0,force to close socket");
				close(sock_context_demo[i].sock_fd);
				sock_context_demo[i].sock_fd = -1;
				continue;
			}

			buf = xy_zalloc(len + 1);
			read_len = recv(sock_context_demo[i].sock_fd, buf, len, MSG_DONTWAIT);
			xy_printf("[socket_demo]recv read_len:%d", read_len);
			if (read_len < 0)
            {
                if (errno == EWOULDBLOCK) //recv time out
                {
                	xy_free(buf);
                    continue;
                }
                else
                {
					xy_free(buf);
					xy_printf("[socket_demo]read - bytes,force to close socket");
					close(sock_context_demo[i].sock_fd);
					sock_context_demo[i].sock_fd = -1;
                    continue;
                }
            }
			else if (read_len == 0) 
			{ 
				xy_free(buf);
				xy_printf("[socket_demo]read 0 bytes,force to close socket");
				close(sock_context_demo[i].sock_fd);
				sock_context_demo[i].sock_fd = -1;
				continue;
			}
			sock_context_demo[i].recv_len += read_len;
			xy_printf("[socket_demo]raw_socket recv:%d", sock_context_demo[i].recv_len);
			//用户需根据自身用户对接收的数据做处理，此处仅为demo演示使用
			if (*(buf + read_len - 4) == 0x01 && *(buf + read_len - 3) == 0x01 && *(buf + read_len - 2) == 0x01 && *(buf + read_len - 1) == 0x01)
			{
				xy_printf("[socket_demo]recv finish");
				sock_context_demo[i].recv_len = 0;
				send_debug_str_to_at_uart("RECV_END\r\n");
				osSemaphoreRelease(recv_lastdata_sem);
			}
			xy_free(buf);
		}
	}
DEMO_RECV_OUT:
	socket_demo_recv_thread = NULL;
	osThreadExit();	
}

void create_socket_demo_recv_thread()
{
	if (socket_demo_recv_thread == NULL)
	{
		socket_demo_recv_thread = osThreadNew(socket_demo_recv_func, NULL,
											  "at_raw_socket_demo", 0x800, XY_OS_PRIO_NORMAL);
	}
}

void socket_demo_reset_ctx(void)
{
	int i = 0;
	for (i = 0; i < SOCKET_DEMO_NUM_MAX; i++)
	{
		sock_context_demo[i].sock_fd = -1;
		sock_context_demo[i].quit = 0;
		sock_context_demo[i].recv_len = 0;
	}
}

void socket_demo_func(void)
{
	int i = 0;
	char remote_ip[16] = SOCKET_DEMO_REMOTE_IPADDR;
	int remote_port = SOCKET_DEMO_REMOTE_PORT;

	socket_demo_reset_ctx();

	//demo仅演示一路socket处理
	if (create_conn_socket(remote_ip, remote_port, SOCKET_DEMO_TCP_TYPE, (int *)&(sock_context_demo[1].sock_fd)) == XY_ERR)
	{
		for (i = 0; i < SOCKET_DEMO_NUM_MAX; i++)
		{
			if (sock_context_demo[i].sock_fd >= 0)
			{
				close(sock_context_demo[i].sock_fd);
				sock_context_demo[i].sock_fd = -1;
				sock_context_demo[i].quit = 0;
			}
		}
		xy_printf( "[socket_demo]creat one socket fail, exit demo");
		goto DEMO_OUT;
	}

/**	尚未创建接收线程，用户也可测试发送上行数据
	for (i = 0; i < SOCKET_DEMO_NUM_MAX; i++)
	{
		if (sock_context_demo[i].sock_fd >= 0)
		{
			char data_buf[4] = {0};
			*data_buf = 0x00;
			*(data_buf+1) = 0x00;
			*(data_buf+2) = 0x00;
			*(data_buf+3) = 0x00;
			socket_demo_send_data(sock_context_demo[i].sock_fd,data_buf);
		}
	}
*/

	//创建数据接收线程
	create_socket_demo_recv_thread();
	
	/** 等待接收线程释放信号量*/
	osSemaphoreAcquire(recv_lastdata_sem, osWaitForever);

	//测试发送上行数据
	for (i = 0; i < SOCKET_DEMO_NUM_MAX; i++)
	{
		if (sock_context_demo[i].sock_fd >= 0)
		{
			//测试数据
			char data_test[4] = {0};
			*data_test = 0x00;
			*(data_test + 1) = 0x00;
			*(data_test + 2) = 0x00;
			*(data_test + 3) = 0x00;
			socket_demo_send_data(sock_context_demo[i].sock_fd, data_test);
		}
	}

	//关闭socket及接收线程
	for (i = 0; i < SOCKET_DEMO_NUM_MAX; i++)
	{
		if (sock_context_demo[i].sock_fd >= 0)
			sock_context_demo[i].quit = 1;
		else
			continue;
		while (sock_context_demo[i].sock_fd >= 0)
			osDelay(500);
		sock_context_demo[i].quit = 0;
	}

DEMO_OUT:
	socket_demo_thread = NULL;
	osThreadExit();		
}

/**
 * 该DEMO供OPENCPU客户在芯翼核上进行开发参考使用。实现单个socket的数据收发参考，其中关键点为接收线程的退出，
 * 由于RTOS操作系统不支持资源的回收，需要quit标记通知接收线程自行退出。
 */
void xy_socket_demo_init()
{
	recv_lastdata_sem = osSemaphoreNew(1, 0);
	if(socket_demo_thread == NULL)
		socket_demo_thread = osThreadNew((osThreadFunc_t)socket_demo_func, NULL, "xy_socket_demo", 0x800, XY_OS_PRIO_NORMAL);
}

#endif //DEMO_TEST