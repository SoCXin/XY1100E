#ifndef _ST7735_H
#define _ST7735_H

/*
 *lcd's width and height
 *
 */
#define Lcd_Width 	128
#define Lcd_Height 	160

/*
 *lcd's screen orientation
 *normal: Normal vertical
 *CW90:  Turn 180 degrees vertical on normal vertical
 *CCW90:  Turn 90 degrees crosswise on a vertical basis
 *CW180:  90 degrees horizontal on vertical basis
 *
 */
#define normal      0xc8
#define CW90        0x68
#define CCW90       0xa8
#define CW180       0x08

/*
 *lcd's color definition
 *
 */
#define WHITE         	 0xFFFF
#define RED           	 0xF800
#define GREEN         	 0x07E0
#define BLUE         	 0x001F
#define YELLOW        	 0xFFE0
#define BLACK         	 0x0000
#define PURPLE         	 0xF81F


void St7735_Init(void);
void Disp_Color(unsigned int color);
void disp_32x32(int x,int y,char *dp,int font_color,int back_color);

#endif

