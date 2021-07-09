#ifndef __INTERRUPT_H__
#define __INTERRUPT_H__

#include "hw_ints.h"
#include "hw_nvic.h"
#include "hw_types.h"
#include "debug.h"
#include "replace.h"

//extern void (*g_NVICRAMVectors[NUM_INTERRUPTS])(void);

//*****************************************************************************
//
// If building with a C++ compiler, make all of the definitions in this header
// have a C binding.
//
//*****************************************************************************
#ifdef __cplusplus
extern "C"
{
#endif

//*****************************************************************************
//
// Macro to generate an interrupt priority mask based on the number of bits
// of priority supported by the hardware.
//
//*****************************************************************************
#define INT_PRIORITY_MASK       ((0xFF << (8 - NUM_PRIORITY_BITS)) & 0xFF)

//*****************************************************************************
//
// Prototypes for the APIs.
//
//*****************************************************************************
#if !USE_ROM_INT
extern unsigned char IntMasterDisable(void);
extern void IntRegister(unsigned long ulInterrupt, unsigned long *g_pRAMVectors, void (*pfnHandler)(void));
extern void IntUnregister(unsigned long ulInterrupt, unsigned long *g_pRAMVectors);
extern void IntPriorityGroupingSet(unsigned long ulBits);
extern unsigned long IntPriorityGroupingGet(void);
extern void IntPrioritySet(unsigned long ulInterrupt,
                           unsigned char ucPriority);
extern long IntPriorityGet(unsigned long ulInterrupt);
extern void IntEnable(unsigned long ulInterrupt);
extern void IntDisable(unsigned long ulInterrupt);
extern void IntPendSet(unsigned long ulInterrupt);
extern void IntPendClear(unsigned long ulInterrupt);
#endif

//*****************************************************************************
//
// Mark the end of the C bindings section for C++ compilers.
//
//*****************************************************************************
#ifdef __cplusplus
}
#endif

#endif // __INTERRUPT_H__
