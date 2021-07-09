#pragma once

/*******************************************************************************
 *							             Include header files							                 *
 ******************************************************************************/
#include "at_context.h"
#include "xy_proxy.h"

/*******************************************************************************
 *                             Macro definitions                               *
 ******************************************************************************/
/**
* @brief 根据at_context 发送AT错误信息到对应的外设，该接口会执行reset context操作
* @param err_no  错误码，8007、8003 @see AT_XY_ERR
* @param ctx  at_context上下文
* @note 用于at_ctl内部出错处理，需要执行reset context的场景
*/
#define AT_ERR_BY_CONTEXT(err_no, ctx)   send_at_err_to_context(err_no, ctx, __FILE__, __LINE__)

/**
* @brief 直接发送AT错误信息到对应的外设，该接口不会执行reset context操作
* @param err_no  错误码，8007、8003 @see AT_XY_ERR
* @param ctx  at_context上下文
* @note 一般用于farps中的处理，不执行reset context可以避免因多线程操作全局context导致返回的error信息不正确
*/
#define AT_ERR_TO_PERIPHERAL(err_no, ctx) send_at_err_to_periperal(err_no, ctx, __FILE__, __LINE__)

#define AT_CTRL_THREAD_NAME "at_ctl"

/*******************************************************************************
 *                             Type definitions                                *
 ******************************************************************************/
typedef struct at_msg
{
	at_context_t *ctx;
	int msg_id; //see  at_msg_id
	int size;
	char data[0];
} at_msg_t;

/**用于封装at_ctl向主控线程发送的数据*/
typedef struct at_proxy_data
{
  at_context_t *proxy_ctx;
  int at_req_type;
  char data[0];
} at_proxy_data_t;

/*******************************************************************************
 *                       Global variable declarations                          *
 ******************************************************************************/
extern osMessageQueueId_t at_farps_q; //receive  extern mcu  at  cmd  to  user task
extern osMutexId_t at_farps_m;
extern osMutexId_t g_farps_write_m;
extern struct at_user_inform_proc_e *g_sysapp_urc;
extern struct at_user_serv_proc_e *g_sysapp_req_serv;
extern char g_atctl_req_type; //at_ctl解析出来的AT类型作为参数传递给proxy线程，避免at_ctl和proxy共用全局导致异常

/*******************************************************************************
 *                       Global function declarations                          *
 ******************************************************************************/
/**
 * @brief at module initialization operation
 * @note initial global at_context,such as uart/ble/nearps context,
 * @note initial related queue and task
 */
void at_init();

/**
 * @brief at ctl task entry
 * @note receive msg from user/dsp/uart/ble and handle it
 */
void at_ctl(void);

/**
 * @brief send msg to atctl task, core function 
 * @param type   indicate the info source, from nearps or farps
 * @param data   cmd data
 * @param size   cmd size
 * @param fd   indicate the fd or sendor, @see AT_SRC_FD
 * @note 
 * @return success return AT_OK, or return -1
 */
int send_msg_2_atctl(int type, void *data, int size, int fd);

/**
 * @brief forward requst cmd to other context
 * @param msg [IN] msg wanted to be forwaded
 * @note for example, AT+CGSN=1,should forward its context(uart/ble/user) to dsp context
 */
 void forward_req_at_proc(at_msg_t *msg);

 /**
 * @brief send err info to specified context
 * @param errno [IN] error number,see @AT_XY_ERR
 * @param ctx [IN] the specified context
 * @param file [IN] the file of the caller 
 * @param line [IN] the line in the file of the caller
 * @note
 */
void send_at_err_to_context(int errno, at_context_t *ctx, char *file, int line);

/**
 * @brief send response to user task queue
 * @param queue_fd [IN] the blocked user task queue
 * @param data [IN] at cmd data
 * @param size [IN] cmd data size
 * @note at_ctl use the func to notify the specified blocked task queue
 */
int send_rsp_str_2_app(osMessageQueueId_t queue_fd, char *data, int size);

