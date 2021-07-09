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
#include "hw_timer.h"
#include "dma.h"
#include "uart.h"
#include "hw_csp.h"
#include "rf_drv.h"
#include "prcm.h"
#include "timer.h"
#include "gpio.h"
#include "rtc_tmr.h"
#include "sema.h"
#include "flash_vendor.h"
#include "xy_hwi.h"
#include "xy_utils.h"
#include "rtc_tmr.h"
#include "hw_utc.h"
#include "ipcmsg.h"
#include "csp.h"
#include "dump.h"
#include "watchdog.h"
#include "gpio_wakeup.h"

extern T_IpcMsg_ChInfo g_IpcMsg_ChInfo[IpcMsg_Channel_MAX];

//extern T_RingBuf_Ctl g_ringbuf_ctl[IpcMsg_Channel_MAX];

extern unsigned long g_icm_flag;
#define AlignAddr(size)  (((unsigned int )size + 0x1000 - 1) & (~(0x1000 - 1)))
void Wakeup_INT_Handler(void)
{
	extern uint32_t _Flash_Used;
	extern uint32_t _Ram_Text;
	extern uint32_t _Ram_Data;
	uint32_t Dump_Flash_Area = ARM_FLASH_BASE_LEN +  FOTA_BACKUP_LEN_MAX;
	uint32_t M3_Flash_Size = AlignAddr(AlignAddr((uint32_t)&_Flash_Used) + (uint32_t)&_Ram_Text + (uint32_t)&_Ram_Data);
	uint32_t Dump_Flash_Remain =  Dump_Flash_Area - M3_Flash_Size;
	uint32_t DSP_DUMP_Size = RAM_INVAR_NV_MAXLEN + DSP_DATA_DRAM_LEN + DSP_DATA_SRAM_LEN + RAM_FACTORY_NV_MAXLEN + RAM_ICM_BUF_LEN + RAM_XTEND_SBUF_LEN + 0x9000;

	if(HWREGB(PRCM_BASE + 0x1C) & (1 << 0))
	{
		HWREGB(PRCM_BASE + 0x1C) &= 0xFE;
		if(g_softap_fac_nv->dump_mem_into_flash == 1)
		{
			if(Is_Ringbuf_Empty(g_IpcMsg_ChInfo[IpcMsg_Dump].RingBuf_rcv) != true)
			{
				if(g_softap_fac_nv->off_debug == 0)
				{
					portDISABLE_INTERRUPTS();
				}

				if(Dump_Flash_Remain > DSP_DUMP_Size)
				{
					HWREGB(DUMP_FLASH_SYNC) |= 0x1;
				}
				DumpRegister_from_Normal();
				while(1);
			}
		}
		if(Is_Ringbuf_Empty(g_IpcMsg_ChInfo[IpcMsg_Normal].RingBuf_rcv) != true)
		{
			if(g_icm_flag==0)
				IpcMsg_Semaphore_Give(&(g_IpcMsg_ChInfo[IpcMsg_Normal].read_sema));
		}
		
	}
		
	else
	{
#if GPIO_WAKEUP
		if(g_gpi_wakeupsrc & 0x38)
		{
			do
				{HWREGB(0xA0058001) = 0;}
			while(HWREGB(0xA0058001)&0x38);
			g_gpi_wakeupsrc >>= 0x3;
			osMessageQueuePut(gpio_wakeup_msg_q, (const void*)(&g_gpi_wakeupsrc), 0, osNoWait);
			g_gpi_wakeupsrc = 0;
		}
		else
		{
			g_gpi_wakeupsrc = HWREGB(0xA0058001);
			do
				{HWREGB(0xA0058001) = 0;}
			while(HWREGB(0xA0058001)&0x38);
			if(g_gpi_wakeupsrc & 0x38)
			{
				g_gpi_wakeupsrc >>= 0x3;
				osMessageQueuePut(gpio_wakeup_msg_q, (const void*)(&g_gpi_wakeupsrc), 0, osNoWait);
				g_gpi_wakeupsrc = 0;
			}
		}
#else
		HWREGB(0xA0058001) = 0;
#endif
	}
}
volatile unsigned char g_plllock_check_print = 0;
volatile unsigned long g_plllock_err_num = 0;
volatile unsigned char g_plllock_check_enable = 0;
volatile unsigned long g_plllock_check_num = 0;

#if PLL_DEBUG
volatile unsigned char g_AonPrcm_Reg[0x30] = {0};
volatile unsigned char g_CorePrcm_Reg[0x73] = {0};
volatile unsigned char g_Dfe_Reg[0x1000] = {0};

#endif

void PLL_INT_Handler(void)
{
	g_plllock_check_num++;

	if(g_plllock_check_enable)
	{
#if PLL_DEBUG
		WatchdogDisable(WDT_BASE);

		memcpy(g_AonPrcm_Reg,(char*)0xA0058000,0x30);
		memcpy(g_CorePrcm_Reg,(char*)0xA0110000,0x73);
		memcpy(g_Dfe_Reg,(char*)0x2601f000,0x1000);

		xy_assert(0);
#endif

		g_plllock_check_print = 1;
		g_plllock_err_num++;

	}
	else
		g_plllock_check_enable = 1;

}

void WDT_INT_Handler(void)
{	
	WatchdogIntClear(WDT_BASE);
	xy_assert(0);
}

void rtc_int_handler(void)
{
	//while(HWREG(UTC_BASE + UTCALARMCLKCNT_CFG)&(1<<9));
    volatile unsigned long reg_val = HWREG(UTC_BASE + UTC_INT_STAT);

    (void) reg_val;
	//if(reg_val&UTC_INT_ALARM)
	{
		//wake up dsp core from standby
		//if(HWREG(DSP_LPM_STATE)==DSP_STANDBY_READY)
			//HWREGB(PRCM_BASE + 0x1c) |= 0x02;

		//rtc_alarm_clear();
		osSemaphoreRelease(g_rtc_sem);
	}
	
}
//extern uint64_t csp_overlow_time;


/**
  * @brief  This function is executed in case of error occurrence.
  * @param  file: The file name as string.
  * @param  line: The line in file as a number.
  * @retval None
  */
void _Error_Handler(char *file, int line)
{
	(void) file;
	(void) line;
    /* USER CODE BEGIN Error_Handler_Debug */
    /* User can add his own implementation to report the HAL error return state */
    while(1)
    {
    }
    /* USER CODE END Error_Handler_Debug */
}


