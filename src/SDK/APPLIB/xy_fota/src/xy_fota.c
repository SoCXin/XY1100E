/***************************************************************************************************************************
 * 芯翼自研的FOTA方案相关API接口，该源文件客户无权修改，只能调用相应的API接口。如果有额外需求，可与芯翼FAE联系！ *
 **************************************************************************************************************************/
#include "xy_utils.h"
#include <qspi_flash.h>
#include "xy_system.h"
#include "xy_fota.h"
#include "dma.h"
#include "softap_nv.h"
#include "oss_nv.h"
#include "xy_flash.h"
#include "xy_watchdog.h"
#if TELECOM_VER
#include "ota_crc.h"
#endif
#include "rsa.h"
#include "xy_at_api.h"
#include "sys_init.h"
#include "xy_app_hook.h"

#if XY_FOTA

/**
* @brief macro for fota flag to star
*/
#define FOTA_UPGRADE_NEEDED 0x1
#define FOTA_UPGRADE_BYPASS 0x0

/**
* @brief fota package magic num
*/
#define MAGIC_NUM 3513

/**
* @brief fota package hmac key
*/
#define HMAC_KEY "XY1100"

/**
* @brief fota package data buffer length
*/
#define BUF_LEN 1024

/**
* @brief 差分包中用户可携带数据的长度
*/ 
#define USER_FOTA_DATA_LEN 64

/**
* @brief 差分包中RSA加密SHA校验值后的长度
*/ 
#define DELTA_RSA_SHA_LEN 80

/**
* @brief FOTA升级结果标志位
*/
extern int ota_status_flag;

/**
* @brief head struct for fota detla file
*/
typedef struct {
    uint32_t fota_flag;
    uint32_t fota_base_addr;
    uint8_t ming_sha[20];
    uint8_t reserve[20];
    uint32_t fota_info_crc;
} Flash_Secondary_Boot_Fota_Def;

/**
* @brief 差分包数据头部信息结构体,用户不需要关注
*/
typedef struct
{
    uint32_t magic_num;
    uint8_t total_sha[80];
    uint32_t version;
    uint8_t old_image_version_sha[20];	///< old_image_version_sha
    uint8_t new_image_version_sha[20];	///< new_image_version_sha
    uint32_t image_num;
    uint32_t extra_num;
    uint32_t user_delta_info_num;
    uint32_t backup_start_addr;
    uint32_t backup_len;
    uint32_t fota_packet_base;              ///< 差分包存放地址
    uint32_t lzma_model_properties;
    uint32_t lzma_dict_size;
    uint32_t user_data_len;
} delta_head_info_t;

/*******************************************************************************
 *                Fota updates related functions                               *
 ******************************************************************************/


/* 检验差分包是否适用于该版本，根据差分包中携带的SHA校验值判断 */
static int ota_version_check(delta_head_info_t *head)
{
    uint8_t temp_sha[SHA1HashSize] = {0};

    memcpy(temp_sha, (const void *)(VERSION_MIMG_SHA), SHA1HashSize);

    //源版本SHA1校验值比较
    if(memcmp(head->old_image_version_sha, temp_sha, SHA1HashSize) != 0)
    {
        xy_rearrange_DWORD(temp_sha, SHA1HashSize);

        if(memcmp(head->old_image_version_sha, temp_sha, SHA1HashSize) != 0)
        {
            softap_printf(USER_LOG, WARN_LOG, "error: image version error");
            return XY_ERR;
        }
    }
       
    return XY_OK;
}


/*获取差分包的头部信息结构体*/
ota_context_t *xy_get_OTA_context(char *pkt_data, int len)
{
    if(len < sizeof(delta_head_info_t))
        return XY_ERR;

    ota_context_t *ota_context = (ota_context_t *)xy_zalloc(sizeof(ota_context_t));

    ota_context->flash_base = ((delta_head_info_t *)pkt_data)->fota_packet_base;
	
    return ota_context;
}

