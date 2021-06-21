#include "bms_msg_info.h"
#include "tlv_protocol.h"
#include "nb_master_init.h"
#include "nb_master_api.h"


#include "stddef.h"
#include "osal.h"

/*****************************************************************************************/
/**
  * @func   void request_bms_bat_monitor(void)
  * @brief  nb主机打包并发送电池监控信息
  * @param  none
  * @retval none
  */
/*****************************************************************************************/
void request_bms_bat_monitor(void)
{
	tlv_cmd_info_t req_cmd_info;

	req_cmd_info.cmd_id = BAT_MONITOR_MSG_GET;
	req_cmd_info.data_buf = NULL;
	req_cmd_info.data_len = 0;
	req_cmd_info.data_type = DATA_FRAME_TYPE_REQUEST;

	nb_master_send_request_ack(&req_cmd_info);
}

/*****************************************************************************************/
/**
  * @func   void burstack_bms_running_sta_info(void)
  * @brief  nb主机打包并发送bms运行状态通知
  * @param  none
  * @retval none
  */
/*****************************************************************************************/
void burstack_bms_running_sta_info(void)
{
	tlv_cmd_info_t rsp_cmd_info;
	bms_running_sta_info_ack_t running_sta_ack;

	set_bms_running_sta_ack(&running_sta_ack);

	//用户需将全部data转换大小端，demo此处只转换测试data

	rsp_cmd_info.cmd_id = BMS_RUNNING_STA_INFO;
	rsp_cmd_info.data_buf = (char *)&running_sta_ack;
	rsp_cmd_info.data_len = sizeof(bms_running_sta_info_ack_t);
	rsp_cmd_info.data_type = DATA_FRAME_TYPE_RESPONSE;

	nb_master_send_request_ack(&rsp_cmd_info);
}

/*****************************************************************************************/
/**
  * @func   void nb_master_proc_task(void)
  * @brief  nb主机处理从SPI接收到的每一条完整消息
  * @attention 该接收线程内部，可能需要操控获取NB相关信息，可以调用xy_ps_api.h以及其他平台的任何外部接口进行NB芯片的操控
  */
/*****************************************************************************************/
void nb_master_proc_task(void)
{
	tlv_cmd_info_t recv_cmd_info;

	while(1)
	{
		osMessageQueueGet(nb_master_proc_Queue, &recv_cmd_info, NULL, osWaitForever);

		switch(recv_cmd_info.cmd_id)
		{
		case BAT_MONITOR_MSG_GET:
			send_debug_str_to_at_uart("\r\nMaster Request then Receive Slave Response\r\n");
			break;

		case BMS_RUNNING_STA_INFO:
			send_debug_str_to_at_uart("\r\nMaster Receive Slave Burst and Master BurstACK\r\n");
			burstack_bms_running_sta_info();
			break;
		default:
			break;
		}

		if(recv_cmd_info.data_buf != NULL)
		{
			xy_free(recv_cmd_info.data_buf);
		}
	}
}

/*****************************************************************************************/
/**
  * @func   void nb_master_test(void)
  * @brief  用户的主任务线程DEMO，该实例线程中随时可能经SPI总线发送动作请求给底板BMS系统，进行双方的操控
  * @attention 该用户任务线程内部，可能需要操控获取NB相关信息，可以调用xy_ps_api.h以及其他平台的任何外部接口进行NB芯片的操控
  */
/*****************************************************************************************/
void nb_master_test(void)
{
	while(1)
	{
		osDelay(2500);
		send_debug_str_to_at_uart("\r\nMaster Request\r\n");
		request_bms_bat_monitor();
	}
}
