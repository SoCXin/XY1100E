#ifndef __TIMER_H__
#define __TIMER_H__


#include "hw_ints.h"
#include "hw_memmap.h"
#include "hw_timer.h"
#include "hw_types.h"
#include "debug.h"
#include "interrupt.h"
#include "replace.h"

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
// Values that can be passed to TimerConfigure as the ulConfig parameter.
//
//*****************************************************************************


#if !USE_ROM_TIMER
//*****************************************************************************
//
// Prototypes for the APIs.
//
//*****************************************************************************
extern void TimerEnable(unsigned long ulBase);
extern void TimerDisable(unsigned long ulBase);
extern void TimerConfigure(unsigned long ulBase, unsigned long ulConfig);
extern void TimerControlLevel(unsigned long ulBase, unsigned char bInvert);
extern void TimerPrescaleSet(unsigned long ulBase, unsigned long ulValue);
extern unsigned long TimerPrescaleGet(unsigned long ulBase);
extern void TimerLoadSet(unsigned long ulBase, unsigned long ulValue);
extern unsigned long TimerValueGet(unsigned long ulBase);
extern void TimerMatchSet(unsigned long ulBase, unsigned long ulValue);
extern unsigned long TimerMatchGet(unsigned long ulBase);
extern void TimerPWMSet(unsigned long ulBase, unsigned long ulValue);
extern unsigned long TimerPWMGet(unsigned long ulBase);
extern void TimerIntRegister(unsigned long ulIntChannel, unsigned long *g_pRAMVectors, void (*pfnHandler)(void));
extern void TimerIntUnregister(unsigned long ulIntChannel, unsigned long *g_pRAMVectors);
extern void TimerIntEnable(unsigned long ulBase, unsigned long ulIntFlags);
#endif

//*****************************************************************************
//
// Mark the end of the C bindings section for C++ compilers.
//
//*****************************************************************************
#ifdef __cplusplus
}
#endif

#endif // __TIMER_H__