/* FOTA初始化函数，初始化相关全局变量 */
ota_context_t *ota_update_Init(void)
{
    ota_context_t *ota_context;
    ota_context = xy_zalloc(sizeof(ota_context_t));
    
    //HMAC计算算法全局变量初始化
    if(ota_context == NULL)
    {
        softap_printf(USER_LOG, WARN_LOG, "error: fota_context malloc error");
        return NULL;
    }
    
    ota_context->hmac_context = (HMACContext *)xy_zalloc(sizeof(HMACContext));
    if(ota_context->hmac_context == NULL)
        goto error;

    hmacReset(ota_context->hmac_context, SHA1, (const uint8_t*)HMAC_KEY, strlen(HMAC_KEY));

    ota_context->head_info = (delta_head_info_t *)xy_zalloc(sizeof(delta_head_info_t));
    if(ota_context->head_info == NULL)
        goto error;

    //设置NV标志位
    g_softap_fac_nv->fota_res = 0;
    SAVE_FAC_PARAM(fota_res);

    //获取差分区域的起始地址和长度
    xy_get_OTA_flash_info(&(ota_context->flash_base), &(ota_context->flash_maxlen));

    //初始化已接收数据长度
    ota_context->recv_size = 0;
    
    softap_printf(USER_LOG, WARN_LOG, "[ota_ram_init]xy_fota_init");

    return ota_context;
error:
    if(ota_context->head_info != NULL)
        xy_free(ota_context->head_info);

    if(ota_context->hmac_context != NULL)
        xy_free(ota_context->hmac_context);

    if(ota_context != NULL)
        xy_free(ota_context);

    softap_printf(USER_LOG, WARN_LOG, "[ota_ram_init]xy_fota_init error");
    return NULL;
}


/*FOTA去初始化函数，内部进行动态内存的释放 */
void ota_update_deInit(ota_context_t *ota_context)
{
    if(ota_context == NULL)
        return;

    if(ota_context->head_info != NULL)
        xy_free(ota_context->head_info);

    if(ota_context->hmac_context != NULL)
        xy_free(ota_context->hmac_context);

    if(ota_context != NULL)
        xy_free(ota_context);


    g_softap_fac_nv->fota_res = 2;
    SAVE_FAC_PARAM(fota_res);
    
    softap_printf(USER_LOG, WARN_LOG, "[ota_ram_deinit]xy_fota_deinit");
}

/*对差分包数据进行处理，检查差分包头的完整性，版本检测，进行HMAC计算，最后将差分包数据写入flash*/
int ota_downlink_packet_proc(ota_context_t *ota_context, char* data, uint32_t size)
{
    uint32_t offset = 0;
	char * input_data =data;
    uint32_t input_len = size;
    delta_head_info_t *head_info;
    head_info = (delta_head_info_t *)ota_context->head_info;

    if(ota_context == NULL || ota_context->head_info == NULL
         || ota_context->hmac_context == NULL)
    {
        softap_printf(USER_LOG, WARN_LOG, "error:(Input)head_info/hmac_context is NULL");
        return XY_ERR;
    }

    if(ota_context->recv_size + input_len > ota_context->flash_maxlen)
    {
        softap_printf(USER_LOG, WARN_LOG, "error:Dtle file size too large!");
        return XY_ERR;
    }

    if(ota_context->recv_size + input_len < (sizeof(delta_head_info_t)))
    {
        memcpy((char *)head_info + ota_context->recv_size, input_data, input_len);
        ota_context->recv_size += input_len;
        return XY_OK;
    }

    if(ota_context->recv_size < (sizeof(delta_head_info_t)))
    {
        memcpy((char *)head_info + ota_context->recv_size, input_data, (sizeof(delta_head_info_t)) - ota_context->recv_size);
        input_data += (sizeof(delta_head_info_t)) - ota_context->recv_size;
        input_len -= (sizeof(delta_head_info_t)) - ota_context->recv_size;
        ota_context->recv_size = (sizeof(delta_head_info_t));

        if(head_info->magic_num != MAGIC_NUM)
        {
            softap_printf(USER_LOG, WARN_LOG, "error: magic_num(%d) != %d", head_info->magic_num, MAGIC_NUM);
            return XY_ERR;
        }

        if(ota_version_check(head_info))
            return XY_ERR;
		
		/* 差分包过大flash不足 */
        if(head_info->fota_packet_base < ota_context->flash_base)
        {
            softap_printf(USER_LOG, WARN_LOG, "error: delta is too big");
            return XY_ERR;
        }

        ota_context->flash_base = head_info->fota_packet_base;
        
        //差分包前84个字节不作为校验内容
		hmacInput(ota_context->hmac_context, (const uint8_t *)&(head_info->version), (sizeof(delta_head_info_t)) - 84);
		softap_printf(USER_LOG, WARN_LOG, "delta_save_addr: %x", head_info->fota_packet_base);
    }

    if(ota_context->recv_size == (sizeof(delta_head_info_t)))
    {
        xy_flash_erase(ota_context->flash_base, ota_context->flash_maxlen);
        xy_flash_write_no_erase(ota_context->flash_base, head_info, (sizeof(delta_head_info_t)));
		if(input_len > 0)
		{
        	xy_flash_write_no_erase(ota_context->flash_base + (sizeof(delta_head_info_t)), input_data, input_len);
		}
        ota_context->recv_size += input_len;
    }
	else
	{
		xy_flash_write_no_erase(ota_context->flash_base + ota_context->recv_size, input_data, input_len);
		ota_context->recv_size += input_len;
	}

    while (input_len >= BUF_LEN)
    {
        hmacInput(ota_context->hmac_context, (const uint8_t *)(input_data+offset), BUF_LEN);
        offset += BUF_LEN;
        input_len -= BUF_LEN;
    }

    if (input_len > 0)
    {
        hmacInput(ota_context->hmac_context, (const uint8_t *)(input_data+offset), input_len);
        offset += input_len;
        input_len = 0;
    }
    
    return XY_OK;
}


