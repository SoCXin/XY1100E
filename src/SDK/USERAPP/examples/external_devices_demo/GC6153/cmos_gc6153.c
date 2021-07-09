#include "cmos_gc6153.h"

void Delay(uint32_t data)
{
    for(uint32_t count = 0; count < data * 1000; count++);
}

/**
  * @brief This function initialize the registers of CMOS sensor
  * @note    the output format should be YUV422, order: YUYV
*            data output should be at pclk falling edge
*            VSYNC should be low active
*            HSYNC should be hight active
  * @param[in]  None
  * @retval None
  */
 void GC6153_Initial_Setting(GC6153_SERIALSensor* cmos_senser)
 {
	/*SYS*/
	cmos_senser->I2C_Write_Reg(0xfe, 0xa0);
	cmos_senser->I2C_Write_Reg(0xfe, 0xa0);
	cmos_senser->I2C_Write_Reg(0xfe, 0xa0);
	cmos_senser->I2C_Write_Reg(0xfa, 0x11);
	cmos_senser->I2C_Write_Reg(0xfc, 0x00);
	cmos_senser->I2C_Write_Reg(0xf6, 0x00);
	cmos_senser->I2C_Write_Reg(0xfc, 0x12); //1a herve 20181207

	/*ANALOG & CISCTL*/  
	cmos_senser->I2C_Write_Reg(0xfe, 0x00);
	cmos_senser->I2C_Write_Reg(0x01, 0x40);
	cmos_senser->I2C_Write_Reg(0x02, 0x12);
	cmos_senser->I2C_Write_Reg(0x0d, 0x40);
	cmos_senser->I2C_Write_Reg(0x14, MIRROR);
	cmos_senser->I2C_Write_Reg(0x16, AD_NUM);
	cmos_senser->I2C_Write_Reg(0x17, COL_SWITCH);
	cmos_senser->I2C_Write_Reg(0x1c, 0x31);
	cmos_senser->I2C_Write_Reg(0x1d, 0xbb);
	cmos_senser->I2C_Write_Reg(0x1f, 0x3f);
	cmos_senser->I2C_Write_Reg(0x73, 0x20);
	cmos_senser->I2C_Write_Reg(0x74, 0x71);
	cmos_senser->I2C_Write_Reg(0x77, 0x22);
	cmos_senser->I2C_Write_Reg(0x7a, 0x08);
	cmos_senser->I2C_Write_Reg(0x11, 0x18);
	cmos_senser->I2C_Write_Reg(0x13, 0x48);
	cmos_senser->I2C_Write_Reg(0x12, 0xc8);
	cmos_senser->I2C_Write_Reg(0x70, 0xc8);
	cmos_senser->I2C_Write_Reg(0x7b, 0x18);
	cmos_senser->I2C_Write_Reg(0x7d, 0x30);
	cmos_senser->I2C_Write_Reg(0x7e, 0x02);

	cmos_senser->I2C_Write_Reg(0xfe, 0x10);
	cmos_senser->I2C_Write_Reg(0xfe, 0x00);
	cmos_senser->I2C_Write_Reg(0xfe, 0x00);
	cmos_senser->I2C_Write_Reg(0xfe, 0x00);
	cmos_senser->I2C_Write_Reg(0xfe, 0x00);
	cmos_senser->I2C_Write_Reg(0xfe, 0x00);
	cmos_senser->I2C_Write_Reg(0xfe, 0x10);
	cmos_senser->I2C_Write_Reg(0xfe, 0x00);

	cmos_senser->I2C_Write_Reg(0x49, 0x61);
	cmos_senser->I2C_Write_Reg(0x4a, 0x40);
	cmos_senser->I2C_Write_Reg(0x4b, 0x58);

	/*ISP*/                     
	cmos_senser->I2C_Write_Reg(0xfe, 0x00);
	cmos_senser->I2C_Write_Reg(0x39, 0x02);
	cmos_senser->I2C_Write_Reg(0x3a, 0x80);
	cmos_senser->I2C_Write_Reg(0x20, 0x7e);
	cmos_senser->I2C_Write_Reg(0x26, 0x87);

	/*BLK*/               
	cmos_senser->I2C_Write_Reg(0x33, 0x10);
	cmos_senser->I2C_Write_Reg(0x37, 0x06);
	cmos_senser->I2C_Write_Reg(0x2a, 0x21);

	/*GAIN*/                    
	cmos_senser->I2C_Write_Reg(0x3f, 0x16);

	/*DNDD*/                     
	cmos_senser->I2C_Write_Reg(0x52, 0xa6);///ab-0909    //a6   0917
	cmos_senser->I2C_Write_Reg(0x53, 0x81);///81-0909   //84  0917
	cmos_senser->I2C_Write_Reg(0x54, 0x43);
	cmos_senser->I2C_Write_Reg(0x56, 0x78);
	cmos_senser->I2C_Write_Reg(0x57, 0xaa);
	cmos_senser->I2C_Write_Reg(0x58, 0xff);
	                          
	/*ASDE*/                  
	cmos_senser->I2C_Write_Reg(0x5b, 0x60);
	cmos_senser->I2C_Write_Reg(0x5c, 0x50);
	cmos_senser->I2C_Write_Reg(0xab, 0x2a);
	cmos_senser->I2C_Write_Reg(0xac, 0xb5);
	                             
	/*INTPEE*/                   
	cmos_senser->I2C_Write_Reg(0x5e, 0x06);
	cmos_senser->I2C_Write_Reg(0x5f, 0x06);
	cmos_senser->I2C_Write_Reg(0x60, 0x44);
	cmos_senser->I2C_Write_Reg(0x61, 0xff);
	cmos_senser->I2C_Write_Reg(0x62, 0x69);
	cmos_senser->I2C_Write_Reg(0x63, 0x13);///13-0909   //14   0917
	
	/*CC*/                      
	cmos_senser->I2C_Write_Reg(0x65, 0x13);
	cmos_senser->I2C_Write_Reg(0x66, 0x26);
	cmos_senser->I2C_Write_Reg(0x67, 0x07);
	cmos_senser->I2C_Write_Reg(0x68, 0xf5);
	cmos_senser->I2C_Write_Reg(0x69, 0xea);
	cmos_senser->I2C_Write_Reg(0x6a, 0x21);
	cmos_senser->I2C_Write_Reg(0x6b, 0x21);
	cmos_senser->I2C_Write_Reg(0x6c, 0xe4);
	cmos_senser->I2C_Write_Reg(0x6d, 0xfb);
	                             
	/*YCP*/                     
	cmos_senser->I2C_Write_Reg(0x81, 0x3b);///3b-0909
	cmos_senser->I2C_Write_Reg(0x82, 0x3b);
	cmos_senser->I2C_Write_Reg(0x83, 0x4b);///4b-0909    50   0917
	cmos_senser->I2C_Write_Reg(0x84, 0x90);
	cmos_senser->I2C_Write_Reg(0x86, 0xf0);
	cmos_senser->I2C_Write_Reg(0x87, 0x1d);
	cmos_senser->I2C_Write_Reg(0x88, 0x16);
	cmos_senser->I2C_Write_Reg(0x8d, 0x74);
	cmos_senser->I2C_Write_Reg(0x8e, 0x25);
	                            
	/*AEC*/                     
	cmos_senser->I2C_Write_Reg(0x90, 0x36);
	cmos_senser->I2C_Write_Reg(0x92, 0x43);
	cmos_senser->I2C_Write_Reg(0x9d, 0x32);
	cmos_senser->I2C_Write_Reg(0x9e, 0x81);//81   0917
	cmos_senser->I2C_Write_Reg(0x9f, 0xf4);
	cmos_senser->I2C_Write_Reg(0xa0, 0xa0);
	cmos_senser->I2C_Write_Reg(0xa1, 0x04);
	cmos_senser->I2C_Write_Reg(0xa3, 0x2d);
	cmos_senser->I2C_Write_Reg(0xa4, 0x01);
	                            
	/*AWB*/                     
	cmos_senser->I2C_Write_Reg(0xb0, 0xc2);
	cmos_senser->I2C_Write_Reg(0xb1, 0x1e);
	cmos_senser->I2C_Write_Reg(0xb2, 0x10);
	cmos_senser->I2C_Write_Reg(0xb3, 0x20);
	cmos_senser->I2C_Write_Reg(0xb4, 0x2d);
	cmos_senser->I2C_Write_Reg(0xb5, 0x1b);
	cmos_senser->I2C_Write_Reg(0xb6, 0x2e);
	cmos_senser->I2C_Write_Reg(0xb8, 0x13);
	cmos_senser->I2C_Write_Reg(0xba, 0x60);
	cmos_senser->I2C_Write_Reg(0xbb, 0x62);
	cmos_senser->I2C_Write_Reg(0xbd, 0x78);
	cmos_senser->I2C_Write_Reg(0xbe, 0x55);
	cmos_senser->I2C_Write_Reg(0xbf, 0xa0);
	cmos_senser->I2C_Write_Reg(0xc4, 0xe7);
	cmos_senser->I2C_Write_Reg(0xc5, 0x15);
	cmos_senser->I2C_Write_Reg(0xc6, 0x16);
	cmos_senser->I2C_Write_Reg(0xc7, 0xeb);
	cmos_senser->I2C_Write_Reg(0xc8, 0xe4);
	cmos_senser->I2C_Write_Reg(0xc9, 0x16);
	cmos_senser->I2C_Write_Reg(0xca, 0x16);
	cmos_senser->I2C_Write_Reg(0xcb, 0xe9);
	cmos_senser->I2C_Write_Reg(0x22, 0xf8);
	                           
	/*SPI*/                    
	cmos_senser->I2C_Write_Reg(0xfe, 0x02);
	cmos_senser->I2C_Write_Reg(0x01, 0x01);
	cmos_senser->I2C_Write_Reg(0x02, 0x80);
	cmos_senser->I2C_Write_Reg(0x03, 0x20);
	cmos_senser->I2C_Write_Reg(0x04, 0x20);
	cmos_senser->I2C_Write_Reg(0x0a, 0x00);
	cmos_senser->I2C_Write_Reg(0x13, 0x10);
	cmos_senser->I2C_Write_Reg(0x24, 0x00);
	cmos_senser->I2C_Write_Reg(0x28, 0x03);
	cmos_senser->I2C_Write_Reg(0xfe, 0x00);
	                            
	/*OUTPUT*/
	cmos_senser->I2C_Write_Reg(0xf2, 0x03);
	cmos_senser->I2C_Write_Reg(0xfe, 0x00);
 }


 /**
  * @brief  
  * @param[in]  None
  * @retval None
  */

