#ifndef __HW_ANALOG_H__
#define __HW_ANALOG_H__

#include "hw_memmap.h"

//*****************************************************************************
//
// The following are defines for the Micro  register
// addresses.
//
//*****************************************************************************
#define	REG_8_ANABUCK           0xA0110030
#define	REG_8_ANAPMU1           0xA0110031
#define	REG_8_ANAPMU2           0xA0110032
#define	REG_8_ANAPMU3           0xA0110033
#define	REG_8_ANAPMU4           0xA0110034
#define	REG_8_ANAPMU5           0xA0110035
#define	REG_8_ANAPMU6           0xA0110036
#define	REG_8_ANAPMU7           0xA0110037
#define	REG_8_ANAPMU8           0xA0110038
#define	REG_8_ANAPMU9           0xA0110039
#define	REG_8_ANAPMU10          0xA011003A
#define	REG_8_ANAPMU11          0xA011003B
#define	REG_8_ANAPMU12          0xA011003C
#define	REG_8_ANAPMU13          0xA011003D
#define	REG_8_ANAXTAL1          0xA011003E
#define	REG_8_ANAXTAL2          0xA011003F
#define	REG_8_ANAXTAL3          0xA0110040
#define	REG_8_ANAXTAL4          0xA0110041
#define	REG_8_ANAAUXCAL         0xA0110042
#define	REG_8_ANAAUXCALDONE     0xA0110043
#define	REG_8_ANAAUXADC1        0xA0110044
#define	REG_8_ANAAUXADC2        0xA0110045
#define	REG_8_AUXDIVCFG         0xA0110046
// Reserved.
#define	REG_8_ANAAUX1           0xA0110048
#define	REG_8_ANAAUX2           0xA0110049
#define	REG_8_ANAAUX3           0xA011004A
#define	REG_8_ANAAUX4           0xA011004B
#define	REG_8_ANAAUX5           0xA011004C
#define	REG_8_ANAAUX6           0xA011004D
#define	REG_8_ANAAUX7           0xA011004E
#define	REG_8_ANABBLDO          0xA011004F
#define	REG_8_ANAAUX9           0xA0110050
#define	REG_8_ANAAUX10          0xA0110051
#define	REG_8_ANAAUX11          0xA0110052
#define	REG_8_ANAAUX12          0xA0110053
#define	REG_8_ANABBPLL1         0xA0110054
#define	REG_8_ANABBPLL2         0xA0110055
#define	REG_8_ANABBPLL3         0xA0110056
// Reserved.
#define	REG_8_ANABBPLL5         0xA0110058
#define	REG_8_ANABBPLL6         0xA0110059
#define	REG_8_ANABBPLL7         0xA011005A
#define	REG_8_ANABBPLL8         0xA011005B
#define	REG_8_ANABBPLL9         0xA011005C
#define	REG_8_ANABBPLL10        0xA011005D
#define	REG_8_ANABBPLL11        0xA011005E
#define	REG_8_ANABBPLL12        0xA011005F
#define	REG_8_ANABBPLL13        0xA0110060
#define	REG_8_ANABBPLL14        0xA0110061
#define	REG_8_ANABBPLL15        0xA0110062
#define	REG_8_ANABBPLL16        0xA0110063
#define	REG_8_ANABBPLL17        0xA0110064
#define	REG_8_ANABBPLL18        0xA0110065
#define	REG_8_ANABBPLL19        0xA0110066
#define	REG_8_ANABBPLL20        0xA0110067
#define	REG_8_ANABBPLL21        0xA0110068
#define	REG_8_ANABBPLL22        0xA0110069
#define	REG_8_ANABBPLL23        0xA011006A
#define	REG_8_ANABBPLL24        0xA011006B
#define	REG_8_ANABBPLL25        0xA011006C
#define	REG_8_ANABBPLL26        0xA011006D
#define	REG_8_ANABBPLL27        0xA011006E
#define	REG_8_ANABBPLL28        0xA011006F
#define	REG_8_ANACFG            0xA0058018
#define	REG_8_ANACFG1           0xA0058019
#define	REG_8_ANACFG2           0xA005801A
// Reserved.
#define	REG_8_ANACFG3           0xA005801C
#define	REG_8_ANACFG4           0xA005801D
#define	REG_8_ANACFG5           0xA005801E
#define	REG_8_ANACFG6           0xA005801F
#define	REG_8_ANACFG7           0xA0058020
#define	REG_8_ANACFG8           0xA0058021
// Reserved.
// Reserved.
#define	REG_8_ANACNTL_EN        0xA0058024


#define REG_32_RX_RSSI_CIC      (DFE_REG_BASE + 0x14)

#define REG_32_RX_DLY_REG0      (DFE_REG_BASE + 0x24)
#define REG_32_RX_DLY_REG1      (DFE_REG_BASE + 0x28)
#define REG_32_RX_DLY_REG2      (DFE_REG_BASE + 0x2C)
#define REG_32_RX_DLY_REG3      (DFE_REG_BASE + 0x30)
#define REG_32_RX_DLY_REG4      (DFE_REG_BASE + 0x34)
#define REG_32_RX_DLY_REG5      (DFE_REG_BASE + 0x38)
#define REG_32_RX_DLY_REG6      (DFE_REG_BASE + 0x3C)
#define REG_32_RX_DLY_REG7      (DFE_REG_BASE + 0x84)

