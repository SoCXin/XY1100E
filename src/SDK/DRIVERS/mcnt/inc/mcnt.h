#ifndef __MCNT_H__
#define __MCNT_H__

#include "hw_types.h"
#include "hw_mcnt.h"
#include "interrupt.h"

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



//*****************************************************************************
//
// API Function prototypes
//
//*****************************************************************************
extern void MCNTStart(void);
extern void MCNTStop(void);
extern void MCNTSetCNT32K(unsigned long ulCounter);
extern unsigned long MCNTGetCNT32K(void);
extern unsigned long MCNTGetMCNT(void);
extern unsigned char MCNTGetIntStatus(void);
extern void MCNTSetIntStatus(unsigned char ucIntstatus);
extern void MCNTIntRegister(unsigned long *g_pRAMVectors, void (*pfnHandler)(void));
extern void MCNTIntUnregister(unsigned long *g_pRAMVectors);

//*****************************************************************************
//
// Mark the end of the C bindings section for C++ compilers.
//
//*****************************************************************************
#ifdef __cplusplus
}
#endif

#endif //  __MCNT_H__
