#ifndef __HW_DFE_H__
#define __HW_DFE_H__

//*****************************************************************************
//
// The following are defines for DFE register address.
//
//*****************************************************************************
#define      DFE_RX_LO_FCW                     (0x00 + DFE_REG_BASE)
#define      DFE_RX_LO_PHS_ADJ                 (0x04 + DFE_REG_BASE)
#define      DFE_RX_SPUR_DDFS_TW               (0x08 + DFE_REG_BASE)
#define      DFE_RX_SPUR_MUGP                  (0x0c + DFE_REG_BASE)
#define      DFE_RX_DC_EST_IQ                  (0x10 + DFE_REG_BASE)
#define      DFE_RX_RSSI                       (0x14 + DFE_REG_BASE)
#define      DFE_RX_EN                         (0x18 + DFE_REG_BASE)
#define      DFE_RX_GAIN_CIC                   (0x1c + DFE_REG_BASE)
#define      DFE_RX_GAIN_ACI                   (0x20 + DFE_REG_BASE)
#define      RX_DC_AVG_NUM                     (0x22 + DFE_REG_BASE)
#define      DFE_RX_DLY_REG0                   (0x24 + DFE_REG_BASE)
#define      DFE_RX_DLY_REG1                   (0x28 + DFE_REG_BASE)
#define      DFE_RX_DLY_REG2                   (0x2c + DFE_REG_BASE)
#define      DFE_RX_DLY_REG3                   (0x30 + DFE_REG_BASE)
#define      DFE_RX_DLY_REG4                   (0x34 + DFE_REG_BASE)
#define      DFE_RX_DLY_REG5                   (0x38 + DFE_REG_BASE)
#define      DFE_RX_DLY_REG6                   (0x3c + DFE_REG_BASE)
#define      DFE_TX_LO_FCW                     (0x40 + DFE_REG_BASE)
#define      DFE_TX_LO_PHS_ADJ                 (0x44 + DFE_REG_BASE)
#define      DFE_TX_IQ_AMP                     (0x48 + DFE_REG_BASE)
#define      DFE_TX_IQ_PHS_EST                 (0x4c + DFE_REG_BASE)
#define      DFE_TX_CTRL_BYPASS                (0x50 + DFE_REG_BASE)
#define      DFE_TX_RF_TSSI_AVG_NUM            (0x54 + DFE_REG_BASE)
#define      DFE_TX_RF_TSSI_AVG                (0x58 + DFE_REG_BASE)
#define      DFE_TX_DC_EST_IQ                  (0x5c + DFE_REG_BASE)
#define      DFE_TX_EN                         (0x60 + DFE_REG_BASE)
#define      DFE_TX_DLY_REG1                   (0x64 + DFE_REG_BASE)
#define      DFE_DFE_FIFO_CTRL                 (0x68 + DFE_REG_BASE)
#define      DFE_DFE_FIFO_STA                  (0x6c + DFE_REG_BASE)
#define      DFE_DFE_FIFO_DATA_NUM             (0x70 + DFE_REG_BASE)
#define      DFE_TX_DLY_REG0                   (0x74 + DFE_REG_BASE)
#define      DFE_AD9361_RXFREQ_INI             (0x78 + DFE_REG_BASE)
#define      DFE_AD9361_TXFREQ_INI             (0x7c + DFE_REG_BASE)
#define      DFE_AD9361_GAIN_CTRL              (0x80 + DFE_REG_BASE)
#define      DFE_RX_DLY_REG7                   (0x84 + DFE_REG_BASE)
#define      DFE_RXHB_GAIN_CTRL                (0x88 + DFE_REG_BASE)
#define      DFE_RXLB_BB_GAIN_CTRL             (0x8c + DFE_REG_BASE)
#define      DFE_RXBB_TIA_OS_IQ                (0x90 + DFE_REG_BASE)
#define      DFE_TX_DLY_REG2                   (0x94 + DFE_REG_BASE)
#define      DFE_TX_DLY_REG3                   (0x98 + DFE_REG_BASE)
#define      DFE_TX_DLY_REG4                   (0x9c + DFE_REG_BASE)
#define      DFE_TXHB_GAIN_CTRL                (0xa0 + DFE_REG_BASE)
#define      DFE_TXLB_BB_GAIN_CTRL             (0xa4 + DFE_REG_BASE)
#define      DFE_TX_RF_BB_PU_CTRL              (0xa8 + DFE_REG_BASE)
#define      DFE_DFE_TMUX                      (0xac + DFE_REG_BASE)
#define      DFE_ANA_TXCFG0                    (0xb0 + DFE_REG_BASE)
#define      DFE_ANA_TXCFG1                    (0xb4 + DFE_REG_BASE)
#define      DFE_ANA_TXCFG2                    (0xb8 + DFE_REG_BASE)
#define      DFE_ANA_TXCFG3                    (0xbc + DFE_REG_BASE)
#define      DFE_ANA_TXCFG4                    (0xc0 + DFE_REG_BASE)
#define      DFE_ANA_TXCFG5                    (0xc4 + DFE_REG_BASE)
#define      DFE_ANA_TXCFG6                    (0xc8 + DFE_REG_BASE)
#define      DFE_ANA_TXCFG7                    (0xcc + DFE_REG_BASE)
#define      DFE_ANA_TXCFG8                    (0xd0 + DFE_REG_BASE)
#define      DFE_ANA_PLL1                      (0xd4 + DFE_REG_BASE)
#define      DFE_ANA_PLL2                      (0xd8 + DFE_REG_BASE)
#define      DFE_ANA_PLL3                      (0xdc + DFE_REG_BASE)
#define      DFE_ANA_PLL4                      (0xe0 + DFE_REG_BASE)
#define      DFE_ANA_PLL5                      (0xe4 + DFE_REG_BASE)
#define      DFE_ANA_PLL6                      (0xe8 + DFE_REG_BASE)
#define      DFE_ANA_PLL7                      (0xec + DFE_REG_BASE)
#define      DFE_ANA_PLL8                      (0xf0 + DFE_REG_BASE)
#define      DFE_ANA_PLL9                      (0xf4 + DFE_REG_BASE)
#define      DFE_ANA_RXPWR_CTL                 (0xf8 + DFE_REG_BASE)
#define      DFE_ANA_RXCFG1                    (0xfc + DFE_REG_BASE)
#define      DFE_ANA_RXCFG2                    (0x100 + DFE_REG_BASE)
#define      DFE_ANA_RXCFG3                    (0x104 + DFE_REG_BASE)
#define      DFE_ANA_RXCFG4                    (0x108 + DFE_REG_BASE)
#define      DFE_ANA_RXCFG5                    (0x10c + DFE_REG_BASE)
#define      DFE_ANA_RXCFG6                    (0x110 + DFE_REG_BASE)
#define      DFE_ANA_RXCFG7                    (0x114 + DFE_REG_BASE)
#define      DFE_ANA_RXCFG8                    (0x118 + DFE_REG_BASE)

