#pragma once

#include "osal.h"

typedef struct
{
	char protocol_type; // 0:udp 1:tcp
    char remote_ip[40];
    int remote_port;
    unsigned int duration; // second
    unsigned int packet_size; // byte
    unsigned int rate; // bps
    unsigned int print_interval;
	int  rai_val;
} xyperf_arguments_t;


struct xyperf_udp_datagram {
	signed long id;
	unsigned long tv_sec;
	unsigned long tv_usec;
};

extern osThreadId_t g_xyperf_TskHandle;



