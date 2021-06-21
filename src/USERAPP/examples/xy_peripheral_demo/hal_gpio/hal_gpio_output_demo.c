/**
* @file        gpio_out_demo.c
* @ingroup     peripheral
* @brief       GPIO输出Demo，配置GPIO为推挽输出模式，并定时切换GPIO输出状态
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
#define GPIO_OUT_TASK_PRIORITY    10
#define GPIO_OUT_STACK_SIZE       500

//任务全局变量
osThreadId_t g_hal_gpio_out_TskHandle =NULL;


/**
 * @brief 初始化函数
 * @code
 *	HAL_GPIO_InitTypeDef gpio_init;

	//初始化GPIO引脚
	gpio_init.Pin		= HAL_GPIO_PIN_NUM_7;
	gpio_init.Mode		= GPIO_MODE_OUTPUT_PP;
	gpio_init.Pull		= GPIO_NOPULL;
	HAL_GPIO_Init(&gpio_init);
 * @endcode
 */
void hal_gpio_out_init(void)
{
	HAL_GPIO_InitTypeDef gpio_init;

	//初始化GPIO引脚
	gpio_init.Pin		= HAL_GPIO_PIN_NUM_7;
	gpio_init.Mode		= GPIO_MODE_OUTPUT_PP;
	gpio_init.Pull		= GPIO_NOPULL;
	HAL_GPIO_Init(&gpio_init);
}


/**
 * @brief 任务线程
 */
void hal_gpio_out_work_task(void)
{
	while(1)
	{
		//隔3s切换GPIO引脚的输出状态，用于调试
		
		HAL_GPIO_WritePin(HAL_GPIO_PIN_NUM_7, HAL_GPIO_PIN_SET);
		xy_printf("\nHAL_GPIO_PIN_NUM_7  = 1\n");
		//通过osDelay释放线程控制权
		osDelay(3000);

		HAL_GPIO_WritePin(HAL_GPIO_PIN_NUM_7, HAL_GPIO_PIN_RESET);
		xy_printf("\nHAL_GPIO_PIN_NUM_7  = 0\n");
		//通过osDelay释放线程控制权
		osDelay(3000);
	}
}


/**
 * @brief 任务创建
 */
void hal_gpio_out_work_task_init(void)
{
	g_hal_gpio_out_TskHandle = osThreadNew((osThreadFunc_t)hal_gpio_out_work_task,NULL,"hal_gpio_out_work_task",GPIO_OUT_STACK_SIZE,osPriorityNormal);
}



#endif
