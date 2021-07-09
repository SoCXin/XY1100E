#include "inter_core_msg.h"
#include "xy_utils.h"
#include "xy_system.h"
#include "hw_types.h"
#include "diag_out.h"
#include "xy_list.h"
#if BLE_ENABLE
#include "ble_api.h"
#include "ble_func.h"
#endif

extern osThreadId_t g_xylog_TskHandle;
unsigned int g_log_size = 0;
//when prepare into deepsleep or standby,not allow to print log
unsigned int log_totalsize_overlimit = 0;
unsigned int log_queue_overlimit = 0;
extern unsigned int user_log_limit;

//when prepare into deepsleep or standby,not allow to print log
int xy_log_print(unsigned char src_id, unsigned char log_level,const char *format,...)
{
    va_list arg;
    int done=0;
	char *log_message = NULL;
	char *log_message_auc = NULL;
	AP_LOG_T *ap_log;
	int ret;
	
	//内核没有调度,open log没打开或者log线程没创建，直接返回
	if (osKernelGetState() == osKernelInactive || g_softap_fac_nv->open_log == 0 || g_xylog_TskHandle == NULL)
		return 1;

	//中断和临界区中不得调用打印接口！！！
	if (osCoreGetState() == osCoreInInterrupt || osCoreGetState() == osCoreInCritical)
		xy_assert(0);

#if BLE_ENABLE
    if(g_ble_stat.chip_work_type == XY_NB_MODE && g_softap_fac_nv->open_log == 1)
#else
    if(g_softap_fac_nv->open_log==1 && !DSP_IS_NOT_LOADED())
#endif
    {
		log_message = (char*)xy_zalloc(AP_LOG_MAX_SIZE);

		ap_log = (AP_LOG_T*)log_message;
		va_start (arg, format);
		done = vsnprintf (log_message + sizeof(AP_LOG_T), AP_LOG_MAX_SIZE - sizeof(AP_LOG_T) - 1,format, arg);
		va_end (arg);
		ap_log->src_id = src_id;
		ap_log->log_level = log_level;
		ap_log->ap_total_len = sizeof(AP_LOG_T) + strlen((char *)log_message + sizeof(AP_LOG_T)) + 1;
		ap_log->time = osKernelGetTickCount();


		log_message_auc = (char*)xy_zalloc_Align(ap_log->ap_total_len);
		memcpy(log_message_auc, log_message, ap_log->ap_total_len);
		xy_free(log_message);

		ret = diag_msg_out(log_message_auc, sizeof(void *), 1);
		if(ret == LOG_FALSE)
		{
			xy_free(log_message_auc);
			log_queue_overlimit++;
			return done;
		}
    }
    else
    {
    	va_start (arg, format);
    	diag_static_logM3(src_id, log_level, osKernelGetTickCount(), format, arg);
    	va_end (arg);
    }

    return done;
}

int xy_print_stream(uint8_t * buf, uint32_t len)
{
	uint32_t str_len = 0;
	char * pstream = NULL;
	uint32_t i = 0;

	str_len = (len * 2) > (AP_LOG_MAX_SIZE - 1 - sizeof(AP_LOG_T)) ? (AP_LOG_MAX_SIZE - 1 - sizeof(AP_LOG_T)) : (len * 2);
	str_len &= ~((uint32_t)1); /* must be even */
	pstream = (char*)xy_malloc(str_len + 1);

	for(i = 0; i < len; i ++)
	{
		sprintf((pstream + i*2), "%02X", *(buf + i));
	}

	*(pstream + str_len) = '\0';

	xy_log_print(USER_LOG, WARN_LOG, "%s", pstream);

	xy_free(pstream);

	return (str_len/2);
}