#define REG_32_TX_DLY_REG0      (DFE_REG_BASE + 0x74)
#define REG_32_TX_DLY_REG1      (DFE_REG_BASE + 0x64)
#define REG_32_TX_DLY_REG2      (DFE_REG_BASE + 0x94)
#define REG_32_TX_DLY_REG3      (DFE_REG_BASE + 0x98)
#define REG_32_TX_DLY_REG4      (DFE_REG_BASE + 0x9C)

#define REG_32_TX_IQ_AMP_EST    (DFE_REG_BASE + 0x48)
#define REG_32_TX_IQ_PHS_EST_Q  (DFE_REG_BASE + 0x4C)
#define REG_32_TX_DC_EST_IQ     (DFE_REG_BASE + 0x5C)

#define REG_8_TX_TSSI_AVG_NUM   (DFE_REG_BASE + 0x54)
#define REG_8_RXHB_LNA_GC       (DFE_REG_BASE + 0x88)
#define REG_8_RXHB_LNA_RO_ADJ   (DFE_REG_BASE + 0x89)
#define REG_8_RXHB_GM_GC_SD_ADJ (DFE_REG_BASE + 0x8A)
#define REG_8_RXLB_LNA_GC       (DFE_REG_BASE + 0x8C)
#define REG_8_RXLB_GM_GC_SD_ADJ (DFE_REG_BASE + 0x8D)
#define REG_8_RXBB_PGA_GC       (DFE_REG_BASE + 0x8E)
#define REG_8_TXHB_MX_CAS_GC    (DFE_REG_BASE + 0xA0)
#define REG_8_TXHB_PAD_GC       (DFE_REG_BASE + 0xA1)
#define REG_8_TXHB_PA_GC        (DFE_REG_BASE + 0xA2)
#define REG_8_TXLB_MX_CAS_GC    (DFE_REG_BASE + 0xA4)
#define REG_8_TXLB_PA_GC        (DFE_REG_BASE + 0xA5)
#define REG_8_TXBB_GC           (DFE_REG_BASE + 0xA6)
#define REG_8_TXBB_PU           (DFE_REG_BASE + 0xA8)
#define REG_8_TXHB_BIAS_PAD_PU  (DFE_REG_BASE + 0xA9)
#define REG_8_TXLB_BIAS_PA_PU   (DFE_REG_BASE + 0xAA)
#define REG_8_ANATXPWRCTL       (DFE_REG_BASE + 0xB0)
#define REG_8_ANATXCFG23        (DFE_REG_BASE + 0xC7)
#define REG_8_ANARFPLL4         (DFE_REG_BASE + 0xD6)
#define REG_8_ANARFPLL8         (DFE_REG_BASE + 0xDA)
#define REG_8_ANARXPWRCTL1      (DFE_REG_BASE + 0xF8)
#define REG_8_ANARXPWRCTL2      (DFE_REG_BASE + 0xF9)
#define REG_8_ANARXCFG1         (DFE_REG_BASE + 0xFB)


#define	REG_32_ANAPMU_CNTL_23_0_BUCK                                        0xA0110030
#define	REG_32_ANAPMU_CNTL_55_24                                            0xA0110034
#define	REG_32_ANAPMU_CNTL_87_56                                            0xA0110038
#define	REG_32_ANAXTAL16M_CNTL_15_0_PMU_CNTL_101_88                         0xA011003C
#define	REG_32_ANAAUX_RCCAL_7_0_XTAL16M_CTUNE_7_0_CNTL_23_16                0xA0110040
#define	REG_32_ANAAUX_DIV_CFG_5_0_AUX_ADC_DOUT_11_0                         0xA0110044
#define	REG_32_ANAAUX0                                                      0xA0110048
#define	REG_32_ANABBLDO_CNTL_7_0_ANAAUX1                                    0xA011004C
#define	REG_32_ANAAUX_CNTL_31_0                                             0xA0110050
#define	REG_32_ANABBPLL_FRACN_23_0                                          0xA0110054
#define	REG_32_ANABBPLL_CAL_EN_DIVN_9_0_LOCK_LOST_3_0_CBANK_READOUT_7_0     0xA0110058
#define	REG_32_ANABBPLL_READOUT_31_0                                        0xA011005C
#define	REG_32_ANABBPLL_CNTL_31_0                                           0xA0110060
#define	REG_32_ANABBPLL_CNTL_63_32                                          0xA0110064
#define	REG_32_ANABBPLL_CNTL_95_64                                          0xA0110068
#define	REG_32_ANABBPLL_CNTL_127_96                                         0xA011006C
#define	REG_32_ANACFG                                                       0xA0058018
#define	REG_32_ANAAON_CNTL_31_0                                             0xA005801C
#define	REG_32_ANAAON_CNTL_63_32                                            0xA0058020
#define	REG_32_ANACNTL_EN                                                   0xA0058024


