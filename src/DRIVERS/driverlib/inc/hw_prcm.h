#ifndef __HW_PRCM_H__
#define __HW_PRCM_H__


//*****************************************************************************
//
// The following are defines for the AON PRCM register offsets.
//
//*****************************************************************************
#define AON_SLPWKP_CTL                  0xA0058000
#define AON_UP_STAT                     0xA0058004
#define AON_AONGPREG                    0xA0058014

#define AON_8_SYSMEM_CTL                0xA0058002
#define AON_8_DSPMEM_CTL                0xA0058003

//*****************************************************************************
//
// The following are defines for the bit fields in the AON_UP_STAT register.
//
//*****************************************************************************
#define AON_UP_STAT_WDTRST              0x00000040
#define AON_UP_STAT_STANDBY             0x00000020
#define AON_UP_STAT_DEEPSLEEP           0x00000010
#define AON_UP_STAT_SLEEP               0x00000008
#define AON_UP_STAT_SOFTRST             0x00000004
#define AON_UP_STAT_PINRST              0x00000002
#define AON_UP_STAT_POR                 0x00000001


//*****************************************************************************
//
// The following are defines for the bit fields in the AON_AONGPREG register.
//
//*****************************************************************************
// bit 6, UTC Control Set to 0x03
// bit 5-3, External Flash Delay Config
// bit 2, OPLPM Wakeup, PLL Config
// bit 1, AONGPREG_CORE_LOAD_FLAG_ARM
// bit 0, AONGPREG_CORE_LOAD_FLAG_DSP
#define AONGPREG_UTC_POR                0x40
#define AONGPREG_FLASH_DELAY            0x38
#define AONGPREG_OPLPM_FLAG             0x04
#define AONGPREG_CORE_LOAD_FLAG_ARM     0x02
#define AONGPREG_CORE_LOAD_FLAG_DSP     0x01


//*****************************************************************************
//
// The following are defines for the Core PRCM register offsets.
//
//*****************************************************************************
#define PRCM_CKG_CTL                    0x00000008
#define PRCM_CLKRST_CTL                 0x0000000C
#define PRCM_CHIP_VER                   0x00000010
#define PRCM_DSPCLKRSTCTL               0x00000018
#define PRCM_BOOTMODE_CTL               0x00000024


//*****************************************************************************
//
// The following are defines for the bit fields in the PRCM_CKG_CTL register.
//
//*****************************************************************************
#define PRCM_CKG_CTL_I2C1_EN            0x00000001
#define PRCM_CKG_CTL_SPI_EN             0x00000002
#define PRCM_CKG_CTL_UART1_EN           0x00000004
#define PRCM_CKG_CTL_QSPI_EN            0x00000008
#define PRCM_CKG_CTL_WDT_EN             0x00000010
#define PRCM_CKG_CTL_UTC_EN             0x00000020
#define PRCM_CKG_CTL_MCNT_EN            0x00000040
#define PRCM_CKG_CTL_CSP1_EN            0x00000080
#define PRCM_CKG_CTL_CSP2_EN            0x00000100
#define PRCM_CKG_CTL_CSP3_EN            0x00000200
#define PRCM_CKG_CTL_CSP4_EN            0x00000400
#define PRCM_CKG_CTL_DFE_EN             0x00000800
#define PRCM_CKG_CTL_ANASNS_EN          0x00001000
#define PRCM_CKG_CTL_AES_EN             0x00002000
#define PRCM_CKG_CTL_SEMA_EN            0x00004000
#define PRCM_CKG_CTL_DMAC_EN            0x00008000
#define PRCM_CKG_CTL_I2C2_EN            0x00010000
// 0x00020000, Reserved.
#define PRCM_CKG_CTL_GPIO_EN            0x00040000
#define PRCM_CKG_CTL_BKSRAM_EN          0x00080000
#define PRCM_CKG_CTL_ALL_EN             0x000FFFFF


#endif // __HW_PRCM_H__
