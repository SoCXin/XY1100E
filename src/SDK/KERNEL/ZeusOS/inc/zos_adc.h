/*
 * Copyright (c) 2019-2029, ZeusOS Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author            Notes
 * 2021-03-23     longmain       first version
 */


#ifndef __ZOS_ADC_H__
#define __ZOS_ADC_H__

#include "zos_def.h"

typedef enum
{
	ZOS_ADC_BUS_0,
    ZOS_ADC_BUS_VBAT,
	ZOS_ADC_BUS_NONE
} ZOS_ADC_BUS;


/**
 *  \brief ADC读取，电压范围0~1000mV
 *
 *  \return 电压值，mV
 *
 *  \details  More details
 */
zos_base_t zos_adc_read(ZOS_ADC_BUS channel);

/**
 *  \brief 获取VBAT
 *  
 *  \return 电压值，mV
 *  
 *  \details  More details
 */
zos_uint32_t zos_vbat_read(void);

#endif
