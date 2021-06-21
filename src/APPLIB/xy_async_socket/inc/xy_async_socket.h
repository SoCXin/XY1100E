#ifndef _XY_ASYNC_SOCKET_API_H
#define _XY_ASYNC_SOCKET_API_H

#include "osal.h"
#include "lwip/sockets.h"
/*******************************************************************************
 *                           Include header files                              *
 ******************************************************************************/
extern osMutexId_t xy_socket_mutux;

/*******************************************************************************
 *                             Macro definitions                               *
 ******************************************************************************/
#define MAX_SOCKET MEMP_NUM_NETCONN
#define LOCK_XY_SOCKET()     osMutexAcquire(xy_socket_mutux, osWaitForever)
#define UNLOCK_XY_SOCKET()   osMutexRelease(xy_socket_mutux)

#define EVENT_PROC_SYNCH_OPERATION 0

#define SOCK_OK     0
#define SOCK_ERROR  -1
/*******************************************************************************
 *                       Global function declarations                          *
 ******************************************************************************/

/*******************************************************************************
 *                      Inline function implementations                        *
 ******************************************************************************/
enum sock_type {
	SOC_TYPE_TCP = 0,
	SOC_TYPE_UDP,
};

enum protocol_ver {
	SOC_PROT_IPV4 = 0,
	SOC_PROT_IPV6,
};

enum socket_status_t
{
    XY_SOCKET_NONE = 0,
	XY_SOCKET_CREATED,
    XY_SOCKET_CONNECTING,
    XY_SOCKET_CONNECTED, //ebtablish
    XY_SOCKET_CLOSING,
    XY_SOCKET_CLOSED,
};


struct xy_socket_list_t
{
    int skt_fd;
    int status;
	int protocol; //IPPROTO_UDP,IPPROTO_TCP
	int af_family; //AF_INET,AF_INET6
    void *event_callback;
	int send_size;
	int acked_size;
};

enum socket_event_id {
	XY_SOCKET_CONNECT_RSP = 0,//TCP connect回复
	XY_SOCKET_SEND_RSP, //TCP send 结果回复
	XY_SOCKET_RECV_DATA_IND,//TCP  UDP下行上报
	XY_SOCKET_CLOSE_IND, //TCP 被动close事件上报
	XY_SOCKET_ERR_IND, //TCP 异常事件上报
};

enum skt_thd_msg_id
{
	EVENT_ID_PROC = 0,
};

struct sock_event_type {
	int ev_id;
	int skt_fd;
	int err;
	int data_len;
};

struct skt_event_msg {
	int 	msg_id;
	int 	size;
	char	data[0];
};

/**
 * @brief 			This is funtion user creat socket
 * @param protocal_version   枚举类型：SOC_PROT_IPV4；SOC_PROT_IPV6
 * @param socket_type   枚举类型：SOC_TYPE_TCP；SOC_TYPE_UDP
 * @return 			失败：SOCK_ERROR 成功：socket_fd
 * @warning         注意入参,否则断言死机
 */
int xy_socket_create(int protocal_version, int socket_type);

/**
 * @brief 			This is funtion user bind socket port
 * @param skt_fd    socket fd
 * @param local_port   本地端口号，两个字节无符号类型，填0将使用随机端口号绑定
 * @return 			失败：SOCK_ERROR 成功：SOCK_OK
 * @warning         注意入参,否则bind失败和端口号越界（超过65535）
 */
int xy_socket_bind(int skt_fd, u16_t local_port);

/**
 * @brief 			This is funtion user connect socket
 * @param skt_fd    socket fd
 * @param remote_ip  服务器ip，点分十进制
 * @param remote_ip  服务器端口
 * @return 			失败：SOCK_ERROR 成功：socket_fd
 * @warning         注意入参,IP点分十进制，服务器端口两个字节无符号
 */
int xy_socket_connect(int skt_fd, char *remote_ip, u16_t remote_port);

/**
 * @brief 			This is funtion user send socket data
 * @param skt_fd    socket fd
 * @param data      将发送到数据data_len
 * @param data_len  将发送到数据长度
 * @return 			失败：SOCK_ERROR 成功：发送数据的长度
 * @warning         注意入参
 */
