#pragma once

#include "osal.h"

enum wireshark_msg_type
{
	WIRESHARK_UP_DATA = 0,
	WIRESHARK_DOWN_DATA,
	WIRESHARK_END,
};

struct wireshark_info
{
	unsigned short len;
	char flow_flag;
	char padding;
	unsigned int time;
	char data[0];
};

extern osMessageQueueId_t wireshark_msg_q;
extern osThreadId_t g_wireshark_TskHandle;
extern int g_wireshark_set;

void wireshark_task();
void wireshark_forward_format_print(void *data, unsigned short len, char in);
