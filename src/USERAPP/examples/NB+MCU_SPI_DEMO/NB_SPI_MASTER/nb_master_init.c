
/** 
* @file        nb_master_init.c
* @brief       该文件用于实现SPI做主机时的数据收发处理，用户可以任意制定各种消息传输规则
* @attention   用户不得直接在该文件中进行修改，仅提供参考！\n
*    必须设计好软看门狗机制,以容错通信异常！！！
*
*/


#include "nb_master_init.h"
#include "nb_master_api.h"
#include "tlv_protocol.h"

#include "hal_gpio.h"
#include "hal_spi.h"
#include "stddef.h"
#include "osal.h"
#include "xy_system.h"

osThreadId_t  nb_master_recv_TskHander = NULL;
osThreadId_t  nb_master_proc_TskHander = NULL;

osMessageQueueId_t nb_master_proc_Queue = NULL;
osSemaphoreId_t nb_master_Semaphore = NULL;
osMutexId_t nb_master_Send_Mutex = NULL;

HAL_SPI_HandleTypeDef nb_spi_master;

//函数声明，中断处理函数需放在ram上
static void nb_master_gpio_int_handler(void) __attribute__((section(".ramtext.nb_master_gpio_int_handler")));
static void nb_master_recv_task(void);

/*****************************************************************************************/
/**
  * @func   void nb_master_spi_init(void)
  * @brief  nb主机spi硬件初始化
  * @attention 用户使用时，需要在user_peripheral_init中调用该外设初始化
  */
/*****************************************************************************************/
void nb_master_spi_init(void)
{
	//如果NV中用到了此外设相关的GPIO,使用此外设期间需关闭standby,否则使用有异常
	//用户需根据项目实际情况在合适位置再开启standby,降低功耗
	xy_standby_lock();
	
	nb_master_Semaphore = osSemaphoreNew(0xFFFF, 0);
	HAL_NVIC_IntRegister(HAL_GPIO_IRQn, nb_master_gpio_int_handler, 1);

	HAL_GPIO_InitTypeDef spi_master_gpio;

	spi_master_gpio.Pin = NB_MASTER_SPI_MOSI_PIN;
	spi_master_gpio.Mode = GPIO_MODE_AF_PER;
	spi_master_gpio.Pull = GPIO_NOPULL;
	spi_master_gpio.Alternate = HAL_REMAP_SPI_MOSI;
	HAL_GPIO_Init(&spi_master_gpio);

	spi_master_gpio.Pin = NB_MASTER_SPI_SCLK_PIN;
	spi_master_gpio.Mode = GPIO_MODE_AF_PER;
	spi_master_gpio.Pull = GPIO_NOPULL;
	spi_master_gpio.Alternate = HAL_REMAP_SPI_SCLK;
	HAL_GPIO_Init(&spi_master_gpio);

	spi_master_gpio.Pin = NB_MASTER_SPI_MISO_PIN;
	spi_master_gpio.Mode = GPIO_MODE_AF_PER;
	spi_master_gpio.Pull = GPIO_NOPULL;
	spi_master_gpio.Alternate = HAL_REMAP_SPI_MISO;
	HAL_GPIO_Init(&spi_master_gpio);

	spi_master_gpio.Pin = NB_MASTER_SPI_CS_PIN;
	spi_master_gpio.Mode = GPIO_MODE_AF_PER;
	spi_master_gpio.Pull = GPIO_NOPULL;
	spi_master_gpio.Alternate = HAL_REMAP_SPI_NSS1;
	HAL_GPIO_Init(&spi_master_gpio);

	spi_master_gpio.Pin		= NB_MASTER_IN_MSG_PIN;
	spi_master_gpio.Mode	= GPIO_MODE_IT_HIGH_LEVEL;
	spi_master_gpio.Pull	= GPIO_PULLUP;
	HAL_GPIO_Init(&spi_master_gpio);

	nb_spi_master.Instance = HAL_SPI;
	nb_spi_master.Init.Mode = HAL_SPI_MODE_MASTER;
	nb_spi_master.Init.WorkMode = HAL_SPI_WORKMODE_0;
	nb_spi_master.Init.DataSize = HAL_SPI_DATASIZE_8BIT;
	nb_spi_master.Init.SPI_CLK_DIV = HAL_SPI_CLKDIV_32;
	nb_spi_master.Init.FirstBit = HAL_SPI_FIRSTBIT_MSB;
	nb_spi_master.Init.NSS = HAL_SPI_NSS_SOFT;
	HAL_SPI_Init(&nb_spi_master);
}

/*****************************************************************************************/
/**
  * @func   void bms_slave_task_init(void)
  * @brief  nb主机任务初始化
  * @attention  用户使用时，需要在user_task_init中调用该任务的初始化入口
  */
/*****************************************************************************************/
void nb_master_task_init(void)
{
	nb_master_recv_TskHander = osThreadNew((osThreadFunc_t)nb_master_recv_task, NULL, "nb_recv", 2048, 12);

	nb_master_proc_TskHander = osThreadNew((osThreadFunc_t)nb_master_proc_task, NULL, "nb_proc", 2048, 11);

	nb_master_proc_Queue = osMessageQueueNew(10, sizeof(tlv_cmd_info_t));

	nb_master_Send_Mutex = osMutexNew(osMutexPrioInherit);

	void nb_master_test(void);
	osThreadNew((osThreadFunc_t)nb_master_test, NULL, "nb_master_test", 2048, 10);
}

/*****************************************************************************************/
/**
  * @func   static void nb_master_gpio_int_handler(void)
  * @brief  nb主机gpio中断处理函数，用以接收从机发来的gpio中断，释放信号量以让接收线程处理
  * @param  none
  * @retval none
  */
/*****************************************************************************************/
void nb_master_gpio_int_handler(void)
{
	uint32_t sta = HAL_GPIO_ReadAndClearIntFlag(NB_MASTER_IN_MSG_PIN);
	HAL_GPIO_IntCtl(NB_MASTER_IN_MSG_PIN, DISABLE);

	if(sta & (1 << NB_MASTER_IN_MSG_PIN))
	{
		while(osOK == osSemaphoreAcquire(nb_master_Semaphore, osNoWait));
		osSemaphoreRelease(nb_master_Semaphore);
	}
}



/*****************************************************************************************/
/**
  * @func   static void nb_master_recv_task(void)
  * @brief  nb主机接收处理线程，接收到从机gpio中断信号量后开始接收从机发来的数据（主机发送时钟）\n
  *  接收到的数据放在临时缓存中，并按照协议规则进行完整性解析，解析出完整的一条消息后，发送给业务线程进行处理
  */
/*****************************************************************************************/
static void nb_master_recv_task(void)
{
	uint8_t rcv_buf[256];
	uint16_t rcv_len = 0;
	tlv_cmd_info_t cmd_info;
	int ret;

	while(1)
	{
		osSemaphoreAcquire(nb_master_Semaphore, osWaitForever);

		rcv_len += nb_master_recv_data(&rcv_buf[rcv_len]);

		ret = (uint16_t) unpack_tlv((char *)rcv_buf, &rcv_len, &cmd_info);

		if(ret == TLV_UNPACK_SUCCESS)
		{
			osMessageQueuePut(nb_master_proc_Queue, &cmd_info, 0, osWaitForever);
		}
		HAL_GPIO_IntCtl(NB_MASTER_IN_MSG_PIN, ENABLE);
	}
}
