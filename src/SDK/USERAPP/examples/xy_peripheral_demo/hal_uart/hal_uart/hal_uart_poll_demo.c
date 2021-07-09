/**
* @file		hal_uart_poll_demo.c
* @ingroup	peripheral
* @brief	UART外设Demo，使用轮询方式接收
* 			在这个Demo中,UART先进行阻塞等待8个字节的数据，阻塞的时间为500ms，如果在500ms内收到8个字节的数据，则将接收到的数据进行回写。 \n
* 			如果在500ms内没有收到8个字节的数据，则检测到底接收到了多少个数据，如果接收数据的个数不为0，则回写相应的数据。
*
* @warning	1、除非是特别简单的应用，一般不会使用轮询模式。如果要使用轮询模式，一定要有个良好的程序架构或实现机制，避免程序无限挂起。
* 			2、用户使用外设时需注意：1.使用前关闭standby，使用完开启standby 2.确保没有在NV中配置外设所使用的GPIO，否则芯片进出standby时会根据NV重配GPIO，导致外设无法正常使用
***********************************************************************************/

#if DEMO_TEST


#include "xy_api.h"

//任务参数配置
#define UART_TX_PIN        HAL_GPIO_PIN_NUM_6
#define UART_RX_PIN        HAL_GPIO_PIN_NUM_7

//任务全局变量
osThreadId_t  hal_uart_poll_demo_TskHandle = NULL;
HAL_UART_HandleTypeDef hal_uart_poll_demo_handle;

/**
* @brief	cUART初始化函数，这个函数描述了进行串口初始化需要的相关步骤。 \n
* 			在初始化内部需要设置串口的RX TX引脚与GPIO引脚的对应关系、上下拉状态，
* 			以及串口的波特率、数据长度、校验位、停止位等 \n
*/
void hal_uart_poll_demo_peri_init(void)
{
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
	hal_uart_poll_demo_handle.Instance = HAL_UART1;
	hal_uart_poll_demo_handle.Init.BaudRate = 9600;
	hal_uart_poll_demo_handle.Init.WordLength = UART_WORDLENGTH_8B;
	hal_uart_poll_demo_handle.Init.StopBits = UART_STOPBITS_1;
	hal_uart_poll_demo_handle.Init.Parity = UART_PARITY_NONE;
	HAL_UART_Init(&hal_uart_poll_demo_handle);
}

/**
* @brief 	当前HAL库支持轮询和中断两种模式,此代码为UART的轮询模式Demo。 \n
* 		 	在这个Demo中,UART先进行阻塞等待8个字节的数据，阻塞的时间为500ms，如果在500ms内收到8个字节的数据，则将接收到的数据进行回写。 \n
* 		 	如果在500ms内没有收到8个字节的数据，则检测到底接收到了多少个数据，如果接收数据的个数不为0，则回写相应的数据。
*
* @note  	除非是特别简单的应用，一般不会使用轮询模式。如果要使用轮询模式，一定要有个良好的程序架构或实现机制，避免程序无限挂起。
*/
void hal_uart_poll_demo_task(void)
{
	uint8_t data[9] = {'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 0};
	while(1)
	{
		HAL_StatusTypeDef ret = HAL_OK;
		//接收8个字节，设置超时时间为500ms
		ret = HAL_UART_Receive(&hal_uart_poll_demo_handle, data, 8, 500);
		if(ret == HAL_OK)
		{
			//成功接收指定字节数
			//用户进行数据处理，此处demo仅将接收到的数据回写
			HAL_UART_Transmit(&hal_uart_poll_demo_handle, data, 8, 500);
		}
		else if(ret == HAL_TIMEOUT)
		{
			//超时未成功接收指定字节数,hal_uart_poll_demo_handle.RxXferCount为实际接收字节数
			if(hal_uart_poll_demo_handle.RxXferCount > 0)
			{
				//用户进行数据处理，此处demo仅将接收到的数据回写
				HAL_UART_Transmit(&hal_uart_poll_demo_handle, data, hal_uart_poll_demo_handle.RxXferCount, 500);
			}
		}
		//通过osDelay释放线程控制权
		osDelay(1000);	
	}
}

/**
 * @brief 任务创建
 * 
 */
void hal_uart_poll_demo_task_init(void)
{
	hal_uart_poll_demo_TskHandle = osThreadNew((osThreadFunc_t)hal_uart_poll_demo_task, NULL, "hal_uart_poll_demo_task", 512, osPriorityNormal);
}


#endif