//*****************************************************************************
//
// The following are defines for the bit fields in the bus.
//
//*****************************************************************************
// REG_8_ANACNTL_EN 0xA0058024
#define ANACNTL_EN_TXLDO_PALDO_CNTL_EN_Pos              3
#define ANACNTL_EN_TXLDO_PALDO_CNTL_EN_Msk              (1UL << ANACNTL_EN_TXLDO_PALDO_CNTL_EN_Pos)
#define ANACNTL_EN_RFLDO_CNTL_EN_Pos                    2
#define ANACNTL_EN_RFLDO_CNTL_EN_Msk                    (1UL << ANACNTL_EN_RFLDO_CNTL_EN_Pos)
#define ANACNTL_EN_AUX_CNTL_EN_Pos                      1
#define ANACNTL_EN_AUX_CNTL_EN_Msk                      (1UL << ANACNTL_EN_AUX_CNTL_EN_Pos)
#define ANACNTL_EN_BBLDO_CNTL_EN_Pos                    0
#define ANACNTL_EN_BBLDO_CNTL_EN_Msk                    (1UL << ANACNTL_EN_BBLDO_CNTL_EN_Pos)

// REG_8_ANABBLDO 0xA011004F

// REG_8_ANAAUX2 0xA0110049
#define ANAAUX2_BBLDO_TRX_EN_Pos                        7
#define ANAAUX2_BBLDO_TRX_EN_Msk                        (1UL << ANAAUX2_BBLDO_TRX_EN_Pos)

// REG_8_RXHB_LNA_RO_ADJ (DFE_REG_BASE + 0x89)
#define RXHB_LNA_RO_ADJ_RXHB_LNA_BYP_EN_Pos             7
#define RXHB_LNA_RO_ADJ_RXHB_LNA_BYP_EN_Msk             (1UL << RXHB_LNA_RO_ADJ_RXHB_LNA_BYP_EN_Pos)
#define RXHB_LNA_RO_ADJ_RXHB_LNA_RST_BYP_Pos            5
#define RXHB_LNA_RO_ADJ_RXHB_LNA_RST_BYP_Msk            (1UL << RXHB_LNA_RO_ADJ_RXHB_LNA_RST_BYP_Pos)
#define RXHB_LNA_RO_ADJ_RXHB_LNA_RST_BYPVAL_Pos         4
#define RXHB_LNA_RO_ADJ_RXHB_LNA_RST_BYPVAL_Msk         (1UL << RXHB_LNA_RO_ADJ_RXHB_LNA_RST_BYPVAL_Pos)
#define RXHB_LNA_RO_ADJ_RXHB_LNA_RO_ADJ_Pos             0
#define RXHB_LNA_RO_ADJ_RXHB_LNA_RO_ADJ_Msk             (0xFUL << RXHB_LNA_RO_ADJ_RXHB_LNA_RO_ADJ_Pos)

// REG_8_RXHB_GM_GC_SD_ADJ (DFE_REG_BASE + 0x8A)
#define RXHB_GM_GC_SD_ADJ_RXHB_GM_SD_ADJ_Pos            4
#define RXHB_GM_GC_SD_ADJ_RXHB_GM_SD_ADJ_Msk            (0xF << RXHB_GM_GC_SD_ADJ_RXHB_GM_SD_ADJ_Pos)
#define RXHB_GM_GC_SD_ADJ_RXHB_GM_GC_Pos                0
#define RXHB_GM_GC_SD_ADJ_RXHB_GM_GC_Msk                (0xF << RXHB_GM_GC_SD_ADJ_RXHB_GM_GC_Pos)

// REG_8_TXBB_PU (DFE_REG_BASE + 0xA8)
#define TXBB_PU_TXBB_RSTB_Pos                           1
#define TXBB_PU_TXBB_RSTB_Msk                           (1UL << TXBB_PU_TXBB_RSTB_Pos)
#define TXBB_PU_TXBB_PU_Pos                             0
#define TXBB_PU_TXBB_PU_Msk                             (1UL << TXBB_PU_TXBB_PU_Pos)

// REG_8_TXHB_BIAS_PAD_PU (DFE_REG_BASE + 0xA9)
#define TXHB_BIAS_PAD_PU_TXHB_MX_LO_BIAS_RST_Pos        7
#define TXHB_BIAS_PAD_PU_TXHB_MX_LO_BIAS_RST_Msk        (1UL << TXHB_BIAS_PAD_PU_TXHB_MX_LO_BIAS_RST_Pos)
#define TXHB_BIAS_PAD_PU_TXHB_MX_PU_Pos                 6
#define TXHB_BIAS_PAD_PU_TXHB_MX_PU_Msk                 (1UL << TXHB_BIAS_PAD_PU_TXHB_MX_PU_Pos)
#define TXHB_BIAS_PAD_PU_TXHB_MX_GM_BIAS_RST_Pos        5
#define TXHB_BIAS_PAD_PU_TXHB_MX_GM_BIAS_RST_Msk        (1UL << TXHB_BIAS_PAD_PU_TXHB_MX_GM_BIAS_RST_Pos)
#define TXHB_BIAS_PAD_PU_TXHB_PAD_PU_Pos                4
#define TXHB_BIAS_PAD_PU_TXHB_PAD_PU_Msk                (1UL << TXHB_BIAS_PAD_PU_TXHB_PAD_PU_Pos)
#define TXHB_BIAS_PAD_PU_TXHB_PA_PU_Pos                 2
#define TXHB_BIAS_PAD_PU_TXHB_PA_PU_Msk                 (1UL << TXHB_BIAS_PAD_PU_TXHB_PA_PU_Pos)
#define TXHB_BIAS_PAD_PU_TXHB_BIAS_PU_Pos               0
#define TXHB_BIAS_PAD_PU_TXHB_BIAS_PU_Msk               (1UL << TXHB_BIAS_PAD_PU_TXHB_BIAS_PU_Pos)

