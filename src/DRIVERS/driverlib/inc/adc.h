#ifndef __ADC_H__
#define __ADC_H__

#include "hw_memmap.h"
#include "hw_types.h"


#define USE_ADC_CAL				1  // use adc calibration or not


// ADC mode select
#define ADC_MODE_DOUBLE_P		0x00	// double-ended, P to signal, N to GND
#define ADC_MODE_DOUBLE_N		0x01	// double-ended, P to GND, N to signal
#define ADC_MODE_DOUBLE_PN		0x02	// double-ended, both P and N to signal
#define ADC_MODE_SINGLE_P		0x03	// single-ended, P to signal
#define ADC_MODE_SINGLE_N		0x04	// single-ended, N to signal
#define ADC_MODE_VBAT_CHECK     0x05	// single-ended, P to signal and vbat_in to P

// ADC sample speed select
#define ADC_SPEED_480K          0x1F
#define ADC_SPEED_240K          0x3F

// ADC measure range
#define ADC_RANGE_1V            0x00
#define ADC_RANGE_4V            0x01

// error code
#define ADC_MODE_ERROR			(-32768)
#define ADC_CALI_ERROR			(-32767)
#define ADC_RANGE_ERROR			(-32766)

#define WAIT_DSP_ADC_IDLE()		while(HWREGB(0xA0058015) & 0x80)
#define SET_ARM_ADC_BUSY()		(HWREGB(0xA0058015) |= 0x40)
#define SET_ARM_ADC_IDLE()		(HWREGB(0xA0058015) &= 0xBF)


#if USE_ADC_CAL
// ideal code computing method: IDEAL_CODE = (ideal voltage)*2048/1000, rounded to integer
#define DOUBLE_IDEAL_CODE_P250_N0		(512)		// in P250 N0 mV
#define DOUBLE_IDEAL_CODE_P750_N0		(1535)		// in P750 N0 mV
#define DOUBLE_IDEAL_CODE_P0_N250		(-512)		// in P0 N250 mV
#define DOUBLE_IDEAL_CODE_P0_N750		(-1536)		// in P0 N750 mV
#define DOUBLE_IDEAL_CODE_P250_N750		(-1024)		// in P250 N750 mV
#define DOUBLE_IDEAL_CODE_P750_N250		(1024)		// in P750 N250 mV

#define DOUBLE_EXPAND_MULTIPLE		100000		// the max num is 10^5
#define double_gainerror_P			((DOUBLE_IDEAL_CODE_P750_N0   - DOUBLE_IDEAL_CODE_P250_N0)   * DOUBLE_EXPAND_MULTIPLE / (flash_otp_cal.real_code_P750_N2 - flash_otp_cal.real_code_P250_N0))
#define double_gainerror_N			((DOUBLE_IDEAL_CODE_P0_N750   - DOUBLE_IDEAL_CODE_P0_N250)   * DOUBLE_EXPAND_MULTIPLE / (flash_otp_cal.real_code_P0_N750 - flash_otp_cal.real_code_P0_N250))
#define double_gainerror_PN			((DOUBLE_IDEAL_CODE_P750_N250 - DOUBLE_IDEAL_CODE_P250_N750) * DOUBLE_EXPAND_MULTIPLE / (flash_otp_cal.real_code_P750_N250 - flash_otp_cal.real_code_P250_N750))
#define double_offset_P				((DOUBLE_IDEAL_CODE_P250_N0   * DOUBLE_EXPAND_MULTIPLE) - (double_gainerror_P  * flash_otp_cal.real_code_P250_N0))
#define double_offset_N				((DOUBLE_IDEAL_CODE_P0_N250   * DOUBLE_EXPAND_MULTIPLE) - (double_gainerror_N  * flash_otp_cal.real_code_P0_N250))
#define double_offset_PN			((DOUBLE_IDEAL_CODE_P250_N750 * DOUBLE_EXPAND_MULTIPLE) - (double_gainerror_PN * flash_otp_cal.real_code_P250_N750))


