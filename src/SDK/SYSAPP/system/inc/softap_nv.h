#pragma once

/*******************************************************************************
 *							 Include header files							   *
 ******************************************************************************/
#include <stdint.h>
#include "rtc_tmr.h"

/*******************************************************************************
 *                             Type definitions                                *
 ******************************************************************************/

#define		BK_FLAG_APP    		(1<<7)
#define		BK_FLAG_ONENET    	(1<<0)
#define		BK_FLAG_CDP	    	(1<<1)

//application config,such as fota/DM/...
typedef struct
{
	uint8_t have_retry_num;
	uint8_t ue_iccid[23];
	uint32_t last_reg_time;
} dm_cfg_t;

typedef struct
{
	uint32_t in_use;
	uint32_t nextMID;
} cdp_bakmsg_t;

//flash addr 0x27005EA0,size 96 BYTES,max is RAM_NV_VOLATILE_SOFTAP_LEN (336 bytes)
typedef struct
{
	uint32_t    	next_PS_sec;//M3 write,next TAU/eDRX time,related with ps_deepsleep_state;0 represent NO PSM;0XFFFFFFFF represent infinite
	uint64_t 	    rtc_msec_reset;//M3 write,save rtc timer ms value before reset
	uint32_t        last_FOTA_time; //seconds,Time point of last FOTA
	char            ps_deepsleep_state;  //M3 write,related with next_PS_sec. 1,TAU;2,eDRX/drx
	char            padding[11];//M3 paddings

	uint64_t  cclk_local_sec; //snap  local time  of  latest update CCLK time,seconds
	uint64_t  cclk_netl_sec;  //snap  network time  of  latest update CCLK time,seconds,such as CTZEU,beijing time
	int 				cdp_nnmi;
	int 				cdp_nsmi;
	cdp_bakmsg_t		cdp_bakmsg;
	
	dm_cfg_t        	dm_cfg;
	uint8_t    	app_recovery_flag;	//deep sleep recovery flag- bit0:cdp,bit1:onenet 
	char            	g_zone;      //beijing is "+32"
	char            	padding2[2];
	

}softap_var_nv_t;

//size 236 BYTES,start from 3840 end at 4076
typedef struct 
{
	//used for DSP,must cache aligned
	struct rtc_event_info cp_rtc_list[0];// cp_rtc_list 32byte align ,24*0 = 0

	//used for M3,must cache aligned
	struct rtc_event_info ap_rtc_list[RTC_TIMER_AP_MAX];// ap_rtc_list 32byte align  ,24*9 = 216
	char            padding[20];// 
	
}rtclist_var_nv_t;

/*******************************************************************************
 *                       Global variable declarations                          *
 ******************************************************************************/
//#define RAM_SOFT_AP_NV_LEN					sizeof(softap_var_nv_t)
extern softap_var_nv_t *g_softap_var_nv;

