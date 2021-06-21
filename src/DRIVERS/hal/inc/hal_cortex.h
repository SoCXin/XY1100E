/**
  ******************************************************************************
  * @file    hal_cortex.h
  * @brief   此文件包含hal库核心变量，函数原型，宏定义等.
  ******************************************************************************
  */
#pragma once


#include "hal_def.h"
#include "hw_prcm.h"
#include "replace.h"
#include "hw_memmap.h"


/**
  * @brief	SysTick频率枚举.
  */
typedef enum
{
  HAL_TICK_FREQ_10HZ         = 100U,				/*!< 10赫兹频率 */
  HAL_TICK_FREQ_100HZ        = 10U,					/*!< 100赫兹频率 */
  HAL_TICK_FREQ_1KHZ         = 1U,					/*!< 1000赫兹频率 */
  HAL_TICK_FREQ_DEFAULT      = HAL_TICK_FREQ_1KHZ	/*!< 默认频率，1000赫兹 */
} HAL_TickFreqTypeDef;


typedef enum
{
  HAL_CSP1_IRQn                   = 0,      /*!< 16 CSP1 中断号                              */
  HAL_I2C1_IRQn                   = 1,      /*!< 17 I2C1 中断号                              */
  HAL_SPI1_IRQn                   = 2,      /*!< 18 SPI1 中断号                              */
  HAL_TIM1_IRQn                   = 3,      /*!< 19 TIM1 中断号                              */
  HAL_UART1_IRQn                  = 4,      /*!< 20 UART1 中断号                             */
  HAL_UTC_IRQn                    = 5,      /*!< 21 UTC 中断号                               */
  HAL_WDT_IRQn                    = 6,      /*!< 22 WDT 中断号                               */
  HAL_MEASURECNT_IRQn             = 7,      /*!< 23 MEASURECNT 中断号                        */
  HAL_TIM2_IRQn                   = 8,      /*!< 24 TIM2 中断号                              */
  HAL_CSP2_IRQn                   = 9,      /*!< 25 CSP2 中断号                              */
  HAL_ROOTCLK_XTAL_IRQn           = 10,     /*!< 26 ROOTCLK XTAL 中断号                      */
  HAL_ROOTCLK_PLL_IRQn            = 11,     /*!< 27 ROOTCLK pll 中断号                       */
  HAL_QSPI_IRQn                   = 12,     /*!< 28 QSPI 中断号                              */
  HAL_SEMAPHORE_IRQn              = 13,     /*!< 29 SEMAPHORE 中断号                         */
  HAL_TIM3_IRQn                   = 14,     /*!< 30 TIM3 中断号                              */
  HAL_TIM4_IRQn                   = 15,     /*!< 31 TIM4 中断号                              */
  HAL_DMAC0_IRQn                  = 16,     /*!< 32 DMAC0 中断号                             */
  HAL_DMAC1_IRQn                  = 17,     /*!< 33 DMAC1 中断号                             */
  HAL_DMAC2_IRQn                  = 18,     /*!< 34 DMAC2 中断号                             */
  HAL_DMAC3_IRQn	    	      = 19,     /*!< 35 DMAC3 中断号                             */
  HAL_WAKEUP_IRQn		          = 20,     /*!< 36 WAKEUP 中断号                            */
  HAL_GPIO_IRQn		              = 21,     /*!< 37 GPIO 中断号                              */
  HAL_I2C2_IRQn		              = 22,     /*!< 38 I2C2 中断号                              */
  HAL_CSP3_IRQn		              = 23,     /*!< 39 CSP3 中断号                              */
  HAL_CSP4_IRQn		              = 24,     /*!< 40 CSP4 中断号                              */
  HAL_DFEBUF_IRQn	    	      = 25      /*!< 41 DFEBUF 中断号                            */
} HAL_IRQn_Type;

typedef void(*HAL_IRQnHandle_Type)(void);

/**
  * @brief  This function register interrupt.
  * @param  IRQn: specifies the IRQ source.
  *         This parameter can be one of the IRQn_Type.
  * @param  IRQnHandle: specifies the IRQ handle function.
  *         This parameter can be one of the IRQnHandle_Type.
  * @param  priority: specifies the IRQ priority.
  * @warning  双核运行状态下，擦写flash接口中会锁调度，直到flash操作完成。由于仅锁调度不屏蔽中断，此时中断还会得到响应， \n
  ***  这就需要保证中断handler及其调用的所有函数中不涉及任何的flash读写，具体限制体现为：
  ***	  ​1.  不能执行flash上的代码（text）\n
  ***	  2.  不能读取任何位于flash上的常量 （rodata）\n
  ***	  3.  不能调用flash相关api  \n
  ​***  如果将锁调度替换成锁中断，则没有上面的几个限制，但有一定的负面影响，具体表现为： \n
  ​*** flash操作期间，所有中断得不到响应，无论中断触发多少次，等恢复全局中断以后都只会执行一次中断处理， \n
  ​*** 典型的如系统的1ms tick中断，在整个flash操作过程中，tick都不会增长，直到恢复全局中断以后才会增长一次， \n
  ​*** 这就导致系统维护的tick定时和真实时间产生偏离，软件定时器就会受此影响导致真实响应时间晚于预期的时间，其时间差即为flash操作耗时。 \n
  ​*** 其他依赖中断次数的场景皆有类似问题。
  */
void HAL_NVIC_IntRegister(HAL_IRQn_Type IRQn, HAL_IRQnHandle_Type IRQnHandle, uint32_t priority);

/**
  * @brief  This function Unregister interrupt.
  * @param  IRQn: specifies the IRQ source.
  *         This parameter can be one of the IRQn_Type.
  * @retval None
  */
void HAL_NVIC_IntUnregister(HAL_IRQn_Type IRQn);

/**
  * @brief  初始化SysTick中断并开启SysTick中断.
  * @param  TicksNumb: SysTick Reload寄存器里的值.频率为39.168M时，TicksNumb设置为39168，则SysTick中断频率为1K赫兹.
  * @retval status:  - 0  函数执行成功.
  *                  - 1  函数执行失败.
  */
uint32_t HAL_SYSTICK_Config(uint32_t TicksNumb);

/**
  * @brief  配置SysTick时钟源.
  * @param  CLKSource: 具体的SysTick时钟源.
  *			参数可以是以下值:
  *             @arg SYSTICK_CLKSOURCE_HCLK_DIV8: AHB 8分频作为 SysTick 时钟源.
  *             @arg SYSTICK_CLKSOURCE_HCLK: AHB 时钟作为 SysTick 时钟源.
  * @retval 无
  */
void HAL_SYSTICK_CLKSourceConfig(uint32_t CLKSource);

/**
  * @brief  SysTick 中断服务函数.
  * @retval 无
  */
void HAL_SYSTICK_IRQHandler(void);

/**
  * @brief	获取SysTick当前值.
  * @note	此函数被声明为弱函数，用户可根据需求重写该函数.
  * @retval	SysTick当前值
  */
uint32_t HAL_GetTick(void);

/**
  * @brief  SysTick中断回调函数.
  * @retval None
  */
void HAL_SYSTICK_Callback(void);


