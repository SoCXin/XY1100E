#ifndef __HW_DMA_H__
#define __HW_DMA_H__

#include "hw_memmap.h"

//*****************************************************************************
//
// The following are defines for the Micro Direct Memory Access register
// addresses.
//
//*****************************************************************************
#define DMAC_CHx_SA         0x00
#define DMAC_CHx_DA         0x04
#define DMAC_CHx_CTRL       0x08
#define DMAC_CHx_TC         0x0c
#define DMAC_CHx_NP         0x10
#define DMAC_CH0_SA        (0x00 + DMAC_BASE)
#define DMAC_CH0_DA        (0x04 + DMAC_BASE)
#define DMAC_CH0_CTRL      (0x08 + DMAC_BASE)
#define DMAC_CH0_TC        (0x0c + DMAC_BASE)
#define DMAC_CH0_NP        (0x10 + DMAC_BASE)
#define DMAC_CH1_SA        (0x20 + DMAC_BASE)
#define DMAC_CH1_DA        (0x24 + DMAC_BASE)
#define DMAC_CH1_CTRL      (0x28 + DMAC_BASE)
#define DMAC_CH1_TC        (0x2c + DMAC_BASE)
#define DMAC_CH1_NP        (0x30 + DMAC_BASE)
#define DMAC_INT_STAT      (0x40 + DMAC_BASE)
#define DMAC_CH2_SA        (0x60 + DMAC_BASE)
#define DMAC_CH2_DA        (0x64 + DMAC_BASE)
#define DMAC_CH2_CTRL      (0x68 + DMAC_BASE)
#define DMAC_CH2_TC        (0x6c + DMAC_BASE)
#define DMAC_CH2_NP        (0x70 + DMAC_BASE)
#define DMAC_CH3_SA        (0x80 + DMAC_BASE)
#define DMAC_CH3_DA        (0x84 + DMAC_BASE)
#define DMAC_CH3_CTRL      (0x88 + DMAC_BASE)
#define DMAC_CH3_TC        (0x8c + DMAC_BASE)
#define DMAC_CH3_NP        (0x90 + DMAC_BASE)


