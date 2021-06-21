#ifndef __HW_SEMA_H__
#define __HW_SEMA_H__


//*****************************************************************************
//
// The following are defines for the AES register offsets.
//
//*****************************************************************************
#define REG_SEMA_SEMA0              0x00000000
#define REG_SEMA_M0GNT              0x00000080
#define REG_SEMA_CTL                0x000000A0


//*****************************************************************************
//
// The following are defines for the bit fields in the REG_SEMA_SEMAX register.
//
//*****************************************************************************
#define SEMA_SEMA_MASTER_Pos        0
#define SEMA_SEMA_MASTER_Msk        (0xFFUL << SEMA_SEMA_MASTER_Pos)
#define SEMA_SEMA_REQ_Pos           8
#define SEMA_SEMA_REQ               (1UL << SEMA_SEMA_REQ_Pos)
#define SEMA_SEMA_RELEASE           (0UL << SEMA_SEMA_REQ_Pos)
#define SEMA_SEMA_REQ_PRI_Pos       9
#define SEMA_SEMA_REQ_PRI_Msk       (3UL << SEMA_SEMA_REQ_PRI_Pos)
#define SEMA_SEMA_REQ_PRI_0         (0UL << SEMA_SEMA_REQ_PRI_Pos)
#define SEMA_SEMA_REQ_PRI_1         (1UL << SEMA_SEMA_REQ_PRI_Pos)
#define SEMA_SEMA_REQ_PRI_2         (2UL << SEMA_SEMA_REQ_PRI_Pos)
#define SEMA_SEMA_REQ_PRI_3         (3UL << SEMA_SEMA_REQ_PRI_Pos)
#define SEMA_SEMA_OWNER_Pos         12
#define SEMA_SEMA_OWNER_Msk         (0xF << SEMA_SEMA_OWNER_Pos)


//*****************************************************************************
//
// The following are defines for the bit fields in the REG_SEMA_MXGNT register.
//
//*****************************************************************************
#define SEMA_MGNT_SEMA0             0x00000001
#define SEMA_MGNT_SEMA1             0x00000002
#define SEMA_MGNT_SEMA2             0x00000004
#define SEMA_MGNT_SEMA3             0x00000008
#define SEMA_MGNT_SEMA4             0x00000010
#define SEMA_MGNT_SEMA5             0x00000020
#define SEMA_MGNT_SEMA6             0x00000040
#define SEMA_MGNT_SEMA7             0x00000080
#define SEMA_MGNT_SEMA8             0x00000100
#define SEMA_MGNT_SEMA9             0x00000200
#define SEMA_MGNT_SEMA10            0x00000400
#define SEMA_MGNT_SEMA11            0x00000800
#define SEMA_MGNT_SEMA12            0x00001000
#define SEMA_MGNT_SEMA13            0x00002000
#define SEMA_MGNT_SEMA14            0x00004000
#define SEMA_MGNT_SEMA15            0x00008000
#define SEMA_MGNT_SEMA16            0x00010000
#define SEMA_MGNT_SEMA17            0x00020000
#define SEMA_MGNT_SEMA18            0x00040000
#define SEMA_MGNT_SEMA19            0x00080000
#define SEMA_MGNT_SEMA20            0x00100000
#define SEMA_MGNT_SEMA21            0x00200000
#define SEMA_MGNT_SEMA22            0x00400000
#define SEMA_MGNT_SEMA23            0x00800000
#define SEMA_MGNT_SEMA24            0x01000000
#define SEMA_MGNT_SEMA25            0x02000000
#define SEMA_MGNT_SEMA26            0x04000000
#define SEMA_MGNT_SEMA27            0x08000000
#define SEMA_MGNT_SEMA28            0x10000000
#define SEMA_MGNT_SEMA29            0x20000000
#define SEMA_MGNT_SEMA30            0x40000000
#define SEMA_MGNT_SEMA31            0x80000000


//*****************************************************************************
//
// The following are defines for the bit fields in the REG_SEMA_CTL register.
//
//*****************************************************************************
#define SEMA_CTL_RST_Pos            0
#define SEMA_CTL_RST                (1UL << SEMA_CTL_RST_Pos)
#define SEMA_CTL_REQ_FULL_Pos       1
#define SEMA_CTL_REQ_FULL_Msk       (1UL << SEMA_CTL_REQ_FULL_Pos)
#define SEMA_CTL_REQ_FULL           (1UL << SEMA_CTL_REQ_FULL_Pos)
#define SEMA_CTL_REQ_NFULL          (0UL << SEMA_CTL_REQ_FULL_Pos)
#define SEMA_CTL_M0_INT_EN_Pos      8
#define SEMA_CTL_M0_INT_EN          (1UL << SEMA_CTL_M0_INT_EN_Pos)
#define SEMA_CTL_M1_INT_EN_Pos      9
#define SEMA_CTL_M1_INT_EN          (1UL << SEMA_CTL_M1_INT_EN_Pos)
#define SEMA_CTL_M2_INT_EN_Pos      10
#define SEMA_CTL_M2_INT_EN          (1UL << SEMA_CTL_M2_INT_EN_Pos)
#define SEMA_CTL_M3_INT_EN_Pos      11
#define SEMA_CTL_M3_INT_EN          (1UL << SEMA_CTL_M3_INT_EN_Pos)
#define SEMA_CTL_M4_INT_EN_Pos      12
#define SEMA_CTL_M4_INT_EN          (1UL << SEMA_CTL_M4_INT_EN_Pos)
#define SEMA_CTL_M5_INT_EN_Pos      13
#define SEMA_CTL_M5_INT_EN          (1UL << SEMA_CTL_M5_INT_EN_Pos)
#define SEMA_CTL_M6_INT_EN_Pos      14
#define SEMA_CTL_M6_INT_EN          (1UL << SEMA_CTL_M6_INT_EN_Pos)
#define SEMA_CTL_M7_INT_EN_Pos      15
#define SEMA_CTL_M7_INT_EN          (1UL << SEMA_CTL_M7_INT_EN_Pos)

#endif // __HW_SEMA_H__
