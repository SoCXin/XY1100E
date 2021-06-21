#ifndef __SIM_CSP_H__
#define __SIM_CSP_H__

#include "xinyi_hardware.h"
#include "hw_csp.h"
#include "hw_prcm.h"
//#include "compiler.h"
#include "hw_memmap.h"
//#include "hw_sysctl.h"
#include "hw_types.h"
#include "debug.h"
//#include "interrupt.h"
#include "xinyi_hardware.h"

//typedef enum {DISABLE = 0, ENABLE = !DISABLE} FunctionalState;

#ifndef   __INLINE
  #define __INLINE                  inline
#endif


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
//=============================================================================================================
typedef struct _csp_regs {
  uint32_t mode1;              //0x0
  uint32_t mode2;              //0x4
  uint32_t tx_frame_ctrl;      //0x8
  uint32_t rx_frame_ctrl;      //0xc
  uint32_t tx_rx_enable;       //0x10
  uint32_t int_enable;         //0x14
  uint32_t int_status;          //0x18
  uint32_t pin_io_data;        //0x1c
  uint32_t reserve1;           //0x20
  uint32_t aysnc_param_reg;    //0x24
  uint32_t irda_x_mode_div;    //0x28
  uint32_t sm_cfg;             //0x2c
  uint32_t reserved2[52];
  uint32_t tx_dma_io_ctrl;     //0x100
  uint32_t tx_dma_io_len;      //0x104
  uint32_t tx_fifo_ctrl;       //0x108
  uint32_t tx_fifo_level_chk;  //0x10c
  uint32_t tx_fifo_op;         //0x110
  uint32_t tx_fifo_status;     //0x114
  uint32_t tx_fifo_data;       //0x118
  uint32_t reserved3;          //0x11c
  uint32_t rx_dma_io_ctrl;     //0x120
  uint32_t rx_dma_io_len;      //0x124
  uint32_t rx_fifo_ctrl;       //0x128
  uint32_t rx_fifo_level_chk;  //0x12c
  uint32_t rx_fifo_op;         //0x130
  uint32_t rx_fifo_status;     //0x134
  uint32_t rx_fifo_data;       //0x138
} csp_regs;

extern volatile csp_regs *csp1;

