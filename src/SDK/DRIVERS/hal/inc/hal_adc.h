/**
  ******************************************************************************
  * @file   hal_adc.h
  * @brief  此文件包含ADC外设的变量，枚举，结构体定义，函数声明等.
  * @note   ADC参考电压是1V，精度12bit, 采样频率有240K和480K可选。校准后的偏差+-3mv。\n
  *         ADC支持双端模式，双端模式需要两个电压输入信号，双端模式的电压是差分电压。\n
  *         当量程设置为1V时（目前仅支持1V量程）：\n
  *         双端模式，P端接地，量程为-1V~0V，输入正电压读数均为负数；\n
  *         双端模式，N端接地，量程为0~1V；\n
  *         双端模式，两端接信号，量程为 -1V~1V。\n
  *         内部电压模式，量程为0-5V。无需接线，测量结果为芯片内部VBAT电压。\n
  ******************************************************************************
  */

/* Define to prevent recursive inclusion -------------------------------------*/
#pragma once

/* Includes ------------------------------------------------------------------*/
#include "hal_cortex.h"

/** 
  * @brief  ADC工作模式枚举 
**/
typedef enum {
  HAL_ADC_MODE_TYPE_DOUBLE_P  = 0, /*!< 双端P模式, P 端连接信号端, N 端接地   */
  HAL_ADC_MODE_TYPE_DOUBLE_N,      /*!< 双端N模式, N 端连接信号端, P 端接地   */
  HAL_ADC_MODE_TYPE_DOUBLE_PN,     /*!< 双端PN模式, P 和 N 端都连接信号端     */
  HAL_ADC_MODE_TYPE_SINGLE_P,      /*!< 单端P模式, P 端连接信号端             */
  HAL_ADC_MODE_TYPE_SINGLE_N,      /*!< 单端N模式, N 端连接信号端             */
  HAL_ADC_MODE_TYPE_VBAT           /*!< 内部电压模式, 无需接线                */
} HAL_ADCMode_TypeDef;

/** 
  * @brief  ADC工作采样频率枚举 
**/
typedef enum {
	HAL_ADC_SPEED_TYPE_480K = 0x1F,  /*!< 480K工作采样频率  */ 
	HAL_ADC_SPEED_TYPE_240K = 0x3F   /*!< 240K工作采样频率  */
} HAL_ADCSpeed_TypeDef;

/** 
  * @brief  ADC电压测量范围枚举 
**/
typedef enum {
	HAL_ADC_RANGE_TYPE_1V = 0x00,    /*!< 1V电压测量范围  */
} HAL_ADCRange_TypeDef;

/** 
  * @brief  ADC错误类型枚举 
**/
typedef enum {
  HAL_ADC_ERROR_MODE_ERROR      = -32768, /*!< 工作模式错误     */
  HAL_ADC_ERROR_NO_CALIBRATION  = -32767, /*!< 未校验错误       */
  HAL_ADC_ERROR_RANGE_ERROR     = -32766  /*!< 电压测量范围错误 */
} HAL_ADCError_TypeDef;

/** 
  * @brief  ADC初始化结构体 
**/
typedef struct
{
	HAL_ADCMode_TypeDef Mode;           /*!< ADC工作模式      */
	HAL_ADCSpeed_TypeDef Speed;         /*!< ADC工作采样频率  */
	HAL_ADCRange_TypeDef ADCRange;      /*!< ADC电压测量范围  */
} HAL_ADC_InitTypeDef;

/** 
  * @brief  ADC 控制结构体 
**/
typedef struct
{
	HAL_ADC_InitTypeDef Init;   /*!< ADC初始化结构体*/
} HAL_ADC_HandleTypeDef;


/**
  * @brief  初始化ADC外设.       
  * @param  hadc 指向一个包含ADC具体配置信息的 HAL_ADC_HandleTypeDef 结构体的指针.
  *         详情参考 @ref HAL_ADC_HandleTypeDef.
  * @retval HAL status.详情参考 @ref HAL_StatusTypeDef.
  */
HAL_StatusTypeDef HAL_ADC_Init(HAL_ADC_HandleTypeDef *hadc);

/**
  * @brief  去初始化ADC外设.
  * @param  hadc 指向一个包含ADC具体配置信息的 HAL_ADC_HandleTypeDef 结构体的指针.
  *         详情参考 @ref HAL_ADC_HandleTypeDef.
  * @retval HAL status.详情参考 @ref HAL_StatusTypeDef.
  */
HAL_StatusTypeDef HAL_ADC_DeInit(HAL_ADC_HandleTypeDef *hadc);

/**
  * @brief  获取数据寄存器值.
  * @param  hadc 指向一个包含ADC具体配置信息的 HAL_ADC_HandleTypeDef 结构体的指针.
  *         详情参考 @ref HAL_ADC_HandleTypeDef.
  * @retval 有符号的数据寄存器值.
  */
int32_t HAL_ADC_GetValue(HAL_ADC_HandleTypeDef *hadc);

/**
  * @brief  将数据寄存器值转换为中间值.
  * @param  hadc 指向一个包含ADC具体配置信息的 HAL_ADC_HandleTypeDef 结构体的指针.
  *         详情参考 @ref HAL_ADC_HandleTypeDef.
  * @retval 有符号的中间值.
  */
int32_t HAL_ADC_GetConverteValue(HAL_ADC_HandleTypeDef *hadc, int32_t realvalue);

/**
  * @brief  将中间值转换为未经过校验的电压值.
  * @param  hadc 指向一个包含ADC具体配置信息的 HAL_ADC_HandleTypeDef 结构体的指针.
  *         详情参考 @ref HAL_ADC_HandleTypeDef.
  * @retval 有符号的未经过校验的电压值.
  */
int32_t HAL_ADC_GetVoltageValue(HAL_ADC_HandleTypeDef *hadc, int32_t conv_val);

/**
  * @brief  将中间值转换为圆整值.
  * @param  hadc 指向一个包含ADC具体配置信息的 HAL_ADC_HandleTypeDef 结构体的指针.
  *         详情参考 @ref HAL_ADC_HandleTypeDef.
  * @retval 圆整值
  */
int32_t HAL_ADC_GetRoundingValue(HAL_ADC_HandleTypeDef *hadc, int32_t conv_val);

/**
  * @brief  将中间值转换为校验后的电压值.
  * @param  hadc 指向一个包含ADC具体配置信息的 HAL_ADC_HandleTypeDef 结构体的指针.
  *         详情参考 @ref HAL_ADC_HandleTypeDef.
  * @retval 有符号的校验后的电压值.
  */
int32_t HAL_ADC_GetVoltageValueWithCal(HAL_ADC_HandleTypeDef *hadc, int32_t conv_val);




