/**
  ******************************************************************************
  * @file	hal_tim.h
  * @brief	此文件包含定时器外设的变量，枚举，结构体定义，函数声明等. \n
  * 		定时器共有9种工作模式，详情参考 @ref HAL_TIMMode_TypeDef，9种工作模式区别及使用参照Demo. \n
  * 		定时器频率 = 外设时钟/时钟分频/重载值（timer frequency = pclk / ClockDivision / Reload） \n
  *			PWM频率 = 外设时钟/时钟分频/重载值（timer frequency = pclk / ClockDivision / Reload） \n
  *			PWM占空比 = PWM寄存器值/重载值（pwm duty cycle = PWMReg / Reload） \n
  ******************************************************************************
  */
 
/* Define to prevent recursive inclusion -------------------------------------*/
#pragma once

/* Includes ------------------------------------------------------------------*/
#include "hal_cortex.h"
#include "hw_memmap.h"

/** 
  * @brief  定时器工作模式枚举  
  */
typedef enum {
  HAL_TIM_MODE_ONE_SHOT    = 0x00000000,	/*!< 一次触发模式 */  
  HAL_TIM_MODE_CONTINUOUS  = 0x00000100,	/*!< 循环触发模式 */  
  HAL_TIM_MODE_COUNTER     = 0x00000200,	/*!< 计数模式 */  
  HAL_TIM_MODE_PWM_SINGLE  = 0x00000300,	/*!< 单PWM模式 */  
  HAL_TIM_MODE_CAPTURE     = 0x00000400,	/*!< 捕获模式 */  
  HAL_TIM_MODE_COMPARE     = 0x00000500,	/*!< 比较模式 */  
  HAL_TIM_MODE_GATED       = 0x00000600,	/*!< 门模式 */  
  HAL_TIM_MODE_CAP_AND_CMP = 0x00000700,	/*!< 捕获比较模式 */  
  HAL_TIM_MODE_PWM_DUAL    = 0x00000800 	/*!< 双PWM模式 */
} HAL_TIMMode_TypeDef;


/** 
  * @brief  定时器分频系数枚举  
  */
typedef enum {
  HAL_TIM_CLK_DIV_1   = 0x00000000,	/*!< 1分频 */
  HAL_TIM_CLK_DIV_2   = 0x00001000,	/*!< 2分频 */
  HAL_TIM_CLK_DIV_4   = 0x00002000,	/*!< 4分频 */
  HAL_TIM_CLK_DIV_8   = 0x00003000,	/*!< 8分频 */
  HAL_TIM_CLK_DIV_16  = 0x00004000,	/*!< 16分频 */
  HAL_TIM_CLK_DIV_32  = 0x00005000,	/*!< 32分频 */
  HAL_TIM_CLK_DIV_64  = 0x00006000,	/*!< 64分频 */
  HAL_TIM_CLK_DIV_128 = 0x00007000	/*!< 128分频 */
} HAL_TIMClkDiv_TypeDef;

/** 
  * @brief  定时器中断类型枚举  
  */
typedef enum {
  HAL_TIM_ALL_EVENTS 				= 0x0020, /*!< 所有定义的事件都会触发中断，包含重载，比较和输入事件 */
  HAL_TIM_CAP_DEASSERTION_EVENTS	= 0x0040, /*!< 仅输入事件触发中断 */
  HAL_TIM_RELOAD_COMP_EVENTS		= 0x0060, /*!< 仅重载，比较事件触发中断 */
} HAL_TIMInt_TypeDef;

/** 
  * @brief  定时器延时周期枚举.此处周期为系统时钟周期，主要用于双PWM模式两路PWM波之间的延时.
  */
typedef enum {
  HAL_TIM_DELAY_0_CYCLES	= 0x00000000,	/*!< 延时0个周期 */
  HAL_TIM_DELAY_2_CYCLES	= 0x00000002,	/*!< 延时2个周期 */
  HAL_TIM_DELAY_4_CYCLES	= 0x00000004,	/*!< 延时4个周期 */
  HAL_TIM_DELAY_8_CYCLES	= 0x00000006,	/*!< 延时8个周期 */
  HAL_TIM_DELAY_16_CYCLES	= 0x00000008,	/*!< 延时16个周期 */
  HAL_TIM_DELAY_32_CYCLES	= 0x0000000A,	/*!< 延时32个周期 */
  HAL_TIM_DELAY_64_CYCLES	= 0x0000000C,	/*!< 延时64个周期 */
  HAL_TIM_DELAY_128_CYCLES	= 0x0000000E	/*!< 延时128个周期 */
} HAL_TIMDelay_TypeDef;

