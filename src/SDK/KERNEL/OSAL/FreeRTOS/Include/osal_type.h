#ifndef __OSAL_TYPE_H
#define __OSAL_TYPE_H


#ifndef __OSAL_H_
	#error "cannot include osal_type.h directly, please include osal.h "
#endif


//  ==== Enumerations, structures, defines ====

/// Version information.
typedef struct {
  uint32_t                       api;   ///< API version (major.minor.rev: mmnnnrrrr dec).
  uint32_t                    kernel;   ///< Kernel version (major.minor.rev: mmnnnrrrr dec).
} osVersion_t;

/// Core state.
typedef enum {
  osCoreNormal            =  0,         ///< Normal running.
  osCoreInInterrupt       =  1,         ///< Running in interrupt.
  osCoreInCritical        =  2,         ///< Running in critical.
  osCoreError             = -1,         ///< Error.
  osCoreReserved          = 0x7FFFFFFFU ///< Prevents enum down-size compiler optimization.
} osCoreState_t;

/// Kernel state.
typedef enum {
  osKernelInactive        =  0,         ///< Inactive.
  osKernelReady           =  1,         ///< Ready.
  osKernelRunning         =  2,         ///< Running.
  osKernelLocked          =  3,         ///< Locked.
  osKernelSuspended       =  4,         ///< Suspended.
  osKernelError           = -1,         ///< Error.
  osKernelReserved        = 0x7FFFFFFFU ///< Prevents enum down-size compiler optimization.
} osKernelState_t;

/// Thread state.
typedef enum {
  osThreadInactive        =  0,         ///< Inactive.
  osThreadReady           =  1,         ///< Ready.
  osThreadRunning         =  2,         ///< Running.
  osThreadBlocked         =  3,         ///< Blocked.
  osThreadTerminated      =  4,         ///< Terminated.
  osThreadError           = -1,         ///< Error.
  osThreadReserved        = 0x7FFFFFFF  ///< Prevents enum down-size compiler optimization.
} osThreadState_t;

/// Priority values.
typedef enum {
  osPriorityNone          = 0,          ///< No priority (not initialized).
  osPriorityIdle          = 1,          ///< Reserved for Idle thread.
  osPriorityLow           = 4,          ///< Priority: low
  osPriorityLow1          = 4+1,        ///< Priority: low + 1
  osPriorityLow2          = 4+2,        ///< Priority: low + 2
  osPriorityLow3          = 4+3,        ///< Priority: low + 3
  osPriorityBelowNormal   = 8,          ///< Priority: below normal
  osPriorityBelowNormal1  = 8+1,        ///< Priority: below normal + 1
  osPriorityBelowNormal2  = 8+2,        ///< Priority: below normal + 2
  osPriorityBelowNormal3  = 8+3,        ///< Priority: below normal + 3
  osPriorityNormal        = 12,         ///< Priority: normal
  osPriorityNormal1       = 12+1,       ///< Priority: normal + 1
  osPriorityNormal2       = 12+2,       ///< Priority: normal + 2
  osPriorityNormal3       = 12+3,       ///< Priority: normal + 3
  osPriorityAboveNormal   = 16,         ///< Priority: above normal
  osPriorityAboveNormal1  = 16+1,       ///< Priority: above normal + 1
  osPriorityAboveNormal2  = 16+2,       ///< Priority: above normal + 2
  osPriorityAboveNormal3  = 16+3,       ///< Priority: above normal + 3
  osPriorityHigh          = 20,         ///< Priority: high
  osPriorityHigh1         = 20+1,       ///< Priority: high + 1
  osPriorityHigh2         = 20+2,       ///< Priority: high + 2
  osPriorityHigh3         = 20+3,       ///< Priority: high + 3
  osPriorityRealtime      = 24,         ///< Priority: realtime
  osPriorityRealtime1     = 24+1,       ///< Priority: realtime + 1
  osPriorityRealtime2     = 24+2,       ///< Priority: realtime + 2
  osPriorityRealtime3     = 24+3,       ///< Priority: realtime + 3
  osPriorityISR           = 31,         ///< Reserved for ISR deferred thread.
  osPriorityError         = -1,         ///< System cannot determine priority or illegal priority.
  osPriorityReserved      = 0x7FFFFFFF  ///< Prevents enum down-size compiler optimization.
} osPriority_t;

/// Entry point of a thread.
typedef void (*osThreadFunc_t) (void *argument);

/// Timer callback function.
typedef void (*osTimerFunc_t) (void *argument);

