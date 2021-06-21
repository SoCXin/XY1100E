//*****************************************************************************
//
// hw_memmap.h - Macros defining the memory map of Stellaris.
//
// Copyright (c) 2005-2011 Texas Instruments Incorporated.  All rights reserved.
// Software License Agreement
// 
// Texas Instruments (TI) is supplying this software for use solely and
// exclusively on TI's microcontroller products. The software is owned by
// TI and/or its suppliers, and is protected under applicable copyright
// laws. You may not combine this software with "viral" open-source
// software in order to form a larger program.
// 
// THIS SOFTWARE IS PROVIDED "AS IS" AND WITH ALL FAULTS.
// NO WARRANTIES, WHETHER EXPRESS, IMPLIED OR STATUTORY, INCLUDING, BUT
// NOT LIMITED TO, IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
// A PARTICULAR PURPOSE APPLY TO THIS SOFTWARE. TI SHALL NOT, UNDER ANY
// CIRCUMSTANCES, BE LIABLE FOR SPECIAL, INCIDENTAL, OR CONSEQUENTIAL
// DAMAGES, FOR ANY REASON WHATSOEVER.
// 
// This is part of revision 7611 of the Stellaris Firmware Development Package.
//
//*****************************************************************************

#ifndef __HW_MEMMAP_H__
#define __HW_MEMMAP_H__

#include <stdint.h>                         /* Include standard types */


#ifdef __cplusplus
    #define	__I	    volatile                /*!< defines 'read only' permissions      */
#else
    #define	__I     volatile const          /*!< defines 'read only' permissions      */
#endif
#define		__O     volatile                /*!< defines 'write only' permissions     */
#define		__IO    volatile                /*!< defines 'read / write' permissions   */

//*****************************************************************************
//
// The following are defines for the base address of the memories and
// peripherals.
//
//*****************************************************************************

#define PLATFORM_ARM_M3         1

#ifdef PLATFORM_ARM_M3
#define SYSRAM_BASE             0x20000000  // System RAM Base Addr
#define SEMA_BASE       	    0x25000000  // IPC-Semaphore
#define DMAC_BASE       	    0x25010000  // DMA Control
#define AES_DATA_IN_BASE	    0x26000000  // AES Data In
#define AES_DATA_OUT_BASE	    0x26000010  // AES Data Out
#define DFE_REG_BASE            0x2601F000  // DFE_Analog
#define QSPI_DATA_BASE          0x27000000  // QSPI Flash Data
#define RFPLL_CNTL_BASE			0x2601F0DF  // RFPLL Bus Base
#endif

#ifdef PLATFORM_DSP_F1
#define SYSRAM_BASE             0x50000000  // System RAM Base Addr
#define SEMA_BASE       	    0x80000000  // IPC-Semaphore
#define DMAC_BASE       	    0x80010000  // DMA Control
#define AES_DATA_IN_BASE        0x80020000  // AES Data In
#define AES_DATA_OUT_BASE       0x80020010  // AES Data Out
#define AES_DATA_DMA_IN_BASE    0x26000000  // AES Data In by DMA
#define AES_DATA_DMA_OUT_BASE   0x26000010  // AES Data Out by DMA
#define DFE_BUFFER_BASE         0x80030000  // DFE Buffer for DSP
#define DFE_REG_BASE            0x8003F000  // DFE Register for DSP
#define QSPI_DATA_BASE          0x51000000  // QSPI Flash Data
#define RFPLL_CNTL_BASE			0x8003F0DF  // RFPLL Bus Base
#endif

