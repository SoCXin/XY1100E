/**
* @file        cmos_spi_recv_demo.c
* @brief       这个文件是摄像头GC6153数据接收示例文件
*/
#include "cmos_recv_demo.h"
#include "dma.h"
#include "xy_utils.h"
#include "xy_api.h"
#include "at_uart.h"

#define I2C_SCK_PIN           HAL_GPIO_PIN_NUM_18
#define I2C_SDA_PIN           HAL_GPIO_PIN_NUM_19

#define SPI_MOSI_PIN          HAL_GPIO_PIN_NUM_8
#define SPI_SCLK_PIN          HAL_GPIO_PIN_NUM_7
#define SPI_CS_PIN            HAL_GPIO_PIN_NUM_9

#define CMOS_PWDN             HAL_GPIO_PIN_NUM_20
#define CMOS_RST              HAL_GPIO_PIN_NUM_21
#define CMOS_CLK              HAL_GPIO_PIN_NUM_13
#define CMOS_PWER             HAL_GPIO_PIN_NUM_6


#define SPI_SLAVE_TASK_PRIORITY    10
#define SPI_SLAVE_STACK_SIZE       1024

/**@struct SPI_ConfigTypeDef
* @brief spi initial options \n
*/
typedef struct{
	HAL_SPI_ClkDiv_TypeDef spi_clk_div; ///< spi clock dive
	HAL_SPI_Work_ModeTypeDef spi_work_mode; ///< work mode
	HAL_SPI_Data_SizeTypeDef spi_data_width; ///< data width
    uint32_t tx_thread; ///< interrupt thread  Can not be greater than 128
    uint32_t rx_thread; ///< interrupt thread  Can not be greater than 128
}SPI_ConfigTypeDef;

//This semiphore is used in the interrupt.
static osSemaphoreId_t g_spi_slave_sem = NULL;
static osThreadId_t  g_spi_slave_TskHandle = NULL;

typedef enum
{
	Frame_Start,
	Line_Start,
	Sync,
	Last_Line,
	Frame_Stop

}Receive_Status;


typedef union
{
	uint32_t data32[16];
	uint8_t  data8[64];

}Temp_Buffer;

uint8_t* recvBuffer = (uint8_t*)0x20047000;

Temp_Buffer tempBuffer;

//uint8_t* cmosBuffer =  0x20047000;

//__attribute__((aligned(32))) uint8_t recvBuffer[640] = {0};

//__attribute__((aligned(32))) uint8_t picBuffer[640] = {0};

//volatile int32_t recv_count = 0;
//volatile int32_t inner_count = 0;

volatile Receive_Status receive_status = Frame_Stop;
volatile uint8_t pre_value = 0;
volatile uint8_t ff_count = 0;
volatile int32_t recv_bytes_count = 0;
volatile int32_t line_num = 0;
volatile int32_t rem_flag = 0;
volatile intmax_t pack_count = 0;
HAL_SPI_HandleTypeDef spi_cmos;
HAL_TIM_HandleTypeDef tim_cmos;
HAL_I2C_HandleTypeDef i2c_cmos;
uint8_t  readdata[5];

//void SPI_CpltCallback(void) __attribute__((section(".ramtext")));

