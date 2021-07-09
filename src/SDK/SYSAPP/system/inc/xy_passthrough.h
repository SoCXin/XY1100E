#pragma once

#include <stdint.h>

/*******************************************************************************
 *                             Type definitions                                *
 ******************************************************************************/
typedef enum xy_passthrough_type
{
    PASSTHR_INVALID = 0,
    PASSTHR_NORMAL_PPP,   //ppp模式，通常以+++作为退出标志符
    PASSTHR_FIXED_LENGTH, //固定长度模式，收到指定长度数据后退出
    PASSTHR_SOCKET_PPP,   //socket数据透传模式，类似于ppp，以+++作为退出符
    PASSTHR_SMS,          //短信模式
    PASSTHR_MAX,
} xy_passthrough_type_t;

typedef struct xy_passthr_msg
{
    uint8_t msg_id;
    uint32_t len;
    char data[0];
} xy_passthr_msg_t;

/**
  * @brief  AT通道传输透传数据的回调函数声明
  */
typedef int (*data_proc_func)(char *data, unsigned int data_len);

/*******************************************************************************
 *                       Global variable declarations                          *
 ******************************************************************************/
extern data_proc_func g_at_passthr_hook;

/*******************************************************************************
 *                       Global function declarations                          *
 ******************************************************************************/
/**
 * @brief  用户关注！收到切换为透传模式AT指令后，调用该接口切换为透传模式接收状态
 * @param  mode [IN] 指定透传模式
 * @param  rcv_len [IN] 待接收的透传数据长度，PPP模式下该值必须填0，以指示长度不确定
 * @param  func [IN] 接收到的透传数据的回调接口，用户实现该接口内容
 * @note   该接口内部实现不建议客户做任何修改，客户仅需关注func回调接口和超时时长及超时处理即可
 */
void xy_enterPassthroughMode(xy_passthrough_type_t mode, uint32_t rcv_len, data_proc_func func);

/**
 * @brief 退出透传模式，切换到AT命令模式
 * @note
 */
void xy_exitPassthroughMode();

/** 
* @brief  用户关注！供用户处理PPP模式接收到的透传数据，需要客户自行拼接和缓存
*/
int passthr_normal_ppp_hook(char *buf, uint32_t len);

/** 
* @brief  用户关注！供用户自行处理指定长度的透传数据
*/
int passthr_fixed_len_hook(char *buf, uint32_t len);
