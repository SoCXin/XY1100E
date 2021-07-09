#ifndef __HW_TYPES_H__
#define __HW_TYPES_H__

#include <stdint.h>

//*****************************************************************************
//
// Define a boolean type, and values for true and false.
//
//*****************************************************************************


//*****************************************************************************
//
// Macros for hardware access, both direct and via the bit-band region.
//
//*****************************************************************************
#define HWREG(x)	(*((volatile unsigned long *)(x)))
#define HWREGH(x)	(*((volatile unsigned short *)(x)))
#define HWREGB(x)	(*((volatile unsigned char *)(x)))

#define HWD_REG_WRITE32(addr, data) (*(volatile uint32_t*)addr = (uint32_t)data)
#define HWD_REG_WRITE16(addr, data) (*(volatile uint16_t*)addr = (uint16_t)data)
#define HWD_REG_WRITE08(addr, data) (*(volatile uint8_t*)addr = (uint8_t)data)

#define HWD_REG_READ32(addr)        (*(volatile uint32_t*)addr)
#define HWD_REG_READ16(addr)        (*(volatile uint16_t*)addr)
#define HWD_REG_READ08(addr)        (*(volatile uint8_t*)addr)
	
#endif // __HW_TYPES_H__