//CMOS 接收中断函数
__weak void HAL_SPI_IRQHandler(void)
{
	pack_count++;
	HAL_SPI_Receive(&spi_cmos,(uint8_t *)tempBuffer.data32,64,0);

	for(int i = 0; i < 64; i++)
	{
		switch(receive_status)
		{
			case Frame_Stop:
				if(tempBuffer.data8[i] == 0xff)
				{
					if(pre_value == 0xff)
					{
						ff_count++;
					}
					pre_value = tempBuffer.data8[i];
				}

				else
				{
					if(ff_count >= 2)
					{
						if(tempBuffer.data8[i] == 0x01)
						{
							ff_count = 0;
							receive_status = Frame_Start;
						}
					}
				}
				break;

			case Frame_Start:
				if(tempBuffer.data8[i] == 0xff)
				{
					if(pre_value == 0xff)
					{
						ff_count++;
					}
					pre_value = tempBuffer.data8[i];
				}
				else
				{
					if(ff_count >= 2)
					{
						if(tempBuffer.data8[i] == 0x02)
						{
							ff_count = 0;
							receive_status = Line_Start;
						}
					}
				}
				break;

			case Line_Start:
				if(tempBuffer.data8[i] == 0xff)
				{
					if(pre_value == 0xff)
					{
						ff_count++;
					}
					pre_value = tempBuffer.data8[i];
				}
				else
				{
					if(ff_count >= 2)
					{
						if(tempBuffer.data8[i] == 0x40)
						{
							ff_count = 0;
							receive_status = Sync;

							if(i % 2 == 0)
							{
								rem_flag = 0;
							}
							else
							{
								rem_flag = 1;
							}
						}
						if(tempBuffer.data8[i] == 0x00)
						{
							ff_count = 0;
							receive_status = Last_Line;

							if(i % 2 == 0)
							{
								rem_flag = 0;
							}
							else
							{
								rem_flag = 1;
							}
						}
					}
				}
				break;

			case Sync:
				if(i % 2 == rem_flag)
				{
					*(recvBuffer + line_num * 240 + recv_bytes_count) = tempBuffer.data8[i];
					recv_bytes_count++;

					if(recv_bytes_count >= 240)
					{
						line_num++;

						recv_bytes_count = 0;
						receive_status = Frame_Start;

//						if(line_num >= 320)
//						{
//							line_num = 0;
//							recv_bytes_count = 0;
//							receive_status = Frame_Stop;
//							xy_SPI_IntCmd(SPI1, SPI_INTTYPE_RX_FIFO_NE, DISABLE);
//						    xy_Semaphore_Give(&g_spi_slave_sem);
//						}

					}
				}
				break;

			case Last_Line:
				if(i % 2 == rem_flag)
				{
					*(recvBuffer + line_num * 240 + recv_bytes_count) = tempBuffer.data8[i];
					recv_bytes_count++;

					if(recv_bytes_count >= 240)
					{
						line_num = 0;
						recv_bytes_count = 0;
						receive_status = Frame_Stop;
						HAL_SPI_DISABLE_IT(&spi_cmos,HAL_SPI_IT_RX_NOT_EMPTY);
						osSemaphoreRelease(g_spi_slave_sem);
					}
				}
				break;
			default:
				break;

		}
	}

}

//CMOS 接收原始数据中断函数
//void SPI_CpltCallback(void)
//{
//	xy_SPI_Receive_32(SPI1, (uint32_t*)(recvBuffer + recv_count * 64), 64);
//
//	recv_count++;
//	if(recv_count >= 1300)
//	{
//		recv_count = 0;
//		xy_SPI_IntCmd(SPI1, SPI_INTTYPE_RX_FIFO_NE, DISABLE);
//		xy_Semaphore_Give(&g_spi_slave_sem);
//	}
//}

//CMOS DMA传输函数
//void SPI_CpltCallback(void)
//{
//	xy_SPI_Receive_32(SPI1, (uint32_t*)(recvBuffer + inner_count * 64), 64);
//
//	inner_count++;
//
//	if(inner_count >=10)
//	{
//		inner_count = 0;
//		DMAChannelTransfer(cmosBuffer + recv_count * 640, recvBuffer, 640, DMA_CHANNEL_1);
//		recv_count++;
//	}
//
//	if(recv_count >= 130)
//	{
//		recv_count = 0;
//		xy_SPI_IntCmd(SPI1, SPI_INTTYPE_RX_FIFO_NE, DISABLE);
//		xy_Semaphore_Give(&g_spi_slave_sem);
//	}
//}

// Other Possible Functions
void Demo_Function()
{
    uint8_t send_buffer[2] = {0xAA, 0xBB};
    HAL_SPI_Transmit(&spi_cmos,(void *)send_buffer, sizeof(send_buffer),0);
}


