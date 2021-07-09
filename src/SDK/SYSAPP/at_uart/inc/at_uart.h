#pragma once

#include <stdint.h>
#include "osal.h"

extern unsigned int g_autobaud_state;
extern volatile uint8_t csp_out_flag;
extern unsigned int g_autobaud_rate;
extern osSemaphoreId_t g_at_uart_sem;

void AutoBaudStart() __attribute__((section(".ramtext")));

int at_write_to_uart(char *buf,int size) __attribute__((section(".ramtext")));

uint32_t csp_write_allout_state() __attribute__((section(".ramtext")));

void at_uart_int_handler(void) __attribute__((section(".ramtext")));

void wait_for_csp_tx_allout(void) __attribute__((section(".ramtext")));

void at_uart_init();

void AT_uart_rx_task(void);

uint32_t creat_AT_uart_rx_task();

/**
 * @brief AT结束标志判断
 *
 * @return bool
 * @note 根据波特率的不同，延迟一段时间判断csp的接收FIFO是否为空，从而判断AT是否接收结束
 */
void xy_tick_delay_for_ATend(void);

int check_channel_fifo();
