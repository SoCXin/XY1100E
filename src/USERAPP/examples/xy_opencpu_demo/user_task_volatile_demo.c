/** 
* @file        user_task_volatile_demo.c
* @ingroup     opencpu
* @brief       用户高频率周期性数据采集的参考任务代码,如：用户以秒级别的频率周期性采集并保存数据到本地flash，当数据收集到一定阈值，再一次性发送给远程云端。
用户通过修改USER_RTC_OFFSET宏值来定义周期性时长，在user_process中实现用户数据的处理，如云通信等。
为了保证芯片在任何状态下都能正常处理RTC事件，请不要修改参考代码的主框架。
* @attention   必须设置high_freq_data=1;keep_retension_mem=1;work_mode=1
* @par
必须设计好软看门狗机制！！！
* @note         !!!该源文件默认不编译，用户不得直接在该文件中修改！！！\n
*               而应该是自行创建新的私有的源文件，将demo示例代码拷贝过去后再做修改。
*/



#include "xy_api.h"
#include "dsp_process.h"


#if DEMO_TEST


/**
 * @brief user task period lenth,user must vary it by self production
 */	
#define   USER_RTC_OFFSET   (40+osKernelGetTickCount()%60)


osThreadId_t  g_user_task_vol_Handle = NULL;
osSemaphoreId_t	user_rtc_vol_sem = NULL;


/**
 * @brief 软件看门狗定时器，用户根据自身产品设置每次工作的保护时长，一旦超时，则必须进行容错处理
 * @attention   一般的，仅需在用户的主任务中设置该定时器即可。在执行FOTA下载之前，必须重设软看门狗时长，防止下载过程中软看门狗超时
 */
osTimerId_t g_user_dog_timer4 = NULL;

/**
 * @brief 用户软看门狗超时，由客户自行进行容错处理，一般为软重启后，继续之前未完成事务
 * @attention   具体采用哪种策略，由客户根据产品特征自行定义，但必须保护到位，否则有变砖风险
 */	
static  void user_dog_timeout(uint16_t *timer)
{
	(void) timer;

	xy_reboot();
}


/**
 * @brief 用户周期性RTC超时回调接口，不建议用户修改
 */	
static void user_rtc_timeout_cb(void *para)
{
	(void) para;

	if(user_rtc_vol_sem == NULL)
	{
		user_rtc_vol_sem = osSemaphoreNew(0xFFFF, 0);
	}
	xy_printf("user RTC callback\n");
	
	osSemaphoreRelease(user_rtc_vol_sem);
	
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
	
   	osDelay(2000);
	
	//CDP api,see:cdp_regsiter/cdp_senddata

	//onenet api,see cis_simple_api_demo_task/onenet_api_demo_task
}

/**
 * @brief 保存用户的数据到芯翼平台指定的RAM空间，即g_user_vol_data中，至多能保存USER_VOL_DATA_LEN字节数
 * @attention 芯翼平台保证g_user_vol_data里的数据在深睡唤醒过程中不会丢失
 */	
int  save_user_data()
{
	//set g_user_vol_data value,but length must < USER_VOL_DATA_LEN
	(*((int *)g_user_vol_data)) ++;
	osDelay(100);
	*((int *)g_user_vol_data+USER_VOL_DATA_LEN/4-1) += osKernelGetTickCount();

	return 0;
}

/**
 * @brief 用户数据采集和处理主任务，采集的频率USER_RTC_OFFSET用户自行修改，何时上报一次数据给网络用户自行决定
 * @attention 为了功耗更优，尽量将user_rtc_vol_sem存满后再进行网络上报
 */	
static void user_task_vol_demo(void *args)
{
	int ret;

	(void) args;

	xy_assert(g_softap_fac_nv->high_freq_data==1 && g_softap_fac_nv->keep_retension_mem==1 && g_softap_fac_nv->work_mode!=0);

	/*minitor user program abnormal running,propose to set user watchdog timer*/
	g_user_dog_timer4 = osTimerNew((osTimerFunc_t)(user_dog_timeout), osTimerOnce, NULL, "USER_WATCHDOG");
	while(1)
	{		
		//wait user RTC timeout,maybe happen after PS RTC wakeup
		if(user_rtc_vol_sem != NULL)
		{	
			ret = osSemaphoreAcquire(user_rtc_vol_sem, osWaitForever);

			/*minitor user program abnormal running,propose to set user watchdog timer*/
			osTimerStart(g_user_dog_timer4,3*60*1000);

			//user RTC timeout
			if(ret == osOK)
			{
				xy_printf("user_task_volatile_demo RTC timeout\n");	
			
				//not need load DSP core
				xy_work_lock(0);

				//user do process...
				xy_printf("test  g_user_vol_data:%d;%d\n",*((int *)g_user_vol_data),*((int *)g_user_vol_data+USER_VOL_DATA_LEN/4-1));


				//user can vary this trigger condition,such as threshold / timeout / count / length ...
				if((*((int *)g_user_vol_data)) % 2 == 0)
				{
					//load DSP,and send IP packet to server
					dynamic_load_up_dsp(0);
					while(DSP_IS_NOT_LOADED());
					
					//wait PDP active
					if(xy_wait_tcpip_ok(2*60) == XY_ERR)
						xy_assert(0);
					
					xy_printf("tcpip is ok,start IP packet send\n");			

					//Data communication with server through NB 
					user_process();
				}
				else
				{
					save_user_data();
				}

				//free user data until finished current communication,because maybe resend.

				xy_printf("set next volatile user rtc:%d\n",USER_RTC_OFFSET);

				//for to reduce BSS stress,and improve communication success rate,user task must set rand UTC,see  xy_rtc_set_by_day
				xy_rtc_timer_create(RTC_TIMER_USER2,USER_RTC_OFFSET,user_rtc_timeout_cb,NULL);

				xy_printf("volatile user task end,goto deep sleep\n");	
							
				xy_work_unlock(0);

				osTimerStop(g_user_dog_timer4);
			}
			else
				xy_assert(0);
		}
		//first POWENON,only set next user RTC,not use NB PS
		else
		{
			//wakeup by other cause,such as PS RTC/PIN
			if(xy_rtc_next_offset_by_ID(RTC_TIMER_USER2))
			{
				xy_printf("volatile user RTC have setted\n");	
				user_rtc_vol_sem = osSemaphoreNew(0xFFFF, 0);
				continue;
			}
			
			xy_printf("first set volatile user RTC:%d\n",USER_RTC_OFFSET);	

			//not need load DSP core
			xy_work_lock(0);	

			//for to reduce BSS stress,and improve communication success rate,user task must set rand UTC,see  xy_rtc_set_by_day
			xy_rtc_timer_create(RTC_TIMER_USER2,USER_RTC_OFFSET,user_rtc_timeout_cb,NULL);
			
			user_rtc_vol_sem = osSemaphoreNew(0xFFFF, 0);
			
			osDelay(5000);
			
			xy_work_unlock(0);
		}
	}
}

/**
 * @brief 用户任务初始化函数，在user_task_init中添加
 * @attention   
 */	
void user_task_volatile_demo_init()
{
	g_user_task_vol_Handle = osThreadNew ((osThreadFunc_t)(user_task_vol_demo),NULL,"user_task_vol_demo",0x400,osPriorityNormal); 
}
#endif
