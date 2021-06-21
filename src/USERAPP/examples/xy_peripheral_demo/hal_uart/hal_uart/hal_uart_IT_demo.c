/**
* @file		hal_uart_IT_demo.c
* @ingroup	peripheral
* @brief	UART外设Demo，使用中断方式接收
* 			此demo使用中断方式接收指定长度数据，fifo中收到数据就触发中断开始接收。接收到指定长度数据则将数据回写至uart；
* 			如果没接收到指定长度数据，则将实际接收长度的数据回写至uart\n
*
* 
* @warning	用户使用外设时需注意：1.使用前关闭standby，使用完开启standby 2.确保没有在NV中配置外设所使用的GPIO，否则芯片进出standby时会根据NV重配GPIO，导致外设无法正常使用
***********************************************************************************/

#if DEMO_TEST


#include "xy_api.h"

//任务参数配置
#define UART_TX_PIN        HAL_GPIO_PIN_NUM_6
#define UART_RX_PIN        HAL_GPIO_PIN_NUM_7

//任务全局变量
osThreadId_t  hal_uart_IT_demo_TskHandle = NULL;
osSemaphoreId_t  g_hal_uart_IT_demo_sem = NULL;

HAL_UART_HandleTypeDef hal_uart_IT_demo_handle;

/**
 * @brief 错误中断回调函数。注意：1.用户在使用时需将__weak 删除！！！2.用户需保证中断及中断里调用的函数都在ram上！！！
 */
void HAL_UART_ErrorCallback(HAL_UART_HandleTypeDef* huart) __RAM_FUNC;
__weak void HAL_UART_ErrorCallback(HAL_UART_HandleTypeDef* huart)
{
	//用户根据实际需求添加错误处理代码

	//错误处理完后恢复ErrorCode
	huart->ErrorCode = HAL_UART_ERROR_NONE;
	HAL_UART_DeInit(huart);
	HAL_UART_Init(huart);
}

/**
 * @brief 中断回调函数。注意：1.用户在使用时需将__weak 删除！！！2.用户需保证中断及中断里调用的函数都在ram上！！！
 */
void HAL_UART_RxCpltCallback(HAL_UART_HandleTypeDef *huart) __RAM_FUNC;
__weak void HAL_UART_RxCpltCallback(HAL_UART_HandleTypeDef *huart)
{
	UNUSED(huart);
	osSemaphoreRelease(g_hal_uart_IT_demo_sem);
}

/**
 * @brief 中断服务函数。注意：1.用户在使用时需将__weak 删除！！！2.用户需保证中断及中断里调用的函数都在ram上！！！
 */
void HAL_UART_IRQHandler(void) __RAM_FUNC;
__weak void HAL_UART_IRQHandler(void)
{
	UART_IRQHandler(&hal_uart_IT_demo_handle);
}

/**
* @brief	UART初始化函数，这个函数描述了进行串口初始化需要的相关步骤。 \n
* 			在初始化内部需要设置串口的RX TX引脚与GPIO引脚的对应关系、上下拉状态，
* 			以及串口的波特率、数据长度、校验位、停止位等 \n
*/
void hal_uart_IT_demo_peri_init(void)
{
	//创建信号量
	g_hal_uart_IT_demo_sem = osSemaphoreNew(0xFFFF, 0);

	HAL_GPIO_InitTypeDef uart_gpio_init;

	//映射UART的RX引脚
	uart_gpio_init.Pin = UART_RX_PIN;
	uart_gpio_init.Mode = GPIO_MODE_AF_INPUT;
	uart_gpio_init.Pull = GPIO_PULLUP;
	uart_gpio_init.Alternate = HAL_REMAP_UART_RXD;
	HAL_GPIO_Init(&uart_gpio_init);

	//映射UART的TX引脚
	uart_gpio_init.Pin = UART_TX_PIN;
	uart_gpio_init.Mode = GPIO_MODE_AF_PER;
	uart_gpio_init.Pull = GPIO_NOPULL;
	uart_gpio_init.Alternate = HAL_REMAP_UART_TXD;
	HAL_GPIO_Init(&uart_gpio_init);

	//初始化UART
	hal_uart_IT_demo_handle.Instance = HAL_UART1;
	hal_uart_IT_demo_handle.Init.BaudRate = 9600;
	hal_uart_IT_demo_handle.Init.WordLength = UART_WORDLENGTH_8B;
	hal_uart_IT_demo_handle.Init.StopBits = UART_STOPBITS_1;
	hal_uart_IT_demo_handle.Init.Parity = UART_PARITY_NONE;
	HAL_UART_Init(&hal_uart_IT_demo_handle);
}

/**
 * @brief	此demo使用中断方式接收指定长度数据，fifo中收到数据就触发中断开始接收。接收到指定长度数据则将数据回写至uart；
* 			如果没接收到指定长度数据，则将实际接收长度的数据回写至uart\n
 */
void hal_uart_IT_demo_task(void)
{
	char *str_out = NULL;
	uint8_t data[512] = {0};
	while(1)
	{
		//开启中断接收数据
		HAL_UART_Receive_IT(&hal_uart_IT_demo_handle, data, 256);
		//获取信号量，释放线程控制权；中断接收完成后会释放信号量，获取到信号量后线程继续执行
		osSemaphoreAcquire(g_hal_uart_IT_demo_sem, osWaitForever);
		//hal_uart_IT_demo_handle.RxXferCount为实际接收字节数
		if(hal_uart_IT_demo_handle.RxXferCount > 0)
		{
			//用户进行数据处理，此处demo仅将接收到的数据回写
			HAL_UART_Transmit(&hal_uart_IT_demo_handle, data, hal_uart_IT_demo_handle.RxXferCount, 500);
			str_out = xy_zalloc(200);
			snprintf(str_out, 200, "hal_uart_IT_demo_handle.RxXferCount: %d\n", hal_uart_IT_demo_handle.RxXferCount);
			send_debug_str_to_at_uart(str_out);
			xy_free(str_out);
		}
	}
}

/**
 * @brief 任务创建
 * 
 */
void hal_uart_IT_demo_task_init(void)
{
	hal_uart_IT_demo_TskHandle = osThreadNew((osThreadFunc_t)hal_uart_IT_demo_task, NULL, "hal_uart_IT_demo_task", 1024, osPriorityNormal);
}


#endif
