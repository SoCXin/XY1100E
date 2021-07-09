#include "tlv_protocol.h"


#include <string.h>
#include "osal.h"
#include "xy_utils.h"

/*****************************************************************************************/
/**
  * @func   uint16_t crc16_calc(uint8_t *packet, int32_t length)
  * @brief  crc校验和函数
  * @param  packet         需要crc校验的数据
  *         length         需要crc校验的数据长度
  * @retval (crc)          返回(crc)校验和
  */
/*****************************************************************************************/
uint16_t crc16_calc(uint8_t *packet, int32_t length)
{
    uint16_t crc = 0xFFFF;
    int32_t i, j;

    for (j = 0; j < length; j++) {
        crc = crc ^ (*packet);
        packet++;

        for (i = 0; i < 8; i++) {
            if (crc & 0x0001) {
                crc = crc >> 1;
                crc = crc ^ 0xa001;
            } else {
                crc = crc >> 1;
            }
        }
    }
    return (crc);
}

/*****************************************************************************************/
/**
  * @func   void tlv_buf_bidendian_change(tlv_format_t *buf)
  * @brief  将数据格式转换成大端模式
  * @param  buf           tlv命令结构体
  * @retval none
  */
/*****************************************************************************************/
void tlv_format_t_B_L_REVERT(tlv_format_t *buf)
{
	buf->data_len = B_L_REVERT16(buf->data_len);
	buf->complement_len = B_L_REVERT16(buf->complement_len);
	buf->crc = B_L_REVERT16(buf->crc);
	buf->seq_num = B_L_REVERT16(buf->seq_num);
	buf->flag = B_L_REVERT16(buf->flag);
}

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
unsigned short g_tlv_seqnum = 0;
int pack_tlv(char *buf,unsigned short buf_len, tlv_cmd_info_t* p_cmd_info)
{
	tlv_format_t *tlv_ptr = (tlv_format_t *)buf;
	if(buf_len < p_cmd_info->data_len + sizeof(tlv_format_t))
	{
		return -1;
	}
	tlv_ptr->flag = p_cmd_info->cmd_id;
	tlv_ptr->data_len = p_cmd_info->data_len + 8;
	tlv_ptr->complement_len = 0xFFFF - tlv_ptr->data_len;
	osCoreEnterCritical();
	tlv_ptr->seq_num = g_tlv_seqnum;
	g_tlv_seqnum++;
	tlv_ptr->ver_num = 1;
	tlv_ptr->data_type = p_cmd_info->data_type;
	osCoreExitCritical();

	if(p_cmd_info->data_buf != NULL)
	{
		if(p_cmd_info->data_type & TLV_DATA_SEC_MASK)
		{
			//RC4
			memcpy(tlv_ptr->data_buf,p_cmd_info->data_buf,p_cmd_info->data_len);
		}
		else
		{
			memcpy(tlv_ptr->data_buf,p_cmd_info->data_buf,p_cmd_info->data_len);
		}
	}



	tlv_ptr->seq_num = B_L_REVERT16(tlv_ptr->seq_num);
	tlv_ptr->flag = B_L_REVERT16(tlv_ptr->flag);

	tlv_ptr->crc = crc16_calc((uint8_t *)&(tlv_ptr->seq_num),((int32_t)tlv_ptr->data_len - 4));

	tlv_ptr->data_len = B_L_REVERT16(tlv_ptr->data_len);
	tlv_ptr->complement_len = B_L_REVERT16(tlv_ptr->complement_len);
	tlv_ptr->crc = B_L_REVERT16(tlv_ptr->crc);
//	tlv_format_t_B_L_REVERT(tlv_ptr);
	return (p_cmd_info->data_len + sizeof(tlv_format_t));

}

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
int  unpack_tlv(char *buf, unsigned short *buf_len, tlv_cmd_info_t* p_cmd_info)
{
	int ret = TLV_UNPACK_SUCCESS;
	char *tlv_ptr = buf;
	unsigned short len = *buf_len;
	unsigned short remain_len = len;
	tlv_format_t * tlv_ptr_tmp;
	unsigned short tmp_data_len = 0, tmp_complement_len = 0;

start_check:
	if(remain_len < TLV_MIN_SIZE)
	{
		ret = TLV_UNPACK_LEN_SHORT;
		goto end;
	}
	//******check head
	tmp_data_len = B_L_REVERT16(*(unsigned short *)tlv_ptr);
	tmp_complement_len = B_L_REVERT16(*((unsigned short *)tlv_ptr + 1));
	while((tmp_data_len != (0xFFFF - tmp_complement_len)) && (remain_len > 4))
	{
		tlv_ptr++;
		remain_len--;
		tmp_data_len = B_L_REVERT16(*(unsigned short *)tlv_ptr);
		tmp_complement_len = B_L_REVERT16(*((unsigned short *)tlv_ptr + 1));
	}
	if(remain_len < TLV_MIN_SIZE)
	{
		ret = TLV_UNPACK_LEN_SHORT;
		goto end;
	}
	//tlv unpacket start
	tlv_ptr_tmp = (tlv_format_t *)tlv_ptr;

	if(tmp_data_len > TLV_MAX_SIZE)
	{
		tlv_ptr++;
		remain_len--;
		//try to find next head, and unpacket again
		goto start_check;
	}

	if(remain_len < tmp_data_len + 2)	//2，data_len本身长度2个字节
	{
		ret = TLV_UNPACK_RCV_UNFINISHED;
		goto end;
	}

	if(tlv_ptr_tmp->crc != B_L_REVERT16(crc16_calc((uint8_t *)&(tlv_ptr_tmp->seq_num),((int32_t)tmp_data_len - 4))))
	{
		tlv_ptr++;
		remain_len--;
		//try to find next head, and unpacket again
		goto start_check;
	}
	//unpack success
	tlv_format_t_B_L_REVERT(tlv_ptr_tmp);
	p_cmd_info->cmd_id = tlv_ptr_tmp->flag;
	p_cmd_info->data_len = tlv_ptr_tmp->data_len-8;
	if(p_cmd_info->data_len)
		p_cmd_info->data_buf = (char *)xy_malloc(p_cmd_info->data_len);
	else
		p_cmd_info->data_buf = NULL;
	p_cmd_info->data_type = tlv_ptr_tmp->data_type;

	if(tlv_ptr_tmp->data_type&TLV_DATA_SEC_MASK)
	{
		//RC4
		memcpy(p_cmd_info->data_buf, tlv_ptr_tmp->data_buf, p_cmd_info->data_len);
	}
	else
		memcpy(p_cmd_info->data_buf, tlv_ptr_tmp->data_buf, p_cmd_info->data_len);
	remain_len -= (tlv_ptr_tmp->data_len + 2);

end:
	if(remain_len != len)
		memmove(buf, buf+len-remain_len, remain_len);
	*buf_len = remain_len;
	return ret;



}