//=============================================================================
#define      RX_SPUR_MUGP						(0xC + DFE_REG_BASE)
#define      RX_SPUR_CTRL_DLY_BYPASS			(0xe + DFE_REG_BASE)
#define      RX_DC_EST_I						(0x10 + DFE_REG_BASE)
#define      RX_DC_EST_Q						(0x11 + DFE_REG_BASE)
#define      RX_RSSI_CIC0						(0x14  + DFE_REG_BASE)
#define      RX_RSSI_CIC1						(0x15  + DFE_REG_BASE)
#define      RX_RSSI_CIC1_CTRL_DLY6_RDY			(0x15  + DFE_REG_BASE)
#define      RX_RSSI_ACI0						(0x16  + DFE_REG_BASE)
#define      RX_RSSI_ACI1						(0x17  + DFE_REG_BASE)
#define      RX_DC_AVG_RDY_AVG_EN				(0x17  + DFE_REG_BASE)
#define      TX_IQ_PHS_EST_I0					(0x4a + DFE_REG_BASE)
#define      TX_IQ_PHS_EST_Q0					(0x4c + DFE_REG_BASE)
#define      TX_DVGA_GAIN0						(0x4e + DFE_REG_BASE)
#define      TX_DDFS_BYPASS						(0x50 + DFE_REG_BASE)
#define      TX_MODE_CTRL_DLY_BYPASS			(0x51 + DFE_REG_BASE)
#define      TX_DC_COMP_BYPASS					(0x52 + DFE_REG_BASE)
#define      TX_IQ_COMP_BYPASS					(0x53 + DFE_REG_BASE)
#define      TX_RF_TSSI_AVG_NUM					(0x54 + DFE_REG_BASE)
#define      TX_DC_EST_I0						(0x5c + DFE_REG_BASE)
#define      TX_DC_EST_Q0						(0x5e + DFE_REG_BASE)
#define      DFE_TESTMUX						(0xac + DFE_REG_BASE)
#define      TX_EN								(0x60 + DFE_REG_BASE)
#define      DFE_FIFO_CTRL0						(0x68 + DFE_REG_BASE)
#define      DFE_FIFO_CTRL1						(0x69 + DFE_REG_BASE)
#define      DFE_FIFO_CTRL2						(0x6A + DFE_REG_BASE)
#define      DFE_FIFO_CTRL3						(0x6B + DFE_REG_BASE)


