#include "xy_utils.h"
#include "xy_system.h"
#include "xy_wireshark.h"
#include "inter_core_msg.h"
#include "netif_mgmt.h"
#include "ipcmsg.h"
#include "xy_nbiot_module_define.h"
#include "zero_copy_shm.h"


int g_wireshark_set = 0;
osThreadId_t g_wireshark_TskHandle = NULL;
osMessageQueueId_t wireshark_msg_q = NULL;

void wireshark_forward_format_print(void *data, unsigned short len, char in)
{
	if(g_wireshark_set == 1)
	{
		struct common_msg *msg = NULL;
		
		msg = xy_zalloc(sizeof(struct common_msg) + len);
		msg->size = len;

		if (data != NULL)
			memcpy(msg->data, data, len);
		
		if(in == 0)
			msg->msg_id = WIRESHARK_UP_DATA;
		else if(in == 1)
			msg->msg_id = WIRESHARK_DOWN_DATA;
		
		osMessageQueuePut(wireshark_msg_q, &msg, 0, osWaitForever);
	}
}

#if IS_DSP_CORE
void wireshark_packet_proc(void *data, unsigned short len, char in)
{
	diag_wireshark_dataM3((char *)data, len, in, osKernelGetTickCount());
}
#else
void wireshark_packet_proc(void *data, unsigned short len, char in)
{
	struct wireshark_info *pwireshark_info = NULL;
	int ret = 0;
	
	if(DSP_IS_NOT_LOADED())
	{
		softap_printf(USER_LOG, WARN_LOG, "dsp not runned and wireshark packet dropped!!!");
		return;
	}

	pwireshark_info = (struct wireshark_info *)xy_zalloc_Align(len + sizeof(struct wireshark_info));
	
	pwireshark_info->len = len;
	pwireshark_info->flow_flag = in;
	pwireshark_info->time = osKernelGetTickCount();

	memcpy(pwireshark_info->data, data, len);

	ret = shm_zero_copy(pwireshark_info, len + sizeof(struct wireshark_info), ICM_WIRESHARKDATA);

	if(ret == -1)
	{
		softap_printf(USER_LOG, WARN_LOG, "write data failed and wireshark packet dropped!!!");
		xy_free(pwireshark_info);
	}
}
#endif //IS_DSP_CORE

void wireshark_task()
{
	struct common_msg *msg = NULL;
	wireshark_msg_q = osMessageQueueNew(50, sizeof(void *));

	while(1) 
	{	
		osMessageQueueGet(wireshark_msg_q, (void *)(&msg), NULL, osWaitForever);
		if(msg == NULL)
		{
            xy_assert(0);
            continue;
		}
		switch(msg->msg_id)
		{
			case WIRESHARK_DOWN_DATA:
				wireshark_packet_proc(msg->data,msg->size,1);
				break;
			case WIRESHARK_UP_DATA:
				wireshark_packet_proc(msg->data,msg->size,0);
				break;
			case WIRESHARK_END:
				goto END;
			default:
				break;
		}
		xy_free(msg);
	}
END:
	xy_free(msg);
	if(wireshark_msg_q != NULL)
	{
		xy_queue_Delete(wireshark_msg_q);
		wireshark_msg_q = NULL;
	}
	g_wireshark_TskHandle = NULL;
	osThreadExit();
	return;
}