/**
  * @brief  定时器极性枚举.
  * @note	不同模式下极性对应的含义不同，具体含义如下: \n
  * 		一次触发模式：负极性表示定时器关闭时输出为低电平，开启定时器Count寄存器计数值达到Reload寄存器重载值后翻转为高电平。正极性与负极性含义相反。 \n
  *			循环触发模式：含义与一次触发模式相同。 \n
  *			计数模式: 对于输出，负极性表示定时器关闭时输出为低电平，开启定时器Count寄存器计数值达到Reload寄存器重载值后翻转为高电平。对于输入，负极性表示下升沿计数。正极性与负极性含义相反。 \n
  *			单PWM模式: 负极性表示定时器关闭时输出为低电平，开启定时器Count寄存器计数值达到PWM寄存器的值后翻转为高电平，Count寄存器计数值达到Reload寄存器重载值后重新输出低电平。正极性与负极性含义相反。 \n
  *			捕获模式: 负极性表示捕获上升沿。正极性与负极性含义相反。 \n
  *			比较模式: 含义与一次触发模式相同。 \n
  *			门模式: 负极性表示在输入为高电平时Count寄存器计数，下降沿触发中断。正极性与负极性含义相反。 \n
  *			捕获比较模式: 负极性表示捕获上升沿。正极性与负极性含义相反。 \n
  *			双PWM模式：含义与单PWM模式相同。区别在于双PWM模式会输出另一类极性相反的PWM波，两路PWM波可以控制延时多少个系统时钟周期
  */
typedef enum {
  HAL_TIM_Polarity_Reset	= 0x00000000,	/*!< 设置输入输出负极性 */
  HAL_TIM_Polarity_Set		= 0x00008000	/*!< 设置输入输出正极性 */
} HAL_TIMPol_TypeDef;


/**
  * @brief  定时器PWM延时枚举.主要用于双PWM模式两路PWM波之间的延时.
  */
typedef enum {
  HAL_TIM_PWMDelay_Reset	= 0x00000000,	/*!< PWM不延时 */
  HAL_TIM_PWMDelay_Set		= 0x00008000	/*!< PWM延时 */
} HAL_TIMPolarity_TypeDef;


/**
  * @brief 定时器寄存器结构体
  */
typedef struct
{
  uint32_t TMR_COUNT;		/*!< 定时器计数寄存器 */
  uint32_t TMR_RLD;			/*!< 定时器重载寄存器 */
  uint32_t TMR_PWM;			/*!< 定时器PWM寄存器 */
  uint32_t TMR_CTL;			/*!< 定时器控制寄存器 */
  uint32_t TMR_CNT_OFFSET;	/*!< 定时器计数偏移寄存器 */
} HAL_TIM_TypeDef;


/**
  * @brief 定时器初始化结构体
  */
typedef struct
{
  HAL_TIMMode_TypeDef   	Mode; 			/*!< 定时器工作模式 */
  uint32_t          		Reload;			/*!< 定时器重载值 */
  uint32_t          		PWMReg;			/*!< 定时器PWM寄存器 */
  HAL_TIMClkDiv_TypeDef 	ClockDivision;	/*!< 定时器时钟分频 */
  HAL_TIMPolarity_TypeDef	TIMPolarity;	/*!< 定时器极性 */
  HAL_TIMDelay_TypeDef	  	PWMDualDelay;	/*!< 定时器双PWM延时 */
} HAL_TIM_InitTypeDef;


/**
  * @brief  定时器控制结构体
  */
typedef struct
{
  HAL_TIM_TypeDef			*Instance;	/*!< 定时器基地址 */
  HAL_TIM_InitTypeDef		Init;		/*!< 定时器初始化结构体 */
  HAL_LockTypeDef			Lock;		/*!< 定时器设备锁 */
} HAL_TIM_HandleTypeDef;


/**
  * @brief  定时器工作状态枚举
  */
typedef enum
{
  HAL_TIM_STATE_RESET             = 0x00U,  /*!< 外设未初始化 */
  HAL_TIM_STATE_READY             = 0x01U,  /*!< 外设初始化完成可以使用 */
  HAL_TIM_STATE_BUSY              = 0x02U,  /*!< 一个内部线程正在进行 */
  HAL_TIM_STATE_TIMEOUT           = 0x03U,  /*!< 超时状态 */
  HAL_TIM_STATE_ERROR             = 0x04U   /*!< 错误状态 */
} HAL_TIM_StateTypeDef;


/**
  * @brief  初始化定时器.
  * @param  htim 指向一个包含定时器具体配置信息的HAL_TIM_HandleTypeDef结构体的指针.详情参考 @ref HAL_TIM_HandleTypeDef.
  * @retval HAL status.详情参考 @ref HAL_StatusTypeDef.
  */
HAL_StatusTypeDef HAL_TIM_Init(HAL_TIM_HandleTypeDef *htim);

/**
  * @brief  去初始化定时器.
  * @param  htim 指向一个包含定时器具体配置信息的HAL_TIM_HandleTypeDef结构体的指针.详情参考 @ref HAL_TIM_HandleTypeDef.
  * @retval 无
  */
void HAL_TIM_DeInit(HAL_TIM_HandleTypeDef *htim);

