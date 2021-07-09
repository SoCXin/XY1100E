#ifndef __DMA_H__
#define __DMA_H__

#include "hw_types.h"
#include "hw_dma.h"
#include "debug.h"
#include "interrupt.h"
#include "replace.h"
#include "xy_utils.h"

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
//! \addtogroup dma_api
//! @{
//
//*****************************************************************************

#define DMA_CHANNEL_0           0x00000000
#define DMA_CHANNEL_1           0x00000001
#define DMA_CHANNEL_2           0x00000003
#define DMA_CHANNEL_3           0x00000004

extern osMutexId_t dma_transfer_mutex;

//*****************************************************************************
//
// API Function prototypes
//
//*****************************************************************************
#if !USE_ROM_DMA
extern unsigned long DMAErrorStatusGet(unsigned long ulChannelNum);
extern void DMAErrorStatusClear(unsigned long ulChannelNum);
extern void DMAChannelRequest(unsigned long ulChannelNum);
extern void DMAChannelControlSet(unsigned long ulChannelNum, unsigned long ulControl);
extern void DMAChannelTransferSet(unsigned long ulChannelNum, void *pvSrcAddr, void *pvDstAddr, unsigned long ulTransferSize);
extern void DMAChannelNextPointerSet(unsigned long ulChannelNum, void *pvNextPointer);
extern void DMAIntRegister(unsigned long ulIntChannel, unsigned long *g_pRAMVectors, void (*pfnHandler)(void));
extern void DMAIntUnregister(unsigned long ulIntChannel, unsigned long *g_pRAMVectors);
extern unsigned long DMAIntStatus(unsigned long ulChannelNum);
extern void DMAIntClear(unsigned long ulChanMask);
extern void DMAChannelWaitIdle(unsigned long ulChannelNum);
extern void DMAChannelTransfer(unsigned long ulDst, unsigned long ulSrc, unsigned long ulLenByte, unsigned long ulChannelNum);
extern void DMAWaitAESDone(void);
#endif

extern void DMAChannelTransfer_withMutex(unsigned long ulDst, unsigned long ulSrc, unsigned long ulLenByte, unsigned long ulChannelNum);

//*****************************************************************************
//
// Mark the end of the C bindings section for C++ compilers.
//
//*****************************************************************************
#ifdef __cplusplus
}
#endif

#endif //  __DMA_H__
