/**
* @file		hal_i2c_slave_IT_demo.c
* @ingroup	peripheral
* @brief	I2C从设备中断接收工作模式Demo
* @warning	用户使用外设时需注意：1.使用前关闭standby，使用完开启standby 2.确保没有在NV中配置外设所使用的GPIO，否则芯片进出standby时会根据NV重配GPIO，导致外设无法正常使用
***********************************************************************************/

#if DEMO_TEST


#include "xy_api.h"

//任务参数配置
#define I2C_SCL_PIN        HAL_GPIO_PIN_NUM_6
#define I2C_SDA_PIN        HAL_GPIO_PIN_NUM_7

//任务全局变量
osThreadId_t g_i2c_slave_IT_test_TskHandle = NULL;
osSemaphoreId_t  g_i2c_slave_IT_test_sem = NULL;

HAL_I2C_HandleTypeDef i2c_slave_IT;

/**
 * @brief 错误中断回调函数。注意：1.用户在使用时需将__weak 删除！！！2.用户需保证中断及中断里调用的函数都在ram上！！！
 */
void HAL_I2C_ErrorCallback(HAL_I2C_HandleTypeDef *I2CHandleStruct) __RAM_FUNC;
__weak void HAL_I2C_ErrorCallback(HAL_I2C_HandleTypeDef *I2CHandleStruct)
{
	//用户根据实际需求添加错误处理代码

	//错误处理完后恢复ErrorCode
	I2CHandleStruct->ErrorCode = HAL_I2C_ERROR_NONE;
}

/**
 * @brief 中断回调函数。注意：1.用户在使用时需将__weak 删除！！！2.用户需保证中断及中断里调用的函数都在ram上！！！
 */
void HAL_I2C_RxCpltCallback(HAL_I2C_HandleTypeDef *I2CHandleStruct) __RAM_FUNC;
__weak void HAL_I2C_RxCpltCallback(HAL_I2C_HandleTypeDef *I2CHandleStruct)
{
	UNUSED(I2CHandleStruct);
	osSemaphoreRelease(g_i2c_slave_IT_test_sem);
}

/**
 * @brief 中断服务函数。注意：1.用户在使用时需将__weak 删除！！！2.用户需保证中断及中断里调用的函数都在ram上！！！
 */
void HAL_I2C1_IRQHandler(void) __RAM_FUNC;
__weak void HAL_I2C1_IRQHandler(void)
{
	I2C_IRQHandler(&i2c_slave_IT);
}

/**
 * @brief 初始化函数
 * @code
 *	HAL_GPIO_InitTypeDef i2c_slave_IT_gpio;

	//I2C参数配置
	i2c_slave_IT.Instance = HAL_I2C1;
	i2c_slave_IT.Init.AddressingMode = HAL_I2C_ADDRESS_7BITS;
	i2c_slave_IT.Init.ClockSpeed = HAL_I2C_CLK_SPEED_100K;
	i2c_slave_IT.Init.Mode = HAL_I2C_MODE_SLAVE;
	i2c_slave_IT.Init.OwnAddress = 0x24;

	//映射I2C的SCL引脚
	i2c_slave_IT_gpio.Pin = I2C_SCL_PIN;
	i2c_slave_IT_gpio.Mode = GPIO_MODE_AF_PER;
	i2c_slave_IT_gpio.Pull = GPIO_NOPULL;
	i2c_slave_IT_gpio.Alternate = HAL_REMAP_I2C1_SCLK;
	HAL_GPIO_Init(&i2c_slave_IT_gpio);

	//映射I2C的SDA引脚
	i2c_slave_IT_gpio.Pin = I2C_SDA_PIN;
	i2c_slave_IT_gpio.Mode = GPIO_MODE_AF_PER;
	i2c_slave_IT_gpio.Pull = GPIO_NOPULL;
	i2c_slave_IT_gpio.Alternate = HAL_REMAP_I2C1_SDA;
	HAL_GPIO_Init(&i2c_slave_IT_gpio);

	//初始化I2C
	if(HAL_I2C_Init(&i2c_slave_IT) != HAL_OK)
	{
		send_debug_str_to_at_uart("\r\nerror\r\n");
	}
	else
	{
		send_debug_str_to_at_uart("\r\nnormal\r\n");
	}
 * @endcode
 */
void i2c_slave_IT_test_init(void)
{
	//创建信号量
	g_i2c_slave_IT_test_sem = osSemaphoreNew(0xFFFF, 0);

	HAL_GPIO_InitTypeDef i2c_slave_IT_gpio;

	//I2C参数配置
	i2c_slave_IT.Instance = HAL_I2C1;
	i2c_slave_IT.Init.AddressingMode = HAL_I2C_ADDRESS_7BITS;
	i2c_slave_IT.Init.ClockSpeed = HAL_I2C_CLK_SPEED_100K;
	i2c_slave_IT.Init.Mode = HAL_I2C_MODE_SLAVE;
	i2c_slave_IT.Init.OwnAddress = 0x24;

	//映射I2C的SCL引脚
	i2c_slave_IT_gpio.Pin = I2C_SCL_PIN;
	i2c_slave_IT_gpio.Mode = GPIO_MODE_AF_PER;
	i2c_slave_IT_gpio.Pull = GPIO_NOPULL;
	i2c_slave_IT_gpio.Alternate = HAL_REMAP_I2C1_SCLK;
	HAL_GPIO_Init(&i2c_slave_IT_gpio);

	//映射I2C的SDA引脚
	i2c_slave_IT_gpio.Pin = I2C_SDA_PIN;
	i2c_slave_IT_gpio.Mode = GPIO_MODE_AF_PER;
	i2c_slave_IT_gpio.Pull = GPIO_NOPULL;
	i2c_slave_IT_gpio.Alternate = HAL_REMAP_I2C1_SDA;
	HAL_GPIO_Init(&i2c_slave_IT_gpio);

	//初始化I2C
	if(HAL_I2C_Init(&i2c_slave_IT) != HAL_OK)
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
void i2c_slave_IT_task(void)
{
	uint8_t data[5] = {'a', 'b', 'c', 'd', 0};
	while(1)
	{
		//中断接收4个字节，等待中断接收完成释放信号量
		HAL_I2C_Slave_Receive_IT(&i2c_slave_IT, data, 4);
		//等待获取信号量，释放线程控制权
		osSemaphoreAcquire(g_i2c_slave_IT_test_sem, osWaitForever);	
		//打印接收到的字符，用于调试
		send_debug_str_to_at_uart((char *)data);
		//发送接收到的4个字节，超时时间设置为HAL_MAX_DELAY，即一直等待到4个字节发送完成
		HAL_I2C_Slave_Transmit(&i2c_slave_IT, data, 4, HAL_MAX_DELAY);
	}
}

/**
 * @brief 任务创建
 *
 */
void i2c_slave_IT_task_init(void)
{
	g_i2c_slave_IT_test_TskHandle = osThreadNew((osThreadFunc_t)i2c_slave_IT_task, NULL, "i2c_slave_IT_task", 512, osPriorityNormal);
}


#endif
