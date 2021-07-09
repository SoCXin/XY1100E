/**
  ******************************************************************************
  * @file	hal_gpio.h
  * @brief	此文件包含GPIO外设的变量，枚举，结构体定义，函数声明等.
  * @note	对于输入模式，每个GPIO并不同时支持上拉和下拉，仅支持上拉或下拉中的其中一种状态。在XY1100芯片中，GPIO10/11/12仅支持下拉，其余的GPIO仅支持上拉.\n
  * 		对于输出模式，仅支持推挽输出，开漏输出模式目前尚未支持.\n
  *			对于中断触发方式，支持下降沿触发，双边沿触发，高电平触发，上升沿触发目前尚未支持。如果外部输入信号并不平稳，需要外部增加消抖电路\n
  *			Analog模式目前尚未支持.\n
  ******************************************************************************
  */

/* Define to prevent recursive inclusion -------------------------------------*/
#pragma once

/* Includes ------------------------------------------------------------------*/
#include "hal_cortex.h"

/**
  * @brief  GPIO枚举. 实际使用GPIO0 ~ GPIO24，其余为预留.
  */
typedef enum {
	HAL_GPIO_PIN_NUM_0 = 0,
	HAL_GPIO_PIN_NUM_1,
	HAL_GPIO_PIN_NUM_2,
	HAL_GPIO_PIN_NUM_3,
	HAL_GPIO_PIN_NUM_4,
	HAL_GPIO_PIN_NUM_5,
	HAL_GPIO_PIN_NUM_6,
	HAL_GPIO_PIN_NUM_7,
	HAL_GPIO_PIN_NUM_8,
	HAL_GPIO_PIN_NUM_9,
	HAL_GPIO_PIN_NUM_10,
	HAL_GPIO_PIN_NUM_11,
	HAL_GPIO_PIN_NUM_12,
	HAL_GPIO_PIN_NUM_13,
	HAL_GPIO_PIN_NUM_14,
	HAL_GPIO_PIN_NUM_15,
	HAL_GPIO_PIN_NUM_16,
	HAL_GPIO_PIN_NUM_17,
	HAL_GPIO_PIN_NUM_18,
	HAL_GPIO_PIN_NUM_19,
	HAL_GPIO_PIN_NUM_20,
	HAL_GPIO_PIN_NUM_21,
	HAL_GPIO_PIN_NUM_22,
	HAL_GPIO_PIN_NUM_23,
	HAL_GPIO_PIN_NUM_24,
	HAL_GPIO_PIN_NUM_25,
	HAL_GPIO_PIN_NUM_26,
	HAL_GPIO_PIN_NUM_27,
	HAL_GPIO_PIN_NUM_28,
	HAL_GPIO_PIN_NUM_29,
	HAL_GPIO_PIN_NUM_30,
	HAL_GPIO_PIN_NUM_31,
	HAL_GPIO_PIN_NUM_32,
	HAL_GPIO_PIN_NUM_33,
	HAL_GPIO_PIN_NUM_34,
	HAL_GPIO_PIN_NUM_35,
	HAL_GPIO_PIN_NUM_36,
	HAL_GPIO_PIN_NUM_37,
	HAL_GPIO_PIN_NUM_38,
	HAL_GPIO_PIN_NUM_39,
	HAL_GPIO_PIN_NUM_40,
	HAL_GPIO_PIN_NUM_41,
	HAL_GPIO_PIN_NUM_42,
	HAL_GPIO_PIN_NUM_43,
	HAL_GPIO_PIN_NUM_44,
	HAL_GPIO_PIN_NUM_45,
	HAL_GPIO_PIN_NUM_46,
	HAL_GPIO_PIN_NUM_47,
	HAL_GPIO_PIN_NUM_48,
	HAL_GPIO_PIN_NUM_49,
	HAL_GPIO_PIN_NUM_50,
	HAL_GPIO_PIN_NUM_51,
	HAL_GPIO_PIN_NUM_52,
	HAL_GPIO_PIN_NUM_53,
	HAL_GPIO_PIN_NUM_54,
	HAL_GPIO_PIN_NUM_55,
	HAL_GPIO_PIN_NUM_56,
	HAL_GPIO_PIN_NUM_57,
	HAL_GPIO_PIN_NUM_58,
	HAL_GPIO_PIN_NUM_59,
	HAL_GPIO_PIN_NUM_60,
	HAL_GPIO_PIN_NUM_61,
	HAL_GPIO_PIN_NUM_62,
	HAL_GPIO_PIN_NUM_63
} HAL_GPIOPin_TypeDef;


/**
  * @brief  GPIO重映射外设引脚枚举
  */