// REG_8_TXLB_BIAS_PA_PU (DFE_REG_BASE + 0xAA)
#define TXLB_BIAS_PA_PU_TXLB_PA_GM_BIAS_RST_Pos         5
#define TXLB_BIAS_PA_PU_TXLB_PA_GM_BIAS_RST_Msk         (1UL << TXLB_BIAS_PA_PU_TXLB_PA_GM_BIAS_RST_Pos)
#define TXLB_BIAS_PA_PU_TXLB_PA_PU_Pos                  4
#define TXLB_BIAS_PA_PU_TXLB_PA_PU_Msk                  (1UL << TXLB_BIAS_PA_PU_TXLB_PA_PU_Pos)
#define TXLB_BIAS_PA_PU_TXLB_MX_LO_BIAS_RST_Pos         3
#define TXLB_BIAS_PA_PU_TXLB_MX_LO_BIAS_RST_Msk         (1UL << TXLB_BIAS_PA_PU_TXLB_MX_LO_BIAS_RST_Pos)
#define TXLB_BIAS_PA_PU_TXLB_MX_PU_Pos                  2
#define TXLB_BIAS_PA_PU_TXLB_MX_PU_Msk                  (1UL << TXLB_BIAS_PA_PU_TXLB_MX_PU_Pos)
#define TXLB_BIAS_PA_PU_TXLB_BIAS_PU_Pos                0
#define TXLB_BIAS_PA_PU_TXLB_BIAS_PU_Msk                (1UL << TXLB_BIAS_PA_PU_TXLB_BIAS_PU_Pos)

// REG_8_ANATXPWRCTL (DFE_REG_BASE + 0xB0)
#define ANATXPWRCTL_TXLB_TSSI_ATT_EN_Pos                5
#define ANATXPWRCTL_TXLB_TSSI_ATT_EN_Msk                (1UL << ANATXPWRCTL_TXLB_TSSI_ATT_EN_Pos)
#define ANATXPWRCTL_TXHB_TSSI_ATT_EN_Pos                4
#define ANATXPWRCTL_TXHB_TSSI_ATT_EN_Msk                (1UL << ANATXPWRCTL_TXHB_TSSI_ATT_EN_Pos)
#define ANATXPWRCTL_TXLB_TSSI_PU_Pos                    3
#define ANATXPWRCTL_TXLB_TSSI_PU_Msk                    (1UL << ANATXPWRCTL_TXLB_TSSI_PU_Pos)
#define ANATXPWRCTL_TXHB_TSSI_PU_Pos                    2
#define ANATXPWRCTL_TXHB_TSSI_PU_Msk                    (1UL << ANATXPWRCTL_TXHB_TSSI_PU_Pos)
#define ANATXPWRCTL_TXLDO_PALDO_PU_Pos                  0
#define ANATXPWRCTL_TXLDO_PALDO_PU_Msk                  (1UL << ANATXPWRCTL_TXLDO_PALDO_PU_Pos)

// REG_8_ANATXCFG23 (DFE_REG_BASE + 0xC7)
#define ANATXCFG23_TXLDO_PALDO_BYP_FLAG_Pos             0
#define ANATXCFG23_TXLDO_PALDO_BYP_FLAG_Msk             (1UL << ANATXCFG23_TXLDO_PALDO_BYP_FLAG_Pos)

// REG_8_ANARFPLL4 (DFE_REG_BASE + 0xD6)
#define ANARFPLL4_RFPLL_LOCK_DET_Pos                    1
#define ANARFPLL4_RFPLL_LOCK_DET_Msk                    (1UL << ANARFPLL4_RFPLL_LOCK_DET_Pos)

// REG_8_ANARFPLL8 (DFE_REG_BASE + 0xDA)
#define ANARFPLL8_RFPLL_PU_Pos                          3
#define ANARFPLL8_RFPLL_PU_Msk                          (1UL << ANARFPLL8_RFPLL_PU_Pos)
#define ANARFPLL8_RFPLL_LDO_FILTER_EN_Pos               2
#define ANARFPLL8_RFPLL_LDO_FILTER_EN_Msk               (1UL << ANARFPLL8_RFPLL_LDO_FILTER_EN_Pos)
#define ANARFPLL8_RFPLL_LDO_PU_Pos                      1
#define ANARFPLL8_RFPLL_LDO_PU_Msk                      (1UL << ANARFPLL8_RFPLL_LDO_PU_Pos)
#define ANARFPLL8_RFPLL_CAL_EN_Pos                      0
#define ANARFPLL8_RFPLL_CAL_EN_Msk                      (1UL << ANARFPLL8_RFPLL_CAL_EN_Pos)

