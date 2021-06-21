/**
* @file        hal_gpio_input_demo.c
* @ingroup     peripheral
* @brief       GPIO输入Demo，配置GPIO为上拉输入模式，并读取输入状态
* @attention     
-# 对于输入模式，每个GPIO并不同时支持上拉和下拉，仅支持上拉或下拉中的其中一种状态。在XY1100芯片中，GPIO10/11/12仅支持下拉，其余的GPIO仅支持上拉；
-# GPIO的中断触发方式，支持下降沿触发，双边沿触发，高电平触发，不支持上升沿触发。如果外部输入信号并不平稳，需要外部增加消抖电路，推荐电容大小为0.1uF；
-# Analog模式目前尚未支持；
-# Open Drain（开漏输出）模式目前尚未支持；
-# 使用xy_GPIO_SetIOMode设置GPIO模式时，如果设置为普通GPIO模式，参数需配置成GPIO_MODE_GPIO_为前缀的选项，如果有外设映射到GPIO上，参数需要配置成GPIO_MODE_HW_为前缀的选项，其中，GPIO_MODE_HW_PER为硬件自动设置GPIO模式，配置成合适的模式，其他的为手动指定映射外设情况下的GPIO模式。
*
***********************************************************************************/

#if DEMO_TEST


#include "xy_api.h"

//任务参数配置
#define GPIO_IN_TASK_PRIORITY    10
#define GPIO_IN_STACK_SIZE       500

//任务全局变量
osThreadId_t g_hal_gpio_in_TskHandle = NULL;


/**
 * @brief 初始化函数
 * @code
 *	HAL_GPIO_InitTypeDef gpio_init;

	//初始化GPIO引脚
	gpio_init.Pin		= HAL_GPIO_PIN_NUM_6;
	gpio_init.Mode		= GPIO_MODE_INPUT;
	gpio_init.Pull		= GPIO_PULLUP;
	HAL_GPIO_Init(&gpio_init);
 * @endcode
 */
void hal_gpio_in_init(void)
{
	HAL_GPIO_InitTypeDef gpio_init;

	//初始化GPIO引脚
	gpio_init.Pin		= HAL_GPIO_PIN_NUM_6;
	gpio_init.Mode		= GPIO_MODE_INPUT;
	gpio_init.Pull		= GPIO_PULLUP;
	HAL_GPIO_Init(&gpio_init);
}


/**
 * @brief 任务线程
 */
void hal_gpio_in_work_task(void)
{
	while(1)
	{
		uint8_t gpio_in_value = 0xff;

		gpio_in_value = HAL_GPIO_ReadPin(HAL_GPIO_PIN_NUM_6);
		xy_printf("HAL_GPIO_PIN_NUM_6 Value:%d", gpio_in_value);
		//通过osDelay释放线程控制权
		osDelay(1000);	
	}
}


/**
 * @brief 任务创建
 */
void hal_gpio_in_work_task_init(void)
{
    g_hal_gpio_in_TskHandle = osThreadNew ((osThreadFunc_t)hal_gpio_in_work_task,NULL,"hal_gpio_in_work_task",GPIO_IN_STACK_SIZE,osPriorityNormal);
}



#endif