int xy_socket_send(int skt_fd, void *data, int data_len);

/**
 * @brief 			这个函数用于UDP socket指定服务器发送数据
 * @param skt_fd    socket fd
 * @param data      将发送到数据data_len
 * @param data_len  将发送到数据长度
 * @param to_addr   点分十进制服务器ip，或IPV6以冒号隔开的地址
 * @param to_port   服务器端口号
 * @return 			失败：SOCK_ERROR 成功：发送数据的长度
 * @warning         注意入参
 */
int xy_socket_sendto(int skt_fd, void *data, int data_len, char *to_addr, u16_t to_port);

/**
 * @brief 			这个函数用于socket接收服务器下行数据
 * @param skt_fd    socket fd
 * @param recv_buff 接收数据的buff，建议以mallc形式申请内存
 * @param recv_len  需要接收的长度
 * @return 			失败：SOCK_ERROR 成功：实际读取的数据长度
 * @warning         
 */
int xy_socket_recv(int skt_fd, void* recv_buff, int recv_len);

/**
 * @brief 			这个函数用于udp socket接收服务器下行数据
 * @param skt_fd    socket fd
 * @param recv_buff 接收数据的buff，建议以mallc形式申请内存
 * @param recv_len  需要接收的长度
 * @param from_addr 存放服务器的点分十进制ip，或IPV6以冒号隔开
 * @param from_port int指针，存放服务器的端口号
 * @return 			失败：SOCK_ERROR 成功：实际读取的数据长度
 * @warning         注意入参
 */
int xy_socket_recvfrom(int skt_fd, void *recv_buff, int recv_len, char *from_addr, u16_t *from_port);

/**
 * @brief 			这个函数用于关闭socket
 * @param skt_fd    socket fd
 * @return 			失败：SOCK_ERROR 成功：SOCK_OK
 * @warning         注意入参的正确性
 */
int xy_socket_close(int skt_fd);

/**
 * @brief 			这个用于socket回调函数类型，函数回调实现参照 static int app_socket_client_callback(void *pevent);
 * @param event_param    指针，实则为结构体首地址，(struct sock_event_type *)
 * @return 			0
 * @warning         注意入参的正确性
 */
typedef int (*socket_event_callback_func)(void *event_param);  

/**
 * @brief 			这个用于根据域名获取点分十进制IP
 * @param domain    域名,such as "www.baidu.com"
 * @param ip_addr   存放点分ip_buff,such as "10.0.10.180"
 * @param ip_len    IP的实际长度
 * @return 			失败：SOCK_ERROR 成功：SOCK_OK
 * @warning         注意入参的正确性
 */
int xy_get_ip_by_domain_name(char *domain, char *ip_addr, int ip_len);

/**
 * @brief 			获取ACK值
 * @param skt_fd    socket fd
 * @return 			失败：SOCK_ERROR 成功：acked_size
 * @warning         注意入参的正确性
 */
int xy_socket_get_acked_size(int skt_fd);

/**
 * @brief 			获取发送数据的总值
 * @param skt_fd    socket fd
 * @return 			失败：SOCK_ERROR 成功：发送数据的总长度
 * @warning         注意入参的正确性
 */
int xy_socket_get_sent_size(int skt_fd);

/**
 * @brief 			获取当前socket的状态
 * @param skt_fd    socket fd
 * @return 			失败：XY_SOCKET_NONE 成功：当前socket的状态
 * @warning         注意入参的正确性
 */
int xy_socket_get_status(int skt_fd);

/**
 * @brief 			注册socket事件回调函数
 * @param skt_fd    socket fd
 * @param pcallback socket 回调函数
 * @return 			失败：SOCK_ERROR 成功：SOCK_OK
 * @warning         注意入参的正确性
 */
int xy_socket_event_register(int skt_fd, socket_event_callback_func pcallback);

int ip_addr_port_to_sockaddr(ip_addr_t *src_addr, u16_t port, struct sockaddr *dst_sa);

int ip_addr_port_from_sockaddr(struct sockaddr *src_sa, ip_addr_t *dst_addr, u16_t *port);

void xy_async_socket_resource_init();

int xy_async_socket_api_event(void *arg, int event_id, void *pbuf, u16_t size, s8_t err);

#endif
