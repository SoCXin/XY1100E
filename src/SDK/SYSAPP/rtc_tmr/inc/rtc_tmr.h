
#pragma once

/*******************************************************************************
 *							 Include header files							   *
 ******************************************************************************/
#include "xy_utils.h"
#include "sema.h"

/*******************************************************************************
 *                             Type definitions                                *
 ******************************************************************************/
#define SEMA_MASTER_SELECT 	SEMA_MASTER_ARM

#define RTC_ALARM_MAX 		0xFFFFFFFFFFFFFFFF

#define RTC_TMR_THREAD_NAME "rtc_tmr"

/**
* @brief M3核RTC个数，目前只能为9个，为了保证易变NV和flash不越界，除于水外其他人不得修改总的个数，不得增减ID
*/
typedef enum {
	RTC_TIMER_AP_BASE,
	RTC_TIMER_LPM = RTC_TIMER_AP_BASE,
	RTC_TIMER_CMCCDM,    //CMCC  DM keep alive
	RTC_TIMER_UNUSED,    //unused
	RTC_TIMER_ONENET,    //onenet  update RTC
	RTC_TIMER_CDP,       //CDP  update RTC

	RTC_TIMER_AP_USER_BASE,
	RTC_TIMER_AP_USER1 = RTC_TIMER_AP_USER_BASE,
	RTC_TIMER_AP_USER2,
	RTC_TIMER_AP_USER3,
	RTC_TIMER_AP_USER4,
	
	RTC_TIMER_AP_END = RTC_TIMER_AP_USER4,
	
	RTC_TIMER_AP_MAX ,//9
}RTC_TIMER_AP_ID;
	
typedef enum {

	RTC_TIMER_CP_BASE,
		
	RTC_TIMER_CP_USER_BASE = RTC_TIMER_CP_BASE,
	//RTC_TIMER_CP_USER1 = RTC_TIMER_CP_USER_BASE,
	//RTC_TIMER_CP_USER2,
	RTC_TIMER_CP_USER_END = RTC_TIMER_CP_USER_BASE,
	
	RTC_TIMER_CP_END = RTC_TIMER_CP_USER_END,
	
	RTC_TIMER_CP_MAX = 0 ,//0
}RTC_TIMER_CP_ID;

/**
* @brief  universal time
* @param 
*/
struct rtc_time {
	
	uint32_t tm_msec;   /* always ignore */
	uint32_t tm_sec;	/* Seconds (0-61, allows for leap seconds) */
	uint32_t tm_min;	/* Minutes (0-59) */
	uint32_t tm_hour;	/* Hours (0-23) */
	uint32_t tm_mday;	/* Day of the month (1-31) */
	uint32_t tm_mon;	/* Month (0-11) */
	uint32_t tm_year;	/* Years */
	uint32_t tm_wday;	/* Day of the week (1-7)*/
	uint32_t tm_yday;	/* Day of the year (0-365),always ignore */
	uint32_t tm_isdst;	/* Non-0 if daylight savings time is in effect,always ignore */
};

/**
* @brief RTC timeout callback declaration
* @param  para in para  
*/
typedef void (*rtc_timeout_cb_t)(void *para);

/**
* @brief task info struct using RTC 
* @param task_name  
* @param rtc_sec  next universal time by sec
* @param event_type  RTC ID,see  RTC_TIMER_ID
* @param callback  
* @param data  in param
sizeof(struct rtc_event_info ) = 24
*/
struct rtc_event_info 
{	
	uint64_t rtc_msec;
	char  timer_id;
	rtc_timeout_cb_t callback;
	void * data;
	
};

/*******************************************************************************
 *                       Global variable declarations                          *
 ******************************************************************************/
extern osSemaphoreId_t g_rtc_sem;
extern osMutexId_t g_rtc_mutex;
extern int g_RTC_wakeup_type;
extern char g_need_offtime;
extern struct rtc_event_info *ap_rtc_event_arry;
extern struct rtc_event_info *cp_rtc_event_arry;
/*******************************************************************************
 *                       Global function declarations                          *
 ******************************************************************************/

void rtc_mutex_acquire(void);

void rtc_mutex_release(void);
int is_rtc_event_expired(struct rtc_event_info * event);

uint64_t xy_mktime(struct rtc_time *tp);
void rtc_event_refresh();
int rtc_timer_reset(uint64_t ullms);

int rtc_event_add_by_offset(char timer_id, uint64_t msec_offset, rtc_timeout_cb_t callback, void *data);

int rtc_event_add_by_offset_accurate(char timer_id, uint64_t msec_offset, rtc_timeout_cb_t callback, void *data);

int rtc_event_add_by_global(char timer_id, uint64_t rtc_msec, rtc_timeout_cb_t callback, void *data);


int rtc_event_delete(char timer_id);

int rtc_event_delete_nomutex(char timer_id);

int rtc_timer_adjust(uint64_t msencond);

uint64_t rtc_get_global_byoffset(uint64_t msec_offset);



struct rtc_event_info *rtc_get_next_event();
struct rtc_event_info* rtc_get_next_event_nomutex();




void rtc_task_init();

uint32_t rtc_get_next_offset_by_ID(char timer_id);

int reset_universal_timer(struct rtc_time *rtctime, int zone_sec);

//if never attached,now_wall_time is local time,not UT time,maybe is 2018/10/1
int get_universal_timer(struct rtc_time *rtctime, int zone_sec);

uint32_t get_system_sec();

uint64_t Get_CP_ALARM_RAM();

void reset_user_alarm_deepsleep();

void Set_CP_ALARM_RAM(uint64_t rtcmsc);

void restore_RTC_list(void);

void Set_AP_ALARM_RAM(uint64_t rtcmsc);

