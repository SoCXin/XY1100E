#ifndef __FLASH_VENDOR_H__
#define __FLASH_VENDOR_H__

#include "hw_types.h"
#include "hw_dma.h"
#include "debug.h"
#include "interrupt.h"
#include "dma.h"
#include "qspi_flash.h"
#include "replace.h"

#ifdef __cplusplus
extern "C"
{
#endif

enum FLASH_VENDOR {
    FLASH_UNKNOWN = 0,
    FLASH_GD25Q16,
    FLASH_GD25Q32,
    FLASH_XT25F16,
    FLASH_XT25F32,
    FLASH_PU25F16,
    FLASH_PU25F32,
};

typedef enum {
	SR1,
	SR2,
	SR3
}FLASH_STATUS_REG;

typedef enum {
	FLASH_PROTECT_MODE_0,      /* cmp 0, none;             cmp 1, 000000H-3FFFFFH */
	FLASH_PROTECT_MODE_1,      /* cmp 0, 3F0000H-3FFFFFH;  cmp 1, 000000H-3EFFFFH */
	FLASH_PROTECT_MODE_2,      /* cmp 0, 3E0000H-3FFFFFH;  cmp 1, 000000H-3DFFFFH */
	FLASH_PROTECT_MODE_3,      /* cmp 0, 3C0000H-3FFFFFH;  cmp 1, 000000H-3BFFFFH */
	FLASH_PROTECT_MODE_4,      /* cmp 0, 380000H-3FFFFFH;  cmp 1, 000000H-37FFFFH */
	FLASH_PROTECT_MODE_5,      /* cmp 0, 300000H-3FFFFFH;  cmp 1, 000000H-2FFFFFH */
	FLASH_PROTECT_MODE_6,      /* cmp 0, 200000H-3FFFFFH;  cmp 1, 000000H-1FFFFFH */
	FLASH_PROTECT_MODE_9 = 9,  /* cmp 0, 000000H-00FFFFH;  cmp 1, 010000H-3FFFFFH */
	FLASH_PROTECT_MODE_10,     /* cmp 0, 000000H-01FFFFH;  cmp 1, 020000H-3FFFFFH */
	FLASH_PROTECT_MODE_11,     /* cmp 0, 000000H-03FFFFH;  cmp 1, 040000H-3FFFFFH */
	FLASH_PROTECT_MODE_12,     /* cmp 0, 000000H-07FFFFH;  cmp 1, 080000H-3FFFFFH */
	FLASH_PROTECT_MODE_13,     /* cmp 0, 000000H-0FFFFFH;  cmp 1, 100000H-3FFFFFH */
	FLASH_PROTECT_MODE_14,     /* cmp 0, 000000H-1FFFFFH;  cmp 1, 200000H-3FFFFFH */
	FLASH_PROTECT_MODE_15,     /* cmp 0, 000000H-3FFFFFH;  cmp 1, none            */
	FLASH_PROTECT_MODE_17 = 17,/* cmp 0, 3FF000H-3FFFFFH;  cmp 1, 000000H-3FEFFFH */
	FLASH_PROTECT_MODE_18,     /* cmp 0, 3FE000H-3FFFFFH;  cmp 1, 000000H-3FDFFFH */
	FLASH_PROTECT_MODE_19,     /* cmp 0, 3FC000H-3FFFFFH;  cmp 1, 000000H-3FBFFFH */
	FLASH_PROTECT_MODE_20,     /* cmp 0, 3F8000H-3FFFFFH;  cmp 1, 000000H-3F7FFFH */
	FLASH_PROTECT_MODE_22 = 22,/* cmp 0, 3F8000H-3FFFFFH;  cmp 1, 000000H-3F7FFFH */
	FLASH_PROTECT_MODE_25 = 25,/* cmp 0, 000000H-000FFFH;  cmp 1, 001000H-3FFFFFH */
	FLASH_PROTECT_MODE_26,     /* cmp 0, 000000H-001FFFH;  cmp 1, 002000H-3FFFFFH */
	FLASH_PROTECT_MODE_27,     /* cmp 0, 000000H-003FFFH;  cmp 1, 004000H-3FFFFFH */
	FLASH_PROTECT_MODE_28,     /* cmp 0, 000000H-007FFFH;  cmp 1, 008000H-3FFFFFH */
	FLASH_PROTECT_MODE_30      /* cmp 0, 000000H-007FFFH;  cmp 1, 008000H-3FFFFFH */
} FLASH_PROTECT_MODE;

#define RDID_GD25Q16                    0xC84015
#define RDID_GD25Q32                    0xC84016
#define RDID_XT25F16                    0x0B4015
#define RDID_XT25F32                    0x0B4016
#define RDID_PU25F16                    0x856015
#define RDID_PU25F32                    0x856016
#define RDID_BY25Q16                    0x684015
#define RDID_BY25Q32                    0x684016


#define OTP_BASE_ADDR_GD25Q16           0x27000000
#define OTP_BASE_ADDR_GD25Q32           0x27001000
#define OTP_BASE_ADDR_XT25F16           0x27000000
#define OTP_BASE_ADDR_XT25F32           0x27000000
#define OTP_BASE_ADDR_PU25F16           0x27001000
#define OTP_BASE_ADDR_PU25F32           0x27001000

#define FLASH_CMD_WRITE_ENABLE  	    0x06
#define FLASH_CMD_WRITE_DISABLE  	    0x04
#define FLASH_CMD_CHIP_ERASE    	    0x60
#define FLASH_CMD_SECTOR_ERASE		    0x20
#define FLASH_CMD_BLOCK_ERASE_32K	    0x52
#define FLASH_CMD_BLOCK_ERASE_64K	    0xD8
#define	FLASH_CMD_READ_DEVICEID		    0x9F
#define	FLASH_CMD_READ_UNIQUEID         0x4B
#define	FLASH_CMD_SECURITY_ERASE        0x44
#define	FLASH_CMD_SECURITY_WRITE        0x42
#define	FLASH_CMD_SECURITY_READ         0x48

#define FLASH_CMD_WRITE_STATUS_REG		0x01		// 01H S7-S0 S15-S8		GD25Q16/XT25F16/XT25F32/PU25Q16/PU25Q32
#define FLASH_CMD_WRITE_STATUS_REG1		0x01        // 01H S7-S0			GD25Q32
#define FLASH_CMD_WRITE_STATUS_REG2		0x31        // 31H S15-S8			GD25Q32

#define FLASH_CMD_READ_STATUS_REG1		0x05        // 05H S7-S0			GD25Q16/GD25Q32/XT25F16/XT25F32/PU25Q16/PU25Q32
#define FLASH_CMD_READ_STATUS_REG2		0x35        // 35H S15-S8			GD25Q16/GD25Q32/XT25F16/XT25F32/PU25Q16/PU25Q32

#define	STATUS_REG2_QE					0x02		// bit S9				GD25Q16/GD25Q32/XT25F16/XT25F32/PU25Q16/PU25Q32
#define	STATUS_REG2_LB					0x04		// bit S10				GD25Q16/XT25F16/XT25F32
#define	STATUS_REG2_LB1					0x08		// bit S11				GD25Q32/PU25Q16/PU25Q32
#define	STATUS_REG2_LB2					0x10		// bit S12				GD25Q32/PU25Q16/PU25Q32
#define	STATUS_REG2_LB3					0x20		// bit S13				GD25Q32/PU25Q16/PU25Q32

#define STATUS_REG1_WIP					0x01		// bit S0
#define STATUS_REG1_WEL					0x02		// bit S1

#define QSPI_READ_DEFAULT               0x00000003
#define QSPI_READ_QUAD                  0x041220EB  //0x0802006B
#define QSPI_READ_XIP                   0x041220EB
#define QSPI_READ_SECURITY              0x08000048

#define QSPI_WRITE_DEFAULT              0x00000002
#define QSPI_WRITE_QUAD                 0x00020032
#define QSPI_WRITE_SECURITY             0x00000042

#define QSPI_DELAY_DEFAULT              0x03010101

#define RDID_GD25Q16                    0xC84015
#define RDID_GD25WQ16					0xC86515

#define AON_AONGPREG0                   0xA0058014		// bit 7, 1: Flash Detect Valid;
#define AON_AONGPREG1                   0xA0058015		// bit 3, 1: GD25WQ16; 0: GD25Q16;

extern  QSPI_FLASH_Def xinyi_flash;
extern void sys_flash_init(void);
extern void FLASH_ChipErase(QSPI_FLASH_Def *flash_vendor);
extern void FLASH_SectorErase(QSPI_FLASH_Def *flash_vendor, unsigned int address);
extern void FLASH_BlockErase32K(QSPI_FLASH_Def *flash_vendor, unsigned int address);
extern void FLASH_BlockErase64K(QSPI_FLASH_Def *flash_vendor, unsigned int address);
extern void FLASH_WriteData(QSPI_FLASH_Def *flash_vendor, unsigned long ulSrcAddr, unsigned long ulDstAddr, unsigned long ulLenByte, unsigned long ulChannelNum);
extern void FLASH_ReadData(QSPI_FLASH_Def *flash_vendor, unsigned long ulSrcAddr, unsigned long ulDstAddr, unsigned long ulLenByte, unsigned long ulChannelNum);
extern void FLASH_WriteEnable(QSPI_FLASH_Def *flash_vendor);
extern void FLASH_WaitIdle(QSPI_FLASH_Def *flash_vendor);
extern void FLASH_EnableQEFlag(QSPI_FLASH_Def *flash_vendor);
extern void FLASH_EnterQPIMode(QSPI_FLASH_Def *flash_vendor);
extern void FLASH_EnterXIPMode(QSPI_FLASH_Def *flash_vendor, char xip_dummy);
extern void FLASH_ExitXIPMode(QSPI_FLASH_Def *flash_vendor);
extern void FLASH_SetReadMode(QSPI_FLASH_Def *flash_vendor, unsigned int mode);
extern void FLASH_SetWriteMode(QSPI_FLASH_Def *flash_vendor, unsigned int mode);
extern void FLASH_SetReadParameter(QSPI_FLASH_Def *flash_vendor, unsigned char parameter);
extern void Flash_Exchange_Recover(QSPI_FLASH_Def *flash_vendor, unsigned long ulExchangeAddr);
extern void FLASH_SetProtectMode(QSPI_FLASH_Def *flash_vendor, FLASH_PROTECT_MODE protect_mode, uint8_t cmp);

extern unsigned char guid[16];
extern void FLASH_GetDeviceID(void);

#ifdef __cplusplus
}
#endif

#endif //  __FLASH_VENDOR_H__
