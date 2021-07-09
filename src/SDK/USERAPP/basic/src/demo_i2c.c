#include  "zos.h"

#ifdef BOARD_USING_I2C_DEMO


//任务全局变量
osThreadId_t  g_i2c_master_test_TskHandle = NULL;

/**
 * @brief 任务线程
 */
void i2c_master_test_task(void)
{
	zos_uint8_t txdata[8] = {'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H'};
	zos_uint8_t rxdata[9] = {};
	zos_uint8_t reg;
	zos_uint16_t devaddress = 0x23;
	ZOS_I2C_CONFIG i2c_cfg;
	zos_uint8_t mode;
	i2c_cfg.mode=ZOS_I2C_MODE_MASTER;
	i2c_cfg.speed=ZOS_I2C_SPEED_100K;
	mode=zos_i2c_init(ZOS_I2C_BUS_0,&i2c_cfg);
	while(1)
	{
		if(mode == ZOS_EOK)
		{
			reg=0x01;
			zos_i2c_write(ZOS_I2C_BUS_0,devaddress,&reg,1);
			reg=0x07;
			zos_i2c_write(ZOS_I2C_BUS_0,devaddress,&reg, 1);
			reg=0x11;
			zos_i2c_write(ZOS_I2C_BUS_0,devaddress,&reg, 1);
			zos_i2c_read(ZOS_I2C_BUS_0,devaddress,rxdata,2);
			zos_printf("light is %d   %d\r\n",rxdata[0],rxdata[1]);

		}
		else
			zos_printf("I2C_INIT ERROR\r\n");
		//通过osDelay释放线程控制权
		osDelay(1000);
	}
}

/**
 * @brief 任务创建
 */
void i2c_master_task_init(void)
{
    g_i2c_master_test_TskHandle = osThreadNew((osThreadFunc_t)i2c_master_test_task, NULL, "i2c_master_test_task", 512, osPriorityNormal);
}


#endif //DEMO_TEST
