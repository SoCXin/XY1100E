
/** 
* @file        bms_slave_init.c
* @brief       该文件用于实现SPI做从机时的数据收发处理，用户可以任意制定各种消息传输规则
* @attention   用户不得直接在该文件中进行修改，仅提供参考！\n
*    必须设计好软看门狗机制,以容错通信异常！！！
*
*/
#include "tlv_protocol.h"
#include "bms_slave_init.h"
#include "bms_slave_api.h"

#include "hal_gpio.h"
#include "hal_spi.h"

#include <string.h>
#include "osal.h"
#include "xy_system.h"

osThreadId_t  bms_slave_recv_TskHander = NULL;
osThreadId_t  bms_slave_proc_TskHander = NULL;
osMessageQueueId_t bms_slave_proc_Queue = NULL;
osSemaphoreId_t bms_slave_Semaphore = NULL;

HAL_SPI_HandleTypeDef bms_spi_slave;


//函数声明，中断处理函数需放在ram上
static void bms_slave_spi_int_handle(void) __attribute__((section(".ramtext.bms_slave_spi_int_handle")));
static void bms_slave_rcv_task(void);

/*****************************************************************************************/
/**
  * @func   void bms_slave_spi_init(void)
  * @brief  bms从机spi硬件初始化
  * @attention 用户使用时，需要在user_peripheral_init中调用该外设初始化
  * @warning   从机需设置TXFIFO深度为1，配合SPI_INTTYPE_TX_FIFO_NF中断，从而判断从机发送完成。
  */
/*****************************************************************************************/
void bms_slave_spi_init(void)
{
	//如果NV中用到了此外设相关的GPIO,使用此外设期间需关闭standby,否则使用有异常
	//用户需根据项目实际情况在合适位置再开启standby,降低功耗
	xy_standby_lock();
	
	HAL_GPIO_InitTypeDef spi_slave_gpio;

	spi_slave_gpio.Pin = BMS_SLAVE_SPI_CS_PIN;
	spi_slave_gpio.Mode = GPIO_MODE_AF_PER;
	spi_slave_gpio.Pull = GPIO_NOPULL;
	spi_slave_gpio.Alternate = HAL_REMAP_SPI_CS;
	HAL_GPIO_Init(&spi_slave_gpio);

	spi_slave_gpio.Pin = BMS_SLAVE_SPI_MOSI_PIN;
	spi_slave_gpio.Mode = GPIO_MODE_AF_PER;
	spi_slave_gpio.Pull = GPIO_NOPULL;
	spi_slave_gpio.Alternate = HAL_REMAP_SPI_MOSI;
	HAL_GPIO_Init(&spi_slave_gpio);

	spi_slave_gpio.Pin = BMS_SLAVE_SPI_MISO_PIN;
	spi_slave_gpio.Mode = GPIO_MODE_AF_PER;
	spi_slave_gpio.Pull = GPIO_NOPULL;
	spi_slave_gpio.Alternate = HAL_REMAP_SPI_MISO;
	HAL_GPIO_Init(&spi_slave_gpio);

	spi_slave_gpio.Pin = BMS_SLAVE_SPI_SCLK_PIN;
	spi_slave_gpio.Mode = GPIO_MODE_AF_PER;
	spi_slave_gpio.Pull = GPIO_NOPULL;
	spi_slave_gpio.Alternate = HAL_REMAP_SPI_SCLK;
	HAL_GPIO_Init(&spi_slave_gpio);

	spi_slave_gpio.Pin	= BMS_SLAVE_OUT_MSG_PIN;
	spi_slave_gpio.Mode	= GPIO_MODE_OUTPUT_PP;
	spi_slave_gpio.Pull	= GPIO_NOPULL;
	HAL_GPIO_Init(&spi_slave_gpio);
	HAL_GPIO_WritePin(BMS_SLAVE_OUT_MSG_PIN, HAL_GPIO_PIN_RESET);

	spi_slave_gpio.Pin	= BMS_SLAVE_WAKEUP_PIN;
	spi_slave_gpio.Mode	= GPIO_MODE_OUTPUT_PP;
	spi_slave_gpio.Pull	= GPIO_NOPULL;
	HAL_GPIO_Init(&spi_slave_gpio);
	HAL_GPIO_WritePin(BMS_SLAVE_WAKEUP_PIN, HAL_GPIO_PIN_RESET);

	bms_spi_slave.Instance = HAL_SPI;
	bms_spi_slave.Init.Mode = HAL_SPI_MODE_SLAVE;
	bms_spi_slave.Init.WorkMode = HAL_SPI_WORKMODE_0;
	bms_spi_slave.Init.DataSize = HAL_SPI_DATASIZE_8BIT;
	HAL_SPI_Init(&bms_spi_slave);

	HAL_SPI_SET_TXFIFO_THD(&bms_spi_slave, 1);
	HAL_SPI_SET_RXFIFO_THD(&bms_spi_slave, 1);
	bms_slave_Semaphore = osSemaphoreNew(0xFFFF, 0);
	HAL_NVIC_IntRegister(HAL_SPI1_IRQn,bms_slave_spi_int_handle, 1);
	HAL_SPI_ENABLE_IT(&bms_spi_slave, HAL_SPI_IT_RX_NOT_EMPTY);
}