typedef enum {
	HAL_REMAP_UART_TXD      = 0,
	HAL_REMAP_UART_RTS      = 1,
	HAL_REMAP_UART_RXD      = 2,
	HAL_REMAP_UART_CTS      = 3,
	HAL_REMAP_CSP1_SCLK     = 4,
	HAL_REMAP_CSP1_TXD      = 5,
	HAL_REMAP_CSP1_TFS      = 6,
	HAL_REMAP_CSP1_RXD      = 7,
	HAL_REMAP_CSP1_RFS      = 8,
	HAL_REMAP_CSP2_SCLK     = 9,
	HAL_REMAP_CSP2_TXD      = 10,
	HAL_REMAP_CSP2_TFS      = 11,
	HAL_REMAP_CSP2_RXD      = 12,
	HAL_REMAP_CSP2_RFS      = 13,
	HAL_REMAP_I2C1_SCLK     = 14,
	HAL_REMAP_I2C1_SDA      = 15,
	HAL_REMAP_SPI_SCLK      = 16,
	HAL_REMAP_SPI_MOSI      = 17,
	HAL_REMAP_SPI_NSS1      = 18,
	HAL_REMAP_SPI_MISO      = 19,
	HAL_REMAP_SPI_NSS2      = 20,
	HAL_REMAP_SPI_CS        = 38,
	HAL_REMAP_TMR1_AF_OUTP  = 21,
	HAL_REMAP_TMR1_AF_OUTN  = 22,
	HAL_REMAP_TMR1_AF_INPUT = 23,
	HAL_REMAP_TMR2_AF_OUTP  = 24,
	HAL_REMAP_TMR2_AF_OUTN  = 25,
	HAL_REMAP_TMR2_AF_INPUT = 26,
	HAL_REMAP_TMR3_AF_OUTP  = 27,
	HAL_REMAP_TMR3_AF_OUTN  = 28,
	HAL_REMAP_TMR3_AF_INPUT = 29,
	HAL_REMAP_TMR4_AF_OUTP  = 30,
	HAL_REMAP_TMR4_AF_OUTN  = 31,
	HAL_REMAP_TMR4_AF_INPUT = 32,
	HAL_REMAP_AP_nTRST      = 33,
	HAL_REMAP_AP_SWCLKTCK   = 34,
	HAL_REMAP_AP_TDI        = 35,
	HAL_REMAP_AP_TDO        = 36,
	HAL_REMAP_AP_SWDIOTMS   = 37,
	HAL_REMAP_DSP_JTRST     = 39,
	HAL_REMAP_DSP_JTCK      = 40,
	HAL_REMAP_DSP_JTDI      = 41,
	HAL_REMAP_DSP_JTMS      = 42,
	HAL_REMAP_DSP_JTDO      = 43,
	HAL_REMAP_GPI3          = 44,
	HAL_REMAP_GPI2          = 45,
	HAL_REMAP_GPI1          = 46,
	HAL_REMAP_GPI0          = 47,
	HAL_REMAP_I2C2_SCLK     = 48,
	HAL_REMAP_I2C2_SDA      = 49,
	HAL_REMAP_CSP3_SCLK     = 50,
	HAL_REMAP_CSP3_TXD      = 51,
	HAL_REMAP_CSP3_TFS      = 52,
	HAL_REMAP_CSP3_RXD      = 53,
	HAL_REMAP_CSP3_RFS      = 54,
	HAL_REMAP_CSP4_SCLK     = 55,
	HAL_REMAP_CSP4_TXD      = 56,
	HAL_REMAP_CSP4_TFS      = 57,
	HAL_REMAP_CSP4_RXD      = 58,
	HAL_REMAP_CSP4_RFS      = 59
} HAL_GPIORemap_TypeDef;


/** 
  * @brief  GPIO状态枚举 
  */   
typedef enum
{
	HAL_GPIO_PIN_RESET = 0u,	/*!< GPIO 低电平状态 */
	HAL_GPIO_PIN_SET			/*!< GPIO 高电平状态 */
} HAL_GPIO_PinState;


/**
  * @brief GPIO初始化结构体
  */
typedef struct
{
  HAL_GPIOPin_TypeDef Pin;			/*!< 具体配置的GPIO. 详情参考 @ref HAL_GPIOPin_TypeDef */
  uint32_t Mode;					/*!< GPIO工作模式 */
  uint32_t Pull;	                /*!< GPIO上下拉模式 */
  HAL_GPIORemap_TypeDef Alternate;	/*!< GPIO重映射引脚. 详情参考 @ref HAL_GPIORemap_TypeDef */
}HAL_GPIO_InitTypeDef;

/**  @brief GPIO 可配置模式 \n
  *			定义规则	: 0xX0x0y0YZ \n
  *           - X  : 用于区分普通GPIO模式、中断模式 \n
  *           - x  : 用于区分GPIO中断类型 \n
  *           - y  : 用于区分普通GPIO模式、复用模式 \n
  *           - Y  : 用于区分GPIO推挽输出、开漏输出 \n
  *           - Z  : 用于区分GPIO输入模式、输出模式
  */


#define  GPIO_MODE_AF_PER                		0x00001000U   /*!< 复用外设模式			*/

#define  GPIO_MODE_AF_INPUT                  	0x00002000U   /*!< 复用外设输入模式		*/
#define  GPIO_MODE_AF_PP                  		0x00002001U   /*!< 复用外设推挽输出模式	*/
#define  GPIO_MODE_AF_OD                  		0x00002011U   /*!< 复用外设开漏输出模式	*/


