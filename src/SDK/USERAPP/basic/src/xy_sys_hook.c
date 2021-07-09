/** 
* @file        xy_sys_hook.c
* @ingroup     hook
* @brief       芯翼平台需要客户参与的HOOK函数
* @note    idle线程中执行的函数皆必须放在RAM上，否则会造成频繁的唤醒flash，功耗太高约5mA
*/

#include "xy_utils.h"
#include "xy_system.h"
#include "factory_nv.h"
#include "oss_nv.h"
#include "xy_at_api.h"
#include "sys_init.h"
#include "xy_watchdog.h"
#include "low_power.h"
#include "rtc_tmr.h"

/**********************************************************************************************************************/
/**********************************************************************************************************************/
/***************************************************about   AT command *****************************************************/
/**********************************************************************************************************************/
/**********************************************************************************************************************/


/**
* @brief  standby唤醒时，通过识别AT命令的第三个字符来确定AT命令的边界，以确保能够准确接收完整的AT命令
* @note     用户仅容许修改大括号里的字符，如“AT*XXXX”，仅需将‘*’添加进入即可
* @warning  目前仅能支持小于等于9600波特率的AT命令standby唤醒功能，波特率过高将会造成脏数据，只能人为关闭standby \n
*           对于非AT命令的数据通信，该方案无法执行，必须人为调用接口xy_standby_set来动态开关standby
*/
char g_AT_Req_third_chars[] = {'+', '\r', 'D', 'I','E','e'};
char *p_AT_Req_third_char = g_AT_Req_third_chars; /*user can not change p_AT_Req_third_char and g_third_char_num*/
uint32_t g_third_char_num = sizeof(g_AT_Req_third_chars);



/**
 * @brief  丢弃客户不需要的URC，以降低对底板MCU的干扰
 * @param  buf [IN] URC string,such as "+CGEV:0,1"
 * @return bool,if XY_OK, will be droped;if XY_ERR, will be sent to external MCU
 * @note   
 */
int drop_unused_urc_hook(char *buf)
{
	(void) buf;
/*
	//user private URC
	if(at_strstr(buf,"+USERURC:"))
		return XY_OK;
*/
	return XY_ERR;
}





/**********************************************************************************************************************/
/**********************************************************************************************************************/
/***********************************************about  low power**********************************************/
/**********************************************************************************************************************/
/**********************************************************************************************************************/


/**
 * @brief  idle线程中执行的函数皆必须放在RAM上，否则会造成频繁的唤醒flash，功耗抬高约5mA
 * @note   
 */
int user_allow_deepsleep_hook()	 		__attribute__((section(".ramtext")));
void user_deepsleep_before_hook()					__attribute__((section(".ramtext")));

int user_allow_standby_hook()				__attribute__((section(".ramtext")));
void user_standby_before_hook()				__attribute__((section(".ramtext")));
void user_standby_after_hook()				__attribute__((section(".ramtext")));
void user_powerdown_URC_Hook(char *at_str)				__attribute__((section(".ramtext")));



/**
 * @brief 平台进入deepsleep前执行该钩子函数，只有当返回XY_OK时才可能进入深睡，该钩子函数作为通用工作锁机制的补充。
 * @return bool,see  @ref  xy_ret_Status_t
 * @warning 在成功进入深睡前可能会多次从idle线程中退出，例如外部中断，进而会造成该接口可能被执行多次，需要用户在接口实现时考虑到多次调用带来的影响。
 * @warning 该接口供用户追加进入深睡的定制条件。例如与外部MCU相连的某个引脚为高电平时，不允许深睡，就可以在此实现。 
 * @warning 该接口在idle线程中调用，因此内部不能使用信号量、互斥量、消息收发等阻塞或释放调度权的操作，例如不能进行flash的擦写操作、xy_printf打印等。
 */
int user_allow_deepsleep_hook()
{
	return  XY_OK;
}


/**
 * @brief 芯片进入deepsleep前会调用该函数，用户可以在此保存私有数据到flash中，也可以进行RTC事件异常的容错等
 * @warning 在成功进入深睡前可能会多次从idle线程中退出，例如外部中断，进而会造成该接口可能被执行多次，需要用户在接口实现时考虑到多次调用带来的影响。
 * @warning 该接口在idle线程中调用，因此内部不能使用信号量、互斥量、消息收发等阻塞或释放调度权的操作，例如不能进行xy_printf打印等。
 * @warning 该接口在idle线程中调用，因此用户添加的代码可能会延长睡眠时长，造成功耗略微增加，尤其是写flash动作。
 */
