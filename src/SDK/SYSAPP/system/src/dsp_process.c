#include "flash_vendor.h"
#include "at_global_def.h"
#include "at_context.h"
#include "lpm_adapt.h"
#include "low_power.h"
#include "xy_system.h"
#include "xy_utils.h"
#include "softap_nv.h"
#include "oss_nv.h"
#include "rtc_tmr.h"
#include "csp.h"
#include "gpio.h"
#include "sys_init.h"
#include "rtc_adapt.h"
#include "xy_flash.h"
#include "flash_adapt.h"
#include "xy_sys_hook.h"
#include "dsp_process.h"
#include "prcm.h"
#include "ipcmsg.h"

#define		PS_CAMP_ON			0xaaaa1111          //old IP addr still valid
#define		PS_DO_ATTACH		0xbbbb2222          //retrieve new IP addr
#define		PS_DO_TAU			0Xcccc3333	        //TAU wakeup,same for PS_CAMP_ON

typedef struct {
	unsigned int image_id[3];
	unsigned int load_addr;
	unsigned int exec_addr;
	unsigned int len_byte;
	unsigned int image_content_crc;
	unsigned int image_info_crc;
} Flash_Image_Def;


// External Flash First 256 bytes
typedef struct {
	// Header Info
	unsigned int image_num;
	unsigned int image_info_addr_base;
	unsigned int user_code;
	unsigned int nv_addr_base;
	unsigned int nv_len_byte;
	unsigned int exchange_addr_base;
	unsigned int exchange_len_byte;
	unsigned int header_info_crc;

	// Image Info
    Flash_Image_Def image_info[7];
} Flash_Header_Def;

osMutexId_t g_dynamic_dsp_load_m = NULL;


extern unsigned char hclk_div_reg[16];
extern volatile unsigned int g_current_baudrate;

void PRCM_Clock_Reset(unsigned long ulBase,unsigned char pclk_div, unsigned char hclk_div)
{
	volatile unsigned char ulReg = HWREGB(ulBase+PRCM_CLKRST_CTL);
	if(pclk_div)
		ulReg = ((ulReg&0x3F)|((pclk_div-1)<<6));
	if(hclk_div)
		ulReg = ((ulReg&0xE1)|(hclk_div_reg[hclk_div-1]<<1));
	HWREGB(ulBase+PRCM_CLKRST_CTL) = ulReg;

}

//wakeup from normal deepsleep,and IP addr still valid
int  g_normal_wakeup = 0;

//power ON or POWER OFF time have already compensated,set DSP work mode
void set_DSP_work_mode()
{
	struct rtc_time cur_rtctime;
	int cur_rtc_sec = 0;
	//volatile unsigned long intsave;

	rtc_timer_read(&cur_rtctime);
	cur_rtc_sec = xy_mktime(&cur_rtctime)/1000;
	
	g_standby_dsp_ready = 0;
	g_deepsleep_dsp_ready = 0;

	rtc_mutex_acquire();
	osCoreEnterCritical();
	
	if(/*g_softap_fac_nv->xtal_32k==1 && */g_softap_fac_nv->ps_utcwkup_dis==1)
	{
		g_dsp_mode_dbg |= 1 << 3;
		HWREG(PS_START_SYNC) = PS_DO_ATTACH;
	}
	else if ((get_sys_up_stat() == UTC_WAKEUP || get_sys_up_stat() == EXTPIN_WAKEUP) && 
	g_softap_var_nv->next_PS_sec != 0 && g_softap_var_nv->next_PS_sec >= (unsigned int)(cur_rtc_sec - 1))
	{
		if (g_RTC_wakeup_type == 1) //tau/edrx/drx rtc timeout
			g_dsp_mode_dbg |= 1 << 2;
		else
			g_dsp_mode_dbg |= 1 << 1;
		
		HWREG(PS_START_SYNC) = PS_CAMP_ON;
		g_normal_wakeup = 1;
	}
	else
	{
		g_dsp_mode_dbg |= 1 << 3;
		//set sharing memory to default,trigger PS to power on
		HWREG(PS_START_SYNC) = PS_DO_ATTACH;
	}

	g_softap_var_nv->next_PS_sec = 0;
	rtc_event_delete_nomutex(RTC_TIMER_LPM);

	//wait DSP have finish read NV and start working
	//wait_dsp_init_ok();
	osCoreExitCritical();
	rtc_mutex_release();

}

