#ifndef __XY_MPU_H__
#define __XY_MPU_H__

#include "xinyi_hardware.h"


#define MPU_RASR_XN_ENABLE				(0UL << MPU_RASR_XN_Pos) 
#define MPU_RASR_XN_DISABLE				(1UL << MPU_RASR_XN_Pos) 

#define	MPU_AP_ALL_PROHIBIT	  		 	(0x0UL << MPU_RASR_AP_Pos) 
#define MPU_AP_PRIV_RW_USR_PROHIBIT	 	(0x1UL << MPU_RASR_AP_Pos) 
#define MPU_AP_PRIV_RW_USR_RO			(0x2UL << MPU_RASR_AP_Pos) 
#define MPU_AP_PRIV_RW_USR_RW			(0x3UL << MPU_RASR_AP_Pos) 
#define MPU_AP_ALL_NA					(0x4UL << MPU_RASR_AP_Pos) 
#define MPU_AP_PRIV_RO_USR_PROHIBIT		(0x5UL << MPU_RASR_AP_Pos) 
#define MPU_AP_PRIV_RO_USR_RO		 	(0x6UL << MPU_RASR_AP_Pos) 

#define MPU_MEM_STRONGLY_ORDERED_SHARABLE			((0x0UL<<MPU_RASR_TEX_Pos)|(0x0UL<<MPU_RASR_C_Pos)|(0x0UL<<MPU_RASR_B_Pos))
#define MPU_MEM_SHARED_DEVICE_SHARABLE				((0x0UL<<MPU_RASR_TEX_Pos)|(0x0UL<<MPU_RASR_C_Pos)|(0x1UL<<MPU_RASR_B_Pos))		//with write buffer
#define MPU_MEM_WRITE_THOUGH_NOT_ALLOCATE			((0x0UL<<MPU_RASR_TEX_Pos)|(0x1UL<<MPU_RASR_C_Pos)|(0x0UL<<MPU_RASR_B_Pos))
#define MPU_MEM_WRITE_BACK_NOT_ALLOCATE				((0x0UL<<MPU_RASR_TEX_Pos)|(0x1UL<<MPU_RASR_C_Pos)|(0x1UL<<MPU_RASR_B_Pos))
#define MPU_MEM_NON_CACHEABLE						((0x1UL<<MPU_RASR_TEX_Pos)|(0x0UL<<MPU_RASR_C_Pos)|(0x0UL<<MPU_RASR_B_Pos))
#define MPU_MEM_ATR_RESERVED						((0x1UL<<MPU_RASR_TEX_Pos)|(0x0UL<<MPU_RASR_C_Pos)|(0x1UL<<MPU_RASR_B_Pos))
#define MPU_MEM_IMPLEMENTATION_DEFINED				((0x1UL<<MPU_RASR_TEX_Pos)|(0x1UL<<MPU_RASR_C_Pos)|(0x0UL<<MPU_RASR_B_Pos))
#define MPU_MEM_WRITE_BACK_WITH_ALLOCATE			((0x1UL<<MPU_RASR_TEX_Pos)|(0x1UL<<MPU_RASR_C_Pos)|(1x0UL<<MPU_RASR_B_Pos))
#define MPU_MEM_NON_SHARED_DEVICE					((0x2UL<<MPU_RASR_TEX_Pos)|(0x1UL<<MPU_RASR_C_Pos)|(0x0UL<<MPU_RASR_B_Pos))

#define MPU_RASR_SIZE_32B					(0x4UL << MPU_RASR_SIZE_Pos)
#define MPU_RASR_SIZE_64B					(0x5UL << MPU_RASR_SIZE_Pos)
#define MPU_RASR_SIZE_128B					(0x6UL << MPU_RASR_SIZE_Pos)
#define MPU_RASR_SIZE_256B					(0x7UL << MPU_RASR_SIZE_Pos)
#define MPU_RASR_SIZE_512B					(0x8UL << MPU_RASR_SIZE_Pos)
#define MPU_RASR_SIZE_1K					(0x9UL << MPU_RASR_SIZE_Pos)
#define MPU_RASR_SIZE_2K					(0xAUL << MPU_RASR_SIZE_Pos)
#define MPU_RASR_SIZE_4K					(0xBUL << MPU_RASR_SIZE_Pos)
#define MPU_RASR_SIZE_8K					(0xCUL << MPU_RASR_SIZE_Pos)
#define MPU_RASR_SIZE_16K					(0xDUL << MPU_RASR_SIZE_Pos)
#define MPU_RASR_SIZE_32K					(0xEUL << MPU_RASR_SIZE_Pos)
#define MPU_RASR_SIZE_64K					(0xFUL << MPU_RASR_SIZE_Pos)
#define MPU_RASR_SIZE_128K					(0x10UL << MPU_RASR_SIZE_Pos)
#define MPU_RASR_SIZE_256K					(0x11UL << MPU_RASR_SIZE_Pos)
#define MPU_RASR_SIZE_512K					(0x12UL << MPU_RASR_SIZE_Pos)
#define MPU_RASR_SIZE_1M					(0x13UL << MPU_RASR_SIZE_Pos)
#define MPU_RASR_SIZE_2M					(0x14UL << MPU_RASR_SIZE_Pos)
#define MPU_RASR_SIZE_4M					(0x15UL << MPU_RASR_SIZE_Pos)
#define MPU_RASR_SIZE_8M					(0x16UL << MPU_RASR_SIZE_Pos)
#define MPU_RASR_SIZE_16M					(0x17UL << MPU_RASR_SIZE_Pos)
#define MPU_RASR_SIZE_32M					(0x18UL << MPU_RASR_SIZE_Pos)
#define MPU_RASR_SIZE_64M					(0x19UL << MPU_RASR_SIZE_Pos)
#define MPU_RASR_SIZE_128M					(0x1AUL << MPU_RASR_SIZE_Pos)
#define MPU_RASR_SIZE_256M					(0x1BUL << MPU_RASR_SIZE_Pos)
#define MPU_RASR_SIZE_512M					(0x1CUL << MPU_RASR_SIZE_Pos)
#define MPU_RASR_SIZE_1G					(0x1DUL << MPU_RASR_SIZE_Pos)
#define MPU_RASR_SIZE_2G					(0x1EUL << MPU_RASR_SIZE_Pos)

void disable_watchponit(void);

uint32_t enable_watchponit(uint32_t addr, uint32_t power, uint32_t rw, uint32_t region);

void Enable_MPU_Interrupt_Protect();

void Disable_MPU_Interrupt_protect();
#endif
