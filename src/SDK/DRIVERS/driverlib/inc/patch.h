#ifndef __PATCH_H__
#define __PATCH_H__

#include "hw_sema.h"
#include "hw_gpio.h"
#include "hw_ints.h"
#include "hw_memmap.h"
#include "hw_types.h"
#include "debug.h"
#include "interrupt.h"
//#include "rom.h"

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

extern void PATCH_GPIOConflictStatusClear(unsigned char ucPins);
extern void PATCH_SEMARequest(unsigned long ulBase, unsigned char ucSemaMaster, unsigned char ucSemaSlave);
extern void PATCH_SEMARelease(unsigned long ulBase, unsigned char ucSemaMaster, unsigned char ucSemaSlave);
extern void PATCH_UARTPrint(char* fmt, ...);

//*****************************************************************************
//
// Mark the end of the C bindings section for C++ compilers.
//
//*****************************************************************************
#ifdef __cplusplus
}
#endif

#endif //  __PATCH_H__
