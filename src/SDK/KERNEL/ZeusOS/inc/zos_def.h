/*
 * Copyright (c) 2019-2029, ZeusOS Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author            Notes
 * 2021-03-23     longmain       first version
 */

#ifndef __ZOS_DEF_H__
#define __ZOS_DEF_H__
#include  "sys_debug.h"

typedef signed   char                   zos_int8_t;      /**<  8bit integer type */
typedef signed   short                  zos_int16_t;     /**< 16bit integer type */
typedef signed   long                   zos_int32_t;     /**< 32bit integer type */
typedef signed long long                zos_int64_t;     /**< 64bit integer type */
typedef unsigned char                   zos_uint8_t;     /**<  8bit unsigned integer type */
typedef unsigned short                  zos_uint16_t;    /**< 16bit unsigned integer type */
typedef unsigned long                   zos_uint32_t;    /**< 32bit unsigned integer type */
typedef unsigned long long              zos_uint64_t;    /**< 64bit unsigned integer type */

typedef int                             zos_bool_t;      /**< boolean type */
typedef long                            zos_base_t;      /**< Nbit CPU related date type */
typedef unsigned long                   zos_ubase_t;     /**< Nbit unsigned CPU related data type */

typedef zos_base_t                       zos_err_t;       /**< Type for error number */
typedef zos_uint32_t                     zos_time_t;      /**< Type for time stamp */
typedef zos_uint32_t                     zos_tick_t;      /**< Type for tick count */
typedef zos_base_t                       zos_flag_t;      /**< Type for flags */
typedef zos_ubase_t                      zos_size_t;      /**< Type for size number */
typedef zos_ubase_t                      zos_dev_t;       /**< Type for device */
typedef zos_base_t                       zos_off_t;       /**< Type for offset */


/* ZeusOS error code definitions */
#define ZOS_EOK                          0               /**< There is no error */
#define ZOS_ERROR                       -1               /**< A generic error happens */
#define ZOS_ETIMEOUT                     2               /**< Timed out */
#define ZOS_EFULL                        3               /**< The resource is full */
#define ZOS_EEMPTY                       4               /**< The resource is empty */
#define ZOS_ENOMEM                       5               /**< No memory */
#define ZOS_ENOSYS                       6               /**< No system */
#define ZOS_EBUSY                        7               /**< Busy */
#define ZOS_EIO                          8               /**< IO error */
#define ZOS_EINTR                        9               /**< Interrupted system call */
#define ZOS_EINVAL                       10              /**< Invalid argument */

/**
 * @def ZOS_NULL
 * Similar as the \c NULL in C library.
 */
#define ZOS_NULL                         ((void *)0)


#define ZOS_ASSERT(x)                    Sys_Assert(x)



// Timeout value.
#define ZOS_WAITING_FOREVER         0xFFFFFFFFU ///< Wait forever timeout value.
#define ZOS_WAITING_NO              0x00000000U ///< No wait timeout value.

// #define TASK_CTRL_CLOSE            0x00                /**< Close thread. */
// #define TASK_CTRL_CHANGE_PRIORITY  0x01                /**< Change thread priority. */
// #define TASK_CTRL_INFO             0x02                /**< Get thread information. */


#define ZOS_IPC_FLAG_FIFO                0x00            /**< FIFOed IPC. @ref IPC. */
#define ZOS_IPC_FLAG_PRIO                0x01            /**< PRIOed IPC. @ref IPC. */

#define TIMER_FLAG_ONE_SHOT          0x0             /**< one shot timer */
#define TIMER_FLAG_PERIODIC          0x2             /**< periodic timer */


///zos task state.
typedef enum {
  task_state_Inactive        =  0,         ///< Inactive.
  task_state_Ready           =  1,         ///< Ready.
  task_state_Running         =  2,         ///< Running.
  task_state_Blocked         =  3,         ///< Blocked.
  task_state_Terminated      =  4,         ///< Terminated.
  task_state_Error           = -1,         ///< Error.
  task_state_Reserved        = 0x7FFFFFFF  ///< Prevents enum down-size compiler optimization.
} task_state_t;

///zos Priority values.
typedef enum {
  task_Priority_None          = 0,          ///< No priority (not initialized).
  task_Priority_Idle          = 1,          ///< Reserved for Idle thread.
  task_Priority_Low           = 4,          ///< Priority: low
  task_Priority_Low1          = 4+1,        ///< Priority: low + 1
  task_Priority_Low2          = 4+2,        ///< Priority: low + 2
  task_Priority_Low3          = 4+3,        ///< Priority: low + 3
  task_Priority_BelowNormal   = 8,          ///< Priority: below normal
  task_Priority_BelowNormal1  = 8+1,        ///< Priority: below normal + 1
  task_Priority_BelowNormal2  = 8+2,        ///< Priority: below normal + 2
  task_Priority_BelowNormal3  = 8+3,        ///< Priority: below normal + 3
  task_Priority_Normal        = 12,         ///< Priority: normal
  task_Priority_Normal1       = 12+1,       ///< Priority: normal + 1
  task_Priority_Normal2       = 12+2,       ///< Priority: normal + 2
  task_Priority_Normal3       = 12+3,       ///< Priority: normal + 3
  task_Priority_AboveNormal   = 16,         ///< Priority: above normal
  task_Priority_AboveNormal1  = 16+1,       ///< Priority: above normal + 1
  task_Priority_AboveNormal2  = 16+2,       ///< Priority: above normal + 2
  task_Priority_AboveNormal3  = 16+3,       ///< Priority: above normal + 3
  task_Priority_High          = 20,         ///< Priority: high
  task_Priority_High1         = 20+1,       ///< Priority: high + 1
  task_Priority_High2         = 20+2,       ///< Priority: high + 2
  task_Priority_High3         = 20+3,       ///< Priority: high + 3
  task_Priority_Realtime      = 24,         ///< Priority: realtime
  task_Priority_Realtime1     = 24+1,       ///< Priority: realtime + 1
  task_Priority_Realtime2     = 24+2,       ///< Priority: realtime + 2
  task_Priority_Realtime3     = 24+3,       ///< Priority: realtime + 3
  task_Priority_ISR           = 31,         ///< Reserved for ISR deferred thread.
  task_Priority_Error         = -1,         ///< System cannot determine priority or illegal priority.
  task_Priority_Reserved      = 0x7FFFFFFF  ///< Prevents enum down-size compiler optimization.
} task_priority_t;

// Mutex attributes (attr_bits in \ref osMutexAttr_t).
typedef enum {
  mutex_flag_Recursive   = 0x00000001U,   ///< Recursive mutex.
  mutex_flag_PrioInherit = 0x00000002U,   ///< Priority inherit protocol.
  mutex_flag_Robust      = 0x00000008U    ///< Robust mutex, cannot set as this type
} mutex_type_t;
/// \details Thread ID identifies the thread.
typedef void *zos_task_t;

/// \details Timer ID identifies the timer.
typedef void *zos_timer_t;

/// \details Event Flags ID identifies the event flags.
typedef void *zos_event_t;

/// \details Mutex ID identifies the mutex.
typedef void *zos_mutex_t;

/// \details Semaphore ID identifies the semaphore.
typedef void *zos_sem_t;

/// \details Message Queue ID identifies the message queue.
typedef void *zos_mq_t;

#endif //__ZOS_DEF_H__



