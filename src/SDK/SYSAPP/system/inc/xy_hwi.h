/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __XY_HWI_H_
#define __XY_HWI_H_


#ifdef __cplusplus
 extern "C" {
#endif

void _Error_Handler(char *, int);
#define Error_Handler() _Error_Handler(__XYFILE__, __LINE__)

void PLL_INT_Handler(void);
void at_uart_int_handler(void);
void rtc_int_handler(void);
void WDT_INT_Handler(void);

#ifdef __cplusplus
}
 
#endif

#endif