#define SYSRAM_SIZE             0x00080000  // System RAM Capacity Bytes
#define I2C1_BASE       	    0xA0000000  // IIC1
#define SPI_BASE        	    0xA0010000  // SPI
#define UART1_BASE			    0xA0020000  // UART1
#define QSPI_BASE			    0xA0030000  // QSPI Flash Control
#define WDT_BASE        	    0xA0040000  // Watch Dog
#define UTC_BASE        	    0xA0050000  // UTC
#define AON_BASE        	    0xA0058000  // AON PRCM
#define MEASURECNT_BASE         0xA0060000  // Measure Counter
#define TIMER3_BASE			    0xA0070000  // Timer3
#define TIMER4_BASE			    0xA0080000  // Timer4
#define CSP3_BASE       	    0xA0090000  // CSP3
#define CSP4_BASE       	    0xA00A0000  // CSP4
#define CSP2_BASE       	    0xA0100000  // CSP2
#define PRCM_BASE       	    0xA0110000  // Core PRCM
#define CSP1_BASE       	    0xA0120000  // CSP1
#define GPIO_BASE       	    0xA0140000  // GPIO
#define TIMER1_BASE			    0xA0150000  // Timer1
#define TIMER2_BASE			    0xA0160000  // Timer2
#define AES_BASE        	    0xA0170000  // AES
#define I2C2_BASE       	    0xA0180000  // IIC2
#define ADC_BASE                0xA0190000  // ADC
#define BBPLL_CNTL_BASE			0xA0110060  // BBPLL Bus Base


#define MEM_TYPE_UNKNOWN        0x00
#define MEM_TYPE_REG            0x01
#define MEM_TYPE_RAM            0x02
#define MEM_TYPE_FLASH          0x03

#define QSPI_FLASH_MAX_CAPACITY 0x01000000

/*------------- Universal Asynchronous Receiver Transmitter (UART) -----------*/
typedef struct
{
  __IO uint16_t CONTROL;     // 0x00 Control register [8:0] ua_cr R/W 0x00000128
       uint16_t RESERVED0;   // 0x02 -should not use
  __IO uint16_t MODE;        // 0x04 Mode register [11:0] ua_mr R/W 0x00000000
       uint16_t RESERVED1;   // 0x06 -should not use
  __O  uint16_t INT_ENA;     // 0x08 Interrupt enable register [9:0] ua_ier WO
       uint16_t RESERVED2;   // 0x0A -should not use
  __O  uint16_t INT_DIS;     // 0x0C Interrupt disable register [9:0] ua_idr WO
       uint16_t RESERVED3;   // 0x0E -should not use
  __I  uint16_t INT_MASK;    // 0x10 Interrupt mask register [9:0] ua_imr RO 0x00000000
       uint16_t RESERVED4;   // 0x12 -should not use
  __I  uint16_t INT_STAT;    // 0x14 Channel interrupt status register [9:0] ua_cisr Note 5 0x00000000
       uint16_t RESERVED5;   // 0x16 -should not use
  __IO uint16_t BAUD_RATE;   // 0x18 Baud rate generator register [15:0] ua_brgr R/W See Note 4
       uint16_t RESERVED6;   // 0x1A -should not use
  __IO uint16_t RX_TIMEOUT;  // 0x1C Receiver timeout register [7:0] ua_rtor R/W 0x00000000
       uint16_t RESERVED7;   // 0x1E -should not use
  __IO uint16_t RX_TRIGGER;  // 0x20 Receiver FIFO trigger level register [5:0] ua_rtrig R/W 0x00000020
       uint16_t RESERVED8;   // 0x22 -should not use
  __IO uint16_t MODEM_CTRL;  // 0x24 Modem control register [5:0] ua_mcr R/W 0x00000000
       uint16_t RESERVED9;   // 0x26 -should not use
  __I  uint16_t MODEM_STAT;  // 0x28 Modem status register [8:0] ua_msr RO See Note 1
       uint16_t RESERVED10;  // 0x2A -should not use
  __I  uint16_t CHAN_STAT;   // 0x2C Channel status register [12:0] ua_csr RO 0x00000000
       uint16_t RESERVED11;  // 0x2E -should not use
  __IO uint16_t FIFO_DATA;   // 0x30 Receive FIFO [15:0 or 7:0 See Note 2] ua_rfifo RO 0x00000000, 0x30 Transmit FIFO [15:0 or 7:0 See Note 3] ua_tfifo WO
       uint16_t RESERVED12;  // 0x32 -should not use
  __IO uint16_t BAUD_DIV;    // 0x34 Baud rate divider register [7:0] ua_bdiv R/W 0x0000000F
       uint16_t RESERVED13;  // 0x36 -should not use
  __IO uint16_t FLOW_DELAY;  // 0x38 Flow delay register [15:0] ua_fdel R/W 0x00000000
       uint16_t RESERVED14;  // 0x3A -should not use
  __IO uint16_t IRX_PW;      // 0x3C IR Minimum received pulse width [15:0] ua_pmin R/W See Note 4
       uint16_t RESERVED15;  // 0x3E -should not use
  __IO uint16_t ITX_PW;      // 0x40 IR transmitted pulse width [7:0] ua_pwid R/W See Note 4
       uint16_t RESERVED16;  // 0x42 -should not use
  __IO uint16_t TX_TRIGGER;  // 0x44 Transmitter FIFO trigger level [5:0] ua_ttrig R/W 0x00000020
       uint16_t RESERVED17;  // 0x46 -should not use
  __IO uint16_t XX_TRIGGER;  // 0x48 Trigger event for tb commands
       uint16_t RESERVED18;  // 0x4A -should not use
       uint32_t RESERVED19;  // 0x4C -should not use
  __IO uint16_t ABD_CTRL;    // 0x50 ABD control
       uint16_t RESERVED20;  // 0x52 -should not use
  __I  uint16_t ABD_STAT;    // 0x54 ABD status
       uint16_t RESERVED21;  // 0x56 -should not use
  __I  uint16_t ABD_TIMERL;  // 0x58 ABD timer low parts
       uint16_t RESERVED22;  // 0x5A -should not use
  __I  uint16_t ABD_TIMERH;  // 0x5C ABD timer high parts
       uint16_t RESERVED23;  // 0x5E -should not use
} NBIOT_UART_TypeDef;


