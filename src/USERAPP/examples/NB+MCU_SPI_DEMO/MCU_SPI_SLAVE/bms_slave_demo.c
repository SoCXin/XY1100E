#include "bms_msg_info.h"
#include "tlv_protocol.h"
#include "bms_slave_init.h"
#include "bms_slave_api.h"

#include "stddef.h"
#include "osal.h"

/*****************************************************************************************/
/**
  * @func   void respons_bms_bat_monitor(void)
  * @brief  打包并发送bms电池监控信息
  * @param  none
  * @retval none
  */
/*****************************************************************************************/
void respons_bms_bat_monitor(void)
{
	tlv_cmd_info_t ack_cmd_info;
	bms_bat_monitor_msg_rsp_t bat_monitor_msg;

	get_bms_battery_monitor_msg(&bat_monitor_msg);

	//用户需将全部data转换大小端，demo此处只转换测试data
	bat_monitor_msg.totalVol = B_L_REVERT16(bat_monitor_msg.totalVol);
	bat_monitor_msg.soh = B_L_REVERT16(bat_monitor_msg.soh);

	ack_cmd_info.cmd_id = BAT_MONITOR_MSG_GET;
	ack_cmd_info.data_buf = (char *)&bat_monitor_msg;
	ack_cmd_info.data_len = sizeof(bms_bat_monitor_msg_rsp_t);
	ack_cmd_info.data_type = DATA_FRAME_TYPE_RESPONSE;

	bms_slave_response(&ack_cmd_info);
}

/*****************************************************************************************/
/**
  * @func   void burst_bms_running_sta_info(void)
  * @brief  打包并发送bms运行状态通知
  * @param  none
  * @retval none
  */
/*****************************************************************************************/
void burst_bms_running_sta_info(void)
{
	tlv_cmd_info_t burst_cmd_info;
	bms_running_sta_info_burst_t running_sta;

	get_bms_running_sta(&running_sta);

	//用户需将全部data转换大小端，demo此处只转换测试data
	running_sta.runSts = B_L_REVERT16(running_sta.runSts);

	burst_cmd_info.cmd_id = BMS_RUNNING_STA_INFO;
	burst_cmd_info.data_buf = (char *)&running_sta;
	burst_cmd_info.data_len = sizeof(bms_running_sta_info_burst_t);
	burst_cmd_info.data_type = DATA_FRAME_TYPE_BURST;

	bms_slave_burst(&burst_cmd_info);
}


/*****************************************************************************************/
/**
  * @func   void bms_slave_proc_task(void)
  * @brief  bms从机处理从SPI接收到的每一条完整消息
  * @attention 该接收线程内部，可能需要操控获取NB相关信息，可以调用xy_ps_api.h以及其他平台的任何外部接口进行NB芯片的操控
  */
/*****************************************************************************************/
void bms_slave_proc_task(void)
{
	tlv_cmd_info_t recv_cmd_info;

	while(1)
	{
		osMessageQueueGet(bms_slave_proc_Queue, &recv_cmd_info, NULL, osWaitForever);

		switch(recv_cmd_info.cmd_id)
		{
		case BAT_MONITOR_MSG_GET:
			send_debug_str_to_at_uart("\r\nSlave Response\r\n");
			respons_bms_bat_monitor();
			break;

		case BMS_RUNNING_STA_INFO:
			send_debug_str_to_at_uart("\r\nSlave Receive Master's BurstACK\r\n");
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
  * @func   void bms_slave_test(void)
  * @brief  用户的主任务线程DEMO，该实例线程中随时可能经SPI总线发送动作请求给主设备，进行双方的操控
  * @attention 该用户任务线程内部，可能需要操控获取NB相关信息，可以调用xy_ps_api.h以及其他平台的任何外部接口进行NB芯片的操控
  */
/*****************************************************************************************/
volatile int bms_slave_test_data = 0;
void bms_slave_test(void)
{
	while(1)
	{
		osDelay(3500);
		send_debug_str_to_at_uart("\r\nSlave Burst\r\n");
		burst_bms_running_sta_info();
	}
}


/**
 * @brief	用于slave主动上报测试的AT命令函数
 */
int at_slave_burst_req(char *at_buf, char **prsp_cmd)
{
	if(g_req_type==AT_CMD_REQ || g_req_type==AT_CMD_ACTIVE)
		{
			int  cmd=0;
			void *p[] = {&cmd};

			if(at_parse_param("%d", at_buf, p) != AT_OK)
			{
				*prsp_cmd = AT_ERR_BUILD(ATERR_PARAM_INVALID);
				return AT_END;
			}

			burst_bms_running_sta_info();
			sprintf(*prsp_cmd  + strlen(*prsp_cmd ), "\r\nOK\r\n");
			sprintf(*prsp_cmd  + strlen(*prsp_cmd ), "\r\nSlave Burst\r\n");
		}

		if (g_req_type == AT_CMD_TEST)
		{
			*prsp_cmd = xy_malloc(32);

			burst_bms_running_sta_info();
			sprintf(*prsp_cmd  + strlen(*prsp_cmd ), "\r\nOK\r\n");
			sprintf(*prsp_cmd  + strlen(*prsp_cmd ), "\r\nSlave Burst\r\n");
		}

		return AT_END;
}






