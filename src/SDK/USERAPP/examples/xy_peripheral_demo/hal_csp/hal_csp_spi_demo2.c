/**
* @file		hal_csp_spi_demo2.c
* @ingroup	peripheral
* @brief	CSP1默认用于SIMCARD，CSP2默认用于AT，CSP3默认用于LOG，CSP4可以自由配置
*			此处配置为SPI master模式，并且NSS片选端配置成通用IO模式，由用户通过API控制
* @warning	CSP不支持配置为slave模式；如果从设备为多个时（超过一个），建议NSS片选端自定义选用多个IO
			用户使用外设时需注意：1.使用前关闭standby，使用完开启standby 2.确保没有在NV中配置外设所使用的GPIO，否则芯片进出standby时会根据NV重配GPIO，导致外设无法正常使用
***********************************************************************************/
#if DEMO_TEST

#include "xy_api.h"

#define CSP_SPI_MOSI_PIN          HAL_GPIO_PIN_NUM_6
#define CSP_SPI_MISO_PIN          HAL_GPIO_PIN_NUM_7
#define CSP_SPI_SCLK_PIN          HAL_GPIO_PIN_NUM_8
#define CSP_SPI_NSS1_PIN          HAL_GPIO_PIN_NUM_9

#define CSP_SPI_SPEED             1000000UL      		//最高支持6.5MHz(39.168MHz/6=6.528MHz)

osThreadId_t  g_hal_csp_spi_demo2_TskHandle = NULL;

HAL_CSP_HandleTypeDef hal_csp_spi_demo2_handle;

/**
 * @brief	初始化函数
 * @code
 *	HAL_GPIO_InitTypeDef hal_csp_spi_demo2_gpio_init;

	//映射SPI的MOSI引脚
	hal_csp_spi_demo2_gpio_init.Pin			= CSP_SPI_MOSI_PIN;
	hal_csp_spi_demo2_gpio_init.Mode 		= GPIO_MODE_AF_PER;
	hal_csp_spi_demo2_gpio_init.Pull 		= GPIO_NOPULL;
	hal_csp_spi_demo2_gpio_init.Alternate 	= HAL_REMAP_CSP4_TXD;
	HAL_GPIO_Init(&hal_csp_spi_demo2_gpio_init);

	//映射SPI的MISO引脚
	hal_csp_spi_demo2_gpio_init.Pin 		= CSP_SPI_MISO_PIN;
	hal_csp_spi_demo2_gpio_init.Mode 		= GPIO_MODE_AF_PER;
	hal_csp_spi_demo2_gpio_init.Pull 		= GPIO_NOPULL;
	hal_csp_spi_demo2_gpio_init.Alternate = HAL_REMAP_CSP4_RXD;
	HAL_GPIO_Init(&hal_csp_spi_demo2_gpio_init);

	//映射SPI的SCLK引脚
	hal_csp_spi_demo2_gpio_init.Pin 		= CSP_SPI_SCLK_PIN;
	hal_csp_spi_demo2_gpio_init.Mode 		= GPIO_MODE_AF_PER;
	hal_csp_spi_demo2_gpio_init.Pull 		= GPIO_NOPULL;
	hal_csp_spi_demo2_gpio_init.Alternate 	= HAL_REMAP_CSP4_SCLK;
	HAL_GPIO_Init(&hal_csp_spi_demo2_gpio_init);

	//映射SPI的NSS引脚
	hal_csp_spi_demo2_gpio_init.Pin 		= CSP_SPI_NSS1_PIN;
	hal_csp_spi_demo2_gpio_init.Mode 		= GPIO_MODE_OUTPUT_PP;
	hal_csp_spi_demo2_gpio_init.Pull 		= GPIO_NOPULL;
	hal_csp_spi_demo2_gpio_init.Alternate 	= HAL_REMAP_CSP4_TFS;
	HAL_GPIO_Init(&hal_csp_spi_demo2_gpio_init);

	//初始化CSP为SPI
	hal_csp_spi_demo2_handle.Instance			= HAL_CSP4;
	hal_csp_spi_demo2_handle.Init.SPI_Mode		= HAL_MASTER_MODE;
	hal_csp_spi_demo2_handle.Init.SPI_WorkMode	= HAL_MODE_0;
	hal_csp_spi_demo2_handle.Init.SPI_Clock		= CSP_SPI_SPEED;
	HAL_CSP_SPI_Init(&hal_csp_spi_demo2_handle);

	//设置通过API控制NSS引脚
	HAL_CSP_SPI_SetNSSManual(&hal_csp_spi_demo2_handle, HAL_ENABLE);

	HAL_CSP_SPI_SetTxFrame(&hal_csp_spi_demo2_handle, 8, 1, 1);
	HAL_CSP_SPI_SetRxFrame(&hal_csp_spi_demo2_handle, 8, 1, 1);

	//使能CSP
	HAL_CSP_Ctrl(&hal_csp_spi_demo2_handle, HAL_ENABLE);
 * @endcode 
 */
