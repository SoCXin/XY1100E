/**
* @file		hal_csp_uart_IT_demo.c
* @ingroup	peripheral
* @brief	CSP_UART中断接收模式Demo
*			在这个Demo中，先开启,CSP_UART中断接收，等待数据的到来，当串口接收到数据后，则触发接收完成中断，
* 			在中断内部会释放信号量，将接收到的数据进行回写。\n
*
* @note	    1、CSP1默认用于SIMCARD，CSP2默认用于AT，CSP3默认用于LOG，CSP4可以自由配置，此处将CSP配置为UART
* 			2、对于接收非定长数据，建议使用中断方式进行接收.\n
*
* @warning	1、CSP配置为UART模式时只支持无校验。
* 			2、用户使用外设时需注意：1.使用前关闭standby，使用完开启standby 2.确保没有在NV中配置外设所使用的GPIO，否则芯片进出standby时会根据NV重配GPIO，导致外设无法正常使用
***********************************************************************************/
#if DEMO_TEST

#include "xy_api.h"

#define CSP_UART_REMAP_TX       HAL_REMAP_CSP4_TXD
#define CSP_UART_REMAP_RX       HAL_REMAP_CSP4_RXD
#define CSP_UART_TX_PIN         HAL_GPIO_PIN_NUM_6
#define CSP_UART_RX_PIN         HAL_GPIO_PIN_NUM_7


osThreadId_t  g_hal_csp_uart_IT_demo_TskHandle = NULL;
osSemaphoreId_t  g_hal_csp_uart_IT_demo_sem = NULL;

HAL_CSP_HandleTypeDef hal_csp_uart_IT_demo_handle;


/**
 * @brief 错误中断回调函数。注意：1.用户在使用时需将__weak 删除！！！2.用户需保证中断及中断里调用的函数都在ram上！！！
 */
void HAL_CSP_ErrorCallback(HAL_CSP_HandleTypeDef *hcsp) __RAM_FUNC;
__weak void HAL_CSP_ErrorCallback(HAL_CSP_HandleTypeDef *hcsp)
{
	//用户根据实际需求添加错误处理代码

	//错误处理完后恢复ErrorCode
	hcsp->ErrorCode = HAL_CSP_ERROR_NONE;
}

/**
 * @brief	中断回调函数。注意：1.用户在使用时需将__weak 删除！！！2.用户需保证中断及中断里调用的函数都在ram上！！！
 */
void HAL_CSP_RxCpltCallback(HAL_CSP_HandleTypeDef *hcsp) __RAM_FUNC;
__weak void HAL_CSP_RxCpltCallback(HAL_CSP_HandleTypeDef *hcsp)
{
	UNUSED(hcsp);
	osSemaphoreRelease(g_hal_csp_uart_IT_demo_sem);
}

/**
 * @brief	中断服务函数。注意：1.用户在使用时需将__weak 删除！！！2.用户需保证中断及中断里调用的函数都在ram上！！！
 * 			用户注意CSP中断函数与CSP的匹配。HAL_CSP1与HAL_CSP1_IRQHandler匹配，依此类推。
 */
void HAL_CSP4_IRQHandler(void) __RAM_FUNC;
__weak void HAL_CSP4_IRQHandler(void)
{
	CSP_IRQHandler(&hal_csp_uart_IT_demo_handle);
}

/**
* @brief	csp_UART初始化函数，这个函数描述了进行串口初始化需要的相关步骤。 \n
* 			在初始化内部需要设置串口的RX TX引脚与GPIO引脚的对应关系、上下拉状态，
* 			以及串口的波特率、数据长度、停止位等 \n
*/
void hal_csp_uart_IT_demo_peri_init(void)
{
	//创建信号量
	g_hal_csp_uart_IT_demo_sem = osSemaphoreNew(0xFFFF, 0);

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
	hal_csp_uart_IT_demo_handle.Instance				= HAL_CSP4;
	hal_csp_uart_IT_demo_handle.Init.UART_BaudRate	= 9600;
	hal_csp_uart_IT_demo_handle.Init.UART_DataBits	= HAL_CSP_DATA_BITS_8;
	hal_csp_uart_IT_demo_handle.Init.UART_StopBits	= HAL_CSP_STOP_BITS_1;
	HAL_CSP_UART_Init(&hal_csp_uart_IT_demo_handle);

	//使能CSP
	HAL_CSP_Ctrl(&hal_csp_uart_IT_demo_handle, HAL_ENABLE);
}

/**
 * @brief	在这个Demo中，先开启,CSP_UART中断接收，等待数据的到来，当串口接收到数据后，则触发接收完成中断，
 * 			在中断内部会释放信号量，将接收到的数据进行回写。\n
 */
void hal_csp_uart_IT_demo_task(void)
{
	char *str_out = NULL;
	uint8_t data[512] = {0};
	while(1)
	{
		//开启中断接收数据
		HAL_CSP_Receive_IT(&hal_csp_uart_IT_demo_handle, data, 32);
		//获取信号量，释放线程控制权；中断接收完成后会释放信号量，获取到信号量后线程继续执行
		osSemaphoreAcquire(g_hal_csp_uart_IT_demo_sem, osWaitForever);
		//hal_csp_uart_IT_demo_handle.RxXferCount为实际接收字节数
		if(hal_csp_uart_IT_demo_handle.RxXferCount > 0)
		{
			//用户进行数据处理，此处demo仅将接收到的数据回写
			HAL_CSP_Transmit(&hal_csp_uart_IT_demo_handle, data, hal_csp_uart_IT_demo_handle.RxXferCount, 500);
			str_out = xy_zalloc(200);
			snprintf(str_out, 200, "hal_csp_uart_IT_demo_handle.RxXferCount: %d\n", hal_csp_uart_IT_demo_handle.RxXferCount);
			send_debug_str_to_at_uart(str_out);
			xy_free(str_out);
		}
	}
}

/**
 * @brief	任务创建
 */
void hal_csp_uart_IT_demo_task_init(void)
{
	g_hal_csp_uart_IT_demo_TskHandle = osThreadNew((osThreadFunc_t)hal_csp_uart_IT_demo_task, NULL, "hal_csp_uart_IT_demo_task", 1024, osPriorityNormal);
}

#endif