void set_dsp_no_work()  
{ 
	HWREG(PS_START_SYNC) = 0;
	//change log uart to ARM from DSP
	HWREGB(0xA0058000) = 0x00; // anoprcm->slpwkp_ctl, disable extpin and utc wakeup,in case of waking up dsp core by mistake
	//HWREGB(0xA0110001) = 0x4; // coreprcm->pwrctlgate1 enable dsp core to be off when dsp sleep
	//HWREGB(0xA0058003) = 0x2;//dsp mem off
}

void dynamic_wait_dsp_init_ok()
{
	volatile unsigned long i;
	// Enable DSP,XY_DSPCLK_DIV is 4,yus
	HWD_REG_WRITE32(0xA0110018, (HWD_REG_READ32(0xA0110018)&0x1E)|0x00);

    for(i = 0; i < 100; i++)
    {
    }
	
	HWD_REG_WRITE32(0xA0110018, (HWD_REG_READ32(0xA0110018)&0x1E)|0x21);
	//wait for a while so that dsp have enough time to change DSP_LPM_STATE to alive
	for(i = 0; i < 1000; i++)
    {
    }
//	while(HWREG(PS_START_SYNC)!=SYNC_END_FLAG);
}


void dynamic_set_DSP_work_mode()
{
	struct rtc_time cur_rtctime;
	int cur_rtc_sec = 0;


	rtc_timer_read(&cur_rtctime);
	cur_rtc_sec = xy_mktime(&cur_rtctime)/1000;
	rtc_mutex_acquire();
	osCoreEnterCritical();

	if ((get_sys_up_stat() == UTC_WAKEUP || get_sys_up_stat() == EXTPIN_WAKEUP) && 
	g_softap_var_nv->next_PS_sec != 0 && g_softap_var_nv->next_PS_sec >= (unsigned int)(cur_rtc_sec - 1))
	{
		if (g_RTC_wakeup_type == 1) //tau/edrx/drx rtc timeout
			g_dsp_mode_dbg |= 1 << 2;
		else
			g_dsp_mode_dbg |= 1 << 1;
		
		HWREG(PS_START_SYNC) = PS_CAMP_ON;
	}
	else
	{
		g_dsp_mode_dbg |= 1 << 3;
		HWREG(PS_START_SYNC) = PS_DO_ATTACH;
	}
	
	g_softap_var_nv->next_PS_sec = 0;
	rtc_event_delete_nomutex(RTC_TIMER_LPM);
	dynamic_wait_dsp_init_ok();
	
	osCoreExitCritical();
	rtc_mutex_release();

	g_standby_dsp_ready = 0;
	g_deepsleep_dsp_ready = 0;
	if((g_softap_fac_nv->off_debug == 0) && (osKernelGetState() != osKernelInactive))
		print_dsp_dbg();
}

