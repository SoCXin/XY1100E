#ifndef _RF_DRV_H_
#define _RF_DRV_H_

//#include "DataTypes.h"
#include "hw_dfe.h"
#include "hw_analog.h"
#include "hw_types.h"
#include "replace.h"

#define XTAL_CLK                    38400000
#define	BBPLL_CLK_DEFAULT			368640000

// 7.35GHz = 7350MHz = 7350000KHz
#define RFPLL_HIGH_LOW_BAND_Hz		(uint64_t)7350000000
#define RFPLL_HIGH_LOW_BAND_KHz     7350000
#define RFPLL_BAND_MODE_HIGH        1
#define RFPLL_BAND_MODE_LOW         0

// [191:188] = {tx_l, rx_l, rx_h, tx_h}
#define BAND_ENABLE_TXL             0x08
#define BAND_ENABLE_RXL             0x04
#define BAND_ENABLE_RXH             0x02
#define BAND_ENABLE_TXH             0x01

#define RFPLL_DIV1_RANGE_LOW        1500000000
#define RFPLL_DIV1_RANGE_HIGH       2250000000
#define RFPLL_DIV2_RANGE_LOW         750000000
#define RFPLL_DIV2_RANGE_HIGH       1125000000
#define RFPLL_DIV3_RANGE_LOW         500000000
#define RFPLL_DIV3_RANGE_HIGH        750000000
#define RFPLL_DIV4_RANGE_LOW         375000000
#define RFPLL_DIV4_RANGE_HIGH        562500000

#define RFPLL_DIV_1                 0
#define RFPLL_DIV_2                 1
#define RFPLL_DIV_3                 2
#define RFPLL_DIV_4                 3

#define GAIN_TABLE_SIZE_RX_HB_LNA   9
#define GAIN_TABLE_SIZE_RX_LB_LNA   7
#define GAIN_TABLE_SIZE_RX_MIXER    5

#define GAIN_TABLE_SIZE_TX_LB_MIXER 19
#define GAIN_TABLE_SIZE_TX_LB_PA    4
#define GAIN_TABLE_SIZE_TX_BB       10

#define GAIN_TABLE_SIZE_TX_HB_MIXER 26
#define GAIN_TABLE_SIZE_TX_HB_PAD   39
#define GAIN_TABLE_SIZE_TX_HB_PA    8

#define RX_HB_GAIN_LNA              1
#define RX_HB_GAIN_LNA_GC           2
#define RX_HB_GAIN_LNA_RO_ADJ       3
#define RX_HB_GAIN_MIXER            4
#define RX_HB_GAIN_MIXER_GC         5
#define RX_HB_GAIN_MIXER_SD_ADJ     6
#define RX_LB_GAIN_LNA              7
#define RX_LB_GAIN_LNA_GC           RX_LB_GAIN_LNA
#define RX_LB_GAIN_MIXER            8
#define RX_LB_GAIN_MIXER_GC         9
#define RX_LB_GAIN_MIXER_SD_ADJ     10
#define RX_BB_GAIN_PGA              11
#define RX_BB_GAIN_PGA_GC           RX_BB_GAIN_PGA
#define RX_DFE_GAIN_CIC             12
#define RX_DFE_GAIN_ACI             13

#define TX_HB_MX_CAS_GC             1
#define TX_HB_PAD_GC                2
#define TX_HB_PA_GC                 3
#define TX_LB_MX_CAS_GC             4
#define TX_LB_PA_GC                 5
#define TX_BB_GC                    6

#define RX_DELAY_ACI                0
#define RX_DELAY_CIC                1
#define RX_DELAY_PGA                2
#define RX_DELAY_DC_CAL             3
#define RX_DELAY_MIXER              4
#define RX_DELAY_LNA                5
#define RX_DELAY_SUBFRAME           6
#define RX_DELAY_ONE_MS             7

#define TX_DELAY_BB_GAIN            1
#define TX_DELAY_RF_GAIN            2
#define TX_DELAY_BB_PU              3
#define TX_DELAY_RF_PU              4


typedef struct {
	int16_t dBTen;
	int16_t GainReg;
} TableType1;

typedef struct {
	int16_t dBTen;
	int16_t GainReg1;
    int16_t GainReg2;
} TableType2;

