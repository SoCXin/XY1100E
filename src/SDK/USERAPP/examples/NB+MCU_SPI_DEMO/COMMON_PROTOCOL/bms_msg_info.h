#ifndef __BMS_MSG_INFO_H
#define __BMS_MSG_INFO_H

#include <stdint.h>

/**
  * @brief  bms开机信息获取Request结构体
  */
typedef struct
{
	uint8_t sourceID;
	uint8_t reserve[3];
} bms_startup_mst_req_t;

/**
  * @brief  bms开机信息获取Response结构体
  */
typedef struct
{
	uint16_t swVersion;
	uint16_t hwVersion;
	uint16_t protocolVersion;
	uint16_t vendorID;
	uint16_t runSts;
	uint16_t  devType;
	uint8_t batID[6];
	uint8_t batCipher[16];
	uint8_t lastRebootReason;
	uint8_t reserve[8];

} bms_startup_mst_rsp_t;

/**
  * @brief  电池监控信息Request结构体
  */
typedef struct
{
	char data[0];
} bms_bat_monitor_msg_req_t;

/**
  * @brief  电池监控信息Response结构体
  */
typedef struct
{
	uint16_t totalVol;
	uint16_t cap;
	uint16_t temperature[4];
	uint16_t errorSts;
	uint16_t cellVolt[30];
	uint16_t batEnergy;
	uint16_t currents;
	uint16_t calibrationSts;
	uint16_t soh;
//	uint16_t reserved;
} bms_bat_monitor_msg_rsp_t;

/**
  * @brief  bms运行状态通知Burst结构体
  */
typedef struct
{
	uint16_t runSts;
	uint16_t reserved;
} bms_running_sta_info_burst_t;

/**
  * @brief  bms运行状态通知BurstACK结构体
  */
typedef struct
{
	uint8_t ack;
	uint8_t reserved[3];
} bms_running_sta_info_ack_t;


/*****************************************************************************************/
/**
  * @func   void get_bms_battery_monitor_msg(bms_bat_monitor_msg_rsp_t *bat_monitor_msg)
  * @brief  填充bms电池监控信息
  * @param  bat_monitor_msg         电池监控信息Response结构体
  * @retval none
  */
/*****************************************************************************************/
void get_bms_battery_monitor_msg(bms_bat_monitor_msg_rsp_t *bat_monitor_msg);

/*****************************************************************************************/
/**
  * @func   void get_bms_running_sta(bms_running_sta_info_burst_t *running_sta)
  * @brief  填充bms运行状态通知
  * @param  running_sta         bms运行状态通知Burst结构体
  * @retval none
  */
/*****************************************************************************************/
void get_bms_running_sta(bms_running_sta_info_burst_t *running_sta);
/*****************************************************************************************/
/**
  * @func   void set_bms_running_sta_ack(bms_running_sta_info_ack_t *running_sta_ack)
  * @brief  填充bms运行状态通知（ACK）
  * @param  running_sta_ack         bms运行状态通知BurstACK结构体
  * @retval none
  */
/*****************************************************************************************/
void set_bms_running_sta_ack(bms_running_sta_info_ack_t *running_sta_ack);


#endif	/* __BMS_MSG_INFO_H */
