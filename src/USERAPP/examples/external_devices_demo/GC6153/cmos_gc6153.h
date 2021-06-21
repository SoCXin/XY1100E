/**
* @file        cmos_gc6153.h
* @brief       这个文件是GC6153驱动头文件. 
**********************************************************************************
* @attention
* User can not call the function in flash when excute interrupt handler, include the subfunction, if necessary,
* put this function in ram, just declare this function with a suffix " __attribute__((section(".ramtext"))) "
*
**********************************************************************************
*/

#pragma once

#include <stdint.h>
#include <stdbool.h>

#define GC6153_SERIAL_MCLK                          10000000
#define GC6153_SERIAL_WRITE_ID_0                    (0x80)
/* SENSOR PREVIEW SIZE */
#define GC6153_SERIAL_IMAGE_SENSOR_PV_WIDTH         (240)
#define GC6153_SERIAL_IMAGE_SENSOR_PV_HEIGHT        (320)

/* SENSOR CAPTURE SIZE */
#define GC6153_SERIAL_IMAGE_SENSOR_FULL_WIDTH       (240)
#define GC6153_SERIAL_IMAGE_SENSOR_FULL_HEIGHT      (320)

/* SENSOR PIXEL/LINE NUMBERS IN ONE PERIOD */
#define GC6153_SERIAL_PV_PERIOD_PIXEL_NUMS          (240)
#define GC6153_SERIAL_PV_PERIOD_LINE_NUMS           (320)
#define GC6153_SERIAL_FULL_PERIOD_PIXEL_NUMS        (240)
#define GC6153_SERIAL_FULL_PERIOD_LINE_NUMS         (320)

/* ===============================================================================
   ========================= No Changing The Macro Below =========================
   ===============================================================================
*/
/* CONFIG THE ISP GRAB START X & START Y, CONFIG THE ISP GRAB WIDTH & HEIGHT */
#define GC6153_SERIAL_PV_GRAB_START_X               (0)
#define GC6153_SERIAL_PV_GRAB_START_Y               (1)
#define GC6153_SERIAL_FULL_GRAB_START_X             (0)
#define GC6153_SERIAL_FULL_GRAB_START_Y             (1)
#define GC6153_SERIAL_PV_GRAB_WIDTH                 (GC6153_SERIAL_IMAGE_SENSOR_PV_WIDTH - 2)
#define GC6153_SERIAL_PV_GRAB_HEIGHT                (GC6153_SERIAL_IMAGE_SENSOR_PV_HEIGHT - 2)
#define GC6153_SERIAL_FULL_GRAB_WIDTH               (GC6153_SERIAL_IMAGE_SENSOR_FULL_WIDTH - 2)
#define GC6153_SERIAL_FULL_GRAB_HEIGHT              (GC6153_SERIAL_IMAGE_SENSOR_FULL_HEIGHT - 2)

/* FLICKER OF FREQUENCY */
#define GC6153_SERIAL_50HZ                          100
#define GC6153_SERIAL_60HZ                          120

/* HW I2C SPEED */
#define GC6153_SERIAL_HW_I2C_SPEED                  100 /* Kbps */

/* FRAME RATE UNIT */
#define GC6153_SERIAL_FPS(x)                        ((uint32_t)(10 * (x)))


#define IMAGE_NORMAL 
 
#ifdef IMAGE_NORMAL
#define MIRROR 0x7c
#define AD_NUM 0x05
#define COL_SWITCH 0x18
#endif

#ifdef IMAGE_H_MIRROR
#define MIRROR 0x69
#define AD_NUM 0x04
#define COL_SWITCH 0x19
#endif

#ifdef IMAGE_V_MIRROR
#define MIRROR 0x7e
#define AD_NUM 0x05
#define COL_SWITCH 0x18
#endif

#ifdef IMAGE_HV_MIRROR
#define MIRROR 0x6b
#define AD_NUM 0x04
#define COL_SWITCH 0x19
#endif

typedef struct
{
    bool BypassAe;
    bool BypassAwb;
    bool CapState; ///< KAL_TRUE: in capture state, else in preview state 
    bool PvMode; ///< KAL_TRUE: preview size output, else full size output 
    bool VideoMode; ///< KAL_TRUE: video mode, else preview mode 
	bool NightMode;///< KAL_TRUE:work in night mode, else normal mode
    uint8_t BandingFreq; ///< GC6153_SERIAL_50HZ or GC6153_SERIAL_60HZ for 50Hz/60Hz 
    uint32_t InternalClock; ///< internal clock which using process pixel(for exposure) 
    uint32_t Pclk; ///< output clock which output to baseband 
    uint32_t Gain; ///< base on 0x40 
    uint32_t Shutter; ///< unit is (linelength / internal clock) s 
    uint32_t FrameLength; ///< total line numbers in one frame(include dummy line) 
    uint32_t LineLength; ///< total pixel numbers in one line(include dummy pixel) 
    //sensor_data_struct *NvramData;
    void (*I2C_Write_Reg) (uint8_t reg_address, uint8_t data);
    uint8_t (*I2C_Read_Reg) (uint8_t reg_address);
    void (*Rst_Set)(int i);
    void (*Pwn_Set)(int i);
    void (*Clk_Set)(bool option);
} GC6153_SERIALSensor;

void GC6153_Set_Clock(GC6153_SERIALSensor* coms_senser, uint32_t InternalClock);

/**
 * @brief 
 * 
 * @param cmos_senser 
 * @return int32_t 
 */
int32_t GC6153_Open(GC6153_SERIALSensor* cmos_senser);

/**
  * @brief This function initialize the registers of CMOS sensor
  * @note    the output format should be YUV422, order: YUYV
*            data output should be at pclk falling edge
*            VSYNC should be low active
*            HSYNC should be hight active
  * @param[in]  None
  * @retval None
  */
 void GC6153_Initial_Setting(GC6153_SERIALSensor* cmos_senser);

 /**
  * @brief This function initialize the registers of CMOS sensor
  * @note    the output format should be YUV422, order: YUYV
*            data output should be at pclk falling edge
*            VSYNC should be low active
*            HSYNC should be hight active
  * @param[in]  None
  * @retval sensor Id
  */
int32_t GC6153_Power_On(GC6153_SERIALSensor* cmos_senser);

/**
  * @brief This function initialize the registers of CMOS sensor
  * @note    the output format should be YUV422, order: YUYV
*            data output should be at pclk falling edge
*            VSYNC should be low active
*            HSYNC should be hight active
  * @param[in]  None
  * @retval None
  */
void GC6153_Power_Down(GC6153_SERIALSensor* cmos_senser);
