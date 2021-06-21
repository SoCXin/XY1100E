#include "bms_msg_info.h"

/*****************************************************************************************/
/**
  * @func   void get_bms_battery_monitor_msg(bms_bat_monitor_msg_rsp_t *bat_monitor_msg)
  * @brief  填充bms电池监控信息
  * @param  bat_monitor_msg         电池监控信息Response结构体
  * @retval none
  */
/*****************************************************************************************/
void get_bms_battery_monitor_msg(bms_bat_monitor_msg_rsp_t *bat_monitor_msg)
{
	bat_monitor_msg->totalVol = 0x1122;
	bat_monitor_msg->soh = 0x3344;
}

/*****************************************************************************************/
/**
  * @func   void get_bms_running_sta(bms_running_sta_info_burst_t *running_sta)
  * @brief  填充bms运行状态通知
  * @param  running_sta         bms运行状态通知Burst结构体
  * @retval none
  */
/*****************************************************************************************/
void get_bms_running_sta(bms_running_sta_info_burst_t *running_sta)
{
	running_sta->runSts = 0x0F;
}

/*****************************************************************************************/
/**
  * @func   void set_bms_running_sta_ack(bms_running_sta_info_ack_t *running_sta_ack)
  * @brief  填充bms运行状态通知（ACK）
  * @param  running_sta_ack         bms运行状态通知BurstACK结构体
  * @retval none
  */
/*****************************************************************************************/
void set_bms_running_sta_ack(bms_running_sta_info_ack_t *running_sta_ack)
{
	running_sta_ack->ack = 1;
	running_sta_ack->reserved[0] = 0;
	running_sta_ack->reserved[1] = 0;
	running_sta_ack->reserved[2] = 0;
}
