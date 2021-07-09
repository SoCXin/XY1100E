/**
  ******************************************************************************
  * @file    xy_mem.c
  * @brief   This file contains memory function prototype.
  ******************************************************************************
  * @attention
  *
  * Licensed under the Apache License, Version 2.0 (the "License");
  * you may not use this file except in compliance with the License.
  * You may obtain a copy of the License at

  * http://www.apache.org/licenses/LICENSE-2.0

  * Unless required by applicable law or agreed to in writing, software
  * distributed under the License is distributed on an "AS IS" BASIS,
  * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
  * See the License for the specific language governing permissions and
  * limitations under the License.
  *
  ******************************************************************************
  */

#include "xy_mem.h"
#include "xy_utils.h"
#include "oss_nv.h"
#include "task.h"
#include "osal.h"
#include "ipcmsg.h"
#include "inter_core_msg.h"
#include "xy_memmap.h"
#include "factory_nv.h"
#include "xy_flash.h"
#include "xy_list.h"

uint32_t g_ps_mem_used;
uint32_t g_ps_mem_used_max;
uint32_t g_ps_mem_used_threshod;

int  is_SRAM_addr(unsigned int addr)
{
	xy_assert((addr >= SRAM_BASE) && (addr <= SRAM_BASE + SRAM_LENGTH));
	return 0;
}
int  is_DSP_heap_addr(unsigned int addr)
{(void) addr;return 0;}
int  is_ARM_heap_addr(unsigned int addr)
{(void) addr;return 0;}
int  is_DSP_flash_addr(unsigned int addr)
{(void) addr;return 0;}
int  is_ARM_flash_addr(unsigned int addr)
{(void) addr;return 0;}
/** @brief  将M3核的内存地址转化为DSP核的内存地址
  * @param  addr是需要转化的原内存地址
  * @retval 转化后的内存地址
  */
unsigned int get_Dsp_Addr_from_ARM(unsigned int addr)
{
    unsigned int DSPAddr = addr;

	//flash
	if(addr >= FLASH_BASE && addr < (FLASH_BASE + FLASH_LENGTH)) 
	{
		DSPAddr = addr + FLASH_M3_DSP_OFFSET;
    }
	//sram
	else if(addr >= SRAM_BASE && addr < (SRAM_BASE + SRAM_LENGTH)) 
	{
	    DSPAddr = addr + SRAM_M3_DSP_OFFSET;
    }
	//retension memory
	else if(addr >= BACKUP_MEM_BASE && addr < (BACKUP_MEM_BASE + XY_FTL_AVAILABLE_SIZE)) 
	{
        DSPAddr = addr + BACKUP_MEM_OFFSET;
    }
	//AES
    else if(addr >= AES_START && addr < (AES_START + AES_LENGTH))
    {
        DSPAddr = addr + AES_OFFSET;
    }
	
	return DSPAddr;
}
/** @brief  将DSP核的内存地址转化为M3核的内存地址
  * @param  addr是需要转化的原内存地址
  * @retval 转化后的内存地址
  */
unsigned int get_ARM_Addr_from_Dsp(unsigned int addr)
{
    unsigned int DSPAddr = addr;

	//flash
	if(addr >= (FLASH_BASE + FLASH_M3_DSP_OFFSET) && addr < (FLASH_BASE + FLASH_M3_DSP_OFFSET + FLASH_LENGTH)) 
	{
		DSPAddr = addr - FLASH_M3_DSP_OFFSET;
    }
	//sram
	else if(addr >= (SRAM_BASE + SRAM_M3_DSP_OFFSET) && addr < (SRAM_BASE + SRAM_M3_DSP_OFFSET + SRAM_LENGTH)) 
	{
	    DSPAddr = addr - SRAM_M3_DSP_OFFSET;
    }
	//retension memory
	else if(addr >= (BACKUP_MEM_BASE + BACKUP_MEM_OFFSET) && addr < (BACKUP_MEM_BASE + BACKUP_MEM_OFFSET + XY_FTL_AVAILABLE_SIZE)) 
	{
        DSPAddr = addr - BACKUP_MEM_OFFSET;
    }
	//AES
    else if(addr >= ((unsigned int)AES_START + (unsigned int)AES_OFFSET) && addr < ((unsigned int)AES_START + (unsigned int)AES_OFFSET + (unsigned int)AES_LENGTH))
    {
        DSPAddr = addr - AES_OFFSET;
    }
	
	return DSPAddr;
}


