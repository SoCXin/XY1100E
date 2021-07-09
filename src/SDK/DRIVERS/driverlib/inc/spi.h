#ifndef __SPI_H__
#define __SPI_H__

#include "hw_ints.h"
#include "hw_memmap.h"
#include "hw_spi.h"
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
// Values that can be passed to SSIConfigSetExpClk.
//
//*****************************************************************************
#define SPI_FRF_MOTO_MODE_0     0x00000000  // Moto fmt, polarity 0, phase 0
#define SPI_FRF_MOTO_MODE_1     0x00000004  // Moto fmt, polarity 0, phase 1
#define SPI_FRF_MOTO_MODE_2     0x00000002  // Moto fmt, polarity 1, phase 0
#define SPI_FRF_MOTO_MODE_3     0x00000006  // Moto fmt, polarity 1, phase 1

#define SPI_FIFO_BYTES_RX       (4*32)
#define SPI_FIFO_BYTES_TX       (4*128)

//*****************************************************************************
//
// Prototypes for the APIs.
//
//*****************************************************************************
#if !USE_ROM_SPI
extern void SPIConfigSetExpClk(unsigned long ulBase, unsigned long ulDivide,
                               unsigned long ulProtocol, unsigned long ulMode,
                               unsigned long ulDataWidth);
extern void SPIEnable(unsigned long ulBase);
extern void SPIDisable(unsigned long ulBase);
extern void SPIIntRegister(unsigned long ulBase, unsigned long *g_pRAMVectors, void(*pfnHandler)(void));
extern void SPIIntUnregister(unsigned long ulBase, unsigned long *g_pRAMVectors);
extern void SPIIntEnable(unsigned long ulBase, unsigned long ulIntFlags);
#endif
extern void SPIIntDisable(unsigned long ulBase, unsigned long ulIntFlags);
#if !USE_ROM_SPI
extern unsigned long SPIIntStatus(unsigned long ulBase, unsigned char bMasked);
#endif
extern void SPIDataPut(unsigned long ulBase, unsigned long ulData);
#if !USE_ROM_SPI
extern long SPIDataPutNonBlocking(unsigned long ulBase, unsigned long ulData);
#endif
extern void SPIDataGet(unsigned long ulBase, unsigned long *pulData);
#if !USE_ROM_SPI
extern long SPIDataGetNonBlocking(unsigned long ulBase, unsigned long *pulData);
extern void SPISetTxFifoThreshold(unsigned long ulBase, unsigned long ulThreshold);
extern void SPISetRxFifoThreshold(unsigned long ulBase, unsigned long ulThreshold);
extern void SPISetDelay(unsigned long ulBase, unsigned long ulDelay);
extern void SPIEnManualCS(unsigned long ulBase);
extern void SPIDisManualCS(unsigned long ulBase);
extern void SPIEnManualTransmit(unsigned long ulBase);
extern void SPIDisManualTransmit(unsigned long ulBase);
extern void SPIStartManualTransmit(unsigned long ulBase);
extern void SPIEnModeFail(unsigned long ulBase);
extern void SPIDisModeFail(unsigned long ulBase);
extern void SPITxFifoFlush(unsigned long ulBase);
extern void SPITxFifoNormal(unsigned long ulBase);
extern void SPIRxFifoFlush(unsigned long ulBase);
extern void SPIRxFifoNormal(unsigned long ulBase);
#endif

//*****************************************************************************
//
// Mark the end of the C bindings section for C++ compilers.
//
//*****************************************************************************
#ifdef __cplusplus
}
#endif

#endif // __SPI_H__
