/*
 * FreeRTOS Kernel V10.3.1
 * Copyright (C) 2020 Amazon.com, Inc. or its affiliates.  All Rights Reserved.
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy of
 * this software and associated documentation files (the "Software"), to deal in
 * the Software without restriction, including without limitation the rights to
 * use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of
 * the Software, and to permit persons to whom the Software is furnished to do so,
 * subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS
 * FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR
 * COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER
 * IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
 * CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 *
 * http://www.FreeRTOS.org
 * http://aws.amazon.com/freertos
 *
 * 1 tab == 4 spaces!
 */


#ifndef FREERTOS_CONFIG_H
#define FREERTOS_CONFIG_H

#include "xinyi_hardware.h"
#include "xy_clk_config.h"

// assertion
#include "sys_debug.h"
#define configASSERT(x)    Sys_Assert(x)


/***************************************************************************************************************/
/*                                        FreeRTOS base configuration options                                  */
/***************************************************************************************************************/
#define configUSE_PREEMPTION					1                       //1: use preemption kennel 0: use coroutine (always set 1)
#define configUSE_TIME_SLICING					1						//1: use time slicing
#define configUSE_PORT_OPTIMISED_TASK_SELECTION	1                       //1: Cortex-M3 use "CLZ" to calculate the leading zero, set 0 if not have this command 
#define configUSE_TICKLESS_IDLE					0                       //1: use tickless
#define configUSE_QUEUE_SETS					1                       //1: use queue
#define configCPU_CLOCK_HZ						(XY_HCLK)               //CPU clock frequency
#define configTICK_RATE_HZ						(1000)                  //tick rate frequency, current Period is 1ms
#define configMAX_PRIORITIES					(32)                    //max priorities
#define configMINIMAL_STACK_SIZE				(64)                    //the minimal stack size
#define configMAX_TASK_NAME_LEN					(16)                    //task name string length
#define configUSE_16_BIT_TICKS					0                       //1: 32 bits unsigned variable to record ticks 0: 16 bits variable
#define configIDLE_SHOULD_YIELD					1                       //1: Idle tasks give up CPU usage to other user tasks of the same priority
#define configUSE_TASK_NOTIFICATIONS            1                       //1: Enable task notification 0: Disable
#define configUSE_MUTEXES						1                       //1: use mutex
#define configQUEUE_REGISTRY_SIZE				0                       //0: disable queue registry, if not 0, it's the max number of queue registry
#define configCHECK_FOR_STACK_OVERFLOW			2                       //1: enable stack overflow check, user need provide stack overflow check hook function, user can set 1 or 2
                                                                        //0: disable stack overflow check
#define configRECORD_STACK_HIGH_ADDRESS         1                       //1: record stack bottom
#define configUSE_RECURSIVE_MUTEXES				1                       //1: enable recursive mutex
#define configUSE_APPLICATION_TASK_TAG			0                       //1: enable application task tag
#define configUSE_COUNTING_SEMAPHORES			1                       //1: enable counting semaphores
#define configUSE_NEWLIB_REENTRANT				1                       //1: enable newlib reent, create a reent struct for every task

/***************************************************************************************************************/
/*                                FreeRTOS memory allocation config                                            */
/***************************************************************************************************************/
#define configSUPPORT_STATIC_ALLOCATION         0                       //1: support static allocation
#define configSUPPORT_DYNAMIC_ALLOCATION        1                       //1: support dynamic allocation
#define configTOTAL_HEAP_SIZE					((size_t)(1024*30))     //system heap size
#define configAPPLICATION_ALLOCATED_HEAP        1                       //1: use allocatied heap

/***************************************************************************************************************/
/*                                FreeRTOS hook function config                                                */
/***************************************************************************************************************/
#define configUSE_IDLE_HOOK						1                       //1: use idle hook
#define configUSE_TICK_HOOK						1                       //1: use time slicing hook
#define configUSE_MALLOC_FAILED_HOOK			0                       //1: use malloc failed hook
#define configUSE_DAEMON_TASK_STARTUP_HOOK		0                       //1: use daemon task startup hook

extern void task_switch_in_hook(void);
extern void task_switch_out_hook(void);
#ifndef TRACEALYZER_ENABLE
#define traceTASK_SWITCHED_IN()                 task_switch_in_hook()
#define traceTASK_SWITCHED_OUT()                task_switch_out_hook()
#endif

/***************************************************************************************************************/
/*                                FreeRTOS running time and task status collection config                      */
/***************************************************************************************************************/
#define configGENERATE_RUN_TIME_STATS	        0                       //1: enable runtime statistics function
#define configUSE_TRACE_FACILITY				1                       //1: enable visual trace debugging
#define configUSE_STATS_FORMATTING_FUNCTIONS	0                       //1: if set 1 and configUSE_TRACE_FACILITY is 1, compiling "prvWriteNameToBuffer()","vTaskList()","vTaskGetRunTimeStats()"