/**
  * @brief Initial I2C device
  * @param[in]  None
  * @retval None
  */
int32_t CMOS_I2C_Init(void)
{
	HAL_GPIO_InitTypeDef i2c_cmos_gpio;

	i2c_cmos_gpio.Pin = I2C_SCK_PIN;
	i2c_cmos_gpio.Mode = GPIO_MODE_AF_PER;
	i2c_cmos_gpio.Pull = GPIO_NOPULL;
	i2c_cmos_gpio.Alternate = HAL_REMAP_I2C1_SCLK;
	HAL_GPIO_Init(&i2c_cmos_gpio);
	
	i2c_cmos_gpio.Pin = I2C_SDA_PIN;
	i2c_cmos_gpio.Mode = GPIO_MODE_AF_PER;
	i2c_cmos_gpio.Pull = GPIO_NOPULL;
	i2c_cmos_gpio.Alternate = HAL_REMAP_I2C1_SDA;
	HAL_GPIO_Init(&i2c_cmos_gpio);
	
	i2c_cmos.Instance = HAL_I2C1;
	i2c_cmos.Init.AddressingMode = HAL_I2C_ADDRESS_7BITS;
	i2c_cmos.Init.ClockSpeed = HAL_I2C_CLK_SPEED_100K;
	i2c_cmos.Init.Mode = HAL_I2C_MODE_MASTER;

	HAL_I2C_Init(&i2c_cmos);

    return 0;
}

/**
  * @brief Initial PWM device
  * @note  This fuction can block the process note!!!!! User must put this functin in user_sys_init()
  * @param[in]  None
  * @retval None
  */
void CMOS_PWM_Init(void)
{
	HAL_GPIO_InitTypeDef cmos_gpio;

	cmos_gpio.Pin = CMOS_CLK;
	cmos_gpio.Mode = GPIO_MODE_AF_PP;
	cmos_gpio.Pull = GPIO_NOPULL;
	cmos_gpio.Alternate = HAL_REMAP_TMR1_AF_OUTP;
	HAL_GPIO_Init(&cmos_gpio);

	//uint32_t temp2 = 1;

	//HWREG((uint32_t)(0xA0140000 + 0x90)) &= ~(temp2 << 18);
	tim_cmos.Instance				=	HAL_TIM1;
	tim_cmos.Init.Mode				=	HAL_TIM_MODE_PWM_SINGLE;
	tim_cmos.Init.Reload			=	4;
	tim_cmos.Init.ClockDivision	    =	HAL_TIM_CLK_DIV_1;
	tim_cmos.Init.PWMReg            =   4 / 2;
	tim_cmos.Init.TIMPolarity		=	HAL_TIM_Polarity_Set;

	HAL_TIM_Init(&tim_cmos);

	HAL_TIM_Start(&tim_cmos);

}


/**
  * @brief Initial SPI device  
  * @note  This fuction can block the process note!!!!! User must put this functin in user_sys_init()
  * @param[in]  None
  * @retval None
  */
