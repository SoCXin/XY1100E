#ifndef __I2C_H__
#define __I2C_H__

#include "hw_i2c.h"
#include "hw_ints.h"
#include "hw_memmap.h"
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
// Prototypes for the APIs.
//
//*****************************************************************************
#if !USE_ROM_I2C
extern void I2CIntRegister(unsigned long ulBase, unsigned long *g_pRAMVectors, void(fnHandler)(void));
extern void I2CIntUnregister(unsigned long ulBase, unsigned long *g_pRAMVectors);
extern unsigned char I2CBusBusy(unsigned long ulBase);
extern unsigned char I2CMasterBusy(unsigned long ulBase);
extern void I2CControlSet(unsigned long ulBase, unsigned long ulCmd);
extern unsigned long I2CControlGet(unsigned long ulBase);
extern unsigned long I2CDataGet(unsigned long ulBase);
extern void I2CDataPut(unsigned long ulBase, unsigned char ucData);
extern void I2CMasterEnable(unsigned long ulBase);
extern unsigned long I2CMasterErr(unsigned long ulBase);
extern void I2CMasterInitExpClk(unsigned long ulBase, unsigned long ulI2CClk, unsigned char bFast);
extern void I2CIntDisable(unsigned long ulBase, unsigned long ulIntFlags);
extern void I2CIntEnable(unsigned long ulBase, unsigned long ulIntFlags);
extern void I2CIntClear(unsigned long ulBase);
extern void I2CAddrSet(unsigned long ulBase, unsigned long ulAddr, unsigned char ucNorMode, unsigned char ucReceive);
extern void I2CSlaveEnable(unsigned long ulBase);
extern void I2CACKModeSet(unsigned long ulBase, unsigned char ucEnable);
extern void I2CFifoClear(unsigned long ulBase);
#endif

//*****************************************************************************
//
// Mark the end of the C bindings section for C++ compilers.
//
//*****************************************************************************
#ifdef __cplusplus
}
#endif

#endif // __I2C_H__