void hal_csp_spi_demo2_peri_init(void)
{
	HAL_GPIO_InitTypeDef hal_csp_spi_demo2_gpio_init;

	//映射SPI的MOSI引脚
	hal_csp_spi_demo2_gpio_init.Pin			= CSP_SPI_MOSI_PIN;
	hal_csp_spi_demo2_gpio_init.Mode 		= GPIO_MODE_AF_PER;
	hal_csp_spi_demo2_gpio_init.Pull 		= GPIO_NOPULL;
	hal_csp_spi_demo2_gpio_init.Alternate 	= HAL_REMAP_CSP4_TXD;
	HAL_GPIO_Init(&hal_csp_spi_demo2_gpio_init);

	//映射SPI的MISO引脚
	hal_csp_spi_demo2_gpio_init.Pin 		= CSP_SPI_MISO_PIN;
	hal_csp_spi_demo2_gpio_init.Mode 		= GPIO_MODE_AF_PER;
	hal_csp_spi_demo2_gpio_init.Pull 		= GPIO_NOPULL;
	hal_csp_spi_demo2_gpio_init.Alternate = HAL_REMAP_CSP4_RXD;
	HAL_GPIO_Init(&hal_csp_spi_demo2_gpio_init);

	//映射SPI的SCLK引脚
	hal_csp_spi_demo2_gpio_init.Pin 		= CSP_SPI_SCLK_PIN;
	hal_csp_spi_demo2_gpio_init.Mode 		= GPIO_MODE_AF_PER;
	hal_csp_spi_demo2_gpio_init.Pull 		= GPIO_NOPULL;
	hal_csp_spi_demo2_gpio_init.Alternate 	= HAL_REMAP_CSP4_SCLK;
	HAL_GPIO_Init(&hal_csp_spi_demo2_gpio_init);

	//映射SPI的NSS引脚
	hal_csp_spi_demo2_gpio_init.Pin 		= CSP_SPI_NSS1_PIN;
	hal_csp_spi_demo2_gpio_init.Mode 		= GPIO_MODE_OUTPUT_PP;
	hal_csp_spi_demo2_gpio_init.Pull 		= GPIO_NOPULL;
	hal_csp_spi_demo2_gpio_init.Alternate 	= HAL_REMAP_CSP4_TFS;
	HAL_GPIO_Init(&hal_csp_spi_demo2_gpio_init);

	//初始化CSP为SPI
	hal_csp_spi_demo2_handle.Instance			= HAL_CSP4;
	hal_csp_spi_demo2_handle.Init.SPI_Mode		= HAL_MASTER_MODE;
	hal_csp_spi_demo2_handle.Init.SPI_WorkMode	= HAL_MODE_0;
	hal_csp_spi_demo2_handle.Init.SPI_Clock		= CSP_SPI_SPEED;
	HAL_CSP_SPI_Init(&hal_csp_spi_demo2_handle);

	//设置通过API控制NSS引脚
	HAL_CSP_SPI_SetNSSManual(&hal_csp_spi_demo2_handle, HAL_ENABLE);

	HAL_CSP_SPI_SetTxFrame(&hal_csp_spi_demo2_handle, 8, 1, 1);
	HAL_CSP_SPI_SetRxFrame(&hal_csp_spi_demo2_handle, 8, 1, 1);

	//使能CSP
	HAL_CSP_Ctrl(&hal_csp_spi_demo2_handle, HAL_ENABLE);
}

/**
 * @brief	任务线程
 */
void hal_csp_spi_demo2_task(void)
{
	uint8_t txdata[8] = {'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H'};
	uint8_t rxdata[9] = {'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 0};
	while(1)
	{
		for(uint8_t i = 0; i < sizeof(txdata); i++)
		{
			//通过API拉低NSS引脚
			HAL_GPIO_WritePin(CSP_SPI_NSS1_PIN,HAL_GPIO_PIN_RESET);
			//发送1个字节，接收1个字节，设置超时时间为500ms
			HAL_CSP_SPI_TransmitReceive(&hal_csp_spi_demo2_handle, &txdata[i], &rxdata[i], 1, 500);
			//通过API拉高NSS引脚
			HAL_GPIO_WritePin(CSP_SPI_NSS1_PIN,HAL_GPIO_PIN_SET);
			//改变发送的字符，以示区分
			txdata[i] += sizeof(txdata);															
		}
		//通过AT口打印出接收到的数据，用于调试
		send_debug_str_to_at_uart((char *)rxdata);
		//通过osDelay释放线程控制权
		osDelay(1000);
	}
}

/**
 * @brief	任务创建
 */
void hal_csp_spi_demo2_task_init(void)
{
	g_hal_csp_spi_demo2_TskHandle = osThreadNew((osThreadFunc_t)hal_csp_spi_demo2_task, NULL, "hal_csp_spi_demo2_task", 512, osPriorityNormal);
}

#endif
