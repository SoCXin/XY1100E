/**
* @file			timer_counter_mode_demo.c
* @ingroup		peripheral
* @brief		Timer定时器Demo, 工作在counter模式.
* @note			根据TPOL配置外部输入信号上升沿（0）或下降沿（1）计数。此模式下配置timer预分频无效。外部输入跳变沿作为时钟触发timer count计数，
				到达reload值触发中断并且重置timer count为1。
* @attention	如果对时间精度不敏感，不建议直接使用定时器外设进行定时功能的开发，建议使用软定时器替代。
				timer frequency = pclk / ClockDivision / Reload
				以timer reload值为1500为例，示意图如下：
				                        _                                       _                  
				                       | |                                     | |                 
				 ______________________| |_____________________________________| |____________
				|--------------------- |---------------------------------------|
				                                     
				    _    _         _    _       _    _                     _    _                
				   | |  | |  ...  | |  | |     | |  | |        ...        | |  | |               
				 __| |__| |__...__| |__| |_____| |__| |________...________| |__| |____________
				|------1500个上升沿---- |----------------1500个上升沿------------|

***********************************************************************************/
#if DEMO_TEST


#include "xy_api.h"


//任务参数配置
#define TIMER_TASK_PRIORITY     10
#define TIMER_STACK_SIZE        0x400

//demo宏定义
#define TimHandle          		TimCounterHandle
HAL_TIM_HandleTypeDef TimHandle;

//任务全局变量
osThreadId_t  g_hal_counter_timer_TskHandle = NULL;
osSemaphoreId_t  g_hal_counter_timer_sem = NULL;

//demo静态全局变量
static uint32_t counter_value = 0;
static uint16_t interrupt_times = 0;


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
	interrupt_times++;
	osSemaphoreRelease(g_hal_counter_timer_sem);
}


/**
 * @brief TIMER counter模式初始化函数
 *  	这个函数描述了timer初始化为counter模式时需要的相关步骤。\n
 * 		在初始化函数内部需要设置counter模式的输入引脚与GPIO引脚的对应关系、复用方式、上下拉状态， \n
 * 		以及定时器的编号、工作模式、重载值、时钟极性等\n
 * 		且需要注册定时中断，然后打开定时器。\n
 */
void hal_counter_timer_init(void)
{
	//创建信号量
	g_hal_counter_timer_sem = osSemaphoreNew(0xFFFF, 0);

	//映射GPIO为Timer的输入引脚
	HAL_GPIO_InitTypeDef gpio_init;

	gpio_init.Pin		= HAL_GPIO_PIN_NUM_8;
	gpio_init.Mode		= GPIO_MODE_AF_INPUT;
	gpio_init.Pull		= GPIO_PULLUP;
	gpio_init.Alternate	= HAL_REMAP_TMR2_AF_INPUT;
	HAL_GPIO_Init(&gpio_init);

	//初始化Timer
	TimHandle.Instance				=	HAL_TIM2;
	TimHandle.Init.Mode				=	HAL_TIM_MODE_COUNTER;
	TimHandle.Init.Reload			=	1800;
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
void hal_counter_timer_work_task(void)
{


	while(1)
	{
		//等待获取信号量，释放线程控制权
		if(osOK == osSemaphoreAcquire(g_hal_counter_timer_sem, osWaitForever))
		{
			//获取counter寄存器的值
			counter_value = HAL_TIM_GetCount(&TimHandle);
			//打印调试信息，用户可在此做实际业务
			xy_printf("Counter Value:%d\n", counter_value);
			xy_printf("Interrupt Times:%d\n", interrupt_times);
		}

	}
}


/**
 * @brief 任务创建
 *
 */
void hal_counter_timer_work_task_init(void)
{
	g_hal_counter_timer_TskHandle = osThreadNew((osThreadFunc_t)hal_counter_timer_work_task,NULL,"hal_counter_timer_work_task",TIMER_STACK_SIZE,osPriorityNormal);
}


#endif
