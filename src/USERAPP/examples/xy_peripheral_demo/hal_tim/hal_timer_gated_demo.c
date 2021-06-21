/**
* @file			timer_gated_mode_demo.c
* @ingroup		peripheral
* @brief		Timer定时器Demo, 工作在gated模式。
* @note			配置TPOL来选择此模式下输入高电平（0）/低电平（1）时定时器计数。
				timer count到达reload值时产生中断并且自动重置tiemr count为1；
				输入信号电平翻转时也会产生中断，可通过API控制屏蔽此中断。
* @attention	如果对时间精度不敏感，不建议直接使用定时器外设进行定时功能的开发，建议使用软定时器替代。
				gated模式需要配置输入引脚，并且确定极性，如果TPOL配置成1，则gated模式定时器低电平使能，上升沿或重载会产生中断。
				timer frequency = pclk / ClockDivision / Reload
				以timer reload值为306*12000、TPOL为0举例：前两个中断由于电平翻转产生，最后一个中断由于计数到reload值产生，示意图如下：
				                        _                  _                       _                  
				                       | |                | |                     | |                 
				 ______________________| |________________| |_____________________| |___________________
				|--------------------- |------------------------------------------|
				                                     
				    ___________________                    _____________________________________________    
				   |                   |                  |                               
				 __|                   |__________________|                                
				|  |-----count计数-----|---count停止计数---|--count计数至306*12000--|---count重新计数------

***********************************************************************************/

#if DEMO_TEST



#include "xy_api.h"

#define TIMER_TASK_PRIORITY     10
#define TIMER_STACK_SIZE        0x400


#define TimHandle          		TimGatedHandle

osThreadId_t  g_hal_gated_time_TskHandle = NULL;
osSemaphoreId_t  g_hal_gated_time_sem = NULL;

static uint8_t hal_gated_timer_counter = 0;

HAL_TIM_HandleTypeDef TimHandle;

/**
 * @brief TIMER中断处理函数
 * @note 可以在定时器中断函数中做时间敏感业务相关处理
 * @warning 部分函数和打印log xy_printf()不能在此处调用，否则会触发断言
 * 注意：1.用户在使用时需将__weak 删除！！！2.用户需保证中断及中断里调用的函数都在ram上！！！
 * 用户注意Timer中断函数与Timer的匹配。HAL_TIM1与HAL_TIM1_IRQHandler匹配，依此类推。
 */
void HAL_TIM1_IRQHandler(void) __RAM_FUNC;
__weak void HAL_TIM1_IRQHandler(void)
{
	hal_gated_timer_counter++;
	osSemaphoreRelease(g_hal_gated_time_sem);
}


/**
 * @brief TIMER初始化函数
 * @code
 *	//创建信号量
	g_hal_gated_time_sem = osSemaphoreNew(0xFFFF, 0);

	//映射GPIO为Timer的输入引脚
	HAL_GPIO_InitTypeDef gpio_init;

	gpio_init.Pin		= HAL_GPIO_PIN_NUM_8;
	gpio_init.Mode		= GPIO_MODE_AF_INPUT;
	gpio_init.Pull		= GPIO_PULLUP;
	gpio_init.Alternate	= HAL_REMAP_TMR1_AF_INPUT;
	HAL_GPIO_Init(&gpio_init);

	//初始化Timer
	TimHandle.Instance				=	HAL_TIM1;
	TimHandle.Init.Mode				=	HAL_TIM_MODE_GATED;
	TimHandle.Init.Reload			=	306 * 2000;
	TimHandle.Init.ClockDivision	=	HAL_TIM_CLK_DIV_128;
	TimHandle.Init.TIMPolarity		=	HAL_TIM_Polarity_Set;
	HAL_TIM_Init(&TimHandle);

	//设置Timer只在reload和compare事件下才触发中断
	HAL_TIM_IntControl(&TimHandle, HAL_TIM_RELOAD_COMP_EVENTS);

	//注册Timer中断服务函数
	HAL_TIM_IT_REGISTER(&TimHandle);

	//开启Timer
	HAL_TIM_Start(&TimHandle);
 * @endcode
 *
 */
void hal_gated_timer_init(void)
{
	//创建信号量
	g_hal_gated_time_sem = osSemaphoreNew(0xFFFF, 0);

	//映射GPIO为Timer的输入引脚
	HAL_GPIO_InitTypeDef gpio_init;

	gpio_init.Pin		= HAL_GPIO_PIN_NUM_8;
	gpio_init.Mode		= GPIO_MODE_AF_INPUT;
	gpio_init.Pull		= GPIO_PULLUP;
	gpio_init.Alternate	= HAL_REMAP_TMR1_AF_INPUT;
	HAL_GPIO_Init(&gpio_init);

	//初始化Timer
	TimHandle.Instance				=	HAL_TIM1;
	TimHandle.Init.Mode				=	HAL_TIM_MODE_GATED;
	TimHandle.Init.Reload			=	306 * 2000;
	TimHandle.Init.ClockDivision	=	HAL_TIM_CLK_DIV_128;
	TimHandle.Init.TIMPolarity		=	HAL_TIM_Polarity_Set;
	HAL_TIM_Init(&TimHandle);

	//设置Timer只在reload和compare事件下才触发中断
	HAL_TIM_IntControl(&TimHandle, HAL_TIM_RELOAD_COMP_EVENTS);

	//注册Timer中断服务函数
	HAL_TIM_IT_REGISTER(&TimHandle);

	//开启Timer
	HAL_TIM_Start(&TimHandle);
}


/**
 * @brief 任务线程
 *
 */
void hal_gated_timer_work_task(void)
{
	while(1)
	{
		//等待获取信号量，释放线程控制权
		if(osOK == osSemaphoreAcquire(g_hal_gated_time_sem, osWaitForever))
		{
			//打印调试信息，用户可在此做实际业务
			xy_printf("hal_gated_timer_counter: %d\n", hal_gated_timer_counter);
		}
		
	}
}


/**
 * @brief 任务创建
 *
 */
void hal_gated_timer_work_task_init(void)
{
	g_hal_gated_time_TskHandle = osThreadNew((osThreadFunc_t)hal_gated_timer_work_task,NULL,"hal_gated_timer_work_task",TIMER_STACK_SIZE,osPriorityNormal);
}



#endif