#define      DFE_FIFO_STATUS0					(0x6c + DFE_REG_BASE)
#define      DFE_FIFO_STATUS1					(0x6d + DFE_REG_BASE)
#define      DFE_FIFO_STATUS2					(0x6e + DFE_REG_BASE)


//*****************************************************************************
//
// The following are defines for the bit fields in the RX_SPUR_MUGP register.
//
//*****************************************************************************
#define RX_SPUR_MUG_POS           		0
#define RX_SPUR_MUG_MSK               	(0XFUL << RX_SPUR_MUG_POS)
#define RX_SPUR_MUG0                  	(1UL << RX_SPUR_MUG_POS)
#define RX_SPUR_MUG1                  	(2UL << RX_SPUR_MUG_POS)
#define RX_SPUR_MUG2                  	(4UL << RX_SPUR_MUG_POS)
#define RX_SPUR_MUG3                  	(8UL << RX_SPUR_MUG_POS)
#define RX_SPUR_MUP_POS           		4
#define RX_SPUR_MUP_MSK           	  	(0XFUL << RX_SPUR_MUP_POS)
#define RX_SPUR_MUP0                  	(1UL << RX_SPUR_MUP_POS)
#define RX_SPUR_MUP1                  	(2UL << RX_SPUR_MUP_POS)
#define RX_SPUR_MUP2                  	(4UL << RX_SPUR_MUP_POS)
#define RX_SPUR_MUP3                  	(8UL << RX_SPUR_MUP_POS)
#define RX_SPUR_BYPASS_POS			    16
#define RX_SPUR_BYPASS_MSK	        	(1UL << RX_SPUR_BYPASS_POS)
#define RX_SPUR_BYPASS_SET				(1UL << RX_SPUR_BYPASS_POS)
#define RX_CTRL_DLY_BYPASS_POS			20
#define RX_CTRL_DLY_BYPASS_MSK			(1UL << RX_CTRL_DLY_BYPASS_POS)
#define RX_CTRL_DLY_BYPASS_SET			(1UL << RX_CTRL_DLY_BYPASS_POS)

//*****************************************************************************
//
// The following are defines for the bit fields in the RX_DC_EST_I_Q register.
//
//*****************************************************************************
#define RX_DC_EST_I_POS           		0
#define RX_DC_EST_I_MSK              	(0XFFUL << RX_DC_EST_I_POS)
#define RX_DC_EST_Q_POS           		8
#define RX_DC_EST_Q_MSK              	(0XFFUL << RX_DC_EST_Q_POS)

