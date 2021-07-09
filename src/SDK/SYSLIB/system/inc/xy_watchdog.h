#pragma once
#include "stdint.h"

#define WDT_DEFAULT_SEC 120

//see  get_softreset_type()
void soft_reset_by_wdt(int soft_rst_reason);

uint32_t wdt_task_init();

uint32_t wdt_task_deinit();
