#ifndef _GOLD_CARD_H
#define _GOLD_CARD_H

#include  "xy_api.h"

typedef void (*demo_func)(void);

typedef struct _demo_proc
{
    uint8_t demo_num;
    demo_func proc;
}demo_proc;

extern demo_proc g_demo_proc_list[];
extern uint8_t g_demo_proc_list_len;

#endif
