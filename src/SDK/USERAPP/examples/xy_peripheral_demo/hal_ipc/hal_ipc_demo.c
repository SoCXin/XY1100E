/**
* @file		hal_ipc_demo.c
* @brief	此demo展示核间通信api使用方法。\n
* 			在main函数中向CP发送核间消息，然后等待CP回复核间消息，收到CP的核间消息后复重复此过程。
***********************************************************************************/
#include "stdio.h"
#include "string.h"
#include "hal_cortex.h"
#include "hal_ipc.h"
#include "xy_mem.h"
#include "xy_utils.h"

//任务全局变量
osThreadId_t  hal_ipc_demo_TaskHandle = NULL;



/**
 * @brief	此函数为处理CP核间无需回复消息的函数。当AP接收到CP无需回复的核间消息后，会调用此函数进行消息处理。
 */
int hal_ipc_demo_noack_req_proc(int msg_id, char *data_buf, int data_len)
{
	//1.用户处理CP发送过来的核间数据。此demo仅打印数据，用户根据实际需求进行数据处理
	xy_printf("CP noack req data has been received:msg_id:%d,data:%s,length:%d\n", msg_id, data_buf, data_len);

	return 0;
}

/**
 * @brief	此函数为处理CP核间需要回复消息的函数。当AP接收到CP需要回复的核间消息后，会调用此函数进行消息处理。
 */
int hal_ipc_demo_ack_req_proc(int msg_id, char *data_buf, int data_len)
{
	//1.用户处理CP发送过来的核间数据。此demo仅打印数据，用户根据实际需求进行数据处理
	xy_printf("CP ack req data has been received:msg_id:%d,data:%s,length:%d\n", msg_id, data_buf, data_len);

	//2.回复CP的核间消息
	HAL_IPC_Message ipc_msg_tmp = {0};
	char *str = xy_zalloc(50);
	memcpy(str,"CP_TO_AP_USER_ACK_REQ rsp",strlen("CP_TO_AP_USER_ACK_REQ rsp"));
	ipc_msg_tmp.msg_id = CP_TO_AP_USER_ACK_REQ;
	ipc_msg_tmp.is_ack = 1;
	ipc_msg_tmp.len = sizeof("CP_TO_AP_USER_ACK_REQ rsp");
	ipc_msg_tmp.buf = str;
	HAL_IPC_Transmit(&ipc_msg_tmp);
	xy_free(str);

	return 0;
}



/**
 * @brief	初始化函数，初始化核间消息相关功能，注册处理CP核间数据的函数
 */
void hal_ipc_demo_init()
{
	//注册函数处理从CP发过来的无需回复的核间数据
	HAL_IPC_RegisterEventHandler(CP_TO_AP_USER_NOACK_REQ, hal_ipc_demo_noack_req_proc);
	//注册函数处理从CP发过来的需要回复的核间数据
	HAL_IPC_RegisterEventHandler(CP_TO_AP_USER_ACK_REQ, hal_ipc_demo_ack_req_proc);
}

/**
 * @brief	核间消息demo的任务函数
 */
void hal_ipc_demo_task()
{
	while(1)
	{
		HAL_IPC_Message ipc_msg_tmp = {0};
		char *str = xy_zalloc(50);

		//向CP发送需要回复的核间消息
		char *rsp_buf = xy_zalloc(50);
		memcpy(str,"AP_TO_CP_USER_ACK_REQ",strlen("AP_TO_CP_USER_ACK_REQ"));
		ipc_msg_tmp.msg_id = AP_TO_CP_USER_ACK_REQ;
		ipc_msg_tmp.is_ack = 0;
		ipc_msg_tmp.len = strlen("AP_TO_CP_USER_ACK_REQ");
		ipc_msg_tmp.buf = str;
		HAL_IPC_TransmitReceive(&ipc_msg_tmp, rsp_buf, 100, HAL_MAX_DELAY);
		if(strlen(rsp_buf) != 0)
			xy_printf("CP rsp data has been received:%s\n", rsp_buf);
		else
			xy_printf("CP rsp data has not been received\n");
		xy_free(rsp_buf);
		osDelay(3000);

		//向CP发送无需回复的核间消息
		memcpy(str,"AP_TO_CP_USER_NOACK_REQ",strlen("AP_TO_CP_USER_NOACK_REQ"));
		ipc_msg_tmp.msg_id = AP_TO_CP_USER_NOACK_REQ;
		ipc_msg_tmp.is_ack = 0;
		ipc_msg_tmp.len = strlen("AP_TO_CP_USER_NOACK_REQ");
		ipc_msg_tmp.buf = str;
		HAL_IPC_Transmit(&ipc_msg_tmp);
		osDelay(3000);

		xy_free(str);
	}
}


/**
 * @brief	任务创建函数
 *
 */
void hal_ipc_demo_task_create(void)
{
	hal_ipc_demo_TaskHandle = osThreadNew((osThreadFunc_t)hal_ipc_demo_task, NULL, "hal_ipc_demo_task", 1024, osPriorityNormal);
}


