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
osThreadId_t  g_spi_slave_test_TskHandle = NULL;
HAL_SPI_HandleTypeDef spi_slave;

/**
 * @brief 初始化函数
 * @code
 *	HAL_GPIO_InitTypeDef spi_slave_gpio;

	//SPI参数配置
	spi_slave.Instance = HAL_SPI;
	spi_slave.Init.Mode = HAL_SPI_MODE_SLAVE;
	spi_slave.Init.WorkMode = HAL_SPI_WORKMODE_0;
	spi_slave.Init.DataSize = HAL_SPI_DATASIZE_8BIT;

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
	if(HAL_SPI_Init(&spi_slave) != HAL_OK)
	{
		send_debug_str_to_at_uart("\r\nerror\r\n");
	}
	else
	{
		send_debug_str_to_at_uart("\r\nnormal\r\n");
	}
 * @endcode
 */
void spi_slave_test_init(void)
{
	HAL_GPIO_InitTypeDef spi_slave_gpio;

	//SPI参数配置
	spi_slave.Instance = HAL_SPI;
	spi_slave.Init.Mode = HAL_SPI_MODE_SLAVE;
	spi_slave.Init.WorkMode = HAL_SPI_WORKMODE_0;
	spi_slave.Init.DataSize = HAL_SPI_DATASIZE_8BIT;

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
	if(HAL_SPI_Init(&spi_slave) != HAL_OK)
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
void spi_slave_test_task(void)
{
	uint8_t data[9] = {'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 0};
	while(1)
	{
		//接收8个字节，超时时间设置为HAL_MAX_DELAY，即一直等待到8个字节接收完成
		HAL_SPI_Receive(&spi_slave, data, 8, HAL_MAX_DELAY);
		//发送8个字节，超时时间设置为HAL_MAX_DELAY，即一直等待到8个字节发送完成
		HAL_SPI_Transmit(&spi_slave, data, 8, HAL_MAX_DELAY);
		//打印接收到的字符，用于调试
		send_debug_str_to_at_uart((char *)data);
		//通过osDelay释放线程控制权
		osDelay(1000);	
	}
}

/**
 * @brief 任务创建
 *
 */
void spi_slave_test_task_init(void)
{
	g_spi_slave_test_TskHandle = osThreadNew((osThreadFunc_t)spi_slave_test_task, NULL, "spi_slave_test_task", 512, osPriorityNormal);
}


#endif
