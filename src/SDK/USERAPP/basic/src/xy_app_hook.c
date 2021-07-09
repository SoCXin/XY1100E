/** 
* @file    fota_hook_func.c
* @brief   芯翼自研的FOTA功能，客户需要参与二次开发的hook函数
*/
#include "xy_memmap.h"
#include <qspi_flash.h>
#include "dma.h"
#include "sha.h"
#include "xy_app_hook.h"
#if XY_FOTA
#include "xy_fota.h"
#endif


/*云平台获取设备厂商信息函数，厂商信息由用户提供*/
void xy_get_Manufacturer(char *manufacturer, int size)
{
    if(size > (int)(strlen("Open Mobile Alliance")))
        strcat(manufacturer, "Open Mobile Alliance");
    else
        xy_printf("[xy_get_Manufacturer]error!");
    return;
}


/*云平台获取设备SN信息函数，SN信息由用户提供*/
void xy_get_SNumber(char *serial_number, int size)
{
    if(size > (int)(strlen("345000123")))
        strcat(serial_number, "345000123");
    else
        xy_printf("[xy_get_SNumber]error!");
	return;
}


/*设备FOTA升级状态的HOOK函数,用户可根据FOTA的升级状态,进行相关处理*/
void xy_fota_state_hook(XY_FOTA_STATE state)
{
    switch(state)
    {
    case OTA_FIRMWARE_DOWNLOADING:
        /*minitor user program abnormal running,propose to set user watchdog timer*/
        break;
    case OTA_FIRMWARE_DOWNLOADED:
        break;
    case OTA_FIRMWARE_UPDATING:
        break;
    case OTA_FIRMWARE_UPDATE_SUCCESS:
        break;
    case OTA_FIRMWARE_UPDATE_FAILED:
        break;
    default:
        break;
    }
    xy_printf("[xy_fota_state_hook]state:%d", state);

    return;
}


/**
 * @brief 用户处理差分包中携带的用户信息
 * @ingroup fota api
 * @param data 用户数据指针
 * @param len  用户数据长度（64）
 */
bool xy_fota_private_data_hook(uint8_t *data, int len)
{
	(void) data;
	(void) len;
    //proc user data by user
    return true;
}


