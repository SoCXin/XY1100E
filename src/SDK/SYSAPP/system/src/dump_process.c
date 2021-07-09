#include "diag_read_msg.h"
#include "diag_out.h"
#include "ItemStruct.h"
#include "uart.h"
#include "csp.h"
#include "FreeRTOS.h"
#include "factory_nv.h"
#include "oss_nv.h"
#include "xy_system.h"
#include "diag_mem_dump.h"
#include "TypeDefine.h"
#include "xy_memmap.h"
#include "sys_init.h"
#include "watchdog.h"
#include "diag_format.h"
#include "xy_watchdog.h"


extern unsigned short log_index;
extern unsigned long Current_Stack_Addr;
#define Align_Dump(size , align)  (((unsigned int )size + align - 1) & (~(align - 1)))

void Dump_M3_Into_Flash(void)
{
	extern uint32_t _Flash_Used;
	extern uint32_t _Ram_Text;
	extern uint32_t _Ram_Data;
	extern uint32_t _Ram_Bss;
	extern uint32_t __Main_Stack_Size;

	unsigned int Sram_Size = 0;
	unsigned int Main_Stack_Size = 0;
	unsigned int Task_Stack_Addr = 0;
	unsigned int Dump_Stack_Addr = 0;
	unsigned int offset = 0;

	uint32_t flashdumpsize = ARM_FLASH_BASE_LEN + FOTA_BACKUP_LEN_MAX - (uint32_t)&_Flash_Used -  (uint32_t)&_Ram_Text - (uint32_t)&_Ram_Data - 0x1000;
	if(g_softap_fac_nv->dump_mem_into_flash == 1)
	{
		if(flashdumpsize >= (uint32_t)&_Ram_Text + (uint32_t)&_Ram_Data + (uint32_t)&_Ram_Bss + (uint32_t)&__Main_Stack_Size + RAM_ICM_BUF_LEN + RAM_XTEND_SBUF_LEN +0x3000)
		{
			TaskStatus_t StatusArray = {0};

			vTaskGetInfo(NULL, &StatusArray, pdTRUE, eInvalid );
			Current_Stack_Addr = (unsigned long)(StatusArray.pxStackBase);

			Sram_Size 	 	= (unsigned int)&_Ram_Text + (uint32_t)&_Ram_Data + (uint32_t)&_Ram_Bss;
			Main_Stack_Size = (unsigned int)&__Main_Stack_Size;
			Task_Stack_Addr = (unsigned int)(StatusArray.pxStackBase) + SRAM_OFFSET_BASE;
			Dump_Stack_Addr = Align_Dump((Align_Dump((uint32_t)&_Flash_Used, 0x1000) + (uint32_t)&_Ram_Text + (uint32_t)&_Ram_Data),0x1000) + ARM_FLASH_BASE_ADDR;
			offset = 0;

			xy_flash_write((unsigned int)Dump_Stack_Addr, (void *)M3_SRAM_BASE, Align_Dump(Sram_Size, 0x1000));
			offset += Align_Dump(Sram_Size, 0x1000);

			xy_flash_write((unsigned int)(Dump_Stack_Addr + offset), (void *)(M3_SRAM_END - Align_Dump(Main_Stack_Size, 0x1000)), Align_Dump(Main_Stack_Size, 0x1000));
			offset += Align_Dump(Main_Stack_Size, 0x1000);

			xy_flash_write((unsigned int)(Dump_Stack_Addr + offset), (void *)(Align_Dump(RAM_ICM_BUF_START - 0x1000, 0x1000)), Align_Dump(RAM_ICM_BUF_LEN + RAM_XTEND_SBUF_LEN, 0x2000));
			offset += Align_Dump(RAM_ICM_BUF_LEN + RAM_XTEND_SBUF_LEN, 0x2000);

			xy_flash_write((unsigned int)(Dump_Stack_Addr + offset), (void *)(Align_Dump(Task_Stack_Addr - 0x1000, 0x1000)), 0x3000);
		}
	}
}

extern void Dump_Notice(void);
extern unsigned char query_pc_info();
extern void PC_READY();

unsigned char dump_req_times = 0;
extern unsigned char dump_cnf_times;
volatile unsigned int pcDelay = 0;
volatile unsigned char qPcTimes = 0;

void Start_Dump(void)
{
	if(g_softap_fac_nv->work_mode == 0 || (g_softap_fac_nv->work_mode == 1 && !DSP_IS_NOT_LOADED() ) || (g_softap_fac_nv->work_mode == 2 && !DSP_IS_NOT_LOADED() ))
	{
		Dump_Notice();
	}
	else
	{
		vTaskSuspendAll();
		log_index = 0;

		while(qPcTimes ++ < 2)
		{
			send_mem_ready();
			pcDelay = 100000;
			while(1)
			{
				if(pcDelay-- ==0)
				{
					break;
				}
			}

			PC_CMD(START_DUMP);
			if(0 != query_pc_info())
			{
				break;
			}
			pcDelay = 1000000;
			while(1)
			{
				if(pcDelay-- ==0)
				{
					break;
				}
			}
		}

		while(1)
		{
			if(g_softap_fac_nv->off_debug == 0)
			{
				WatchdogDisable(WDT_BASE);
				if(0 == query_pc_info())
				{
					PC_CMD(START_DUMP);
				}
				else
				{
					mem_dump_info("Dump_Begin", 0x20000000, 0x60000);
					mem_dump_info("sram", 0x20000000, 0x60000);
					mem_dump_info("backupMem", 0x24000000, 0x1000);
					Dump_M3_Into_Flash();
					mem_dump_info("Dump_End", 0x20000000, 0x60000);
				}
			}
			else
			{
				WatchdogDisable(WDT_BASE);
				mem_dump_info("Dump_Begin", 0x20000000, 0x60000);
				mem_dump_info("sram", 0x20000000, 0x60000);
				mem_dump_info("backupMem", 0x24000000, 0x1000);
				Dump_M3_Into_Flash();
				mem_dump_info("Dump_End", 0x20000000, 0x60000);
				WatchdogEnable(WDT_BASE);
				soft_reset_by_wdt(RB_BY_ASSERT);
			}
		}

	}
	
}

volatile char start_dump = 0;

void Start_Dump_No_Recursion(void)
{
	while(start_dump == 1);
	start_dump++;

	Start_Dump();
}
