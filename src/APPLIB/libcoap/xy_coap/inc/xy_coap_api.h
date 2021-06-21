/**
* @file        xy_coap_api.h
* @brief       This file is the COAP API that users can call.
* @attention   请参考coap_opencpu_demo.c进行二次开发。
* @par
*/
#ifndef _XY_COAP_API_H_
#define _XY_COAP_API_H_
#include <stdint.h>

/**
 *@ingroup coapapi
 *@brief create COAP client and register receive COAP message callback func
 *@param ipaddr       [IN]     destination address
 *@param port         [IN]     destination port
 *@retval #coap_context_t  0 is failed, or the other return value is succeed.
 */
coap_context_t *xy_coap_client_create(char *ipaddr,unsigned short port);

/**
 *@ingroup coapapi
 *@brief delete COAP client and free memory
 *@param ctx          [IN]     CoAP stack's global state
 *@retval #void
 */
void xy_coap_client_delete(coap_context_t *ctx);

/**
 *@ingroup coapapi
 *@brief config COAP message header
 *@param msgid        [IN]     message ID in COAP message
 *@param token        [IN]     token field in COAP message
 *@param tkl          [IN]     length of token
 *@retval #int  0 is succeed, or the other return value is failed.
 */
int xy_config_coap_head(int msgid, char *token, int tkl);

/**
 *@ingroup coapapi
 *@brief config COAP message option
 *@param opt          [IN]     COAP message option type,see pdu.h for detailed types
 *@param opt_value    [IN]     COAP message option value
 *@retval #int  0 is succeed, or the other return value is failed.
 */
int xy_config_coap_option(int opt, char *opt_value);

/**
 *@ingroup coapapi
 *@brief send and receive COAP message,synchronous mode
 *@param ctx          [IN]     CoAP stack's global state
 *@param method       [IN]     method code in COAP message
 *@param payload      [IN]     payload field in COAP message
 *@param payload_len  [IN]     payload length
 *@retval #int  0 is succeed, or the other return value is failed.
 */
int xy_coap_send(coap_context_t *ctx, char *method, char *payload,int payload_len);

/**
 *@ingroup coapapi
 *@brief send COAP message,asynchronous mode
 *@param ctx          [IN]     CoAP stack's global state
 *@param type         [IN]     type in COAP message
 *@param method       [IN]     method code in COAP message
 *@param payload      [IN]     payload field in COAP message
 *@param payload_len  [IN]     payload length
 *@retval #int  0 is succeed, or the other return value is failed.
 */
int xy_coap_asy_send(coap_context_t *ctx,char *type, char *method, char* payload,int payload_len);

/**
 *@ingroup coapapi
 *@brief receive COAP message,asynchronous mode
 *@param ctx          [IN]     CoAP stack's global state
 *@param quitflag     [IN]     quit receive coap message
 *@retval #int  0 is succeed, or the other return value is failed.
 */
int xy_coap_asy_receive(coap_context_t *ctx,int *quitflag);

#endif

