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
#define ZOS_NULL                         (0)



#endif //__ZOS_DEF_H__S



