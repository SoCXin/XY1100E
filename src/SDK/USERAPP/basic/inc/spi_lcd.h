#ifndef _SPI_LCD_H
#define _SPI_LCD_H

#include "xy_api.h"
//these define are provide a modification for user

//#define SPI_MOSI_PIN          HAL_GPIO_PIN_NUM_21
//#define SPI_SCLK_PIN          HAL_GPIO_PIN_NUM_20
//#define SPI_NSS1_PIN          HAL_GPIO_PIN_NUM_6
//
//#define SPI_CLKDIV            HAL_SPI_CLKDIV_2      //can use: 2 4 8 16 32 64 128 256
//#define SPI_WORK_MODE         HAL_SPI_WORKMODE_3     //can use: 0 1 2 3
//#define SPI_DATA_WIDTH        HAL_SPI_DATASIZE_8BIT    //can use: 8 16 32

#define LCD_RS_PIN            HAL_GPIO_PIN_NUM_20
#define LCD_RST_PIN           HAL_GPIO_PIN_NUM_21

#define SPI_MASTER_TASK_PRIORITY    10
#define SPI_MASTER_STACK_SIZE       1024

void Lcd_Write_Cmd(unsigned char cmd);
void Lcd_Write_Data(unsigned char data);
void Lcd_Write_Data16(unsigned int data);
void lcd_spi_master_gpio_init(void);
void lcd_spi_master_work_task_init(void);

extern HAL_SPI_HandleTypeDef spi_lcd;
#endif