//Need real-time load DSP,and start use  NB stack to communication,after  load complete,need to check whether to attempt to attach
void dynamic_load_up_dsp(int isDump)
{
	#define USER_CODE_NOT_LOAD_DSP      0x00000010

	/*must == bootrom_v_2_1_secondary_boot_gcc--boot_loader.h*/
	Flash_Header_Def flash_header;
	//unsigned char flash_header_valid = 0;
	int i;
	volatile unsigned char mem_status = 0;
	uint8_t * temp = NULL;
	//unsigned int intsave=0;
	//static unsigned char s_loadup_dsp_flag =0;
	

	if(isDump == 0)
	{
	    network_task_init();
	}

	if( (HWREGB(0xA011000D) & 0x01) && g_softap_fac_nv->work_mode == 2)
	{
		HWREGB(PRCM_BASE + 0x60) |= 0x06;
		HWREGB(PRCM_BASE + 0x5B) |= 0x04;
		HWREGB(PRCM_BASE + 0x0C) &= 0xFE;

		while((!(HWREGB(0xA011000D)&0x04)) || (!(HWREGB(0xA011000D)&0x02)));

		uint8_t hclk_valid_value[16] = {0, 0, 1, 1, 1, 1, 1, 1, 0, 1, 0, 1, 0, 0, 0, 1};
		unsigned char hclk_div = 10;
	    unsigned char pclk_div = 1;


		hclk_div = g_softap_fac_nv->hclk_div;
		pclk_div = g_softap_fac_nv->pclk_div;

    	if(hclk_div <= 16 && hclk_div >= 3)
    	{
        	if(hclk_valid_value[hclk_div - 1] != 1)
        	{
            	hclk_div = 10;
        	}
    	}
    	else
    	{
        	hclk_div = 10;
    	}

    	if(pclk_div > 4 || pclk_div == 0)
    	{
        	pclk_div = 1;
    	}

        g_XY_HCLK = XY_BBPLL_CLK / hclk_div;
		g_XY_PCLK = g_XY_HCLK / pclk_div;
		g_XY_PCLK_DIV = pclk_div;
		g_XY_HCLK_DIV = hclk_div;
		g_OS_SYS_CLOCK = g_XY_HCLK;

		//M3核时钟重配
		PRCM_Clock_Reset(PRCM_BASE, XY_PCLK_DIV ,XY_HCLK_DIV);

		//向DSP发送全局变量
		HWREG(CLOCK_DIV_VALUE) = (g_XY_PCLK_DIV << 8) | g_XY_HCLK_DIV; 

		//Reset Log COM
		if(g_softap_fac_nv->open_log != 0)
		{
			CSPUARTModeSet(CSP3_BASE, XY_PCLK, 921600, 8, 1);
		}
		

		if(g_softap_fac_nv->close_at_uart == 0)
		{
			//Reset AT COM
			CSPUARTModeSet(AT_UART_CSP, XY_PCLK, g_current_baudrate, 8, 1);
		}
		
	}
	
	//if not dump, use mutex lock; or no use mutex lock to prevent deadlock
	if(isDump == 0)
	{	
		if(osCoreGetState() != osCoreInCritical && g_dynamic_dsp_load_m != NULL && osKernelGetState() != osKernelInactive)
			osMutexAcquire(g_dynamic_dsp_load_m, osWaitForever);
		if(!DSP_IS_NOT_LOADED())
		{
			if(osCoreGetState() != osCoreInCritical && g_dynamic_dsp_load_m != NULL && osKernelGetState() != osKernelInactive)
				osMutexRelease(g_dynamic_dsp_load_m);
			return;
		}
	}
	//maybe dead while interrupt process,and can not acquire mux
	else
	{
		if(!DSP_IS_NOT_LOADED())
			return;
	}
	
	//if(s_loadup_dsp_flag == 0)
	{
		//s_loadup_dsp_flag++;
		osCoreEnterCritical();
		//HWREGB(0xA0110001) = 0; // dsp core 
		//HWREGB(0xA0058003) = 0;//dsp mem
		//read PS/PHY/LPM var nv,if only M3 core work,do restore in dynamic_load_up_dsp
		if(g_softap_fac_nv->high_freq_data==1)
		{
			/* BACKUP_MEM_BASE存的数据包含头部魔数，xy_ftl_read读出的数据不包含头部魔数 */
			temp = xy_malloc(DSP_VOLATILE_ALL_LEN);
			if(xy_ftl_read(NV_FLASH_DSP_VOLATILE_BASE,(unsigned char *)temp,DSP_VOLATILE_ALL_LEN)!= XY_OK)
			{		 
				memset((void *)BACKUP_MEM_BASE,0,DSP_VOLATILE_ALL_LEN);
			}
			else
			{
				memcpy((uint8_t *)BACKUP_MEM_BASE+4, temp, DSP_VOLATILE_ALL_LEN-4);
			}
			xy_free(temp);
		}
		

		mem_status = ((HWREGB(AON_8_SYSMEM_CTL) >> 4) & 0x0F);

		//XY_DSPCLK_DIV is 4,yus
		HWD_REG_WRITE32(0xA0110018, (HWD_REG_READ32(0xA0110018)&0x1E)|0x60);

		 // Read External Flash Header Information
		DMAChannelMemcpy((unsigned long)&flash_header, PARTITION_INFO_FLASH_BASE, 0x100, FLASH_DMA_CHANNEL);

		/*must == bootrom_v_2_1_secondary_boot_gcc--boot_loader.h*/
		//if(flash_header_valid == 1)
		{
			for(i = 0; i < (int)(flash_header.image_num); i++)
			{
				if((flash_header.image_info[i].image_id[0] & 0xFFFFFF00) == ((((unsigned char)'D') << 24) | (((unsigned char)'S') << 16) | (((unsigned char)'P') << 8) | 0x00))
				{
					if((flash_header.image_info[i].load_addr != flash_header.image_info[i].exec_addr) && ((flash_header.user_code & USER_CODE_NOT_LOAD_DSP) == 0))
					{
						if(0 == AON_Addr_Overlap_Unused_Mem(mem_status, flash_header.image_info[i].exec_addr, flash_header.image_info[i].len_byte))
						{
							// Need to Copy Data from External Flash to Internal SoC
							DMAChannelMemcpy((unsigned long)flash_header.image_info[i].exec_addr, (unsigned long)flash_header.image_info[i].load_addr, flash_header.image_info[i].len_byte, FLASH_DMA_CHANNEL);
						}
					}
				}
			}
		}
		HWREGB(0xA0110001) = 0x0; // coreprcm->pwrctlgate1
		osCoreExitCritical();
	}
	
	
	dynamic_set_DSP_work_mode();
	
	if(isDump == 0)
	{
		if(osCoreGetState() != osCoreInCritical && g_dynamic_dsp_load_m != NULL && osKernelGetState() != osKernelInactive)
			osMutexRelease(g_dynamic_dsp_load_m);
	}

	softap_printf(USER_LOG, WARN_LOG, "DSP loaded");
}

