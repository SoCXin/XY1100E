#ifndef __HW_UART_H__
#define __HW_UART_H__

//*****************************************************************************
//
// The following are defines for the UART register offsets.
//
//*****************************************************************************
#define UART_CONTROL	    0x00000000    // Control register [8:0] ua_cr R/W 0x00000128
#define UART_MODE	        0x00000004    // Mode register [11:0] ua_mr R/W 0x00000000
#define UART_INT_ENA        0x00000008    // Interrupt enable register [9:0] ua_ier WO
#define UART_INT_DIS        0x0000000C    // Interrupt disable register [9:0] ua_idr WO
#define UART_INT_MASK       0x00000010    // Interrupt mask register [9:0] ua_imr RO 0x00000000
#define UART_INT_STAT       0x00000014    // Channel interrupt status register [9:0] ua_cisr Note 5 0x00000000
#define UART_BAUD_RATE      0x00000018    // Baud rate generator register [15:0] ua_brgr R/W See Note 4
#define UART_RX_TIMEOUT     0x0000001C    // Receiver timeout register [7:0] ua_rtor R/W 0x00000000
#define UART_RX_TRIGGER     0x00000020    // Receiver FIFO trigger level register [5:0] ua_rtrig R/W 0x00000020
#define UART_MODEM_CTRL     0x00000024    // Modem control register [5:0] ua_mcr R/W 0x00000000
#define UART_MODEM_STAT     0x00000028    // Modem status register [8:0] ua_msr RO See Note 1
#define UART_CHAN_STAT      0x0000002C    // Channel status register [12:0] ua_csr RO 0x00000000
#define UART_FIFO_DATA      0x00000030    // Receive FIFO [15:0 or 7:0 See Note 2] ua_rfifo RO 0x00000000, 0x30 Transmit FIFO [15:0 or 7:0 See Note 3] ua_tfifo WO
#define UART_BAUD_DIV       0x00000034    // Baud rate divider register [7:0] ua_bdiv R/W 0x0000000F
#define UART_FLOW_DELAY     0x00000038    // Flow delay register [15:0] ua_fdel R/W 0x00000000
#define UART_IRX_PW         0x0000003C    // IR Minimum received pulse width [15:0] ua_pmin R/W See Note 4
#define UART_ITX_PW         0x00000040    // IR transmitted pulse width [7:0] ua_pwid R/W See Note 4
#define UART_TX_TRIGGER     0x00000044    // Transmitter FIFO trigger level [5:0] ua_ttrig R/W 0x00000020
#define UART_XX_TRIGGER     0x00000048    // Trigger event for tb commands
#define UART_ABD_CTRL       0x00000050    // ABD control
#define UART_ABD_STAT       0x00000054    // ABD status
#define UART_ABD_TIMERL     0x00000058    // ABD timer low parts
#define UART_ABD_TIMERH     0x0000005C    // ABD timer high parts

//*****************************************************************************
//
// The following are defines for the bit fields in the UART_O_DR register.
//
//*****************************************************************************
#define UART_TX_FIFO_BYTES	64
#define UART_RX_FIFO_BYTES	64


/****** UART Control Codes *****/

#define UART_CONTROL_Pos                0
#define UART_CONTROL_Msk               (0x1FFUL << UART_CONTROL_Pos)

/*----- UART Control Codes: Miscellaneous Controls  -----*/
#define UART_CONTROL_TX_EN               (0x4UL << UART_CONTROL_Pos)    ///< Transmitter; arg: 1=enabled
#define UART_CONTROL_TX_DIS              (0x5UL << UART_CONTROL_Pos)    ///< Transmitter; arg: 1=disabled
#define UART_CONTROL_RX_EN               (0x2UL << UART_CONTROL_Pos)    ///< Receiver; arg: 1=enabled
#define UART_CONTROL_RX_DIS              (0x3UL << UART_CONTROL_Pos)    ///< Receiver; arg: 1=disabled
#define UART_CONTROL_BREAK_START         (0x7UL << UART_CONTROL_Pos)    ///< Continuous Break transmission; arg: 1=enabled
#define UART_CONTROL_BREAK_STOP          (0x8UL << UART_CONTROL_Pos)    ///< Continuous Break transmission; arg: 1=disabled
#define UART_ABORT_SEND                  (0x1UL << UART_CONTROL_Pos)    ///< Abort \ref UART_Send
#define UART_ABORT_RECEIVE               (0x0UL << UART_CONTROL_Pos)    ///< Abort \ref UART_Receive
//#define UART_ABORT_TRANSFER            (0x1AUL << UART_CONTROL_Pos)   ///< Abort \ref UART_Transfer

