/** 
* @file        connectivity_test.c
* @ingroup     cloud
* @brief       该测试用例通过周期性的发送数据给云平台，来进行连通性的压力测试
* @attention   该测试用例仅用于连通性测试，不应该作为客户产品开发参考代码. \n
***************软件看门狗用来监控连通性测试是否成功，若软看门狗超时，则记录下当前失败的一些状态值到flash值，以进一步定位。
**********************************************************************************
*/

/**
 * @defgroup cloud Cloud
 */

#include "xy_api.h"


#if  DEMO_TEST
#include  "xy_demo.h"
#include  "ps_netif_api.h"
#if TELECOM_VER

#define CONN_TASK_RTC_TIME	(10*60)
#define CONN_TASK_WAIT_TIME	(10*60)
#define CDP_REREG_TIMES		3

/**
 * @brief Connectivity任务句柄
 */
static osThreadId_t g_connection_TskHandle = NULL;

static osTimerId_t g_user_dog_timer = NULL;

char send_data[50] = {0};

typedef struct {
	int netif_stat;
	int oosFlag;	
	int rssi;	
	int sysup_stat;
	struct xy_wall_clock  time;		
} FailInfo_t;


typedef struct {
    int  poweron_stat;
    char *poweron_resn;
} ENUM_STR_T;

//记录FAILINFO的flash空间大小
#define FAILINFO_FLASH_SIZE		0x8000      //32K

//记录FAILINFO保存的flash偏移值(复用FOTA BACKUP FLASH)
static unsigned int FAILINFO_OFFSET_FLASH_BASE = 0x271EC000;

//记录FAILINFO的flash空间起始地址(复用FOTA BACKUP FLASH)
static unsigned int FAILINFO_FLASH_BASE = 0x271EC000 + 0x1000;



static ENUM_STR_T s_wakeup_reson_str[]={
    {POWER_ON, 		"POWER_ON"},
    {PIN_RESET, 	"PIN_RESET"},
    {SOFT_RESET, 	"SOFT_RESET"},
    {UTC_WAKEUP, 	"UTC_WAKEUP"},
    {EXTPIN_WAKEUP, "EXTPIN_WAKEUP"},
    {WDT_RESET, 	"WDT_RESET"},
    {UNKNOWN_ON, 	"UNKNOWN_ON"},
   	{0,0},  
};

static unsigned int s_failInfo_offset = 0;
static FailInfo_t cloud_FailInfo = {0};

extern char g_OOS_flag;

/**
 * @brief 记录当前的网络状态信息和rtc时间
 * @note 
 */

void cloud_save_failInfo()
{

	memset(&cloud_FailInfo, 0x00, sizeof(FailInfo_t));
	cloud_FailInfo.netif_stat = ps_netif_is_ok();	//网口状态
	cloud_FailInfo.oosFlag = g_OOS_flag;			//OOS
	cloud_FailInfo.sysup_stat = get_sys_up_stat();		//启动状态
	xy_get_RSSI(&cloud_FailInfo.rssi);			//信号强度，99代表无效值
	xy_rtc_get_time(&cloud_FailInfo.time);			//当前rtc时间
	
	xy_flash_read(FAILINFO_OFFSET_FLASH_BASE, &s_failInfo_offset, sizeof(unsigned int));
	if(s_failInfo_offset == 0xFFFFFFFF)
	{
		s_failInfo_offset = 0;
	}

	if(get_sys_up_stat() == POWER_ON)
	{
		xy_flash_erase(FAILINFO_OFFSET_FLASH_BASE, 0x8000+0x1000);
	}
	
	if((FAILINFO_FLASH_BASE+s_failInfo_offset+sizeof(FailInfo_t)) > (FAILINFO_FLASH_BASE+FAILINFO_FLASH_SIZE))
		xy_assert(0);
	
	xy_flash_write(FAILINFO_FLASH_BASE+s_failInfo_offset, (unsigned char *)&cloud_FailInfo, sizeof(FailInfo_t));	
	s_failInfo_offset += sizeof(FailInfo_t);
	xy_flash_write(FAILINFO_OFFSET_FLASH_BASE, (unsigned char *)&s_failInfo_offset, sizeof(unsigned int));

}

/**
 * @brief Cloud任务资源初始化
 * @note 先检查是否设置用户RTC定时器，如果没有，就设置用户RTC定时器；如果设置就直接退出
 */