// ideal code computing method: IDEAL_CODE = (ideal voltage)*4095/1000, rounded to integer
#define SINGLE_P_IDEAL_CODE_50		205		// in 50  mV
#define SINGLE_P_IDEAL_CODE_200		819		// in 200 mV
#define SINGLE_P_IDEAL_CODE_300		1229	// in 300 mV
#define SINGLE_P_IDEAL_CODE_450		1843	// in 450 mV
#define SINGLE_P_IDEAL_CODE_550		2252	// in 550 mV
#define SINGLE_P_IDEAL_CODE_700		2867	// in 700 mV
#define SINGLE_P_IDEAL_CODE_800		3276	// in 800 mV
#define SINGLE_P_IDEAL_CODE_950		3890	// in 950 mV

#define SINGLE_P_EXPAND_MULTIPLE	100000		// the max num is 10^5
#define single_P_gainerror1			((SINGLE_P_IDEAL_CODE_200 - SINGLE_P_IDEAL_CODE_50)  * SINGLE_P_EXPAND_MULTIPLE / (flash_otp_cal.real_code_P200 - flash_otp_cal.real_code_P50))
#define single_P_gainerror2			((SINGLE_P_IDEAL_CODE_450 - SINGLE_P_IDEAL_CODE_300) * SINGLE_P_EXPAND_MULTIPLE / (flash_otp_cal.real_code_P450 - flash_otp_cal.real_code_P300))
#define single_P_gainerror3			((SINGLE_P_IDEAL_CODE_700 - SINGLE_P_IDEAL_CODE_550) * SINGLE_P_EXPAND_MULTIPLE / (flash_otp_cal.real_code_P700 - flash_otp_cal.real_code_P550))
#define single_P_gainerror4			((SINGLE_P_IDEAL_CODE_950 - SINGLE_P_IDEAL_CODE_800) * SINGLE_P_EXPAND_MULTIPLE / (flash_otp_cal.real_code_P950 - flash_otp_cal.real_code_P800))
#define single_P_offset1			((SINGLE_P_IDEAL_CODE_50  * SINGLE_P_EXPAND_MULTIPLE) - (single_P_gainerror1 * flash_otp_cal.real_code_P50))
#define single_P_offset2			((SINGLE_P_IDEAL_CODE_300 * SINGLE_P_EXPAND_MULTIPLE) - (single_P_gainerror2 * flash_otp_cal.real_code_P300))
#define single_P_offset3			((SINGLE_P_IDEAL_CODE_550 * SINGLE_P_EXPAND_MULTIPLE) - (single_P_gainerror3 * flash_otp_cal.real_code_P550))
#define single_P_offset4			((SINGLE_P_IDEAL_CODE_800 * SINGLE_P_EXPAND_MULTIPLE) - (single_P_gainerror4 * flash_otp_cal.real_code_P800))


// ideal code computing method: IDEAL_CODE = (ideal voltage)*4095/1000, rounded to integer
#define SINGLE_N_IDEAL_CODE_50		205		// in 50  mV
#define SINGLE_N_IDEAL_CODE_200		819		// in 200 mV
#define SINGLE_N_IDEAL_CODE_300		1229	// in 300 mV
#define SINGLE_N_IDEAL_CODE_450		1843	// in 450 mV
#define SINGLE_N_IDEAL_CODE_550		2252	// in 550 mV
#define SINGLE_N_IDEAL_CODE_700		2867	// in 700 mV
#define SINGLE_N_IDEAL_CODE_800		3276	// in 800 mV
#define SINGLE_N_IDEAL_CODE_950		3890	// in 950 mV

