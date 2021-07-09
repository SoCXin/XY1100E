#ifndef _OSS_NV_H_
#define _OSS_NV_H_

#include "xy_flash.h"
#include "xy_utils.h"

#define  HAVE_FREE_LOCK     (*((volatile int *)FREELOCK_FLAG))

#define     BK_FLAG_INVALID       0  //volatile nv invalid
#define     BK_FLAG_INUSE       0xaa55a5a5  //volatile nv,not include user data
#define     BK_FLAG_INUSE_2     0x55AA5A5A  //softap volatile nv + user data,g_softap_fac_nv->keep_retension_mem must 1


#define OFFSET_FAC_PARAM(param) ((unsigned int)&(((softap_fac_nv_t *)0)->param))

#define FAC_PARAM_LEN(param) (sizeof(((softap_fac_nv_t *)0)->param))

/* 实时写factory nv传入的offset不包括头部魔数 */
#define SAVE_FAC_PARAM(param)	\
		{  \
			if(xy_ftl_write(NV_FLASH_FACTORY_BASE,DSP_FAC_LEN-4+OFFSET_FAC_PARAM(param),(unsigned char *)&g_softap_fac_nv->param,FAC_PARAM_LEN(param)) != XY_OK) \
			{  \
				xy_flash_erase(NV_FLASH_FACTORY_BASE,NV_FLASH_FACTORY_LEN);  \
				xy_assert(0);  \
			} \
		}
//if DSP can vary softap fac param,this API will cover old val and drop some vary err!
#define SAVE_SOFTAP_FAC()	\
		{  \
			if(xy_ftl_write(NV_FLASH_FACTORY_BASE,DSP_FAC_LEN-4,(unsigned char *)g_softap_fac_nv,sizeof(softap_fac_nv_t)) != XY_OK) \
			{  \
				xy_flash_erase(NV_FLASH_FACTORY_BASE,NV_FLASH_FACTORY_LEN);  \
				xy_assert(0);  \
			} \
		}


enum {
	OPERATION_VER = 0,  //ru ku ru ku version
	SDK_VER,            //normal XY SDK,AT REQ is synchronous
	JINKA_VER,          //jinka special request,e.g "OK"
	CMIOT_VER,          //zhong yi wu lian
	QUECTEL_VER,        //yi yuan
	ZeusOS_VER,        //河北骑士
	CUSTOMER_MAX = 8,
};
	
#define  CHECK_SDK_TYPE(cust_type)    (g_softap_fac_nv->ver_type&(1<<cust_type))
#define  DSP_FAC_LEN            (sizeof(T_PsNvInfo)+sizeof(rf_dciq_t))
#define  SOFTAP_FAC_FLASH_BASE  (NV_FLASH_FACTORY_BASE+DSP_FAC_LEN)
#define  SOFTAP_FAC_RAM_BASE  (RAM_FACTORY_NV_BASE+DSP_FAC_LEN)

extern void *g_invar_nv;
extern int   g_invar_len;
extern int g_dsp_mode_dbg;//0,INIT;1,CAMPON;2;TAU;3,ATTACH;4,NULL
extern unsigned int cdp_fota_flag;
extern unsigned int cis_fota_flag;
extern unsigned int g_fota_info_flag;
extern int cis_last_fota_pkt;
extern int cdp_last_fota_pkt;
extern volatile unsigned int g_current_baudrate;

extern char g_need_offtime;

extern void restore_RTC_list();
void  erase_flash_by_abnormal(int state);
void nv_save_by_user(int dev_id);
void nv_save_special(unsigned int addr,  unsigned char *data, unsigned int size);
void save_softap_var_nv();
void nv_restore();
void nv_write_to_flash();
void print_dsp_dbg();
void reboot_nv();
void save_factory_nv();
void save_volatile_nv(int reBoot);
void xy_nv_ftl_init();

#endif