#define  GPIO_MODE_INPUT                        0x00000000U   /*!< 普通GPIO输入模式		*/

#define  GPIO_MODE_IT_FALLING                   0x10200000U   /*!< 下降沿外部中断模式	*/
#define  GPIO_MODE_IT_RISING_FALLING            0x10400000U   /*!< 双边沿外部中断模式	*/
#define  GPIO_MODE_IT_HIGH_LEVEL                0x10800000U   /*!< 高电平外部中断模式	*/

#define  GPIO_MODE_OUTPUT_PP                    0x00000001U   /*!< 推挽输出模式	*/
#define  GPIO_MODE_OUTPUT_OD                    0x00000011U   /*!< 开漏输出模式	*/

#define  GPIO_MODE_INOUT                        0x00000003U   /*!< 输入输出模式	*/



 /**
   * @brief GPIO 上拉下拉状态
   */
#define  GPIO_NOPULL        0x00000000U   /*!< 无上下拉  */
#define  GPIO_PULLUP        0x00000001U   /*!< 上拉		*/
#define  GPIO_PULLDOWN      0x00000002U   /*!< 下拉		*/

/**
  * @brief  初始化GPIO.
  * @param  GPIO_Init 指向一个包含GPIO具体配置信息的 HAL_GPIO_InitTypeDef 结构体的指针.详情参考 @ref HAL_GPIO_InitTypeDef.
  * @retval 无
  */
void HAL_GPIO_Init(HAL_GPIO_InitTypeDef* GPIO_Init);

/**
  * @brief  去初始化GPIO.
  * @param  GPIO_Init 指向一个包含GPIO具体配置信息的 HAL_GPIO_InitTypeDef 结构体的指针.详情参考 @ref HAL_GPIO_InitTypeDef.
  * @retval 无
  */
void HAL_GPIO_DeInit(HAL_GPIOPin_TypeDef GPIO_Pin);

/**
  * @brief  读取GPIO输入状态.
  * @param  GPIO_Pin 具体的GPIO引脚.详情参考 @ref HAL_GPIOPin_TypeDef.
  * @retval 引脚状态.详情参考 @ref HAL_GPIO_PinState.
  */
HAL_GPIO_PinState HAL_GPIO_ReadPin(HAL_GPIOPin_TypeDef GPIO_Pin);

/**
  * @brief  设置GPIO输出状态.
  * @param  GPIO_Pin 具体的GPIO引脚.详情参考 @ref HAL_GPIOPin_TypeDef.
  * @param  PinState 设置的输出状态.详情参考 @ref HAL_GPIO_PinState.
  *         参数可以是以下枚举值:
  *				@arg HAL_GPIO_PIN_RESET:	GPIO输出低电平
  *				@arg HAL_GPIO_PIN_SET:		GPIO输出高电平
  * @retval 无
  */
void HAL_GPIO_WritePin(HAL_GPIOPin_TypeDef GPIO_Pin, HAL_GPIO_PinState PinState);

/**
  * @brief  翻转GPIO引脚输出状态.
  * @param  GPIO_Pin 具体的GPIO引脚.详情参考 @ref HAL_GPIOPin_TypeDef.
  * @note	此函数只能用于GPIO模式设置为输出模式时
  * @retval 无
  */
void HAL_GPIO_TogglePin(HAL_GPIOPin_TypeDef GPIO_Pin);

/**
  * @brief  GPIO中断控制.
  * @param  GPIO_Pin 具体的GPIO引脚.详情参考 @ref HAL_GPIOPin_TypeDef.
  * @param  NewState 使能或者失能GPIO中断.详情参考 @ref HAL_FunctionalState.
  * 		参数可以是以下枚举值:
  *				@arg HAL_ENABLE：	使能GPIO中断
  *				@arg HAL_DISABLE:	失能GPIO中断
  * @retval 无
  */
void HAL_GPIO_IntCtl(HAL_GPIOPin_TypeDef GPIOPin, HAL_FunctionalState NewState);

/**
  * @brief  GPIO中断服务函数.
  * @param  无
  * @retval 无
  */
void HAL_GPIO_IRQHandler(void) __RAM_FUNC;

/**
  * @brief  读取并清除GPIO寄存器中断值.
  * @param  GPIO_Pin 具体的GPIO引脚.详情参考 @ref HAL_GPIOPin_TypeDef.
  * @note	1.多个GPIO发生中断时，通过此函数获取GPIO中断状态寄存器的值判断具体触发中断的GPIO.\n
  * 		2.仅清除中断时也通过调用此函数，参数填相应的GPIO，如果多个GPIO设置了中断，则填其中任意一个GPIO即可.
  * @retval GPIO中断状态寄存器的值
  */
uint32_t HAL_GPIO_ReadAndClearIntFlag(HAL_GPIOPin_TypeDef GPIOPin);

/**
  * @brief  注册GPIO中断.用户使用GPIO中断时需调用此函数注册中断.
  * @param  无
  * @retval 无
  */
void HAL_GPIO_IT_REGISTER(void);

