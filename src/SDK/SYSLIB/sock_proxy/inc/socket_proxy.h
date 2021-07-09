#ifndef SOCKET_PROXY_H
#define SOCKET_PROXY_H


#include "socket_proxy_com.h"


typedef struct addr_hdr {
	uint8_t  sa_len;//add this to adapt union pj_sockaddr for that struct sockaddr_in has sin_len
	uint8_t  sa_family;//change uint16 to uint8
} addr_hdr;


/*
typedef struct addr_hdr {
	uint16_t  sa_family;
} addr_hdr;
*/

/**
 * This union describes a generic socket address.
 */
typedef union pj_sockaddr {
	addr_hdr	    addr;	/**< Generic transport address.	    */
	struct sockaddr_in  ipv4;	/**< IPv4 transport address.	    */
	struct sockaddr_in6 ipv6;	/**< IPv6 transport address.	    */
} pj_sockaddr;


struct request_msg {

	int ext_thread_id;
	int  request_msg_id;
	int msglen;
	union {
		proxy_request_socket_msg 			request_sock_msg;
		proxy_request_listen_msg			request_listen_msg;
		proxy_request_accept_msg			request_accept_msg;
		proxy_request_shutdown_msg  		request_shutdown_msg;
		proxy_request_select_msg			request_select_msg;
		proxy_request_close_msg				request_close_msg;
		proxy_request_send_msg	 			request_send_msg;
		proxy_request_recv_msg  			request_recv_msg;
		proxy_request_recv_from_msg 		request_recv_from_msg;
		proxy_request_send_to_msg   		request_send_to_msg;
		proxy_request_connect_msg			request_connect_msg;
		proxy_request_bind_msg				request_bind_msg;
		proxy_request_set_opt_msg			request_set_opt_msg;
		proxy_request_get_opt_msg   		request_get_opt_msg;
		proxy_request_get_sock_name_msg 	request_get_sock_name_msg;
		proxy_request_get_peer_name_msg 	request_get_peer_name_msg;
		proxy_request_ioctrl_msg			request_ioctrl_msg;
		proxy_request_ioctrl_sock_msg   	request_ioctrl_sock_msg;
	} request_msg_list;
};

struct response_msg {

	int ext_thread_id;
	union {
		proxy_response_sock_msg 			response_sock_msg;
		proxy_response_listen_msg			response_listen_msg;
		proxy_response_accept_msg     		response_accept_msg;
		proxy_response_shutdown_msg			response_shutdown_msg;
		proxy_response_close_msg 			response_close_msg;
		proxy_response_select_msg 			response_select_msg;
		proxy_response_send_msg  			response_send_msg;
		proxy_response_recv_msg 	 		response_recv_msg;
		proxy_response_send_to_msg			response_send_to_msg;
		proxy_response_recv_from_msg 		response_recv_from_msg;
		proxy_response_connect_msg			response_connect_msg;
		proxy_response_bind_msg				response_bind_msg;
		proxy_response_set_opt_msg  		response_set_opt_msg;
		proxy_response_get_opt_msg  		response_get_opt_msg;
		proxy_response_get_sock_name_msg 	response_get_sock_name_msg;
		proxy_response_get_peer_name_msg 	response_get_peer_name_msg;
		proxy_response_ioctrl_msg			response_ioctrl_msg;
		proxy_response_ioctrl_sock_msg  	response_ioctrl_sock_msg;


	} response_msg_list;
};

#if XY_SOCKET_PROXY
int send_msg_to_sockproxy(void *buf,unsigned int len);
#endif

int write_to_cp(struct response_msg * p_response_msg, int msglen);



void proxy_sock_task(void);

void proxy_sock_init(void);


#endif