// REG_8_ANARXPWRCTL1 (DFE_REG_BASE + 0xF8)
#define ANARXPWRCTL1_RXBB_PGA_BIAS_PU_Pos               7
#define ANARXPWRCTL1_RXBB_PGA_BIAS_PU_Msk               (1UL << ANARXPWRCTL1_RXBB_PGA_BIAS_PU_Pos)
#define ANARXPWRCTL1_RXBB_PGA_PU_I_Pos                  6
#define ANARXPWRCTL1_RXBB_PGA_PU_I_Msk                  (1UL << ANARXPWRCTL1_RXBB_PGA_PU_I_Pos)
#define ANARXPWRCTL1_RXBB_PGA_PU_Q_Pos                  5
#define ANARXPWRCTL1_RXBB_PGA_PU_Q_Msk                  (1UL << ANARXPWRCTL1_RXBB_PGA_PU_Q_Pos)
#define ANARXPWRCTL1_RXBB_ADC_PU_Pos                    4
#define ANARXPWRCTL1_RXBB_ADC_PU_Msk                    (1UL << ANARXPWRCTL1_RXBB_ADC_PU_Pos)
#define ANARXPWRCTL1_RXBB_ADC_BIAS_PU_Pos               3
#define ANARXPWRCTL1_RXBB_ADC_BIAS_PU_Msk               (1UL << ANARXPWRCTL1_RXBB_ADC_BIAS_PU_Pos)
#define ANARXPWRCTL1_RXBB_TIA_PU_Pos                    2
#define ANARXPWRCTL1_RXBB_TIA_PU_Msk                    (1UL << ANARXPWRCTL1_RXBB_TIA_PU_Pos)
#define ANARXPWRCTL1_RFLDO_PU_Pos                       0
#define ANARXPWRCTL1_RFLDO_PU_Msk                       (1UL << ANARXPWRCTL1_RFLDO_PU_Pos)

// REG_8_ANARXPWRCTL2 (DFE_REG_BASE + 0xF9)
#define ANARXPWRCTL2_RXLB_GM_BIAS_PU_Pos                7
#define ANARXPWRCTL2_RXLB_GM_BIAS_PU_Msk                (1UL << ANARXPWRCTL2_RXLB_GM_BIAS_PU_Pos)
#define ANARXPWRCTL2_RXLB_LO_PU_Pos                     6
#define ANARXPWRCTL2_RXLB_LO_PU_Msk                     (1UL << ANARXPWRCTL2_RXLB_LO_PU_Pos)
#define ANARXPWRCTL2_RXLB_LNA_PU_Pos                    5
#define ANARXPWRCTL2_RXLB_LNA_PU_Msk                    (1UL << ANARXPWRCTL2_RXLB_LNA_PU_Pos)
#define ANARXPWRCTL2_RXHB_LNA_PU_Pos                    4
#define ANARXPWRCTL2_RXHB_LNA_PU_Msk                    (1UL << ANARXPWRCTL2_RXHB_LNA_PU_Pos)
#define ANARXPWRCTL2_RXHB_GM_BIAS_PU_Pos                3
#define ANARXPWRCTL2_RXHB_GM_BIAS_PU_Msk                (1UL << ANARXPWRCTL2_RXHB_GM_BIAS_PU_Pos)
#define ANARXPWRCTL2_RXHB_LO_PU_Pos                     2
#define ANARXPWRCTL2_RXHB_LO_PU_Msk                     (1UL << ANARXPWRCTL2_RXHB_LO_PU_Pos)
#define ANARXPWRCTL2_RXHB_LNA_BIAS_PU_Pos               0
#define ANARXPWRCTL2_RXHB_LNA_BIAS_PU_Msk               (1UL << ANARXPWRCTL2_RXHB_LNA_BIAS_PU_Pos)

// REG_8_ANARXCFG1 (DFE_REG_BASE + 0xFB)
#define ANARXCFG1_RXBB_ADC_RST_BYP_Pos                  2
#define ANARXCFG1_RXBB_ADC_RST_BYP_Msk                  (1UL << ANARXCFG1_RXBB_ADC_RST_BYP_Pos)
#define ANARXCFG1_RXBB_ADC_RSTB_BYPVAL_Pos              1
#define ANARXCFG1_RXBB_ADC_RSTB_BYPVAL_Msk              (1UL << ANARXCFG1_RXBB_ADC_RSTB_BYPVAL_Pos)
#define ANARXCFG1_RXHB_LNA_EN_Pos                       0
#define ANARXCFG1_RXHB_LNA_EN_Msk                       (1UL << ANARXCFG1_RXHB_LNA_EN_Pos)

