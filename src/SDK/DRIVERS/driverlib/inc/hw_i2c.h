#ifndef __HW_I2C_H__
#define __HW_I2C_H__

//*****************************************************************************
//
// The following are defines for the I2C register offsets.
//
//*****************************************************************************
#define I2C_CTL                 0x00000000
#define I2C_STAT                0x00000004
#define I2C_ADDR                0x00000008
#define I2C_DATA                0x0000000C
#define I2C_INT_STAT            0x00000010
#define I2C_TRANS_SIZE          0x00000014
#define I2C_SLAVE_MONITOR       0x00000018
#define I2C_TIMEOUT             0x0000001C
#define I2C_INT_MASK            0x00000020
#define I2C_INT_EN              0x00000024
#define I2C_INT_DIS             0x00000028


//*****************************************************************************
//
// The following are defines for the bit fields in the I2C_CTL register.
//
//*****************************************************************************
#define I2C_CTL_M_RW_Pos        0
#define I2C_CTL_M_RW_Msk        (1UL << I2C_CTL_M_RW_Pos)
#define I2C_CTL_M_READ          (1UL << I2C_CTL_M_RW_Pos)
#define I2C_CTL_M_WRITE         (0UL << I2C_CTL_M_RW_Pos)

#define I2C_CTL_MS_Pos          1
#define I2C_CTL_MS_Msk          (1UL << I2C_CTL_MS_Pos)
#define I2C_CTL_MASTER          (1UL << I2C_CTL_MS_Pos)
#define I2C_CTL_SLAVE           (0UL << I2C_CTL_MS_Pos)

#define I2C_CTL_NEA_Pos         2
#define I2C_CTL_NEA_Msk         (1UL << I2C_CTL_NEA_Pos)
#define I2C_CTL_NEA_N           (1UL << I2C_CTL_NEA_Pos)
#define I2C_CTL_NEA_E           (0UL << I2C_CTL_NEA_Pos)

#define I2C_CTL_ACKEN_Pos       3
#define I2C_CTL_ACKEN_Msk       (1UL << I2C_CTL_ACKEN_Pos)
#define I2C_CTL_ACKEN_EN        (1UL << I2C_CTL_ACKEN_Pos)
#define I2C_CTL_ACKEN_DIS       (0UL << I2C_CTL_ACKEN_Pos)

#define I2C_CTL_HOLD_Pos        4
#define I2C_CTL_HOLD_Msk        (1UL << I2C_CTL_HOLD_Pos)

#define I2C_CTL_SLVMON_Pos      5
#define I2C_CTL_SLVMON_Msk      (1UL << I2C_CTL_SLVMON_Pos)
#define I2C_CTL_SLVMON_EN       (1UL << I2C_CTL_SLVMON_Pos)
#define I2C_CTL_SLVMON_DIS      (0UL << I2C_CTL_SLVMON_Pos)

#define I2C_CTL_CLR_FIFO_Pos    6
#define I2C_CTL_CLR_FIFO_Msk    (1UL << I2C_CTL_CLR_FIFO_Pos)
#define I2C_CTL_CLR_FIFO_EN     (1UL << I2C_CTL_CLR_FIFO_Pos)

#define I2C_CTL_DIVISOR_B_Pos   8
#define I2C_CTL_DIVISOR_B_Msk   (0x3FUL << I2C_CTL_DIVISOR_B_Pos)

#define I2C_CTL_DIVISOR_A_Pos   14
#define I2C_CTL_DIVISOR_A_Msk   (0x3UL << I2C_CTL_DIVISOR_A_Pos)

#define I2C_CTL_DIVISOR_Msk     (0xFFUL << I2C_CTL_DIVISOR_B_Pos)

//*****************************************************************************
//
// The following are defines for the bit fields in the I2C_STAT register.
//
//*****************************************************************************
#define I2C_STAT_RXRW_Pos       3
#define I2C_STAT_RXRW_Msk       (1UL << I2C_STAT_RXRW_Pos)

#define I2C_STAT_RXDV_Pos       5
#define I2C_STAT_RXDV_Msk       (1UL << I2C_STAT_RXDV_Pos)

#define I2C_STAT_TXDV_Pos       6
#define I2C_STAT_TXDV_Msk       (1UL << I2C_STAT_TXDV_Pos)

#define I2C_STAT_RXOVF_Pos      7
#define I2C_STAT_RXOVF_Msk      (1UL << I2C_STAT_RXOVF_Pos)

#define I2C_STAT_BA_Pos         8
#define I2C_STAT_BA_Msk         (1UL << I2C_STAT_BA_Pos)


//*****************************************************************************
//
// The following are defines for the bit fields in the I2C_INT register.
//
//*****************************************************************************
#define I2C_INT_COMP            0x00000001
#define I2C_INT_DATA            0x00000002
#define I2C_INT_NACK            0x00000004
#define I2C_INT_TO              0x00000008
#define I2C_INT_SLV_RDY         0x00000010
#define I2C_INT_RX_OVF          0x00000020
#define I2C_INT_TX_OVF          0x00000040
#define I2C_INT_RX_UNF          0x00000080
#define I2C_INT_ARB_LOST        0x00000200
#define I2C_INT_ALL             0x000002FF


#endif // __HW_I2C_H__
