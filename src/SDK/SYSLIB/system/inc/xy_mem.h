/**
  ******************************************************************************
  * @file    xy_mem.h
  * @brief   This file contains memory defines, enumeration and
  *          structures definitions.
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

#ifndef _XY_MEM_H
#define _XY_MEM_H
#include <stdint.h>

#include "osal.h"
#include "portable.h"
#include "FreeRTOS.h"

#define CACHE_LINE		32

#define EXT_MEM_TRACE       0
#define EXT_MEM_TRACE_NUM   100

/* 关闭heap6的trace后该功能无法使用 */
#if !configUSE_HEAP_MALLOC_DEBUG
#define EXT_MEM_TRACE       0
#endif

typedef struct _MemTrace
{
	void * mem;
	char * file;
	uint16_t line;
	uint16_t isfree; 
} MemTrace_t;

/* ring buffer control */
typedef struct _RBCtrl
{
	void * startAddr;
	uint32_t uintSize;
	uint32_t pos;
	uint32_t size;
} RBCtrl_t;

/** @brief  重新分配内存
  * @param  mem是原来内存的地址
  * @param  size是需要重新分配的内存的大小
  * @note 如果需要32字节cache行对齐要用xy_realloc_Align
  */
#define xy_realloc(mem, size)   		osMemoryRealloc(mem, size)
#define xy_realloc_Align(mem, size)   	osMemoryReallocAlign(mem, size)

void xy_MemFree(void *mem);
void xy_MemFree_Ps(void *mem);

#if( configUSE_HEAP_MALLOC_DEBUG == 1 )
void* xy_MemAllocDebug(size_t size, char *file, int line, uint8_t loose, uint8_t clean);
void* xy_MemAllocDebug_Ps(size_t size, char *file, int line, uint8_t loose, uint8_t clean);

#define XY_MALLOC(size)          xy_MemAllocDebug(size, __FILE__, __LINE__, 0, 0)
#define XY_ZALLOC(size)          xy_MemAllocDebug(size, __FILE__, __LINE__, 0, 1)
#define XY_MALLOC_LOOSE(size)    xy_MemAllocDebug(size, __FILE__, __LINE__, 1, 0)
#define XY_ZALLOC_LOOSE(size)    xy_MemAllocDebug(size, __FILE__, __LINE__, 1, 1)

#define XY_MALLOC_PS(size)       xy_MemAllocDebug_Ps(size, __FILE__, __LINE__, 0, 0)
#define XY_ZALLOC_PS(size)       xy_MemAllocDebug_Ps(size, __FILE__, __LINE__, 0, 1)

#else
void* xy_MemAlloc(size_t size, uint8_t loose, uint8_t clean);
void* xy_MemAlloc_Ps(size_t size, uint8_t loose, uint8_t clean);

#define XY_MALLOC(size)          xy_MemAlloc(size, 0, 0)
#define XY_ZALLOC(size)          xy_MemAlloc(size, 0, 1)
#define XY_MALLOC_LOOSE(size)    xy_MemAlloc(size, 1, 0)
#define XY_ZALLOC_LOOSE(size)    xy_MemAlloc(size, 1, 1)

#define XY_MALLOC_PS(size)       xy_MemAlloc_Ps(size, 0, 0)
#define XY_ZALLOC_PS(size)       xy_MemAlloc_Ps(size, 0, 1)

#endif

#if( configUSE_HEAP_MALLOC_DEBUG == 1 )
void* xy_MemAllocAlignDebug(size_t size, char *file, int line, uint8_t loose, uint8_t clean);
#define XY_MALLOC_ALIGN(size)          xy_MemAllocAlignDebug(size, __FILE__, __LINE__, 0, 0)
#define XY_ZALLOC_ALIGN(size)          xy_MemAllocAlignDebug(size, __FILE__, __LINE__, 0, 1)
#define XY_MALLOC_ALIGN_LOOSE(size)    xy_MemAllocAlignDebug(size, __FILE__, __LINE__, 1, 0)
#define XY_ZALLOC_ALIGN_LOOSE(size)    xy_MemAllocAlignDebug(size, __FILE__, __LINE__, 1, 1)
#else
void* xy_MemAllocAlign(size_t size, uint8_t loose, uint8_t clean);
#define XY_MALLOC_ALIGN(size)          xy_MemAllocAlign(size, 0, 0)
#define XY_ZALLOC_ALIGN(size)          xy_MemAllocAlign(size, 0, 1)
#define XY_MALLOC_ALIGN_LOOSE(size)    xy_MemAllocAlign(size, 1, 0)
#define XY_ZALLOC_ALIGN_LOOSE(size)    xy_MemAllocAlign(size, 1, 1)
#endif


/** @brief  宽松的内存申请接口，返回NULL表示申请失败
  */
#define xy_malloc_loose(size)   		XY_MALLOC_LOOSE(size)
#define xy_malloc_Align_loose(size)   	XY_MALLOC_ALIGN_LOOSE(size)
#define xy_zalloc_loose(size)   		XY_ZALLOC_LOOSE(size)
#define xy_zalloc_Align_loose(size)   	XY_ZALLOC_ALIGN_LOOSE(size)

#define xy_free_ps(mem)                 xy_MemFree_Ps(mem)
#define xy_malloc_ps(ulSize)            XY_MALLOC_PS(ulSize)
#define xy_zalloc_ps(ulSize)            XY_ZALLOC_PS(ulSize)

extern uint32_t g_ps_mem_used;
extern uint32_t g_ps_mem_used_max;
extern uint32_t g_ps_mem_used_threshod;

int  is_DSP_heap_addr(unsigned int addr);
int  is_ARM_heap_addr(unsigned int addr);

int  is_DSP_flash_addr(unsigned int addr);
int  is_ARM_flash_addr(unsigned int addr);

int  is_SRAM_addr(unsigned int addr);

/** @brief  将M3核的内存地址转化为DSP核的内存地址
  * @param  addr是需要转化的原内存地址
  * @retval 转化后的内存地址
  */
unsigned int get_Dsp_Addr_from_ARM(unsigned int addr);

/** @brief  将DSP核的内存地址转化为M3核的内存地址
  * @param  addr是需要转化的原内存地址
  * @retval 转化后的内存地址
  */
unsigned int get_ARM_Addr_from_Dsp(unsigned int addr);
void *shm_mem_malloc(int ulSize);

#if EXT_MEM_TRACE
void RBInit(RBCtrl_t * rb, uint32_t uintsize, uint32_t size);
void RBPush(RBCtrl_t * rb, void * uint, uint32_t uintsize);
void extMemTraceInit(void);
void extMemTrace(void * mem, uint8_t isfree);
void getMemTrace(MemTrace_t * mt);
#endif

size_t CalRealSize(void * mem);

#endif
