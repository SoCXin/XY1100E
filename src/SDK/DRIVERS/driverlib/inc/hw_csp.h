#ifndef __HW_CSP_H__
#define __HW_CSP_H__


//*****************************************************************************
//
// The following are defines for the CSP register offsets.
//
//*****************************************************************************
#define	CSP_MODE1								0x00000000
#define	CSP_MODE2                   			0x00000004
#define	CSP_TX_FRAME_CTRL           			0x00000008
#define	CSP_RX_FRAME_CTRL           			0x0000000C
#define	CSP_TX_RX_ENABLE	        			0x00000010
#define	CSP_INT_ENABLE              			0x00000014
#define	CSP_INT_STATUS              			0x00000018
#define	CSP_PIN_IO_DATA             			0x0000001c
// 0x00000020 Reserved.		
#define	CSP_AYSNC_PARAM_REG         			0x00000024
#define	CSP_IRDA_X_MODE_DIV         			0x00000028
#define	CSP_SM_CFG                  			0x0000002c
#define	CSP_TX_DMA_IO_CTRL          			0x00000100
#define	CSP_TX_DMA_IO_LEN           			0x00000104
#define	CSP_TX_FIFO_CTRL            			0x00000108
#define	CSP_TX_FIFO_LEVEL_CHK       			0x0000010c
#define	CSP_TX_FIFO_OP              			0x00000110
#define	CSP_TX_FIFO_STATUS          			0x00000114
#define	CSP_TX_FIFO_DATA            			0x00000118
#define	CSP_RX_DMA_IO_CTRL          			0x00000120
#define	CSP_RX_DMA_IO_LEN           			0x00000124
#define	CSP_RX_FIFO_CTRL            			0x00000128
#define	CSP_RX_FIFO_LEVEL_CHK       			0x0000012C
#define	CSP_RX_FIFO_OP              			0x00000130
#define	CSP_RX_FIFO_STATUS          			0x00000134
#define	CSP_RX_FIFO_DATA            			0x00000138



