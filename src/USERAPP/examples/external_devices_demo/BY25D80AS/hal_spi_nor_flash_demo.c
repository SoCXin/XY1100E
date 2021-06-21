/**
* @file        spi_nor_flash_demo.c
* @brief       This file is the peripheral spi flash test demo for BY25D80AS.
*			   经测试，2分频，hal库放flash的情况下，写1M大约需要18s；2分频，hal库放ram的情况下，写1M大约需要8s。
* @author      xinyi
*/
#if DEMO_TEST

#include "xy_api.h"

//spi flash 引脚及参数配置
#define SPI_MOSI_PIN          HAL_GPIO_PIN_NUM_6
#define SPI_MISO_PIN          HAL_GPIO_PIN_NUM_7
#define SPI_SCLK_PIN          HAL_GPIO_PIN_NUM_8
#define SPI_NSS1_PIN          HAL_GPIO_PIN_NUM_9

#define SPI_HOLD_PIN          HAL_GPIO_PIN_NUM_13
#define SPI_WP_PIN            HAL_GPIO_PIN_NUM_18

//spi flash 任务参数配置
#define SPI_FLASH_TASK_PRIORITY    10
#define SPI_FLASH_STACK_SIZE       1024

//spi flash 任务全局变量
osThreadId_t  g_spi_flash_TskHandle = NULL;
static HAL_SPI_HandleTypeDef spi_master;

//spi flash demo全局变量
uint8_t write_data;
uint8_t read_data;
uint8_t result_id[2];
uint32_t write_length = 1024;
uint8_t write_data_k[256] = {[0 ... 255] = 125};
static volatile uint8_t jtag_test = 0;


/**
 * @brief spi flash 工作函数调用的钩子函数
 *
 */
void user_hook_spi_flash_handler(void)
{
	//demo局部变量
	uint8_t readID_command[4] = {0x90, 0x00, 0x00, 0x00};
//	uint8_t erase_command = 0x20;
//	uint32_t erase_address = 0x000000;
	uint8_t write_command = 0x02;
	uint32_t write_address = 0x000000;
	uint8_t read_command = 0x03;
	uint32_t read_address = 0x000000;
	uint8_t write_enable = 0x06;
//	uint8_t write_disable = 0x04;
	uint8_t dummy[2] = {0xFF,0xFF};
	uint8_t tmp_address;
	uint8_t* write_address_k = write_data_k;
	uint8_t chip_erase_command = 0x60;

	while(!jtag_test);
	jtag_test--;


	/******************** 读取厂商ID ********************/
	HAL_SPI_ChipSelect(&spi_master, HAL_SPI_CS_PIN_1);
	//写命令
	HAL_SPI_Transmit(&spi_master,readID_command, 4,0);
	osDelay(100);
	HAL_SPI_Clean_RxFIFO(&spi_master);
	//读数据
	HAL_SPI_Transmit(&spi_master,dummy, 2,0);
	HAL_SPI_Receive(&spi_master, result_id, 2,0);
	HAL_SPI_ChipSelect(&spi_master, HAL_SPI_CS_NONE);

	xy_printf("result_id:%x %x\n", result_id[0], result_id[1]);

	/******************** 擦除chip ********************/
	//写使能命令
	HAL_SPI_ChipSelect(&spi_master, HAL_SPI_CS_PIN_1);
	HAL_SPI_Transmit(&spi_master,&write_enable, 1,0);
	osDelay(100);
	HAL_SPI_ChipSelect(&spi_master, HAL_SPI_CS_NONE);
	//写擦除命令
	HAL_SPI_ChipSelect(&spi_master, HAL_SPI_CS_PIN_1);
	HAL_SPI_Transmit(&spi_master,&chip_erase_command, 1,0);
	osDelay(100);
	HAL_SPI_ChipSelect(&spi_master, HAL_SPI_CS_NONE);

	xy_printf("chip erase success\n");

	osDelay(5000);//此处需加延时，否则前一部分数据写入错误

	/******************** 写数据 ********************/
	//从0x000000地址处开始，每次写256个byte
	write_address = 0x000000;
	xy_printf("write begin\n");
	for(uint32_t j = 0; j < 4*1024; j++)
	{
		//写使能命令
		HAL_SPI_ChipSelect(&spi_master, HAL_SPI_CS_PIN_1);
		HAL_SPI_Transmit(&spi_master,&write_enable, 1,0);
		osDelay(2);
		HAL_SPI_ChipSelect(&spi_master, HAL_SPI_CS_NONE);

		HAL_SPI_ChipSelect(&spi_master, HAL_SPI_CS_PIN_1);
		//写命令
		HAL_SPI_Transmit(&spi_master,&write_command, 1,0);
		//写地址
		for(uint8_t i = 3; i > 0; i--)
		{
			tmp_address = (write_address & (0xFF << (i-1)*8)) >> (i-1)*8;
			HAL_SPI_Transmit(&spi_master, &tmp_address, 1, 0);
		}
		HAL_SPI_Transmit(&spi_master ,write_address_k, 256, 0);
		HAL_SPI_ChipSelect(&spi_master, HAL_SPI_CS_NONE);

		write_address += 0x100;
	}
	xy_printf("write 1M flah success\n");

	osDelay(5000);//此处需加延时，否则前一部分数据读出错误

	/******************** 读数据 ********************/
	//从0x000000地址处开始，每隔0x100读出1个byte数据并打印，看数据是否写入成功
	read_address = 0x000000;
	for(uint32_t j = 0; j < 4*1024; j++)
	{
		HAL_SPI_ChipSelect(&spi_master, HAL_SPI_CS_PIN_1);
		//写命令
		HAL_SPI_Transmit(&spi_master, &read_command, 1, 0);
		//写地址
		for(uint8_t i = 3; i > 0; i--)
		{
			tmp_address = (read_address & (0xFF << (i-1)*8)) >> (i-1)*8;
			HAL_SPI_Transmit(&spi_master, &tmp_address, 1, 0);
		}
		osDelay(10);
		HAL_SPI_Clean_RxFIFO(&spi_master);
		//读数据
		HAL_SPI_Transmit(&spi_master, dummy, 1, 0);
		HAL_SPI_Receive(&spi_master, &read_data, 1, 0);
		HAL_SPI_ChipSelect(&spi_master, HAL_SPI_CS_NONE);

		xy_printf("%p:%d\n", read_address, read_data);
		read_address += 0x100;
	}
}



