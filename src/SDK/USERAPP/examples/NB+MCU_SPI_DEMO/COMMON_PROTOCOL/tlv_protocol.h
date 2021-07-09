#ifndef __BMS_PROTOCOL_H
#define __BMS_PROTOCOL_H

#include <stdint.h>


#define TLV_DATA_TYPE_RESPONSE  0
#define TLV_DATA_TYPE_REQUEST   1
#define TLV_DATA_TYPE_BURST     2
#define TLV_DATA_TYPE_BURSTACK  3
#define TLV_DATA_SEC_RC4        1<<7
#define TLV_DATA_TYPE_MASK      0x7f
#define TLV_DATA_SEC_MASK       (1<<7)

#define TLV_MIN_SIZE 12
#define TLV_MAX_SIZE (256 - 2)  //局部数组大小 - data_len本身长度2个字节，局部数组大小可根据实际调整
#define TLV_TAG_MIN  1
#define TLV_TAG_MAX  17
#define TLV_HEAD  0xa5

#define B_L_REVERT16(var) (((var) & 0xff) << 8 | ((var) & 0xff00) >> 8)
#define B_L_REVERT32(var) (((var) & 0xff) << 24 | ((var) & 0xff00) << 8) | ((var) & 0xff0000) >> 8) ((var) & 0xff000000) >> 24)

/**
  * @brief  tlv命令结构体
  */
typedef struct tlv_cmd_info
{
	unsigned short cmd_id;
	char reserve[3];
	char* data_buf;
	unsigned short data_len;
	unsigned short data_type;
} tlv_cmd_info_t;

/**
  * @brief  tlv协议结构体
  *  1B	    2B	  3B 4B	       5B 6B	   7B 8B   9B 10B    11B       12B    13B...NB
  * head   flag  data_len  complement_len   crc   seq_num  ver_num  data_type   data
  *        通信命令     数据长度                               长度补数                   CRC校验和            序列号                 版本号                 数据帧类型                  数据
  */
typedef struct tlv_format
{
	unsigned short data_len;
	unsigned short complement_len;
	unsigned short crc;
	unsigned short seq_num;
	unsigned char ver_num;
	unsigned char data_type;
  	unsigned short flag;
	char data_buf[0];
} tlv_format_t;


//use for tlv_cmd_info_t 'cmd_id' member and tlv_format_t 'flag' member
#define BMS_STARTUP_MSG_GET     0x01
#define NB_STARTUP_MSG_GET      0x02
#define BAT_MONITOR_MSG_GET     0x03
#define BMS_RUNNING_STA_INFO    0x04


//use for tlv_cmd_info_t 'data_type' member and tlv_format_t 'data_type' member
#define DATA_FRAME_TYPE_RESPONSE    0
#define DATA_FRAME_TYPE_REQUEST     1
#define DATA_FRAME_TYPE_BURST       2
#define DATA_FRAME_TYPE_BURSTACK    3


//use for 'unpack_tlv' return value
#define TLV_UNPACK_SUCCESS            0
#define TLV_UNPACK_LEN_SHORT         -1
#define TLV_UNPACK_RCV_UNFINISHED    -2

/*****************************************************************************************/
/**
  * @func   int pack_tlv(char *buf,unsigned short buf_len, tlv_cmd_info_t* p_cmd_info)
  * @brief  tlv格式组包
  * @param  buf         组包后存放空间
  *         buf_len         buf长度
  * @retval 组包后有效数据长度
  *         p_cmd_info  待组包的数据结构体指针
  */
/*****************************************************************************************/
int  pack_tlv(char *buf, unsigned short buf_len, tlv_cmd_info_t* p_cmd_info);
/*****************************************************************************************/
/**
  * @func   int  unpack_tlv(char *buf, unsigned short *buf_len, tlv_cmd_info_t* p_cmd_info)
  * @brief  每次从spi接收buf中尝试解出一组有效数据,丢弃无效数据
  * @param  buf         spi接收的数据缓冲区
  *         buf_len     数据缓冲区内待处理数据长度
  * @retval 解包结果 TLV_UNPACK_SUCCESS表示成功解出一个有效帧，其他为错误码
  *         p_cmd_info  指向解出的有效帧
  */
/*****************************************************************************************/
int  unpack_tlv(char *buf, unsigned short *buf_len, tlv_cmd_info_t* p_cmd_info);


#endif	/* __BMS_PROTOCOL_H */
