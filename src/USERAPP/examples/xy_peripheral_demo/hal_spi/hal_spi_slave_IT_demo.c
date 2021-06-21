/**
* @file		hal_spi_slave_demo.c
* @ingroup	peripheral
* @brief	SPI从设备工作模式Demo
* @warning	用户使用外设时需注意：1.使用前关闭standby，使用完开启standby 2.确保没有在NV中配置外设所使用的GPIO，否则芯片进出standby时会根据NV重配GPIO，导致外设无法正常使用
***********************************************************************************/

#if DEMO_TEST


#include "xy_api.h"

//任务参数配置
#define SPI_MOSI_PIN        HAL_GPIO_PIN_NUM_6
#define SPI_MISO_PIN        HAL_GPIO_PIN_NUM_7
#define SPI_SCLK_PIN        HAL_GPIO_PIN_NUM_8
#define SPI_CS_PIN          HAL_GPIO_PIN_NUM_9

//任务全局变量
osThreadId_t  g_spi_slave_IT_test_TskHandle = NULL;
osSemaphoreId_t  g_spi_slave_IT_test_sem = NULL;

HAL_SPI_HandleTypeDef spi_slave_IT;

/**
 * @brief 错误中断回调函数。注意：1.用户在使用时需将__weak 删除！！！2.用户需保证中断及中断里调用的函数都在ram上！！！
 */
void HAL_SPI_ErrorCallback(HAL_SPI_HandleTypeDef *SPIHandleStruct) __RAM_FUNC;
__weak void HAL_SPI_ErrorCallback(HAL_SPI_HandleTypeDef *SPIHandleStruct)
{
	//用户根据实际需求添加错误处理代码

	//错误处理完后恢复ErrorCode
	SPIHandleStruct->ErrorCode = HAL_SPI_ERROR_NONE;
}

/**
 * @brief 中断回调函数。注意：1.用户在使用时需将__weak 删除！！！2.用户需保证中断及中断里调用的函数都在ram上！！！
 */
void HAL_SPI_RxCpltCallback(HAL_SPI_HandleTypeDef *SPIHandleStruct) __RAM_FUNC;
__weak void HAL_SPI_RxCpltCallback(HAL_SPI_HandleTypeDef *SPIHandleStruct)
{
	UNUSED(SPIHandleStruct);
	osSemaphoreRelease(g_spi_slave_IT_test_sem);
}

/**
 * @brief 中断服务函数。注意：1.用户在使用时需将__weak 删除！！！2.用户需保证中断及中断里调用的函数都在ram上！！！
 */
void HAL_SPI_IRQHandler(void) __RAM_FUNC;
__weak void HAL_SPI_IRQHandler(void)
{
	SPI_IRQHandler(&spi_slave_IT);
}

/**
 * @brief 初始化函数
 * @code
 *	HAL_GPIO_InitTypeDef spi_slave_gpio;

	//SPI参数配置
	spi_slave_IT.Instance = HAL_SPI;
	spi_slave_IT.Init.Mode = HAL_SPI_MODE_SLAVE;
	spi_slave_IT.Init.WorkMode = HAL_SPI_WORKMODE_0;
	spi_slave_IT.Init.DataSize = HAL_SPI_DATASIZE_8BIT;

	//映射SPI的MOSI引脚
	spi_slave_gpio.Pin = SPI_MOSI_PIN;
	spi_slave_gpio.Mode = GPIO_MODE_AF_PER;
	spi_slave_gpio.Pull = GPIO_NOPULL;
	spi_slave_gpio.Alternate = HAL_REMAP_SPI_MOSI;
	HAL_GPIO_Init(&spi_slave_gpio);

	//映射SPI的MISO引脚
	spi_slave_gpio.Pin = SPI_MISO_PIN;
	spi_slave_gpio.Mode = GPIO_MODE_AF_PER;
	spi_slave_gpio.Pull = GPIO_NOPULL;
	spi_slave_gpio.Alternate = HAL_REMAP_SPI_MISO;
	HAL_GPIO_Init(&spi_slave_gpio);

	//映射SPI的SCLK引脚
	spi_slave_gpio.Pin = SPI_SCLK_PIN;
	spi_slave_gpio.Mode = GPIO_MODE_AF_PER;
	spi_slave_gpio.Pull = GPIO_NOPULL;
	spi_slave_gpio.Alternate = HAL_REMAP_SPI_SCLK;
	HAL_GPIO_Init(&spi_slave_gpio);

	//映射SPI的CS引脚
	spi_slave_gpio.Pin = SPI_CS_PIN;
	spi_slave_gpio.Mode = GPIO_MODE_AF_PER;
	spi_slave_gpio.Pull = GPIO_NOPULL;
	spi_slave_gpio.Alternate = HAL_REMAP_SPI_CS;
	HAL_GPIO_Init(&spi_slave_gpio);

	//初始化SPI
	if(HAL_SPI_Init(&spi_slave_IT) != HAL_OK)
	{
		send_debug_str_to_at_uart("\r\nerror\r\n");
	}
	else
	{
		send_debug_str_to_at_uart("\r\nnormal\r\n");
	}
 * @endcode
 */