/**
 * @brief spi flash 初始化函数
 * @code
 *  spi_master.Instance = SPI;
	spi_master.Init.Mode = HAL_SPI_MODE_MASTER;
	spi_master.Init.WorkMode = HAL_SPI_WORKMODE_0;
	spi_master.Init.DataSize = HAL_SPI_DATASIZE_8BIT;
	spi_master.Init.SPI_CLK_DIV = HAL_SPI_CLKDIV_2;
	spi_master.Init.NSS = HAL_SPI_NSS_SOFT;

	HAL_GPIO_InitTypeDef spi_master_gpio;
	spi_master_gpio.Pin = HAL_GPIO_PIN_NUM_6;
	spi_master_gpio.Mode = GPIO_MODE_AF_PER;
	spi_master_gpio.Pull = GPIO_NOPULL;
	spi_master_gpio.Alternate = HAL_REMAP_SPI_MOSI;
	HAL_GPIO_Init(&spi_master_gpio);

	spi_master_gpio.Pin = HAL_GPIO_PIN_NUM_8;
	spi_master_gpio.Mode = GPIO_MODE_AF_PER;
	spi_master_gpio.Pull = GPIO_NOPULL;
	spi_master_gpio.Alternate = HAL_REMAP_SPI_SCLK;
	HAL_GPIO_Init(&spi_master_gpio);

	spi_master_gpio.Pin = HAL_GPIO_PIN_NUM_7;
	spi_master_gpio.Mode = GPIO_MODE_AF_PER;
	spi_master_gpio.Pull = GPIO_NOPULL;
	spi_master_gpio.Alternate = HAL_REMAP_SPI_MISO;
	HAL_GPIO_Init(&spi_master_gpio);

	spi_master_gpio.Pin = HAL_GPIO_PIN_NUM_9;
	spi_master_gpio.Mode = GPIO_MODE_AF_PER;
	spi_master_gpio.Pull = GPIO_NOPULL;
	spi_master_gpio.Alternate = HAL_REMAP_SPI_NSS1;
	HAL_GPIO_Init(&spi_master_gpio);

	spi_master_gpio.Pin = SPI_HOLD_PIN;
	spi_master_gpio.Mode = GPIO_MODE_GPIO_OUT_PP;
	spi_master_gpio.Pull = GPIO_NOPULL;
	HAL_GPIO_Init(&spi_master_gpio);

	spi_master_gpio.Pin = SPI_WP_PIN;
	spi_master_gpio.Mode = GPIO_MODE_GPIO_OUT_PP;
	spi_master_gpio.Pull = GPIO_NOPULL;
	HAL_GPIO_Init(&spi_master_gpio);

	HAL_GPIO_WritePin(SPI_HOLD_PIN, HAL_GPIO_PIN_SET);
	HAL_GPIO_WritePin(SPI_WP_PIN, HAL_GPIO_PIN_SET);

	if(SPI_Init(&spi_master) != HAL_OK)
	{
		send_debug_str_to_at_uart("\r\nerror\r\n");
	}
	else
	{
		send_debug_str_to_at_uart("\r\nnormal\r\n");
	}
 * @endcode
 * 
 */

