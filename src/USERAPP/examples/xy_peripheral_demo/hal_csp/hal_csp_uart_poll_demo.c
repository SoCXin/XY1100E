/**
* @file	hal_csp_uart_poll_demo.c
* @ingroup	peripheral
* @brief	CSP1默认用于SIMCARD，CSP2默认用于AT，CSP3默认用于LOG，CSP4可以自由配置，此处将CSP配置为UART
* 			此demo使用轮询方式接收数据，超时时间内接收到指定数据长度，将接收到的数据回写至uart；
* 			如果超时时间未接收到指定长度数据，将实际接收长度的数据回写至uart
* @warning 	CSP配置为UART模式时只支持无校验。
			用户使用外设时需注意：1.使用前关闭standby，使用完开启standby 2.确保没有在NV中配置外设所使用的GPIO，否则芯片进出standby时会根据NV重配GPIO，导致外设无法正常使用
***********************************************************************************/
#if DEMO_TEST

#include "xy_api.h"

#define CSP_UART_REMAP_TX       HAL_REMAP_CSP4_TXD
#define CSP_UART_REMAP_RX       HAL_REMAP_CSP4_RXD
#define CSP_UART_TX_PIN         HAL_GPIO_PIN_NUM_6
#define CSP_UART_RX_PIN         HAL_GPIO_PIN_NUM_7


osThreadId_t  g_hal_csp_uart_poll_demo_TskHandle = NULL;
osSemaphoreId_t  g_hal_csp_uart_poll_demo_sem = NULL;

HAL_CSP_HandleTypeDef hal_csp_uart_poll_demo_handle;

/**
* @brief	csp_UART初始化函数，这个函数描述了进行串口初始化需要的相关步骤。 \n
* 			在初始化内部需要设置串口的RX TX引脚与GPIO引脚的对应关系、上下拉状态，
* 			以及串口的波特率、数据长度、停止位等 \n
*/

void hal_csp_uart_poll_demo_peri_init(void)
{
	//创建信号量
	g_hal_csp_uart_poll_demo_sem = osSemaphoreNew(0xFFFF, 0);

	HAL_GPIO_InitTypeDef csp_uart_gpio_init;

	//映射UART的TX引脚
	csp_uart_gpio_init.Pin		= CSP_UART_TX_PIN;
	csp_uart_gpio_init.Mode		= GPIO_MODE_AF_PER;
	csp_uart_gpio_init.Pull		= GPIO_NOPULL;
	csp_uart_gpio_init.Alternate	= CSP_UART_REMAP_TX;
	HAL_GPIO_Init(&csp_uart_gpio_init);

	//映射UART的RX引脚
	csp_uart_gpio_init.Pin		= CSP_UART_RX_PIN;
	csp_uart_gpio_init.Mode 		= GPIO_MODE_AF_INPUT;
	csp_uart_gpio_init.Pull		= GPIO_PULLUP;
	csp_uart_gpio_init.Alternate 	= CSP_UART_REMAP_RX;
	HAL_GPIO_Init(&csp_uart_gpio_init);

	//初始化CSP为UART
	hal_csp_uart_poll_demo_handle.Instance				= HAL_CSP4;
	hal_csp_uart_poll_demo_handle.Init.UART_BaudRate	= 9600;
	hal_csp_uart_poll_demo_handle.Init.UART_DataBits	= HAL_CSP_DATA_BITS_8;
	hal_csp_uart_poll_demo_handle.Init.UART_StopBits	= HAL_CSP_STOP_BITS_1;
	HAL_CSP_UART_Init(&hal_csp_uart_poll_demo_handle);

	//使能CSP
	HAL_CSP_Ctrl(&hal_csp_uart_poll_demo_handle, HAL_ENABLE);
}

/**
 * @brief	任务线程
 * 			此demo使用轮询方式接收数据，超时时间内接收到指定数据长度，将接收到的数据回写至uart；
* 			如果超时时间未接收到指定长度数据，将实际接收长度的数据回写至uart
 */
void hal_csp_uart_poll_demo_task(void)
{
	uint8_t data[9] = {'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 0};
	while(1)
	{
		HAL_StatusTypeDef ret = HAL_OK;
		//接收8个字节，设置超时时间为500ms
		ret = HAL_CSP_Receive(&hal_csp_uart_poll_demo_handle, data, 8, 500);
		if(ret == HAL_OK)
		{
			//成功接收指定字节数
			//用户进行数据处理，此处demo仅将接收到的数据回写
			HAL_CSP_Transmit(&hal_csp_uart_poll_demo_handle, data, 8, 500);
		}
		else if(ret == HAL_TIMEOUT)
		{
			//超时未成功接收指定字节数，hal_csp_uart_poll_demo_handle.RxXferCount为实际接收字节数
			if(hal_csp_uart_poll_demo_handle.RxXferCount > 0)
			{
				//用户进行数据处理，此处demo仅将接收到的数据回写
				HAL_CSP_Transmit(&hal_csp_uart_poll_demo_handle, data, hal_csp_uart_poll_demo_handle.RxXferCount, 500);
			}
		}
		//通过osDelay释放线程控制权
		osDelay(1000);															
	}
}

/**
 * @brief	任务创建函数
 */
void hal_csp_uart_poll_demo_task_init(void)
{
	g_hal_csp_uart_poll_demo_TskHandle = osThreadNew((osThreadFunc_t)hal_csp_uart_poll_demo_task, NULL, "hal_csp_uart_poll_demo_task", 512, osPriorityNormal);
}

#endif
