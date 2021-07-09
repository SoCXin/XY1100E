#ifndef __NB_MASTER_API_H
#define __NB_MASTER_API_H

#include <stdint.h>
#include "tlv_protocol.h"
#include "bms_msg_info.h"

/*****************************************************************************************/
/**
  * @func   void nb_master_send_request_ack(tlv_cmd_info_t *p_cmd_info)
  * @brief  nb主机发送Request应答
  * @param  p_cmd_info           tlv命令结构体
  * @retval none
  */
/*****************************************************************************************/
void nb_master_send_request_ack(tlv_cmd_info_t *p_cmd_info);
/*****************************************************************************************/
/**
  * @func   uint16_t nb_master_recv_data(uint8_t *buf)
  * @brief  nb主机接收数据线程
  * @param  buf           接收到的数据
  * @retval read_len      接收到的数据长度
  */
/*****************************************************************************************/
uint16_t nb_master_recv_data(uint8_t *buf);


#endif	/* __NB_MASTER_API_H */
