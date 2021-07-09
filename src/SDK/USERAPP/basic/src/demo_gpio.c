/** 
* @file        demo_gpio.c
*/
#include  "zos.h"

#ifdef BOARD_USING_GPIO_DEMO
//任务参数配置
#define GPIO_IN_TASK_PRIORITY    10
#define GPIO_IN_STACK_SIZE       500

//任务全局变量
osThreadId_t g_hal_gpio_in_TskHandle = NULL;


/************************************************
 * 
 * 中断测试函数
 * 
 * 
void gpio6_callback(void)
{
	zos_printf("this is pin 6 irq\r\n");
	zos_pin_irq_enable(PIN_6,PIN_IRQ_ENABLE);
}

void gpio7_callback(void)
{
	zos_printf("this is pin 7 irq\r\n");
	zos_pin_irq_enable(PIN_7,PIN_IRQ_ENABLE);
}

 void hal_gpio_int_work_task(void)
 {
	zos_printf("hal_gpio_int_work_task runing\r\n");
	zos_pin_attach_irq(PIN_6,PIN_IRQ_MODE_FALLING,gpio6_callback);
	zos_pin_attach_irq(PIN_7,PIN_IRQ_MODE_HIGH_LEVEL,gpio7_callback);
 	while(1)
 	{
 		osDelay(1000);
 	}
 }
*******************************************************/

/************************************************
 * 
 * GPIO输入测试
 * 
 * 
void hal_gpio_in_work_task(void)
{
	zos_pin_mode(PIN_7,PIN_MODE_INPUT);
	while(1)
	{
		uint8_t gpio_in_value = 0xff;

		gpio_in_value = zos_pin_read(PIN_7);
		if(gpio_in_value == PIN_HIGH)
		{
			zos_printf("pin 7 high\r\n");
		}
		else if(gpio_in_value == PIN_LOW)
		{
			zos_printf("pin 7 low\r\n");
		}
		
		//通过osDelay释放线程控制权
		osDelay(1000);	
	}
}
*******************************************************/


void hal_gpio_out_work_task(void)
{
	zos_pin_mode(PIN_9,PIN_MODE_OUTPUT);
	while(1)
	{
		//隔3s切换GPIO引脚的输出状态，用于调试
		
		zos_pin_write(PIN_9, PIN_LOW);
		zos_printf("pin 9 low\r\n");
		//通过zos_task_delay()释放线程控制权
		zos_task_delay(3000);

		zos_pin_write(PIN_9, PIN_HIGH);
		zos_printf("pin 9 high\r\n");
		//通过zos_task_delay()释放线程控制权
		zos_task_delay(3000);
	}
}

/**
 * @brief 任务线程
 */



/**
 * @brief 任务创建
 */
void hal_pin_in_test(void)
{
    g_hal_gpio_in_TskHandle = osThreadNew ((osThreadFunc_t)hal_gpio_out_work_task,NULL,"hal_gpio_out_work_task",GPIO_IN_STACK_SIZE,osPriorityNormal);
}


#endif //BOARD_USING_GPIO_DEMO
