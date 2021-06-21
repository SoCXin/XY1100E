/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __XINYI_CONF_H
#define __XINYI_CONF_H

/* Includes ------------------------------------------------------------------*/
/* Uncomment/Comment the line below to enable/disable peripheral header file inclusion */
//#include "xinyi_prcm.h"
//#include "xinyi_gpio.h"
//#include "xinyi_qspi.h"
#include "xinyi_nvic.h"
//#include "xinyi_uart.h"
//#include "xinyi_tim.h"
//#include "xinyi_spi.h"
//#include "xinyi_csp.h"


/* Exported types ------------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/
/* Uncomment the line below to expanse the "assert_param" macro in the
   Standard Peripheral Library drivers code */
/* #define USE_FULL_ASSERT    1 */

/* Exported macro ------------------------------------------------------------*/
#ifdef  USE_FULL_ASSERT

/**
  * @brief  The assert_param macro is used for function's parameters check.
  * @param  expr: If expr is false, it calls assert_failed function which reports
  *         the name of the source file and the source line number of the call
  *         that failed. If expr is true, it returns no value.
  * @retval None
  */
  #define assert_param(expr) ((expr) ? (void)0 : assert_failed((uint8_t *)__FILE__, __LINE__))
/* Exported functions ------------------------------------------------------- */
  void assert_failed(uint8_t* file, uint32_t line);
#else
  #define assert_param(expr) ((void)0)
  //#define assert_param(expr) while(!(expr))
#endif /* USE_FULL_ASSERT */

#endif /* __XINYI_CONF_H */

/******************* (C) COPYRIGHT 2020 xinyi *****END OF FILE****/