//*****************************************************************************
//
// The following are defines for the bit fields in the CSP_MODE1 register.
//
//*****************************************************************************
#define	CSP_MODE1_SYNC_MODE_Pos					0
#define	CSP_MODE1_SYNC_MODE_Msk					(1UL << CSP_MODE1_SYNC_MODE_Pos)
#define	CSP_MODE1_SYNC                          (1UL << CSP_MODE1_SYNC_MODE_Pos)
#define	CSP_MODE1_ASYN                          (0UL << CSP_MODE1_SYNC_MODE_Pos)
#define	CSP_MODE1_CLOCK_MODE_Pos				1
#define	CSP_MODE1_CLOCK_MODE_Msk				(1UL << CSP_MODE1_CLOCK_MODE_Pos)
#define	CSP_MODE1_CLOCK_MODE_Master             (0UL << CSP_MODE1_CLOCK_MODE_Pos)
#define	CSP_MODE1_CLOCK_MODE_Slave              (1UL << CSP_MODE1_CLOCK_MODE_Pos)
#define	CSP_MODE1_LOOP_BACK_EN_Pos				2
#define	CSP_MODE1_LOOP_BACK_EN_Msk				(1UL << CSP_MODE1_LOOP_BACK_EN_Pos)
#define	CSP_MODE1_HPSIR_EN_Pos					3
#define	CSP_MODE1_HPSIR_EN_Msk					(1UL << CSP_MODE1_HPSIR_EN_Pos)
#define	CSP_MODE1_HPSIR_EN					    (1UL << CSP_MODE1_HPSIR_EN_Pos)
#define	CSP_MODE1_ENDIAN_CTRL_Pos				4
#define	CSP_MODE1_ENDIAN_CTRL_Msk				(1UL << CSP_MODE1_ENDIAN_CTRL_Pos)
#define	CSP_MODE1_ENDIAN_MSBF				    (0UL << CSP_MODE1_ENDIAN_CTRL_Pos)
#define	CSP_MODE1_ENDIAN_LSBF				    (1UL << CSP_MODE1_ENDIAN_CTRL_Pos)
#define	CSP_MODE1_CSP_EN_Pos					5
#define	CSP_MODE1_CSP_EN_Msk					(1UL << CSP_MODE1_CSP_EN_Pos)
#define	CSP_MODE1_CSP_EN					    (1UL << CSP_MODE1_CSP_EN_Pos)
#define	CSP_MODE1_RXD_ACT_EDGE_Pos				6
#define	CSP_MODE1_RXD_ACT_EDGE_Msk				(1UL << CSP_MODE1_RXD_ACT_EDGE_Pos)
#define	CSP_MODE1_RXD_ACT_RISE                  (0UL << CSP_MODE1_RXD_ACT_EDGE_Pos)
#define	CSP_MODE1_RXD_ACT_FALL                  (1UL << CSP_MODE1_RXD_ACT_EDGE_Pos)
#define	CSP_MODE1_TXD_ACT_EDGE_Pos				7
#define	CSP_MODE1_TXD_ACT_EDGE_Msk				(1UL << CSP_MODE1_TXD_ACT_EDGE_Pos)
#define	CSP_MODE1_TXD_ACT_RISE                  (0UL << CSP_MODE1_TXD_ACT_EDGE_Pos)
#define	CSP_MODE1_TXD_ACT_FALL                  (1UL << CSP_MODE1_TXD_ACT_EDGE_Pos)
#define	CSP_MODE1_RFS_ACT_LEVEL_Pos				8
#define	CSP_MODE1_RFS_ACT_LEVEL_Msk				(1UL << CSP_MODE1_RFS_ACT_LEVEL_Pos)
#define	CSP_MODE1_TFS_ACT_LEVEL_Pos				9
#define	CSP_MODE1_TFS_ACT_LEVEL_Msk				(1UL << CSP_MODE1_TFS_ACT_LEVEL_Pos)
#define	CSP_MODE1_CLK_IDLE_MODE_Pos			    10
#define	CSP_MODE1_CLK_IDLE_MODE_Msk			    (1UL << CSP_MODE1_CLK_IDLE_MODE_Pos)
#define CSP_MODE1_CLK_IDLE_KEEP_LEVEL           (0UL << CSP_MODE1_CLK_IDLE_MODE_Pos)
#define CSP_MODE1_CLK_IDLE_TOGGLE               (1UL << CSP_MODE1_CLK_IDLE_MODE_Pos)
#define	CSP_MODE1_CLK_IDLE_LEVEL_Pos			11
#define	CSP_MODE1_CLK_IDLE_LEVEL_Msk			(1UL << CSP_MODE1_CLK_IDLE_LEVEL_Pos)
#define	CSP_MODE1_CLK_IDLE_LOGIC0			    (0UL << CSP_MODE1_CLK_IDLE_LEVEL_Pos)
#define	CSP_MODE1_CLK_IDLE_LOGIC1			    (1UL << CSP_MODE1_CLK_IDLE_LEVEL_Pos)
#define	CSP_MODE1_CLK_PIN_MODE_Pos				12
#define	CSP_MODE1_CLK_PIN_MODE_Msk				(1UL << CSP_MODE1_CLK_PIN_MODE_Pos)
#define	CSP_MODE1_CLK_PIN_MODE_CSP				(0UL << CSP_MODE1_CLK_PIN_MODE_Pos)
#define	CSP_MODE1_CLK_PIN_MODE_IO				(1UL << CSP_MODE1_CLK_PIN_MODE_Pos)
#define	CSP_MODE1_RFS_PIN_MODE_Pos				13
#define	CSP_MODE1_RFS_PIN_MODE_Msk				(1UL << CSP_MODE1_RFS_PIN_MODE_Pos)
#define	CSP_MODE1_RFS_PIN_MODE_CSP				(0UL << CSP_MODE1_RFS_PIN_MODE_Pos)
#define	CSP_MODE1_RFS_PIN_MODE_IO				(1UL << CSP_MODE1_RFS_PIN_MODE_Pos)
#define	CSP_MODE1_TFS_PIN_MODE_Pos				14
#define	CSP_MODE1_TFS_PIN_MODE_Msk				(1UL << CSP_MODE1_TFS_PIN_MODE_Pos)
#define	CSP_MODE1_TFS_PIN_MODE_CSP				(0UL << CSP_MODE1_TFS_PIN_MODE_Pos)
#define	CSP_MODE1_TFS_PIN_MODE_IO				(1UL << CSP_MODE1_TFS_PIN_MODE_Pos)
#define	CSP_MODE1_RXD_PIN_MODE_Pos				15
#define	CSP_MODE1_RXD_PIN_MODE_Msk				(1UL << CSP_MODE1_RXD_PIN_MODE_Pos)
#define	CSP_MODE1_RXD_PIN_MODE_CSP				(0UL << CSP_MODE1_RXD_PIN_MODE_Pos)
#define	CSP_MODE1_RXD_PIN_MODE_IO				(1UL << CSP_MODE1_RXD_PIN_MODE_Pos)
#define	CSP_MODE1_TXD_PIN_MODE_Pos				16
#define	CSP_MODE1_TXD_PIN_MODE_Msk				(1UL << CSP_MODE1_TXD_PIN_MODE_Pos)
#define	CSP_MODE1_TXD_PIN_MODE_CSP				(0UL << CSP_MODE1_TXD_PIN_MODE_Pos)
#define	CSP_MODE1_TXD_PIN_MODE_IO				(1UL << CSP_MODE1_TXD_PIN_MODE_Pos)
#define	CSP_MODE1_SCLK_IO_MODE_Pos				17
#define	CSP_MODE1_SCLK_IO_MODE_Msk				(1UL << CSP_MODE1_SCLK_IO_MODE_Pos)
#define	CSP_MODE1_SCLK_IO_MODE_Output			(0UL << CSP_MODE1_SCLK_IO_MODE_Pos)
#define	CSP_MODE1_SCLK_IO_MODE_Input			(1UL << CSP_MODE1_SCLK_IO_MODE_Pos)
#define	CSP_MODE1_RFS_IO_MODE_Pos				18
#define	CSP_MODE1_RFS_IO_MODE_Msk				(1UL << CSP_MODE1_RFS_IO_MODE_Pos)
#define	CSP_MODE1_RFS_IO_MODE_Output            (0UL << CSP_MODE1_RFS_IO_MODE_Pos)
#define	CSP_MODE1_RFS_IO_MODE_Input             (1UL << CSP_MODE1_RFS_IO_MODE_Pos)
#define	CSP_MODE1_TFS_IO_MODE_Pos				19
#define	CSP_MODE1_TFS_IO_MODE_Msk				(1UL << CSP_MODE1_TFS_IO_MODE_Pos)
#define	CSP_MODE1_TFS_IO_MODE_Output            (0UL << CSP_MODE1_TFS_IO_MODE_Pos)
#define	CSP_MODE1_TFS_IO_MODE_Input             (1UL << CSP_MODE1_TFS_IO_MODE_Pos)
#define	CSP_MODE1_RXD_IO_MODE_Pos				20
#define	CSP_MODE1_RXD_IO_MODE_Msk				(1UL << CSP_MODE1_RXD_IO_MODE_Pos)
#define	CSP_MODE1_RXD_IO_MODE_Output			(0UL << CSP_MODE1_RXD_IO_MODE_Pos)
#define	CSP_MODE1_RXD_IO_MODE_Input		    	(1UL << CSP_MODE1_RXD_IO_MODE_Pos)
#define	CSP_MODE1_TXD_IO_MODE_Pos				21
#define	CSP_MODE1_TXD_IO_MODE_Msk				(1UL << CSP_MODE1_TXD_IO_MODE_Pos)
#define	CSP_MODE1_TXD_IO_MODE_Output			(0UL << CSP_MODE1_TXD_IO_MODE_Pos)
#define	CSP_MODE1_TXD_IO_MODE_Input		    	(1UL << CSP_MODE1_TXD_IO_MODE_Pos)
#define	CSP_MODE1_IRDA_WIDTH_DIV_Pos			22
#define	CSP_MODE1_IRDA_WIDTH_DIV_Msk			(0xFFUL << CSP_MODE1_IRDA_WIDTH_DIV_Pos)
#define	CSP_MODE1_IrDA_IDLE_LEVEL_Pos			30
#define	CSP_MODE1_IrDA_IDLE_LEVEL_Msk			(1UL << CSP_MODE1_IrDA_IDLE_LEVEL_Pos)
#define	CSP_MODE1_IrDA_IDLE_LEVEL_HIGH			(1UL << CSP_MODE1_IrDA_IDLE_LEVEL_Pos)
#define	CSP_MODE1_IrDA_IDLE_LEVEL_LOW			(0UL << CSP_MODE1_IrDA_IDLE_LEVEL_Pos)
#define	CSP_MODE1_TX_UFLOW_REPEAT_Pos			31
#define	CSP_MODE1_TX_UFLOW_REPEAT_Msk			(1UL << CSP_MODE1_TX_UFLOW_REPEAT_Pos)


