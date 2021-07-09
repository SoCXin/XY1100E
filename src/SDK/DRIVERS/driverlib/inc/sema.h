#ifndef __SEMA_H__
#define __SEMA_H__

#include "hw_sema.h"
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

#define SEMA_MASTER_ARM             1
#define SEMA_MASTER_DSP             2
            
#define SEMA_SLAVE_WDT              0
#define SEMA_SLAVE_UTC              1
#define SEMA_SLAVE_UART             2
#define SEMA_SLAVE_TIMER1           3
#define SEMA_SLAVE_TIMER2           4
#define SEMA_SLAVE_SPI              5
#define SEMA_SLAVE_QSPI_DATA        6
#define SEMA_SLAVE_QSPI_REG         21
#define SEMA_SLAVE_PRCM             7
#define SEMA_SLAVE_MRASURECNT       8
#define SEMA_SLAVE_I2C1             9
#define SEMA_SLAVE_GPIO             10
#define SEMA_SLAVE_CSP1             11
#define SEMA_SLAVE_CSP2             12
#define SEMA_SLAVE_BACKUP_MEM       13
#define SEMA_SLAVE_SYS_MEM0         14
#define SEMA_SLAVE_SYS_MEM1         15
#define SEMA_SLAVE_SYS_MEM2         16
#define SEMA_SLAVE_SYS_MEM3         17
#define SEMA_SLAVE_TIMER3           18
#define SEMA_SLAVE_TIMER4           19
#define SEMA_SLAVE_DMAC             20
#define SEMA_SLAVE_AES_REG          22
#define SEMA_SLAVE_AES_DATA         23
#define SEMA_SLAVE_I2C2             24
#define SEMA_SLAVE_CSP3             25
#define SEMA_SLAVE_CSP4             26

// Software needs to appy the access permission by semaphore block first before access the resources.
// In case DMA, software needs to apply for it as well before start dma operation.

extern void SEMAMasterEnable(unsigned long ulBase, unsigned char ucSemaMaster, unsigned char ucSemaSlave);
extern void SEMAMasterDisable(unsigned long ulBase, unsigned char ucSemaMaster, unsigned char ucSemaSlave);
extern void SEMARequest(unsigned long ulBase, unsigned char ucSemaMaster, unsigned char ucSemaSlave);//, unsigned char ucMasterMask);
extern void SEMARequestNonBlocking(unsigned long ulBase, unsigned char ucSemaMaster, unsigned char ucSemaSlave, unsigned char ucMasterMask);
extern void SEMARequestPriSet(unsigned long ulBase, unsigned char ucSemaSlave, unsigned long ucPriority);
extern unsigned char SEMAMasterGet(unsigned long ulBase, unsigned char ucSemaSlave);
extern unsigned long SEMASlaveGet(unsigned long ulBase, unsigned char ucSemaMaster);
extern void SEMAReset(unsigned long ulBase);
extern unsigned char SEMAReqQueueState(unsigned long ulBase);
extern void SEMAMasterIntEnable(unsigned long ulBase, unsigned long ulSemaMasterInt);
extern void SEMARelease(unsigned long ulBase, unsigned char ucSemaMaster, unsigned char ucSemaSlave);//, unsigned char ucMasterMask);
#if !USE_ROM_SEMA
extern void SEMAIntRegister(unsigned long *g_pRAMVectors, void (*pfnHandler)(void));
extern void SEMAIntUnregister(unsigned long *g_pRAMVectors);
#endif

//*****************************************************************************
//
// Mark the end of the C bindings section for C++ compilers.
//
//*****************************************************************************
#ifdef __cplusplus
}
#endif

#endif //  __SEMA_H__
