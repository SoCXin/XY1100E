#ifndef __HW_SPI_H__
#define __HW_SPI_H__

//*****************************************************************************
//
// The following are defines for the SPI register offsets.
//
//*****************************************************************************
#define SPI_CONFIG              0x00000000  // RW 0x000 Configuration
#define SPI_STATUS              0x00000004  // RO 0x004 Interrupt Status
#define SPI_IEN                 0x00000008  // WO - Interrupt Enable.
#define SPI_IDIS                0x0000000C  // WO - Interrupt Disable
#define SPI_IMASK               0x00000010  // RO 0x0 Interrupt Mask
#define SPI_ENABLE              0x00000014  // RW 0x0 SPI Enable
#define SPI_DELAY               0x00000018  // RW 0x0 Delay Register
#define SPI_TXD                 0x0000001C  // WO - Transmit Data(TX FIFO)
#define SPI_RXD                 0x00000020  // RO 0x0 Receive Data(RX FIFO)
#define SPI_SIC                 0x00000024  // RW 0x0FF Slave Idle Count Register
#define SPI_TX_THRESH           0x00000028  // RW 0x01 TX FIFO Threshold level
#define SPI_RX_THRESH           0x0000002C  // RW 0x01 RX FIFO Threshold level
#define SPI_TX_FIFO_OP          0x00000030  // RW 0x02 TX FIFO Operation
#define SPI_RX_FIFO_OP          0x00000034  // RW 0x02 RX FIFO Operation
#define SPI_TX_FIFO_STATUS      0x00000038  // RW 0x20 TX FIFO Status
#define SPI_RX_FIFO_STATUS      0x0000003C  // RW 0x20 RX FIFO Status
#define SPI_MOD_ID              0x000000FC  // RO 0x01090100 Module ID register

//*****************************************************************************
//
// The following are defines for the bit fields in the SPI_CONFIG register.
//
//*****************************************************************************
#define SPI_CONFIG_MODE_Pos             0UL
#define SPI_CONFIG_MODE_Msk             (1UL << SPI_CONFIG_MODE_Pos)
#define SPI_CONFIG_MODE_MASTER          (1UL << SPI_CONFIG_MODE_Pos)
#define SPI_CONFIG_MODE_SLAVE           (0UL << SPI_CONFIG_MODE_Pos)

#define SPI_CONFIG_CPOL_Pos             1UL
#define SPI_CONFIG_CPOL_Msk             (1UL << SPI_CONFIG_CPOL_Pos)
#define SPI_CONFIG_CPOL_HIGH            (1UL << SPI_CONFIG_CPOL_Pos)
#define SPI_CONFIG_CPOL_LOW             (0UL << SPI_CONFIG_CPOL_Pos)

#define SPI_CONFIG_CPHA_Pos             2UL
#define SPI_CONFIG_CPHA_Msk             (1UL << SPI_CONFIG_CPHA_Pos)
#define SPI_CONFIG_CPHA_ACTIVE          (0UL << SPI_CONFIG_CPHA_Pos)
#define SPI_CONFIG_CPHA_INACTIVE        (1UL << SPI_CONFIG_CPHA_Pos)

#define SPI_CONFIG_CLK_DIV_Pos          3UL
#define SPI_CONFIG_CLK_DIV_Msk          (7UL << SPI_CONFIG_CLK_DIV_Pos)
#define SPI_CONFIG_CLK_DIV_2            (0UL << SPI_CONFIG_CLK_DIV_Pos)
#define SPI_CONFIG_CLK_DIV_4            (1UL << SPI_CONFIG_CLK_DIV_Pos)
#define SPI_CONFIG_CLK_DIV_8            (2UL << SPI_CONFIG_CLK_DIV_Pos)
#define SPI_CONFIG_CLK_DIV_16           (3UL << SPI_CONFIG_CLK_DIV_Pos)
#define SPI_CONFIG_CLK_DIV_32           (4UL << SPI_CONFIG_CLK_DIV_Pos)
#define SPI_CONFIG_CLK_DIV_64           (5UL << SPI_CONFIG_CLK_DIV_Pos)
#define SPI_CONFIG_CLK_DIV_128          (6UL << SPI_CONFIG_CLK_DIV_Pos)
#define SPI_CONFIG_CLK_DIV_256          (7UL << SPI_CONFIG_CLK_DIV_Pos)

#define SPI_CONFIG_WORD_SIZE_Pos        6UL
#define SPI_CONFIG_WORD_SIZE_Msk        (3UL << SPI_CONFIG_WORD_SIZE_Pos)
#define SPI_CONFIG_WORD_SIZE_BITS_8     (0UL << SPI_CONFIG_WORD_SIZE_Pos)
#define SPI_CONFIG_WORD_SIZE_BITS_16    (1UL << SPI_CONFIG_WORD_SIZE_Pos)
#define SPI_CONFIG_WORD_SIZE_BITS_24    (2UL << SPI_CONFIG_WORD_SIZE_Pos)
#define SPI_CONFIG_WORD_SIZE_BITS_32    (3UL << SPI_CONFIG_WORD_SIZE_Pos)