//*****************************************************************************
//
// The following are defines for the bit fields in the CSP_MODE2 register.
//
//*****************************************************************************
#define	CSP_MODE2_RXD_DELAY_LEN_Pos				0
#define	CSP_MODE2_RXD_DELAY_LEN_Msk				(0xFF << CSP_MODE2_RXD_DELAY_LEN_Pos)
#define	CSP_MODE2_TXD_DELAY_LEN_Pos				8
#define	CSP_MODE2_TXD_DELAY_LEN_Msk				(0xFF << CSP_MODE2_TXD_DELAY_LEN_Pos)
#define	CSP_MODE2_ENA_CTRL_MODE_Pos				16
#define	CSP_MODE2_ENA_CTRL_MODE_Msk				(1UL << CSP_MODE2_ENA_CTRL_MODE_Pos)
#define	CSP_MODE2_FRAME_CTRL_MODE_Pos			17
#define	CSP_MODE2_FRAME_CTRL_MODE_Msk			(1UL << CSP_MODE2_FRAME_CTRL_MODE_Pos)
#define	CSP_MODE2_TFS_SOURCE_MODE_Pos			18
#define	CSP_MODE2_TFS_SOURCE_MODE_Msk			(1UL << CSP_MODE2_TFS_SOURCE_MODE_Pos)
#define	CSP_MODE2_RFS_MS_MODE_Pos				19
#define	CSP_MODE2_RFS_MS_MODE_Msk				(1UL << CSP_MODE2_RFS_MS_MODE_Pos)
#define	CSP_MODE2_TFS_MS_MODE_Pos				20
#define	CSP_MODE2_TFS_MS_MODE_Msk				(1UL << CSP_MODE2_TFS_MS_MODE_Pos)
#define	CSP_MODE2_CSP_CLK_DIVISOR_Pos			21
#define	CSP_MODE2_CSP_CLK_DIVISOR_Msk			(0x3FF << CSP_MODE2_CSP_CLK_DIVISOR_Pos)
#define	CSP_MODE2_IRDA_DATA_WIDTH_Pos			31
#define	CSP_MODE2_IRDA_DATA_WIDTH_Msk			(1UL << CSP_MODE2_IRDA_DATA_WIDTH_Pos)
#define	CSP_MODE2_IRDA_DATA_WIDTH_3_16			(0UL << CSP_MODE2_IRDA_DATA_WIDTH_Pos)
#define	CSP_MODE2_IRDA_DATA_WIDTH_1_6		    (1UL << CSP_MODE2_IRDA_DATA_WIDTH_Pos)


