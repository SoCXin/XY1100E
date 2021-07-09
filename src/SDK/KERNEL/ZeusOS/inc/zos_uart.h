/**
 *  @file    zos_uart.h
 *  @brief   ZeusOS UART驱动
 *
 * Copyright (c) 2019-2029, ZeusOS Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author            Notes
 * 2021-03-23     longmain       first version
 */


#ifndef __ZOS_UART_H__
#define __ZOS_UART_H__

#include "zos_def.h"

typedef enum
{
	ZOS_UART_BUS_1,
	ZOS_UART_BUS_NONE
} ZOS_UART_BUS;

typedef enum
{
	ZOS_UART_DATA_BITS_6,
	ZOS_UART_DATA_BITS_7,
	ZOS_UART_DATA_BITS_8
} UART_DATA_BITS;

typedef enum
{
	ZOS_UART_PARITY_NONE,
	ZOS_UART_PARITY_ODD,
	ZOS_UART_PARITY_EVEN
} UART_PARITY;

typedef enum
{
	ZOS_UART_STOP_BITS_1,
	ZOS_UART_STOP_BITS_2
} UART_STOP_BITS;

typedef struct
{
    unsigned int  baud_rate;
    UART_DATA_BITS  data_bits;
    UART_PARITY  parity;
    UART_STOP_BITS  stop_bits;
} ZOS_UART_CONFIG;

typedef void (*ZOS_UART_RECV_CALLBACK)(zos_uint8_t *rev_data, zos_uint16_t len);

/**
 *  \brief 串口初始化
 *
 *  \param [in] dev 串口设备号
 *  \param [in] config 串口配置
 *  \param [in] callback 串口接收中断回调函数
 *  \return  0：初始化成功，其他：初始化失败
 *
 *  \details  串口波特率支持2400,4800,9600,19200,38400,57600,115200。
 *
 *  \details 校验位仅UART_BUS_1有效。
 */
zos_err_t zos_uart_init(ZOS_UART_BUS dev,  ZOS_UART_CONFIG *config,  ZOS_UART_RECV_CALLBACK callback);

/**
 *  \brief 串口去初始化
 *
 *  \param [in] dev 串口设备号
 *  \return  none
 *
 *  \details  More details
 */
void zos_uart_deinit(ZOS_UART_BUS dev);

/**
 *  \brief 串口写数据
 *
 *  \param [in] dev 串口设备号
 *  \param [in] data 待发送数据首地址
 *  \param [in] len 待发送数据长度
 *  \return  0：写数据成功，其他：写数据失败
 *
 *  \details  More details
 */
zos_err_t zos_uart_write(ZOS_UART_BUS dev, zos_uint8_t *data, zos_uint16_t len);

#endif
