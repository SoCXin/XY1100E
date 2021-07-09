/*******************************************************************************
 *							 Include header files							   *
 ******************************************************************************/
#include "xy_ps_ext_api.h"
#include "xy_ps_api.h"
#include "xy_utils.h"
#include "at_ps_cmd.h"

/*******************************************************************************
 *						  Local variable definitions						   *
 ******************************************************************************/
/**全局数组，保存用户回调函数*/
xy_ps_event_callback_t g_ps_event_cb[XY_PS_EVENT_MAX] = {NULL};

/*******************************************************************************
 *						Global function implementations 			void*		   *
 ******************************************************************************/
void xy_Reg_Ps_Event(uint32_t eventId, void* func)
{
	xy_assert(func != NULL && eventId > XY_PS_EVENT_MIN && eventId < XY_PS_EVENT_MAX);
	if (g_ps_event_cb[eventId] == NULL)
	{
		g_ps_event_cb[eventId] = (xy_ps_event_callback_t)func;
		softap_printf(USER_LOG, WARN_LOG, "eventId:%d register success", eventId);
	}
	else
	{
		softap_printf(USER_LOG, WARN_LOG, "eventId:%d is already register!", eventId);
	}
}

void xy_DeReg_Ps_Event(uint32_t eventId)
{
	xy_assert(eventId > XY_PS_EVENT_MIN && eventId < XY_PS_EVENT_MAX);
	if (g_ps_event_cb[eventId] != NULL)
	{
		g_ps_event_cb[eventId] = NULL;
		softap_printf(USER_LOG, WARN_LOG, "eventId:%d deregister", eventId);
	}

}

void notify_Ps_Event(uint32_t eventId, void *data, uint32_t data_len)
{
	xy_assert(eventId > XY_PS_EVENT_MIN && eventId < XY_PS_EVENT_MAX);
	xy_ps_event_param_t eventParam = {0};
	eventParam.eventId = eventId;
	eventParam.data = data;
	eventParam.data_len = data_len;
	if (g_ps_event_cb[eventId] != NULL)
		g_ps_event_cb[eventId](&eventParam);
}