int32_t CMOS_SPI_Init(void)
{
    uint32_t ret = 0;
    SPI_ConfigTypeDef spi_config;
    HAL_GPIO_InitTypeDef spi_cmos_gpio;

    spi_cmos.Instance = HAL_SPI;
    spi_cmos.Init.Mode = HAL_SPI_MODE_SLAVE;
    spi_cmos.Init.WorkMode = HAL_SPI_WORKMODE_0;
    spi_cmos.Init.DataSize = HAL_SPI_DATASIZE_8BIT;
    spi_config.tx_thread = 64;
    spi_config.rx_thread = 64;

    spi_cmos_gpio.Pin = SPI_MOSI_PIN;
    spi_cmos_gpio.Mode = GPIO_MODE_AF_PER;
    spi_cmos_gpio.Pull = GPIO_NOPULL;
    spi_cmos_gpio.Alternate = HAL_REMAP_SPI_MOSI;
	HAL_GPIO_Init(&spi_cmos_gpio);

	spi_cmos_gpio.Pin = SPI_SCLK_PIN;
	spi_cmos_gpio.Mode = GPIO_MODE_AF_PER;
	spi_cmos_gpio.Pull = GPIO_NOPULL;
	spi_cmos_gpio.Alternate = HAL_REMAP_SPI_SCLK;
	HAL_GPIO_Init(&spi_cmos_gpio);

	spi_cmos_gpio.Pin = SPI_CS_PIN;
	spi_cmos_gpio.Mode = GPIO_MODE_AF_PER;
	spi_cmos_gpio.Pull = GPIO_NOPULL;
	spi_cmos_gpio.Alternate = HAL_REMAP_SPI_CS;
	HAL_GPIO_Init(&spi_cmos_gpio);

    //initiate spi device
	HAL_SPI_Init(&spi_cmos);

    //register interrupt
	HAL_SPI_Receive_IT(&spi_cmos,readdata,5);

    //set fifo thread for interrupt 
	HAL_SPI_SET_TXFIFO_THD(&spi_cmos,spi_config.tx_thread);
	HAL_SPI_SET_RXFIFO_THD(&spi_cmos,spi_config.rx_thread);

    //the function will block the process if it is failed

	g_spi_slave_sem = osSemaphoreNew(0xFFFF, 0);

    //dispose resources
    if(g_spi_slave_sem == NULL)
    {
    	HAL_SPI_DISABLE(&spi_cmos);
    	HAL_NVIC_IntUnregister(HAL_SPI1_IRQn);

    	HAL_GPIO_DeInit(SPI_MOSI_PIN);
    	HAL_GPIO_DeInit(SPI_SCLK_PIN);
    	HAL_GPIO_DeInit(SPI_CS_PIN);

        return -1;
    }

     //open interrupt 
    HAL_SPI_ENABLE_IT(&spi_cmos,HAL_SPI_IT_RX_NOT_EMPTY);
	
    //open device
    HAL_SPI_ENABLE(&spi_cmos);

    return ret;
}


/**
  * @brief Initial GPIO device
  * @note  This fuction can block the process note!!!!! User must put this functin in user_sys_init()
  * @param[in]  None
  * @retval None
  */

int32_t CMOS_GPIO_Init(void)
{
	HAL_GPIO_InitTypeDef cmos_gpio;

	cmos_gpio.Pin = CMOS_PWDN;
	cmos_gpio.Mode = GPIO_MODE_OUTPUT_PP;
	cmos_gpio.Pull = GPIO_NOPULL;
	HAL_GPIO_Init(&cmos_gpio);

	cmos_gpio.Pin = CMOS_RST;
	cmos_gpio.Mode = GPIO_MODE_OUTPUT_PP;
	cmos_gpio.Pull = GPIO_NOPULL;
	HAL_GPIO_Init(&cmos_gpio);

	cmos_gpio.Pin = CMOS_PWER;
	cmos_gpio.Mode = GPIO_MODE_OUTPUT_PP;
	cmos_gpio.Pull = GPIO_NOPULL;
	HAL_GPIO_Init(&cmos_gpio);

	HAL_GPIO_WritePin(CMOS_PWDN,HAL_GPIO_PIN_RESET);
	HAL_GPIO_WritePin(CMOS_RST,HAL_GPIO_PIN_RESET);
	HAL_GPIO_WritePin(CMOS_RST,HAL_GPIO_PIN_SET);

	return 0;
}

void CMOS_Clock_Set(bool option)
{
    if (option == false)
    {
    	HAL_TIM_Stop(&tim_cmos);
    }
    else
    {
    	HAL_TIM_Start(&tim_cmos);
    }
}

 uint8_t write_buffer[2] = {0};
 void CMOS_Write_Reg(uint8_t reg_address, uint8_t data)
 {
     write_buffer[0] = reg_address;
     write_buffer[1] = data;

     HAL_I2C_Master_Transmit(&i2c_cmos,0x40,write_buffer,2,0);
 }

 volatile uint32_t temp1, temp2;

 uint8_t CMOS_Read_Reg(uint8_t reg_address)
 {
	 temp1 = HAL_I2C_Master_Transmit(&i2c_cmos,0x40,&reg_address, 1,0);
	 uint8_t temp = 0;
	 temp2 = HAL_I2C_Master_Receive(&i2c_cmos,0x40,&temp, 1,0);
	 return temp;
 }