/*----- UART Control Codes: Mode Parameters: Data Bits -----*/
#define UART_MODE_DATA_BITS_Pos              1
#define UART_MODE_DATA_BITS_Msk             (3UL << UART_MODE_DATA_BITS_Pos)
#define UART_MODE_DATA_BITS_6               (3UL << UART_MODE_DATA_BITS_Pos)    ///< 6 Data bit
#define UART_MODE_DATA_BITS_7               (2UL << UART_MODE_DATA_BITS_Pos)    ///< 7 Data bits
#define UART_MODE_DATA_BITS_8               (0UL << UART_MODE_DATA_BITS_Pos)    ///< 8 Data bits (default)

/*----- UART Control Codes: Mode Parameters: Parity -----*/
#define UART_MODE_PARITY_Pos                 3
#define UART_MODE_PARITY_Msk                (7UL << UART_MODE_PARITY_Pos)
#define UART_MODE_PARITY_NONE               (4UL << UART_MODE_PARITY_Pos)       ///< No Parity (default)
#define UART_MODE_PARITY_EVEN               (0UL << UART_MODE_PARITY_Pos)       ///< Even Parity
#define UART_MODE_PARITY_ODD                (1UL << UART_MODE_PARITY_Pos)       ///< Odd Parity
#define UART_MODE_PARITY_ZERO		        (2UL << UART_MODE_PARITY_Pos)       ///< Force 0
#define UART_MODE_PARITY_ONE		        (3UL << UART_MODE_PARITY_Pos)       ///< Force 1

/*----- UART Control Codes: Mode Parameters: Stop Bits -----*/
#define UART_MODE_STOP_BITS_Pos              6
#define UART_MODE_STOP_BITS_Msk             (3UL << UART_MODE_STOP_BITS_Pos)
#define UART_MODE_STOP_BITS_1               (0UL << UART_MODE_STOP_BITS_Pos)    ///< 1 Stop bit (default)
#define UART_MODE_STOP_BITS_1_5             (1UL << UART_MODE_STOP_BITS_Pos)    ///< 1.5 Stop bits
#define UART_MODE_STOP_BITS_2               (2UL << UART_MODE_STOP_BITS_Pos)    ///< 2 Stop bits

/*----- UART Control Codes: Mode Parameters: Channel Mode Bits -----*/
#define UART_MODE_CHANNEL_MODE_BITS_Pos      8
#define UART_MODE_CHANNEL_MODE_BITS_Msk     (3UL << UART_MODE_CHANNEL_MODE_BITS_Pos)
#define UART_MODE_CHANNEL_MODE_NORMAL       (0UL << UART_MODE_CHANNEL_MODE_BITS_Pos)
#define UART_MODE_CHANNEL_MODE_AUTO_ECHO    (1UL << UART_MODE_CHANNEL_MODE_BITS_Pos)
#define UART_MODE_CHANNEL_MODE_LOCAL_BACK   (2UL << UART_MODE_CHANNEL_MODE_BITS_Pos)
#define UART_MODE_CHANNEL_MODE_REMOTE_BACK  (3UL << UART_MODE_CHANNEL_MODE_BITS_Pos)

/*----- UART Control Codes: Mode Parameters: Clock Control Bits -----*/
#define UART_MODE_SEL_CLOCK_Pos		 0
#define UART_MODE_SEL_CLOCK_UART	(0UL << UART_MODE_SEL_CLOCK_Pos)   ///< The sel_clk is sourced from uart_clk
#define UART_MODE_SEL_CLOCK_UART_DIV_8	(1UL << UART_MODE_SEL_CLOCK_Pos)   ///< The sel_clk is sourced from uart_clk/8

#define UART_Baudrate_CD_Pos	     0
#define UART_Baudrate_CD_Msk	    (0xFFFFUL << UART_Baudrate_CD_Pos)   ///< Baud_Sample = sel_clk/CD
#define UART_Baudrate_CD_DIS	    (0x0000UL << UART_Baudrate_CD_Pos)   ///< Disables Baud_Sample
#define UART_Baudrate_CD_BYPASS     (0x0001UL << UART_Baudrate_CD_Pos)   ///< Clock divisor bypass (Baud_Sample = sel_clk)

#define UART_Baudrate_BDIV_Pos	      0
#define UART_Baudrate_BDIV_Msk	     (0xFF << UART_Baudrate_BDIV_Pos)	///< 4 to 255 : Baud rate = (sel_clk/((BDIV+1)*CD)), 0 to 3 is ingored


/*----- UART Control Codes: Mode Parameters: Default UART or IrDA Mode -----*/
#define UART_MODE_BITS_Pos              11
#define UART_MODE_ASYNCHRONOUS         (0x0UL << UART_MODE_BITS_Pos)   ///< UART (Asynchronous); arg = Baudrate
#define UART_MODE_IRDA                 (0x1UL << UART_MODE_BITS_Pos)   ///< UART IrDA; arg = Baudrate


