#ifndef __HAL_IPC_H__
#define __HAL_IPC_H__


/**
 * @brief	消息ID枚举，用于注册核间消息处理函数
 * @note	用户可自定义消息ID
 */
typedef enum {
	AP_TO_CP_USER_NOACK_REQ = 1002,
	AP_TO_CP_USER_ACK_REQ,
	CP_TO_AP_USER_NOACK_REQ,
	CP_TO_AP_USER_ACK_REQ,
	USER_REQ_MAX
}HAL_IPC_MessageID;

/**
 * @brief	消息结构体，结构体将通过核间通信发送至M3
 */
typedef struct
{
	HAL_IPC_MessageID	msg_id;
	unsigned int		is_ack;
	unsigned int		len;
    char				*buf;
} HAL_IPC_Message;


/**
 * @brief	核间消息处理函数指针
 */
typedef int (*IPC_EventHandler)(int msg_id, char *data_buf, int data_len);

/**
 * @brief	核间消息处理函数注册函数
 */
void HAL_IPC_RegisterEventHandler(HAL_IPC_MessageID msg_id, IPC_EventHandler func);

/**
 * @brief	发送核间消息函数
 */
int HAL_IPC_Transmit(HAL_IPC_Message* ipc_msg);

/**
 * @brief	发送核间消息并等待返回数据函数
 */
int HAL_IPC_TransmitReceive(HAL_IPC_Message* ipc_msg, void *rcv_msg, int rcv_msg_len, unsigned int timeout);


#endif
