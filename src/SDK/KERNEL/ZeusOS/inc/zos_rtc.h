/*
 * Copyright (c) 2019-2029, ZeusOS Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author            Notes
 * 2021-03-23     longmain       first version
 */

#ifndef __ZOS_RTC_H__
#define __ZOS_RTC_H__

#include "zos_def.h"


/**
* @brief 用户可用的RTC ID，目前仅支持4个，用户不得增减ID，否则会造成平台内存越界
* @param
*/
typedef enum
{
  OC_RTC_TIMER_USER1 = 5,
  OC_RTC_TIMER_USER2 = 6,
  OC_RTC_TIMER_USER3 = 7,
  OC_RTC_TIMER_USER4 = 8,
  //RTC_TIMER_USER_END = RTC_TIMER_AP_USER_END,
} ZOS_RTC_TIMER_ID;

/**
* @brief  wall time struct
*/
struct nb_time_t
{
  zos_uint32_t tm_sec;  /* Seconds (0-59) */
  zos_uint32_t tm_min;  /* Minutes (0-59) */
  zos_uint32_t tm_hour; /* Hours (0-23) */
  zos_uint32_t tm_mday; /* Day of the month (1-31) */
  zos_uint32_t tm_mon;  /* Month (0-11) */
  zos_uint32_t tm_year; /* Years */
  zos_uint32_t tm_wday; /* Day of the week (0-6),always ignore */
  zos_uint32_t tm_yday; /* Day of the year (0-365),always ignore */
};

typedef void (*zos_rtc_timeout_cb_t)(void *para);

/**
  * @brief   获取NTP网络时间，并更新到本地，以同步本地的世界时间
  * @param   ser_name, ntp服务器地址,NULL表示默认选用ntp1.aliyun.com
  * @param   timeout, 单位秒，填0时，平台内部使用默认值20秒；因NB速率低，建议该值不得小于20秒
  * @param   wall_clock,  世界时间返回值
  * @return  BOOL,see  @ref  xy_ret_Status_t.
  * @note    目前芯翼平台支持两种世界时间的更新途径，一种是3GPP在attach时后台更新当前世界时间；\n
  *  另一种是通过xy_get_ntp_time获取当前网络世界时间。
  * @warning    使用该接口必须确保之前已经PDP激活成功过，否则返回XY_ERR；若timeout超时，则会返回XY_Err_Timeout错误
  * @attention   由于RTC精度不高，如果客户需要更精准的当前时间，可以每隔一段时间调用该接口，通过NTP服务器更新当前世界时间。
  */
zos_err_t zos_rtc_get_ntp_time(const char *ser_name, int timeout, struct nb_time_t *wall_clock);

/**
  * @brief   获取NTP网络时间，并更新到本地，以同步本地的世界时间
  * @param   ser_name, ntp服务器地址,NULL表示默认选用ntp1.aliyun.com
  * @param   timeout, 单位秒，填0时，平台内部使用默认值20秒；因NB速率低，建议该值不得小于20秒
  *	@param   zone_sec ,time zone，必须设置当地的时区时间，例如在中国必须设置8*60*60
  * @return  BOOL,see  @ref  xy_ret_Status_t.
  * @warning 使用该接口必须确保之前已经PDP激活成功过，否则返回XY_ERR；若timeout超时，则会返回XY_Err_Timeout错误
  * @note    该接口与xy_get_ntp_time的差异仅在于时区的设置，如果不关心时区问题，不建议使用该接口
  */
zos_err_t zos_rtc_set_by_NTP(const char *ser_name, int timeout, int zone_sec);

/**
  * @brief   获取本地的当前世界时间
  * @param   rtctime	   wall time,such as 2020/10/1 12:0:0
  * @return  BOOL,see  @ref  xy_ret_Status_t.return  XY_ERR when not attached
  * @warning    使用该接口必须确保之前已经PDP激活成功过，即已经获取过最新的世界时间；否则返回XY_ERR
  * @attention  该接口未考虑时区问题，获取的就是当地的年月日事件，例如北京时间。该接口不能用于带时区功能的转换
  */
zos_err_t zos_rtc_get_time(struct nb_time_t *rtctime);

/**
  * @brief   获取某时区偏移的本地当前世界时间
  * @param   rtctime	   wall time,such as 2020/10/1 12:0:0
  * @param   offset_sec	   get offset wall time,such as after 8 hours,maybe negative value,such as -100S
  * @return  BOOL,see  @ref  xy_ret_Status_t.return  XY_ERR when not attached
  * @warning    使用该接口必须确保之前已经PDP激活成功过，即已经获取过最新的世界时间；否则返回XY_ERR
  * @note    offset_sec时间偏移可以灵活使用，如设为0时，该接口则与xy_rtc_get_time功能一致，即获取当前的世界时间；\n
  *  再如offset_sec设为(0-8*60*60)，即向前8小时的世界时间，则对应当前北京时间的世界时间。
  */
zos_err_t zos_rtc_get_UT_offset(struct nb_time_t *wall_clock, int offset_sec);

/**
  * @brief   获取RTC当前的秒数值，通常用于与某个时刻点进行比较，以决定是否超时
  * @param   need_UT_time  是否需要当前世界时间的秒数
  * @return  int  seconds,0表示当前未attach成功过
  * @note    建议用户入参填0. \n
  *** 如果用户想与世界时间绑定，则必须确保之前已经attach成功，否则会返回值0
  * @attention  用户使用该接口有两种场景，一种是网络通信相关的，此时入参需要填1，且需保证已经attach成功过； \n
  ***  另一种是本地定时行为，不关心是否已联网，例如单核MCU工作场景。 \n
  ***  用户在使用时，必须清楚使用该接口的目的，以填写正确的入参，否则会造成attach成功前后值的突变。
  */
