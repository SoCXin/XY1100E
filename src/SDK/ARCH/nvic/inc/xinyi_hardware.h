#ifndef __XINYI_HARDWARE_H_
#define __XINYI_HARDWARE_H_

#ifdef __cplusplus
 extern "C" {
#endif


#if( ( defined __SOC_XINYI_1100__ ) && ( defined(__SOC_XINYI_2100__ ) ) )
	#error "cannot defined __SOC_XINYI_1100__ and __SOC_XINYI_2100__ concurrent"
#endif

#if( ( !defined __SOC_XINYI_1100__ ) && ( !defined(__SOC_XINYI_2100__ ) ) )
	#error "must define one of __SOC_XINYI_1100__ and __SOC_XINYI_2100__"
#endif


#ifdef __SOC_XINYI_2100__

#if( ( defined __XINYI_2100_CORE_AP__ ) && ( defined(__XINYI_2100_CORE_CP__ ) ) )
	#error "cannot defined __XINYI_2100_CORE_AP__ and __XINYI_2100_CORE_CP__ concurrent"
#endif

#if( ( !defined __XINYI_2100_CORE_AP__ ) && ( !defined(__XINYI_2100_CORE_CP__ ) ) )
	#error "must define one of __XINYI_2100_CORE_AP__ and __XINYI_2100_CORE_CP__"
#endif

#endif


/**
 * @brief Configuration of the Cortex-M3 Processor and Core Peripherals
 */
#define __MPU_PRESENT             1 /*!< xinyi devices provide an MPU */
#define __NVIC_PRIO_BITS          3 /*!< xinyi uses 3 Bits for the Priority Levels    */
#define __Vendor_SysTickConfig    0 /*!< Set to 1 if different SysTick Config is used */

/**
 * @brief xinyi Interrupt Number Definition, according to the selected device
 *        in @ref Library_configuration_section
 */
typedef enum IRQn
{
/******  Cortex-M3 Processor Exceptions Numbers ***************************************************/
  NonMaskableInt_IRQn         = -14,    /*!< 2  Non Maskable Interrupt                            */
  HardFault_IRQn              = -13,    /*!< 3  Cortex-M3 Hard Fault Interrupt                    */
  MemoryManagement_IRQn       = -12,    /*!< 4  Cortex-M3 Memory Management Interrupt             */
  BusFault_IRQn               = -11,    /*!< 5  Cortex-M3 Bus Fault Interrupt                     */
  UsageFault_IRQn             = -10,    /*!< 6  Cortex-M3 Usage Fault Interrupt                   */
  SVCall_IRQn                 = -5,     /*!< 11 Cortex-M3 SV Call Interrupt                       */
  DebugMonitor_IRQn           = -4,     /*!< 12 Cortex-M3 Debug Monitor Interrupt                 */
  PendSV_IRQn                 = -2,     /*!< 14 Cortex-M3 Pend SV Interrupt                       */
  SysTick_IRQn                = -1,     /*!< 15 Cortex-M3 System Tick Interrupt                   */
/******  xinyi specific Interrupt Numbers *********************************************************/
#ifdef __SOC_XINYI_1100__
  CSP1_IRQn                   = 0,      /*!< 16 xinyi CSP1 Interrupt                              */
  I2C1_IRQn                   = 1,      /*!< 17 xinyi I2C1 Interrupt                              */
  SPI1_IRQn                   = 2,      /*!< 18 xinyi SPI1 Interrupt                              */
  TIM1_IRQn                   = 3,      /*!< 19 xinyi TIM1 Interrupt                              */
  UART1_IRQn                  = 4,      /*!< 20 xinyi UART1 Interrupt                             */
  UTC_IRQn                    = 5,      /*!< 21 xinyi UTC Interrupt                               */
  WDT_IRQn                    = 6,      /*!< 22 xinyi WDT Interrupt                               */
  MEASURECNT_IRQn             = 7,      /*!< 23 xinyi MEASURECNT Interrupt                        */
  TIM2_IRQn                   = 8,      /*!< 24 xinyi TIM2 Interrupt                              */
  CSP2_IRQn                   = 9,      /*!< 25 xinyi CSP2 Interrupt                              */
  ROOTCLK_XTAL_IRQn           = 10,     /*!< 26 xinyi ROOTCLK XTAL Interrupt                      */
  ROOTCLK_PLL_IRQn            = 11,     /*!< 27 xinyi ROOTCLK pll Interrupt                       */
  QSPI_IRQn                   = 12,     /*!< 28 xinyi QSPI Interrupt                              */
  SEMAPHORE_IRQn              = 13,     /*!< 29 xinyi SEMAPHORE Interrupt                         */
  TIM3_IRQn                   = 14,     /*!< 30 xinyi TIM3 Interrupt                              */
  TIM4_IRQn                   = 15,     /*!< 31 xinyi TIM4 Interrupt                              */
  DMAC0_IRQn                  = 16,     /*!< 32 xinyi DMAC0 Interrupt                             */
  DMAC1_IRQn                  = 17,     /*!< 33 xinyi DMAC1 Interrupt                             */
  DMAC2_IRQn                  = 18,     /*!< 34 xinyi DMAC2 Interrupt                             */
  DMAC3_IRQn	    	      = 19,     /*!< 35 xinyi DMAC3 Interrupt                             */
  WAKEUP_IRQn		          = 20,     /*!< 36 xinyi WAKEUP Interrupt                            */
  GPIO_IRQn		              = 21,     /*!< 37 xinyi GPIO Interrupt                              */
  I2C2_IRQn		              = 22,     /*!< 38 xinyi I2C2 Interrupt                              */
  CSP3_IRQn		              = 23,     /*!< 39 xinyi CSP3 Interrupt                              */
  CSP4_IRQn		              = 24,     /*!< 40 xinyi CSP4 Interrupt                              */
  DFEBUF_IRQn	    	      = 25      /*!< 41 xinyi DFEBUF Interrupt                            */
#endif
/*------------------------------------------------------------------------------------------------*/

#ifdef __SOC_XINYI_2100__
  CLKTIM_IRQn                 = 0,      /*!< 16 xinyi CLK timer Interrupt                         */
  CSP1_IRQn                   = 1,      /*!< 17 xinyi CSP1 Interrupt                              */
  I2C1_IRQn                   = 2,      /*!< 18 xinyi I2C1 Interrupt                              */
  SPI1_IRQn                   = 3,      /*!< 19 xinyi SPI1 Interrupt                              */
  TIM1_IRQn                   = 4,      /*!< 20 xinyi TIM1 Interrupt                              */
  UART2_IRQn                  = 5,      /*!< 21 xinyi UART1 Interrupt                             */
  UTC_IRQn                    = 6,      /*!< 22 xinyi UTC Interrupt                               */
  LPUART_IRQn                 = 7,      /*!< 23 xinyi LpUART Interrupt                            */
  LPTIM_IRQn                  = 8,      /*!< 24 xinyi LpTimer Interrupt                           */
  MCNT_IRQn                   = 9,      /*!< 25 xinyi MEASURECNT Interrupt                        */
  TIM2_IRQn                   = 10,     /*!< 26 xinyi TIM2 Interrupt                              */
  CSP2_IRQn                   = 11,     /*!< 27 xinyi CSP2 Interrupt                              */
  LPMODE_IRQn                 = 12,     /*!< 28 xinyi break lpmode Interrupt                      */
  HRC_MISS_IRQn               = 13,     /*!< 29 xinyi hrc miss Interrupt                          */
  ROOTCLK_XTAL_IRQn           = 14,     /*!< 30 xinyi ROOTCLK XTAL Interrupt                      */
  ROOTCLK_PLL_IRQn            = 15,     /*!< 31 xinyi ROOTCLK pll Interrupt                       */
  QSPI_IRQn                   = 16,     /*!< 32 xinyi QSPI Interrupt                              */
  SEMAPHORE_IRQn              = 17,     /*!< 33 xinyi SEMAPHORE Interrupt                         */
  TIM3_IRQn                   = 18,     /*!< 34 xinyi TIM3 Interrupt                              */
  TIM4_IRQn                   = 19,     /*!< 35 xinyi TIM4 Interrupt                              */
  DMAC0_IRQn                  = 20,     /*!< 36 xinyi DMAC0 Interrupt                             */
  DMAC1_IRQn                  = 21,     /*!< 37 xinyi DMAC1 Interrupt                             */
  DMAC2_IRQn                  = 22,     /*!< 38 xinyi DMAC2 Interrupt                             */
  DMAC3_IRQn	    	      = 23,     /*!< 39 xinyi DMAC3 Interrupt                             */
  WAKEUP_IRQn		          = 24,     /*!< 40 xinyi WAKEUP Interrupt                            */
  GPIO_IRQn		              = 25,     /*!< 41 xinyi GPIO Interrupt                              */
  I2C2_IRQn		              = 26,     /*!< 42 xinyi I2C2 Interrupt                              */
  CSP3_IRQn		              = 27,     /*!< 43 xinyi CSP3 Interrupt                              */
  CSP4_IRQn		              = 28,     /*!< 44 xinyi CSP4 Interrupt                              */
  DMAC4_IRQn                  = 29,     /*!< 45 xinyi DMAC4 Interrupt                             */
  DMAC5_IRQn                  = 30,     /*!< 46 xinyi DMAC5 Interrupt                             */
  DMAC6_IRQn                  = 31,     /*!< 47 xinyi DMAC6 Interrupt                             */
  DMAC7_IRQn	    	      = 32,     /*!< 48 xinyi DMAC7 Interrupt                             */
  WDT_IRQn                    = 33,     /*!< 49 xinyi WDT Interrupt                               */
  CACHE_IRQn                  = 34,     /*!< 50 xinyi CACHE Interrupt                             */
  DFE_IRQn	    	          = 35,     /*!< 51 xinyi DFE Interrupt                               */
  PHYTMR_IRQn	    	      = 36,     /*!< 52 xinyi PHYTMR Interrupt                            */
  ADC_IRQn	    	          = 37,     /*!< 53 xinyi ADC Interrupt                               */
  TRNG_IRQn	    	          = 38,     /*!< 54 xinyi TRNG Interrupt                              */
  SHA_IRQn	    	          = 39,     /*!< 55 xinyi SHA Interrupt                               */
  BB_IRQn	    	          = 40,     /*!< 56 xinyi BB Interrupt                                */
  CMP_IRQn	    	          = 41,     /*!< 57 xinyi CMP Interrupt                               */
  KEYSCAN_IRQn	    	      = 42,     /*!< 58 xinyi key scan Interrupt                          */
  ISO7816_IRQn	    	      = 43,     /*!< 59 xinyi iso7816 Interrupt                           */
  SSPI_IRQn	    	          = 44,     /*!< 60 xinyi SSPI Interrupt                              */
  RC32K_IRQn	    	      = 45,     /*!< 61 xinyi rc 32K calibration Interrupt                */
#endif
/*------------------------------------------------------------------------------------------------*/
} IRQn_Type;


#include "core_cm3.h"
#include <stdint.h>
#include "bitband.h"

#ifdef __CC_ARM
#pragma anon_unions
#endif

/*!< xinyi Standard Peripheral Library old types (maintained for legacy purpose) */
typedef int32_t  s32;
typedef int16_t  s16;
typedef int8_t   s8;

typedef const int32_t sc32;   /*!< Read Only */
typedef const int16_t sc16;   /*!< Read Only */
typedef const int8_t  sc8;    /*!< Read Only */

typedef __IO int32_t  vs32;
typedef __IO int16_t  vs16;
typedef __IO int8_t   vs8;

typedef __I int32_t vsc32;    /*!< Read Only */
typedef __I int16_t vsc16;    /*!< Read Only */
typedef __I int8_t  vsc8;     /*!< Read Only */

typedef uint32_t  u32;
typedef uint16_t u16;
typedef uint8_t  u8;

typedef const uint32_t uc32;  /*!< Read Only */
typedef const uint16_t uc16;  /*!< Read Only */
typedef const uint8_t  uc8;   /*!< Read Only */

typedef __IO uint32_t  vu32;
typedef __IO uint16_t vu16;
typedef __IO uint8_t  vu8;

typedef __I uint32_t vuc32;   /*!< Read Only */
typedef __I uint16_t vuc16;   /*!< Read Only */
typedef __I uint8_t  vuc8;    /*!< Read Only */

typedef enum {RESET = 0, SET = !RESET} FlagStatus, ITStatus;

typedef enum {DISABLE = 0, ENABLE = !DISABLE} FunctionalState;
#define IS_FUNCTIONAL_STATE(STATE) (((STATE) == DISABLE) || ((STATE) == ENABLE))

//typedef enum {ERROR = 0, SUCCESS = !ERROR} ErrorStatus;


 /**
  * @}
  It must be included here to guarantee that these typedef have been defined.
  */
#include "xinyi_conf.h"


#ifdef __cplusplus
 }
#endif

#endif	/* end of __XINYI_HARDWARE_H_ */
