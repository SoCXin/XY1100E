#ifndef SOCKET_PROXY_COM_H
#define SOCKET_PROXY_COM_H

#include "lwip/sockets.h"


#define MAX_SOCKDATA_LEN      500//150//512//5400

#define MAX_OPT_VAL_LEN  20//512

#define SOCK_ADDRLEN	 28

#define PROXY_PROTO_BASE	0x00
#define	PROXY_PROTO_UNKOWN	PROXY_PROTO_BASE
#define PROXY_PROTO_UDP		0x01
#define	PROXY_PROTO_TCP		0x02
#define PROXY_PROTO_TCP_UDP	0x03
#define PROXY_PROTO_MAX		PROXY_PROTO_TCP_UDP

//for compile ok
#define IPV6ADDLEN_MAX 28


enum proxy_type {
	PROXY_SOCK,
	PROXY_CLOSE,
	PROXY_SEND,
	PROXY_SEND_TO,
	PROXY_RECV,
	PROXY_RECV_FROM,
	PROXY_SELECT,
	PROXY_CONNECT,
	PROXY_BIND,
	PROXY_SETOPT,
	PROXY_GETOPT,
	PROXY_GETSOCKNAME,
	PROXY_GETPEERNAME,
	PROXY_IOCTRL,
	PROXY_IOCTRLSOCK,
	PROXY_LISTEN,
	PROXY_ACCEPT,
	PROXY_SHUTDOWN,
};

typedef struct proxy_request_set_opt_msg {
	int sock;
	int level;
	int option_name;
	int option_len;
	char option_val[MAX_OPT_VAL_LEN];
} proxy_request_set_opt_msg;

typedef struct proxy_response_set_opt_msg {
	int ret;
	int err;
} proxy_response_set_opt_msg;

typedef struct proxy_request_get_sock_name_msg {
	int sock;
} proxy_request_get_sock_name_msg;

typedef struct proxy_request_ioctrl_msg {
	int sock;
	long cmd;
	char argp[MAX_OPT_VAL_LEN];
} proxy_request_ioctrl_msg;

typedef struct proxy_response_ioctrl_sock_msg {
	int ret;
	int err;
} proxy_response_ioctrl_sock_msg;


typedef struct proxy_request_socket_msg {
	int domain;
	int type;
	int protocol;
} proxy_request_socket_msg;

typedef struct proxy_request_close_msg {
	int sock;
} proxy_request_close_msg;

typedef struct proxy_request_send_msg {
	int sock;
	int size;
	int flags;
	char data[0];
} proxy_request_send_msg;

typedef struct proxy_request_select_msg {
	int maxfdp1;
	char para_bitmap;//for each bit,0 means NULL,1 means not NULL. bit0:readset bit1:writeset bit2:exceptset bit3:timeout 
	fd_set readset;
	fd_set writeset;
	fd_set exceptset;
	struct lwip_timeval timeout;
} proxy_request_select_msg;

typedef struct proxy_request_recv_msg {
	int sock;
	int size;
	int flags;
	//char data[0];
} proxy_request_recv_msg;

typedef struct proxy_request_recv_from_msg {
	int sock;
	int size;
	int flags;
	int sock_len;
	//char data[MAX_SIP_LEN];
} proxy_request_recv_from_msg;

typedef struct proxy_request_send_to_msg {
	int sock;
	int size;
	int flags;
	int socketlen;
	char  peer_addr[SOCK_ADDRLEN];
	char data[0];
} proxy_request_send_to_msg;

typedef struct proxy_request_bind_msg {
	int sock;
	int socklen;
	char cid;
	char gw[IPV6ADDLEN_MAX];
	char  local_addr[SOCK_ADDRLEN];
} proxy_request_bind_msg;

typedef struct proxy_request_connect_msg {
	int sock;
	int socket_len;
	char  peer_addr[SOCK_ADDRLEN];
} proxy_request_connect_msg;

typedef struct proxy_request_get_opt_msg {
	int sock;
	int level;
	int option_name;
	int option_len;
} proxy_request_get_opt_msg;

typedef struct proxy_response_get_opt_msg {
	int ret;
	int err;
	int option_len;
	char option_value[MAX_OPT_VAL_LEN];
} proxy_response_get_opt_msg;

typedef struct proxy_response_get_sock_name_msg {
	int ret;
	int err;
	int name_len;
	char name[SOCK_ADDRLEN];
} proxy_response_get_sock_name_msg;

typedef struct proxy_request_get_peer_name_msg {
	int sock;
} proxy_request_get_peer_name_msg;

typedef struct proxy_response_get_peer_name_msg {
	int ret;
	int err;
	int name_len;
	char name[SOCK_ADDRLEN];
} proxy_response_get_peer_name_msg;

typedef struct proxy_response_ioctrl_msg {
	int ret;
	int err;
	char argp[MAX_OPT_VAL_LEN];
} proxy_response_ioctrl_msg;

typedef struct proxy_request_ioctrl_sock_msg {
	int sock;
	long cmd;
	char argp[MAX_OPT_VAL_LEN];
} proxy_request_ioctrl_sock_msg;


typedef struct proxy_response_sock_msg {
	int err;
	int ret;
} proxy_response_sock_msg;

typedef struct proxy_response_close_msg {
	int err;
	int ret;
} proxy_response_close_msg;

typedef struct proxy_response_select_msg {
	int err;
	int ret;
	fd_set readset;
	fd_set writeset;
	fd_set exceptset;
} proxy_response_select_msg;

typedef struct proxy_response_send_msg {
	int ret;
	int err;
} proxy_response_send_msg;

typedef struct proxy_response_recv_msg {
	int err;
	int ret;
	char data[0];
} proxy_response_recv_msg;

typedef struct proxy_response_connect_msg {
	int err;
	int ret;
} proxy_response_connect_msg;

typedef struct proxy_response_bind_msg {
	int err;
	int ret;
} proxy_response_bind_msg;

typedef struct proxy_response_recv_from_msg {
	int err;
	int ret;
	int socklen;
	int len;
	char addr[SOCK_ADDRLEN];
	char data[0];
} proxy_response_recv_from_msg;

typedef struct proxy_response_send_to_msg {
	int ret;
	int err;
} proxy_response_send_to_msg;

typedef struct proxy_request_listen_msg {
	int sock;
	int backlog;
} proxy_request_listen_msg;

typedef struct proxy_response_listen_msg {
	int ret;
	int err;
} proxy_response_listen_msg;

typedef struct proxy_response_accept_msg {
	int err;
	int ret;
	char addr[SOCK_ADDRLEN];
	int socklen;
} proxy_response_accept_msg;

typedef struct proxy_request_accept_msg {
	int sock;
	int socklen;
} proxy_request_accept_msg;

typedef struct proxy_request_shutdown_msg {
	int sock;
	int how;
} proxy_request_shutdown_msg;

typedef struct proxy_response_shutdown_msg {
	int ret;
	int err;
} proxy_response_shutdown_msg;


typedef struct proxy_response_set_msg {
	int ret;
	int err;
} proxy_response_set_msg;

#endif