#define CSP_TFS_AS_GPIO   	   (1<<14)
#define CSP_INT_MASK_ALL     0x1FFF
#define CSP_INT_TX_DONE_MASK 0x02
#define CSP_SM_EN			 	     0x80
#define CSP_TX_IO_MODE   	   1
#define CSP_RX_IO_MODE   	   1
#define CSP_ENABLE		       (1<<5)
#define CSP_RX_EN	 		       1
#define CSP_TX_EN	 		       (1<<1)
#define CSP_TXFIFO_RESET     1
#define CSP_RXFIFO_RESET     1
#define CSP_LITTLE_ENDIAN    (0<<4)  //MSB first in tb
#define CSP_LITTLE_ENDIAN1   (1<<4)  //LSB
#define CSP_SMART_CARD       18
#define CSP_TXFIFO_RESET     1
#define CSP_RXFIFO_RESET     1
#define CSP_TXFIFO_START     (1<<1)
#define CSP_RXFIFO_START     (1<<1)
#define CSP_RXFIFO_EMPTY     (1<<8)
#define CSP_TXFIFO_EMPTY     (1<<8)
#define CSP_SM_RETRY         (0<<10)//(1<<10) //Disable retry
#define CSP_SM_CLK_STOP      (1<<8)
#define CSP_SCLK_PIN_MODE    (1<<12)
#define CSP_SCLK_PIN_VALUE   (1<<4)
#define CSP_SYNC_MODE         1
#define CSP_RX_DATA_EDGE_POS (0<<6)
#define CSP_TX_DATA_EDGE_NEG (1<<7)
#define CSP_ENA_AUTO_CLEAR   (1<<16)
#define CSP_FRM_CONTINUE     (1<<17)
#define CSP_TX_FIFO_WIDTH_WORD 2
#define CSP_RX_FIFO_WIDTH_WORD 2
#define CSP_TX_DONE          (1<<1)
#define CSP_TX_ALL_DONE      (1<<12)
#define DEBUG_BAUD_RATE      1200
//#define SYS_CLOCK            84400000
#define SYS_CLOCK           100000000
#define CSP_UART_1_STOP_BIT  1
#define CSP_CLOCK_SLAVE_MODE (1<<1)
#define CSP_RFS_SLAVE_MODE   (1<<19)
#define CSP_I2S_SYNC_CHG     (1<<21)
#define CSP_I2S_RFS_VALID_LEV (1<<8)
#define CSP_TFS_VALID_LEV    (1<<9)
//==========================================================================================================
//#define RESET 0
#define CSP_FLAG_TC 1
#define CSP_FLAG_RXNE 2
#define C_SAMPLE_DIV  24
#define C_CSP_SM_CLK_DIV  24 //20//6
#define CSP_SM_EN 0x80
//=================================================================================================
#define SIM_CSP_BASEADDR    			 CSP1_BASE
//=========================================================================================================
static __INLINE uint16_t SIM_GetFlagStatus( uint16_t CSP_FLAG)
{
	  if(CSP_FLAG == CSP_FLAG_TC )
		  return((uint16_t)HWREG(SIM_CSP_BASEADDR +  CSP_TX_FIFO_STATUS) & CSP_TX_FIFO_STATUS_FULL_Msk );
	  if (CSP_FLAG == CSP_FLAG_RXNE )
		  return((uint16_t)HWREG(SIM_CSP_BASEADDR +  CSP_RX_FIFO_STATUS) & CSP_RX_FIFO_STATUS_EMPTY_Msk);

	  return (uint16_t)0;
}

static __INLINE void SIM_SendOneByteData( uint8_t Data)
{
	  HWREG(SIM_CSP_BASEADDR +  CSP_TX_FIFO_DATA)=(Data & 0xFF);
	  return;
}

static __INLINE uint8_t SIM_ReceiveOneByteData(void)
{
  return (uint8_t)(HWREG(SIM_CSP_BASEADDR +  CSP_RX_FIFO_DATA) & 0xff);
}
static __INLINE void SIM_resetRxFifo(void)
{
	  HWREG(SIM_CSP_BASEADDR +  CSP_RX_FIFO_OP) = 0x1;
	  HWREG(SIM_CSP_BASEADDR +  CSP_RX_FIFO_OP) = 0x2;
	  return;
}
static __INLINE void SIM_resetTxFifo(void)
{
	  HWREG(SIM_CSP_BASEADDR +  CSP_TX_FIFO_OP) = 0x1;
	  HWREG(SIM_CSP_BASEADDR +  CSP_TX_FIFO_OP) = 0x2;
	  return;
}

static __INLINE void SIM_disableandenable(void)
{
	  HWREG(SIM_CSP_BASEADDR +  CSP_MODE1) &= ~(0x1 <<5);
	  HWREG(SIM_CSP_BASEADDR +  CSP_MODE1) |= (0x1 <<5);
	  return;
}
//===========================================================================================================
extern void SIM_SmartCardCmd(FunctionalState NewState);
extern void SIM_ITConfig( uint16_t CSP_IT,FunctionalState NewState); //(CSP6, CSP_IT_PE, ENABLE)
//extern void SIM_SendOneByteData( uint8_t Data);
extern void SIM_SmartCard_BD( int F_div_D,int CSP_sm_clk_div);
extern void SIM_SmartCard_WT(int wt_time);
extern void SIM_TxAllOut();
extern void SIM_SetGuardTime( uint8_t CSP_GuardTime);

#ifdef __cplusplus
}
#endif

#endif //  __CSP_H__

