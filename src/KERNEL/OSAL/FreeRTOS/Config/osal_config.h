#ifndef __OSAL_CONFIG_H
#define __OSAL_CONFIG_H


#ifndef __OSAL_H_
	#error "cannot include osal_config.h directly, please include osal.h "
#endif


/* ==== Memory Management Select ==== */
#define USE_FreeRTOS_HEAP_6    configMALLOC_USE_HEAP_6
#define USE_FreeRTOS_HEAP_7    configMALLOC_USE_HEAP_7

/* ==== Core Management Functions ==== */
#define INCLUDE_osCoreEnterCritical         1
#define INCLUDE_osCoreExitCritical          1
#define INCLUDE_osCoreGetState              1

/* ==== Kernel Management Functions ==== */
#define INCLUDE_osKernelInitialize          1
#define INCLUDE_osKernelGetInfo             0
#define INCLUDE_osKernelGetState            1
#define INCLUDE_osKernelIsRunningIdle       1
#define INCLUDE_osKernelStart               1
#define INCLUDE_osKernelLock                1
#define INCLUDE_osKernelUnlock              1
#define INCLUDE_osKernelRestoreLock         1
#define INCLUDE_osKernelGetTickCount        1
#define INCLUDE_osKernelGetTickFreq         1
#define INCLUDE_osKernelGetSysTimerCount    0
#define INCLUDE_osKernelGetSysTimerFreq     1

/* ==== Thread Management Functions ==== */
#define INCLUDE_osThreadNew                 1
#define INCLUDE_osThreadGetName             1
#define INCLUDE_osThreadGetId               1
#define INCLUDE_osThreadGetState            1
#define INCLUDE_osThreadGetStackSpace       1
#define INCLUDE_osThreadSetPriority         1
#define INCLUDE_osThreadGetPriority         1
#define INCLUDE_osThreadYield               1
#define INCLUDE_osThreadSuspend             1
#define INCLUDE_osThreadResume              1
#define INCLUDE_osThreadExit                1
#define INCLUDE_osThreadTerminate           1
#define INCLUDE_osThreadGetCount            1
#define INCLUDE_osThreadEnumerate           0

/* ==== Thread Flags Functions ==== */
#define INCLUDE_osThreadFlagsSet            1
#define INCLUDE_osThreadFlagsClear          1
#define INCLUDE_osThreadFlagsGet            1
#define INCLUDE_osThreadFlagsWait           1

/* ==== Generic Wait Functions ==== */
#define INCLUDE_osDelay                     1
#define INCLUDE_osDelayUntil                1

/* ==== Timer Management Functions ==== */
#define INCLUDE_osTimerNew                  1
#define INCLUDE_osTimerGetName              1
#define INCLUDE_osTimerStart                1
#define INCLUDE_osTimerStop                 1
#define INCLUDE_osTimerIsRunning            1
#define INCLUDE_osTimerDelete               1

/* ==== Mutex Management Functions ==== */
#define INCLUDE_osMutexNew                  1
#define INCLUDE_osMutexAcquire              1
#define INCLUDE_osMutexRelease              1
#define INCLUDE_osMutexGetOwner             1
#define INCLUDE_osMutexDelete               1

/* ==== Semaphore Management Functions ==== */
#define INCLUDE_osSemaphoreNew              1
#define INCLUDE_osSemaphoreAcquire          1
#define INCLUDE_osSemaphoreRelease          1
#define INCLUDE_osSemaphoreGetCount         1
#define INCLUDE_osSemaphoreDelete           1

/* ==== Message Queue Management Functions ==== */
#define INCLUDE_osMessageQueueNew           1
#define INCLUDE_osMessageQueuePut           1
#define INCLUDE_osMessageQueueGet           1
#define INCLUDE_osMessageQueueGetCapacity   1
#define INCLUDE_osMessageQueueGetMsgSize    1
#define INCLUDE_osMessageQueueGetCount      1
#define INCLUDE_osMessageQueueGetSpace      1
#define INCLUDE_osMessageQueueReset         1
#define INCLUDE_osMessageQueueDelete        1

/* ==== Memory Management Functions ==== */
#define INCLUDE_osMemoryAlloc               1
#define INCLUDE_osMemoryAllocAlign          1
#define INCLUDE_osMemoryRealloc             1
#define INCLUDE_osMemoryReallocAlign        1
#define INCLUDE_osMemoryFree                1

/* ==== Sleep Management Functions ==== */
#ifndef configUSE_LOW_POWER_FLAG
  #define configUSE_LOW_POWER_FLAG 0
#endif
#define INCLUDE_osThreadSetLowPowerFlag     (1 && (configUSE_LOW_POWER_FLAG == 1 ))
#define INCLUDE_osThreadGetLowPowerFlag     (1 && (configUSE_LOW_POWER_FLAG == 1 ))
#define INCLUDE_osThreadGetLowPowerTime     (1 && (configUSE_LOW_POWER_FLAG == 1 ))
#define INCLUDE_osTimerSetLowPowerFlag      (1 && (configUSE_LOW_POWER_FLAG == 1 ))
#define INCLUDE_osTimerGetLowPowerFlag      (1 && (configUSE_LOW_POWER_FLAG == 1 ))
#define INCLUDE_osTimerGetLowPowerTime      (1 && (configUSE_LOW_POWER_FLAG == 1 ))


//  ==== OS config ====
#define OS_TICK_RATE_HZ      configTICK_RATE_HZ
#define OS_TICK_PERIOD_MS    (1000 / OS_TICK_RATE_HZ)


#endif  /* __OSAL_CONFIG_H */