// UART flags
#define UART_FLAG_INITIALIZED       (1U << 0)
#define UART_FLAG_POWERED           (1U << 1)
#define UART_FLAG_CONFIGURED        (1U << 2)
#define UART_FLAG_TX_ENABLED        (1U << 3)
#define UART_FLAG_RX_ENABLED        (1U << 4)
#define UART_FLAG_SEND_ACTIVE       (1U << 5)


// UART Control Register
#define UART_CTRL_RXRES  0x0001  // Software reset for RX data path. When set to 1, the receiver logic is reset and all pending receiver data is discarded. This bit is self clearing once the reset has completed.
#define UART_CTRL_TXRES  0x0002  // Software reset for TX data path. When set to 1, the transmitter logic is reset and all pending transmitter data is discarded. This bit is self clearing once the reset has completed.
#define UART_CTRL_RXEN   0x0004  // Receive enable. When set to 1, the receiver is enabled, provided the RXDIS field is set to 0.
#define UART_CTRL_RXDIS  0x0008  // Receive disable. When set to 1, the receiver is disabled, regardless of the value of RXEN.
#define UART_CTRL_TXEN   0x0010  // Transmit enable. When set to 1, the transmitter is enabled, provided the TXDIS field is set to 0.
#define UART_CTRL_TXDIS  0x0020  // Transmit disable. When set to 1, the transmitter is disabled, regardless of the value of TXEN.
#define UART_CTRL_RSTTO  0x0040  // Restart receiver timeout counter. When set to 1, the receiver timeout counter is restarted. This bit is self clearing once the restart has completed..
#define UART_CTRL_STTBRK 0x0080  // Start transmitter break. When set to 1, while a break is not being transmitted, this bit will start to transmit a break after the characters currently present in FIFO and the transmit shift register have been transmitted. Can only be set if STPBRK is not high.
#define UART_CTRL_STPBRK 0x0100  // Stop transmitter break. When set to 1, while a break is being transmitted, this bit will stop transmission of the break after a minimum of one character length and transmit a high level during 12 bit periods. Can be set regardless of the value of STTBRK.

// UART Mode Register
#define UART_MODE_CLKS         0x0001  // sel_clk clock source select. This field defines whether a pre-scalar of 8 is applied to the baud rate generator input clock.
                                        // 0 : the sel_clk is sourced from uart_clk.
                                        // 1 : the sel_clk is sourced from uart_clk/8.
#define UART_MODE_CHRL_MASK    0x0006  // Character length select. Defines the number of bits in each character.
#define UART_MODE_CHRL_6BITS   0x0006  // Character length select. Defines the number of bits in each character.
#define UART_MODE_CHRL_7BITS   0x0004  // Character length select. Defines the number of bits in each character.
#define UART_MODE_CHRL_8BITS   0x0000  // Character length select. Defines the number of bits in each character.
#define UART_MODE_PAR_MASK     0x0038  // Parity type select for rx and tx
#define UART_MODE_PAR_EVEN     0x0000  // 000 : even parity
#define UART_MODE_PAR_ODD      0x0008  // 001 : odd parity
#define UART_MODE_PAR_SPACE    0x0010  // 010 : forced to 0 parity (space)
#define UART_MODE_PAR_MARK     0x0018  // 011 : forced to 1 parity (mark)
#define UART_MODE_PAR_NONE     0x0020  // 1XX : no parity set
#define UART_MODE_NBSTOP_MASK  0x00c0  // Number of stop bits. 
#define UART_MODE_NBSTOP_ONE   0x0000  // 00 : 1 stop bit
#define UART_MODE_NBSTOP_ONE5  0x0040  // 01 : 1.5 stop bits
#define UART_MODE_NBSTOP_TWO   0x0080  // 10 : 2 stop bits
#define UART_MODE_CHMODE_MASK  0x0300  // Channel mode. Defines the mode of operation of the UART.
#define UART_MODE_CHMODE_NORML 0x0000  // 00 : normal
#define UART_MODE_CHMODE_AECHO 0x0100  // 01 : automatic echo
#define UART_MODE_CHMODE_LLOOP 0x0200  // 10 : local loopback
#define UART_MODE_CHMODE_RLOOP 0x0300  // 11 : remote loopback
#define UART_MODE_UCLKEN       0x0400  // External uart_clk source select. This field directly drives the output ua_uclken, which is to used within the ASIC clock control circuit to select uart_clk input as:
                                        // 0 : APB clock, pclk
                                        // 1 : a user-defined clock