//*****************************************************************************
//
// The following are defines for the bit fields in the CSP_TX_FRAME_CTRL register.
//
//*****************************************************************************
#define	CSP_TX_FRAME_CTRL_TX_DATA_LEN_Pos		0
#define	CSP_TX_FRAME_CTRL_TX_DATA_LEN_Msk		(0xFF << CSP_TX_FRAME_CTRL_TX_DATA_LEN_Pos)
#define	CSP_TX_FRAME_CTRL_TX_DATA_BITS_8		(7UL << CSP_TX_FRAME_CTRL_TX_DATA_LEN_Pos)
#define	CSP_TX_FRAME_CTRL_TX_DATA_BITS_7		(6UL << CSP_TX_FRAME_CTRL_TX_DATA_LEN_Pos)
#define	CSP_TX_FRAME_CTRL_TX_DATA_BITS_6		(5UL << CSP_TX_FRAME_CTRL_TX_DATA_LEN_Pos)
#define	CSP_TX_FRAME_CTRL_TX_SYNC_LEN_Pos		8
#define	CSP_TX_FRAME_CTRL_TX_SYNC_LEN_Msk		(0xFF << CSP_TX_FRAME_CTRL_TX_SYNC_LEN_Pos)
#define	CSP_TX_FRAME_CTRL_TX_SYNC_BITS_9		(8UL << CSP_TX_FRAME_CTRL_TX_SYNC_LEN_Pos)
#define	CSP_TX_FRAME_CTRL_TX_SYNC_BITS_8		(7UL << CSP_TX_FRAME_CTRL_TX_SYNC_LEN_Pos)
#define	CSP_TX_FRAME_CTRL_TX_FRAME_LEN_Pos		16
#define	CSP_TX_FRAME_CTRL_TX_FRAME_LEN_Msk		(0xFF << CSP_TX_FRAME_CTRL_TX_FRAME_LEN_Pos)
#define	CSP_TX_FRAME_CTRL_TX_SHIFTER_LEN_Pos	24
#define	CSP_TX_FRAME_CTRL_TX_SHIFTER_LEN_Msk	(0x1F << CSP_TX_FRAME_CTRL_TX_SHIFTER_LEN_Pos)
#define	CSP_TX_FRAME_CTRL_SLAVE_CLK_SAMPLE_Pos	29
#define	CSP_TX_FRAME_CTRL_SLAVE_CLK_SAMPLE_Msk	(1UL << CSP_TX_FRAME_CTRL_SLAVE_CLK_SAMPLE_Pos)
#define	CSP_TX_FRAME_CTRL_CSP_CLK_DIVISOR_Pos	30
#define	CSP_TX_FRAME_CTRL_CSP_CLK_DIVISOR_Msk	(3UL << CSP_TX_FRAME_CTRL_CSP_CLK_DIVISOR_Pos)