/**
 * @brief write at request/response/info to specific farps
 * @param farps_fd [IN] farps fd,such as uart/ble fd,see @at_source_fd
 * @param buf [IN] at request/response/info data
 * @param len [IN] at request/response/info data size
 * @note  the caller need to free the memory--buf
 */
int at_farps_fd_write(int farps_fd, void *buf, int size);

/**
 * @brief write at request/response/info to specific nearps
 * @param nearps_fd [IN] nearps fd,such as dsp fd,see @at_source_fd
 * @param buf [IN] at request/response/info data
 * @param len [IN] at request/response/info data size
 * @note  the caller need to free the memory--buf
 */
int at_nearps_fd_write(int nearps_fd, void *buf, int size);

/**
 * @brief  write at request/response/info to the specific context
 * @param ctx [IN] the specific context,such as dsp/ble/uart/user/
 * @param buf [IN] at request/response/info data address
 * @param size [IN] at request/response/info data size
 * @return  0/1: write success,-1: write fail
 */
int at_write_by_ctx(at_context_t *ctx, void *buf, int size);

/**
 * @brief broadcast response or info to each farps entity which have broadcast property
 * @param buf [IN] response or info  data
 * @param size [IN] response or info data size
 * @return  parase result,XY_OK is success
 * @note
 * @warning only TTY and BLE can receive
 */
int at_rsp_info_broadcast(void* buf, int size);

/**
 * @brief shm module use the function send dsp reponse/info to at_ctl
 * @param buf [IN] shm received data
 * @param len [IN] shm received data size
 * @note  only used for dsp context now
 */
void at_rcv_from_nearps(void *buf, unsigned int len);

/**
 * @brief [deprecated]
 * @param buf [IN]
 * @param len [IN]
 * @warning [deprecated] may be used backwards
 */
void at_rcv_from_farps(void *buf, unsigned int len);

/**
 * @brief process data come from uart
 * @param buf [IN] at request data
 * @param len [IN] at request data size
 * @param fd [IN] the related fd with uart,such as uart/ble, fd defition see @at_source_fd
 * @note
 */
void at_recv_from_uart(char *buf, unsigned long data_len, int fd);

/**
 * @brief process data come from ble
 * @param buf [IN] at request data
 * @param len [IN] at request data size
 * @param fd [IN] the related fd with uart,such as uart/ble, fd defition see @at_source_fd
 * @note
 */
#if BLE_ENABLE
void at_recv_from_ble(char *buf, unsigned long data_len, int fd);

/**
 * @brief 发送debug信息给蓝牙口
 * @param  buf 待发送的数据
 * @note
 */
void send_debug_str_to_ble(char *buf);

#endif //BLE_ENABLE

/**
  * @brief sys_up_urc func
  * @note  see at_SIMST_info,zhongyi request send "POWERON" followed by "^SIMST:"
  */
void sys_up_urc(void);

/**
  * @brief sys_up_urc func
  * @note  see at_SIMST_info,zhongyi request send "POWERON" followed by "^SIMST:"
  */
void at_send_to_shm(void* buf ,unsigned int len);

/**
  * @brief sys_up_urc func
  * @note  see at_SIMST_info,zhongyi request send "POWERON" followed by "^SIMST:"
  */
void send_at_to_ps(char* buf ,unsigned int len);

/**
 * @brief 主控线程处理平台内部命令接口
 * @param  msg  see xy_proxy_msg_t
 * @return int 
 */
int proc_at_proxy_req(xy_proxy_msg_t *msg);

/**
 * @brief 
 * @param  buf 
 * @return int 
 */
int at_write_nearps_directly(void *buf);

/**
 * @brief 发送错误信息到外设，不走context处理流程
 * @param  err_no  错误码
 * @param  ctx context fd, @see AT_SRC_FD
 * @note
 */
void send_at_err_to_periperal(int err_no, at_context_t* ctx, char *file, int line);

/**
 * @brief 根据context fd发送debug信息给合适的外设，如uart及ble
 * @param  buf
 * @param  fd context fd, @see AT_SRC_FD
 * @note
 */
void send_debug_str_to_at_peripheral(char *buf, int fd);