/*****************************************************************************************/
/**
  * @func   void bms_slave_task_init(void)
  * @brief  bms从机任务初始化
  * @attention  用户使用时，需要在user_task_init中调用该任务的初始化入口
  */
/*****************************************************************************************/
void bms_slave_task_init(void)
{
	bms_slave_recv_TskHander = osThreadNew((osThreadFunc_t)bms_slave_rcv_task, NULL, "bms_recv", BMS_SLAVE_RECV_TASK_STACK_SIZE, 12);

	bms_slave_proc_TskHander = osThreadNew((osThreadFunc_t)bms_slave_proc_task, NULL, "bms_proc", BMS_SLAVE_PROC_TASK_STACK_SIZE, 11);

	bms_slave_proc_Queue = osMessageQueueNew(10, sizeof(tlv_cmd_info_t));

	extern int at_slave_burst_req(char *at_buf, char **prsp_cmd);
	register_app_at_req("AT+BURST", at_slave_burst_req);

//	void bms_slave_test(void);
//	osThreadId_t  bms_test_TskHander = NULL;
//	bms_test_TskHander = osThreadNew((osThreadFunc_t)bms_slave_test, NULL, "bms_test", 1024, 10);
}

/*****************************************************************************************/
/**
  * @func   static void bms_slave_spi_int_handle(void)
  * @brief  bms从机spi接收和发送中断处理函数    \n
  *			SPI_INTTYPE_RX_FIFO_NE：spi接收FIFO非空中断；SPI_INTTYPE_TX_FIFO_NF：spi发送FIFO非满中断
  * @attention  因为从机不能产生时钟，所以发送时需要主机提供时钟信号。因此BMS需要先拉高中断信号线，NB接受到此中断信号后，认为BMS有数据要发送，因此产生时钟去驱动从机数据传输。 \n
  * 等到从机数据传输完毕后(即SPI_INTTYPE_TX_FIFO_NF为空)，从机再拉低中断信号，表示结束此次传输，主机不再产生时钟信号。
  */
/*****************************************************************************************/
void bms_slave_spi_int_handle(void)
{
	uint32_t ITState = HAL_SPI_GetITState(&bms_spi_slave);
	uint32_t ITEnable = HAL_SPI_GetITEnable(&bms_spi_slave);

	if((HAL_RESET != (ITEnable & HAL_SPI_IT_RX_NOT_EMPTY)) && (HAL_RESET != (ITState & HAL_SPI_IT_RX_NOT_EMPTY)))
	{
		HAL_SPI_DISABLE_IT(&bms_spi_slave, HAL_SPI_IT_RX_NOT_EMPTY);
		osSemaphoreRelease(bms_slave_Semaphore);
	}

	if((HAL_RESET != (ITEnable & HAL_SPI_IT_TX_NOT_FULL)) && (HAL_RESET != (ITState & HAL_SPI_IT_TX_NOT_FULL)))
	{
		HAL_SPI_DISABLE_IT(&bms_spi_slave, HAL_SPI_IT_TX_NOT_FULL);
		HAL_GPIO_WritePin(BMS_SLAVE_OUT_MSG_PIN, HAL_GPIO_PIN_RESET);
	}
}

/*****************************************************************************************/
/**
  * @func   static void bms_slave_rcv_task(void)
  * @brief  底板MCU(BMS)从机接收处理线程，接收到从机gpio中断信号量后开始接收主机发来的数据 \n
  *  接收到的数据放在临时缓存中，并按照协议规则进行完整性解析，解析出完整的一条消息后，发送给业务线程进行处理
  */
/*****************************************************************************************/
static void bms_slave_rcv_task(void)
{
	uint8_t rcv_buf[256];
	uint16_t rcv_len = 0;
	tlv_cmd_info_t cmd_info;
	int ret;

	while(1)
	{
		osSemaphoreAcquire(bms_slave_Semaphore, osWaitForever);

		rcv_len += bms_slave_recv_data(&rcv_buf[rcv_len]);

		ret = unpack_tlv((char *)rcv_buf, &rcv_len, &cmd_info);

		if(ret == TLV_UNPACK_SUCCESS)
		{
			osMessageQueuePut(bms_slave_proc_Queue, &cmd_info, 0, osWaitForever);
		}

		HAL_SPI_ENABLE_IT(&bms_spi_slave, HAL_SPI_IT_RX_NOT_EMPTY);
	}
}
