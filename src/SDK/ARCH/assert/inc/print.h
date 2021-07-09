#ifndef PIRNT_H
#define PRINT_H

#ifdef __cplusplus
 extern "C" {
#endif

#include <stdint.h>
#include "hardware.h"

/*!< Global Declaration */
// enable assert print or not, default is enable, and value is 0, set value not 0 to disable
extern volatile char assert_print;

// macro to open or close assert print
#define ASSERT_PRINT_OPEN()       do{assert_print = 0;}while(0)
#define ASSERT_PRINT_CLOSE()      do{assert_print = 1;}while(0)
#define IS_ASSERT_PRINT_OPEN()    (assert_print == 0)


// Transplant layer to print single character
// Achieve a single character output, you can complete the transplant

#ifdef __SOC_XINYI_1100__

#include "csp.h"
#if BLE_ENABLE
#define PRINT_CHAR(ch)                                                           \
{                                                                                \
	if(IS_ASSERT_PRINT_OPEN())                                                   \
	{                                                                            \
		CSPCharPut(CSP2_BASE, (uint8_t)(ch));      /* write one character */     \
		CSPCharPut(CSP4_BASE, (uint8_t)(ch));      /* write one character */     \
	}                                                                            \
}
#else
#define PRINT_CHAR(ch)                                                           \
{                                                                                \
	if(IS_ASSERT_PRINT_OPEN())                                                   \
	{                                                                            \
		CSPCharPut(CSP2_BASE, (uint8_t)(ch));      /* write one character */     \
	}                                                                            \
}
#endif  /* BLE_ENABLE */

#endif  /* __SOC_XINYI_1100__ */

#ifdef __SOC_XINYI_2100__
#include "uart.h"
#define PRINT_CHAR(ch)                                                           \
{                                                                                \
	if(IS_ASSERT_PRINT_OPEN())                                                   \
	{                                                                            \
		UARTCharPut(UART1_BASE, (uint8_t)(ch));      /* write one character */   \
	}                                                                            \
}
#endif


#define PRINT_BUF(buf)                              \
{                                                   \
	uint32_t buf_cnt = 0;                           \
	uint8_t *p_buf = (uint8_t *)(buf);              \
	while(p_buf[buf_cnt] != '\0')                   \
	{                                               \
		PRINT_CHAR(p_buf[buf_cnt]);                 \
		buf_cnt++;                                  \
	}                                               \
}


#define PRINT_BUF_WITH_LEN(buf, len)                \
{                                                   \
	uint8_t *p_buf = (uint8_t *)(buf);              \
	for(uint32_t i=0; i<(uint32_t)len; i++)         \
	{                                               \
		PRINT_CHAR(p_buf[i]);                       \
	}                                               \
}


#ifdef __cplusplus
}
#endif

#endif  /* end of __PRINT_H */