void user_deepsleep_before_hook()
{
#if USER_CARE
	//对于周期性RTC唤醒模型产品，若没有设置RTC事件，则表明发生异常，用户必须根据自己产品设置一个合理的RTC时刻点，以触发唤醒后再次运行。
	if(rtc_get_next_event() == NULL)
	{
		lpm_string_output("NOT SET RTC WAKEUP EVENT!!!\r\n", strlen("NOT SET RTC WAKEUP EVENT!!!\r\n"));
		xy_rtc_timer_create(RTC_TIMER_USER1,10*60,NULL,NULL);
	}
	//对于开启DRX/eDRX，且关闭PSM的产品，由于存在搜不到小区而强制进入深睡的可能，需要用户自行设置下一次唤醒工作的延迟RTC
	else if(xy_rtc_next_offset_by_ID(RTC_TIMER_LPM)==0)
	{
		lpm_string_output("3GPP NOT campon!!!\r\n", strlen("3GPP NOT campon!!!\r\n"));
		xy_rtc_timer_create(RTC_TIMER_LPM,30*60,NULL,NULL);
	}
#endif

#if USER_CARE
	uint8_t  user_data[100];
	xy_ftl_write((unsigned int)(USER_FLASH_BASE),0,user_data,sizeof(user_data));
	xy_flash_write((unsigned int)(USER_FLASH_BASE),user_data,sizeof(user_data));
#endif
}


/**
 * @brief 平台进入standby前执行该钩子函数，只有当返回XY_OK时才可能进入standby
 * @return bool,see  @ref  xy_ret_Status_t
 * @warning  该接口通常用于无法通过API接口来进行standby睡眠控制的场景。例如与外部MCU相连的某个引脚为高电平时，不允许进standby，就可以在此实现。
 * @warning 该接口在idle线程中调用，因此内部不能使用信号量、互斥量、消息收发等阻塞或释放调度权的操作，例如不能进行flash的擦写操作、xy_printf打印等。
 */
int user_allow_standby_hook()
{
	return XY_OK;
}


/**
 * @brief 供用户在standby睡眠之前执行一些私有动作，如PIN的重配置等
 * @warning 如果客户对功耗非常敏感，可以参照《芯翼XY1100_IO配置及引脚使用说明》对PIN进行重配置，以防止漏电或电流倒灌
 * @warning 该接口在idle线程中调用，因此内部不能使用信号量、互斥量、消息收发等阻塞或释放调度权的操作，例如不能进行flash的擦写操作、xy_printf打印等。
 */
void user_standby_before_hook()
{

}


/**
 * @brief 供用户在standby睡眠唤醒之后执行一些私有动作，如PIN的重配置等
 * @warning 如果客户对功耗非常敏感，可以参照《芯翼XY1100_IO配置及引脚使用说明》对PIN进行重配置，以防止漏电或电流倒灌 
 * @warning 该接口在idle线程中调用，因此内部不能使用信号量、互斥量、消息收发等阻塞或释放调度权的操作，例如不能进行flash的擦写操作、xy_printf打印等。
 */
void user_standby_after_hook()
{

}


/*
 * @brief  系统软看门狗超时回调接口，即由@ref xy_user_dog_set接口设置的软看门狗超时回调函数，内部由用户自行决定容错处理。
 * @warning  用户实现该函数内容时，需要充分考虑当前产品运行的状态，做出不同的容错处理，例如3GPP异常时，可以考虑软重启； \n
 *  云通信异常时，可以考虑放弃此次数据传输，过一天后再执行。无论何种容错，用户必须自行保证设备后续能够正常再次工作。
 */
void user_dog_timeout_hook()
{
	//xy_rtc_timer_create(RTC_TIMER_USER1,USER_RTC_OFFSET,NULL,NULL);
	send_debug_str_to_at_uart("ERR!!!USER dog timeout!!!");
	xy_fast_power_off(0);
}


/**
 * @brief 供用户通过代码来修改某出厂NV参数，仅限于发货后无法直接修改出厂NV配置文件的场景，例如FOTA升级
 * @warning 有些出厂NV参数在二级boot就会被使用，在此处修改无法影响二级boot，需慎重使用
 */
