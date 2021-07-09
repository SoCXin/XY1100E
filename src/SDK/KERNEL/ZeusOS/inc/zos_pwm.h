/*
 * Copyright (c) 2019-2029, ZeusOS Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author            Notes
 * 2021-03-23     longmain       first version
 */

#ifndef __ZOS_PWM_H__
#define __ZOS_PWM_H__

#include "zos_def.h"

typedef enum
{
	ZOS_PWM_BUS_1,
	ZOS_PWM_BUS_NONE
} ZOS_PWM_BUS;


/**
 *  \brief PWM 初始化
 *
 *  \param [in] bus		pwm设备
 *  \param [in] pin		pwm输出IO引脚号
 *  \param [in] period	pwm周期(us)
 *  \param [in] pulse	pwm占空比(us)
 *  \return  0：初始化成功，-1：初始化失败
 */
zos_err_t zos_pwm_init(ZOS_PWM_BUS bus,zos_uint8_t pin,zos_uint32_t period,zos_uint32_t pulse);


/**
 *  \brief PWM 开启
 *
 *  \param [in] bus		pwm设备
 *
 *  \return  0：初始化成功，-1：初始化失败
 */
zos_err_t zos_pwm_enable(ZOS_PWM_BUS bus);

/**
 *  \brief PWM 关闭
 *
 *  \param [in] bus		pwm设备
 *
 *  \return  0：初始化成功，-1：初始化失败
 */
zos_err_t zos_pwm_disable(ZOS_PWM_BUS bus);


/**
 *  \brief PWM 设置占空比
 *
 *  \param [in] bus		pwm设备
 *  \param [in] pulse	占空比
 *
 *  \return  0：初始化成功，-1：初始化失败
 */
zos_err_t zos_pwm_set(ZOS_PWM_BUS bus,zos_uint32_t pulse);


#endif
