/*
 * at_simple_http.h
 *
 *  Created on: 2020��8��19��
 *      Author: Administrator
 */

#ifndef TCPIP_HTTP_ATCMD_AT_HTTP_H_
#define TCPIP_HTTP_ATCMD_AT_HTTP_H_

#include "xy_http_client.h"
#include <stdint.h>
#include "osal.h"

enum XY_HTTP_MSG_FLAG
{
	HTTP_CREATE_REQ,
	HTTP_CFG_REQ,
	HTTP_HEADER_REQ,
	HTTP_CONTENT_REQ,
	HTTP_REQUEST_REQ,
};

typedef struct http_para
{
	int http_id;
	int flag;
	int respond;
	char *respond_buf;
    char success_or_not;
	union {
		struct {
			int *host;
			int *auth_user;
			int *auth_passwd;
		};
		struct {
			int *method;
			int *path;
		};
		struct {
			union {
				struct {
					int type;
					int *data;
				};
				struct {
					int parsed_num;
					union {
						char *content;
						int *header;
					};
				};
			};
			int encode_method;
		};
	};
}Http_Para_t;


extern void at_http_recv(void* argument);
extern void at_http_parse(void* argument);
extern void at_http_init(void);
extern void proxy_http_parse(Http_Para_t *msg );
typedef struct http_context_reference_s {
//	unsigned int ref;
	http_context_t *http_context;
    osThreadId_t http_recv_thread_id;
	osThreadId_t http_at_thread_id;
    int quit;
//    http_context_config_t *http_context_config;
} http_context_reference_t;

#endif /* TCPIP_HTTP_ATCMD_AT_HTTP_H_ */