void spi_slave_IT_test_init(void)
{
	//创建信号量
	g_spi_slave_IT_test_sem = osSemaphoreNew(0xFFFF, 0);

	HAL_GPIO_InitTypeDef spi_slave_gpio;

	//SPI参数配置
	spi_slave_IT.Instance = HAL_SPI;
	spi_slave_IT.Init.Mode = HAL_SPI_MODE_SLAVE;
	spi_slave_IT.Init.WorkMode = HAL_SPI_WORKMODE_0;
	spi_slave_IT.Init.DataSize = HAL_SPI_DATASIZE_8BIT;

	//映射SPI的MOSI引脚
	spi_slave_gpio.Pin = SPI_MOSI_PIN;
	spi_slave_gpio.Mode = GPIO_MODE_AF_PER;
	spi_slave_gpio.Pull = GPIO_NOPULL;
	spi_slave_gpio.Alternate = HAL_REMAP_SPI_MOSI;
	HAL_GPIO_Init(&spi_slave_gpio);

	//映射SPI的MISO引脚
	spi_slave_gpio.Pin = SPI_MISO_PIN;
	spi_slave_gpio.Mode = GPIO_MODE_AF_PER;
	spi_slave_gpio.Pull = GPIO_NOPULL;
	spi_slave_gpio.Alternate = HAL_REMAP_SPI_MISO;
	HAL_GPIO_Init(&spi_slave_gpio);

	//映射SPI的SCLK引脚
	spi_slave_gpio.Pin = SPI_SCLK_PIN;
	spi_slave_gpio.Mode = GPIO_MODE_AF_PER;
	spi_slave_gpio.Pull = GPIO_NOPULL;
	spi_slave_gpio.Alternate = HAL_REMAP_SPI_SCLK;
	HAL_GPIO_Init(&spi_slave_gpio);

	//映射SPI的CS引脚
	spi_slave_gpio.Pin = SPI_CS_PIN;
	spi_slave_gpio.Mode = GPIO_MODE_AF_PER;
	spi_slave_gpio.Pull = GPIO_NOPULL;
	spi_slave_gpio.Alternate = HAL_REMAP_SPI_CS;
	HAL_GPIO_Init(&spi_slave_gpio);

	//初始化SPI
	if(HAL_SPI_Init(&spi_slave_IT) != HAL_OK)
	{
		send_debug_str_to_at_uart("\r\nerror\r\n");
	}
	else
	{
		send_debug_str_to_at_uart("\r\nnormal\r\n");
	}
}

/**
 * @brief 任务线程
 *
 */
void spi_slave_IT_test_task(void)
{
	uint8_t data[9] = {'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 0};
	while(1)
	{
		HAL_SPI_Transmit(&spi_slave_IT, data, 8, 500);
		//中断接收8个字节，等待中断接收完成释放信号量
		HAL_SPI_Receive_IT(&spi_slave_IT, data, 8);
		//等待获取信号量，释放线程控制权
		osSemaphoreAcquire(g_spi_slave_IT_test_sem, osWaitForever);	
		//发送接收到的8个字节，设置超时时间为500ms
		HAL_SPI_Transmit(&spi_slave_IT, data, 8, 500);
		//打印接收到的字符，用于调试
		send_debug_str_to_at_uart((char *)data);
	}
}

/**
 * @brief 任务创建
 *
 */
void spi_slave_IT_test_task_init(void)
{
	g_spi_slave_IT_test_TskHandle = osThreadNew((osThreadFunc_t)spi_slave_IT_test_task, NULL, "spi_slave_IT_test_task", 512, osPriorityNormal);
}


#endif