//*****************************************************************************
//*****************************************************************************
//
// The following are defines for the bit fields in the rx_rssi_cic register.
//
//*****************************************************************************
#define RX_RSSI_CIC_POS           		0
#define RX_RSSI_CIC_MSK              	(0X3FFUL << RX_RSSI_CIC_POS)
#define RX_CTRL_DLY6_RDY_POS         	12
#define RX_CTRL_DLY6_RDY_MSK         	(0X3UL << RX_CTRL_DLY6_RDY_POS)
#define RX_RSSI_ACI_POS           		16
#define RX_RSSI_ACI_MSK              	(0X3FFUL << RX_RSSI_ACI_POS)
#define RX_DC_AVG_RDY_POS           	16
#define RX_DC_AVG_RDY_MSK             	(0X3FFUL << RX_DC_AVG_RDY_POS)
#define RX_DC_AVG_EN_POS           		17
#define RX_DC_AVG_EN_MSK              	(0X3FFUL << RX_DC_AVG_EN_POS)

//*****************************************************************************
//*****************************************************************************
//
// The following are defines for the bit fields in the RX_EN register.
//
//*****************************************************************************
#define RX_EN_POS           			0
#define RX_EN_MSK               		(1UL << RX_EN_POS)
//*****************************************************************************
//
// The following are defines for the bit fields in the rx_gain_aci register.  //0x20
//
//*****************************************************************************
#define RX_GAIN_ACI_POS           		0
#define RX_GAIN_ACI_MSK					(0XFFFFUL << RX_GAIN_ACI_POS)
#define RX_DC_AVG_NUM_POS				16
#define RX_DC_AVG_NUM_MSK				(3UL << RX_DC_AVG_NUM_POS)
#define RX_DC_AVG_NUM0_SET				(1UL << RX_DC_AVG_NUM_POS)
#define RX_DC_AVG_NUM1_SET				(2UL << RX_DC_AVG_NUM_POS)
//*****************************************************************************
//
// The following are defines for the bit fields in the TX_DDFS_BYPASS register. //0x50
//
//*****************************************************************************
#define TX_DDFS_BYPASS_POS				0
#define TX_DDFS_BYPASS_SET				(1UL << TX_DDFS_BYPASS_POS)
#define TX_DLY_TEST_POS					4
#define TX_DLY_TEST_SET					(1UL << TX_DLY_TEST_POS)
#define TX_MODE_POS						8
#define TX_MODE_SET						(1UL<<TX_MODE_POS)
#define TX_CTRL_DLY_BYPASS_POS			9
#define TX_CTRL_DLY_BYPASS_SET			(1UL<<TX_CTRL_DLY_BYPASS_POS)
#define RXADC_CLKPOL_SEL_POS			12
#define RXADC_CLKPOL_SEL_SET			(1UL<<RXADC_CLKPOL_SEL_POS)
#define TXDAC_CLKPOL_SEL_POS			13
#define TXDAC_CLKPOL_SEL_SET			(1UL<<TXDAC_CLKPOL_SEL_POS)
#define TX_DC_COMP_BYPASS_POS			16
#define TX_DC_COMP_BYPASS_SET			(1UL<<TX_DC_COMP_BYPASS_POS)
#define TX_IQ_COMP_BYPASS_POS			24
#define TX_IQ_COMP_BYPASS_SET			(1UL<<TX_IQ_COMP_BYPASS_POS)
//*****************************************************************************
//
// The following are defines for the bit fields in the TX_RF_TSSI_AVG_NUM register.
//
//*****************************************************************************
#define TX_RF_TSSI_AVG_NUM_POS	 		0
#define TX_RF_TSSI_AVG_NUM_MSK			(3UL << TX_RF_TSSI_AVG_NUM_POS)
#define TX_RF_TSSI_AVG_NUM0_SET			(1UL << TX_RF_TSSI_AVG_NUM_POS)
#define TX_RF_TSSI_AVG_NUM1_SET			(2UL << TX_RF_TSSI_AVG_NUM_POS)
#define TX_RF_TSSI_AVG_NUM2_SET			(4UL << TX_RF_TSSI_AVG_NUM_POS)
#define TX_RF_TSSI_AVG_NUM3_SET			(8UL << TX_RF_TSSI_AVG_NUM_POS)
//*****************************************************************************
//
// The following are defines for the bit fields in the TX_RF_TSSI_AVG register.
//
//*****************************************************************************
#define TX_RF_TSSI_AVG_POS	 			0
#define TX_RF_TSSI_AVG_MSK	 			(0XFFFUL << TX_RF_TSSI_AVG_POS)

