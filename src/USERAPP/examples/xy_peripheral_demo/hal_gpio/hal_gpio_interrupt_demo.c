/**
* @file        gpio_int_demo.c
* @ingroup     peripheral
* @brief       GPIO中断demo
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
#define HAL_GPIO_INT_TASK_PRIORITY    10
#define HAL_GPIO_INT_STACK_SIZE       500

//任务全局变量
osThreadId_t g_hal_gpio_int_TskHandle = NULL;
osSemaphoreId_t g_hal_gpio_int_sem = NULL;

static uint8_t hal_gpio_interrupt_times = 0;


/**
 * @brief 中断服务函数。注意：1.用户在使用时需将__weak 删除！！！2.用户需保证中断及中断里调用的函数都在ram上！！！
 */
void HAL_GPIO_IRQHandler(void) __RAM_FUNC;
__weak void HAL_GPIO_IRQHandler(void)
{
	//读清中断，并返回中断状态寄存器的值
	uint32_t sta = HAL_GPIO_ReadAndClearIntFlag(HAL_GPIO_PIN_NUM_6);
	//关闭GPIO中断
	HAL_GPIO_IntCtl(HAL_GPIO_PIN_NUM_6, DISABLE);
	//判断中断是否来自当前GPIO
	if(sta & (1 << HAL_GPIO_PIN_NUM_6))
	{
		hal_gpio_interrupt_times++;
		osSemaphoreRelease(g_hal_gpio_int_sem);
	}
}


/**
 * @brief 初始化函数
 * @code
 *	//注册GPIO中断
	HAL_GPIO_IT_REGISTER();

	//创建信号量
	g_hal_gpio_int_sem = osSemaphoreNew(0xFFFF, 0);

	HAL_GPIO_InitTypeDef gpio_init;

	//初始化GPIO引脚
	gpio_init.Pin		= HAL_GPIO_PIN_NUM_6;
	gpio_init.Mode		= GPIO_MODE_IT_FALLING;
	gpio_init.Pull		= GPIO_PULLUP;
	HAL_GPIO_Init(&gpio_init);

	//初始化GPIO引脚
	gpio_init.Pin		= HAL_GPIO_PIN_NUM_7;
	gpio_init.Mode		= GPIO_MODE_IT_RISING_FALLING;
	gpio_init.Pull		= GPIO_PULLUP;
	HAL_GPIO_Init(&gpio_init);
 * @endcode
 */
void hal_gpio_int_init(void)
{
	//注册GPIO中断
	HAL_GPIO_IT_REGISTER();

	//创建信号量
	g_hal_gpio_int_sem = osSemaphoreNew(0xFFFF, 0);

	HAL_GPIO_InitTypeDef gpio_init;

	//初始化GPIO引脚
	gpio_init.Pin		= HAL_GPIO_PIN_NUM_6;
	gpio_init.Mode		= GPIO_MODE_IT_FALLING;
	gpio_init.Pull		= GPIO_PULLUP;
	HAL_GPIO_Init(&gpio_init);

	//初始化GPIO引脚
	gpio_init.Pin		= HAL_GPIO_PIN_NUM_7;
	gpio_init.Mode		= GPIO_MODE_IT_RISING_FALLING;
	gpio_init.Pull		= GPIO_PULLUP;
	HAL_GPIO_Init(&gpio_init);
}


/**
 * @brief 任务线程
 */
void hal_gpio_int_work_task(void)
{
	while(1)
	{
		//等待获取信号量，释放线程控制权
		if(osOK == osSemaphoreAcquire(g_hal_gpio_int_sem, osWaitForever))
		{
			//打印信息，用于调试，用户可在此做实际业务
			xy_printf("hal_gpio_interrupt times:%d\n", hal_gpio_interrupt_times);
		}
	}
}


/**
 * @brief 任务创建
 */
void hal_gpio_int_work_task_init(void)
{
	g_hal_gpio_int_TskHandle = osThreadNew ((osThreadFunc_t)hal_gpio_int_work_task,NULL,"hal_gpio_int_work_task",HAL_GPIO_INT_STACK_SIZE,osPriorityNormal);
}
#endif