/*------------- Quad SPI Flash Controller (QSPI) -----------*/
typedef struct
{
  __IO uint32_t qspi_config;               //0x0
  __IO uint32_t qspi_dev_rdinstr_config;   //0x4
  __IO uint32_t qspi_dev_wrinstr_config;   //0x8
  __IO uint32_t qspi_dev_delay;            //0xc
  __IO uint32_t qspi_read_data_capture;    //0x10
  __IO uint32_t qspi_dev_size_config;      //0x14
  __IO uint32_t qspi_sram_partition;       //0x18
  __IO uint32_t qspi_ind_ahb_trigger;      //0x1c
  __IO uint32_t qspi_periph_cfg;           //0x20
  __IO uint32_t qspi_remap_add;            //0x24
  __IO uint32_t qspi_mode_bit;             //0x28
  __I  uint32_t qspi_sram_fill_level;      //0x2c
  __IO uint32_t qspi_tx_thresh;            //0x30
  __IO uint32_t qspi_rx_thresh;            //0x34
  __IO uint32_t qspi_write_comp_ctrl;      //0x38
  __IO uint32_t qspi_max_no_of_polls;      //0x3c
  __IO uint32_t qspi_int_status;           //0x40
  __IO uint32_t qspi_int_mask;             //0x44
       uint32_t reserved0;                 //0x48
       uint32_t reserved1;                 //0x4c
  __IO uint32_t qspi_write_prot_l;         //0x50
  __IO uint32_t qspi_write_prot_u;         //0x54
  __IO uint32_t qspi_write_prot_ctrl;      //0x58
       uint32_t reserved2;                 //0x5c
  __IO uint32_t qspi_ind_rd_xfer_ctrl;     //0x60
  __IO uint32_t qspi_ind_rd_xfer_wmark;    //0x64
  __IO uint32_t qspi_ind_rd_xfer_start_add;//0x68
  __IO uint32_t qspi_ind_rd_xfer_num_bytes;//0x6c
  __IO uint32_t qspi_ind_wr_xfer_ctrl;     //0x70
  __IO uint32_t qspi_ind_wr_xfer_wmark;    //0x74
  __IO uint32_t qspi_ind_wr_xfer_start_add;//0x78
  __IO uint32_t qspi_ind_wr_xfer_num_bytes;//0x7c
  __IO uint32_t qspi_ind_ahb_trigger_range;//0x80
       uint32_t reserved3;                 //0x84
       uint32_t reserved4;                 //0x88
       uint32_t reserved5;                 //0x8c
  __IO uint32_t qspi_flash_cmd_ctrl;       //0x90
  __IO uint32_t qspi_flash_cmd_add;        //0x94
       uint32_t reserved6;                 //0x98
       uint32_t reserved7;                 //0x9c
  __I  uint32_t qspi_flash_cmd_rdata_l;    //0xa0
  __I  uint32_t qspi_flash_cmd_rdata_u;    //0xa4
  __IO uint32_t qspi_flash_cmd_wdata_l;    //0xa8
  __IO uint32_t qspi_flash_cmd_wdata_u;    //0xac
  __I  uint32_t qspi_flash_status;         //0xb0
       uint32_t reserved8;                 //0xb4
       uint32_t reserved9;                 //0xb8
       uint32_t reserved10;                //0xbc
       uint32_t reserved11;                //0xc0
       uint32_t reserved12;                //0xc4
       uint32_t reserved13;                //0xc8
       uint32_t reserved14;                //0xcc
       uint32_t reserved15;                //0xd0
       uint32_t reserved16;                //0xd4
       uint32_t reserved17;                //0xd8
       uint32_t reserved18;                //0xdc
       uint32_t reserved19;                //0xe0
       uint32_t reserved20;                //0xe4
       uint32_t reserved21;                //0xe8
       uint32_t reserved22;                //0xec
       uint32_t reserved23;                //0xf0
       uint32_t reserved24;                //0xf4
       uint32_t reserved25;                //0xf8
  __I  uint32_t qspi_module_id;            //0xfc
} NBIOT_QSPI_TypeDef;