#if( configUSE_HEAP_MALLOC_DEBUG == 1 )
void* xy_MemAllocDebug(size_t size, char *file, int line, uint8_t loose, uint8_t clean)
#else
void* xy_MemAlloc(size_t size, uint8_t loose, uint8_t clean)
#endif
{
	void *mem = NULL;
#if( configUSE_HEAP_MALLOC_DEBUG == 1 )
	mem = osMemoryAllocDebug(size, file, line);
#else
	mem = osMemoryAlloc(size);
#endif
	if(mem == NULL)
	{
		if(!loose)
		{
			xy_assert(0);
		}
	}
	else
	{
		#if configUSE_HEAP_MALLOC_DEBUG 
		if(((uint8_t)(g_softap_fac_nv->test) & 0xFE) /* 大于等于2 */
			&& (size > (size_t)(g_softap_fac_nv->test))
			&& (osKernelGetState() == osKernelRunning)
			&& (osCoreGetState() == osCoreNormal)
			&& strncmp(file, "xy_print", 8)  /* exclude xy_printf.c 避免递归 */
			&& strncmp(file, "diag_str", 8)) /* exclude diag_string_print.c 避免递归 */
		{
			softap_printf(USER_LOG, WARN_LOG,"malloc->size:%d,line:%d,file:%s", size, line, file);
		}
		#endif
		if(clean)
		{	
			if(g_softap_fac_nv->test&1)  
				xy_assert(size < 100);   
			memset(mem, 0, size);
		}
	}
#if EXT_MEM_TRACE
	extMemTrace(mem, 0);
#endif

	return mem;
}

/** @brief  内存分配接口，可单独统计内存使用量
  */
#if( configUSE_HEAP_MALLOC_DEBUG == 1 )
void* xy_MemAllocDebug_Ps(size_t size, char *file, int line, uint8_t loose, uint8_t clean)
#else
void* xy_MemAlloc_Ps(size_t size, uint8_t loose, uint8_t clean)
#endif
{
	void *mem = NULL;
#if( configUSE_HEAP_MALLOC_DEBUG == 1 )
	mem = osMemoryAllocDebug(size, file, line);
#else
	mem = osMemoryAlloc(size);
#endif
	if(mem == NULL)
	{
		if(!loose)
		{
			xy_assert(0);
		}
	}
	else
	{
		if(clean)
		{	
			if(g_softap_fac_nv->test&1)  
				xy_assert(size < 100);   
			memset(mem, 0, size);
		}
		vTaskSuspendAll();
		g_ps_mem_used += CalRealSize(mem);
		if(g_ps_mem_used > g_ps_mem_used_max)
		{
			g_ps_mem_used_max = g_ps_mem_used;
		}
		( void ) xTaskResumeAll();
	}

	if((g_ps_mem_used_threshod != 0)
		&& (g_ps_mem_used_max > g_ps_mem_used_threshod*1024))
	{
		xy_assert(0);
	}
	
	return mem;
}


#if( configUSE_HEAP_MALLOC_DEBUG == 1 )
void* xy_MemAllocAlignDebug(size_t size, char *file, int line, uint8_t loose, uint8_t clean)
#else
void* xy_MemAllocAlign(size_t size, uint8_t loose, uint8_t clean)
#endif
{
	void *mem = NULL;
#if( configUSE_HEAP_MALLOC_DEBUG == 1 )
	mem = osMemoryAllocAlignDebug(size, file, line);
#else
	mem = osMemoryAllocAlign(size);
#endif
	if(mem == NULL)
	{
		if(!loose)
		{
			xy_assert(0);
		}
	}
	else
	{
		#if configUSE_HEAP_MALLOC_DEBUG 
		if(((uint8_t)(g_softap_fac_nv->test) & 0xFE) /* 大于等于2 */
			&& (size > (size_t)(g_softap_fac_nv->test))
			&& (osKernelGetState() == osKernelRunning)
			&& (osCoreGetState() == osCoreNormal)
			&& strncmp(file, "xy_print", 8)  /* exclude xy_printf.c 避免递归 */
			&& strncmp(file, "diag_str", 8)) /* exclude diag_string_print.c 避免递归 */
		{
			softap_printf(USER_LOG, WARN_LOG,"malloc->size:%d,line:%d,file:%s", size, line, file);
		}
		#endif

		if(clean)
		{
			memset(mem, 0, size);
		}
	}
#if EXT_MEM_TRACE
	extMemTrace(mem, 0);
#endif

	return mem;
}

