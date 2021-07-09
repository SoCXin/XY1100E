#ifndef __XINYI_NVIC_H
#define __XINYI_NVIC_H

#ifdef __cplusplus
 extern "C" {
#endif


/* Includes ------------------------------------------------------------------*/
#include "xinyi_hardware.h"

#ifdef __SOC_XINYI_1100__
/* defined in startup_xinyi_1100.s */
extern uint32_t g_pfnVectors[16+32];
#endif

#ifdef __SOC_XINYI_2100__
/* defined in startup_xinyi_2100.s */
extern uint32_t g_pfnVectors[16+48];
#endif


/* Interrupt service function type */
typedef void(*IRQnHandle_Type)(void);

#define IS_NORMAL_IRQnHandle(IRQnHandle)    ((IRQnHandle) != NULL)


#ifdef __SOC_XINYI_1100__
#define IS_NVIC_IRQn(IRQn)    (((IRQn) == NonMaskableInt_IRQn)   || \
                               ((IRQn) == HardFault_IRQn)        || \
                               ((IRQn) == MemoryManagement_IRQn) || \
                               ((IRQn) == BusFault_IRQn)         || \
                               ((IRQn) == UsageFault_IRQn)       || \
                               ((IRQn) == SVCall_IRQn)           || \
                               ((IRQn) == DebugMonitor_IRQn)     || \
                               ((IRQn) == PendSV_IRQn)           || \
                               ((IRQn) == SysTick_IRQn)          || \
                               (((IRQn) >= CSP1_IRQn) && ((IRQn) <= DFEBUF_IRQn)))
#endif

#ifdef __SOC_XINYI_2100__
#define IS_NVIC_IRQn(IRQn)    (((IRQn) == NonMaskableInt_IRQn)   || \
                               ((IRQn) == HardFault_IRQn)        || \
                               ((IRQn) == MemoryManagement_IRQn) || \
                               ((IRQn) == BusFault_IRQn)         || \
                               ((IRQn) == UsageFault_IRQn)       || \
                               ((IRQn) == SVCall_IRQn)           || \
                               ((IRQn) == DebugMonitor_IRQn)     || \
                               ((IRQn) == PendSV_IRQn)           || \
                               ((IRQn) == SysTick_IRQn)          || \
                               (((IRQn) >= CLKTIM_IRQn) && ((IRQn) <= CMP_IRQn)))
#endif


/*!< the number of Interrupt Vectors */
#define INT_VECTORS_NUM       42

/*!< NVIC priority group,
  the suffix num is the bits of pre-emption priority,
  and the other bits is subpriority,
  our chip is use 3 bits */
#define NVIC_PriorityUseBits         __NVIC_PRIO_BITS
#define NVIC_PriorityGroup_0         ((uint32_t)0x700)
#define NVIC_PriorityGroup_1         ((uint32_t)0x600)
#define NVIC_PriorityGroup_2         ((uint32_t)0x500)
#define NVIC_PriorityGroup_3         ((uint32_t)0x400)
#define IS_NVIC_PRIORITY_GROUP(GROUP)    (((GROUP) == NVIC_PriorityGroup_0) || \
                                          ((GROUP) == NVIC_PriorityGroup_1) || \
                                          ((GROUP) == NVIC_PriorityGroup_2) || \
                                          ((GROUP) == NVIC_PriorityGroup_3))

/*!< Low Power mode set */
#define NVIC_LP_SEVONPEND            ((uint8_t)0x10)
#define NVIC_LP_SLEEPDEEP            ((uint8_t)0x04)
#define NVIC_LP_SLEEPONEXIT          ((uint8_t)0x02)
#define IS_NVIC_LP(LP) (((LP) == NVIC_LP_SEVONPEND) || \
                        ((LP) == NVIC_LP_SLEEPDEEP) || \
                        ((LP) == NVIC_LP_SLEEPONEXIT))

/*!< SysTick Div Select */
#define SysTick_CLKSource_HCLK_Div8      ((uint32_t)0xFFFFFFFB)
#define SysTick_CLKSource_HCLK           ((uint32_t)0x00000004)
#define IS_SYSTICK_CLK_SOURCE(SOURCE)    (((SOURCE) == SysTick_CLKSource_HCLK) || \
                                          ((SOURCE) == SysTick_CLKSource_HCLK_Div8))


/*!< Function Declaration */
void NVIC_PriorityGroupConfig(uint32_t NVIC_PriorityGroup);
void NVIC_SetVectorTable(uint32_t *NVIC_Vectors);
void NVIC_SystemLPConfig(uint8_t LowPowerMode, FunctionalState NewState);
void NVIC_IntRegister(IRQn_Type IRQn, IRQnHandle_Type IRQnHandle, uint32_t priority);
void NVIC_IntUnregister(IRQn_Type IRQn);
IRQnHandle_Type NVIC_GetIntHandler(IRQn_Type IRQn);
void NVIC_SetIntPending(IRQn_Type IRQn);
void NVIC_ClearIntPending(IRQn_Type IRQn);
FlagStatus NVIC_GetIntPending(IRQn_Type IRQn);
FlagStatus NVIC_GetIntActive(IRQn_Type IRQn);
void NVIC_VectorReset(void);


#ifdef __cplusplus
}
#endif

#endif    /* end of __XINYI_NVIC_H */

/******************* (C) COPYRIGHT 2011 STMicroelectronics *****END OF FILE****/
