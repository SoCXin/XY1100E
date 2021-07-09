/**
* @file			timer_capture_mode_demo.c
* @ingroup		peripheral
* @brief		Timer定时器Demo, 工作在capture模式，连续计时，自动重载。
* @attention	如果对时间精度不敏感，不建议直接使用定时器外设进行定时功能的开发，建议使用软定时器替代。
* @note			timer capture 用于捕获外部输入信号。通过配置TPOL来选择捕获外部输入上升沿（0）或者下降沿（1）。
				当Timer输入引脚上出现捕获波形，会产生一个timer capture中断，并且把当前定时器timer count存入capture count。
				此模式下定时器不会自动重载，timer count到达reload会产生一个timer reload中断（不会把timer count存入capture count），然后timer count置1并自动继续计数。
				timer frequency = pclk / ClockDivision / Reload
				以timer reload值为306*12000为例：外部未输入信号，每隔12s产生1次reload中断；当30s时外部输入信号，则产生capture中断继续计数，36s时再次产生reload中断。示意图如下：
				              _              _        _        _            _    _                     
				             | |            | |      | |      | |          | |  | |                    
				 ____________| |____________| |______| |______| |__________| |__| |____________________________
				|----12s-----|-----12s------|---6s---|---6s---|-----10s----|-2s-|------------------------------ 
				                                     
				                                      _                     _                                             
				                                     | |                   | |                                            
				 ____________________________________| |___________________| |_________________________________
				|---------------30s------------------|-------16s-----------|-----------------------------------
***********************************************************************************/

#if DEMO_TEST


#include "xy_api.h"

//任务参数配置
#define TIMER_TASK_PRIORITY  10	
#define TIMER_STACK_SIZE     0x400	

//demo宏定义
#define TimHandle				TimCaptureHandle
HAL_TIM_HandleTypeDef TimHandle;

//任务全局变量
osThreadId_t  g_hal_capture_timer_TskHandle = NULL;
osSemaphoreId_t g_hal_capture_timer_sem = NULL;

//捕获波形计数
static uint8_t interrupt_times = 0;
static uint32_t capture_count_value = 0;

/**
 * @brief TIMER中断处理函数
 * @note 可以在定时器中断函数中做时间敏感业务相关处理, 每当捕获到波形会进入这个中断。
 * @warning 部分函数和打印log xy_printf()不能在此处调用，否则会触发断言
 * 注意：1.用户在使用时需将__weak 删除！！！2.用户需保证中断及中断里调用的函数都在ram上！！！
 * 用户注意Timer中断函数与Timer的匹配。HAL_TIM1与HAL_TIM1_IRQHandler匹配，依此类推。
 */
void HAL_TIM1_IRQHandler(void) __RAM_FUNC;
__weak void HAL_TIM1_IRQHandler(void)
{
	//HAL_TIM_SetCount(&TimHandle, 0);
	capture_count_value = HAL_TIM_GetCaptureCount(&TimHandle);
	interrupt_times++;
	osSemaphoreRelease(g_hal_capture_timer_sem);
}

/**
 * @brief TIMER capture模式初始化函数
 *    	这个函数描述了timer初始化为capture模式时需要的相关步骤。\n
 * 		在初始化函数内部需要设置capture模式的输入引脚与GPIO引脚的对应关系、复用方式、上下拉状态， \n
 * 		以及定时器的编号、工作模式、重载值、时钟分频、时钟极性等\n
 * 		且需要注册定时中断，然后打开定时器。\n
 */
void hal_capture_timer_init(void)
{
	//创建信号量
	g_hal_capture_timer_sem = osSemaphoreNew(0xFFFF, 0);

	//映射GPIO为Timer的输入引脚
	HAL_GPIO_InitTypeDef gpio_init;

	gpio_init.Pin		= HAL_GPIO_PIN_NUM_8;
	gpio_init.Mode		= GPIO_MODE_AF_INPUT;
	gpio_init.Pull		= GPIO_PULLUP;
	gpio_init.Alternate	= HAL_REMAP_TMR1_AF_INPUT;
	HAL_GPIO_Init(&gpio_init);

	//初始化Timer
	TimHandle.Instance				=	HAL_TIM1;
	TimHandle.Init.Mode				=	HAL_TIM_MODE_CAPTURE;
	TimHandle.Init.Reload			=	306 * 10000;
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
void hal_capture_timer_work_task(void)
{
	while(1)
	{
		//等待获取信号量，释放线程控制权
		if( osOK == osSemaphoreAcquire(g_hal_capture_timer_sem, osWaitForever) )
			{
				//打印调试信息，用户可在此做实际业务
			    xy_printf("interrupt_times: %d, capture_count_value: %d\n", interrupt_times, capture_count_value);
			}
    }
}


/**
 * @brief 任务创建
 *
 */
void hal_capture_timer_work_task_init(void)
{
	g_hal_capture_timer_TskHandle = osThreadNew((osThreadFunc_t)hal_capture_timer_work_task,NULL,"hal_capture_timer_work_task",TIMER_STACK_SIZE,osPriorityNormal);
}
#endif
