/*----------------------------------------------------------------------------
 * Copyright (c) <2016-2018>, <Huawei Technologies Co., Ltd>
 * All rights reserved.
 * Redistribution and use in source and binary forms, with or without modification,
 * are permitted provided that the following conditions are met:
 * 1. Redistributions of source code must retain the above copyright notice, this list of
 * conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright notice, this list
 * of conditions and the following disclaimer in the documentation and/or other materials
 * provided with the distribution.
 * 3. Neither the name of the copyright holder nor the names of its contributors may be used
 * to endorse or promote products derived from this software without specific prior written
 * permission.
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO,
 * THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
 * PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR
 * CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
 * EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 * PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS;
 * OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
 * WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR
 * OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF
 * ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *---------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------
 * Notice of Export Control Law
 * ===============================================
 * Huawei LiteOS may be subject to applicable export control laws and regulations, which might
 * include those applicable to Huawei LiteOS of U.S. and the country in which you are located.
 * Import, export and usage of Huawei LiteOS in any manner by you shall be in compliance with such
 * applicable export control laws and regulations.
 *---------------------------------------------------------------------------*/

#include "sys_init.h"
#include "at_global_def.h"
#include "dma.h"
#include "flash_vendor.h"
#include "gpio.h"
#include "hw_csp.h"
#include "hw_timer.h"
#include "hw_utc.h"
#include "low_power.h"
#include "prcm.h"
#include "rf_drv.h"
#include "rtc_tmr.h"
#include "sema.h"
#include "timer.h"
#include "uart.h"
#include "xinyi_hardware.h"
#include "xy_hwi.h"
#include "xy_system.h"
#include "rtc_tmr.h"
#include "xy_flash.h"
#include "interrupt.h"
#include "csp.h"
#include "oss_nv.h"
#include "utc.h"
#include "at_uart.h"
#if BLE_ENABLE
#include "ble_config.h"
#endif

volatile unsigned long g_plllock_forcextal_num = 0;


volatile unsigned long g_XY_HCLK;
volatile unsigned long g_XY_PCLK;
volatile unsigned char g_XY_PCLK_DIV;
volatile unsigned char g_XY_HCLK_DIV;
volatile unsigned long g_OS_SYS_CLOCK;
volatile unsigned int g_XY_RC32K_CLK = 32768;

//这个函数在.s文件中直接调用，在resethandler之前
void SystemInit(void)
{
	unsigned char work_mode = 0;
	unsigned int xtal_freq = 0;
	unsigned char hclk_div = 10;
	unsigned char pclk_div = 1;
	unsigned int xtal_32k = 0;
	unsigned int rc32k_freq;
	unsigned int cur_fac_addr;
	
	cur_fac_addr = CheckFactoryNvValid();

    if(cur_fac_addr != 0)
	{
		work_mode = ((factory_nv_t *)(cur_fac_addr))->softap_fac_nv.work_mode;
		hclk_div = ((factory_nv_t *)(cur_fac_addr))->softap_fac_nv.hclk_div;
		pclk_div = ((factory_nv_t *)(cur_fac_addr))->softap_fac_nv.pclk_div;
		xtal_32k = ((factory_nv_t *)(cur_fac_addr))->softap_fac_nv.xtal_32k;
		rc32k_freq = ((factory_nv_t *)(cur_fac_addr))->softap_fac_nv.rc32k_freq;
	}
	else
	{
		work_mode = ((factory_nv_t *)(NV_FLASH_MAIN_FACTORY_BASE))->softap_fac_nv.work_mode;
		hclk_div = ((factory_nv_t *)(NV_FLASH_MAIN_FACTORY_BASE))->softap_fac_nv.hclk_div;
		pclk_div = ((factory_nv_t *)(NV_FLASH_MAIN_FACTORY_BASE))->softap_fac_nv.pclk_div;
		xtal_32k = ((factory_nv_t *)(NV_FLASH_MAIN_FACTORY_BASE))->softap_fac_nv.xtal_32k;
		rc32k_freq = ((factory_nv_t *)(NV_FLASH_MAIN_FACTORY_BASE))->softap_fac_nv.rc32k_freq;
	}

	xtal_freq =  ((factory_nv_t *)(NV_FLASH_MAIN_FACTORY_BASE))->softap_fac_nv.xtal_freq;

	uint8_t hclk_valid_value[16] = {0, 0, 1, 1, 1, 1, 1, 1, 0, 1, 0, 1, 0, 0, 0, 1};

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

	if(rc32k_freq != 0)
	{
		g_XY_RC32K_CLK = (rc32k_freq + 5) / 10;
	}
	
	if(work_mode == 2)
	{
		g_XY_HCLK_DIV=1;				
		g_XY_PCLK_DIV=1;				
		g_XY_HCLK=(xtal_freq/g_XY_HCLK_DIV);
		g_XY_PCLK=(g_XY_HCLK/g_XY_PCLK_DIV);
		g_OS_SYS_CLOCK=g_XY_HCLK;
	}
	else
	{
		g_XY_HCLK = XY_BBPLL_CLK / hclk_div;
		g_XY_PCLK = g_XY_HCLK / pclk_div;
		g_XY_PCLK_DIV = pclk_div;
		g_XY_HCLK_DIV = hclk_div;
		g_OS_SYS_CLOCK = g_XY_HCLK;
	}

	HWREG(CLOCK_DIV_VALUE) = (pclk_div << 8) | hclk_div;

}