volatile uint32_t g_dsp_off_request = 0;
int shut_down_dsp_core(void)
{
	//mutex
	if(g_softap_fac_nv->work_mode == 0)
	{
		return 0;
	}
	
	osMutexAcquire(g_dynamic_dsp_load_m, osWaitForever);
	if(DSP_IS_NOT_LOADED())
	{
		osMutexRelease(g_dynamic_dsp_load_m);
		return 1;
	}
	else if(g_dsp_off_request == 0)
	{
		
		//if DSP have sleep,or need AT+OFFTIME,or BUSY,not shark with DSP
		if (g_need_offtime == 0 && !(nearps_ctx.state & SEND_REQ_NOW))
		{
			at_ReqAndRsp_to_ps("AT+CFUN=5\r\n", NULL, 60);
			g_dsp_off_request++;
			osMutexRelease(g_dynamic_dsp_load_m);
			return 1;
		}
		else
		{
			xy_assert(0);
			return 0;
		}

		
		
	}
	else
	{
		osMutexRelease(g_dynamic_dsp_load_m);
		return 1;
	}

	//mutex
	
}

extern unsigned int icm_buf_check();
extern T_IpcMsg_ChInfo g_IpcMsg_ChInfo[IpcMsg_Channel_MAX];
int shut_down_dspcore_process(void)
{
	
	if(g_dsp_off_request == 0)
		return SHUTDOWN_DSP_NONE;

	lpm_EnterCritical();

	if(icm_buf_check())
	{
		lpm_ExitCritical();
		return SHUTDOWN_DSP_REQ;
	}	

	if(Get_CP_ALARM_RAM() != RTC_ALARM_MAX)
	{
		/*
		xy_assert(0);
		lpm_ExitCritical();
		return SHUTDOWN_DSP_REQ;
		*/
		Set_CP_ALARM_RAM(RTC_ALARM_MAX);
		if(RTC_TIMER_CP_MAX != 0)
		{
			memset(((rtclist_var_nv_t *)RAM_NV_VOLATILE_RTCLIST_START)->cp_rtc_list,0,sizeof( ((rtclist_var_nv_t *)RAM_NV_VOLATILE_RTCLIST_START)->cp_rtc_list ));
			rtc_event_refresh();
		}
		
		
	}

	//dsp off
	HWD_REG_WRITE32(0xA0110018, (HWD_REG_READ32(0xA0110018)&0x1E)|0x00);
	lpm_string_output("+DSP POWER OFF", strlen("+DSP POWER OFF"));
	
	//dsp ram free
	
	//
	//free_all_addr_malloc_by_dsp();

	g_IpcMsg_ChInfo[IpcMsg_LOG].RingBuf_send->Read_Pos = 0;
	g_IpcMsg_ChInfo[IpcMsg_LOG].RingBuf_send->Write_Pos = 0;

	HWREG(PS_START_SYNC) = 0;

	HWREG(DSP_LPM_STATE) = DSP_NOT_LOADED;
	g_standby_dsp_ready = 0;
	g_deepsleep_dsp_ready = 0;
	//Set_AP_ALARM_RAM(RTC_ALARM_MAX);
	Set_CP_ALARM_RAM(RTC_ALARM_MAX);


	g_dsp_off_request = 0;
	
	lpm_ExitCritical();

	//softap_printf(USER_LOG, WARN_LOG, "DSP POWER OFF");
	
	return SHUTDOWN_DSP_DONE;
	
}
