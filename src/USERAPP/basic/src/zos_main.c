/** 
* @file        zos_main.c
* @ingroup     zos_main
* @brief       主入口，用户在对应的接口中进行外设和私有应用任务的初始化，但不得长时间阻塞！
*/
/**
 * @defgroup zos main
 */




#include "zos.h"

//任务全局变量
osThreadId_t user_main_task_Handle = NULL;
#define USER_MAIN_STACK_SIZE       1024


void user_main_task(void)
{
	zos_pin_mode(PIN_13,PIN_MODE_OUTPUT);
	while(1)
	{
		//隔3s切换GPIO引脚的输出状态，用于调试
		
		zos_pin_write(PIN_13, PIN_LOW);
		zos_printf("pin(%d) low\r\n",PIN_13);
		//通过osDelay释放线程控制权
		osDelay(3000);

		zos_pin_write(PIN_13, PIN_HIGH);
		zos_printf("pin(%d) high\r\n",PIN_13);
		//通过osDelay释放线程控制权
		osDelay(3000);
	}
}

int zos_main(void)
{
  user_main_task_Handle = osThreadNew ((osThreadFunc_t)user_main_task,NULL,"user_main_task",USER_MAIN_STACK_SIZE,osPriorityNormal);
}
