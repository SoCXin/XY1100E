/**
* @file		hal_spi_master_demo.c
* @ingroup	peripheral
* @brief	SPI主设备工作模式Demo
* @warning	用户使用外设时需注意：1.使用前关闭standby，使用完开启standby 2.确保没有在NV中配置外设所使用的GPIO，否则芯片进出standby时会根据NV重配GPIO，导致外设无法正常使用
***********************************************************************************/

#if DEMO_TEST


#include "xy_api.h"

//任务参数配置
#define SPI_MOSI_PIN        HAL_GPIO_PIN_NUM_6
#define SPI_MISO_PIN        HAL_GPIO_PIN_NUM_7
#define SPI_SCLK_PIN        HAL_GPIO_PIN_NUM_8
#define SPI_NSS1_PIN        HAL_GPIO_PIN_NUM_9
#define SPI_NSS2_PIN        HAL_GPIO_PIN_NUM_20

//任务全局变量
osThreadId_t  g_spi_master_test_TskHandle = NULL;
HAL_SPI_HandleTypeDef spi_master;

/**
 * @brief 初始化函数
 * @code
 *	HAL_GPIO_InitTypeDef spi_master_gpio;

	//SPI参数配置
	spi_master.Instance = HAL_SPI;
	spi_master.Init.Mode = HAL_SPI_MODE_MASTER;
	spi_master.Init.WorkMode = HAL_SPI_WORKMODE_0;
	spi_master.Init.DataSize = HAL_SPI_DATASIZE_8BIT;
	spi_master.Init.SPI_CLK_DIV = HAL_SPI_CLKDIV_32;
	spi_master.Init.FirstBit = HAL_SPI_FIRSTBIT_MSB;
	spi_master.Init.NSS = HAL_SPI_NSS_SOFT;

	//映射SPI的MOSI引脚
	spi_master_gpio.Pin = SPI_MOSI_PIN;
	spi_master_gpio.Mode = GPIO_MODE_AF_PER;
	spi_master_gpio.Pull = GPIO_NOPULL;
	spi_master_gpio.Alternate = HAL_REMAP_SPI_MOSI;
	HAL_GPIO_Init(&spi_master_gpio);

	//映射SPI的MISO引脚
	spi_master_gpio.Pin = SPI_MISO_PIN;
	spi_master_gpio.Mode = GPIO_MODE_AF_PER;
	spi_master_gpio.Pull = GPIO_NOPULL;
	spi_master_gpio.Alternate = HAL_REMAP_SPI_MISO;
	HAL_GPIO_Init(&spi_master_gpio);

	//映射SPI的SCLK引脚
	spi_master_gpio.Pin = SPI_SCLK_PIN;
	spi_master_gpio.Mode = GPIO_MODE_AF_PER;
	spi_master_gpio.Pull = GPIO_NOPULL;
	spi_master_gpio.Alternate = HAL_REMAP_SPI_SCLK;
	HAL_GPIO_Init(&spi_master_gpio);

	//映射SPI的NSS1引脚
	spi_master_gpio.Pin = SPI_NSS1_PIN;
	spi_master_gpio.Mode = GPIO_MODE_AF_PER;
	spi_master_gpio.Pull = GPIO_NOPULL;
	spi_master_gpio.Alternate = HAL_REMAP_SPI_NSS1;
	HAL_GPIO_Init(&spi_master_gpio);

	//映射SPI的NSS2引脚
	spi_master_gpio.Pin = SPI_NSS2_PIN;
	spi_master_gpio.Mode = GPIO_MODE_AF_PER;
	spi_master_gpio.Pull = GPIO_NOPULL;
	spi_master_gpio.Alternate = HAL_REMAP_SPI_NSS2;
	HAL_GPIO_Init(&spi_master_gpio);

	//初始化SPI
	if(HAL_SPI_Init(&spi_master) != HAL_OK)
	{
		send_debug_str_to_at_uart("\r\nerror\r\n");
	}
	else
	{
		send_debug_str_to_at_uart("\r\nnormal\r\n");
	}
 * @endcode
 */