#define SINGLE_N_EXPAND_MULTIPLE	100000		// the max num is 10^5
#define single_N_gainerror1			((SINGLE_N_IDEAL_CODE_200 - SINGLE_N_IDEAL_CODE_50)  * SINGLE_N_EXPAND_MULTIPLE / (flash_otp_cal.real_code_N200 - flash_otp_cal.real_code_N50))
#define single_N_gainerror2			((SINGLE_N_IDEAL_CODE_450 - SINGLE_N_IDEAL_CODE_300) * SINGLE_N_EXPAND_MULTIPLE / (flash_otp_cal.real_code_N450 - flash_otp_cal.real_code_N300))
#define single_N_gainerror3			((SINGLE_N_IDEAL_CODE_700 - SINGLE_N_IDEAL_CODE_550) * SINGLE_N_EXPAND_MULTIPLE / (flash_otp_cal.real_code_N700 - flash_otp_cal.real_code_N550))
#define single_N_gainerror4			((SINGLE_N_IDEAL_CODE_950 - SINGLE_N_IDEAL_CODE_800) * SINGLE_N_EXPAND_MULTIPLE / (flash_otp_cal.real_code_N950 - flash_otp_cal.real_code_N800))
#define single_N_offset1			((SINGLE_N_IDEAL_CODE_50  * SINGLE_N_EXPAND_MULTIPLE) - (single_N_gainerror1 * flash_otp_cal.real_code_N50))
#define single_N_offset2			((SINGLE_N_IDEAL_CODE_300 * SINGLE_N_EXPAND_MULTIPLE) - (single_N_gainerror2 * flash_otp_cal.real_code_N300))
#define single_N_offset3			((SINGLE_N_IDEAL_CODE_550 * SINGLE_N_EXPAND_MULTIPLE) - (single_N_gainerror3 * flash_otp_cal.real_code_N550))
#define single_N_offset4			((SINGLE_N_IDEAL_CODE_800 * SINGLE_N_EXPAND_MULTIPLE) - (single_N_gainerror4 * flash_otp_cal.real_code_N800))


typedef struct
{
	unsigned int calibration_valid;

	short real_code_P250_N0;	// in P250 N0   mV
	short real_code_P750_N2;	// in P750 N0   mV
	short real_code_P0_N250;	// in P0   N750 mV
	short real_code_P0_N750;	// in P0   N250 mV
	short real_code_P250_N750;	// in P250 N750 mV
	short real_code_P750_N250;	// in P750 N250 mV

	short real_code_P50;		// in 50   mV
	short real_code_P200;		// in 200  mV
	short real_code_P300;		// in 300  mV
	short real_code_P450;		// in 450  mV
	short real_code_P550;		// in 550  mV
	short real_code_P700;		// in 700  mV
	short real_code_P800;		// in 800  mV
	short real_code_P950;		// in 950  mV

	short real_code_N50;		// in 50  mV
	short real_code_N200;		// in 200 mV
	short real_code_N300;		// in 300 mV
	short real_code_N450;		// in 450 mV
	short real_code_N550;		// in 550 mV
	short real_code_N700;		// in 700 mV
	short real_code_N800;		// in 800 mV
	short real_code_N950;		// in 950 mV

	short code_tsensor;			// temperature sensor

	unsigned char code_rc;		// rc
	unsigned char reserve1;		// reserve

	unsigned int UniqueID[4];	// UID

	unsigned int calibration_crc;	// CRC
} Flash_OTP_CAL_Def;

extern Flash_OTP_CAL_Def flash_otp_cal;
extern unsigned char otp_valid_flag;

#endif	/* end of USE_ADC_CAL */

// function declaration
void ADC_Init(unsigned int adc_mode, unsigned int sample_speed, unsigned int range);
void ADC_DeInit(void);
void ADC_Open(unsigned int adc_mode, unsigned int sample_speed, unsigned int range);
void ADC_Close(void);
short ADC_Get_Real_Value(void);
short ADC_Get_Converte_Value(unsigned int adc_mode, short real_val);
short ADC_Get_Voltage_Without_Cal(unsigned int adc_mode, unsigned int range, short conv_val);
#if USE_ADC_CAL
short ADC_Get_Rounding_Value(unsigned int adc_mode, short conv_val);
short ADC_Get_Voltage_With_Cal(unsigned int adc_mode, unsigned int range, short conv_val);
#endif	/* end of USE_ADC_CAL */

#endif	/* end of __ADC_H__ */
