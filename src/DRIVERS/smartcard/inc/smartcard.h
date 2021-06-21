#ifndef __SMARTCARD_H
#define __SMARTCARD_H

#include "sim_csp.h"
#include "gpio.h"


#define	CSP_ASYNC_TIMEOUT			8


#define PROTOCOL_T0					0
#define PROTOCOL_T1					1
#define PROTOCOL_T15				15



#define CLK_DELAY_CYCLE				10//9
#define WHILE_DELAY_CLK				28//30


#define MAX_ATR_CHARACTER_LEN 		33

#define RST_HIGH					1
#define RST_LOW						0

#define DEFAULT_Fd					372
#define DEFAULT_Dd					1
#define DEFAULT_WI					10

#define DEFAULT_WT_ETU				9600


#define SC_SUCCESS					1
#define SC_FAILURE					0


#define CLK_STOP_NOT_SUPPORT		0x00	//00 00 0000
#define CLK_STOP_STATE_LOW_ONLY	0x40	//01 00 0000
#define CLK_STOP_STATE_HIGH_ONLY		0x80	//10 00 0000
#define CLK_STOP_STATE_HIGH_LOW		0xc0


#define SIM_CLK_STOP				0x00
#define SIM_CLK_RESTART				0x01

#define SIM_CLK_STOP_TIMEOUT		1860
#define SIM_CLK_RESTART_TIMEOUT		744



#define  VOLTAGE_CLASS_A_SUPPORT	0x01
#define  VOLTAGE_CLASS_B_SUPPORT	0x02   //3V
#define  VOLTAGE_CLASS_C_SUPPORT	0x04   //1.8V




//#define  NV_SIMVCC_5V_BIT	0x01
#define  NV_SIMVCC_3V_BIT	0x02     //3V
#define  NV_SIMVCC_1V8_BIT	0x04     //1.8V

#define  NV_SIMVCC_ALL_BIT	(NV_SIMVCC_1V8_BIT | NV_SIMVCC_3V_BIT)


#define Pw_on    0x62
#define Pw_off   0x63

#define SM_CLK_MIN    4000000
#define SM_CLK_MAX    5000000


typedef enum
{
	 VOLTAGE_CLASS_A		,	//5v
	 VOLTAGE_CALSS_B		,	//3v
	 VOLTAGE_CLASS_C		,	//1.8v
	 VOLTAGE_CLASS_MAX
}Voltage_Class;

typedef enum{
	PROCEDURE_NONE,
	PROCEDURE_COLD_RST_ACTIVATION,
	PROCEDURE_WARM_RST_ACTIVATION,
	PROCEDURE_PPS,
	PROCEDURE_T0_CMD,
	PROCEDURE_T1_CMD,
	PROCEDURE_CLK_STOP,
	PROCEDURE_DEACTIVATION
}SimCard_Procedure;


typedef enum{
	STATE_ATR_NONE,
	//activation
	STATE_ATR_RST_LOW,
	STATE_ATR_VCC_POWERON,
	STATE_ATR_IO_MODE_IN,
	STATE_ATR_CLK_INIT,

	//cold rst
	STATE_ATR_IO_PULLUP,
	STATE_ATR_RST_HIGH,

	//atr parse
	STATE_ATR_PARSE_TS,
	STATE_ATR_PARSE_T0,
	STATE_ATR_PARSE_TA1,
	STATE_ATR_PARSE_TB1,
	STATE_ATR_PARSE_TC1,
	STATE_ATR_PARSE_TD1,
	STATE_ATR_PARSE_TA2,
	STATE_ATR_PARSE_TB2,
	STATE_ATR_PARSE_TC2,
	STATE_ATR_PARSE_TD2,
	STATE_ATR_PARSE_TAi,
	STATE_ATR_PARSE_TBi,
	STATE_ATR_PARSE_TCi,
	STATE_ATR_PARSE_TDi,
	STATE_ATR_PARSE_HISTORY_BYTES,
	STATE_ATR_PARSE_TCK,

	//result
	STATE_ATR_SUCCESS,
	STATE_ATR_FAILURE
}ATR_CTRL_STATE;


typedef enum{
	STATE_PPS_NONE,

	//pps request
	STATE_PPSS_REQUEST,
	STATE_PPS0_REQUEST,
	STATE_PPS1_REQUEST,
	STATE_PPS2_REQUEST,
	STATE_PPS3_REQUEST,
	STATE_PCK_REQUEST,

	//pps response
	STATE_PPSS_RESPONSE,
	STATE_PPS0_RESPONSE,
	STATE_PPS1_RESPONSE,
	STATE_PPS2_RESPONSE,
	STATE_PPS3_RESPONSE,
	STATE_PCK_RESPONSE,

	//pps result
	STATE_PPS_SUCCESS,
	STATE_PPS_FAILURE
		
}PPS_EXCHANGE_STATE;

typedef enum{
	STATE_T0_CMD_NONE,
	STATE_T0_CMD_NULL,
	STATE_T0_CMD_ACK,
	STATE_T0_CMD_INS,
	STATE_T0_CMD_COMPLEMENT_INS,
	STATE_T0_CMD_SW1,
	STATE_T0_CMD_SUCCESS,
	STATE_T0_CMD_FAILURE
}T0_CMD_STATE;


typedef struct {
	uint8_t 		TA2_SpecificMode;
	uint8_t			class_clock;
	uint8_t 		T_protocol_used;
	uint8_t 		WI;
	uint16_t 		Fi;
	uint16_t 		Di;
	uint16_t		clk_div;
	uint16_t 		guard_time;
	uint16_t 		T_indicated;
	Voltage_Class 	current_class;		//size is 4 byte
}SimCard_Profile;


typedef struct{
	SimCard_Profile 	profile;
	SimCard_Procedure	current_procedure;
	ATR_CTRL_STATE 		atr_state;
	PPS_EXCHANGE_STATE 	pps_state;
	T0_CMD_STATE		T0_state;
}SimCard_Item;

typedef struct{
	uint8_t 	PPSS;
	uint8_t 	PPS0;
	uint8_t 	PPS1;
	uint8_t 	PPS2;
	uint8_t 	PPS3;
	uint8_t 	PCK;
}PPS_element;


typedef struct{
	uint8_t 	cla;
	uint8_t 	ins;
	uint8_t 	p1;
	uint8_t 	p2;
	uint8_t 	Lc;
	uint8_t 	*data;
	uint16_t 	Le;			//max is 256 ,larger than uint8_t range

}T_Command_APDU;

typedef struct{

	uint8_t 	*data;
	uint8_t 	sw1;
	uint8_t 	sw2;

}T_Response_APDU;

void SC7816_command(uint8_t *pApduBuf,uint8_t *pRxBuffer,uint32_t *uLen);
void sim_ioldo2_off();
void sim_ioldo2_on();
void SimCard_Deactivation();

#endif /* __SMARTCARD_H */
