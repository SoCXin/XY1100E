/** 
* @file      xy_fota_utils.h
* @brief     FOTA相关的公共接口，如flash内存分布，FOTA状态位保存API等
* @attention   该头文件供芯翼自研FOTA和第三方FOTA方案共同使用，如艾拉比、广升、红石等。如果客户有其他需求，请联系FAE
*/

#pragma once
#include <stdint.h>
#include "xy_fota.h"

/**
 * @brief 获取存放差分包的FLASH的信息：FLASH区域起始地址和大小
 * @param addr FLASH区域首地址
 * @param len  FLASH区域长度
 * @note  该接口可供用户调用，获取差分包存放flash大小，以检测当前差分包能够正常下载保存到flash中
 */
void xy_get_OTA_flash_info(uint32_t *addr, int *len);

/**
 * @brief 获取本地FOTA升级检测的周期时长，单位为秒
 * @note  该接口仅用于本地主动检查FOTA是否升级的场景，如通过http；\n
 *** 对于公有云，如onenet、ctwing等，无需使用该接口，而是由云端主动推送。
 */
int  xy_get_OTA_period();

/**
 * @brief 设置本地FOTA升级检测的周期时长，单位为天
 * @note  该接口仅用于本地主动检查FOTA是否升级的场景，如通过http；\n
 *** 对于公有云，如onenet、ctwing等，无需使用该接口，而是由云端主动推送。
 */
void  xy_set_OTA_period(int days);

/**
 * @brief 检查本地FOTA升级是否已经超时，若超时，则应该与服务器进行链接，尝试FOTA升级
 * @return  bool,see @ref xy_ret_Status_t. XY_OK表示已超时，需要与服务器联系，尝试执行FOTA
 *
 * @note  该接口仅用于本地主动检查FOTA是否升级的场景，如通过http；\n
 *** 对于公有云，如onenet、ctwing等，无需使用该接口，而是由云端主动推送。
 * @warning  建议在确定TCPIP网路已通情况下调用该接口，否则会报XY_Err_NoConnected错误。
 */
int  xy_check_OTA_timeout();

/**
 * @brief 记录最新一次FOTA升级检查的时刻点，以便下次系统上电时检查是否需要FOTA升级检查
 * @note   该接口在与FOTA服务器通信完成后调用，不能在任务初始化时调用，以防止由于无线环境等原因未与FOTA服务器通信而错误更新时刻点
 * @warning  仅用于终端本地触发的FOTA升级检查，且用户必须调用，否则会造成每次上电后都会连接服务器进行FOTA是否升级的检查
 */
void  update_last_OTA_check_time();


