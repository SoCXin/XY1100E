/** 
* @file      http_api.h
* @brief     
* @attention   
*/


#ifndef _XY_HTTP_API_H_
#define _XY_HTTP_API_H_

#include "xy_http_client.h"
#include "xy_utils.h"
#include "at_http.h"
#include "errno.h"
#include "lwip/sockets.h"

#define HTTP_HEADER 0
#define HTTP_BODY 	1
#define HTTP_SOCKERR 2
#define HTTP_FOREVER 0xFFFFFFFF
#define HTTP_NO_WAIT 0

#define XY_TRUE 0
#define XY_FALSE 1

#define XY_OK 0

#define HTTP_CONTEXT_REF_NUM 1
#define HTTP_RECV_NODE_NUMBER	8
/** 
* @brief       创建HTTP/HTTPS实例
* @attention   目前仅支持一个实例
* @param 	   host		     ：此API将解析host，获取到domain_name和port参数
* @param   	   auth_user	 ：授权的用户名
* @param   	   auth_passwd	 ：授权的密码
* @return      成功返回	 ： 返回已成功初始化的http id
* @return      失败返回	 ： XY_NOK
**/
int xy_http_create(char * host,char * auth_user, char * auth_passwd);

/** 
* @brief       HTTPS需要通过此命令配置实例
* @attention   仅HTTPS需要使用此AT命令
* @param 	   http_id：由at_http_create创建的http实例id
* @param 	   type：cfg类型如下
*				typedef enum type{
*				 	SEVER_CERT = 1,   		// 服务器 cert 证书
*				 	CLIENT_CERT = 2,  		// 客户端 cert 证书
*				 	CLIENT_PK = 3	  		// 客户端 pk
*					PRINT_MODE = 8			// 配置打印模式:  1 代表用16进制模式打印收到的字符， 0 代表字符串模式
*				} http_cfg_type_e;
* @param   	   data
* @param   	   encode_method：编码方式如下
*				typedef enum encode_method{
*					ESCAPE_MECHANISM = 0, 	// 转义字符串
*					HEX_CHARACTER = 1	  	// 十六进制字符串
*				} http_encode_method_e;
* @return      成功返回	 ： XY_OK
* @return      失败返回	 ： XY_NOK
**/
int xy_http_cfg(int http_id, int type, char *data, int encode_method);

/** 
* @brief       设置HTTP 头部信息。
* @attention   HTTP 协议要求 header 字段之间以\r\n 分隔
* @attention   当输入的 header 字段为空时，将清空此前已输入 header，否则新输入的 header 将添加在上一次 header 输入之后。
* @param 	   http_id：由at_http_create创建的http实例id
* @param   	   header：将要配置的头部信息
* @param   	   encode_method：编码方式如下
*				typedef enum encode_method{
*					ESCAPE_MECHANISM = 0, 	// 转义字符串
*					HEX_CHARACTER = 1	  	// 十六进制字符串
*				} http_encode_method_e;
* @return      成功返回：XY_OK
* @return      失败返回：XY_ERR
**/
int xy_http_header(int http_id, char *header, int encode_method);

/** 
* @brief       设置HTTP content 信息。
* @attention   HTTP 协议要求 content 字段之间以\r\n 分隔
* @attention   当输入的 content 字段为空时，将清空此前已输入 content，否则新输入的 content 将添加在上一次 content 输入之后。
* @param 	   http_id：由at_http_create创建的http实例id
* @param   	   content：将要配置的content信息
* @param   	   encode_method：编码方式如下
*				typedef enum encode_method{
*					ESCAPE_MECHANISM = 0, 	// 转义字符串
*					HEX_CHARACTER = 1	  	// 十六进制字符串
*				} http_encode_method_e;
* @return      成功返回：XY_OK
* @return      失败返回：XY_ERR
**/
int xy_http_content(int http_id, char *content, int encode_method);

/** 
* @brief       发送 HTTP 请求
* @param 	   http_id		：由at_http_create创建的http实例id
* @param   	   method		：提供的请求如下
*				typedef enum {
*					HTTP_METHOD_GET = 0,
*					HTTP_METHOD_POST,
*					HTTP_METHOD_PUT,
*					HTTP_METHOD_DELETE,
					HTTP_METHOD_HEAD，
*					HTTP_METHOD_CUSTOM = 99
*				} http_method_e;
* @param   	  path			:请求的 URL path 绝对路径参数
* @return      成功	： XY_OK
* @return      失败	： XY_NOK
**/
int xy_http_request(int http_id, int http_method, char *path);

/** 
* @brief       获取http的应答数据头
* @param 	   http_id		：由at_http_create创建的http实例id
* @param   	   buf			: 返回http请求的数据本体
* @return      读取到的数据长度
**/

int  xy_http_recv_header(int http_id, char *buf,  int len, unsigned int timeout);

/** 
* @brief       获取http的应答数据本体
* @param 	   http_id		：由at_http_create创建的http实例id
* @param   	   buf			: 返回http请求的数据头
* @return      读取到的数据长度
**/
int  xy_http_recv_body(int http_id, char *buf,  int len, unsigned int timeout);

/** 
* @brief       删除http\https实例
* @param 	   http_id		：由at_http_create创建的http实例id
* @return      删除完成	： 	   0
**/
int xy_http_close(int http_id);

/** 
* @brief       判断是否还有待接受的header
* @param 	   http_id		：由at_http_create创建的http实例id
* @return      仍有未读数据	： XY_FALSE
* @return      读取完成	： 	   XY_TRUE
**/
char header_recvlist_empty(int http_id);


/** 
* @brief       判断是否还有待接受的主体
* @param 	   http_id		：由at_http_create创建的http实例id
* @return      仍有未读数据	： XY_FALSE
* @return      读取完成	： 	   XY_TRUE
**/
char body_recvlist_empty(int http_id);

/** 
* @brief       判断http socket是否因异常断开
* @param 	   http_id		：由at_http_create创建的http实例id
* @return      socket正常	：         XY_FALSE
* @return      socket断开	： 	       XY_TRUE
**/
char http_socket_disconnect(int http_id);

#endif
