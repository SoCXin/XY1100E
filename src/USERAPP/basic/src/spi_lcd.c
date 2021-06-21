#include "st7735.h"
#include "spi_lcd.h"
#include "font.h"
#include "zos.h"

#ifdef BOARD_USING_SPI_LCD

osThreadId_t  g_lcd_spi_master_TskHandle =NULL;
HAL_SPI_HandleTypeDef spi_lcd;

static void Lcd_Spi_Send(unsigned char *data,unsigned int size)
{
	unsigned int i=0;

	for(i=0;i<size;i++)
	{
		zos_spi_write(ZOS_SPI_BUS_LCD,data,size);
	}
}

void Lcd_Write_Cmd(unsigned char cmd)
{
	zos_pin_write(LCD_RS_PIN,PIN_LOW);
	Lcd_Spi_Send(&cmd,1);
}

void Lcd_Write_Data(unsigned char data)
{
	zos_pin_write(LCD_RS_PIN,PIN_HIGH);
	Lcd_Spi_Send(&data,1);
}

void Lcd_Write_Data16(unsigned int data)
{
	unsigned char dat16[2];

	dat16[0] = data >> 8;

	dat16[1] = data;

	zos_pin_write(LCD_RS_PIN,PIN_HIGH);
	Lcd_Spi_Send(dat16,2);
}

void lcd_spi_master_gpio_init(void)
{
	ZOS_SPI_CONFIG spi_cfg;

	zos_pin_mode(LCD_RS_PIN,PIN_MODE_OUTPUT);
	zos_pin_mode(LCD_RS_PIN,LCD_RST_PIN);

	spi_cfg.clk_div=ZOS_SPI_CLK_DIV2;
	spi_cfg.clk_mode=ZOS_SPI_CLK_MODE3;
	zos_spi_init(ZOS_SPI_BUS_LCD,&spi_cfg);

}

void lcd_spi_master_work_task(void)
{
	lcd_spi_master_gpio_init();
	St7735_Init();

	while(1)
	{
		zos_printf("disp color is BLACK\r\n");
		Disp_Color(BLACK);
		zos_printf("disp color is RED\r\n");
		Disp_Color(RED);
		zos_printf("disp color is GREEN\r\n");
		Disp_Color(GREEN);
		zos_printf("disp color is BLUE\r\n");
		Disp_Color(BLUE);
		zos_printf("disp color is YELLOW\r\n");
		Disp_Color(YELLOW);
		zos_printf("disp color is PURPLE\r\n");
		Disp_Color(PURPLE);
		osDelay(1000);
	}
}

void lcd_spi_master_work_task_init(void)
{

	g_lcd_spi_master_TskHandle = osThreadNew ((osThreadFunc_t)(lcd_spi_master_work_task),NULL,"lcd_spi_master_work_task",SPI_MASTER_STACK_SIZE,osPriorityNormal);

}

#endif