/*----------------------------------------------------------------------------
  System initialization function
 *----------------------------------------------------------------------------*/


void utc_init()
{
	SEMARequest(SEMA_BASE,SEMA_MASTER_SELECT,SEMA_SLAVE_UTC);

	//if(get_sys_up_stat() != UTC_WAKEUP && get_sys_up_stat() != EXTPIN_WAKEUP)
	//if status invalid,reset utc
	if(UTCValidStatusGet() != 0x0f)
	{
		HWD_REG_WRITE32(0xa0050000,0x7);
		UTCIntDisable(UTC_INT_ALARM);
		UTCTimerStop();
	    UTCCalStop();

	    UTCTimerSet(0, 0, 0, 0, 0);
		UTCCalSet(20, 00, 1, 1, 7);

	}
	UTCTimerRun();
	UTCCalRun();
	UTCIntEnable(UTC_INT_ALARM);
	UTCAlarmEnable(UTC_ALARM_ALL);

	SEMARelease(SEMA_BASE,SEMA_MASTER_SELECT,SEMA_SLAVE_UTC);
}

unsigned int g_xtal_switch = 0;
void HardWare_Init(void)
{
	
	get_sys_up_stat();

	HWREGB(0xa011003f)|=0x02;    //300us
	HWREGB(0xa005800c)|=0x0a;    //(4+10)*30us

	HWREGB(0xA011002C)|=(1<<4); //force pll_lock to digital	


    PRCM_Clock_Enable(PRCM_BASE, PRCM_CKG_CTL_SEMA_EN | PRCM_CKG_CTL_WDT_EN | PRCM_CKG_CTL_QSPI_EN | PRCM_CKG_CTL_UTC_EN | PRCM_CKG_CTL_DMAC_EN | PRCM_CKG_CTL_GPIO_EN );
    
    HWREGB(0xA0058000) = 0x00;			//sleep mode clean    

	NVIC_ClearPendingIRQ(WAKEUP_IRQn);

	DMAChannelControlSet(DMA_CHANNEL_0,DMAC_CTRL_DINC_SET|DMAC_CTRL_SINC_SET|DMAC_CTRL_TC_SET|DMAC_CTRL_TYPE_MEM_TO_MEM|DMAC_CTRL_BURST_SIZE_16W|DMAC_CTRL_WORD_SIZE_32b);	

	HWREGB(0xA011002C)|=(1<<4); //force pll_lock to digital

	#ifdef XY_S_VER
	HWREGB(0xA0058034) = 0x05;
	#endif
}