/*当差分包下载完毕后，根据ota_update_Hmac_by_packet动态计算出来的Hmac值，进行整个差分包的SHA合法性检验，若失败，则不能进行升级      */
int ota_delta_check_sync(ota_context_t *ota_context)
{
    bool ret = XY_ERR;
	PUBLIC_KEY_Def * pub_key = NULL;
    uint8_t digest[SHA1HashSize];
	uint8_t digest_decrypt[SHA1HashSize];
	delta_head_info_t *head_info;
    head_info = (delta_head_info_t *)ota_context->head_info;
    
    if(ota_context == NULL || ota_context->head_info == NULL
         || ota_context->hmac_context == NULL)
    {

        softap_printf(USER_LOG, WARN_LOG, "error: (FwValidate)head_info/hmac_context is NULL");
        return XY_ERR;
    }

    hmacResult(ota_context->hmac_context, digest);

	pub_key = (PUBLIC_KEY_Def *)(PUBLIC_KEY);
	rsa_public_decrypt(head_info->total_sha, DELTA_RSA_SHA_LEN, pub_key, (char *)digest_decrypt);

    //validate fota data failed
    if(memcmp(digest, digest_decrypt, SHA1HashSize) == 0)
    {
        softap_printf(USER_LOG, WARN_LOG, "[OTA]: FW validate success");
		//handle user_date
        xy_fota_private_data_hook(head_info->fota_packet_base + (sizeof(delta_head_info_t)), head_info->user_data_len);
        ret = XY_OK;
    }
	else
	{
        softap_printf(USER_LOG, WARN_LOG, "[OTA]: FW validate failed");
        ret = XY_ERR;
	}
    
    return ret;
}


/* 设置二级BOOT的FOTA标志位和相关信息，并重启后开始由二级boot升级 */
void ota_update_start(ota_context_t *ota_context)
{
    uint32_t header_crc = 0;
    Flash_Secondary_Boot_Fota_Def fota_def;
    memset(&fota_def, 0x00, sizeof(Flash_Secondary_Boot_Fota_Def));
    memcpy(&fota_def, (const void *)(FOTA_UPDATE_INFO_ADDR), sizeof(Flash_Secondary_Boot_Fota_Def));
    
    fota_def.fota_flag = FOTA_UPGRADE_NEEDED;
    fota_def.fota_base_addr = ota_context->flash_base;
    
    header_crc = xy_chksum((void *)&fota_def, sizeof(Flash_Secondary_Boot_Fota_Def) - 4);
    fota_def.fota_info_crc = (unsigned short)header_crc;

    xy_flash_write(FOTA_UPDATE_INFO_ADDR, &fota_def, sizeof(Flash_Secondary_Boot_Fota_Def));

    osDelay(200);
    
	//reboot
	soft_reset_by_wdt(RB_BY_FOTA);

}


