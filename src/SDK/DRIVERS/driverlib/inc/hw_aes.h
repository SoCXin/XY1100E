#ifndef __HW_AES_H__
#define __HW_AES_H__


//*****************************************************************************
//
// The following are defines for the AES register offsets.
//
//*****************************************************************************
#define	REG_AES_CTL                     0x00000000
#define REG_AES_STA                     0x00000004
#define REG_AES_KEY0                    0x00000008
#define REG_AES_IV0                     0x00000028
#define REG_AES_AADLEN0                 0x00000038
#define REG_AES_AADLEN1                 0x0000003C
#define REG_AES_PLEN                    0x00000040
#define REG_AES_TAG0                    0x00000044


//*****************************************************************************
//
// The following are defines for the bit fields in the REG_AES_CTL register.
//
//*****************************************************************************
#define AES_CTL_START_Pos               0
#define AES_CTL_START                   (1UL << AES_CTL_START_Pos)
#define AES_CTL_KEY_LOAD_Pos            1
#define AES_CTL_KEY_LOAD                (1UL << AES_CTL_KEY_LOAD_Pos)
#define AES_CTL_KEY_LEN_Pos             8
#define AES_CTL_KEY_LEN_Msk             (3UL << AES_CTL_KEY_LEN_Pos)
#define AES_CTL_KEY_LEN_128             (0UL << AES_CTL_KEY_LEN_Pos)
#define AES_CTL_KEY_LEN_192             (1UL << AES_CTL_KEY_LEN_Pos)
#define AES_CTL_KEY_LEN_256             (2UL << AES_CTL_KEY_LEN_Pos)
#define AES_CTL_ENC_DEC_Pos             10
#define AES_CTL_ENC_DEC_Msk             (1UL << AES_CTL_ENC_DEC_Pos)
#define AES_CTL_ENC                     (1UL << AES_CTL_ENC_DEC_Pos)
#define AES_CTL_DEC                     (0UL << AES_CTL_ENC_DEC_Pos)
#define AES_CTL_MODE_Pos                11
#define AES_CTL_MODE_Msk                (7UL << AES_CTL_MODE_Pos)
#define AES_CTL_MODE_ECB                (0UL << AES_CTL_MODE_Pos)
#define AES_CTL_MODE_CBC                (1UL << AES_CTL_MODE_Pos)
#define AES_CTL_MODE_CFB                (2UL << AES_CTL_MODE_Pos)
#define AES_CTL_MODE_OFB                (3UL << AES_CTL_MODE_Pos)
#define AES_CTL_MODE_CTR                (4UL << AES_CTL_MODE_Pos)
#define AES_CTL_MODE_CCM                (5UL << AES_CTL_MODE_Pos)
#define AES_CTL_CCM_AUTH_LEN_Pos        16
#define AES_CTL_CCM_AUTH_LEN_Msk        (7UL << AES_CTL_CCM_AUTH_LEN_Pos)   // ccm_t = (tlen - 2)/2
#define AES_CTL_CCM_LOAD_LEN_Pos        19
#define AES_CTL_CCM_LOAD_LEN_Msk        (7UL << AES_CTL_CCM_LOAD_LEN_Pos)       // ccm_q = 15 - nlen - 1
#define AES_CTL_DMA_EN_Pos              22
#define AES_CTL_DMA_EN                  (1UL << AES_CTL_DMA_EN_Pos)
#define AES_CTL_CLK_DIV_Pos             24
#define AES_CTL_CLK_DIV_Msk             (1UL << AES_CTL_CLK_DIV_Pos)    // 1, AES core clock divided by 2, for timing
#define AES_CTL_CLK_DIV_EN              (1UL << AES_CTL_CLK_DIV_Pos)


//*****************************************************************************
//
// The following are defines for the bit fields in the REG_AES_STA register.
//
//*****************************************************************************
#define AES_STA_AES_TRANS_Pos           0                                   // 1, AES done
#define AES_STA_AES_TRANS_Msk           (1UL << AES_STA_AES_TRANS_Pos)
#define AES_STA_AES_TRANS_DONE          (1UL << AES_STA_AES_TRANS_Pos)
#define AES_STA_AES_KEY_DONE_Pos        8
#define AES_STA_AES_KEY_DONE_Msk        (1UL << AES_STA_AES_KEY_DONE_Pos)   // key load done, software read clear
#define AES_STA_AES_KEY_DONE            (1UL << AES_STA_AES_KEY_DONE_Pos)

#endif // __HW_AES_H__
