#ifndef __PRCM_H__
#define __PRCM_H__

#include "hw_prcm.h"
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

#define LOAD_IMAGE          0x01
#define NOT_LOAD_IMAGE      0x00

#if !USE_ROM_PRCM
extern unsigned char AON_DSP_Load_Flag_Get(void);
extern unsigned char AON_ARM_Load_Flag_Get(void);
extern void AON_DSP_Load_Flag_Set(uint8_t flag);
extern void AON_ARM_Load_Flag_Set(uint8_t flag);
#endif
    
extern void AON_Flash_Delay_Get(void);
#if !USE_ROM_PRCM
extern void AON_Flash_Delay_Set(uint8_t value);
    
extern unsigned char AON_Addr_Overlap_Unused_Mem(unsigned char mem_status, unsigned long ulAddr, unsigned long ulSizebyte);
    
extern void PRCM_Clock_Enable(unsigned long ulBase, unsigned long ulModule);
extern void PRCM_Clock_Disable(unsigned long ulBase, unsigned long ulModule);

extern unsigned long PRCM_CHIP_VER_GET(unsigned long ulBase);
#endif
extern unsigned char PRCM_BOOTMODE_GET(unsigned long ulBase);

//*****************************************************************************
//
// Mark the end of the C bindings section for C++ compilers.
//
//*****************************************************************************
#ifdef __cplusplus
}
#endif

#endif //  __PRCM_H__
