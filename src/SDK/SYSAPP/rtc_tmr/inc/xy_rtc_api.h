/** 
* @file        xy_rtc_api.h
* @brief       供用户设置RTC定时器事件，同时还提供了接口供用户设置和获取当前世界时间. \n
*	RTC定时器目前只有一个硬件资源，平台通过软件方式实现了多个软RTC ID，目前能给用户使用的为RTC_TIMER_USER1和RTC_TIMER_USER2。\n
*   RTC定时器在深睡时不掉电，超时后能够唤醒芯片执行RTC中断。
* @attention   RTC定时器事件，只支持相对时间的设置方式，如半小时唤醒一次；不支持年月日的设置方式,请根据当前世界时间自行转换为秒偏移。 \n
*    对于通过AT+NRB或xy_reboot进行的正常软重启，平台仍然保持了RTC定时器和当前世界时间有效. \n
*    对于PIN_RESET、assert、watchdog等异常重启，RTC会被重新复位，用户任务每次上电初始化时，需要识别当前用户RTC是否存在，若不存在，必须重设RTC定时器。
* @warning	 平台维护了与基站同步的世界时间，前提是NB终端attach小区成功，若未attach成功，则本地的世界时间是从2018/10/1开始的。 \n
*   如果用户需要正确的世界时间，需要确保attach成功后，再使用RTC接口功能。 \n
*   RTC采用的是32K晶振，精度低，连续运行若干天后，会造成偏差变大，若用户需要较精确的世界时间，可以自行在合适的时机重新出发小区attach。
*/

#pragma once
#include <stdint.h>

#include "rtc_tmr.h"

/**
* @brief 用户可用的RTC ID，目前仅支持4个，用户不得增减ID，否则会造成平台内存越界
* @param 
*/
typedef enum
{
  RTC_TIMER_USER1 = RTC_TIMER_AP_USER1, 
  RTC_TIMER_USER2 = RTC_TIMER_AP_USER2, 
  RTC_TIMER_USER3 = RTC_TIMER_AP_USER3,
  RTC_TIMER_USER4 = RTC_TIMER_AP_USER4,
  //RTC_TIMER_USER_END = RTC_TIMER_AP_USER_END,
} USER_RTC_TIMER_ID;

/**
* @brief  wall time struct
*/
struct xy_wall_clock
{
  uint32_t tm_sec;  /* Seconds (0-59) */
  uint32_t tm_min;  /* Minutes (0-59) */
  uint32_t tm_hour; /* Hours (0-23) */
  uint32_t tm_mday; /* Day of the month (1-31) */
  uint32_t tm_mon;  /* Month (0-11) */
  uint32_t tm_year; /* Years */
  uint32_t tm_wday; /* Day of the week (0-6),always ignore */
  uint32_t tm_yday; /* Day of the year (0-365),always ignore */
};

/**
  * @brief   用户手工设置当前世界时间，不建议用户使用！
  * @param   rtctime	   universal time,such as 2020/10/1 12:0:0
  * @param   zone_sec	   zone sec,such as beijing is 8*60*60,always is 0
  * @return  BOOL,see  @ref  xy_ret_Status_t.unused!
  * @note    供用户手工设置当前的世界时间，不建议使用。\n
  *  目前芯翼平台支持两种世界时间的更新途径，一种是3GPP在attach时后台更新当前世界时间；\n
  *  另一种是通过xy_get_ntp_time获取当前网络世界时间。
  */
int xy_rtc_set_time(struct xy_wall_clock *rtctime, int zone_sec);

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
int xy_get_ntp_time(const char *ser_name, int timeout, struct xy_wall_clock *wall_clock);

/**
  * @brief   获取NTP网络时间，并更新到本地，以同步本地的世界时间
  * @param   ser_name, ntp服务器地址,NULL表示默认选用ntp1.aliyun.com
  * @param   timeout, 单位秒，填0时，平台内部使用默认值20秒；因NB速率低，建议该值不得小于20秒
  *	@param   zone_sec ,time zone，必须设置当地的时区时间，例如在中国必须设置8*60*60
  * @return  BOOL,see  @ref  xy_ret_Status_t.
  * @warning 使用该接口必须确保之前已经PDP激活成功过，否则返回XY_ERR；若timeout超时，则会返回XY_Err_Timeout错误
  * @note    该接口与xy_get_ntp_time的差异仅在于时区的设置，如果不关心时区问题，不建议使用该接口
  */
