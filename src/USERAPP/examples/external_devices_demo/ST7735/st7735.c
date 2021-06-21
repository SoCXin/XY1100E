/*
 *firmware's information:
 *TFT LCD MODULE
 *0.96 inch 80RGB*160DOTS
 *MODULE NUMBER: N096-1608TBBIG11-H13
 *REVISION: A
 */
#include "st7735.h"
#include "spi_lcd.h"

#if DEMO_TEST

void Disp_Windows(void)
{
	Lcd_Write_Cmd(0x2a);
	Lcd_Write_Data(0x00);
	Lcd_Write_Data(0x02);
	Lcd_Write_Data(0x00);
	Lcd_Write_Data(0x81);

	Lcd_Write_Cmd(0x2b);
	Lcd_Write_Data(0x00);
	Lcd_Write_Data(0x01);
	Lcd_Write_Data(0x00);
	Lcd_Write_Data(0xa0);

	Lcd_Write_Cmd(0x2c);
}

void Disp_Color(unsigned int color)
{
	int i=0,j=0;
	Disp_Windows();
	for(i=0;i<Lcd_Height;i++)
		for(j=0;j<Lcd_Width;j++)
			Lcd_Write_Data16(color);
}

void St7735_Rest(void)
{
	HAL_GPIO_WritePin(LCD_RST_PIN,HAL_GPIO_PIN_RESET);

	osDelay(10);

	HAL_GPIO_WritePin(LCD_RST_PIN,HAL_GPIO_PIN_SET);

	osDelay(10);
}

void St7735_SleepOut(void)
{
	Lcd_Write_Cmd(0x11);

	osDelay(10);
}

void St7735_Init(void)
{
	St7735_Rest();
	St7735_SleepOut();
	Lcd_Write_Cmd(0xB1);
	Lcd_Write_Data(0x05);
	Lcd_Write_Data(0x3A);
	Lcd_Write_Data(0x3A);

	Lcd_Write_Cmd(0xB2);
	Lcd_Write_Data(0x05);
	Lcd_Write_Data(0x3A);
	Lcd_Write_Data(0x3A);

	Lcd_Write_Cmd(0xB3);
	Lcd_Write_Data(0x05);
	Lcd_Write_Data(0x3A);
	Lcd_Write_Data(0x3A);
	Lcd_Write_Data(0x05);
	Lcd_Write_Data(0x3A);
	Lcd_Write_Data(0x3A);

	Lcd_Write_Cmd(0xB4);
	Lcd_Write_Data(0x07);

	Lcd_Write_Cmd(0xC0);
	Lcd_Write_Data(0x44);
	Lcd_Write_Data(0x04);
	Lcd_Write_Data(0x04);

	Lcd_Write_Cmd(0xC1);
	Lcd_Write_Data(0xC0);

	Lcd_Write_Cmd(0xC2);
	Lcd_Write_Data(0x0D);
	Lcd_Write_Data(0x00);

	Lcd_Write_Cmd(0xC3);
	Lcd_Write_Data(0x8D);
	Lcd_Write_Data(0x6A);

	Lcd_Write_Cmd(0xC4);
	Lcd_Write_Data(0x8D);
	Lcd_Write_Data(0xEE);

	Lcd_Write_Cmd(0xC5);
	Lcd_Write_Data(0x01);

	Lcd_Write_Cmd(0xE0);
	Lcd_Write_Data(0x0F);
	Lcd_Write_Data(0x11);
	Lcd_Write_Data(0x02);
	Lcd_Write_Data(0x01);
	Lcd_Write_Data(0x0A);
	Lcd_Write_Data(0x01);
	Lcd_Write_Data(0x00);
	Lcd_Write_Data(0x01);
	Lcd_Write_Data(0x06);
	Lcd_Write_Data(0x0E);
	Lcd_Write_Data(0x1B);
	Lcd_Write_Data(0x38);
	Lcd_Write_Data(0x0A);
	Lcd_Write_Data(0x0C);
	Lcd_Write_Data(0x03);
	Lcd_Write_Data(0x1F);

	Lcd_Write_Cmd(0xE1);
	Lcd_Write_Data(0x10);
	Lcd_Write_Data(0x09);
	Lcd_Write_Data(0x04);
	Lcd_Write_Data(0x03);
	Lcd_Write_Data(0x0B);
	Lcd_Write_Data(0x02);
	Lcd_Write_Data(0x00);
	Lcd_Write_Data(0x02);
	Lcd_Write_Data(0x03);
	Lcd_Write_Data(0x07);
	Lcd_Write_Data(0x10);
	Lcd_Write_Data(0x33);
	Lcd_Write_Data(0x0B);
	Lcd_Write_Data(0x0E);
	Lcd_Write_Data(0x07);
	Lcd_Write_Data(0x1E);

	Lcd_Write_Cmd(0x35);
	Lcd_Write_Data(0x00);

	Lcd_Write_Cmd(0x3A);
	Lcd_Write_Data(0x05);

	Lcd_Write_Cmd(0x36);
	Lcd_Write_Data(normal);

	Lcd_Write_Cmd(0x29);

	Lcd_Write_Cmd(0x2C);
}

void lcd_address(int XS,int YS,int x_total,int y_total)
{
	int XE,YE;
	XE = XS + x_total-1;
	YE = YS + y_total-1;
	Lcd_Write_Cmd(0x2a);
	Lcd_Write_Data16(XS);
	Lcd_Write_Data16(XE);
	Lcd_Write_Cmd(0x2b);
	Lcd_Write_Data16(YS);
	Lcd_Write_Data16(YE);
	Lcd_Write_Cmd(0x2c);
}

void ascii_write_data(int ascii0,int font_color,int back_color)
{
	int i;
	for(i=0;i<8;i++)
	{
		if(ascii0 & 0x80)
		{
			Lcd_Write_Data16(font_color);
		}
		else
		{
			Lcd_Write_Data16(back_color);
		}

		ascii0 <<= 1;
	}
}

void disp_32x32(int x,int y,char *dp,int font_color,int back_color)
{
	int i,j;
	lcd_address(x,y,32,32);
	for(i=0;i<32;i++)
	{
		for(j=0;j<4;j++)
		{
			ascii_write_data(*dp,font_color,back_color);
			dp++;
		}
	}
}
#endif
