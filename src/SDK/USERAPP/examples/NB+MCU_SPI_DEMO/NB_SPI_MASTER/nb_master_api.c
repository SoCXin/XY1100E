#include "nb_master_init.h"
#include "nb_master_api.h"
#include "tlv_protocol.h"
#include "bms_msg_info.h"



#include "portmacro.h"



/*****************************************************************************************/
/**
  * @func   void nb_master_send_request_ack(tlv_cmd_info_t *p_cmd_info)
  * @brief  nb主机发送Request应答
  * @param  p_cmd_info           tlv命令结构体
  * @retval none
  */
/*****************************************************************************************/
void nb_master_send_request_ack(tlv_cmd_info_t *p_cmd_info)
{
    osMutexAcquire(nb_master_Send_Mutex, portMAX_DELAY);

	uint16_t buf_len = p_cmd_info->data_len + sizeof(tlv_format_t);

	char *p_tlv_buf = xy_malloc(buf_len);

	pack_tlv(p_tlv_buf, buf_len, p_cmd_info);

	HAL_SPI_ChipSelect(&nb_spi_master, HAL_SPI_CS_PIN_1);

	HAL_SPI_Transmit(&nb_spi_master, (uint8_t *)p_tlv_buf, buf_len, 0);

	xy_free((void *)p_tlv_buf);

	while(HAL_SPI_Get_TxFIFO_Flag(&nb_spi_master, HAL_SPI_TXFIFO_TX_EMPTY) == 0);

	HAL_SPI_ChipSelect(&nb_spi_master, HAL_SPI_CS_NONE);

    osMutexRelease(nb_master_Send_Mutex);
}

/*****************************************************************************************/
/**
  * @func   static void nb_master_send_invalide_data(uint16_t length)
  * @brief  nb主机发送无效数据给从机提供时钟，用于接收从机数据
  * @param  length            发送数据长度
  * @retval none
  */
/*****************************************************************************************/
static void nb_master_send_invalide_data(uint16_t length)
{
	uint32_t invalid_data = 0;
	uint32_t size_of_word = length / 4;
	uint32_t size_of_byte = length % 4;
	uint32_t i;

	if(length > 0)
	{
		osMutexAcquire(nb_master_Send_Mutex, portMAX_DELAY);

		HAL_SPI_ChipSelect(&nb_spi_master, HAL_SPI_CS_PIN_1);

		for(i=0; i<size_of_word; i++)
		{
			HAL_SPI_Transmit(&nb_spi_master, (uint8_t *)&invalid_data, 4, 0);
		}

		if(size_of_byte != 0)
		{
			HAL_SPI_Transmit(&nb_spi_master, (uint8_t *)&invalid_data, size_of_byte, 0);
		}

		//wait for TX FIFO empty
		while(HAL_SPI_Get_TxFIFO_Flag(&nb_spi_master, HAL_SPI_TXFIFO_TX_EMPTY) == 0);

		HAL_SPI_ChipSelect(&nb_spi_master, HAL_SPI_CS_NONE);

		osMutexRelease(nb_master_Send_Mutex);
	}
}

/*****************************************************************************************/
/**
  * @func   uint16_t nb_master_recv_data(uint8_t *buf)
  * @brief  nb主机接收数据线程
  * @param  buf           接收到的数据
  * @retval read_len      接收到的数据长度
  */
/*****************************************************************************************/
uint16_t nb_master_recv_data(uint8_t *buf)
{
	uint16_t read_len = 0;

	nb_master_send_invalide_data(1);

	while(HAL_SPI_Get_RxFIFO_Flag(&nb_spi_master, HAL_SPI_RXFIFO_RX_EMPTY) == 0)
	{
		HAL_SPI_Receive(&nb_spi_master, buf + read_len, 1, 0);
		read_len++;
	}

	return read_len;
}
