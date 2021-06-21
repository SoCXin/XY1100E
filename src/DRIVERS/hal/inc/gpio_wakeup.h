/*
 * @file      gpio_wakeup.h
 * @ingroup   peripheral
 * @brief     用于唤醒standby的唤醒源共有三个，分别是wakeupsrc0/wakeupsrc1/wakeupsrc2，对应每个唤醒都可以配置一个gpio作为外部唤醒源
 * @warning   其中gpio10/11/12不可作为外部唤醒源，不使用的唤醒源需将其gpio配置成GPIO_WAKEUP_UNUSE,唤醒触发方式为低电平唤醒，低电平时间不得小于100us，最好不要超过1ms
 */

/* Define to prevent recursive inclusion -------------------------------------*/
#pragma once

#include <stdint.h>
#include <stddef.h>
#include "osal.h"

#define GPIO_WAKEUP 0               //启用引脚唤醒standby的宏开关，0为关闭，1为打开，默认关闭
#define GPIO_WAKEUP_UNUSE 0xff      //不需要唤醒时引脚的配置

extern volatile uint8_t g_gpi_wakeupsrc;//全局声明
extern osMessageQueueId_t gpio_wakeup_msg_q;//消息句柄

typedef struct gpi_wakeupsrc_gpio
{
	volatile uint8_t wakeupsrc0_gpio;//唤醒源0的gpio，不用时需设置成GPIO_WAKEUP_UNUSE，gpio10/11/12不可用
	volatile uint8_t wakeupsrc1_gpio;//唤醒源1的gpio，不用时需设置成GPIO_WAKEUP_UNUSE，gpio10/11/12不可用
	volatile uint8_t wakeupsrc2_gpio;//唤醒源2的gpio，不用时需设置成GPIO_WAKEUP_UNUSE，gpio10/11/12不可用
}WAKEUPSrc_gpio;

/*****************************************************************************************/
/**
  * @func   xy_gpio_wakeup_config(WAKEUPSrc_gpio WakeupSrc_gpio)
  * @brief  用户配置唤醒源的gpio
  * @param  WakeupSrc_gpio：需要配置的gpio
  * @retval none
  */
/*****************************************************************************************/
void xy_gpio_wakeup_config(WAKEUPSrc_gpio WakeupSrc_gpio);