/***************************************************************************************************************/
/*                                FreeRTOS coroutine config                                                    */
/***************************************************************************************************************/
#define configUSE_CO_ROUTINES 			        0                       //1: enable coroutine, must add file "croutine.c"
#define configMAX_CO_ROUTINE_PRIORITIES         2                       //the number of valid priorites

/***************************************************************************************************************/
/*                                FreeRTOS software timer config                                               */
/***************************************************************************************************************/
#define configTIMER_SERVICE_TASK_NAME           "Tmr Svc"                       //timer task name
#define configUSE_TIMERS				        1                               //1: enable software timer
#define configTIMER_TASK_PRIORITY		        (configMAX_PRIORITIES-1)        //software timer priorities
#define configTIMER_QUEUE_LENGTH		        5                               //queue length of software timer
#define configTIMER_TASK_STACK_DEPTH	        (256)                           //software timer task stack size

/***************************************************************************************************************/
/*                                FreeRTOS idle config                                                         */
/***************************************************************************************************************/
#define configIDLE_TASK_NAME                    "IDLE"                          //idle task name
#define configIDLE_TASK_STACK_DEPTH	            (256)                           //idle task stack size

/***************************************************************************************************************/
/*                                FreeRTOS include functions                                                   */
/***************************************************************************************************************/
#define INCLUDE_xTaskGetSchedulerState          1
#define INCLUDE_vTaskPrioritySet		        1
#define INCLUDE_uxTaskPriorityGet		        1
#define INCLUDE_vTaskDelete				        1
#define INCLUDE_vTaskCleanUpResources	        1
#define INCLUDE_vTaskSuspend			        1
#define INCLUDE_vTaskDelayUntil			        1
#define INCLUDE_vTaskDelay				        1
#define INCLUDE_eTaskGetState			        1
#define INCLUDE_uxTaskGetStackHighWaterMark     1
#define INCLUDE_xTaskGetIdleTaskHandle          1
#define INCLUDE_xTaskGetCurrentTaskHandle       1
#define INCLUDE_xQueueGetMutexHolder            1
#define INCLUDE_xTimerPendFunctionCall	        1

/***************************************************************************************************************/
/*                                FreeRTOS interrupt config                                                    */
/***************************************************************************************************************/
#ifdef __NVIC_PRIO_BITS
	#define configPRIO_BITS       		__NVIC_PRIO_BITS
#else
	#define configPRIO_BITS       		3
#endif

#define configLIBRARY_LOWEST_INTERRUPT_PRIORITY         7                       //interrupt lowest priority
#define configLIBRARY_MAX_SYSCALL_INTERRUPT_PRIORITY    1                       //the max interrupt priority FreeRTOS can manage
#define configKERNEL_INTERRUPT_PRIORITY         ( configLIBRARY_LOWEST_INTERRUPT_PRIORITY << ( 8 - configPRIO_BITS ) )
#define configMAX_SYSCALL_INTERRUPT_PRIORITY    ( configLIBRARY_MAX_SYSCALL_INTERRUPT_PRIORITY << ( 8 - configPRIO_BITS ) )

/***************************************************************************************************************/
/*                                FreeRTOS interrupt service function                                          */
/***************************************************************************************************************/
#define xPortPendSVHandler 	   PendSV_Handler
#define xPortSysTickHandler    SysTick_Handler
#define vPortSVCHandler 	   SVC_Handler


/***************************************************************************************************************/
/*                                FreeRTOS Heap debug config                                                   */
/***************************************************************************************************************/
#define configMALLOC_USE_HEAP_6                 1                       //1: use heap_6.c
#define configMALLOC_USE_HEAP_7                 0                       //1: use heap_7.c

#define configUSE_HEAP_USED_LINKED_LIST         1                       //1: enable used memory node linked to a list
#define configUSE_HEAP_MALLOC_DEBUG             1                       //1: add file and line into memory node structure


/***************************************************************************************************************/
/*                                Custom FreeRTOS for xinyi                                                    */
/***************************************************************************************************************/
#define configTASK_SWITCH_PUSH_REG_NUM    16      // the number of task pushed register when task yield, never changed
#define configTASK_SHARED_STACK_DEPTH     1024    // the mainly stack size of all task (units: word(4 bytes))
#define configUSE_LOW_POWER_FLAG          1       // 1: enable low power flag, use sleep statistics
#define configDYNAMIC_OPTION_SYSTEM_TICK  1       // 1: enable dynamic option system tick


#define configTOTAL_HEAP1_SIZE            (1024*20)          // use for heap_7.c
#define configTOTAL_HEAP2_SIZE            (1024*20)
#define configTOTAL_HEAP3_SIZE            (1024*20)

#ifdef TRACEALYZER_ENABLE
	#include "trcRecorder.h"
#endif

#endif /* FREERTOS_CONFIG_H */
//

