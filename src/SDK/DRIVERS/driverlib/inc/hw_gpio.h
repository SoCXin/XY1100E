#ifndef __HW_GPIO_H__
#define __HW_GPIO_H__


//*****************************************************************************
//
// The following are defines for the SPI register offsets.
//
//*****************************************************************************
#define GPIO_DIN0                       0x00000000
#define GPIO_DIN1                       0x00000004
#define GPIO_DOUT0                      0x00000008
#define GPIO_DOUT1                      0x0000000C
#define GPIO_CTRL0                      0x00000010
#define GPIO_CTRL1                      0x00000014
#define GPIO_PULLUP0                    0x00000018
#define GPIO_PULLUP1                    0x0000001C
#define GPIO_PULLDOWN0                  0x00000020
#define GPIO_PULLDOWN1                  0x00000024
#define GPIO_INPUTEN0                   0x00000028
#define GPIO_INPUTEN1                   0x0000002C
#define GPIO_OD0                        0x00000030
#define GPIO_OD1                        0x00000034
#define GPIO_OUTPUTEN0                  0x00000038
#define GPIO_OUTPUTEN1                  0x0000003C
#define GPIO_CFGSEL0                    0x00000040
#define GPIO_CFGSEL1                    0x00000044
#define GPIO_INTEN0                     0x00000048
#define GPIO_INTEN1                     0x0000004C
#define GPIO_INT_TYPE_LEVEL0            0x00000050
#define GPIO_INT_TYPE_LEVEL1            0x00000054
#define GPIO_INT_BOTH_EDGE0             0x00000058
#define GPIO_INT_BOTH_EDGE1             0x0000005C
#define GPIO_INT_FALL_EDGE0             0x00000060
#define GPIO_INT_FALL_EDGE1             0x00000064
#define GPIO_INT_STAT0                  0x00000068
#define GPIO_INT_STAT1                  0x0000006C
#define GPIO_INT_MASK0                  0x00000070
#define GPIO_INT_MASK1                  0x00000074
#define GPIO_PAD_CFT0                   0x00000078
#define GPIO_PAD_CFT1                   0x0000007C
#define GPIO_PAD_BLKS0                  0x00000080
#define GPIO_PAD_BLKS1                  0x00000084
#define GPIO_ANALOG_EN0                 0x00000088
#define GPIO_ANALOG_EN1                 0x0000008C
#define GPIO_DRVCTL0                    0x00000090
#define GPIO_DRVCTL1                    0x00000094
#define GPIO_DRVCTL2                    0x00000098
#define GPIO_DRVCTL3                    0x0000009C
#define GPIO_DRVCTL4                    0x000000A0
#define GPIO_DRVCTL5                    0x000000A4
#define GPIO_DRVCTL6                    0x000000A8
#define GPIO_PAD_SEL0                   0x000000AC
#define GPIO_PAD_SEL63                  0x000001A8


#endif // __HW_GPIO_H__
