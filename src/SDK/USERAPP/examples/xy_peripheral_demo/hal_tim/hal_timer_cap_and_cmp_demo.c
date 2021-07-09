/**
* @file			timer_cap_and_cmp_demo.c
* @ingroup		peripheral
* @brief		Timer定时器Demo, 工作在capture/compare模式。
* @note			根据TPOL配置，在外部输入信号的第一个上升（0）/下降沿（1）开始Timer计时;
				捕获到外部输入信号上升/下降沿时，产生1个timer capture中断，并且timer count重置为1;
				没有捕获到capture触发信号时，timer count达到compare value（reload值）时产生timer reload中断，timer count置1并自动继续计数。
				timer frequency = pclk / ClockDivision / Reload
				以timer reload值为306*12000为例：外部未输入信号时，每隔12s产生1次reload中断；当30s时外部输入信号，则立刻产生capture中断并重新计数，过12s后再次产生reload中断。示意图如下：
				              _              _        _              _      _              _                     
				             | |            | |      | |            | |    | |            | |                    
				 ____________| |____________| |______| |____________| |____| |____________| |______
				|----12s-----|-----12s------|---6s---|-----12s------|--4s--|-----12s------|--------
				                                     
				                                      _                     _                             
				                                     | |                   | |                            
				 ____________________________________| |___________________| |_____________________
				|---------------30s------------------|-------16s-----------|-----------------------

***********************************************************************************/
#if DEMO_TEST


#include "xy_api.h"

//任务参数配置
#define TIMER_TASK_PRIORITY     10
#define TIMER_STACK_SIZE        0x400
//任务全局变量
osThreadId_t g_hal_cap_and_cmp_timer_TskHandle = NULL;
osSemaphoreId_t g_hal_cap_and_cmp_timer_sem = NULL;

//demo静态全局变量
static uint32_t counter_value = 0;
static uint16_t interrupt_times = 0;
//demo宏定义
#define TimHandle          		TimCapAndCmpHandle

HAL_TIM_HandleTypeDef TimHandle;

/**
 * @brief TIMER中断处理函数
 * @note 可以在定时器中断函数中做时间敏感业务相关处理
 * @warning 部分函数和打印log xy_printf()不能在此处调用，否则会触发断言
 * 注意：1.用户在使用时需将__weak 删除！！！2.用户需保证中断及中断里调用的函数都在ram上！！！
 * 用户注意Timer中断函数与Timer的匹配。HAL_TIM1与HAL_TIM1_IRQHandler匹配，依此类推。
 */
void HAL_TIM2_IRQHandler(void) __RAM_FUNC;
__weak void HAL_TIM2_IRQHandler(void)
{
	counter_value = HAL_TIM_GetCount(&TimHandle);
	interrupt_times++;
	osSemaphoreRelease(g_hal_cap_and_cmp_timer_sem);
}


/**
 * @brief TIMER初始化函数
 * @code
 *	//创建信号量
	g_hal_cap_and_cmp_timer_sem = osSemaphoreNew(0xFFFF, 0);

	//映射GPIO为Timer的输入引脚
	HAL_GPIO_InitTypeDef gpio_init;

	gpio_init.Pin		= HAL_GPIO_PIN_NUM_8;
	gpio_init.Mode		= GPIO_MODE_AF_INPUT;
	gpio_init.Pull		= GPIO_PULLUP;
	gpio_init.Alternate	= HAL_REMAP_TMR2_AF_INPUT;
	HAL_GPIO_Init(&gpio_init);

	//初始化Timer
	TimHandle.Instance				=	HAL_TIM2;
	TimHandle.Init.Mode				=	HAL_TIM_MODE_CAP_AND_CMP;
	TimHandle.Init.Reload			=	306 * 3000;
	TimHandle.Init.ClockDivision	=	HAL_TIM_CLK_DIV_128;
	TimHandle.Init.TIMPolarity		=	HAL_TIM_Polarity_Set;
	HAL_TIM_Init(&TimHandle);

	//注册Timer中断服务函数
	HAL_TIM_IT_REGISTER(&TimHandle);

	//开启Timer
	HAL_TIM_Start(&TimHandle);
 * @endcode
 *
 */
void hal_cap_and_cmp_timer_init(void)
{
	//创建信号量
	g_hal_cap_and_cmp_timer_sem = osSemaphoreNew(0xFFFF, 0);

	//映射GPIO为Timer的输入引脚
	HAL_GPIO_InitTypeDef gpio_init;

	gpio_init.Pin		= HAL_GPIO_PIN_NUM_8;
	gpio_init.Mode		= GPIO_MODE_AF_INPUT;
	gpio_init.Pull		= GPIO_PULLUP;
	gpio_init.Alternate	= HAL_REMAP_TMR2_AF_INPUT;
	HAL_GPIO_Init(&gpio_init);

	//初始化Timer
	TimHandle.Instance				=	HAL_TIM2;
	TimHandle.Init.Mode				=	HAL_TIM_MODE_CAP_AND_CMP;
	TimHandle.Init.Reload			=	306 * 3000;
	TimHandle.Init.ClockDivision	=	HAL_TIM_CLK_DIV_128;
	TimHandle.Init.TIMPolarity		=	HAL_TIM_Polarity_Set;
	HAL_TIM_Init(&TimHandle);

	//注册Timer中断服务函数
	HAL_TIM_IT_REGISTER(&TimHandle);

	//开启Timer
	HAL_TIM_Start(&TimHandle);
}


/**
 * @brief 任务线程
 *
 */
void hal_cap_and_cmp_timer_work_task(void)
{
	while(1)
	{
		//等待获取信号量，释放线程控制权
		if( osOK == osSemaphoreAcquire(g_hal_cap_and_cmp_timer_sem, osWaitForever) )	
		{
			//打印调试信息，用户可在此做实际业务
			xy_printf("Timer Counter Value:%d\n", counter_value);
			xy_printf("Timer Interrupt Times:%d\n", interrupt_times);
		}
	}
}


/**
 * @brief 任务创建
 *
 */
void hal_cap_and_cmp_timer_work_task_init(void)
{
	g_hal_cap_and_cmp_timer_TskHandle = osThreadNew((osThreadFunc_t)hal_cap_and_cmp_timer_work_task,NULL,"hal_cap_and_cmp_timer_work_task",TIMER_STACK_SIZE,osPriorityNormal);
}


#endif