typedef struct {
	int8_t  dB;
	uint8_t GainReg;
} TableType3;

typedef struct {
	int16_t dBTen;
	uint8_t GainReg;
} TableType4;

typedef enum {
    RADIO_STATE_DISABLE = 0,
    RADIO_STATE_RXRU = 1,
    RADIO_STATE_RXIDLE = 2,
    RADIO_STATE_RX = 3,
    RADIO_STATE_RXDISABLE = 4,
    RADIO_STATE_TXRU = 9,
    RADIO_STATE_TXIDLE = 10,
    RADIO_STATE_TX = 11,
    RADIO_STATE_TXDISABLE = 12
} RadioState;


typedef enum {
    AccessModeUnknow = 0,
    AccessModeByte = 1,
    AccessModeWord = 2,
} RegAccessMode;


extern uint8_t REG_Bus_Field_Set(uint32_t ulRegBase, uint8_t ucBit_high, uint8_t ucBit_low, uint32_t ulValue);
extern uint8_t REG_Bus_Field_Get(uint32_t ulRegBase, uint8_t ucBit_high, uint8_t ucBit_low, uint32_t *ulValue);

// System Clock Select
void PRCM_Clock_Mode_Force_XTAL(void);
void PRCM_Clock_Mode_Auto(void);


// For Layer1 Controller:
void RF_RX_Freq_Set(uint32_t RxFreqHz);
void RF_TX_Freq_Set(uint32_t TxFreqHz);
void RF_RX_Gains_Set(int8_t RxGaindB);
void RF_TX_Power_Set(int8_t TxPowerdBm);
void RF_RX_Enable(void);
void RF_RX_Disable(void);
void RF_TX_Enable(void);
void RF_TX_Disable(void);
void RF_RX_Delay_Counter_Set(uint8_t CounterId, uint32_t Value);
void RF_TX_Delay_Counter_Set(uint8_t CounterId, uint32_t Value);
void RF_Init(void);
uint32_t RF_RX_Freq_Get(void);
uint32_t RF_TX_Freq_Get(void);


// For DFE:
// Radio Frequency
void DFE_RX_DDFS_LO_FCW_Set(int32_t ddfs_FreqHz);
void DFE_RX_DDFS_LO_PHS_ADJ_Set(int32_t phase_offset);
void DFE_TX_DDFS_LO_FCW_Set(int32_t ddfs_FreqHz);
void DFE_TX_DDFS_LO_PHS_ADJ_Set(int32_t phase_offset);
int32_t DFE_RX_DDFS_LO_FCW_Get(void);
int32_t DFE_RX_DDFS_LO_PHS_ADJ_Get(void);
int32_t DFE_TX_DDFS_LO_FCW_Get(void);
int32_t DFE_TX_DDFS_LO_PHS_ADJ_Get(void);

// RX AGC
void DFE_RX_Single_Gain_Set(uint8_t GainId, uint32_t GainValue);
void DFE_RX_Single_Gain_Set_By_dBTen(uint8_t GainId, int16_t dBTen);
uint32_t DFE_RX_Single_Gain_Get(uint8_t GainId);
int16_t  DFE_RX_Single_Gain_Get_dBTen(uint8_t GainId);
void DFE_AGC_Enable(uint8_t BandMode);
void DFE_AGC_Disable(uint8_t BandMode);

// TX Power Control
void DFE_TX_Single_Gain_Set(uint8_t GainId, uint32_t GainValue);
void DFE_TX_Single_Gain_Set_By_dBTen(uint8_t GainId, int16_t dBTen);
uint32_t DFE_TX_Single_Gain_Get(uint8_t GainId);
int16_t  DFE_TX_Single_Gain_Get_dBTen(uint8_t GainId);

// Time Adjust
void DFE_RX_Delay_Counter_Set(uint8_t CounterId, uint32_t Value);
void DFE_TX_Delay_Counter_Set(uint8_t CounterId, uint32_t Value);

// Measurement
int16_t DFE_RX_RSSI_CIC_Get(void);
int16_t DFE_RX_RSSI_ACI_Get(void);
int16_t DFE_TX_TSSI_AVG_Get(void);
void DFE_TX_TSSI_AVG_Num_Set(uint8_t avg_num);

