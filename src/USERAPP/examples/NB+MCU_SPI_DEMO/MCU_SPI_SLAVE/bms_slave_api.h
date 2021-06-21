#ifndef __BMS_SLAVE_API_H
#define __BMS_SLAVE_API_H

#include "tlv_protocol.h"
#include "bms_msg_info.h"

/*****************************************************************************************/
/**
  * @func   void bms_slave_response_burst(tlv_cmd_info_t *p_cmd_info)
  * @brief  bms从机发送应答数据处理函数，拉高gpio以产生中断信号给nb主机
  * @param  p_cmd_info            tlv命令结构体
  * @retval none
  */
/*****************************************************************************************/
void bms_slave_response_burst(tlv_cmd_info_t *p_cmd_info);
/*****************************************************************************************/
/**
  * @func   uint16_t bms_slave_recv_data(uint8_t *buf)
  * @brief  bms从机接收（主机发送过来的）数据
  * @param  buf            接收（主机）数据
  * @retval read_len       接收到的（主机）数据长度
  */
/*****************************************************************************************/
uint16_t bms_slave_recv_data(uint8_t *buf);

/*****************************************************************************************/
/**
  * @func   void bms_slave_burst(tlv_cmd_info_t *p_cmd_info)
  * @brief  bms从机打包并发送burst数据，拉高gpio以产生中断信号给nb主机
  * @param  p_cmd_info            tlv命令结构体
  * @retval none
  */
/*****************************************************************************************/
void bms_slave_burst(tlv_cmd_info_t *p_cmd_info);

/*****************************************************************************************/
/**
  * @func   void bms_slave_response(tlv_cmd_info_t *p_cmd_info)
  * @brief  bms从机打包并发送response数据，拉高gpio以产生中断信号给nb主机
  * @param  p_cmd_info            tlv命令结构体
  * @retval none
  */
/*****************************************************************************************/
void bms_slave_response(tlv_cmd_info_t *p_cmd_info);


#endif	/* __BMS_SLAVE_API_H */