void HardWare_Init_AfterNV(void)
{

	rtc_event_refresh();
	// bug3857
	if(g_softap_fac_nv->xtal_type == 1)	// XO
	{
		HWREGB(0xA011003E) = (HWREGB(0xA011003E) & 0x3F) | 0x80;
	}
	else
	{
		HWREGB(0xA011003E) = 0xC6;
	}

	//maql,vary PIN RESET time
	if(g_softap_fac_nv->resetctl==0)
	{
		if(HWREG(0xA0110010) == 0x00010000)	// B0p1, old chip
		{
			HWREGB(0xA005800E) |= 0x08;
		}
		else								// B0p2, new chip
		{
			HWREGB(0xA005800E) &= 0xF7;
		}
	}
	else
	{
		if(HWREG(0xA0110010) == 0x00010000)	// B0p1, old chip
		{
			
			HWREGB(0xA005800E) &= 0xF7;
		}
		else								// B0p2, new chip
		{
			HWREGB(0xA005800E) |= 0x08;
		}
	}

	//bug4126
	if (g_softap_fac_nv->aon_uvlo_en == 0) {
		HWREGB(0xA0058019) &= ~(1<<4);
	} else {
		HWREGB(0xA0058019) |= (1<<4);
	}

	int chip_type = (HWREG(OTPINFO_RAM_ADDR + 0x4C) >> 24) & 0xF; 

	if (g_softap_fac_nv->pmu_ioldo_sel == 1 && chip_type == 2)
	{
		HWREGB(0xA0058021) |= 0x20;    //IOLDO vout select: 0: 3V    1: 1.8V
	} 	
	else
	{
		HWREGB(0xA0058021) &= 0xDF;    //IOLDO vout select: 0: 3V    1: 1.8V
	}
}


//extern void PLL_INT_Handler(void);
extern void Wakeup_INT_Handler(void);
extern void rtc_int_handler(void);
extern void WDT_INT_Handler(void);

void PeripheralIRQ_init(void)
{

	NVIC_IntRegister(WAKEUP_IRQn, Wakeup_INT_Handler, 1);
    HWREG(PRCM_BASE + 0x0) |= (1 << 17);

    NVIC_IntRegister(UTC_IRQn, rtc_int_handler, 1);

	NVIC_IntRegister(WDT_IRQn, WDT_INT_Handler, 0);
#if BLE_ENABLE
    if(g_softap_fac_nv->chip_with_ble)
    	ble_uart_gpio_init();
#endif

	at_uart_init();
}


extern QSPI_FLASH_Def xinyi_flash;
extern void IpcMsg_Buf_init(void);

unsigned int system_init()
{
	unsigned int uwRet = pdPASS;

    HardWare_Init();
	
	IpcMsg_Buf_init();

	sys_flash_init();

	g_dynamic_dsp_load_m = osMutexNew(osMutexPrioInherit);
	/*****************************************************/
	g_rtc_sem = osSemaphoreNew(0x01, 0);
	g_rtc_mutex = osMutexNew(osMutexRecursive);

	/*****************************************************/
	

	FLASH_GetDeviceID();

	nv_restore();

	HardWare_Init_AfterNV();
	    
	PeripheralIRQ_init();

	return uwRet;
}


void delay_by_utc_tick(unsigned char ticks)
{
	unsigned int regval0;
	unsigned int regval1;
	unsigned int carry_bit;

	regval0 = HWREG(UTC_BASE + UTC_CLK_CNT);

	if(((regval0 & 0xFF) + ticks) >= 160)
	{
		regval1 = ((regval0 & 0xFF) + ticks) - 160;

		carry_bit = (regval0 & 0x100);

		regval1 |= (carry_bit ^ 0x100);
	}
	else
	{
		regval1 = regval0 + ticks;
	}
	
	while(HWREG(UTC_BASE + UTC_CLK_CNT) < regval1);

}

// add for bug4000,unused!!!
// return value: 0, normal voltage; 1, low voltage, below 3.06V;
unsigned char get_vbat_detect(void)
{
	unsigned char i;
	
	if(!(HWREGB(0xA0110030) & 0x02))
	{
		return 0;
	}

	// detect low voltage first time
	for(i = 0; i < VBAT_DET_RETRY_NUM; i++)
	{
		// delay at least 1 32k tick
		delay_by_utc_tick(2);		// utc clock always works?????? add check flag?????? 

		if(!(HWREGB(0xA0110030) & 0x02))
		{
			return 0;				// jitter, normal voltage
		}
	}

	// retry more times, really low voltage
	return 1;
}