// TX Calibration
void DFE_TX_DC_EST_I_Set(int16_t value);
void DFE_TX_DC_EST_Q_Set(int16_t value);
uint16_t DFE_TX_IQ_AMP_EST_Get(void);
uint16_t DFE_TX_IQ_PHS_EST_I_Get(void);
uint16_t DFE_TX_IQ_PHS_EST_Q_Get(void);

// Module Bypass Control
void DFE_RX_Module_Bypass_Set(uint8_t ModuleId, uint8_t Bypass);
void DFE_TX_Module_Bypass_Set(uint8_t ModuleId, uint8_t Bypass);


// RF Analog
// PLL
void RF_RFPLL_LDO_Power_On(void);
void RF_RFPLL_LDO_Power_Off(void);
void RF_RFPLL_LDO_Filter_En(void);
void RF_RFPLL_LDO_Filter_Dis(void);
void RF_RFPLL_Power_On(void);
void RF_RFPLL_Power_Off(void);
void RF_RFPLL_On(void);
void RF_RFPLL_Off(void);

void RF_RFPLL_Cal_On(void);
void RF_RFPLL_Cal_Off(void);
void RF_BBPLL_Cal_On(void);
void RF_BBPLL_Cal_Off(void);

void RF_BBPLL_Switch_Outer_Clock(void);

void RF_RFPLL_VCO_Freq_Set(uint64_t ulFreqHz);
void RF_RFPLL_LO_Freq_Set(uint32_t ulFreqHz);
void RF_RFPLL_LO_Div_Set(uint8_t ucValue);
void RF_RFPLL_VCO_High_Band_Set(void);
void RF_RFPLL_VCO_Low_Band_Set(void);
uint8_t RF_RFPLL_VCO_BandMode_Get(void);

void RF_BBPLL_Freq_Set(uint32_t ulFreqHz);
void RF_BBPLL_DivN_Set(uint32_t ulDivN);
void RF_BBPLL_FracN_Set(uint32_t ulFracN);
void RF_RFPLL_DivN_Set(uint32_t ulDivN);
void RF_RFPLL_FracN_Set(uint32_t ulFracN);

uint32_t RF_BBPLL_DivN_Get(void);
uint32_t RF_BBPLL_FracN_Get(void);
uint32_t RF_RFPLL_DivN_Get(void);
uint32_t RF_RFPLL_FracN_Get(void);

uint8_t RF_BBPLL_Lock_Status_Get(void);
uint8_t RF_RFPLL_Lock_Status_Get(void);

void RF_BBPLL_CPLDO_Set(uint8_t ucValue);
void RF_BBPLL_Ref_Cycle_Cnt_Set(uint32_t ulValue);
void RF_BBPLL_ReadOut_Sel_Set(uint8_t ucValue);
void RF_BBPLL_Lock_Threshold_Set(uint8_t ulValue);
void RF_RFPLL_IBIAS_RC_Filter_En(void);
void RF_RFPLL_AVDDFILTER_EN_VCO_SEL_Set(uint8_t ucValue);

// LDO
void RF_RFLDO_CNTL_En(void);
void RF_RFLDO_CNTL_Dis(void);
void RF_BBLDO_CNTL_En(void);
void RF_BBLDO_CNTL_Dis(void);
void RF_RFLDO_Power_On(void);
void RF_RFLDO_Power_Off(void);
void RF_BBLDO_Power_On(void);
void RF_BBLDO_Power_Off(void);
void RF_BBLDO_TRX_EN(void);
void RF_BBLDO_TRX_Dis(void);

void RF_LDO_On(void);
void RF_LDO_Off(void);