#define UART_MODE_IRMODE       0x0800  // Enable IrDA mode
                                        // 0 : Default UART mode
                                        // 1 : Enable IrDA mode

// UART Interrupt Enable/Disable/Mask/Channel Register
#define UART_INT_TOVR     0x1000 // When set to 1, the Transmitter FIFO Overflow interrupt is enabled.
#define UART_INT_TNFUL    0x0800 // When set to 1, the Transmitter FIFO Nearly Full interrupt is enabled.
#define UART_INT_TTRIG    0x0400 // When set to 1, the Transmitter FIFO Trigger interrupt is enabled.
#define UART_INT_DMSI     0x0200 // When set to 1, the Delta Modem Status Indicator interrupt is enabled.
#define UART_INT_TIMEOUT  0x0100 // When set to 1, the Receiver Timeout Error interrupt is enabled.
#define UART_INT_PARE     0x0080 // When set to 1, the Receiver Parity Error interrupt is enabled.
#define UART_INT_FRAME    0x0040 // When set to 1, the Receiver Framing Error interrupt is enabled.
#define UART_INT_ROVR     0x0020 // When set to 1, the Receiver Overflow Error interrupt is enabled.
#define UART_INT_TFUL     0x0010 // When set to 1, the Transmitter FIFO Full interrupt is enabled.
#define UART_INT_TEMPTY   0x0008 // When set to 1, the Transmitter FIFO Empty interrupt is enabled.
#define UART_INT_RFUL     0x0004 // When set to 1, the Receiver FIFO Full interrupt is enabled.
#define UART_INT_REMPTY   0x0002 // When set to 1, the Receiver FIFO Empty interrupt is enabled.
#define UART_INT_RTRIG    0x0001 // When set to 1, the Receiver FIFO Trigger interrupt is enabled.
#define UART_INT_ALL      0x1FFF 

// UART Channel Status Register
#define UART_CSR_TNFUL   0x4000 // Transmitter FIFO Nearly Full continuous status.
                                 // 0: More than one byte is unused in the TX FIFO
                                 // 1: Only one byte is free in the TX FIFO
#define UART_CSR_TTRIG   0x2000 // Transmitter FIFO Trigger continuous status.
                                 // 0 : TX FIFO fill level is less than TTRIG
                                 // 1 : TX FIFO fill level is greater than or equal to TTRIG
#define UART_CSR_FDELT   0x1000 // Receiver flow delay trigger continuous status.
                                 // 0 : RX FIFO fill level is less than FDEL
                                 // 1 : RX FIFO fill level is greater than or equal to FDEL
#define UART_CSR_TACTIVE 0x0800 // Transmitter state machine is active.
                                 // 0 : Transmitter state machine is in an inactive state
                                 // 1 : Transmitter state machine is in an active state
#define UART_CSR_RACTIVE 0x0400 // Receiver state machine is active.
                                 // 0 : Receiver state machine is in an inactive state
                                 // 1 : Receiver state machine is in an active state
#define UART_CSR_DMSI    0x0200 // Delta Modem Status Indicator status. 
#define UART_CSR_TIMEOUT 0x0100 // Receiver Timeout status.
#define UART_CSR_PARE    0x0080 // Receiver Parity Error status.
#define UART_CSR_FRAME   0x0040 // Receiver Frame Error status.
#define UART_CSR_ROVR    0x0020 // Receiver Overflow Error status.
#define UART_CSR_TFUL    0x0010 // Transmitter FIFO Full continuous status.
                                 // 0 : TX FIFO is not full
                                 // 1 : TX FIFO is full
#define UART_CSR_TEMPTY  0x0008 // Transmitter FIFO Empty continuous status.
                                 // 0 : TX FIFO is not empty
                                 // 1 : TX FIFO is empty
#define UART_CSR_RFUL    0x0004 // Receiver FIFO Full continuous status.
                                 // 0 : RX FIFO is not full
                                 // 1 : RX FIFO is full
#define UART_CSR_REMPTY  0x0002 // Receiver FIFO Full continuous status.
                                 // 0 : RX FIFO is not empty
                                 // 1 : RX FIFO is empty
#define UART_CSR_RTRIG   0x0001 // Receiver FIFO Trigger continuous status.
                                 // 0 : RX FIFO fill level is less than RTRIG
                                 // 1 : RX FIFO fill level is greater than or equal to RTRIG

#define UART_ABD_END     0x0001 // It is cleared when abd_ena is set to 1; it is set by hardware when a rising edge is detected in the UART RXD line.
				 // 0 : ABD is working. Do not read timer value while this value is 0
				 // 1 : ABD is idle or finished. Timer value has been updated.


#endif // __HW_UART_H__
