/** 
* @file        xy_fota.h
* @brief       芯翼自研的FOTA算法的用户二次开发接口，主要为差分包的下载和校验相关
* @attention   推荐客户使用艾拉比的FOTA成熟方案，芯翼自研的FOTA方案仅供参考
*/

#pragma once

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include "sha.h"

/**
* @brief FOTA差分升级信息结构体
*/
typedef struct
{
    /**
    * @brief FLASH中FOTA差分包存放区域的起始地址和大小
    */
    uint32_t flash_base ;
    uint32_t flash_maxlen;
    
    /**
    * @brief 接收到的差分包的长度
    */
    uint32_t  recv_size;
    
    /**
    * @brief 用于计算SHA校验值的结构体指针
    */
    HMACContext *hmac_context;
    
    /**
    * @brief 差分包头部信息结构体指针
    */
    void *head_info;
} ota_context_t;


/**
 * @brief 获取存放差分包的FLASH的首地址
 * @param pkt_data 差分包数据
 * @param len  差分包数据长度
 * @note  该接口用户使用，获取存放差分包数据的首地址
 */
ota_context_t *xy_get_OTA_context(char *pkt_data, int len);

/**
 * @brief OTA初始化接口
 * @warning  在开始下载之前，必须调用该接口。 \n
 * 由于内部有内存申请，若不需要升级，不能调用，以防止无效内存浪费。
 */
ota_context_t *ota_update_Init(void);

/**
 * @brief 去初始化FOTA接口，内部进行动态内存的释放
 * @warning  FOTA流程失败也需要调用该接口，否则存在内存泄漏的风险
 */
void ota_update_deInit(ota_context_t *ota_context);

/**
 * @brief  将获取到的差分包数据写入到FLASH中
 * @param data	packet data
 * @param size 	data size
 * @note  内部进行flash的操作，耗时达到几十毫秒级别
 */
int ota_downlink_packet_proc(ota_context_t *ota_context, char* data, uint32_t size);

/**
 * @brief 当差分包下载完毕后，根据ota_update_Hmac_by_packet动态计算出来的Hmac值，进行整个差分包的SHA合法性检验，若失败，则不能进行升级
 * @return BOOL,see  @ref  xy_ret_Status_t.
 * @note  必须与ota_downlink_packet_proc接口捆绑使用，以提高运行速率
 */
int ota_delta_check_sync(ota_context_t *ota_context);


/**
 * @brief 从flash中读取所有差分包，进行SHA校验
 * @param size flash中差分包总字节数
 * @return BOOL,see  @ref  xy_ret_Status_t.
 * @warning 该接口效率较低，需要先把每个差分包数据保存到flash中，最后再统一从flash中读取出来计算并SHA校验。 \n
 ***建议客户使用ota_downlink_packet_proc和ota_delta_check_sync边下载边计算，以加速差分包的下载总时长.
 */
int ota_delta_check_by_flash(uint32_t pkt_addr, size_t size);

/**
 * @brief 完成差分包的校验后，保存差分包到指定flash后，调用该接口开始差分升级 
 * @note  芯翼的差分升级是在二级boot中执行的，调用该接口后会自行进行重启升级。
 */
void ota_update_start(ota_context_t *ota_context);


/**
 * @brief  二级boot中升级完成后，执行重启，进入大版本后，调用该接口查询最终的升级结果
 * @return BOOL,see  @ref  xy_ret_Status_t.
 * @note  该接口指示升级最终结果,可供用户上报云端升级结果
 */
int ota_update_result();



