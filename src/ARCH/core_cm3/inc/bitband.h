#ifndef __BITBAND_H__
#define __BITBAND_H__

#ifdef __cplusplus
 extern "C" {
#endif

#include <stdint.h>

/*!< BitBand and BitBand alias area address translation formula */
//#define BITBAND(addr, bitnum)    ((addr&0xF0000000)+0x2000000+((addr&0xFFFFF)<<5)+(bitnum<<2))
#define MEM_BITBAND(addr, bitnum)     (0x22000000+((addr-0x20000000)*32)+(bitnum*4))
#define PRI_BITBAND(addr, bitnum)     (0x42000000+((addr-0x40000000)*32)+(bitnum*4))
#define MEM_ADDR(addr)                *((volatile uint32_t *)(addr))
#define MEM_BIT_ADDR(addr, bitnum)    MEM_ADDR(MEM_BITBAND((uint32_t)(addr), (bitnum)))
#define PRI_BIT_ADDR(addr, bitnum)    MEM_ADDR(PRI_BITBAND((uint32_t)(addr), (bitnum)))


#ifdef __cplusplus
 }
#endif

#endif	/* end of __BITBAND_H__ */
