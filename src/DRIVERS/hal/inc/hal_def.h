/**
  ******************************************************************************
  * @file    hal_def.h
  * @brief   HAL库常用的宏、定义、枚举和结构体等.
  ******************************************************************************
  */

 #pragma once

 #include<stddef.h>
 #include<stdint.h>

/**
  * @brief  HAL状态枚举定义
  *         HAL_OK：正常\n
  *         HAL_ERROR：错误\n
  *         HAL_BUSY：忙\n
  *         HAL_TIMEOUT：超时\n
  *         HAL_LOST：丢失
  */
typedef enum
{
  HAL_OK       = 0x00U,
  HAL_ERROR    = 0x01U,
  HAL_BUSY     = 0x02U,
  HAL_TIMEOUT  = 0x03U,
  HAL_LOST     = 0x04U
} HAL_StatusTypeDef;

/**
  * @brief  HAL设备锁枚举定义
  *         HAL_UNLOCKED：设备未上锁\n
  *         HAL_LOCKED：设备处于上锁状态
  */
typedef enum
{
  HAL_UNLOCKED = 0x00U,
  HAL_LOCKED   = 0x01U
} HAL_LockTypeDef;

/**
  * @brief  HAL标志或中断状态枚举定义
  *         HAL_RESET：未置位\n
  *         HAL_SET：已置位
  */
typedef enum
{
  HAL_RESET = 0,
  HAL_SET = !HAL_RESET
} HAL_FlagStatus, HAL_ITStatus;

/**
  * @brief  HAL使能/失能状态枚举定义
  *         HAL_DISABLE：失能\n
  *         HAL_ENABLE：使能
  */
typedef enum 
{
  HAL_DISABLE = 0,
  HAL_ENABLE = !HAL_DISABLE
} HAL_FunctionalState;

/*!< 判断是否是使能/失能状态*/
#define HAL_IS_FUNCTIONAL_STATE(STATE) (((STATE) == HAL_DISABLE) || ((STATE) == HAL_ENABLE))

/**
  * @brief  HAL成功/失败状态枚举定义
  *         HAL_SUCCESS：成功\n
  *         HAL_FAIL：失败
  */
typedef enum
{
  HAL_SUCCESS = 0U,
  HAL_FAIL = !HAL_SUCCESS
} HAL_ErrorStatus;

/*!< 置位某一位*/
#define SET_BIT(REG, BIT)     ((REG) |= (BIT))

/*!< 清零某一位*/
#define CLEAR_BIT(REG, BIT)   ((REG) &= ~(BIT))

/*!< 读取某一位*/
#define READ_BIT(REG, BIT)    ((REG) & (BIT))

/*!< 清零入参值*/
#define CLEAR_REG(REG)        ((REG) = (0x0))

/*!< 给入参赋值*/
#define WRITE_REG(REG, VAL)   ((REG) = (VAL))

/*!< 读取入参值*/
#define READ_REG(REG)         ((REG))

/*!< 将CLEARMASK对应位置位为SETMASK，其余位保持不变*/
#define MODIFY_REG(REG, CLEARMASK, SETMASK)  WRITE_REG((REG), (((READ_REG(REG)) & (~(CLEARMASK))) | (SETMASK)))

/*!< 先反转以LSB对齐然后计算前导零，一般用来获取表达优先级的位数*/
#define POSITION_VAL(VAL)     (__CLZ(__RBIT(VAL))) 

/*!< 最大延迟时间*/
#define HAL_MAX_DELAY      0xFFFFFFFFU

/*!< 判断入参REG的BIT位是否置位或为零*/
#define HAL_IS_BIT_SET(REG, BIT)         (((REG) & (BIT)) != 0U)
#define HAL_IS_BIT_CLR(REG, BIT)         (((REG) & (BIT)) == 0U)

/*!< 防止编译器报错*/
#define UNUSED(X) (void)X

/**
  * @brief  给__HANDLE__成员State的值清零
  * @param  __HANDLE__：外设控制结构体
  */
#define __HAL_RESET_HANDLE_STATE(__HANDLE__) ((__HANDLE__)->State = 0U)

/**
  * @brief  设备上锁函数
  * @param  __HANDLE__：外设控制结构体
  */
#define __HAL_LOCK(__HANDLE__)                                           \
								do{                                        \
									if((__HANDLE__)->Lock == HAL_LOCKED)   \
									{                                      \
									   return HAL_BUSY;                    \
									}                                      \
									else                                   \
									{                                      \
									   (__HANDLE__)->Lock = HAL_LOCKED;    \
									}                                      \
								  }while (0U)

/**
  * @brief  设备解锁函数
  * @param  __HANDLE__：外设控制结构体
  */
#define __HAL_UNLOCK(__HANDLE__)                                          \
								  do{                                       \
									  (__HANDLE__)->Lock = HAL_UNLOCKED;    \
									}while (0U)

/*!< 弱函数前缀声明的宏定义*/
#ifndef __weak
#define __weak   __attribute__((weak))
#endif

/*!< 字节对齐前缀声明的宏定义*/
#ifndef __packed
#define __packed __attribute__((__packed__))
#endif

/*!< 函数强制不内联前缀声明的宏定义*/
#ifndef __NOINLINE
#define __NOINLINE __attribute__ ( (noinline) )
#endif

/*!< 将函数放在RAM上的前缀声明的宏定义*/
#ifndef __RAM_FUNC 
#define __RAM_FUNC __attribute__((section(".ramtext")))
#endif
