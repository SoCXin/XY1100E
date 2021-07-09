/* Define to prevent recursive inclusion -------------------------------------*/
#pragma once

#include "xy_utils.h"

#ifdef __cplusplus
extern "C"
{
#endif


#define VBAT_DET_RETRY_NUM 4
#define Error_Handler() _Error_Handler(__XYFILE__, __LINE__)

extern osMutexId_t g_dynamic_dsp_load_m;
extern osSemaphoreId_t g_at_uart_sem;

void _Error_Handler(char *, int);
void PeripheralIRQ_init(void);
unsigned int system_init();
int sys_app_init_after_OFFTIME();
int sys_app_init_before_OFFTIME();
unsigned char get_vbat_detect(void);
void delay_by_utc_tick(unsigned char ticks);
uint32_t CheckFactoryNvValid(void);
void utc_init();
void network_task_init();


#ifdef __cplusplus
}

#endif
