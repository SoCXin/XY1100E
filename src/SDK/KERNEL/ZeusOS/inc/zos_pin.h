/*
 * Copyright (c) 2019-2029, ZeusOS Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author            Notes
 * 2021-03-23     longmain       first version
 */

#ifndef __ZOS_PIN_H__
#define __ZOS_PIN_H__

#ifdef __cplusplus
extern "C" {
#endif

#include "zos_def.h"

#define	PIN_2	2
#define	PIN_3	3
#define	PIN_5	5
#define	PIN_6	6
#define	PIN_7	7
#define	PIN_8	8
#define	PIN_9	9
#define	PIN_10	10
#define	PIN_11	11
#define	PIN_12	12
#define	PIN_13	13
#define	PIN_15	15
#define	PIN_16	16
#define	PIN_20	20
#define	PIN_21	21


#define PIN_MAX 				22
#define PIN_ERROR 				-1
#define PIN_LOW                 0x00
#define PIN_HIGH                0x01

#define PIN_MODE_OUTPUT         0x00
#define PIN_MODE_INPUT          0x01
#define PIN_MODE_INPUT_PULLUP   0x02
#define PIN_MODE_INPUT_PULLDOWN 0x03
#define PIN_MODE_OUTPUT_OD      0x04

//#define PIN_IRQ_MODE_RISING             0x00
#define PIN_IRQ_MODE_FALLING            0x01
#define PIN_IRQ_MODE_RISING_FALLING     0x02
#define PIN_IRQ_MODE_HIGH_LEVEL         0x03
//#define PIN_IRQ_MODE_LOW_LEVEL          0x04

#define PIN_IRQ_DISABLE                 0x00
#define PIN_IRQ_ENABLE                  0x01

#define PIN_IRQ_PIN_NONE                -1


struct zos_pin_irq_hdr
{
    zos_int16_t        pin;
    zos_uint16_t       mode;
    void (*hdr)(void *args);
    void             *args;
};

typedef void (*PIN_CALLBACK)(void);

zos_err_t zos_pin_mode(zos_uint8_t pin, zos_base_t mode);
zos_err_t zos_pin_deinit(zos_uint8_t pin);
zos_err_t zos_pin_write(zos_uint8_t pin, zos_base_t value);
zos_err_t zos_pin_read(zos_uint8_t pin);
zos_err_t zos_pin_attach_irq(zos_uint8_t pin,zos_uint32_t mode, PIN_CALLBACK callback);
zos_err_t zos_pin_detach_irq(zos_uint8_t pin);
zos_err_t zos_pin_irq_enable(zos_uint8_t pin,zos_uint32_t enabled);

#ifdef __cplusplus
}
#endif
#endif //__ZOS_PIN_H__