zos_uint32_t zos_rtc_get_sec(int need_UT_time);

/**
  * @brief   用于系统从上电开始已经过了多少秒了，常用于用户的运行时间异常监控。
  * @return  seconds
  * @note    if products care  power consumption,this value always Less than 5*60 seconds
  */
zos_uint32_t zos_rtc_get_working_time();

/**
  * @brief   用于查询上次正常深睡到现在过了多少秒，以供用户进行异常监控
  * @return  seconds  if 0,indicate not into DEEPSLEEP before
  * @note    如果上次非正常深睡，返回值为0.
  */
zos_uint32_t zosc_rtc_get_last_work_offset();

/**
  * @brief   设置某ID超时RTC事件
  * @param   timer_id	   ID
  * @param   sec_offset	   next timeout offset by seconds
  * @param   callback	   timeout callback
  * @param   data	       timeout callback  input param
  * @return  BOOL,see  @ref  xy_ret_Status_t.unused
  * @note    对于周期性的定时功能，必须设置随机的超时时刻点，以减轻对运营商基站的运营压力，建议使用xy_rtc_set_by_day或xy_rtc_set_by_week接口
  * @warning if period <1 hour,must connect XINYIN AE!!!
  * @attention  超时回调函数不得使用阻塞机制，且代码不能运行太长事件，以免影响其他RTC的运行时序
  */
zos_err_t zos_rtc_timer_create(char timer_id, int sec_offset, zos_rtc_timeout_cb_t callback, void *data);

/**
  * @brief   设置每天某个时间段的随机RTC事件。例如15:00--18:00区间内,则set_rtc_by_day_offset(RTC_TIMER_USER1,user_rtc_timeout_cb,NULL,15,3*60*60,0,0)
  * @param   timer_id, >=RTC_TIMER_USER_BASE and  <RTC_TIMER_MAX
  * @param   hour_start, start  hour
  * @param   sec_span,  span seconds,always >0. used for rand RTC to reduce BSS stress,and improve communication success rate
  * @param   min,
  * @param   sec,
  * @return  BOOL,see  @ref  xy_ret_Status_t.
  * @note    用于设置每天某时间段的周期性定时行为，为了减轻基站的通信压力，每个机器具体的时刻点具有随机性。
  * @warning    使用该接口必须确保之前已经PDP激活成功过，即已经获取过最新的世界时间；否则返回XY_ERR
  * @attention  超时回调函数不得使用阻塞机制，且代码不能运行太长事件，以免影响其他RTC的运行时序
  */
zos_err_t zos_rtc_set_by_day(char timer_id, zos_rtc_timeout_cb_t callback, void *data, int hour_start, int sec_span, int min, int sec);

/**
  * @brief   设置每周某个时间段的随机RTC事件。例如设置星期天15:00--20:00区间内的RTC，则调用set_rtc_by_week(RTC_TIMER_USER1,user_rtc_timeout_cb,NULL,7,15,5*60*60,0,0)
  * @param   timer_id, >=RTC_TIMER_USER_BASE and  <RTC_TIMER_MAX
  * @param   day_week,  is 1-7
  * @param   hour_start, start  hour
  * @param   sec_span,  span seconds,always >0. used for rand RTC to reduce BSS stress,and improve communication success rate
  * @param   min,
  * @param   sec,
  * @return  BOOL,see  @ref  xy_ret_Status_t.
  * @note    用于设置每周某时间段的周期性定时行为，为了减轻基站的通信压力，每个机器具体的时刻点具有随机性。
  * @warning    使用该接口必须确保之前已经PDP激活成功过，即已经获取过最新的世界时间；否则返回XY_ERR
  * @attention  超时回调函数不得使用阻塞机制，且代码不能运行太长事件，以免影响其他RTC的运行时序
  */
zos_err_t zos_rtc_set_by_week(char timer_id, zos_rtc_timeout_cb_t callback, void *data, int day_week, int hour_start, int sec_span, int min, int sec);

/**
  * @brief   Used to delete one RTC timer
  * @param   timer_id	   ID
  * @return  BOOL,see  @ref  xy_ret_Status_t.  unused
  * @note    if period <1 hour,must connect XINYIN AE!!!
  */
zos_err_t zos_rtc_timer_delete(char timer_id);

/**
  * @brief   获取某用户定时器ID的下一次超时的秒数偏移。如果返回0，则表示当前ID未设置定时事件
  * @return  seconds
  * @note    该接口常用于异常重启等保护使用，即重启后查看是否有效，若无效，则重设该定时器。 \n
  *          客户在设置RTC事件时，必须充分考虑各种异常造成的事件未设置成功问题。
  */
zos_uint32_t zos_rtc_next_offset_by_ID(char timer_id);

/**
  * @brief   get year/mon/day/hour/min/sec by msec,and msec is 1970/1/1 offset
  * @note
  */
void zos_rtc_get_time_sec(const zos_uint64_t msec, struct nb_time_t *result);

/**
  * @brief  get msec from  1970/1/1
  * @note   返回值为毫秒
  */
zos_uint64_t zos_rtc_get_time_msec(struct nb_time_t *tp);


#endif
