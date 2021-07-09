#pragma once

#include  "xy_utils.h"
typedef struct
{
    char  host[40];
    int   data_len;
	int   ping_num;
    int   time_out;
    int   interval_time;
	int   ip_type;
	int   rai_val;
} ping_arguments_t;

typedef struct
{
	//k_delayed_work ping_info_timer;
	//bool timer_out;
	int ping_send_num;
	int ping_reply_num;
	int longest_rtt;
    int shortest_rtt;
	int time_average;
} ping_info_t;

extern int g_ping_stop;
extern osThreadId_t  at_ping_thread_id;

void process_ping(ping_arguments_t *ping_arguments);

int stop_ping();

int start_ping(int ip_type, char *host_ip, int data_len, int ping_num, int time_out, int interval_time, int rai_val);

int send_packet_to_ping(char *data, int len);

