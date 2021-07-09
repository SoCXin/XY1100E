/** 
* @brief     FOTA相关的公共接口，如flash内存分布，FOTA状态位保存API等
* @attention   该头文件供芯翼自研FOTA和第三方FOTA方案共同使用，如艾拉比、广升、红石等。
*/
#include "xy_utils.h"
#include "xy_system.h"
#include "softap_nv.h"
#include "factory_nv.h"
#include "xy_flash.h"
#include "softap_nv.h"
#include "xy_fota_utils.h"

#define UPROUND(raw, base)    ((((raw)+(base)-1)/(base))*(base))
#define DOWNROUND(raw, base)  (((raw)/(base))*(base))


/*获取存放差分包的FLASH的信息：FLASH区域起始地址和大小*/
void xy_get_OTA_flash_info(uint32_t *addr, int *len)
{
    extern uint32_t _Flash_Used;
    extern uint32_t _Ram_Text;
    extern uint32_t _Ram_Data;
    uint32_t FLASH_LEN = (uint32_t)&_Flash_Used;
    uint32_t RAM_LEN = (uint32_t)&_Ram_Text + (uint32_t)&_Ram_Data;

    //根据ARM核的image长度计算FOTA差分包存放区域的首地址,各image长度要4K对齐
    *addr = ARM_FLASH_BASE_ADDR + UPROUND(FLASH_LEN, FLASH_SECTOR_LENGTH) + UPROUND(RAM_LEN, FLASH_SECTOR_LENGTH);
    //根据FLASH分区计算FOTA差分包存放区域的大小
    *len = FOTA_BACKUP_BASE - *addr;

    return;
}

/*获取预设的FOTA升级周期*/
int xy_get_OTA_period()
{
	return  g_softap_fac_nv->fota_period*24*60*60;
}

/*设置预设的FOTA升级周期*/
void xy_set_OTA_period(int days)
{
	g_softap_fac_nv->fota_period = days;
}

/*检查预设的FOTA升级周期是否超时*/
int xy_check_OTA_timeout()
{
    int cur_sec = 0;

    cur_sec = xy_rtc_get_sec(0);

    //从未做FOTA，需立即执行FOTA检查
    if(g_softap_var_nv->last_FOTA_time == 0)
    	return  XY_OK;

	//每次都执行FOTA升级检查
	else if(g_softap_fac_nv->fota_period == 0)
		return  XY_OK;

	//周期性检查已超时，执行FOTA检查
	else if(cur_sec > xy_get_OTA_period() + g_softap_var_nv->last_FOTA_time)
		return  XY_OK;

	else
		return  XY_ERR;
}

/*记录最新一次FOTA升级检查的时刻点*/
void  update_last_OTA_check_time()
{
	g_softap_var_nv->last_FOTA_time = xy_rtc_get_sec(0);
}


/*添加版本号等信息查询接口，包含公有云及私有云的FOTA二次开发*/