//*****************************************************************************
//
// The following are defines for the bit fields in the CSP_RX_FRAME_CTRL register.
//
//*****************************************************************************
#define	CSP_RX_FRAME_CTRL_RX_DATA_LEN_Pos		0
#define	CSP_RX_FRAME_CTRL_RX_DATA_LEN_Msk		(0xFF << CSP_RX_FRAME_CTRL_RX_DATA_LEN_Pos)
#define	CSP_RX_FRAME_CTRL_RX_FRAME_LEN_Pos		8
#define	CSP_RX_FRAME_CTRL_RX_FRAME_LEN_Msk		(0xFF << CSP_RX_FRAME_CTRL_RX_FRAME_LEN_Pos)
#define	CSP_RX_FRAME_CTRL_RX_SHIFTER_LEN_Pos	16
#define	CSP_RX_FRAME_CTRL_RX_SHIFTER_LEN_Msk	(0x1F << CSP_RX_FRAME_CTRL_RX_SHIFTER_LEN_Pos)
#define	CSP_RX_FRAME_CTRL_I2S_SYNC_CHG_Pos		21
#define	CSP_RX_FRAME_CTRL_I2S_SYNC_CHG_Msk		(1UL << CSP_RX_FRAME_CTRL_I2S_SYNC_CHG_Pos)


//*****************************************************************************
//
// The following are defines for the bit fields in the CSP_TX_RX_ENABLE register.
//
//*****************************************************************************
#define	CSP_TX_RX_ENABLE_RX_ENA_Pos				0
#define	CSP_TX_RX_ENABLE_RX_ENA				    (1UL << CSP_TX_RX_ENABLE_RX_ENA_Pos)
#define	CSP_TX_RX_ENABLE_TX_ENA_Pos				1
#define	CSP_TX_RX_ENABLE_TX_ENA				    (1UL << CSP_TX_RX_ENABLE_TX_ENA_Pos)


