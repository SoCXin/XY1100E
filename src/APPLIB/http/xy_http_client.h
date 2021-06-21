#ifndef TCPIP_XY_HTTP_CLIENT_H_
#define TCPIP_XY_HTTP_CLIENT_H_
#include <stdint.h>

#include "urlparser.h"
#include "osal.h"
#define HTTP_HEADER 0
#define HTTP_BODY 	1
typedef enum {
	ESCAPE_MECHANISM = 0, 	// 字符串输入
	HEX_CHARACTER = 1	  	//	16进制输入
} http_encode_method_e;

typedef enum {
	SEVER_CERT = 1,   		// 服务器证书
	CLIENT_CERT = 2,  		// 客户端证书
	CLIENT_PK = 3,	  		// 客户端pk
	PRINT_MODE = 8	  		// 更改打印模式
} http_cfg_type_e;

typedef enum {
//	HTTP_INVALID = -1,
	HTTP_METHOD_GET = 0,
	HTTP_METHOD_POST,
	HTTP_METHOD_PUT,
	HTTP_METHOD_DELETE,
	HTTP_METHOD_HEAD,
	HTTP_METHOD_CUSTOM = 99
} http_method_e;

typedef enum {
	HTTP_DEALING_NONE = 0,
	HTTP_DEALING_HEADER,
	HTTP_DEALING_FINISH_HEADER,
	HTTP_DEALING_BODY
} http_dealing_state_e;

typedef enum {
	HTTP_RET_TIMEOUT,
	HTTP_RET_AGAIN,
	HTTP_RET_REMOTE_CLOSED,
	HTTP_RET_ERROR
} HTTP_RET;

#define MAX_HTTP_BUF_LEN 1024
typedef struct http_recv_t
{
	struct http_recv_t *pNext;		/*<< The next free block in the list, it is must be the first member. */
	char type;
	char more;
	unsigned short len;
	char *info;
	char *payload;
} Http_recv_t;


typedef struct http_context_s
{
	int sock;
	int http_id;
	int quit;

	char *host;
	char *domain_name;
	int port;
	int secured; // https or not
	void *tls_context;

	char *auth_user;
	char *auth_passwd;

	char *server_cert;
	char *client_cert;
	char *client_pk;

	int delay_time_print; // delay time print receive data
	int print_mode; // 1 in hex mode, 0 is in string mode

//	http_header_t *http_header;
	char *custom_header;
	char *content;

	char buf[MAX_HTTP_BUF_LEN]; // received data
	int padding;
	char *http_buf;
	http_dealing_state_e http_dealing_state;
	int current_recv_len;
	int left_recv_len;
	int current_processed_len;
	int history_processed_len;
//	int content_length;
	int data_recv_ever;
	int is_content_chunked;
	char http_more_header;
	char http_more_body;
	char print_or_not;		// could use for judge AT or API
	osSemaphoreId_t http_header_sem;
	osSemaphoreId_t http_body_sem;
	char recv_list_length;
	Http_recv_t *recv_list;
	union {
		struct {
			int content_length;
			int content_length_processed;
		};
		struct {
			int chunk_len;
			int chunk_len_processed;
		};
	};
} http_context_t;

/*
	Prototype functions
*/
int http_request(http_context_t *http_context, char *path, int http_method);
int http_connect(http_context_t *http_context);
int handle_http_data(http_context_t *http_context);
int http_close(http_context_t *http_context);

char Is_Recvlist_Empty(Http_recv_t *recv_list);

Http_recv_t *http_get_next_node(Http_recv_t *recv_list);

int http_delete_node(http_context_t *http_context, Http_recv_t *recv_node);

int http_add_list_head(Http_recv_t **list, Http_recv_t * httpdata);

int http_add_list_end(Http_recv_t **list, Http_recv_t * httpdata);

int http_context_init(http_context_t *http_context);

int http_context_clear(http_context_t *http_context);

int http_context_destroy(http_context_t *http_context);

char* make_http_post_put_content(http_context_t *http_context, char *path, int is_post);

char* make_http_delete_content(http_context_t *http_context, char *path);


int https_connect(http_context_t *http_context);

int handle_http_header(http_context_t *http_context);

int handle_http_body_chunked(http_context_t *http_context);

int handle_http_body(http_context_t *http_context);


int https_close(http_context_t *http_context);

int http_recv(http_context_t *http_context, unsigned char* buf, int size, int timeout);
#endif /* TCPIP_HTTP_HTTP_CLIENT_H_ */