void GC6153_Set_Clock(GC6153_SERIALSensor* cmos_senser, uint32_t InternalClock)
{
    if (cmos_senser->InternalClock == InternalClock)
    {
        return;
    }
    cmos_senser->InternalClock = InternalClock;
    uint8_t ClkDiv;
    /* add PLL comment here */
    switch (InternalClock)
    {
		case GC6153_SERIAL_MCLK / 2:
			ClkDiv = 0x00; 
			break; /* pclk = mclk */
		case GC6153_SERIAL_MCLK / 4: 
			ClkDiv = 0x11; 
			break; /* pclk = mclk / 2 */
		case GC6153_SERIAL_MCLK / 8: 
			ClkDiv = 0x32; 
			break; /* pclk = mclk / 4 */
		default:
			return;
			break;
     }
    cmos_senser->I2C_Write_Reg(0xfa, ClkDiv);
}


/**
  * @brief This function initialize the registers of CMOS sensor
  * @note    the output format should be YUV422, order: YUYV
*            data output should be at pclk falling edge
*            VSYNC should be low active
*            HSYNC should be hight active
  * @param[in]  None
  * @retval sensor Id
  */
int32_t GC6153_Power_On(GC6153_SERIALSensor* cmos_senser)
{
    int32_t SensorId;
	
    cmos_senser->Clk_Set(true);
    Delay(20);
    cmos_senser->Pwn_Set(0);
	Delay(20);
	cmos_senser->Pwn_Set(1);
	Delay(20);
	cmos_senser->Pwn_Set(0);;
	Delay(10);

    SensorId = (cmos_senser->I2C_Read_Reg(0xf0) << 8)|cmos_senser->I2C_Read_Reg(0xf1);
    return SensorId;
}

