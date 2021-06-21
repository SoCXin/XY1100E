#ifndef __WATCHDOG_H__
#define __WATCHDOG_H__

#include "hw_ints.h"
#include "hw_memmap.h"
#include "hw_types.h"
#include "hw_watchdog.h"
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
// Prototypes for the APIs.
//
//*****************************************************************************
extern unsigned char WatchdogRunning(unsigned long ulBase);
extern void WatchdogEnable(unsigned long ulBase);
extern void WatchdogResetEnable(unsigned long ulBase);
extern void WatchdogResetDisable(unsigned long ulBase);
extern void WatchdogTimerRepeatEnable(unsigned long ulBase);
extern void WatchdogTimerRepeatDisable(unsigned long ulBase);
extern void WatchdogReloadSet(unsigned long ulBase, unsigned long ulLoadVal);
extern unsigned long WatchdogValueGet(unsigned long ulBase);
#if !USE_ROM_WDT
extern void WatchdogIntRegister(unsigned long *g_pRAMVectors, void(*pfnHandler)(void));
extern void WatchdogIntUnregister(unsigned long *g_pRAMVectors);
extern void WatchdogIntEnable(void);
#endif
extern unsigned long WatchdogIntStatus(unsigned long ulBase, unsigned char bMasked);
extern void WatchdogIntClear(unsigned long ulBase);
extern void WatchdogDisable(unsigned long ulBase);
extern void WatchdogIntDisable(void);


//*****************************************************************************
//
// Mark the end of the C bindings section for C++ compilers.
//
//*****************************************************************************
#ifdef __cplusplus
}
#endif

#endif // __WATCHDOG_H__