//*****************************************************************************
//
// The following are defines for the bit fields in the TX_DC_EST_IQ register.
//
//*****************************************************************************
#define TX_DC_EST_I0_POS	 			0
#define TX_DC_EST_I0_MSK	 			(0XFFFUL << TX_DC_EST_I0_POS)
#define TX_DC_EST_Q0_POS	 			16
#define TX_DC_EST_Q0_MSK	 			(0XFFFUL << TX_DC_EST_Q0_POS)

//*****************************************************************************
//
// The following are defines for the bit fields in the TX_EN register.
//
//*****************************************************************************
#define TX_EN_POS	 					0
#define TX_EN_SET	 					(1UL << TX_EN_POS)

//*****************************************************************************
//
// The following are defines for the bit fields in the DFE_FIFO_CTRL register.
//
//*****************************************************************************
#define INT_EN_POS						0
#define INT_EN_SET						(1UL << INT_EN_POS)
#define RW_BYPASS_TEST_POS	 			4
#define RW_BYPASS_TEST_SET	 			(1UL << RW_BYPASS_TEST_POS)
#define INT_MASK_POS	 				8
#define INT_MASK_MSK	 				(0X7FFFFUL << INT_MASK_POS)
//*****************************************************************************
//
// The following are defines for the bit fields in the wr_lvl_sta register.
//
//*****************************************************************************
#define WR_LVL_STA_POS					0
#define WR_LVL_STA_MSK					(0XFFUL << WR_LVL_STA_POS)
#define RD_LVL_STA_POS					8
#define RD_LVL_STA_MSK					(0XFFUL << RD_LVL_STA_POS)
#define FIFO_EMPTY_STA_POS				16
#define FIFO_EMPTY_STA_MSK				(0X1UL << FIFO_EMPTY_STA_POS)
#define FIFO_FULL_STA_POS				17
#define FIFO_FULL_STA_MSK				(0X1UL << FIFO_FULL_STA_POS)
#define ALMOST_FULL_STA_POS				18
#define ALMOST_FULL_STA_MSK				(0X1UL << ALMOST_FULL_STA_POS)
//*****************************************************************************
//
// The following are defines for the bit fields in the rxgain_ini register.
//
//*****************************************************************************
#define RXGAIN_INI_POS					0
#define RXGAIN_INI_MSK					(0X7FUL << RXGAIN_INI_POS)
#define TXATT_INI_POS					16
#define TXATT_INI_MSK					(0X1FFUL << TXATT_INI_POS)
//*****************************************************************************
//
// The following are defines for the bit fields in the RXHB_LNA register.
//
//*****************************************************************************
#define RXHB_LNA_GC_POS	 				0
#define RXHB_LNA_GC_MSK	 				(0X1FUL << RXHB_LNA_GC_POS)
#define RXHB_LNA_RO_ADJ_POS	 			8
#define RXHB_LNA_RO_ADJ_MSK	 			(0XFUL << RXHB_LNA_RO_ADJ_POS)
#define RXHB_LNA_RST_BYPVAL_POS	 		12
#define RXHB_LNA_RST_BYPVAL_MSK	 		(0XFUL << RXHB_LNA_RST_BYPVAL_POS)
#define RXHB_GM_GC_POS					16	
#define RXHB_GM_GC_MSK	 				(0XFUL << RXHB_GM_GC_POS)
#define RXHB_GM_SD_ADJ_POS   			20
#define RXHB_GM_SD_ADJ_MSK	 			(0XFUL << RXHB_GM_SD_ADJ_POS)
//*****************************************************************************
//
// The following are defines for the bit fields in the RXLB_LNA_GC register.
//
//****************************************************************************
#define RXLB_LNA_GC_POS	 				0
#define RXLB_LNA_GC_MSK	 				(0X7UL << RXLB_LNA_GC_POS)
#define RXLB_GM_GC_POS	 				8
#define RXLB_GM_GC_MSK	 				(0XFUL << RXLB_GM_GC_POS)
#define RXLB_GM_SD_ADJ_POS				12  ///
#define RXLB_GM_SD_ADJ_MSK				(0XFUL << RXLB_GM_SD_ADJ_POS)
#define RXBB_PGA_GC_POS					16
#define RXBB_PGA_GC_MSK	 				(0X1FUL << RXBB_PGA_GC_POS)
//*****************************************************************************
//
// The following are defines for the bit fields in the RXBB_TIA_OS_PUSH_IH register.
//
//****************************************************************************
#define RXBB_TIA_OS_PUSH_I_POS	 		0
#define RXBB_TIA_OS_PUSH_I_MSK	 		(0X3FUL << RXBB_TIA_OS_PUSH_I_POS)
#define RXBB_TIA_BYP_EN_POS	 			7
#define RXBB_TIA_BYP_EN_SET	 			(0X1UL << RXBB_TIA_BYP_EN_POS)
#define RXBB_TIA_OS_PULL_I_POS			8  ///
#define RXBB_TIA_OS_PULL_I_MSK			(0X3FUL << RXBB_TIA_OS_PULL_I_POS)
#define RXBB_TIA_OS_PUSH_Q_POS 			16
#define RXBB_TIA_OS_PUSH_Q_MSK			(0X3FUL << RXBB_TIA_OS_PUSH_Q_POS)
#define RXBB_TIA_OS_PULL_Q_POS			24	
#define RXBB_TIA_OS_PULL_Q_MSK	 		(0X3FUL << RXBB_TIA_OS_PULL_Q_POS)
//*****************************************************************************
//
// The following are defines for the bit fields in the TXHB_MX_CAS_GC register.
//
//****************************************************************************
#define TXHB_MX_CAS_GC_POS				0
#define TXHB_MX_CAS_GC_MSK				(0X3UL << TXHB_MX_CAS_GC_POS)
#define TXHB_PAD_GC_POS					8  ///
#define TXHB_PAD_GC_MSK					(0XFFUL << TXHB_PAD_GC_POS)
#define TXHB_PA_GC_POS 					16
#define TXHB_PA_GC_MSK					(0XFFUL << TXHB_PA_GC_POS)
//*****************************************************************************
//
// The following are defines for the bit fields in the TXLB_MX_CAS_GC register.
//
//****************************************************************************
#define TXLB_MX_CAS_GC_POS				0
#define TXLB_MX_CAS_GC_MSK				(0X3FUL << TXLB_MX_CAS_GC_POS)
#define TXLB_PA_GC_POS					8  ///
#define TXLB_PA_GC_MSK					(0XFFUL << TXLB_PA_GC_POS)
#define TXBB_GC_POS 					16
#define TXBB_GC_MSK						(0XFUL << TXBB_GC_POS)
//*****************************************************************************
//
// The following are defines for the bit fields in the TXBB_PU register.
//
//****************************************************************************
#define TXBB_PU_POS	 					0
#define TXBB_PU_SET	 					(0X1UL << TXBB_PU_POS)
#define TXBB_RSTB_POS					1  ///
#define TXBB_RSTB_SET					(0X1UL << TXBB_RSTB_POS)
#define TXHB_BIAS_PU_POS				8  ///
#define TXHB_BIAS_PU_SET				(0X1UL << TXHB_BIAS_PU_POS)
#define TXHB_PA_PU_POS					10  ///
#define TXHB_PA_PU_SET					(0X1UL << TXHB_PA_PU_POS)
#define TXHB_PAD_PU_POS					12  ///
#define TXHB_PAD_PU_SET					(0X1UL << TXHB_PAD_PU_POS)
#define TXHB_MX_GM_BIAS_RST_POS			13 ///
#define TXHB_MX_GM_BIAS_RST_SET			(0X1UL << TXHB_MX_GM_BIAS_RST_POS)
#define TXHB_MX_PU_POS					14///
#define TXHB_MX_PU_SET					(0X1UL << TXHB_MX_PU_POS)
#define TXHB_MX_LO_BIAS_RST_POS			15///
#define TXHB_MX_LO_BIAS_RST_SET			(0X1UL << TXHB_MX_LO_BIAS_RST_POS)
#define TXLB_BIAS_PU_POS				16///
#define TXLB_BIAS_PU_SET				(0X1UL << TXLB_BIAS_PU_POS)
#define TXLB_MX_PU_POS					18///
#define TXLB_MX_PU_SET					(0X1UL << TXLB_MX_PU_POS)
#define TXLB_MX_LO_BIAS_RST_POS			19///
#define TXLB_MX_LO_BIAS_RST_SET			(0X1UL << TXLB_MX_LO_BIAS_RST_POS)
#define TXLB_PA_PU_POS					20///
#define TXLB_PA_PU_SET					(0X1UL << TXLB_PA_PU_POS)
#define TXLB_PA_GM_BIAS_RST_POS			21///
#define TXLB_PA_GM_BIAS_RST_SET			(0X1UL << TXLB_PA_GM_BIAS_RST_POS)
//==========================================================================================
//*****************************************************************************
//
// The following are defines for the bit fields in the testmux_sel register.
//
//****************************************************************************
#define TESTMUX_SEL_POS				0
#define TESTMUX_SEL_MSK				(0X3FUL << TESTMUX_SEL_POS)
#define TESTMUX_SEL0_SET			(0X1UL << TESTMUX_SEL_POS)
#define TESTMUX_SEL1_SET			(0X2UL << TESTMUX_SEL_POS)
#define TESTMUX_SEL2_SET			(0X4UL << TESTMUX_SEL_POS)
#define TESTMUX_SEL3_SET			(0X8UL << TESTMUX_SEL_POS)
#define TESTMUX_SEL4_SET			(0X10UL << TESTMUX_SEL_POS)
//==========================================================================================================
//*****************************************************************************
//
// The following are defines for the bit fields in the ANATXPWRCTL register.
//
//****************************************************************************
#define O_TXLDO_PALDO_PU_POS		0
#define O_TXLDO_PALDO_PU_MSK		(0X1UL << O_TXLDO_PALDO_PU_POS)
#define O_TXLDO_PALDO_PU_SET		(0X1UL << O_TXLDO_PALDO_PU_POS)
#define O_TXHB_TSSI_PU_POS			2
#define O_TXHB_TSSI_PU_MSK			(0X1UL << O_TXHB_TSSI_PU_POS)
#define O_TXHB_TSSI_PU_SET			(0X1UL << O_TXHB_TSSI_PU_POS)
#define O_TXLB_TSSI_PU_POS			3
#define O_TXLB_TSSI_PU_MSK			(0X1UL << O_TXLB_TSSI_PU_POS)
#define O_TXLB_TSSI_PU_SET			(0X1UL << O_TXLB_TSSI_PU_POS)
#define O_TXHB_TSSI_ATT_EN_POS		4
#define O_TXHB_TSSI_ATT_EN_MSK		(0X1UL << O_TXHB_TSSI_ATT_EN_POS)
#define O_TXHB_TSSI_ATT_EN_SET		(0X1UL << O_TXHB_TSSI_ATT_EN_POS)
#define O_TXLB_TSSI_ATT_EN_POS		5
#define O_TXLB_TSSI_ATT_EN_MSK		(0X1UL << O_TXLB_TSSI_ATT_EN_POS)
#define O_TXLB_TSSI_ATT_EN_SET		(0X1UL << O_TXLB_TSSI_ATT_EN_POS)
#define O_TXLDO_PALDO_CNTL_POS		8
#define O_TXLDO_PALDO_CNTL_MSK		(0XFFFFUL << O_TXLDO_PALDO_CNTL_POS)
#define O_TXLDO_PALDO_VADJ_POS		24
#define O_TXLDO_PALDO_VADJ_MSK		(0XFUL << O_TXLDO_PALDO_VADJ_POS)
//===========================================================================================================
#endif // __HW_DFE_H__