void spi_master_test_init(void)
{
	HAL_GPIO_InitTypeDef spi_master_gpio;

	//SPI参数配置
	spi_master.Instance = HAL_SPI;
	spi_master.Init.Mode = HAL_SPI_MODE_MASTER;
	spi_master.Init.WorkMode = HAL_SPI_WORKMODE_0;
	spi_master.Init.DataSize = HAL_SPI_DATASIZE_8BIT;
	spi_master.Init.SPI_CLK_DIV = HAL_SPI_CLKDIV_32;
	spi_master.Init.FirstBit = HAL_SPI_FIRSTBIT_MSB;
	spi_master.Init.NSS = HAL_SPI_NSS_SOFT;

	//映射SPI的MOSI引脚
	spi_master_gpio.Pin = SPI_MOSI_PIN;
	spi_master_gpio.Mode = GPIO_MODE_AF_PER;
	spi_master_gpio.Pull = GPIO_NOPULL;
	spi_master_gpio.Alternate = HAL_REMAP_SPI_MOSI;
	HAL_GPIO_Init(&spi_master_gpio);

	//映射SPI的MISO引脚
	spi_master_gpio.Pin = SPI_MISO_PIN;
	spi_master_gpio.Mode = GPIO_MODE_AF_PER;
	spi_master_gpio.Pull = GPIO_NOPULL;
	spi_master_gpio.Alternate = HAL_REMAP_SPI_MISO;
	HAL_GPIO_Init(&spi_master_gpio);

	//映射SPI的SCLK引脚
	spi_master_gpio.Pin = SPI_SCLK_PIN;
	spi_master_gpio.Mode = GPIO_MODE_AF_PER;
	spi_master_gpio.Pull = GPIO_NOPULL;
	spi_master_gpio.Alternate = HAL_REMAP_SPI_SCLK;
	HAL_GPIO_Init(&spi_master_gpio);

	//映射SPI的NSS1引脚
	spi_master_gpio.Pin = SPI_NSS1_PIN;
	spi_master_gpio.Mode = GPIO_MODE_AF_PER;
	spi_master_gpio.Pull = GPIO_NOPULL;
	spi_master_gpio.Alternate = HAL_REMAP_SPI_NSS1;
	HAL_GPIO_Init(&spi_master_gpio);

	//映射SPI的NSS2引脚
	spi_master_gpio.Pin = SPI_NSS2_PIN;
	spi_master_gpio.Mode = GPIO_MODE_AF_PER;
	spi_master_gpio.Pull = GPIO_NOPULL;
	spi_master_gpio.Alternate = HAL_REMAP_SPI_NSS2;
	HAL_GPIO_Init(&spi_master_gpio);

	//初始化SPI
	if(HAL_SPI_Init(&spi_master) != HAL_OK)
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
void spi_master_test_task(void)
{
	uint8_t txdata[8] = {'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H'};
	uint8_t rxdata[9] = {'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 0};
	while(1)
	{
/*		//测试1，HAL_SPI_Transmit、HAL_SPI_Receive
		for(uint8_t i = 0; i < sizeof(txdata); i++)
		{
			HAL_SPI_ChipSelect(&spi_master, HAL_SPI_CS_PIN_1);
			HAL_SPI_Transmit(&spi_master, &txdata[i], 1, 0);
			HAL_SPI_Receive(&spi_master, &rxdata[i], 1, 0);
			HAL_SPI_ChipSelect(&spi_master, HAL_SPI_CS_NONE);
			txdata[i] += 5;
		}
*/
		//测试2，HAL_SPI_TransmitReceive
		for(uint8_t i = 0; i < sizeof(txdata); i++)
		{
			//拉低片选引脚
			HAL_SPI_ChipSelect(&spi_master, HAL_SPI_CS_PIN_1);
			//发送1个字节，接收1个字节，设置超时时间为500ms
			HAL_SPI_TransmitReceive(&spi_master, &txdata[i], &rxdata[i], 1, HAL_MAX_DELAY);
			//拉高片选引脚
			HAL_SPI_ChipSelect(&spi_master, HAL_SPI_CS_NONE);
			//改变发送的字符，以示区分
			txdata[i] += 8;
		}
		//打印接收到的字符，用于调试
		send_debug_str_to_at_uart((char *)rxdata);
		//通过osDelay释放线程控制权
		osDelay(1000);	
	}
}

/**
 * @brief 任务创建
 *
 */
void spi_master_test_task_init(void)
{
	g_spi_master_test_TskHandle = osThreadNew((osThreadFunc_t)spi_master_test_task, NULL, "spi_master_test_task", 512, osPriorityNormal);
}


#endif