void CMOS_Rst_Set(int i)
{
	if(i == 0)
	{
		HAL_GPIO_WritePin(CMOS_RST,HAL_GPIO_PIN_RESET);
	}
	else
	{
		HAL_GPIO_WritePin(CMOS_RST,HAL_GPIO_PIN_SET);
	}
}

void CMOS_Pwn_Set(int i)
{
	if(i == 0)
	{
		HAL_GPIO_WritePin(CMOS_PWDN,HAL_GPIO_PIN_RESET);
	}
	else
	{
		HAL_GPIO_WritePin(CMOS_PWDN,HAL_GPIO_PIN_SET);
	}
}

void CMOS_DMA_Set(void)
{
	DMAChannelControlSet((unsigned long)DMA_CHANNEL_1,
			DMAC_CTRL_DINC_SET | DMAC_CTRL_SINC_SET | DMAC_CTRL_TC_SET
			| DMAC_CTRL_TYPE_MEM_TO_MEM | DMAC_CTRL_BURST_SIZE_16W | DMAC_CTRL_WORD_SIZE_32b);
}

GC6153_SERIALSensor cmos_sensor;
volatile int temp = 0;

/**
  * @brief  CMOS recieve task, The user can transplant algorithms here
  * @param[in]  None
  * @retval None
  */

int32_t camera_id = 0;
uint8_t reg_data = 0;

uint8_t reg_check[180] = {0};

void CMOS_RECV_TASK(void)
{
    HWREGB(0xA011003A) = 0x60;
    HWREGB(0xA011003B) &= 0xfe;

    xy_printf("[SPI_CMOS_DEMO]spi_task_working...");

    cmos_sensor.I2C_Write_Reg = CMOS_Write_Reg;
    cmos_sensor.I2C_Read_Reg = CMOS_Read_Reg;
    cmos_sensor.Rst_Set = CMOS_Rst_Set;
    cmos_sensor.Pwn_Set = CMOS_Pwn_Set;
    cmos_sensor.Clk_Set = CMOS_Clock_Set;

    while(temp == 0);

    camera_id = GC6153_Power_On(&cmos_sensor);

    if(camera_id == 0x6153)
    {
    	GC6153_Initial_Setting(&cmos_sensor);
    }


	while(1)
	{
		if( osOK == osSemaphoreAcquire(g_spi_slave_sem, osWaitForever) )
		{

			cmos_sensor.I2C_Write_Reg(0xfe, 0x02);
			reg_data = cmos_sensor.I2C_Read_Reg(0x28);
			cmos_sensor.I2C_Write_Reg(0xfe, 0x00);

			extern osMutexId_t g_farps_write_m;
			osMutexAcquire(g_farps_write_m, osWaitForever);
			at_write_to_uart((char *)recvBuffer, 76800);
			osMutexRelease(g_farps_write_m);

//			for(int i = 0; i < 130; i++)
//			{
//				DMAChannelTransfer(picBuffer, cmosBuffer + i * 640, 640, DMA_CHANNEL_1);
//				at_write_to_uart(picBuffer, 640);
//			}
		}
	}
}

/**
  * @brief  Initial the cmos receive task 
  * @note User must put this functin in user_app_init()
  * @param[in]  None
  * @retval task create result
  */
void CMOS_RECV_TASK_INIT(void)
{
	g_spi_slave_TskHandle = osThreadNew ((osThreadFunc_t)(CMOS_RECV_TASK),NULL,"coms_recv_task",SPI_SLAVE_STACK_SIZE,osPriorityHigh);
}



