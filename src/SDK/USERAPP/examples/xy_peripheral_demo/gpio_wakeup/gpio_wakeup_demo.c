/*
 * @file      gpio_wakeup_demo.c
 * @ingroup   peripheral
 * @brief     用于唤醒standby的唤醒源共有三个，分别是wakeupsrc0/wakeupsrc1/wakeupsrc2，对应每个唤醒都可以配置一个gpio作为外部唤醒源
 * @warning   其中gpio10/11/12不可作为外部唤醒源，不使用的唤醒源需将其gpio配置成GPIO_WAKEUP_UNUSE
 * 			  !!!唤醒触发方式为低电平唤醒，低电平时间不得小于100us，最好不要超过1ms,用户需要自行通过硬件来保证!!!
 *            该功能默认关闭，需要用时请打开gpio_wakeup.h中的GPIO_WAKEUP宏开关
 */
#include "gpio_wakeup.h"
#if DEMO_TEST
#include "xy_api.h"

#define GPIO_WAKEUP_PIN         HAL_GPIO_PIN_NUM_7           //唤醒源0的gpio，gpio10/11/12不可用
#define GPIO_WAKEUP_PIN_UNUSE   GPIO_WAKEUP_UNUSE            //不用的唤醒源需将gpio配置成GPIO_WAKEUP_UNUSE

osThreadId_t  g_gpio_wakeup_TskHandle = NULL;//线程句柄

/*****************************************************************************************/
/**
  * @func   void gpio_wakeup_init_demo(void)
  * @brief  初始化函数：初始化唤醒源的gpio，用户配置唤醒源的gpio，创建任务队列
  * @param  none
  * @retval none
  * @warning 不用的唤醒源需将gpio配置成GPIO_WAKEUP_UNUSE
  */
/*****************************************************************************************/
void gpio_wakeup_init_demo(void)
{
	WAKEUPSrc_gpio WAKEUPSrc_GPIO;
	WAKEUPSrc_GPIO.wakeupsrc0_gpio = GPIO_WAKEUP_PIN;
	WAKEUPSrc_GPIO.wakeupsrc1_gpio = GPIO_WAKEUP_PIN_UNUSE;
	WAKEUPSrc_GPIO.wakeupsrc2_gpio = GPIO_WAKEUP_PIN_UNUSE;
	xy_gpio_wakeup_config(WAKEUPSrc_GPIO);
	gpio_wakeup_msg_q = osMessageQueueNew(20, sizeof(uint8_t));
}

/*****************************************************************************************/
/**
  * @func   void user_hook_standby_wakeup(void)
  * @brief  用户处理函数：非唤醒源0/1/2唤醒
  * @param  none
  * @retval none
  */
/*****************************************************************************************/
void user_hook_standby_wakeup(void)
{
	send_debug_str_to_at_uart("\r\nwakeup from standby\r\n");
	send_debug_str_to_at_uart("\r\ngpio is not the wakeup source!\r\n");
}

/*****************************************************************************************/
/**
  * @func   void user_hook_standby_wakeup_bysrc0(void)
  * @brief  用户处理函数：唤醒源0唤醒standby
  * @param  none
  * @retval none
  */
/*****************************************************************************************/
void user_hook_standby_wakeup_bysrc0(void)
{
	send_debug_str_to_at_uart("\r\nwakeup from standby\r\n");
	send_debug_str_to_at_uart("\r\nwakeup pin:GPIO_PIN_NUM_7\r\n");
}

/*****************************************************************************************/
/**
  * @func   void user_hook_standby_wakeup_bysrc1(void)
  * @brief  用户处理函数：唤醒源1唤醒standby
  * @param  none
  * @retval none
  */
/*****************************************************************************************/
void user_hook_standby_wakeup_bysrc1(void)
{
	send_debug_str_to_at_uart("\r\nwakeup from standby\r\n");
	send_debug_str_to_at_uart("\r\nwakeup pin:GPIO_PIN_NUM_8\r\n");
}

/*****************************************************************************************/
/**
  * @func   void user_hook_standby_wakeup_bysrc2(void)
  * @brief  用户处理函数：唤醒源2唤醒standby
  * @param  none
  * @retval none
  */
/*****************************************************************************************/
void user_hook_standby_wakeup_bysrc2(void)
{
	send_debug_str_to_at_uart("\r\nwakeup from standby\r\n");
	send_debug_str_to_at_uart("\r\nwakeup pin:GPIO_PIN_NUM_9\r\n");
}

/*****************************************************************************************/
/**
  * @brief  用户处理线程
  * @param  none
  * @retval none
  */
/*****************************************************************************************/
void xy_gpio_wakeup_from_standby(void)
{
	uint8_t rcv_msg = 0;

	while(1)
	{
		osMessageQueueGet(gpio_wakeup_msg_q, (void *)(&rcv_msg), NULL, osWaitForever);

		switch(rcv_msg)
		{
			case 0:user_hook_standby_wakeup();break;
			case 1:user_hook_standby_wakeup_bysrc0();break;
			case 2:user_hook_standby_wakeup_bysrc1();break;
			case 3:user_hook_standby_wakeup_bysrc0();user_hook_standby_wakeup_bysrc1();break;
			case 4:user_hook_standby_wakeup_bysrc2();break;
			case 5:user_hook_standby_wakeup_bysrc0();user_hook_standby_wakeup_bysrc2();break;
			case 6:user_hook_standby_wakeup_bysrc1();user_hook_standby_wakeup_bysrc2();break;
			case 7:user_hook_standby_wakeup_bysrc0();user_hook_standby_wakeup_bysrc1();user_hook_standby_wakeup_bysrc2();break;
			default:user_hook_standby_wakeup();break;
		}
	}
}

/*****************************************************************************************/
/**
  * @func   void gpio_wakeup_standby_task_init_demo(void)
  * @brief  初始化线程
  * @param  none
  * @retval none
  */
/*****************************************************************************************/
void gpio_wakeup_standby_task_init_demo(void)
{
	g_gpio_wakeup_TskHandle = osThreadNew ((osThreadFunc_t)(xy_gpio_wakeup_from_standby),NULL,"xy_gpio_wakeup_from_standby",512,osPriorityNormal);
}
#endif