//*****************************************************************************
//
// The following are defines for the bit fields in the CSP_INT_ENABLE/STATUS register.
//
//*****************************************************************************
#define	CSP_INT_RX_DONE							(1UL << 0)
#define	CSP_INT_TX_DONE                         (1UL << 1)
#define	CSP_INT_RX_OFLOW                        (1UL << 2)
#define	CSP_INT_TX_UFLOW                        (1UL << 3)
#define	CSP_INT_DMA_IO_RX_DONE                  (1UL << 4)
#define	CSP_INT_DMA_IO_TX_DONE                  (1UL << 5)
#define	CSP_INT_RXFIFO_FULL                     (1UL << 6)
#define	CSP_INT_TXFIFO_EMPTY                    (1UL << 7)
#define	CSP_INT_RXFIFO_THD_REACH                (1UL << 8)
#define	CSP_INT_TXFIFO_THD_REACH                (1UL << 9)
#define	CSP_INT_UART_FRM_ERR                    (1UL << 10)
#define	CSP_INT_CSP_RX_TIMEOUT                  (1UL << 11)
#define	CSP_INT_CSP_TX_ALLOUT                   (1UL << 12)
#define	CSP_INT_ALL                             0x00001FFF

//*****************************************************************************
//
// The following are defines for the bit fields in the CSP_PIN_IO_DATA register.
//
//*****************************************************************************
#define	CSP_PIN_IO_DATA_RFS_PIN_VALUE_Pos		0
#define	CSP_PIN_IO_DATA_RFS_PIN_VALUE_Msk		(1UL << CSP_PIN_IO_DATA_RFS_PIN_VALUE_Pos)
#define	CSP_PIN_IO_DATA_TFS_PIN_VALUE_Pos		1
#define	CSP_PIN_IO_DATA_TFS_PIN_VALUE_Msk		(1UL << CSP_PIN_IO_DATA_TFS_PIN_VALUE_Pos)
#define	CSP_PIN_IO_DATA_RXD_PIN_VALUE_Pos		2
#define	CSP_PIN_IO_DATA_RXD_PIN_VALUE_Msk		(1UL << CSP_PIN_IO_DATA_RXD_PIN_VALUE_Pos)
#define	CSP_PIN_IO_DATA_TXD_PIN_VALUE_Pos		3
#define	CSP_PIN_IO_DATA_TXD_PIN_VALUE_Msk		(1UL << CSP_PIN_IO_DATA_TXD_PIN_VALUE_Pos)
#define	CSP_PIN_IO_DATA_SCLK_PIN_VALUE_Pos		4
#define	CSP_PIN_IO_DATA_SCLK_PIN_VALUE_Msk		(1UL << CSP_PIN_IO_DATA_SCLK_PIN_VALUE_Pos)


//*****************************************************************************
//
// The following are defines for the bit fields in the CSP_AYSNC_PARAM_REG register.
//
//*****************************************************************************
#define	CSP_AYSNC_PARAM_TIMEOUT_NUM_Pos			0
#define	CSP_AYSNC_PARAM_TIMEOUT_NUM_Msk			(0xFFFF << CSP_AYSNC_PARAM_TIMEOUT_NUM_Pos)
#define	CSP_AYSNC_PARAM_DIV2_Pos				16
#define	CSP_AYSNC_PARAM_DIV2_Msk				(0x3F << CSP_AYSNC_PARAM_DIV2_Pos)


//*****************************************************************************
//
// The following are defines for the bit fields in the CSP_SM_CFG register.
//
//*****************************************************************************
#define	CSP_SM_CLK_DIV_Pos						0
#define	CSP_SM_CLK_DIV_Msk						(0x3F << CSP_SM_CLK_DIV_Pos)
#define	CSP_SM_EN_Pos							7
#define	CSP_SM_EN_Msk							(1UL << CSP_SM_EN_Pos)
#define	CSP_SM_CLK_STP_Pos						8
#define	CSP_SM_CLK_STP_Msk						(1UL << CSP_SM_CLK_STP_Pos)
#define	CSP_SM_RETRY_ENA_Pos					10
#define	CSP_SM_RETRY_ENA_Msk					(1UL << CSP_SM_RETRY_ENA_Pos)


