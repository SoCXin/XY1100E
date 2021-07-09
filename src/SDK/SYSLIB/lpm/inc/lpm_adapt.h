#ifndef __LPM_ADAPT_H__
#define __LPM_ADAPT_H__

#include "low_power.h"
#include "rtc_tmr.h"

#define LPM_LITEOS 0
#define LPM_FREERTOS 1


void lpm_EnterCritical(void);
void lpm_ExitCritical(void);

int lpm_rtc_event_add_by_global_critical(char timer_id, uint64_t rtc_msec, rtc_timeout_cb_t callback, void *data);
int lpm_rtc_event_add_by_offset_critical(char timer_id, uint64_t msec_offset, rtc_timeout_cb_t callback, void *data);
int lpm_rtc_event_delete_critical(char timer_id);

int lpm_rtc_event_add_by_offset_singlecore_wfi(char timer_id, uint64_t msec_offset, rtc_timeout_cb_t callback, void *data);

int lpm_rtc_event_delete_singlecore_wfi(char timer_id);

uint32_t lpm_GetExpectedIdleTick(uint8_t sleep_mode);
void     lpm_osTickHandlerLoop(uint32_t uwElapseTicks);
void     lpm_ScheduleAfterSleep(void);


#endif	/* __LPM_ADAPT_H__ */
