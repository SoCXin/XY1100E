#pragma once

/*******************************************************************************
 *                           Include header files                              *
 ******************************************************************************/
 #include "xy_at_api.h"
 #include "osal.h"
 
/*******************************************************************************
 *                             Macro definitions                               *
 ******************************************************************************/
#define AT_CMD_PREFIX 30

/*******************************************************************************
 *                             Type definitions                                *
 ******************************************************************************/
/**
* @brief FARPS write function declaration
* @param at_rlt   at_cmd progress result or response;
* @param size     size of at_cmd progress result or response;
* @return  see specific func
* @note    farps device should realize func and parameters defintion should refer to above
*/
typedef int (*farps_write_func)(char *at_rlt, int size);

enum at_proc_state
{
	REQ_IDLE = 0x0,
	SEND_REQ_NOW = 0x1,
	RCV_REQ_NOW = 0x2,
};

enum at_context_position
{
	NEAR_PS = 0,
	FAR_PS,
};

enum at_process_state_e
{
	AT_PROCESS_DONE = 0,
	AT_PROCESSING,
};

typedef struct at_context
{
	struct at_context *fwd_ctx;
	char *g_farps_rcv_mem;
	int g_have_rcved_len;
	int g_rcv_buf_len;
	ser_req_func at_proc;              	//代理线程处理的命令的对应函数
	farps_write_func farps_write;      	//farps 回写接口,根据不同外设赋值
	osMessageQueueId_t user_queue_Id;	//indicate queue fd for every user task,specially contain proxy task
	osTimerId_t user_ctx_tmr;			//用户线程软定时器ID
	char *at_param;
	int error_no;
	int fd;								//at上下文fd,可参考@AT_SRC_FD
	uint8_t process_state:2;			//指示外部来的AT请求是否处理完毕，当AT命令尚未处理完毕，不能进入深睡
	uint8_t position:2;					//see NEAR_PS/FAR_PS
	uint8_t state:2;					//see SEND_REQ_NOW
	uint8_t sms_flag:2;					//sms state
	uint8_t retrans_count;				//命令重发计数
	char at_cmd_prefix[AT_CMD_PREFIX];	//letter string XXX,only used for unsolicited string match,such as "+XXX:"
} at_context_t;

typedef struct at_context_dict
{
	struct at_context_dict *pre;
	at_context_t *node;
	struct at_context_dict *next;
} at_context_dict_t;

/*******************************************************************************
 *                       Global variable declarations                          *
 ******************************************************************************/
extern at_context_t nearps_ctx;
extern at_context_t uart_ctx;
extern at_context_t user_rsp_ctx;
#if BLE_ENABLE
extern at_context_t ble_ctx;
#endif
extern at_context_dict_t *g_at_context_dict;
extern osMutexId_t at_ctx_dict_m;
extern osMutexId_t at_user_fd_m;

/*******************************************************************************
 *                       Global function declarations                          *
 ******************************************************************************/
/**
 * @brief reset the specific context 
 * @param ctx [IN] the specific context 
 * @return  0: register success,-1: register fail
 * @note clean state,cmd_prefix,at_proc,at_param,forward context
 * @warning some context paramters must clean up!!!
 */
void reset_ctx(at_context_t *ctx);

/**
 * @brief register the at_context to at_context_dictionary
 * @param ctx  at_context to be registered
 * @return 0: register success, -1: register fail
 * @note
 */
int register_at_context(at_context_t *ctx);

/**
 * @brief deregister the at_context related to this fd to at_context_dictionary
 * @param fd  context fd, See @at_source_fd
 * @return 0: deregister success,-1: deregister fail
 * @note free the related context here
 * @warning 
 */
int deregister_at_context(int fd);

/**
 * @brief     search the at_context related to given fd in at_context dictionary
 * @param fd  context fd, See @at_source_fd
 * @return    the at_context addr if find in the dict, if null: not found in dict,
 * @note      should deal with the return value in code
 * @warning   if return null user need to deal with
 */
at_context_t *search_at_context(int fd);

/**
 * @brief   get available context fd for user
 * @param from_proxy   mark caller is proxy task
 * @return  -1:no avail fd now,may had been distributed all, 1000-1004: avail fd for user
 * @note    used for at_ReqAndRsp_to_ps interface 
 * @warning
 */
int get_avail_atfd_4_user(int from_proxy);

/**
 * @brief  通过软定时器id获取对应的队列ID
 * @param timerId  [IN]软定时器id
 * @return  成功返回对应的队列ID,失败返回NULL
 * @note 主要用于at_ReqAndRsp_to_ps 和 at_ReqAndRsp_to_ps_2接口
 */
osMessageQueueId_t at_related_queue_4_user(osTimerId_t timerId);

