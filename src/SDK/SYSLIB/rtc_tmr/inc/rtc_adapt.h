#ifndef __RTC_ADAPT__
#define __RTC_ADAPT__

/*******************************************************************************
 *                             Type definitions                                *
 ******************************************************************************/

#include "rtc_tmr.h"

/**
 * @brief rtc timer int mode
 *
 * @param :  Timer interrupt mode
**/
enum rtc_irq_freq {
	RTC_SEC_INT		= 0x0,	/* SEC INT */
	RTC_MIN_INT		= 0x1,	/* MIN INT */
	RTC_HOUR_INT	= 0x2,	/* HOUR INT*/
	RTC_DAY_INT		= 0x3,	/* DAY INT */
};

/**
 * @brief  stamp polar register
 *
 * @param :  stamp event polarity
 **/
enum rtc_enable {
	RTC_DISABLE	= 0x0,
	RTC_ENABLE	= 0x1,
};

typedef unsigned long long time_t64; /* Holds time in seconds */

/*******************************************************************************
 *                             Macro definitions                               *
 ******************************************************************************/
#define SEC_PER_MIN  ((time_t64)60)
#define SEC_PER_HOUR ((time_t64)60 * SEC_PER_MIN)
#define SEC_PER_DAY  ((time_t64)24 * SEC_PER_HOUR)
#define RTC_DIVIDER RTC_CLK_DIV_1

/** Number of RTC ticks in a second */
#define RTC_ALARM_SECOND (XY_UTC_CLK / (1UL << RTC_DIVIDER))

/** Number of RTC ticks in a minute */
#define RTC_ALARM_MINUTE (RTC_ALARM_SECOND * 60)

/** Number of RTC ticks in an hour */
#define RTC_ALARM_HOUR (RTC_ALARM_MINUTE * 60)

/** Number of RTC ticks in a day */
#define RTC_ALARM_DAY (RTC_ALARM_HOUR * 24)

extern int rtc_alarm_disable();

extern int rtc_alarm_set(struct rtc_time *rtctime);

extern int rtc_timer_read(struct rtc_time *rtctime);

extern int rtc_timer_run();

extern int rtc_timer_set(struct rtc_time *rtctime);

extern int rtc_alarm_set_accurate(struct rtc_time *rtctime);

extern int rtc_timer_stop();

#endif
