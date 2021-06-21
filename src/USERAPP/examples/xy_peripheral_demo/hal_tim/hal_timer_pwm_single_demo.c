/**
* @file			timer_pwm_single_mode_demo.c
* @ingroup		peripheral
* @brief		PWM single mode产生单路PWM Demo
* @note			配置TPOL来选择此模式下初始时电平状态为高电平（1）或低电平（0）
				timer count达到pwm register翻转输出电平，达到reload value产生1个中断，timer count复位并自动继续计数
				timer frequency = pclk / ClockDivision / Reload
				pwm frequency = pclk / ClockDivision / Reload
				pwm duty cycle = PWMReg / Reload
				以timer reload值为306、PWMReg为306/3、TPOL为0举例，示意图如下：
				           __________________           __________________         
				          |                  |         |                  |        
				 _________|                  |_________|                  |________
				|--306/3--|-----306*2/3------|--306/3--|-----306*2/3------|--------	

***********************************************************************************/

#if DEMO_TEST


#include "xy_api.h"

#define TimHandle          		TimPWMSingleHandle

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

	//初始化Timer
	TimHandle.Instance				=	HAL_TIM1;
	TimHandle.Init.Mode				=	HAL_TIM_MODE_PWM_SINGLE;
	TimHandle.Init.Reload			=	306;
	TimHandle.Init.ClockDivision	=	HAL_TIM_CLK_DIV_128;
	TimHandle.Init.PWMReg			=	306 / 4;
	TimHandle.Init.TIMPolarity		=	HAL_TIM_Polarity_Set;
	HAL_TIM_Init(&TimHandle);

	//开启Timer
	HAL_TIM_Start(&TimHandle);
 * @endcode
 * 
 */
void hal_pwm_single_mode_init(void)
{
	//映射GPIO为Timer的输出引脚	
	HAL_GPIO_InitTypeDef gpio_init;

	gpio_init.Pin		= HAL_GPIO_PIN_NUM_6;
	gpio_init.Mode		= GPIO_MODE_AF_PER;
	gpio_init.Alternate	= HAL_REMAP_TMR1_AF_OUTP;
	HAL_GPIO_Init(&gpio_init);

	//初始化Timer
	TimHandle.Instance				=	HAL_TIM1;
	TimHandle.Init.Mode				=	HAL_TIM_MODE_PWM_SINGLE;
	TimHandle.Init.Reload			=	306;
	TimHandle.Init.ClockDivision	=	HAL_TIM_CLK_DIV_128;
	TimHandle.Init.PWMReg			=	306 / 4;
	TimHandle.Init.TIMPolarity		=	HAL_TIM_Polarity_Set;
	HAL_TIM_Init(&TimHandle);

	//开启Timer
	HAL_TIM_Start(&TimHandle);
}


#endif