#define RFPLL_CNTL_BYPASS_EN_XXLDO_HIGH                 0
#define RFPLL_CNTL_BYPASS_EN_XXLDO_LOW                  0
#define RFPLL_CNTL_PU_LOLDO_HIGH                        1
#define RFPLL_CNTL_PU_LOLDO_LOW                         1
#define RFPLL_CNTL_PU_VCOLDO_HIGH                       2
#define RFPLL_CNTL_PU_VCOLDO_LOW                        2
#define RFPLL_CNTL_PU_MMDLDO_HIGH                       3
#define RFPLL_CNTL_PU_MMDLDO_LOW                        3
#define RFPLL_CNTL_PU_CPLDO_HIGH                        4
#define RFPLL_CNTL_PU_CPLDO_LOW                         4
#define RFPLL_CNTL_FILTER_EN_LOLDO_HIGH                 5
#define RFPLL_CNTL_FILTER_EN_LOLDO_LOW                  5
#define RFPLL_CNTL_FILTER_EN_VCOLDO_HIGH                6
#define RFPLL_CNTL_FILTER_EN_VCOLDO_LOW                 6
#define RFPLL_CNTL_FILTER_EN_MMDLDO_HIGH                7
#define RFPLL_CNTL_FILTER_EN_MMDLDO_LOW                 7
#define RFPLL_CNTL_FILTER_EN_CPLDO_HIGH                 8
#define RFPLL_CNTL_FILTER_EN_CPLDO_LOW                  8
#define RFPLL_CNTL_RES_CNTL_LOLDO_HIGH                  12
#define RFPLL_CNTL_RES_CNTL_LOLDO_LOW                   9
#define RFPLL_CNTL_RES_CNTL_VCOLDO_HIGH                 16
#define RFPLL_CNTL_RES_CNTL_VCOLDO_LOW                  13
#define RFPLL_CNTL_RES_CNTL_MMDLDO_HIGH                 20
#define RFPLL_CNTL_RES_CNTL_MMDLDO_LOW                  17
#define RFPLL_CNTL_RES_CNTL_CPLDO_HIGH                  24
#define RFPLL_CNTL_RES_CNTL_CPLDO_LOW                   21
#define RFPLL_CNTL_I_EXTRA_EN_LDO_HIGH                  27
#define RFPLL_CNTL_I_EXTRA_EN_LDO_LOW                   27
#define RFPLL_CNTL_XTAL_PD_HIGH                         28
#define RFPLL_CNTL_XTAL_PD_LOW                          28
#define RFPLL_CNTL_TPORT_CUR_EXTRA_HIGH                 30
#define RFPLL_CNTL_TPORT_CUR_EXTRA_LOW                  30
#define RFPLL_CNTL_TPORT_BB_BYPASS_HIGH                 31
#define RFPLL_CNTL_TPORT_BB_BYPASS_LOW                  31
#define RFPLL_CNTL_TPORT_BBPLL_PU_HIGH                  32
#define RFPLL_CNTL_TPORT_BBPLL_PU_LOW                   32
#define RFPLL_CNTL_TPORT_RF_BYPASS_HIGH                 33
#define RFPLL_CNTL_TPORT_RF_BYPASS_LOW                  33
#define RFPLL_CNTL_TPORT_RFPLL_PU_HIGH                  34
#define RFPLL_CNTL_TPORT_RFPLL_PU_LOW                   34
#define RFPLL_CNTL_DITHER_EN_HIGH                       35
#define RFPLL_CNTL_DITHER_EN_LOW                        35
#define RFPLL_CNTL_MAN_CODE_IN_HIGH                     44
#define RFPLL_CNTL_MAN_CODE_IN_LOW                      36
#define RFPLL_CNTL_PLL_STM_MODE_HIGH                    46
#define RFPLL_CNTL_PLL_STM_MODE_LOW                     45
#define RFPLL_CNTL_READ_OUT_SEL_HIGH                    47
#define RFPLL_CNTL_READ_OUT_SEL_LOW                     47
#define RFPLL_CNTL_REF_CYCLE_CNT_HIGH                   63
#define RFPLL_CNTL_REF_CYCLE_CNT_LOW                    48
#define RFPLL_CNTL_REF_CYCLE_LOCKTIME_HIGH              73
#define RFPLL_CNTL_REF_CYCLE_LOCKTIME_LOW               64
#define RFPLL_CNTL_REF_CYCLE_STEP1_HIGH                 83
#define RFPLL_CNTL_REF_CYCLE_STEP1_LOW                  74
#define RFPLL_CNTL_REF_CYCLE_STEP3_HIGH                 93
#define RFPLL_CNTL_REF_CYCLE_STEP3_LOW                  84
#define RFPLL_CNTL_REF_CYCLE_WAITTIME_HIGH              105
#define RFPLL_CNTL_REF_CYCLE_WAITTIME_LOW               94
#define RFPLL_CNTL_LOCK_TH_BIN_HIGH                     110
#define RFPLL_CNTL_LOCK_TH_BIN_LOW                      106
#define RFPLL_CNTL_RST_SDM_N_HIGH                       111
#define RFPLL_CNTL_RST_SDM_N_LOW                        111
#define RFPLL_CNTL_AVDDFILTER_EN_VCO_SEL_HIGH           118
#define RFPLL_CNTL_AVDDFILTER_EN_VCO_SEL_LOW            118
#define RFPLL_CNTL_IBIAS_RC_FILTER_EN_HIGH              119
#define RFPLL_CNTL_IBIAS_RC_FILTER_EN_LOW               119
#define RFPLL_CNTL_LPF_R1_CNTL_IN_FSTL_HIGH             123
#define RFPLL_CNTL_LPF_R1_CNTL_IN_FSTL_LOW              120
#define RFPLL_CNTL_LPF_R1_CNTL_IN_ORIG_HIGH             127
#define RFPLL_CNTL_LPF_R1_CNTL_IN_ORIG_LOW              124
#define RFPLL_CNTL_LPF_R2_CNTL_HIGH                     130
#define RFPLL_CNTL_LPF_R2_CNTL_LOW                      128
#define RFPLL_CNTL_LPF_R3_CNTL_HIGH                     133
#define RFPLL_CNTL_LPF_R3_CNTL_LOW                      131
#define RFPLL_CNTL_LPF_VREF_CNTL_HIGH                   137
#define RFPLL_CNTL_LPF_VREF_CNTL_LOW                    134
#define RFPLL_CNTL_LPF_CAL_EN_HIGH                      138
#define RFPLL_CNTL_LPF_CAL_EN_LOW                       138
#define RFPLL_CNTL_VCNTL_TEST_EN_HIGH                   139
#define RFPLL_CNTL_VCNTL_TEST_EN_LOW                    139
#define RFPLL_CNTL_PU_LPF_HIGH                          140
#define RFPLL_CNTL_PU_LPF_LOW                           140
#define RFPLL_CNTL_CP_ISEL_IN_FSTL_HIGH                 145
#define RFPLL_CNTL_CP_ISEL_IN_FSTL_LOW                  141
#define RFPLL_CNTL_CP_ISEL_IN_ORIG_HIGH                 150
#define RFPLL_CNTL_CP_ISEL_IN_ORIG_LOW                  146
#define RFPLL_CNTL_DLY0_CNTL_PFD_HIGH                   153
#define RFPLL_CNTL_DLY0_CNTL_PFD_LOW                    151
#define RFPLL_CNTL_DLY1_CNTL_PFD_HIGH                   158
#define RFPLL_CNTL_DLY1_CNTL_PFD_LOW                    154
#define RFPLL_CNTL_INT_MODE_HIGH                        159
#define RFPLL_CNTL_INT_MODE_LOW                         159
#define RFPLL_CNTL_LP_CP_UBG_HIGH                       160
#define RFPLL_CNTL_LP_CP_UBG_LOW                        160
#define RFPLL_CNTL_PU_CP_HIGH                           161
#define RFPLL_CNTL_PU_CP_LOW                            161
#define RFPLL_CNTL_IBIAS_CNTL_VCO_IN_HIGH               168
#define RFPLL_CNTL_IBIAS_CNTL_VCO_IN_LOW                165
#define RFPLL_CNTL_IBIAS_CNTL_VCO_AUTO_HIGH             169
#define RFPLL_CNTL_IBIAS_CNTL_VCO_AUTO_LOW              169
#define RFPLL_CNTL_VCOBUF_CM_CNTL_HIGH                  172
#define RFPLL_CNTL_VCOBUF_CM_CNTL_LOW                   170
#define RFPLL_CNTL_VBIAS_FILTER_EN_HIGH                 173
#define RFPLL_CNTL_VBIAS_FILTER_EN_LOW                  173
#define RFPLL_CNTL_HB_MODE_HIGH                         174
#define RFPLL_CNTL_HB_MODE_LOW                          174
#define RFPLL_CNTL_VBIAS_CNTL_VCO_HIGH                  177
#define RFPLL_CNTL_VBIAS_CNTL_VCO_LOW                   175
#define RFPLL_CNTL_VCO_AVDDFILTER_HIGH                  178
#define RFPLL_CNTL_VCO_AVDDFILTER_LOW                   178
#define RFPLL_CNTL_IBIAS_CNTL_BUF_HIGH                  180
#define RFPLL_CNTL_IBIAS_CNTL_BUF_LOW                   179
#define RFPLL_CNTL_PU_VCO_HIGH                          181
#define RFPLL_CNTL_PU_VCO_LOW                           181
#define RFPLL_CNTL_IBIAS_CNTL_DIV2_HIGH                 183
#define RFPLL_CNTL_IBIAS_CNTL_DIV2_LOW                  182
#define RFPLL_CNTL_PU_DIV2_HIGH                         184
#define RFPLL_CNTL_PU_DIV2_LOW                          184
#define RFPLL_CNTL_PU_D2C_HIGH                          185
#define RFPLL_CNTL_PU_D2C_LOW                           185
#define RFPLL_CNTL_B_HIGH                               187
#define RFPLL_CNTL_B_LOW                                186
#define RFPLL_CNTL_EN_TXH_HIGH                          188
#define RFPLL_CNTL_EN_TXH_LOW                           188
#define RFPLL_CNTL_EN_RXH_HIGH                          189
#define RFPLL_CNTL_EN_RXH_LOW                           189
#define RFPLL_CNTL_EN_RXL_HIGH                          190
#define RFPLL_CNTL_EN_RXL_LOW                           190
#define RFPLL_CNTL_EN_TXL_HIGH                          191
#define RFPLL_CNTL_EN_TXL_LOW                           191
#define RFPLL_CNTL_PU_LO_HIGH                           25
#define RFPLL_CNTL_PU_LO_LOW                            25
#define RFPLL_CNTL_PU_MMD_HIGH                          26
#define RFPLL_CNTL_PU_MMD_LOW                           26

