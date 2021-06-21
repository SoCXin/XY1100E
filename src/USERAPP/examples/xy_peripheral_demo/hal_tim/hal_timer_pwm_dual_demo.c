/**
* @file			timer_pwm_dual_mode_demo
* @ingroup		peripheral
* @note			可以产生2路互补带死区的PWM信号，配置TPOL来设置极性。
* @brief		PWM dual-mode产生Demo
* @note			配置TPOL来选择此模式下初始时电平状态为高电平（1）或低电平（0）
				timer count达到pwm register翻转输出电平，达到reload value产生1个中断，timer count复位并自动继续计数
				timer frequency = pclk / ClockDivision / Reload
				pwm frequency = pclk / ClockDivision / Reload
				pwm duty cycle = PWMReg / Reload
				以timer reload值为306、PWMReg为306/3、TPOL为0举例，示意图如下：
				           __________________           __________________         
				          |                  |         |                  |        
				 _________|                  |_________|                  |________
				 _________                    _________                    ________
				          |                  |         |                  |
				          |__________________|         |__________________|

				|--306/3--|-----306*2/3------|--306/3--|-----306*2/3------|--------			
***********************************************************************************/

#if DEMO_TEST


#include "xy_api.h"


#define TimHandle          		TimPWMDualHandle

HAL_TIM_HandleTypeDef TimHandle;


/**
 * @brief PWM初始化函数
 * @code
	//映射GPIO为Timer的输出引脚
	HAL_GPIO_InitTypeDef gpio_init;

	gpio_init.Pin		= HAL_GPIO_PIN_NUM_6;
	gpio_init.Mode		= GPIO_MODE_AF_PER;
	gpio_init.Alternate	= HAL_REMAP_TMR1_AF_OUTP;
	HAL_GPIO_Init(&gpio_init);

	gpio_init.Pin		= HAL_GPIO_PIN_NUM_7;
	gpio_init.Mode		= GPIO_MODE_AF_PER;
	gpio_init.Alternate	= HAL_REMAP_TMR1_AF_OUTN;
	HAL_GPIO_Init(&gpio_init);

	//初始化Timer
	TimHandle.Instance				=	HAL_TIM1;
	TimHandle.Init.Mode				=	HAL_TIM_MODE_PWM_DUAL;
	TimHandle.Init.Reload			=	306;
	TimHandle.Init.ClockDivision	=	HAL_TIM_CLK_DIV_128;
	TimHandle.Init.PWMReg			=	306 / 3;
	TimHandle.Init.TIMPolarity		=	HAL_TIM_Polarity_Set;
	TimHandle.Init.PWMDualDelay		=	HAL_TIM_DELAY_64_CYCLES;
	HAL_TIM_Init(&TimHandle);

	//开启Timer
	HAL_TIM_Start(&TimHandle);
 * @endcode
 * 
 */
void hal_pwm_dual_mode_init(void)
{
	//映射GPIO为Timer的输出引脚
	HAL_GPIO_InitTypeDef gpio_init;

	gpio_init.Pin		= HAL_GPIO_PIN_NUM_6;
	gpio_init.Mode		= GPIO_MODE_AF_PER;
	gpio_init.Alternate	= HAL_REMAP_TMR1_AF_OUTP;
	HAL_GPIO_Init(&gpio_init);

	gpio_init.Pin		= HAL_GPIO_PIN_NUM_7;
	gpio_init.Mode		= GPIO_MODE_AF_PER;
	gpio_init.Alternate	= HAL_REMAP_TMR1_AF_OUTN;
	HAL_GPIO_Init(&gpio_init);

	//初始化Timer
	TimHandle.Instance				=	HAL_TIM1;
	TimHandle.Init.Mode				=	HAL_TIM_MODE_PWM_DUAL;
	TimHandle.Init.Reload			=	306;
	TimHandle.Init.ClockDivision	=	HAL_TIM_CLK_DIV_128;
	TimHandle.Init.PWMReg			=	306 / 3;
	TimHandle.Init.TIMPolarity		=	HAL_TIM_Polarity_Set;
	TimHandle.Init.PWMDualDelay		=	HAL_TIM_DELAY_64_CYCLES;
	HAL_TIM_Init(&TimHandle);

	//开启Timer
	HAL_TIM_Start(&TimHandle);
}


#endif