void spi_flash_gpio_init(void)
{
	spi_master.Instance = HAL_SPI;
	spi_master.Init.Mode = HAL_SPI_MODE_MASTER;
	spi_master.Init.WorkMode = HAL_SPI_WORKMODE_0;
	spi_master.Init.DataSize = HAL_SPI_DATASIZE_8BIT;
	spi_master.Init.SPI_CLK_DIV = HAL_SPI_CLKDIV_2;
	spi_master.Init.NSS = HAL_SPI_NSS_SOFT;

	//6-8-7-9
	HAL_GPIO_InitTypeDef spi_master_gpio;
	spi_master_gpio.Pin = SPI_MOSI_PIN;
	spi_master_gpio.Mode = GPIO_MODE_AF_PER;
	spi_master_gpio.Pull = GPIO_NOPULL;
	spi_master_gpio.Alternate = HAL_REMAP_SPI_MOSI;
	HAL_GPIO_Init(&spi_master_gpio);

//	spi_master_gpio.Pin = HAL_GPIO_PIN_NUM_8;
//	spi_master_gpio.Mode = GPIO_MODE_AF_PER;
//	spi_master_gpio.Pull = GPIO_NOPULL;
//	spi_master_gpio.Alternate = HAL_REMAP_SPI_SCLK;
//	HAL_GPIO_Init(&spi_master_gpio);

	spi_master_gpio.Pin = SPI_MISO_PIN;
	spi_master_gpio.Mode = GPIO_MODE_AF_PER;
	spi_master_gpio.Pull = GPIO_NOPULL;
	spi_master_gpio.Alternate = HAL_REMAP_SPI_MISO;
	HAL_GPIO_Init(&spi_master_gpio);

	spi_master_gpio.Pin = SPI_SCLK_PIN;
	spi_master_gpio.Mode = GPIO_MODE_AF_PER;
	spi_master_gpio.Pull = GPIO_NOPULL;
	spi_master_gpio.Alternate = HAL_REMAP_SPI_SCLK;
	HAL_GPIO_Init(&spi_master_gpio);

	spi_master_gpio.Pin = SPI_NSS1_PIN;
	spi_master_gpio.Mode = GPIO_MODE_AF_PER;
	spi_master_gpio.Pull = GPIO_NOPULL;
	spi_master_gpio.Alternate = HAL_REMAP_SPI_NSS1;
	HAL_GPIO_Init(&spi_master_gpio);

	spi_master_gpio.Pin = SPI_HOLD_PIN;
	spi_master_gpio.Mode = GPIO_MODE_OUTPUT_PP;
	spi_master_gpio.Pull = GPIO_NOPULL;
	HAL_GPIO_Init(&spi_master_gpio);

	spi_master_gpio.Pin = SPI_WP_PIN;
	spi_master_gpio.Mode = GPIO_MODE_OUTPUT_PP;
	spi_master_gpio.Pull = GPIO_NOPULL;
	HAL_GPIO_Init(&spi_master_gpio);

	HAL_GPIO_WritePin(SPI_HOLD_PIN, HAL_GPIO_PIN_SET);
	HAL_GPIO_WritePin(SPI_WP_PIN, HAL_GPIO_PIN_SET);

	if(HAL_SPI_Init(&spi_master) != HAL_OK)
	{
		send_debug_str_to_at_uart("\r\nerror\r\n");
	}
	else
	{
		send_debug_str_to_at_uart("\r\nnormal\r\n");
	}
}


/**
 * @brief 任务线程
 *
 */
void spi_flash_work_task(void)
{
	while(1)
	{
		user_hook_spi_flash_handler();
	}
}


/**
 * @brief 任务创建
 * 
 */
void spi_flash_work_task_init(void)
{
	g_spi_flash_TskHandle = osThreadNew ((osThreadFunc_t)(spi_flash_work_task),NULL,"spi_flash_work_task",SPI_FLASH_STACK_SIZE,osPriorityNormal);
}
#endif