int xy_set_RTC_by_NTP(const char *ser_name, int timeout, int zone_sec);

/**
  * @brief   获取本地的当前世界时间
  * @param   rtctime	   wall time,such as 2020/10/1 12:0:0
  * @return  BOOL,see  @ref  xy_ret_Status_t.return  XY_ERR when not attached
  * @warning    使用该接口必须确保之前已经PDP激活成功过，即已经获取过最新的世界时间；否则返回XY_ERR
  * @attention  该接口未考虑时区问题，获取的就是当地的年月日事件，例如北京时间。该接口不能用于带时区功能的转换
  */
int xy_rtc_get_time(struct xy_wall_clock *rtctime);

/**
  * @brief   获取某时区偏移的本地当前世界时间
  * @param   rtctime	   wall time,such as 2020/10/1 12:0:0
  * @param   offset_sec	   get offset wall time,such as after 8 hours,maybe negative value,such as -100S
  * @return  BOOL,see  @ref  xy_ret_Status_t.return  XY_ERR when not attached
  * @warning    使用该接口必须确保之前已经PDP激活成功过，即已经获取过最新的世界时间；否则返回XY_ERR
  * @note    offset_sec时间偏移可以灵活使用，如设为0时，该接口则与xy_rtc_get_time功能一致，即获取当前的世界时间；\n
  *  再如offset_sec设为(0-8*60*60)，即向前8小时的世界时间，则对应当前北京时间的世界时间。
  */
int xy_rtc_get_UT_offset(struct xy_wall_clock *wall_clock, int offset_sec);

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
uint32_t xy_rtc_get_sec(int need_UT_time);

/**
  * @brief   用于系统从上电开始已经过了多少秒了，常用于用户的运行时间异常监控。
  * @return  seconds
  * @note    if products care  power consumption,this value always Less than 5*60 seconds 
  */
uint32_t xy_get_working_time();

/**
  * @brief   用于查询上次正常深睡到现在过了多少秒，以供用户进行异常监控
  * @return  seconds  if 0,indicate not into DEEPSLEEP before
  * @note    如果上次非正常深睡，返回值为0.
  */
uint32_t xy_get_last_work_offset();

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
int xy_rtc_timer_create(char timer_id, int sec_offset, rtc_timeout_cb_t callback, void *data);

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
int xy_rtc_set_by_day(char timer_id, rtc_timeout_cb_t callback, void *data, int hour_start, int sec_span, int min, int sec);

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
int xy_rtc_set_by_week(char timer_id, rtc_timeout_cb_t callback, void *data, int day_week, int hour_start, int sec_span, int min, int sec);

/**
  * @brief   Used to delete one RTC timer
  * @param   timer_id	   ID
  * @return  BOOL,see  @ref  xy_ret_Status_t.  unused
  * @note    if period <1 hour,must connect XINYIN AE!!!
  */
int xy_rtc_timer_delete(char timer_id);

/**
  * @brief   获取某用户定时器ID的下一次超时的秒数偏移。如果返回0，则表示当前ID未设置定时事件
  * @return  seconds
  * @note    该接口常用于异常重启等保护使用，即重启后查看是否有效，若无效，则重设该定时器。 \n
  *          客户在设置RTC事件时，必须充分考虑各种异常造成的事件未设置成功问题。
  */
uint32_t xy_rtc_next_offset_by_ID(char timer_id);

/**
  * @brief   get year/mon/day/hour/min/sec by msec,and msec is 1970/1/1 offset
  * @note   
  */
void xy_gmtime_r(const uint64_t msec, struct rtc_time *result);

/**
  * @brief  get msec from  1970/1/1 
  * @note   返回值为毫秒
  */
uint64_t xy_mktime(struct rtc_time *tp);