void xy_reset_fac_nv_hook()
{
/*
	g_softap_fac_nv->lpm_standby_enable = 0;
	SAVE_FAC_PARAM(lpm_standby_enable);
*/
}


/**
 * @brief   获取电池电量的回调接口
 * @return int    mAh
 * @warning 用户必须实现对电池电量的监控，并提供指示灯等方式提示更换电池 \n
 *     对于FOTA等耗时长的关键动作，必须检测电量是否足够完成该事务 
 * @note  该接口由用户二次开发实现，目前芯翼平台在xy_is_enough_Capacity接口中会调用，以裁决当前电量是否足够FOTA升级。\n
 *  用户根据自身的业务开发，也可以在适当点调用获取当前电量。
 */
unsigned int  xy_getVbatCapacity()
{
	//TO DO by user,see g_softap_fac_nv->min_mah
	unsigned int bCapacity = 90;

	//get  bCapacity  by user

	return  bCapacity;
}


/**
 * @brief  检测电池电量是否足够的回调接口，常见于FOTA等特殊流程
 * @param state   unused
 * @return bool,see  @ref  xy_ret_Status_t
 * @warning  该接口必须配置出厂NV(min_mah)一起使用。目前该接口仅在FOTA升级之前调用，若发现电量不足，则平台会放弃此次升级。\n
 *  用户根据自身的业务开发，也可以适当调用该接口，以决定是否能执行某高功耗的动作。
 */
int  xy_is_enough_Capacity(int state)
{
	(void) state;

	if(xy_getVbatCapacity() >= (unsigned int)(g_softap_fac_nv->min_mah*10))
		return XY_OK;
	else
		return XY_ERR;
}


/**
 * @brief  检测当前的电压是否达到芯翼芯片工作的最低门限，用户使用时要关注NV参数的设置
 * @param state   unused
 * @return bool,see  @ref  xy_ret_Status_t.if XY_ERR,Vbat is too low,and can not do something; 
 * @warning   目前仅考虑flash擦写动作时的电压检测
 * @note  检测VBAT电压是否过低。系统在开机初始化和flash擦写操作前，会调用接口识别是否为低压。 \n
 * 若是第一次识别为低压，则软重启系统，尝试恢复正常电压；若重启后仍然低压，则系统不再进行任何的flash擦写操作，并通过URC或全局告知用户进行策略动作。 \n
 * 该接口必须配置出厂NV(min_mVbat)一起使用，由用户根据自身的产品形态设置合理的低压工作门限。
 */
int  xy_check_low_vBat(int state)
{
	static int  g_vBat_is_low = 0;
	
	(void) state;

	// bug 4076
	if(g_softap_fac_nv->min_mVbat == 0)
		return XY_OK;

	char *at_str = NULL;
	
	if(g_vBat_is_low == 1)
	{
		goto LOW_PROC;
	}

	// 电压正常
	if(xy_getVbat() > (unsigned int)(g_softap_fac_nv->min_mVbat * 100))
		return XY_OK;

	g_vBat_is_low = 1;
	send_debug_str_to_at_uart("+DBGINFO:LOW VBAT\r\n");

	//已经发生过电压检测重启行为，则不再重启，上报debug信息
	if(get_sys_up_stat()==SOFT_RESET && get_softreset_type()==RB_BY_LOW_VOL)
	{
		goto LOW_PROC;
	}
	//第一次检测到低压，触发重启
	else
		soft_reset_by_wdt(RB_BY_LOW_VOL);
	
	return XY_ERR;
	
LOW_PROC:
	at_str = xy_zalloc(50);
	sprintf(at_str, "+DBGINFO:LOW VBAT:%d,work maybe fail!\r\n",xy_getVbat());
	send_debug_str_to_at_uart(at_str);
	xy_free(at_str);

	return XY_ERR;
}


/**
 * @brief 用户定制实现深睡的URC主动上报具体内容，若用户不实现该接口，将会默认上报“+POWERDOWN:”格式
 * @note  at_str由芯翼平台负责申请内存并初始化为0，用户在函数内部直接填写有效的URC即可，长度不得超过40字节
 */
void user_powerdown_URC_Hook(char *at_str)
{
	(void) at_str;
	//memcpy(at_str,"+OFF\r\n",strlen("+OFF\r\n"));
}

