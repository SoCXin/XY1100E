/*
 * Copyright (c) 2019-2029, ZeusOS Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author            Notes
 * 2021-03-23     longmain       first version
 */


#ifndef __ZOS_FLASH_H__
#define __ZOS_FLASH_H__
#include "zos_def.h"

//The first flash address available to the user.
#define  ZOS_FLASH_USER_EXTEND_BASE                         0x271FC000

//Maximun flash size available to users.
#define  ZOS_FLASH_USER_EXTEND_LEN_MAX                      0x20000 //用户扩展存储共128k

//The first flash address available to the user.
#define  ZOS_FLASH_USER_NV_BASE                         0x271FC000

//Maximun flash size available to users.
#define  ZOS_FLASH_USER_NV_LEN_MAX                      0x4000 //NV掉电存储共16K

/**
 * @brief  read data from flash。一个扇区4K字节耗时183us
 * 
 * @param addr     [IN]The address where the data is stored in the flash.
 * @param data     [IN]The first address of the data to be written to ram.
 * @param size     [IN]The length of data.
 */
void zos_flash_read(zos_uint32_t addr, void *data, zos_uint32_t size);

/**
 * @brief  Erase and write new data to flash. 一个扇区4K字节耗时54ms
 * 
 * @param addr     [IN]The address where the data is stored in the flash.
 * @param data     [IN]The first address of the data to be written to flash.Must cache aligned
 * @param size     [IN]The length of the data. 
 * @return BOOL, see  @ref  xy_ret_Status_t.
 * @attention      由于异常断电会造成flash内容损坏，所以客户必须自行添加头尾校验位来确保内容的有效性。 
 * @warning        该接口会执行擦除和写入两个动作，对于非整个扇区写入的场景，建议客户统一先执行xy_flash_erase，再多次调用xy_flash_write_no_erase接口，以降低坏块风险！
 */
zos_err_t zos_flash_write(zos_uint32_t addr,void *data, zos_uint32_t size);

/**
 * @brief   Write new data to flash without erase. such as  FOTA packet download。一个扇区4K字节耗时9ms
 * 
 * @param addr     [IN]The address where the data is stored in the flash.
 * @param data     [IN]The first address of the data to be written to flash.Must cache aligned
 * @param size     [IN]The length of the data. 
 * @return BOOL, see  @ref  xy_ret_Status_t.
 * @attention      由于异常断电会造成flash内容损坏，所以客户必须自行添加头尾校验位来确保内容的有效性。 
 * @warning        使用该接口的前提是能够保证待写入的flash空间已被擦除为全FF，否则会造成脏数据。所以必须与xy_Flash_Erase接口配套使用，即先擦除一大段flash，再一小段一小段分时写入。
 */
zos_err_t zos_flash_write_no_erase(zos_uint32_t addr,void *data, zos_uint32_t size);

/**
 * @brief   Erease block.一个扇区4K字节耗时45ms
 * 
 * @param addr     [IN]The address where the data is stored in the flash.
 * @param size     [IN]The length of the data. 
 * @return BOOL, see  @ref  xy_ret_Status_t.
 */
zos_err_t zos_flash_erase(zos_uint32_t addr, zos_uint32_t size);

/**
 * @brief   检查flash地址是否为FTL磨损的flash地址范围
 * 
 * @return BOOL, see  @ref  xy_ret_Status_t.
 */
zos_err_t zos_flash_is_ftl_addr(zos_uint32_t addr);

/**
 * @brief  磨损均衡的写接口，支持单个参数的修改，其他值不变。一个扇区4K字节耗时58ms
 * 
 * @param addr     [IN]磨损扇区的flash首地址，4K字节对齐
 * @param offset   [IN]待写入的内容偏移，常为结构体某变量在结构体中的偏移
 * @param data     [IN]The first address of the data to be written to flash.
 * @param size     [IN]The length of the data. 
 * @return BOOL, see  @ref  xy_ret_Status_t.
 * @attention   每个4K的flash区域中用户可用的空间为XY_FTL_AVAILABLE_SIZE，其中头和尾部添加了保护字段。 \n
 *        支持用户按照偏移进行部分读写，以方便结构体具体参数的部分修改。
 */
zos_err_t zos_flash_ftl_write(zos_uint32_t addr, zos_int32_t offset, zos_uint8_t *data, zos_uint32_t size);

/**
 * @brief  磨损均衡的读接口，支持偏移读取。一个扇区4K字节耗时1.2ms
 * 
 * @param addr     [IN]The address where the data is stored in the flash.
 * @param data     [IN]ram address
 * @param size     [IN]The length of the data. 
 * @return BOOL, see  @ref  xy_ret_Status_t.
 * @attention   每个4K的flash区域中用户可用的空间为XY_FTL_AVAILABLE_SIZE，其中头和尾部添加了保护字段。 \n
 *    支持用户按照偏移进行部分读写，即addr&0xFFF为地址偏移值。
 */
zos_err_t zos_flash_ftl_read(zos_uint32_t addr, zos_uint8_t *data, zos_uint32_t size);



/** 
 * @brief   供用户进行磨损均衡的flash注册
 *
 * @param addr   必须为4K字节对齐的，即尾部必须为0X000。
 * @param addr   必须为4K的整数倍，即必须是整的sector，一个sector为4K字节。
 * @attention    凡是使用该接口进行磨损和容错机制的，用户可使用的空间大小皆为XY_FTL_AVAILABLE_SIZE，其中头和尾部添加了保护字段。 \n
 *           用户根据写flash的频繁度，决定size的大小，若size=FLASH_SECTOR_LENGTH，则代表放弃磨损均衡，仅使用芯翼的头尾保护机制，以解决异常断电的flash容错。 \n
 *           客户完全可以自行设计磨损和内容保护机制，可以调用xy_Flash_Read和xy_Flash_Write接口。 \n
 *           目前XY1100的flash寿命为10万次，当写频率小于一小时，必须考虑磨损机制，否则无法达到10年使用期限。
 */
void zos_flash_regist_ftl_req(zos_uint32_t addr, zos_uint32_t size);

#endif