/*------------- DMAC (DMAC) -----------*/
typedef struct {
  __IO uint32_t dmac_ch0_sa;       //0x0
  __IO uint32_t dmac_ch0_da;       //0x4
  __IO uint32_t dmac_ch0_ctrl;     //0x8
  __IO uint32_t dmac_ch0_tc;       //0xc
  __IO uint32_t dmac_ch0_np;       //0x10
       uint32_t reserved0;         //0x14
       uint32_t reserved1;         //0x18
       uint32_t reserved2;         //0x1c
  __IO uint32_t dmac_ch1_sa;       //0x20
  __IO uint32_t dmac_ch1_da;       //0x24
  __IO uint32_t dmac_ch1_ctrl;     //0x28
  __IO uint32_t dmac_ch1_tc;       //0x2c
  __IO uint32_t dmac_ch1_np;       //0x30
       uint32_t reserved3;         //0x34
       uint32_t reserved4;         //0x38
       uint32_t reserved5;         //0x3c
  __IO uint32_t dmac_int_status;   //0x40
  __IO uint32_t dmac_ch2_sa;       //0x60
  __IO uint32_t dmac_ch2_da;       //0x64
  __IO uint32_t dmac_ch2_ctrl;     //0x68
  __IO uint32_t dmac_ch2_tc;       //0x6c
  __IO uint32_t dmac_ch2_np;       //0x70
       uint32_t reserved6;         //0x74
       uint32_t reserved7;         //0x78
       uint32_t reserved8;         //0x7c
  __IO uint32_t dmac_ch3_sa;       //0x80
  __IO uint32_t dmac_ch3_da;       //0x84
  __IO uint32_t dmac_ch3_ctrl;     //0x88
  __IO uint32_t dmac_ch3_tc;       //0x8c
  __IO uint32_t dmac_ch3_np;       //0x90
       uint32_t reserved9;         //0x94
       uint32_t reserved10;        //0x98
       uint32_t reserved11;        //0x9c
} NBIOT_DMAC_TypeDef;


/*------------- TIMER1/2/3/4 -----------*/
typedef struct {
  __IO uint32_t timer_count;
  __IO uint32_t timer_rld;
  __IO uint32_t tomer_pwm;
  __IO uint32_t timer_ctl;
} NBIOT_TIMER_TypeDef;


#define NBIOT_UART		        ((NBIOT_UART_TypeDef    *) UART1_BASE	)
#define NBIOT_QSPI              ((NBIOT_QSPI_TypeDef    *) QSPI_BASE	)
#define NBIOT_DMAC              ((NBIOT_DMAC_TypeDef    *) DMAC_BASE	)
#define NBIOT_TIMER1            ((NBIOT_TIMER_TypeDef   *) TIMER1_BASE	)
#define NBIOT_TIMER2            ((NBIOT_TIMER_TypeDef   *) TIMER2_BASE	)
#define NBIOT_TIMER3            ((NBIOT_TIMER_TypeDef   *) TIMER3_BASE	)
#define NBIOT_TIMER4            ((NBIOT_TIMER_TypeDef   *) TIMER4_BASE	)


#endif // __HW_MEMMAP_H__
