#include "st7735.h"
#include "spi_lcd.h"
#include "font.h"

#if DEMO_TEST

osThreadId_t  g_lcd_spi_master_TskHandle =NULL;
HAL_SPI_HandleTypeDef spi_lcd;

static void Lcd_Spi_Send(unsigned char *data,unsigned int size)
{
	unsigned int i=0;

	for(i=0;i<size;i++)
	{
		HAL_SPI_ChipSelect(&spi_lcd, HAL_SPI_CS_PIN_1);
		HAL_SPI_Transmit(&spi_lcd, data, size, 0);
	    HAL_SPI_ChipSelect(&spi_lcd, HAL_SPI_CS_NONE);
	}
}

void Lcd_Write_Cmd(unsigned char cmd)
{
	HAL_GPIO_WritePin(LCD_RS_PIN,HAL_GPIO_PIN_RESET);

	Lcd_Spi_Send(&cmd,1);
}

void Lcd_Write_Data(unsigned char data)
{
	HAL_GPIO_WritePin(LCD_RS_PIN,HAL_GPIO_PIN_SET);

	Lcd_Spi_Send(&data,1);
}

void Lcd_Write_Data16(unsigned int data)
{
	unsigned char dat16[2];

	dat16[0] = data >> 8;

	dat16[1] = data;

	HAL_GPIO_WritePin(LCD_RS_PIN,HAL_GPIO_PIN_SET);

	Lcd_Spi_Send(dat16,2);
}

void lcd_spi_master_gpio_init(void)
{
	spi_lcd.Instance = HAL_SPI;
	spi_lcd.Init.Mode = HAL_SPI_MODE_MASTER;
	spi_lcd.Init.WorkMode = SPI_WORK_MODE;
	spi_lcd.Init.DataSize = SPI_DATA_WIDTH;
	spi_lcd.Init.SPI_CLK_DIV = SPI_CLKDIV;
	spi_lcd.Init.NSS = HAL_SPI_NSS_SOFT;

	HAL_GPIO_InitTypeDef spi_lcd_gpio;
	spi_lcd_gpio.Pin = SPI_MOSI_PIN;
	spi_lcd_gpio.Mode = GPIO_MODE_AF_PER;
	spi_lcd_gpio.Pull = GPIO_NOPULL;
	spi_lcd_gpio.Alternate = HAL_REMAP_SPI_MOSI;
	HAL_GPIO_Init(&spi_lcd_gpio);

	spi_lcd_gpio.Pin = SPI_SCLK_PIN;
	spi_lcd_gpio.Mode = GPIO_MODE_AF_PER;
	spi_lcd_gpio.Pull = GPIO_NOPULL;
	spi_lcd_gpio.Alternate = HAL_REMAP_SPI_SCLK;
	HAL_GPIO_Init(&spi_lcd_gpio);

	spi_lcd_gpio.Pin = SPI_NSS1_PIN;
	spi_lcd_gpio.Mode = GPIO_MODE_AF_PER;
	spi_lcd_gpio.Pull = GPIO_NOPULL;
	spi_lcd_gpio.Alternate = HAL_REMAP_SPI_NSS1;
	HAL_GPIO_Init(&spi_lcd_gpio);

	spi_lcd_gpio.Pin = LCD_RS_PIN;
	spi_lcd_gpio.Mode = GPIO_MODE_OUTPUT_PP;
	spi_lcd_gpio.Pull = GPIO_NOPULL;
	HAL_GPIO_Init(&spi_lcd_gpio);

	spi_lcd_gpio.Pin = LCD_RST_PIN;
	spi_lcd_gpio.Mode = GPIO_MODE_OUTPUT_PP;
	spi_lcd_gpio.Pull = GPIO_NOPULL;
	HAL_GPIO_Init(&spi_lcd_gpio);

	HAL_SPI_Init(&spi_lcd);
}

void lcd_spi_master_work_task(void)
{
	St7735_Init();

	while(1)
	{
		Disp_Color(BLACK);
		disp_32x32(44,50,(char *)xin,RED,BLACK);
		disp_32x32(44,80,(char *)yi,GREEN,BLACK);
		Disp_Color(RED);
		disp_32x32(44,50,(char *)xin,BLUE,RED);
		disp_32x32(44,80,(char *)yi,YELLOW,RED);
		Disp_Color(GREEN);
		disp_32x32(44,50,(char *)xin,PURPLE,GREEN);
		disp_32x32(44,80,(char *)yi,BLACK,GREEN);
		Disp_Color(BLUE);
		disp_32x32(44,50,(char *)xin,BLACK,BLUE);
		disp_32x32(44,80,(char *)yi,PURPLE,BLUE);
		Disp_Color(YELLOW);
		disp_32x32(44,50,(char *)xin,RED,YELLOW);
		disp_32x32(44,80,(char *)yi,GREEN,YELLOW);
		Disp_Color(PURPLE);
		disp_32x32(44,50,(char *)xin,YELLOW,PURPLE);
		disp_32x32(44,80,(char *)yi,BLUE,PURPLE);
		osDelay(1000);
	}
}

void lcd_spi_master_work_task_init(void)
{

	g_lcd_spi_master_TskHandle = osThreadNew ((osThreadFunc_t)(lcd_spi_master_work_task),NULL,"lcd_spi_master_work_task",SPI_MASTER_STACK_SIZE,osPriorityNormal);

}
#endif
