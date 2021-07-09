#include "bms_slave_init.h"
#include "bms_slave_api.h"
#include "tlv_protocol.h"
#include "bms_msg_info.h"



/**
  * @brief  用于从机发送response数据给主机
  * @attention  因为从机不能产生时钟，所以需要主机提供时钟信号。因此BMS需要先拉高中断信号线，NB接受到此中断信号后，认为BMS有数据要发送，因此产生时钟去驱动从机数据传输。 \n
  *	等到从机数据传输完毕后，从机再拉低中断信号，表示结束此次传输，主机不再产生时钟信号。
  */
void  spi_slave_response_data(void *data,int len)
{
	volatile uint32_t delay;

	//拉高中断信号线,使master发送时钟
	HAL_GPIO_WritePin(BMS_SLAVE_OUT_MSG_PIN, HAL_GPIO_PIN_SET);
	for(delay = 0; delay<10; delay++);

	osCoreEnterCritical();

	HAL_SPI_Transmit(&bms_spi_slave, (uint8_t *)data, len, 0);

	osCoreExitCritical();

	HAL_SPI_ENABLE_IT(&bms_spi_slave, HAL_SPI_IT_TX_NOT_FULL);
}

/**
  * @brief  用于从机发送burst数据给主机
  * @attention  因为从机不能产生时钟，所以需要主机提供时钟信号。因此BMS需要先拉高中断信号线，NB接受到此中断信号后，认为BMS有数据要发送，因此产生时钟去驱动从机数据传输。 \n
  *	等到从机数据传输完毕后，从机再拉低中断信号，表示结束此次传输，主机不再产生时钟信号。
  */
void  spi_slave_burst_data(void *data,int len)
{
	volatile uint32_t delay;

	//拉高WAKEUP PIN，唤醒深睡的master
	HAL_GPIO_WritePin(BMS_SLAVE_WAKEUP_PIN, HAL_GPIO_PIN_SET);
	for(delay = 0; delay<10; delay++);

	//拉高中断信号线,使master发送时钟
	HAL_GPIO_WritePin(BMS_SLAVE_OUT_MSG_PIN, HAL_GPIO_PIN_SET);
	for(delay = 0; delay<10; delay++);

	HAL_GPIO_WritePin(BMS_SLAVE_WAKEUP_PIN, HAL_GPIO_PIN_RESET);
	for(delay = 0; delay<10; delay++);

	osCoreEnterCritical();

	HAL_SPI_Transmit(&bms_spi_slave, (uint8_t *)data, len, 0);

	osCoreExitCritical();
	
	HAL_SPI_ENABLE_IT(&bms_spi_slave, HAL_SPI_IT_TX_NOT_FULL);
}


/*****************************************************************************************/
/**
  * @func   void bms_slave_response(tlv_cmd_info_t *p_cmd_info)
  * @brief  bms从机打包并发送response数据，拉高gpio以产生中断信号给nb主机
  * @param  p_cmd_info            tlv命令结构体
  * @retval none
  */
/*****************************************************************************************/
void bms_slave_response(tlv_cmd_info_t *p_cmd_info)
{
	uint16_t buf_len = p_cmd_info->data_len + sizeof(tlv_format_t);

	char *p_tlv_buf = xy_malloc(buf_len);

	pack_tlv(p_tlv_buf, buf_len, p_cmd_info);

	spi_slave_response_data((void *)p_tlv_buf,buf_len);

	xy_free((void *)p_tlv_buf);
}


/*****************************************************************************************/
/**
  * @func   void bms_slave_burst(tlv_cmd_info_t *p_cmd_info)
  * @brief  bms从机打包并发送burst数据，拉高gpio以产生中断信号给nb主机
  * @param  p_cmd_info            tlv命令结构体
  * @retval none
  */
/*****************************************************************************************/
void bms_slave_burst(tlv_cmd_info_t *p_cmd_info)
{
	uint16_t buf_len = p_cmd_info->data_len + sizeof(tlv_format_t);

	char *p_tlv_buf = xy_malloc(buf_len);

	pack_tlv(p_tlv_buf, buf_len, p_cmd_info);

	spi_slave_burst_data((void *)p_tlv_buf,buf_len);

	xy_free((void *)p_tlv_buf);
}



/*****************************************************************************************/
/**
  * @func   uint16_t bms_slave_recv_data(uint8_t *buf)
  * @brief  bms从机接收（主机发送过来的）数据
  * @param  buf            接收（主机）数据
  * @retval read_len       接收到的（主机）数据长度
  */
/*****************************************************************************************/
uint16_t bms_slave_recv_data(uint8_t *buf)
{
	uint16_t read_len = 0;

	while(HAL_SPI_Get_RxFIFO_Flag(&bms_spi_slave, HAL_SPI_RXFIFO_RX_EMPTY) == 0)
	{
		HAL_SPI_Receive(&bms_spi_slave, buf + read_len, 1, 0);
		read_len++;
	}

	return read_len;
}
