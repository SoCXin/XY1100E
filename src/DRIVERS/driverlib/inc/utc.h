#ifndef __UTC_H__
#define __UTC_H__

#include "hw_ints.h"
#include "hw_memmap.h"
#include "hw_spi.h"
#include "hw_types.h"
#include "hw_utc.h"
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

#if !USE_ROM_UTC
extern void UTCTimerStop(void);
extern void UTCTimerRun(void);
extern void UTCCalStop(void);
extern void UTCCalRun(void);
extern void UTCDivStop(void);
extern void UTCDivEn(void);
extern void UTCHourModeSet(unsigned long ulMode);
extern unsigned long UTCHourModeGet(void);
extern void UTCTimerSet(unsigned long ulAMPM, unsigned long ulHour, unsigned long ulMin, unsigned long ulSec, unsigned long ulMinSec);
extern unsigned long UTCTimerChangeGet(void);
extern unsigned long UTCTimerGetBy10ms(void);
extern void UTCTimerAlarmSetBy10ms(unsigned long ulMinSec);
extern unsigned long UTCCalChangeGet(void);
extern void UTCCalAlarmSet(unsigned long ulMonth, unsigned long ulData);
extern void UTCAlarmEnable(unsigned long ulAlarmFlags);
extern void UTCAlarmDisable(unsigned long ulAlarmFlags);
extern void UTCIntEnable(unsigned long ulIntFlags);
extern void UTCIntDisable(unsigned long ulIntFlags);
extern void UTCIntMaskSet(unsigned long ulIntMask);
extern unsigned long UTCIntMaskGet(void);
extern void UTCIntStatusSet(unsigned long ulIntFlags);
extern unsigned long UTCIntStatusGet(void);
extern unsigned long UTCValidStatusGet(void);
extern void UTCKeepRun(unsigned long ulKeepUTC);
extern void UTCClkCntSet(unsigned long ulClkCnt);
extern void UTCIntRegister(unsigned long *g_pRAMVectors, void (*pfnHandler)(void));
extern void UTCIntUnregister(unsigned long *g_pRAMVectors);
#endif
extern void UTCTimerGet(unsigned char *ulAMPM, unsigned char *ulHour, unsigned char *ulMin, unsigned char *ulSec, unsigned char *ulMinSec,unsigned long ulRegData);

extern void UTCTimerAlarmSet(unsigned long ulAMPM, unsigned long ulHour, unsigned long ulMin, unsigned long ulSec, unsigned long ulMS);
extern void UTCTimerAlarmSetAccurate(unsigned long ulAMPM, unsigned long ulHour, unsigned long ulMin, unsigned long ulSec, unsigned long ulMS);

extern void UTCCalSet(unsigned long ulCentury, unsigned long ulYear, unsigned long ulMonth, unsigned long ulData, unsigned long ulDay);
extern void UTCCalGet(unsigned char *ulCentury, unsigned char *ulYear, unsigned char *ulMonth, unsigned char *ulData, unsigned char *ulDay,unsigned long ulRegData);
extern unsigned long UTCClkCntConvert(unsigned long ulRegData);

extern void Switch_to_Outer_UTC_XTAL(void);

extern void UTCCalAlarmGet(unsigned char *ulMonth, unsigned long *ulDate);

extern void UTCTimerAlarmGet(unsigned long *ulAMPM, unsigned long *ulHour, unsigned long *ulMin, unsigned long *ulSec, unsigned long *ulMS);

//*****************************************************************************
//
// Mark the end of the C bindings section for C++ compilers.
//
//*****************************************************************************
#ifdef __cplusplus
}
#endif

#endif // __UTC_H__
