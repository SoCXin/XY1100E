#ifndef __GPIO_H__
#define __GPIO_H__

#include "hw_gpio.h"
#include "hw_ints.h"
#include "hw_memmap.h"
#include "hw_types.h"
#include "debug.h"
#include "interrupt.h"
#include "replace.h"

//*****************************************************************************
//
// If building with a C++ compiler, make all of the definitions in this header
// have a C binding.
//
//*****************************************************************************
#ifdef __cplusplus
extern "C"
{
#endif

//*****************************************************************************
//
// Values that can be passed to GPIODirModeSet as the ulPinIO parameter, and
// returned from GPIODirModeGet.
//
//*****************************************************************************
#define GPIO_DIR_MODE_IN        0x00000000  // Pin is a GPIO input
#define GPIO_DIR_MODE_OUT       0x00000001  // Pin is a GPIO output
#define GPIO_DIR_MODE_INOUT     0x00000002  // Pin is a GPIO input/output
#define GPIO_DIR_MODE_HW        0x00000003  // Pin is a peripheral function

#define GPIO_MODE_HW            0x1
#define GPIO_MODE_GPIO          0x0
    
#define GPIO_CTL_PERIPHERAL     0x1
#define GPIO_CTL_REGISTER       0x0
    
#define GPIO_INT_EDGE           0x0
#define GPIO_INT_LEVEL          0x1
    
#define GPIO_INT_EDGE_SINGLE    0x0
#define GPIO_INT_EDGE_BOTH      0x1

#define GPIO_INT_EDGE_RISE      0x0
#define GPIO_INT_EDGE_FALL      0x1


#define GPIO_PAD_NUM_0          0 
#define GPIO_PAD_NUM_1          1 
#define GPIO_PAD_NUM_2          2 
#define GPIO_PAD_NUM_3          3 
#define GPIO_PAD_NUM_4          4 
#define GPIO_PAD_NUM_5          5 
#define GPIO_PAD_NUM_6          6 
#define GPIO_PAD_NUM_7          7 
#define GPIO_PAD_NUM_8          8 
#define GPIO_PAD_NUM_9          9 
#define GPIO_PAD_NUM_10         10
#define GPIO_PAD_NUM_11         11
#define GPIO_PAD_NUM_12         12
#define GPIO_PAD_NUM_13         13
#define GPIO_PAD_NUM_14         14
#define GPIO_PAD_NUM_15         15
#define GPIO_PAD_NUM_16         16
#define GPIO_PAD_NUM_17         17
#define GPIO_PAD_NUM_18         18
#define GPIO_PAD_NUM_19         19
#define GPIO_PAD_NUM_20         20
#define GPIO_PAD_NUM_21         21
#define GPIO_PAD_NUM_22         22
#define GPIO_PAD_NUM_23         23
#define GPIO_PAD_NUM_24         24
#define GPIO_PAD_NUM_25         25
#define GPIO_PAD_NUM_26         26
#define GPIO_PAD_NUM_27         27
#define GPIO_PAD_NUM_28         28
#define GPIO_PAD_NUM_29         29
#define GPIO_PAD_NUM_30         30
#define GPIO_PAD_NUM_31         31
#define GPIO_PAD_NUM_32         32
#define GPIO_PAD_NUM_33         33
#define GPIO_PAD_NUM_34         34
#define GPIO_PAD_NUM_35         35
#define GPIO_PAD_NUM_36         36
#define GPIO_PAD_NUM_37         37
#define GPIO_PAD_NUM_38         38
#define GPIO_PAD_NUM_39         39
#define GPIO_PAD_NUM_40         40
#define GPIO_PAD_NUM_41         41
#define GPIO_PAD_NUM_42         42
#define GPIO_PAD_NUM_43         43
#define GPIO_PAD_NUM_44         44
#define GPIO_PAD_NUM_45         45
#define GPIO_PAD_NUM_46         46
#define GPIO_PAD_NUM_47         47
#define GPIO_PAD_NUM_48         48
#define GPIO_PAD_NUM_49         49
#define GPIO_PAD_NUM_50         50
#define GPIO_PAD_NUM_51         51
#define GPIO_PAD_NUM_52         52
#define GPIO_PAD_NUM_53         53
#define GPIO_PAD_NUM_54         54
#define GPIO_PAD_NUM_55         55
#define GPIO_PAD_NUM_56         56
#define GPIO_PAD_NUM_57         57
#define GPIO_PAD_NUM_58         58
#define GPIO_PAD_NUM_59         59
#define GPIO_PAD_NUM_60         60
#define GPIO_PAD_NUM_61         61
#define GPIO_PAD_NUM_62         62
#define GPIO_PAD_NUM_63         63


//Peripherals mapping
//Interface	PADS Required	Direction	OpenDrain	PULL??	PeripheralNum	Control
//UART		UA_TXD			OUT			N			N		0				HW
//			UA_RTS			OUT			N			N		1				HW
//			UA_RXD			IN			N			N		2				HW
//			UA_CTS			IN			N			N		3				HW
//CSP1		CSP1_SCLK		INOUT		N			N		4				HW
//			CSP1_TXD		INOUT		N			N		5				HW
//			CSP1_TFS		INOUT		N			N		6				HW
//			CSP1_RXD		INOUT		N			N		7				HW
//			CSP1_RFS		INOUT		N			N		8				HW
//CSP2		CSP2_SCLK		INOUT		N			N		9				HW
//			CSP2_TXD		INOUT		N			N		10				HW
//			CSP2_TFS		INOUT		N			N		11				HW
//			CSP2_RXD		INOUT		N			N		12				HW
//			CSP2_RFS		INOUT		N			N		13				HW
//I2C       I2C_SCLK		INOUT		Y			Weak1	14				HW
//			I2C_SDA			INOUT		Y			Weak1	15				HW
//SPI       SPI_SCLK		INOUT		N			N		16				HW
//			SPI_MOSI		INOUT		N			N		17				HW
//			SPI_SS_N		INOUT		N			N		18				HW
//			SPI_MISO		INOUT		N			N		19				HW
//			SPI_SS1_N		INOUT		N			N		20				HW
//			SPI_CS_N		IN			N			Weak1	38				HW
//Timer1    Tmr1PWM_OUTP	OUT			N			N		21				HW
//			Tmr1PWM_OUTN	OUT			N			N		22				HW
//			Tmr1Gate		IN			N			N		23				HW
//Timer2    Tmr2PWM_OUTP	OUT			N			N		24				HW
//			Tmr2PWM_OUTN	OUT			N			N		25				HW
//			Tmr2Gate		IN			N			N		26				HW
//Timer3    Tmr3PWM_OUTP	OUT			N			N		27				HW
//			Tmr3PWM_OUTN	OUT			N			N		28				HW
//			Tmr3Gate		IN			N			N		29				HW
//Timer4    Tmr4PWM_OUTP	OUT			N			N		30				HW
//			Tmr4PWM_OUTN	OUT			N			N		31				HW
//			Tmr4Gate		IN			N			N		32				HW
//AP JTAG/SWD	nTRST		IN			N			N		33				HW
//			SWCLKTCK		IN			N			N		34				HW
//			TDI				IN			N			N		35				HW
//			TDO				OUT			N			N		36				HW
//			SWDIOTMS		INOUT		N			N		37				HW
//DSP debug	dsp_jtrst		IN			N			N		39				HW
//			dsp_jtck		IN			N			N		40				HW
//			dsp_jtdi		IN			N			N		41				HW
//			dsp_jtms		IN			N			N		42				HW
//			dsp_jtdo		OUT			N			N		43				HW
//GPI       gpi[3:0]		IN			N			N		47-44			HW
//I2C2		I2C_SCLK		INOUT		Y			Weak1	48				HW
//			I2C_SDA			INOUT		Y			Weak1	49				HW
//CSP3		CSP3_SCLK		INOUT		N			N		50				HW
//			CSP3_TXD		INOUT		N			N		51				HW
//			CSP3_TFS		INOUT		N			N		52				HW
//			CSP3_RXD		INOUT		N			N		53				HW
//			CSP3_RFS		INOUT		N			N		54				HW
//CSP4		CSP4_SCLK		INOUT		N			N		55				HW
//			CSP4_TXD		INOUT		N			N		56				HW
//			CSP4_TFS		INOUT		N			N		57				HW
//			CSP4_RXD		INOUT		N			N		58				HW
//			CSP4_RFS		INOUT		N			N		59				HW
//QSPI      QSPI_CS0_N		OUT
//			QSPI_CS1_N		OUT
//			QSPI_CS2_N		OUT
//			QSPI_CS3_N		OUT
//			QSPI_SCLK		OUT
//			QSPI_SI			INOUT
//			QSPI_SO			INOUT
//			QSPI_HOLD_N		INOUT
//			QSPI_WP_N		INOUT
//GPIO		GPIO0			INOUT
//			GPIO1			INOUT
//			GPIO2			INOUT
//			GPIO3			INOUT


#define GPIO_UART_TXD           0
#define GPIO_UART_RTS           1
#define GPIO_UART_RXD           2
#define GPIO_UART_CTS           3
#define	GPIO_CSP1_SCLK			4
#define	GPIO_CSP1_TXD			5
#define	GPIO_CSP1_TFS			6
#define	GPIO_CSP1_RXD			7
#define	GPIO_CSP1_RFS			8
#define	GPIO_CSP2_SCLK			9
#define	GPIO_CSP2_TXD			10
#define	GPIO_CSP2_TFS			11
#define	GPIO_CSP2_RXD			12
#define	GPIO_CSP2_RFS			13
#define	GPIO_I2C1_SCLK			14
#define	GPIO_I2C1_SDA			15
#define	GPIO_SPI_SCLK			16
#define	GPIO_SPI_MOSI			17
#define	GPIO_SPI_SS_N			18
#define	GPIO_SPI_MISO			19
#define	GPIO_SPI_SS1_N			20
#define	GPIO_SPI_CS_N			38
#define	GPIO_TMR1PWM_OUTP		21
#define	GPIO_TMR1PWM_OUTN		22
#define	GPIO_TMR1Gate			23
#define	GPIO_TMR2PWM_OUTP		24
#define	GPIO_TMR2PWM_OUTN		25
#define	GPIO_TMR2Gate			26
#define	GPIO_TMR3PWM_OUTP		27
#define	GPIO_TMR3PWM_OUTN		28
#define	GPIO_TMR3Gate			29
#define	GPIO_TMR4PWM_OUTP		30
#define	GPIO_TMR4PWM_OUTN		31
#define	GPIO_TMR4Gate			32
#define	GPIO_AP_nTRST			33
#define	GPIO_AP_SWCLKTCK		34
#define	GPIO_AP_TDI				35
#define	GPIO_AP_TDO				36
#define	GPIO_AP_SWDIOTMS		37
#define	GPIO_DSP_JTRST 			39
#define	GPIO_DSP_JTCK  			40
#define	GPIO_DSP_JTDI  			41
#define	GPIO_DSP_JTMS  			42
#define	GPIO_DSP_JTDO  			43
#define	GPIO_GPI3				47
#define	GPIO_GPI2				46
#define	GPIO_GPI1				45
#define	GPIO_GPI0				44
#define	GPIO_I2C2_SCLK			48
#define	GPIO_I2C2_SDA			49
#define	GPIO_CSP3_SCLK			50
#define	GPIO_CSP3_TXD			51
#define	GPIO_CSP3_TFS			52
#define	GPIO_CSP3_RXD			53
#define	GPIO_CSP3_RFS			54
#define	GPIO_CSP4_SCLK			55
#define	GPIO_CSP4_TXD			56
#define	GPIO_CSP4_TFS			57
#define	GPIO_CSP4_RXD			58
#define	GPIO_CSP4_RFS			59

#if !USE_ROM_GPIO
extern void GPIOPeripheralPad(unsigned char ucPeriNum, unsigned char ucPadNum);
extern void GPIODirModeSet(unsigned char ucPins, unsigned long ulPinIO);
extern void GPIOOutputODEn(unsigned char ucPins);
extern void GPIOOutputODDis(unsigned char ucPins);
extern void GPIOAnalogEn(unsigned char ucPins);
extern void GPIOAnalogDis(unsigned char ucPins);
extern void GPIOPullupEn(unsigned char ucPins);
extern void GPIOPullupDis(unsigned char ucPins);
extern void GPIOPulldownEn(unsigned char ucPins);
extern void GPIOPulldownDis(unsigned char ucPins);
extern void GPIOPinSet(unsigned char ucPins);
extern void GPIOPinClear(unsigned char ucPins);
extern unsigned char GPIOPinRead(unsigned char ucPins);
extern void GPIOIntEnable(unsigned char ucPins);
extern void GPIOIntDisable(unsigned char ucPins);
extern void GPIOIntTypeSet(unsigned char ucPins, unsigned char ucConfig);
extern void GPIOIntEdgeSet(unsigned char ucPins, unsigned char ucConfig);
extern void GPIOIntSingleEdgeSet(unsigned char ucPins, unsigned char ucConfig);
extern unsigned long GPIOIntStatusGet(unsigned char ucPins);
extern unsigned long GPIOIntMaskGet(unsigned char ucPins);
extern unsigned long GPIOConflictStatusGet(unsigned char ucPins);
extern void GPIOConflictStatusClear(unsigned char ucPins);
extern unsigned long GPIOAllocationStatusGet(unsigned char ucPins);
extern void GPIODrvStrengthSet(unsigned char ucPins, unsigned char ucDrvStrength);
extern void GPIOIntRegister(unsigned long *g_pRAMVectors, void (*pfnHandler)(void));
extern void GPIOIntUnregister(unsigned long *g_pRAMVectors);
extern void GPIOModeSet(unsigned char ucPins, unsigned char ucMode);
extern void GPIOPadConfigSel(unsigned char ucPins, unsigned char ucConfig);
extern void GPIOPadModeSet(unsigned char ucPins, unsigned char ucMode, unsigned char ucConfig);
extern void GPIODirectionSet(unsigned char ucPins, unsigned long ulPinIO);
#endif

//*****************************************************************************
//
// Mark the end of the C bindings section for C++ compilers.
//
//*****************************************************************************
#ifdef __cplusplus
}
#endif

#endif //  __GPIO_H__
