#pragma once

#include "xy_utils.h"

/*******************************************************************************
 *                             Type definitions                                *
 ******************************************************************************/
struct up_data_list
{
	void *next;
	int len;
	char *data;
};

typedef struct passthr_socket_ctx
{
	int sock_fd;					//lwip 返回的fd
	void *hook_cache;				//退出透传模式后，但没有真正关闭socket链接，此时需要保存数据态的处理接口，在ATO切换成数据态后，重新进入透传模式
	unsigned char quit;				//socket数据接收线程退出标识
	unsigned char net_type;			//1:UDP,DGRAM; 0:TCP,STREAM
	unsigned char is_deact;         //pdp去激活标识
	unsigned short remote_port;		//远端端口号
	char *remote_ip;				//远端IP,以字符串表示
	struct up_data_list *data_list; //以链表形式存储上行需要发送的数据包，由接收发线程取出发送
} passthr_socket_ctx_t;

/*******************************************************************************
 *                       Global variable declarations                          *
 ******************************************************************************/
extern passthr_socket_ctx_t *g_socket_passthr_ctx;

/*******************************************************************************
 *                       Global function declarations                          *
 ******************************************************************************/
int at_ATD_req(char *at_buf, char **prsp_cmd);
int at_TRANSPARENTDEMO_req(char *at_buf, char **prsp_cmd);
#if AT_SOCKET
int at_ATO_req(char *at_buf, char **prsp_cmd);
int at_XTRANSSOCKCLOSE_req(char *at_buf, char **prsp_cmd);
int at_XTRANSSOCKOPEN_req(char *at_buf, char **prsp_cmd);
#endif //AT_SOCKET