//*****************************************************************************
//
// The following are defines for the bit fields in the DMAC_CHx_CTRL register.
//
//*****************************************************************************
#define DMAC_CTRL_WORD_SIZE_Pos         0UL
#define DMAC_CTRL_WORD_SIZE_Msk        (3UL << DMAC_CTRL_WORD_SIZE_Pos)
#define DMAC_CTRL_WORD_SIZE_32b        (2UL << DMAC_CTRL_WORD_SIZE_Pos)
#define DMAC_CTRL_BURST_SIZE_Pos        13UL
#define DMAC_CTRL_BURST_SIZE_Msk       (7UL << DMAC_CTRL_BURST_SIZE_Pos)
#define DMAC_CTRL_BURST_SIZE_1W        (0UL << DMAC_CTRL_BURST_SIZE_Pos)
#define DMAC_CTRL_BURST_SIZE_2W        (1UL << DMAC_CTRL_BURST_SIZE_Pos)
#define DMAC_CTRL_BURST_SIZE_4W        (2UL << DMAC_CTRL_BURST_SIZE_Pos)
#define DMAC_CTRL_BURST_SIZE_8W        (3UL << DMAC_CTRL_BURST_SIZE_Pos)
#define DMAC_CTRL_BURST_SIZE_16W       (4UL << DMAC_CTRL_BURST_SIZE_Pos)
#define DMAC_CTRL_ERR_Pos               16UL
#define DMAC_CTRL_ERR_Msk              (1UL << DMAC_CTRL_ERR_Pos)
#define DMAC_CTRL_STRT_Pos              17UL
#define DMAC_CTRL_STRT_Msk             (1UL << DMAC_CTRL_STRT_Pos)
#define DMAC_CTRL_CHG_Pos               18UL
#define DMAC_CTRL_CHG_Msk              (1UL << DMAC_CTRL_CHG_Pos)
#define DMAC_CTRL_TPYP_Pos              21UL
#define DMAC_CTRL_TYPE_Msk             (7UL << DMAC_CTRL_TPYP_Pos)
#define DMAC_CTRL_TYPE_MEM_TO_MEM      (0UL << DMAC_CTRL_TPYP_Pos)
#define DMAC_CTRL_INT_Pos               24UL
#define DMAC_CTRL_INT_Msk              (1UL << DMAC_CTRL_INT_Pos)
#define DMAC_CTRL_INT_SET              (1UL << DMAC_CTRL_INT_Pos)
#define DMAC_CTRL_TC_Pos                25UL
#define DMAC_CTRL_TC_Msk               (1UL << DMAC_CTRL_TC_Pos)
#define DMAC_CTRL_TC_SET               (1UL << DMAC_CTRL_TC_Pos)
#define DMAC_CTRL_SINC_Pos              26UL
#define DMAC_CTRL_SINC_Msk             (1UL << DMAC_CTRL_SINC_Pos)
#define DMAC_CTRL_SINC_SET             (1UL << DMAC_CTRL_SINC_Pos)
#define DMAC_CTRL_SINC_DIS             (0UL << DMAC_CTRL_SINC_Pos)
#define DMAC_CTRL_SDEC_Pos              27UL
#define DMAC_CTRL_SDEC_Msk             (1UL << DMAC_CTRL_SDEC_Pos)
#define DMAC_CTRL_SDEC_SET             (1UL << DMAC_CTRL_SDEC_Pos)
#define DMAC_CTRL_SDEC_DIS             (0UL << DMAC_CTRL_SDEC_Pos)
#define DMAC_CTRL_DINC_Pos              29UL
#define DMAC_CTRL_DINC_Msk             (1UL << DMAC_CTRL_DINC_Pos)
#define DMAC_CTRL_DINC_SET             (1UL << DMAC_CTRL_DINC_Pos)
#define DMAC_CTRL_DINC_DIS             (0UL << DMAC_CTRL_DINC_Pos)
#define DMAC_CTRL_DDEC_Pos              30UL
#define DMAC_CTRL_DDEC_Msk             (1UL << DMAC_CTRL_DDEC_Pos)
#define DMAC_CTRL_DDEC_SET             (1UL << DMAC_CTRL_DDEC_Pos)
#define DMAC_CTRL_DDEC_DIS             (0UL << DMAC_CTRL_DDEC_Pos)


//*****************************************************************************
//
// The following are defines for the bit fields in the DMAC_CHx_TC register.
//
//*****************************************************************************
#define DMAC_TC_TRANSFER_CNT_Pos        0UL
#define DMAC_TC_TRANSFER_CNT_Msk       (0x000FFFFFUL << DMAC_TC_TRANSFER_CNT_Pos)


//*****************************************************************************
//
// The following are defines for the bit fields in the DMAC_INT_STAT register.
//
//*****************************************************************************
#define DMAC_INT_STAT_CHAN0_Pos         0UL
#define DMAC_INT_STAT_CHAN0_Msk        (1UL << DMAC_INT_STAT_CHAN0_Pos)
#define DMAC_INT_STAT_CHAN1_Pos         1UL
#define DMAC_INT_STAT_CHAN1_Msk        (1UL << DMAC_INT_STAT_CHAN1_Pos)
#define DMAC_INT_STAT_CHAN2_Pos         2UL
#define DMAC_INT_STAT_CHAN2_Msk        (1UL << DMAC_INT_STAT_CHAN2_Pos)
#define DMAC_INT_STAT_CHAN3_Pos         3UL
#define DMAC_INT_STAT_CHAN3_Msk        (1UL << DMAC_INT_STAT_CHAN3_Pos)


#endif // __HW_DMA_H__
