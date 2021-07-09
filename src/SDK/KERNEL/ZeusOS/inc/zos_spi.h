/**
 *  @file    zos_spi.h
 *  @brief   ZeusOS SPI驱动
 *
 * Copyright (c) 2019-2029, ZeusOS Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author            Notes
 * 2021-03-23     longmain       first version
 */

#ifndef __ZOS_SPI_H__
#define __ZOS_SPI_H__
#include "zos_def.h"


#define SPI_MOSI_PIN        HAL_GPIO_PIN_NUM_6
#define SPI_MISO_PIN        HAL_GPIO_PIN_NUM_7
#define SPI_SCLK_PIN        HAL_GPIO_PIN_NUM_8
#define SPI_NSS1_PIN        HAL_GPIO_PIN_NUM_9
#define SPI_NSS2_PIN        0


typedef enum
{
    ZOS_SPI_BUS_0,
    ZOS_SPI_BUS_LCD,
	ZOS_SPI_BUS_NONE
} ZOS_SPI_BUS;

typedef enum
{
	ZOS_SPI_CLK_MODE0,
	ZOS_SPI_CLK_MODE1,
	ZOS_SPI_CLK_MODE2,
	ZOS_SPI_CLK_MODE3
} ZOS_SPI_CLK_MODE;

typedef enum
{
	ZOS_SPI_CLK_DIV2,
	ZOS_SPI_CLK_DIV4,
	ZOS_SPI_CLK_DIV8,
	ZOS_SPI_CLK_DIV16,
	ZOS_SPI_CLK_DIV32  ,
	ZOS_SPI_CLK_DIV64  ,
	ZOS_SPI_CLK_DIV128,
	ZOS_SPI_CLK_DIV256
} ZOS_SPI_CLK_DIV;

typedef struct
{
	ZOS_SPI_CLK_MODE  clk_mode;
	ZOS_SPI_CLK_DIV  clk_div;
} ZOS_SPI_CONFIG;

/**
 *  \brief spi初始化
 *
 *  \param [in] dev spi设备
 *  \param [in] config spi配置
 *  \return  0：初始化成功，其他：初始化失败
 *
 *  \details 仅支持主机模式，其工作模式如下
 *    SPI_CLK_MODE0  CCHO = 0, CPOL = 0;
 *    SPI_CLK_MODE1  CCHO = 1, CPOL = 0;
 *    SPI_CLK_MODE2  CCHO = 0, CPOL = 1;
 *    SPI_CLK_MODE3  CCHO = 1, CPOL = 1;
 *    SPI frequency = 39.168M
 */
zos_err_t zos_spi_init(ZOS_SPI_BUS dev, ZOS_SPI_CONFIG *config);


/**
 *  \brief spi去初始化
 *
 *  \param [in] dev spi设备
 *  \return  none
 */
void zos_spi_deinit(ZOS_SPI_BUS dev);


/**
 *  \brief spi写数据
 *
 *  \param [in] dev  spi设备
 *  \param [in] data 待写数据
 *  \param [in] len  待写数据长度
 *  \return  0：写数据成功，其他：写数据失败
 */
zos_err_t zos_spi_write(ZOS_SPI_BUS dev, zos_uint8_t *data, zos_uint16_t len);


/**
 *  \brief spi读数据
 *
 *  \param [in] dev  spi设备
 *  \param [in] w_data 写命令
 *  \param [in] w_len  写命令长度
 *  \param [out] r_data 读取数据存放地址，需先申请内存 
 *  \param [in] r_len  读取数据长度
 *  \return  0：读数据成功，其他：读数据失败
 */
zos_err_t zos_spi_read(ZOS_SPI_BUS dev, zos_uint8_t *w_data, zos_uint16_t w_len,  zos_uint8_t *r_data, zos_uint16_t r_len);


#endif