void  connection_resource_init()
{

	//wakeup by other cause,such as PS RTC/PIN/NRB
	if(xy_rtc_next_offset_by_ID(RTC_TIMER_USER2) != 0)
	{
		xy_printf("cloud RTC have seted!\n");	
	}
	else
	{	
		//for to reduce BSS stress,and improve communication success rate,user task must set rand UTC,see  set_rtc_by_day_offset
		xy_rtc_timer_create(RTC_TIMER_USER2, CONN_TASK_RTC_TIME, NULL, NULL);
		xy_printf("cloud RTC set %d sec\n", CONN_TASK_RTC_TIME);
	}
}


static void dog_timeout(uint16_t *timer)
{
	(void) timer;

	//设置用户定时器
	xy_rtc_timer_create(RTC_TIMER_USER2, CONN_TASK_RTC_TIME, NULL, NULL);

	//保存当前网络状态信息
	cloud_save_failInfo();	

	//进入快速深睡流程
	xy_fast_power_off(0);
}



/**
 * @brief Connection（CDP业务）任务入口
 *@note 
 1）Cloud任务先检查是否已驻网：驻网失败会触发unlock；
 2）驻网成功会向CDP云平台发起注册；
 3）注册成功并向云平台发送数据包；
 4）发送完成后执行去注册操作
 5）重新设置RTC定时器，之后执行unlock进入深睡
 */
void connection_task(void *args)
{
	(void) args;


	//创建软看门狗，防止业务超时或长时间不能进入深睡；软看门狗超时后重设RTC定时器，保存当前网络状态信息并进入快速深睡
	g_user_dog_timer = osTimerNew((osTimerFunc_t)(dog_timeout), osTimerOnce, NULL, "USER_WATCHDOG");
	osTimerStart(g_user_dog_timer, 5*60*1000);

	xy_work_lock(0);
	
	xy_printf("Connection Task start work\n");

	//等待10*60 秒，大于软看门狗时间，超时会进入软看门狗callback
	if(xy_wait_tcpip_ok(CONN_TASK_WAIT_TIME) == 0)
	{
		xy_printf("pdp is fail \n");
		xy_work_unlock(0);	
		
		g_connection_TskHandle = NULL;
		osThreadExit();
    }
	
	//注册失败会保存必要信息并进入深睡；此处需要建议用户产品进行reboot
	//注册超时最长是2分钟左右，一般不会超软看门狗时间，所以此处需要保存失败时网络信息
    if(cdp_register(86400, 60)  != XY_OK)
    {
		xy_printf("CDP cloud regist server fail \n");
		cloud_save_failInfo();	
		
		xy_work_unlock(0);
		g_connection_TskHandle = NULL;
		osThreadExit();
    }
	
	xy_printf("CDP cloud cdp regist SUCC\n");


	memset(send_data,0,50);	
	int rssi = -1;
	xy_get_RSSI(&rssi);
	sprintf(send_data,"RSI:%d Power Reson:%s", rssi, s_wakeup_reson_str[get_sys_up_stat()].poweron_resn);

	//数据发送失败会保存必要信息并进入深睡；此处需要建议用户产品进行reboot
	//发送数据超时最长是2分钟左右，一般不会超软看门狗时间，所以此处需要保存失败时网络信息
	if(cdp_send_syn(send_data, strlen(send_data), 0) != XY_OK)
	{
		xy_printf("CDP cloud send data fail \n");
		cloud_save_failInfo();	
		
		xy_work_unlock(0);
		g_connection_TskHandle = NULL;
		osThreadExit();

		//xy_reboot(0,1);
	}
	

	if(cdp_deregister(30) == XY_OK)
	{
		xy_printf("cloud deregist server success!");
	}
	else
	{
		xy_printf("cloud deregist server fail!");
	}
	
	xy_rtc_timer_create(RTC_TIMER_USER2, CONN_TASK_RTC_TIME, NULL, NULL);
	xy_work_unlock(0);

}

/**
 * @brief Connection（使用CDP业务）任务创建，在user_task_init中添加
 * @note 连通稳定性测试，使用电信云平台上传信号强度信息和启动状态
 */
void  connection_demo_init()
{

	//资源初始化
	connection_resource_init();

	//设置云平台地址和端口
	if(cdp_cloud_setting("221.229.214.202", 5683) != XY_OK)
    {
       xy_assert(0);
    }
	
	g_connection_TskHandle = osThreadNew ((osThreadFunc_t)(connection_task),NULL,"connection_demo",0x800,osPriorityNormal);

}
#endif


#endif
