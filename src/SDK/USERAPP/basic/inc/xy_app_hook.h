/**
* @file        xy_app_hook.h
* @brief       与芯翼平台业务相关的客户回调接口，客户必须自行实现自己的策略
* @attention   用户在回调接口中不宜做耗时太长的事务。
* @note    
*/

#pragma once
#include <stdint.h>

/**
* @brief FOTA升级状态
*/
typedef enum  {
    OTA_FIRMWARE_DOWNLOADING    = 0,   //enter to downloading
    OTA_FIRMWARE_DOWNLOADED     = 2,   //download finish
    OTA_FIRMWARE_UPDATING,             //updating
    OTA_FIRMWARE_UPDATE_SUCCESS,       //update success
    OTA_FIRMWARE_UPDATE_FAILED         //update fail
}XY_FOTA_STATE;

/**
 * @brief 云平台获取设备厂商信息函数
 * @param manufacturer 厂商信息
 * @param size  信息长度
 * @warning  该函数返回的信息为预设值，用户可根据各自需求进行修改
 */
void xy_get_Manufacturer(char *manufacturer, int size);

/**
 * @brief 云平台获取设备SN的函数
 * @param serial_number SN
 * @param size   SN长度
 * @warning  该函数返回的信息为预设值，用户可根据各自需求进行修改
 */
void xy_get_SNumber(char *serial_number, int size);

/**
 * @brief 设备FOTA升级状态的HOOK函数
 * @param state FOTA升级状态,see @ref XY_FOTA_STATE
 * @warning  该HOOK函数穿插在升级的不同状态中，用户在实现该函数体内容时，需要确保不能执行耗时过久或阻塞的行为，以免对芯翼的升级流程产生干扰。
 * @note   该接口仅限于芯翼自研FOTA方案中使用，对于艾拉比及其他FOTA方案不适用
 */
void xy_fota_state_hook(XY_FOTA_STATE state);

/**
 * @brief 差分包中用户携带的私有数据HOOK函数
 * @param data 用户私有数据指针
 * @param len  用户私有数据长度
 * @warning  不建议用户使用这个私有数据能力，若需要使用，务必与芯翼FAE确认清楚
 * @note   该接口仅限于芯翼自研FOTA方案中使用，对于艾拉比及其他FOTA方案不适用
 */
bool xy_fota_private_data_hook(uint8_t *data, int len);



