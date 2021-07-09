/*
 * Copyright (c) 2019-2029, ZeusOS Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author            Notes
 * 2021-03-23     longmain       first version
 */

#ifndef __ZOS_I2C_H__
#define __ZOS_I2C_H__

#include "zos_def.h"
 
typedef enum
{
	ZOS_I2C_BUS_0,
	ZOS_I2C_BUS_NONE
} ZOS_I2C_BUS;

typedef enum
{
	ZOS_I2C_MODE_MASTER,
} ZOS_I2C_MODE_T;

typedef enum
{
    ZOS_I2C_SPEED_100K,
	ZOS_I2C_SPEED_400K
} ZOS_I2C_SPEED_T;


typedef struct
{
	ZOS_I2C_MODE_T  mode;
	ZOS_I2C_SPEED_T  speed;
} ZOS_I2C_CONFIG;

/**
 *  \brief i2c初始化
 *
 *  \param [in] dev i2c设备
 *  \param [in] config i2c配置
 *  \return  0：初始化成功，其他：初始化失败
 *
 *  \details 仅支持主机模式。
 */
zos_err_t zos_i2c_init(ZOS_I2C_BUS dev, ZOS_I2C_CONFIG *config);

/**
 *  \brief i2c去初始化
 *
 *  \param [in] dev i2c设备
 *  \return  none
 *
 *  \details  More details
 */
void zos_i2c_deinit(ZOS_I2C_BUS dev);


/**
 *  \brief i2c写数据
 *
 *  \param [in] dev  i2c设备
 *  \param [in] addr i2c设备地址
 *  \param [in] data 待写数据
 *  \param [in] len  待写数据长度
 *  \return  0：写数据成功，其他：写数据失败
 *
 *  \details  写数据长度不能超过8字节。
 */
zos_err_t zos_i2c_write(ZOS_I2C_BUS dev, zos_uint16_t  addr, zos_uint8_t *data, zos_uint16_t len);

/**
 *  \brief i2c读数据
 *
 *  \param [in] dev  i2c设备
 *  \param [in] addr i2c设备地址
 *  \param [out] data 读取数据存放地址，需先申请内存 
 *  \param [in] len  读取数据长度
 *  \return  0：读数据成功，其他：读数据失败
 *
 *  \details  读取数据长度不能超过8字节。
 */
zos_err_t zos_i2c_read(ZOS_I2C_BUS dev, zos_uint16_t addr, zos_uint8_t *data, zos_uint16_t len);


#endif
