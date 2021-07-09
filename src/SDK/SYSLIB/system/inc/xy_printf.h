#pragma once

#include "xy_log.h"
#include "xy_nbiot_module_define.h"
#include "factory_nv.h"

#define AP_LOG_MAX_SIZE 104
/**
 * @brief only used for XinYi softap print,user cannot use it
 * @warning 当内核没有调度时，系统不会输出log！并且该接口不可以在锁中断或者临界区中的调用，否则会断言！
 */
#define softap_printf(src, loglevel, fmt, args...) \
  do                                               \
  {                                                \
    {                                              \
      xy_log_print(src, loglevel, fmt, ##args);    \
    }                                              \
  } while (0)

/**
  * @brief   Used for print log,see  the standard C library "printf"
  * @param   format	   Refer to the standard C library "printf"
  * @return  int
  * @warning 当内核没有调度时，系统不会输出log！并且该接口不可以在锁中断或者临界区中的调用，否则会断言！ 
  */
int xy_log_print(unsigned char src_id, unsigned char log_level,const char *format,...);
