#ifndef __UART_H__
#define __UART_H__

#include "hw_memmap.h"
#include "hw_types.h"
#include "hw_uart.h"

#ifdef PLATFORM_ARM_M3
#include "debug.h"
#include "hw_ints.h"
#include "interrupt.h"
#else
#include "NatureDSP_types.h"
#endif
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
// API Function prototypes
//
//*****************************************************************************
#if !USE_ROM_UART
extern void UARTParityModeSet(unsigned long ulBase, unsigned long ulParity);
extern unsigned long UARTParityModeGet(unsigned long ulBase);
extern void UARTFIFOLevelSet(unsigned long ulBase, unsigned long ulTxLevel, unsigned long ulRxLevel);
extern void UARTFIFOLevelGet(unsigned long ulBase, unsigned long *pulTxLevel, unsigned long *pulRxLevel);
extern void UARTConfigSetExpClk(unsigned long ulBase, unsigned long ulPreDivide, unsigned long ulCD, unsigned long ulBDIV, unsigned long ulConfig);
extern void UARTConfigGetExpClk(unsigned long ulBase, unsigned long *pulPreDivide, unsigned long *pulCD, unsigned long *pulBDIV, unsigned long *pulConfig);
extern void UARTEnable(unsigned long ulBase);
extern void UARTDisable(unsigned long ulBase);
extern unsigned char UARTCharsAvail(unsigned long ulBase);
extern unsigned char UARTSpaceAvail(unsigned long ulBase);
extern long UARTCharGetNonBlocking(unsigned long ulBase);
extern long UARTCharGet(unsigned long ulBase);
extern void UARTNCharGet(unsigned long ulBase, unsigned char *pulRecvData, unsigned long ulLenByte);
#endif
extern unsigned long UARTCharGetWithTimeout(unsigned long ulBase, unsigned char *pulRecvData, unsigned long ulLenByte);
#if !USE_ROM_UART
extern unsigned char UARTCharPutNonBlocking(unsigned long ulBase, unsigned char ucData);
extern void UARTCharPut(unsigned long ulBase, unsigned char ucData);
extern void UARTNCharPut(unsigned long ulBase, unsigned char *pucData, unsigned long ulLenByte);
extern void UARTBreakCtl(unsigned long ulBase, unsigned char bBreakState);
extern void UARTIntRegister(unsigned long ulBase, unsigned long *g_pRAMVectors, void(*pfnHandler)(void));
extern void UARTIntUnregister(unsigned long ulBase, unsigned long *g_pRAMVectors);
extern void UARTIntEnable(unsigned long ulBase, unsigned long ulIntFlags);
extern void UARTIntDisable(unsigned long ulBase, unsigned long ulIntFlags);
extern unsigned long UARTIntStatus(unsigned long ulBase, unsigned char bMasked);
extern void UARTIntClear(unsigned long ulBase);
extern unsigned long UARTRxErrorGet(unsigned long ulBase);
extern void UARTRxErrorClear(unsigned long ulBase);
#endif
extern void UARTTxIntModeSet(unsigned long ulBase, unsigned long ulMode);
extern unsigned long UARTTxIntModeGet(unsigned long ulBase);
extern void UARTConfigSetBaudrate(unsigned long ulBase, unsigned long ulPclk, unsigned long ulBaudrate, unsigned long ulConfig);
#if !USE_ROM_UART
extern void UARTStringPut(unsigned long ulBase, unsigned char *str);
extern void UARTAutoBaudrate(unsigned long ulBase);
extern void UARTConfigSetMode(unsigned long ulBase, unsigned long ulConfig);
extern void UARTWaitTxDone(unsigned long ulBase);
extern void UARTPrintf(unsigned long ulBase, char* fmt, ...);
#endif
extern void printch2(char ch);
//*****************************************************************************
//
// Mark the end of the C bindings section for C++ compilers.
//
//*****************************************************************************
#ifdef __cplusplus
}
#endif

#endif //  __UART_H__
