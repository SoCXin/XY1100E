/** 
* @file        user_task_demo2.c
* @ingroup     opencpu
* @brief       长供电场景下，用户周期性RTC数据传输的参考代码。
*用户通过修改USER_RTC_OFFSET2宏值来定义周期性时长，在user_process中实现用户数据的处理，如云通信等。
*为了保证芯片在任何状态下都能正常处理RTC事件，请不要修改参考代码的主框架。
* @attention   必须设置g_softap_fac_nv->deepsleep_enable==0 && g_softap_fac_nv->work_mode==0! \n
*  为了防止软硬件异常、无线环境异常等造成功耗过高，用户必须设置好user_dog_time、hard_dog_time两个软硬看门狗时长！
* @note         !!!该源文件默认不编译，用户不得直接在该文件中修改！！！\n
*               而应该是自行创建新的私有的源文件，将demo示例代码拷贝过去后再做修改。
*/

#include "xy_api.h"

/*User code demo of long power supply scenario.g_softap_fac_nv->deepsleep_enable must  set  0*/
/*For abnormal running due to user code defects,propose user to set a watchdog timer,and if timeout, do reboot!*/

#if DEMO_TEST

/**
 * @brief user task period lenth,user must vary it by self production
 */	
#define   USER_RTC_OFFSET2   (40+osKernelGetTickCount()%60)


osThreadId_t  g_user_task_Handle2 = NULL;
osSemaphoreId_t user_rtc_sem2 = NULL;



#if 0
/**
 * @brief       用户根据自己的产品特点，完成软看门狗异常的容错方案
 * @attention   用户也可以不使用芯翼平台的软看门狗机制，自行设计容错方案，但不管采用哪种策略，必须保护到位
 */	
void  user_dog_timeout_hook()
{
	xy_reboot();

}
#endif


/**
 * @brief 用户周期性RTC超时回调接口，不建议用户修改
 */	
static void user_rtc_timeout_cb(void *para)
{
	(void) para;

	if(user_rtc_sem2 == NULL)
	{
		user_rtc_sem2 = osSemaphoreNew(0xFFFF, 0);
	}
	xy_printf("user RTC callback\n");
	
	osSemaphoreRelease(user_rtc_sem2);
	
}


/**
 * @brief 用户周期性的数据处理入口，内部可以进行ONENET/CDP/SOCKET/MQTT等数据通信，也可以执行用户私有行为
 * @attention   如果是进行远程数据通信，请参考云通信的demo实例，在该函数中调用相关接口即可
 */	
static void user_process()
{
	//need user do
    // do something but that shouldn't last long
    xy_printf("user work start\n");

	//ping test
	at_ReqAndRsp_to_ps("AT+NPING=139.224.112.6,32,1,30,1\r\n", NULL, 2);
	

	//CDP api,see:cdp_regsiter/cdp_senddata

	//onenet api,see cis_simple_api_demo_task/onenet_api_demo_task

	/*user must care Whether the data transmission is successful. If not successful,must do something,such as call xy_reboot to reset*/
	
}


static void user_task_demo(void *args)
{
	int ret;

	(void) args;

	xy_assert(g_softap_fac_nv->deepsleep_enable==0 && g_softap_fac_nv->work_mode==0);

	xy_printf("first set user RTC:%d\n",USER_RTC_OFFSET2);	
#if 0
	xy_rtc_set_by_day(RTC_TIMER_USER1,user_rtc_timeout_cb,NULL,15,3*60*60,0,0);
#else
	//for to reduce BSS stress,and improve communication success rate,user task must set rand UTC,see  xy_rtc_set_by_day
	xy_rtc_timer_create(RTC_TIMER_USER1,USER_RTC_OFFSET2,user_rtc_timeout_cb,NULL);
#endif

	user_rtc_sem2 = osSemaphoreNew(0xFFFF, 0);
	//Perform a data transmission action after power on
	osSemaphoreRelease(user_rtc_sem2);

	while(1)
	{		
		ret = osSemaphoreAcquire(user_rtc_sem2, osWaitForever);
		
		//user RTC timeout
		if(ret == osOK)
		{
			xy_printf("user_task_demo RTC timeout\n");	

			//wait PDP active
			if(xy_wait_tcpip_ok(2*60) == XY_ERR)
				xy_assert(0);
			
			xy_printf("tcpip is ok,start IP packet send\n");			

			//Data communication with server through NB 
			user_process();
			
			xy_printf("set next user rtc:%d\n",USER_RTC_OFFSET2);
			
#if 0
			xy_rtc_set_by_day(RTC_TIMER_USER1,user_rtc_timeout_cb,NULL,15,3*60*60,0,0);
#else			
			//for to reduce BSS stress,and improve communication success rate,user task must set rand UTC,see  xy_rtc_set_by_day
			xy_rtc_timer_create(RTC_TIMER_USER1,USER_RTC_OFFSET2,user_rtc_timeout_cb,NULL);
#endif
			xy_printf("user task end！\n");	
			
		}
		else
			xy_assert(0);

	}

}

/**
 * @brief 用户任务初始化函数，在user_task_init中添加
 * @attention   
 */	
void user_task_demo2_init()
{
	g_user_task_Handle2 = osThreadNew ((osThreadFunc_t)(user_task_demo),NULL,"user_task_demo",0x400,osPriorityNormal); 
}
#endif