/* 从FLASH中读取差分包所有数据进行SHA校验 */
int ota_delta_check_by_flash(uint32_t pkt_addr, size_t size)
{
    bool ret = XY_ERR;
	PUBLIC_KEY_Def * pub_key = NULL;
    uint8_t digest[SHA1HashSize];
	uint8_t digest_decrypt[SHA1HashSize];
    uint8_t buf[512] = {0};
    int  read_size = 0;
    delta_head_info_t head_info;

    HMACContext *hmac_context = (HMACContext *)xy_zalloc(sizeof(HMACContext));
    xy_flash_read(pkt_addr + read_size, &head_info, sizeof(delta_head_info_t));
    hmacInput(hmac_context, (const uint8_t *)(&head_info), sizeof(delta_head_info_t) - DELTA_RSA_SHA_LEN - 4);
    read_size += sizeof(delta_head_info_t);
    
    while((read_size + 512) < size)
    {
        memset(buf, 0x00, 512);
        xy_flash_read(pkt_addr + read_size, buf, 512);
        read_size += 512;
        if(hmacInput(hmac_context, (const uint8_t *)buf, 512))
            return XY_ERR;
    }

    memset(buf, 0x00, 512);
    xy_flash_read(pkt_addr + read_size, buf, size - read_size);
    if(hmacInput(hmac_context, (const uint8_t *)(buf), (uint32_t)(size - read_size)))
        return XY_ERR;

    hmacResult(hmac_context, digest);
    
    pub_key = (PUBLIC_KEY_Def *)(PUBLIC_KEY);
    rsa_public_decrypt(head_info.total_sha, DELTA_RSA_SHA_LEN, pub_key, (char *)digest_decrypt);

    //validate fota data failed
    if(memcmp(digest, digest_decrypt, SHA1HashSize) == 0)
    {
        softap_printf(USER_LOG, WARN_LOG, "[OTA]: FW validate success");
        //handle user_date
        xy_fota_private_data_hook(head_info.fota_packet_base + sizeof(delta_head_info_t), head_info.user_data_len);
        ret = XY_OK;
    }
    else
    {
        softap_printf(USER_LOG, WARN_LOG, "[OTA]: FW validate failed");
        ret = XY_ERR;
    }

    return ret;
}

/* 二级boot中升级完成后，执行重启，进入大版本后，调用该接口查询最终的升级结果*/
int ota_update_result()
{
    if(*(uint32_t*)FOTA_RESULT_FLASH_BASE == 1)
    {
        //Take update result, success: 0000 0110
        g_softap_fac_nv->fota_res = g_softap_fac_nv->fota_res|0X02|0X04;
        SAVE_FAC_PARAM(fota_res);     
        ota_status_flag = XY_OK;
        xy_flash_erase(FOTA_RESULT_FLASH_BASE, FOTA_RESULT_FLASH_MAXLEN);
    }
    else if(*(uint32_t*)FOTA_RESULT_FLASH_BASE == 2)
    {
        //Take update result, failed: 0000 1010
        g_softap_fac_nv->fota_res = g_softap_fac_nv->fota_res|0X02|0X08;
        SAVE_FAC_PARAM(fota_res);
        ota_status_flag = XY_ERR;
        xy_flash_erase(FOTA_RESULT_FLASH_BASE, FOTA_RESULT_FLASH_MAXLEN);
    }
    
    return ota_status_flag;
}
#else
int ota_downlink_packet_proc(ota_context_t *ota_context, char* data, uint32_t size){return 0;}
ota_context_t *ota_update_Init(void){return NULL;}
void ota_update_deInit(ota_context_t *ota_context){return;}
int ota_delta_check_sync(ota_context_t *ota_context){return 0;}    
void ota_update_start(ota_context_t *ota_context){return;}    
int ota_delta_check_by_flash(uint32_t pkt_addr, size_t size){return 1;}
int ota_update_result(){return 1;}
#endif


