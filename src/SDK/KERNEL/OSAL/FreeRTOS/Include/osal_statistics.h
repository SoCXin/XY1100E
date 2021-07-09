/**
  ******************************************************************************
  * @file    osal_statistics.h
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

#ifndef __OSAL_STATISTICS_H
#define __OSAL_STATISTICS_H
#include <string.h>
#include "osal.h"
#include "FreeRTOS.h"
#include "task.h"

#define TOTAL_STACK_PEAK_TBL_LEN  40
#define MAIN_STACK_WATER_MARK  (0xA5A5A5A5U)

#define MAX(a , b)  (((a) > (b)) ? (a) : (b))

typedef struct
{
	const char *name; /* task name */
	uint16_t    peakUsed;  /* task stack peak used */
}stackPeak_t;

typedef struct{
    uint32_t flash_total;
    uint32_t flash_used;
	uint32_t ram_total;
    uint32_t text;
    uint32_t data;
	uint32_t bss;
}memStatus_t;

typedef struct{
	const char *name; /* task name */
    uint32_t stackSize;
    uint32_t currentUseSize;
	uint32_t peakUsedSize;
}osStackStatus_t;

extern stackPeak_t stackPeakTbl[];

extern void GetMemStatistics(memStatus_t * MemStatus);
extern uint32_t GetMainStackPeakUsed(void);
extern uint16_t TaskStackMatchHandle(char * Tskname, uint16_t peakUsed);
extern uint32_t GetStackPeakTblUsed(void);
extern void RegTaskNameInStackTbl(const char * name);
extern void GetStackStatistics(osStackStatus_t * Status, UBaseType_t ArraySize);
extern uint32_t GetHeapSize(void);
extern uint32_t GetHeapPeakUsed(void);
extern void ResetHeapPeakUsed(void);

#endif /* __OSAL_STATISTICS_H */
