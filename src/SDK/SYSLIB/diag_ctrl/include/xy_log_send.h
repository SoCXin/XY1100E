#ifndef _XY_LOG_SEND_H
#define _XY_LOG_SEND_H
#include "basetype.h"
int xy_log_trace_msg_send(char *data, unsigned int buffer_len);

int xy_log_task();

void xy_log_trace(char *data, int item_len);

void xy_log_entry(void);

void xy_log_test(void);

#endif