//*****************************************************************************
//
// The following are defines for the bit fields in the CSP_TX_DMA_IO_CTRL register.
//
//*****************************************************************************
#define	CSP_TX_DMA_IO_CTRL_IO_DMA_SEL_Pos		0
#define	CSP_TX_DMA_IO_CTRL_IO_DMA_SEL_Msk		(1UL << CSP_TX_DMA_IO_CTRL_IO_DMA_SEL_Pos)
#define	CSP_TX_DMA_IO_CTRL_IO_MODE		        (1UL << CSP_TX_DMA_IO_CTRL_IO_DMA_SEL_Pos)
#define	CSP_TX_DMA_IO_CTRL_DMA_MODE		        (0UL << CSP_TX_DMA_IO_CTRL_IO_DMA_SEL_Pos)
#define	CSP_TX_DMA_IO_CTRL_TX_ENDIAN_MODE_Pos	4
#define	CSP_TX_DMA_IO_CTRL_TX_ENDIAN_MODE_Msk	(3UL << CSP_TX_DMA_IO_CTRL_TX_ENDIAN_MODE_Pos)


//*****************************************************************************
//
// The following are defines for the bit fields in the CSP_TX_DMA_IO_LEN register.
//
//*****************************************************************************


//*****************************************************************************
//
// The following are defines for the bit fields in the CSP_TX_FIFO_CTRL register.
//
//*****************************************************************************
#define	CSP_TX_FIFO_CTRL_WIDTH_Pos				0
#define	CSP_TX_FIFO_CTRL_WIDTH_Msk				(3UL << CSP_TX_FIFO_CTRL_WIDTH_Pos)
#define	CSP_TX_FIFO_CTRL_THD_Pos				2
#define	CSP_TX_FIFO_CTRL_THD_Msk				(0x3F << CSP_TX_FIFO_CTRL_THD_Pos)


//*****************************************************************************
//
// The following are defines for the bit fields in the CSP_TX_FIFO_LEVEL_CHK register.
//
//*****************************************************************************
#define	CSP_TX_FIFO_LEVEL_CHK_SC_Pos			0
#define	CSP_TX_FIFO_LEVEL_CHK_SC_Msk			(0x1F << CSP_TX_FIFO_LEVEL_CHK_SC_Pos)
#define	CSP_TX_FIFO_LEVEL_CHK_LC_Pos			10
#define	CSP_TX_FIFO_LEVEL_CHK_LC_Msk			(0x1F << CSP_TX_FIFO_LEVEL_CHK_LC_Pos)
#define	CSP_TX_FIFO_LEVEL_CHK_HC_Pos			20
#define	CSP_TX_FIFO_LEVEL_CHK_HC_Msk			(0x1F << CSP_TX_FIFO_LEVEL_CHK_HC_Msk)


//*****************************************************************************
//
// The following are defines for the bit fields in the CSP_TX_FIFO_OP register.
//
//*****************************************************************************
#define	CSP_TX_FIFO_OP_RESET_Pos				0
#define	CSP_TX_FIFO_OP_RESET				    (1UL << CSP_TX_FIFO_OP_RESET_Pos)
#define	CSP_TX_FIFO_OP_START_Pos				1
#define	CSP_TX_FIFO_OP_START				    (1UL << CSP_TX_FIFO_OP_START_Pos)


//*****************************************************************************
//
// The following are defines for the bit fields in the CSP_TX_FIFO_STATUS register.
//
//*****************************************************************************
#define	CSP_TX_FIFO_STATUS_LEVEL_Pos			0
#define	CSP_TX_FIFO_STATUS_LEVEL_Msk			(0x3F << CSP_TX_FIFO_STATUS_LEVEL_Pos)
#define	CSP_TX_FIFO_STATUS_FULL_Pos				7
#define	CSP_TX_FIFO_STATUS_FULL_Msk				(1UL << CSP_TX_FIFO_STATUS_FULL_Pos)
#define	CSP_TX_FIFO_STATUS_EMPTY_Pos			8
#define	CSP_TX_FIFO_STATUS_EMPTY_Msk			(1UL << CSP_TX_FIFO_STATUS_EMPTY_Pos)


//*****************************************************************************
//
// The following are defines for the bit fields in the CSP_TX_FIFO_DATA register.
//
//*****************************************************************************