/// Timer type.
typedef enum {
  osTimerOnce     = 0,                    ///< One-shot timer.
  osTimerPeriodic = 1                     ///< Repeating timer.
} osTimerType_t;

// low power type
typedef enum {
  osLowPowerProhibit = 0,  ///< time will delay for standby sleep
  osLowPowerPermit = 1,    ///< time will on time for standby sleep
  osLowPowerError = 2
} osLowPowerType_t;

typedef enum {
  osIgnorLowPowerFlag = 0,
  osAttentionLowPowerFlag = 1
} osSleepStatisticsType_t;

// Thread stack type
#define osStackShared         0U          ///< Create thread with shared stack

// Timeout value.
#define osWaitForever         0xFFFFFFFFU ///< Wait forever timeout value.
#define osNoWait              0x00000000U ///< No wait timeout value.

/* Limits */
#define MAX_BITS_TASK_NOTIFY      31U
#define MAX_BITS_EVENT_GROUPS     24U

#define THREAD_FLAGS_INVALID_BITS (~((1UL << MAX_BITS_TASK_NOTIFY)  - 1U))
#define EVENT_FLAGS_INVALID_BITS  (~((1UL << MAX_BITS_EVENT_GROUPS) - 1U))

// Flags options (\ref osThreadFlagsWait and \ref osEventFlagsWait).
#define osFlagsWaitAny        0x00000000U ///< Wait for any flag (default).
#define osFlagsWaitAll        0x00000001U ///< Wait for all flags.
#define osFlagsNoClear        0x00000002U ///< Do not clear flags which have been specified to wait for.

// Flags errors (returned by osThreadFlagsXxxx and osEventFlagsXxxx).
#define osFlagsError          0x80000000U ///< Error indicator.
#define osFlagsErrorUnknown   0xFFFFFFFFU ///< osError (-1).
#define osFlagsErrorTimeout   0xFFFFFFFEU ///< osErrorTimeout (-2).
#define osFlagsErrorResource  0xFFFFFFFDU ///< osErrorResource (-3).
#define osFlagsErrorParameter 0xFFFFFFFCU ///< osErrorParameter (-4).
#define osFlagsErrorISR       0xFFFFFFFAU ///< osErrorISR (-6).

// Thread attributes (attr_bits in \ref osThreadAttr_t).
#define osThreadDetached      0x00000000U ///< Thread created in detached mode (default)
#define osThreadJoinable      0x00000001U ///< Thread created in joinable mode

// Mutex attributes (attr_bits in \ref osMutexAttr_t).
typedef enum {
  osMutexRecursive   = 0x00000001U,   ///< Recursive mutex.
  osMutexPrioInherit = 0x00000002U,   ///< Priority inherit protocol.
  osMutexRobust      = 0x00000008U    ///< Robust mutex, cannot set as this type
} osMutexType_t;

/// Status code values returned by CMSIS-RTOS functions.
typedef enum {
  osOK                      =  0,         ///< Operation completed successfully.
  osError                   = -1,         ///< Unspecified RTOS error: run-time error but no other error message fits.
  osErrorTimeout            = -2,         ///< Operation not completed within the timeout period.
  osErrorResource           = -3,         ///< Resource not available.
  osErrorParameter          = -4,         ///< Parameter error.
  osErrorNoMemory           = -5,         ///< System is out of memory: it was impossible to allocate or reserve memory for the operation.
  osErrorISR                = -6,         ///< Not allowed in ISR context: the function cannot be called from interrupt service routines.
  osErrorRTOSStartFailed    = -7,         ///< System start failed
  osErrorIdleBlocked        = -8,         ///< Call Blocked interface in idle task
  osErrorRTOSNotStarted     = -9,         ///< Call Blocked interface before RTOS start
  osErrorNoAlignMemory      = -10,        ///< memory allocated alignment memory failed
  osStatusReserved          = 0x7FFFFFFF  ///< Prevents enum down-size compiler optimization.
} osStatus_t;


/// \details Thread ID identifies the thread.
typedef void *osThreadId_t;

/// \details Timer ID identifies the timer.
typedef void *osTimerId_t;

/// \details Event Flags ID identifies the event flags.
typedef void *osEventFlagsId_t;

/// \details Mutex ID identifies the mutex.
typedef void *osMutexId_t;

/// \details Semaphore ID identifies the semaphore.
typedef void *osSemaphoreId_t;

/// \details Memory Pool ID identifies the memory pool.
typedef void *osMemoryPoolId_t;

/// \details Message Queue ID identifies the message queue.
typedef void *osMessageQueueId_t;


#endif  /* __OSAL_TYPE_H */