/**
  * @brief  开启定时器.
  * @param  htim 指向一个包含定时器具体配置信息的HAL_TIM_HandleTypeDef结构体的指针.详情参考 @ref HAL_TIM_HandleTypeDef.
  * @retval 无
  */
void HAL_TIM_Start(HAL_TIM_HandleTypeDef *htim);

/**
  * @brief  停止定时器.
  * @param  htim 指向一个包含定时器具体配置信息的HAL_TIM_HandleTypeDef结构体的指针.详情参考 @ref HAL_TIM_HandleTypeDef.
  * @retval 无
  */
void HAL_TIM_Stop(HAL_TIM_HandleTypeDef *htim);

/**
  * @brief  获取定时器Count寄存器中的值.
  * @param  htim 指向一个包含定时器具体配置信息的HAL_TIM_HandleTypeDef结构体的指针.详情参考 @ref HAL_TIM_HandleTypeDef.
  * @retval Count寄存器中的值.
  */
uint32_t HAL_TIM_GetCount(HAL_TIM_HandleTypeDef *htim);

/**
  * @brief  获取定时器CaptureCount寄存器中的值.
  * @param  htim 指向一个包含定时器具体配置信息的HAL_TIM_HandleTypeDef结构体的指针.详情参考 @ref HAL_TIM_HandleTypeDef.
  * @retval CaptureCount寄存器中的值.
  */
uint32_t HAL_TIM_GetCaptureCount(HAL_TIM_HandleTypeDef *htim);

/**
  * @brief  设置定时器Count寄存器中的值.
  * @param  htim	指向一个包含定时器具体配置信息的HAL_TIM_HandleTypeDef结构体的指针.详情参考 @ref HAL_TIM_HandleTypeDef.
  * @param	count	设置的定时器Count寄存器中的值.
  * @retval 无
  */
void HAL_TIM_SetCount(HAL_TIM_HandleTypeDef *htim, uint32_t count);

/**
  * @brief  设置定时器pwm寄存器中的值.
  * @param  htim	指向一个包含定时器具体配置信息的HAL_TIM_HandleTypeDef结构体的指针.详情参考 @ref HAL_TIM_HandleTypeDef.
  * @param	count	设置的定时器pwm寄存器中的值.
  * @retval 无
  */
void HAL_TIM_SetPwm(HAL_TIM_HandleTypeDef *htim, uint32_t pulse);

/**
  * @brief  定时器中断控制.
  * @param  htim 	指向一个包含定时器具体配置信息的HAL_TIM_HandleTypeDef结构体的指针.详情参考 @ref HAL_TIM_HandleTypeDef.
  * @param	option	中断控制类型.详情参考 @ref HAL_TIMInt_TypeDef.
  *		参数可以是以下枚举值:
  *		@arg HAL_TIM_ALL_EVENTS:				所有定义的事件都会触发中断，包含重载，比较和输入事件
  *		@arg HAL_TIM_CAP_DEASSERTION_EVENTS:	仅输入事件触发中断
  *		@arg HAL_TIM_RELOAD_COMP_EVENTS:		仅重载，比较事件触发中断
  * @retval 无
  */
void HAL_TIM_IntControl(HAL_TIM_HandleTypeDef *htim, HAL_TIMInt_TypeDef option);

/**
  * @brief  Timer1中断服务函数.Timer1中断触发时调用此函数.
  * @param  无
  * @retval 无
  */
void HAL_TIM1_IRQHandler(void) __RAM_FUNC;

/**
  * @brief  Timer2中断服务函数.Timer2中断触发时调用此函数.
  * @param  无
  * @retval 无
  */
void HAL_TIM2_IRQHandler(void) __RAM_FUNC;

/**
  * @brief  Timer3中断服务函数.Timer3中断触发时调用此函数.
  * @param  无
  * @retval 无
  */
void HAL_TIM3_IRQHandler(void) __RAM_FUNC;

/**
  * @brief  Timer4中断服务函数.Timer4中断触发时调用此函数.
  * @param  无
  * @retval 无
  */
void HAL_TIM4_IRQHandler(void) __RAM_FUNC;

/**
  * @brief  注册定时器中断.
  * @param  htim 指向一个包含定时器具体配置信息的HAL_TIM_HandleTypeDef结构体的指针.详情参考 @ref HAL_TIM_HandleTypeDef.
  * @retval 无
  */
void HAL_TIM_IT_REGISTER(HAL_TIM_HandleTypeDef *htim);




/**
* @brief  定时器 外设.
*/
#define HAL_TIM1   (HAL_TIM_TypeDef *)TIMER1_BASE
#define HAL_TIM2   (HAL_TIM_TypeDef *)TIMER2_BASE
#define HAL_TIM3   (HAL_TIM_TypeDef *)TIMER3_BASE
#define HAL_TIM4   (HAL_TIM_TypeDef *)TIMER4_BASE