void xy_MemFree(void *mem)
{
	extern uint8_t* _Heap_Begin;
	extern uint32_t __Heap_Size;
	uint8_t *heap_addr = (uint8_t *)&_Heap_Begin;
	uint32_t heap_size = (uint32_t)&__Heap_Size;

	if((((uint32_t)mem >= DSP_HEAP_DRAM_START) && ((uint32_t)mem <= DSP_HEAP_DRAM_START + DSP_HEAP_DRAM_LEN))
	||	( ((uint32_t)mem >= get_ARM_Addr_from_Dsp(*((uint32_t *)DSP_EXT_POOL))) && ((uint32_t)mem <= get_ARM_Addr_from_Dsp(*((uint32_t *)DSP_EXT_POOL)) + *((uint32_t *)(DSP_EXT_POOL + 4)) ))
	)
	{
		mem = (void *)(get_Dsp_Addr_from_ARM((unsigned int)mem));
		shm_msg_write((void *)&mem, sizeof(void *),ICM_FREE);
	}
	else if((((uint32_t)mem >= (uint32_t)heap_addr) && ((uint32_t)mem <= (uint32_t)heap_addr + heap_size)) ||
		(((uint32_t)mem >= ARM_RAM_HEAP2_BASE) && ((uint32_t)mem <= ARM_RAM_HEAP2_BASE + ARM_RAM_HEAP2_LEN)))
	{
#if EXT_MEM_TRACE
		extMemTrace(mem, 1);
#endif
		osMemoryFree(mem);
	}
	else
	{
		xy_assert(0);
	}

}

void xy_MemFree_Ps(void *mem)
{
	extern uint8_t* _Heap_Begin;
	extern uint32_t __Heap_Size;
	uint8_t *heap_addr = (uint8_t *)&_Heap_Begin;
	uint32_t heap_size = (uint32_t)&__Heap_Size;

	if((((uint32_t)mem >= (uint32_t)heap_addr) && ((uint32_t)mem <= (uint32_t)heap_addr + heap_size)) ||
		(((uint32_t)mem >= ARM_RAM_HEAP2_BASE) && ((uint32_t)mem <= ARM_RAM_HEAP2_BASE + ARM_RAM_HEAP2_LEN)))
	{
		vTaskSuspendAll();
		g_ps_mem_used -= CalRealSize(mem);
		( void ) xTaskResumeAll();

		(void)osMemoryFree(mem);
	}
	else
	{
		xy_assert(0);
	}

}

#if EXT_MEM_TRACE
/* init ring buffer */
void RBInit(RBCtrl_t * rb, uint32_t uintsize, uint32_t size)
{
	rb->uintSize = uintsize;
	rb->size = size;
	rb->startAddr = xy_malloc(rb->uintSize * rb->size);
	rb->pos = 0;
}
/* push item into ring buffer */
void RBPush(RBCtrl_t * rb, void * uint, uint32_t uintsize)
{
	if(rb->pos >= rb->size)
	{
		rb->pos = 0;
	}
	memcpy((uint8_t *)(rb->startAddr) + (rb->pos)*uintsize, (uint8_t *)uint, uintsize);
	rb->pos ++;
}

RBCtrl_t mTraceRB;

void extMemTraceInit(void)
{
	static uint8_t init = 0;

	if(!init)
	{
		RBInit(&mTraceRB, sizeof(MemTrace_t), EXT_MEM_TRACE_NUM);
		init = 1;
	}
}

/* 记录最近EXT_MEM_TRACE_NUM次的内存申请释放 */
void extMemTrace(void * mem, uint8_t isfree)
{
	MemTrace_t mt;
	if(mem == NULL)
	{
		return;
	}
	mt.mem = mem;
	getMemTrace(&mt);
	if(isfree)
	{
		mt.isfree = 1;
	}
	else
	{
		mt.isfree = 0;
	}
	osCoreEnterCritical();
	RBPush(&mTraceRB, &mt, sizeof(MemTrace_t));
	osCoreExitCritical();
}
#endif