//*****************************************************************************
//
// The following are defines for the bit fields in the CSP_RX_DMA_IO_CTRL register.
//
//*****************************************************************************
#define	CSP_RX_DMA_IO_CTRL_IO_DMA_SEL_Pos		0
#define	CSP_RX_DMA_IO_CTRL_IO_DMA_SEL_Msk		(1UL << CSP_RX_DMA_IO_CTRL_IO_DMA_SEL_Pos)
#define	CSP_RX_DMA_IO_CTRL_IO_MODE		        (1UL << CSP_RX_DMA_IO_CTRL_IO_DMA_SEL_Pos)
#define	CSP_RX_DMA_IO_CTRL_DMA_MODE		        (0UL << CSP_RX_DMA_IO_CTRL_IO_DMA_SEL_Pos)
#define	CSP_RX_DMA_IO_CTRL_DMA_FLUSH_Pos		2
#define	CSP_RX_DMA_IO_CTRL_DMA_FLUSH_Msk		(1UL << CSP_RX_DMA_IO_CTRL_DMA_FLUSH_Pos)
#define	CSP_RX_DMA_IO_CTRL_RX_ENDIAN_MODE_Pos	4
#define	CSP_RX_DMA_IO_CTRL_RX_ENDIAN_MODE_Msk	(3UL << CSP_RX_DMA_IO_CTRL_RX_ENDIAN_MODE_Pos)



//*****************************************************************************
//
// The following are defines for the bit fields in the CSP_RX_DMA_IO_LEN register.
//
//*****************************************************************************


//*****************************************************************************
//
// The following are defines for the bit fields in the CSP_RX_FIFO_CTRL register.
//
//*****************************************************************************
#define	CSP_RX_FIFO_CTRL_WIDTH_Pos              0
#define	CSP_RX_FIFO_CTRL_WIDTH_Msk				(3UL << CSP_RX_FIFO_CTRL_WIDTH_Pos)
#define	CSP_RX_FIFO_CTRL_THD_Pos				2
#define	CSP_RX_FIFO_CTRL_THD_Msk				(0x3F << CSP_RX_FIFO_CTRL_THD_Pos)

//*****************************************************************************
//
// The following are defines for the bit fields in the CSP_RX_FIFO_LEVEL_CHK register.
//
//*****************************************************************************
#define	CSP_RX_FIFO_LEVEL_CHK_SC_Pos			0
#define	CSP_RX_FIFO_LEVEL_CHK_SC_Msk			(0x1F << CSP_RX_FIFO_LEVEL_CHK_SC_Pos)
#define	CSP_RX_FIFO_LEVEL_CHK_LC_Pos			10
#define	CSP_RX_FIFO_LEVEL_CHK_LC_Msk			(0x1F << CSP_RX_FIFO_LEVEL_CHK_LC_Pos)
#define	CSP_RX_FIFO_LEVEL_CHK_HC_Pos			20
#define	CSP_RX_FIFO_LEVEL_CHK_HC_Msk			(0x1F << CSP_RX_FIFO_LEVEL_CHK_HC_Pos)

//*****************************************************************************
//
// The following are defines for the bit fields in the CSP_RX_FIFO_OP register.
//
//*****************************************************************************
#define	CSP_RX_FIFO_OP_RESET_Pos				0
#define	CSP_RX_FIFO_OP_RESET				    (1UL << CSP_RX_FIFO_OP_RESET_Pos)
#define	CSP_RX_FIFO_OP_START_Pos				1
#define	CSP_RX_FIFO_OP_START				    (1UL << CSP_RX_FIFO_OP_START_Pos)

//*****************************************************************************
//
// The following are defines for the bit fields in the CSP_RX_FIFO_STATUS register.
//
//*****************************************************************************
#define	CSP_RX_FIFO_STATUS_LEVEL_Pos			0
#define	CSP_RX_FIFO_STATUS_LEVEL_Msk			(0x3F << CSP_RX_FIFO_STATUS_LEVEL_Pos)
#define	CSP_RX_FIFO_STATUS_FULL_Pos				7
#define	CSP_RX_FIFO_STATUS_FULL_Msk				(1UL << CSP_RX_FIFO_STATUS_FULL_Pos)
#define	CSP_RX_FIFO_STATUS_EMPTY_Pos			8
#define	CSP_RX_FIFO_STATUS_EMPTY_Msk			(1UL << CSP_RX_FIFO_STATUS_EMPTY_Pos)

//*****************************************************************************
//
// The following are defines for the bit fields in the CSP_RX_FIFO_DATA register.
//
//*****************************************************************************



#endif // __HW_CSP_H__