#define BBPLL_CNTL_BYPASS_EN_XXLDO_HIGH                 0
#define BBPLL_CNTL_BYPASS_EN_XXLDO_LOW                  0
#define BBPLL_CNTL_PU_CPLDO_HIGH                        1
#define BBPLL_CNTL_PU_CPLDO_LOW                         1
#define BBPLL_CNTL_PU_VCOLDO_HIGH                       2
#define BBPLL_CNTL_PU_VCOLDO_LOW                        2
#define BBPLL_CNTL_FILTER_EN_CPLDO_HIGH                 3
#define BBPLL_CNTL_FILTER_EN_CPLDO_LOW                  3
#define BBPLL_CNTL_FILTER_EN_VCOLDO_HIGH                4
#define BBPLL_CNTL_FILTER_EN_VCOLDO_LOW                 4
#define BBPLL_CNTL_RES_CNTL_LOLDO_HIGH                  8
#define BBPLL_CNTL_RES_CNTL_LOLDO_LOW                   5
#define BBPLL_CNTL_RES_CNTL_VCOLDO_HIGH                 12
#define BBPLL_CNTL_RES_CNTL_VCOLDO_LOW                  9
#define BBPLL_CNTL_DITHER_EN_HIGH                       13
#define BBPLL_CNTL_DITHER_EN_LOW                        13
#define BBPLL_CNTL_VCO_BAND_SEL_HIGH                    20
#define BBPLL_CNTL_VCO_BAND_SEL_LOW                     14
#define BBPLL_CNTL_PLL_STM_MODE_HIGH                    22
#define BBPLL_CNTL_PLL_STM_MODE_LOW                     21
#define BBPLL_CNTL_READ_OUT_SEL_HIGH                    23
#define BBPLL_CNTL_READ_OUT_SEL_LOW                     23
#define BBPLL_CNTL_REF_CYCLE_CNT_HIGH                   39
#define BBPLL_CNTL_REF_CYCLE_CNT_LOW                    24
#define BBPLL_CNTL_REF_CYCLE_LOCKTIME_HIGH              49
#define BBPLL_CNTL_REF_CYCLE_LOCKTIME_LOW               40
#define BBPLL_CNTL_REF_CYCLE_STEP1OR3_HIGH              59
#define BBPLL_CNTL_REF_CYCLE_STEP1OR3_LOW               50
#define BBPLL_CNTL_REF_CYCLE_WAITTIME_HIGH              65
#define BBPLL_CNTL_REF_CYCLE_WAITTIME_LOW               60
#define BBPLL_CNTL_LOCK_TH_BIN_HIGH                     70
#define BBPLL_CNTL_LOCK_TH_BIN_LOW                      66
#define BBPLL_CNTL_RST_SDM_N_HIGH                       71
#define BBPLL_CNTL_RST_SDM_N_LOW                        71
#define BBPLL_CNTL_LPF_R1_CNTL_IN_HIGH                  79
#define BBPLL_CNTL_LPF_R1_CNTL_IN_LOW                   76
#define BBPLL_CNTL_LPF_C3_CNTL_HIGH                     81
#define BBPLL_CNTL_LPF_C3_CNTL_LOW                      80
#define BBPLL_CNTL_LPF_R2_CNTL_HIGH                     84
#define BBPLL_CNTL_LPF_R2_CNTL_LOW                      82
#define BBPLL_CNTL_LPF_FAST_SETTLE_HIGH                 85
#define BBPLL_CNTL_LPF_FAST_SETTLE_LOW                  85
#define BBPLL_CNTL_LPF_VREF_CNTL_HIGH                   89
#define BBPLL_CNTL_LPF_VREF_CNTL_LOW                    86
#define BBPLL_CNTL_LPF_CAL_EN_HIGH                      90
#define BBPLL_CNTL_LPF_CAL_EN_LOW                       90
#define BBPLL_CNTL_PU_LPF_HIGH                          91
#define BBPLL_CNTL_PU_LPF_LOW                           91
#define BBPLL_CNTL_CP_ISEL_IN_HIGH                      96
#define BBPLL_CNTL_CP_ISEL_IN_LOW                       92
#define BBPLL_CNTL_DLY0_CNTL_PFD_HIGH                   99
#define BBPLL_CNTL_DLY0_CNTL_PFD_LOW                    97
#define BBPLL_CNTL_DLY1_CNTL_PFD_HIGH                   104
#define BBPLL_CNTL_DLY1_CNTL_PFD_LOW                    100
#define BBPLL_CNTL_INT_MODE_HIGH                        105
#define BBPLL_CNTL_INT_MODE_LOW                         105
#define BBPLL_CNTL_LP_CP_UBG_HIGH                       106
#define BBPLL_CNTL_LP_CP_UBG_LOW                        106
#define BBPLL_CNTL_PU_CP_HIGH                           107
#define BBPLL_CNTL_PU_CP_LOW                            107
#define BBPLL_CNTL_VCO_D2C_CNTL_HIGH                    111
#define BBPLL_CNTL_VCO_D2C_CNTL_LOW                     110
#define BBPLL_CNTL_VCO_KVCO_CNTL_HIGH                   113
#define BBPLL_CNTL_VCO_KVCO_CNTL_LOW                    112
#define BBPLL_CNTL_VCO_BGBIAS_HIGH                      115
#define BBPLL_CNTL_VCO_BGBIAS_LOW                       114
#define BBPLL_CNTL_VCO_CTATBIAS_HIGH                    117
#define BBPLL_CNTL_VCO_CTATBIAS_LOW                     116
#define BBPLL_CNTL_FILTER_EN_VCO_HIGH                   118
#define BBPLL_CNTL_FILTER_EN_VCO_LOW                    118
#define BBPLL_CNTL_PU_VCO_HIGH                          119
#define BBPLL_CNTL_PU_VCO_LOW                           119
#define BBPLL_CNTL_PU_MMD_HIGH                          120
#define BBPLL_CNTL_PU_MMD_LOW                           120
#define BBPLL_CNTL_REF_DIV2_HIGH                        121
#define BBPLL_CNTL_REF_DIV2_LOW                         121
#define BBPLL_CNTL_BYPCLK_EN_HIGH                       122
#define BBPLL_CNTL_BYPCLK_EN_LOW                        122
#define BBPLL_CNTL_I_EXTRA_EN_LDO_HIGH                  123
#define BBPLL_CNTL_I_EXTRA_EN_LDO_LOW                   123
#define BBPLL_CNTL_RXADCCLK_PD_HIGH                     124
#define BBPLL_CNTL_RXADCCLK_PD_LOW                      124
#define BBPLL_CNTL_RCCALCLK_PD_HIGH                     125
#define BBPLL_CNTL_RCCALCLK_PD_LOW                      125


#endif // __HW_ANALOG_H__