/**
  * @brief   This function initialize the registers of CMOS sensor
  * @note    the output format should be YUV422, order: YUYV
*            data output should be at pclk falling edge
*            VSYNC should be low active
*            HSYNC should be hight active
  * @param[in]  None
  * @retval None
  */
void GC6153_Power_Down(GC6153_SERIALSensor* cmos_senser)
{
    cmos_senser->Rst_Set(0);
	Delay(1);
    cmos_senser->Pwn_Set(1);
}


int32_t GC6153_Open(GC6153_SERIALSensor* cmos_senser)
{
    if (GC6153_Power_On(cmos_senser) != 0x6153)
    {
        return -1;
    }

    GC6153_Initial_Setting(cmos_senser); /* apply the sensor initial setting */

    /* default setting */
    cmos_senser->BypassAe = false; /* for panorama view */
    cmos_senser->BypassAwb = false; /* for panorama view */
    cmos_senser->CapState = false; /* preview state */
    cmos_senser->PvMode = false; /* preview size output mode */
    cmos_senser->VideoMode = false; /* KAL_TRUE: video mode, else preview mode */
    cmos_senser->NightMode = false;/*KAL_TRUE:work in night mode, else normal mode*/
    cmos_senser->BandingFreq = GC6153_SERIAL_50HZ;
    cmos_senser->InternalClock = 1; /* will be update by setclock function */
    cmos_senser->Shutter = 1; /* default shutter 1 avoid divide by 0 */
    cmos_senser->Gain = 0x40; /* default gain 1x */
    //GC6153_SERIALSensor.FrameLength = GC6153_SERIAL_PV_PERIOD_LINE_NUMS; /* will be update by setdummy function */
    //GC6153_SERIALSensor.LineLength = GC6153_SERIAL_PV_PERIOD_PIXEL_NUMS; /* will be update by setdummy function */

    return 0;
}