#define SPI_CONFIG_CLK_SEL_Pos          8UL
#define SPI_CONFIG_CLK_SEL_Msk          (1UL << SPI_CONFIG_CLK_SEL_Pos)
#define SPI_CONFIG_CLK_SEL_SPI_REF      (0UL << SPI_CONFIG_CLK_SEL_Pos)
#define SPI_CONFIG_CLK_SEL_EXT          (1UL << SPI_CONFIG_CLK_SEL_Pos)

#define SPI_CONFIG_CS_DEC_Pos           9UL
#define SPI_CONFIG_CS_DEC_Msk           (1UL << SPI_CONFIG_CS_DEC_Pos)
#define SPI_CONFIG_CS_DEC_NORMAL        (0UL << SPI_CONFIG_CS_DEC_Pos)
#define SPI_CONFIG_CS_DEC_EXT           (1UL << SPI_CONFIG_CS_DEC_Pos)

#define SPI_CONFIG_CS_LINES_Pos         10UL
#define SPI_CONFIG_CS_LINES_Msk         (15UL << SPI_CONFIG_CS_LINES_Pos)

#define SPI_CONFIG_MANUAL_CS_Pos        14UL
#define SPI_CONFIG_MANUAL_CS_Msk        (1UL << SPI_CONFIG_MANUAL_CS_Pos)

#define SPI_CONFIG_MANUAL_START_EN_Pos  15UL
#define SPI_CONFIG_MANUAL_START_EN_Msk  (1UL << SPI_CONFIG_MANUAL_START_EN_Pos)

#define SPI_CONFIG_MANUAL_START_CMD_Pos 16UL
#define SPI_CONFIG_MANUAL_START_CMD_Msk (1UL << SPI_CONFIG_MANUAL_START_CMD_Pos)

#define SPI_CONFIG_MODE_FAIL_GEN_Pos    17UL
#define SPI_CONFIG_MODE_FAIL_GEN_Msk    (1UL << SPI_CONFIG_MODE_FAIL_GEN_Pos)


//*****************************************************************************
//
// The following are defines for the bit fields in the SPI_STATUS register.
// Also for SPI_IEN/SPI_IDIS/SPI_IMASK registers.
//*****************************************************************************
#define SPI_INT_RX_OVERFLOW_Pos         0UL
#define SPI_INT_RX_OVERFLOW             (1UL << SPI_INT_RX_OVERFLOW_Pos)

#define SPI_INT_MODE_FAIL_Pos           1UL
#define SPI_INT_MODE_FAIL               (1UL << SPI_INT_MODE_FAIL_Pos)

#define SPI_INT_TX_FIFO_NF_Pos          2UL
#define SPI_INT_TX_FIFO_NF              (1UL << SPI_INT_TX_FIFO_NF_Pos)

#define SPI_INT_TX_FIFO_FULL_Pos        3UL
#define SPI_INT_TX_FIFO_FULL            (1UL << SPI_INT_TX_FIFO_FULL_Pos)

#define SPI_INT_RX_FIFO_NE_Pos          4UL
#define SPI_INT_RX_FIFO_NE              (1UL << SPI_INT_RX_FIFO_NE_Pos)

#define SPI_INT_RX_FIFO_FULL_Pos        5UL
#define SPI_INT_RX_FIFO_FULL            (1UL << SPI_INT_RX_FIFO_FULL_Pos)

#define SPI_INT_TX_FIFO_UNDERFLOW_Pos   6UL
#define SPI_INT_TX_FIFO_UNDERFLOW       (1UL << SPI_INT_TX_FIFO_UNDERFLOW_Pos)

#define SPI_INT_ALL                     0x7F

//*****************************************************************************
//
// The following are defines for the bit fields in the SPI_DELAY register.
//
//*****************************************************************************
#define SPI_DELAY_INIT_Pos              0UL
#define SPI_DELAY_INIT_Msk              (0xFFUL << SPI_DELAY_INIT_Pos)

#define SPI_DELAY_AFTER_Pos             8UL
#define SPI_DELAY_AFTER_Msk             (0xFFUL << SPI_DELAY_AFTER_Pos)

#define SPI_DELAY_BTWN_Pos              16UL
#define SPI_DELAY_BTWN_Msk              (0xFFUL << SPI_DELAY_BTWN_Pos)

#define SPI_DELAY_NSS_Pos               24UL
#define SPI_DELAY_NSS_Msk               (0xFFUL << SPI_DELAY_NSS_Pos)


//*****************************************************************************
//
// The following are defines for the bit fields in the FIFO_OP register.
//
//*****************************************************************************
#define SPI_FIFO_OP_START_Pos           0UL
#define SPI_FIFO_OP_START_Msk           1UL

#define SPI_FIFO_OP_RESET_Pos           1UL
#define SPI_FIFO_OP_RESET_Msk           2UL


//*****************************************************************************
//
// The following are defines for the bit fields in the FIFO_STATUS register.
//
//*****************************************************************************
#define SPI_FIFO_STATUS_DATALEN_Pos     0UL
#define SPI_FIFO_STATUS_DATALEN_Msk     0xFFUL

#define SPI_FIFO_STATUS_FULL_Pos        8UL
#define SPI_FIFO_STATUS_FULL_Msk        0x100UL

#define SPI_FIFO_STATUS_EMPTY_Pos       9UL
#define SPI_FIFO_STATUS_EMPTY_Msk       0x200UL

#endif // __HW_SPI_H__