// RX HB/LB/BB
void RF_RXHB_LNA_BIAS_Power_On(void);
void RF_RXHB_LNA_BIAS_Power_Off(void);
void RF_RXHB_LNA_Power_On(void);
void RF_RXHB_LNA_Power_Off(void);
void RF_RXHB_LNA_RST(void);
void RF_RXHB_LNA_EN(void);
void RF_RXHB_LNA_Dis(void);
void RF_RXHB_LNA_BYP_EN(void);
void RF_RXHB_LNA_BYP_Dis(void);
void RF_RXHB_LNA_RST_BYP(void);
void RF_RXHB_LNA_RST_BYPVAL(void);
void RF_RXHB_LO_Power_On(void);
void RF_RXHB_LO_Power_Off(void);
void RF_RXHB_GM_BIAS_Power_On(void);
void RF_RXHB_GM_BIAS_Power_Off(void);
void RF_RXLB_LNA_Power_On(void);
void RF_RXLB_LNA_Power_Off(void);
void RF_RXLB_LO_Power_On(void);
void RF_RXLB_LO_Power_Off(void);
void RF_RXLB_GM_BIAS_Power_On(void);
void RF_RXLB_GM_BIAS_Power_Off(void);
void RF_RXBB_TIA_Power_on(void);
void RF_RXBB_TIA_Power_off(void);
void RF_RXBB_PGA_BIAS_Power_On(void);
void RF_RXBB_PGA_BIAS_Power_Off(void);
void RF_RXBB_PGA_I_Power_On(void);
void RF_RXBB_PGA_I_Power_Off(void);
void RF_RXBB_PGA_Q_Power_On(void);
void RF_RXBB_PGA_Q_Power_Off(void);
void RF_RXBB_ADC_BIAS_Power_On(void);
void RF_RXBB_ADC_BIAS_Power_Off(void);
void RF_RXBB_ADC_Power_On(void);
void RF_RXBB_ADC_Power_Off(void);
void RF_RXBB_ADC_RSTB(void);
void RF_RXBB_ADC_RST_BYP_EN(void);
void RF_RXBB_ADC_RST_BYP_DIS(void);
void RF_RXBB_ADC_RSTB_BYPVAL_EN(void);
void RF_RXBB_ADC_RSTB_BYPVAL_DIS(void);

void RF_RXHB_On(void);
void RF_RXHB_Off(void);
void RF_RXLB_On(void);
void RF_RXLB_Off(void);
void RF_RXBB_On(void);
void RF_RXBB_Off(void);

// TX HB/LB/BB
uint8_t RF_TXLDO_PALDO_BYPASS_Flag_Get(void);

void RF_TXLDO_PALDO_CNTL_En(void);
void RF_TXLDO_PALDO_CNTL_Dis(void);
void RF_TXLDO_PALDO_Power_On(void);
void RF_TXLDO_PALDO_Power_Off(void);

void RF_TXHB_TSSI_Power_On(void);
void RF_TXHB_TSSI_Power_Off(void);
void RF_TXHB_BIAS_Power_On(void);
void RF_TXHB_BIAS_Power_Off(void);
void RF_TXHB_MX_GM_BIAS_RST(void);
void RF_TXHB_MX_Power_On(void);
void RF_TXHB_MX_Power_Off(void);
void RF_TXHB_MX_LO_BIAS_RST(void);
void RF_TXHB_PAD_Power_On(void);
void RF_TXHB_PAD_Power_Off(void);
void RF_TXHB_PA_Power_On(void);
void RF_TXHB_PA_Power_Off(void);
void RF_TXHB_On(void);
void RF_TXHB_Off(void);

void RF_TXLB_TSSI_Power_On(void);
void RF_TXLB_TSSI_Power_Off(void);
void RF_TXLB_BIAS_Power_On(void);
void RF_TXLB_BIAS_Power_Off(void);
void RF_TXLB_MX_LO_BIAS_RST(void);
void RF_TXLB_MX_Power_On(void);
void RF_TXLB_MX_Power_Off(void);
void RF_TXLB_PA_Power_On(void);
void RF_TXLB_PA_Power_Off(void);
void RF_TXLB_PA_GM_BIAS_RST(void);
void RF_TXLB_On(void);
void RF_TXLB_Off(void);

void RF_TXBB_Power_On(void);
void RF_TXBB_Power_Off(void);
void RF_TXBB_RSTB(void);
void RF_TXBB_On(void);
void RF_TXBB_Off(void);

void RF_TX_CTUNE_Set(uint8_t ModuleId, uint8_t ctune_value);

void RF_Band_Enable_Set(uint8_t ucValue);

void RF_RX_On(uint32_t RxLOFreqHz);

void RF_RX_Capture_ADC_Input(void);

void RF_RX_LB_NF_Set(void);

void RF_TX_On(uint32_t TxLOFreqHz);

void RF_TX_Capture_DAC_Output(void);



#endif /* _RF_DRV_H_ */
