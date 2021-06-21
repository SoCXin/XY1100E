#include "xy_watchdog.h"
#include "xy_memmap.h"
#include "xy_system.h"
#include "watchdog.h"
#include "xy_nbiot_module_define.h"
#include "oss_nv.h"
#include "at_uart.h"
#include "sys_init.h"

#define WDT_RELOAD_VAL_DEFAULT (0x80000000) //watchdog reload value when powerup by default


osThreadId_t g_wdt_TskHandle = NULL;


//if softreset,retension mem will reset,so use watchdog to keep retension mem,see  get_softreset_type()
void soft_reset_by_wdt(int soft_rst_reason)
{
	while(csp_write_allout_state() == 0);
	
	HWREGB(0xA0058008) = 0x11; //use coreldo
	HWREG(BACKUP_MEM_RESET_FLAG) = soft_rst_reason;

#if BLE_ENABLE
	extern void ble_reset_in_critical(void);
	ble_reset_in_critical();
#endif

	//reset  SOC  by watchdog
	HWREG(0xA0040000) = 0x21;
	HWREG(0xA0040004) = 10;//32768/1000;//1ms later
	while(1);
}

void wdt_task(void)
{
	unsigned int count,ctrlvalue,dsp_watchdog_value;
	static unsigned int s_dsp_watchdog_value = 0;

	unsigned int reload_value;

	while(1)
	{
		count = WatchdogValueGet(WDT_BASE);

		ctrlvalue = HWREG(WDT_BASE + WDT_CTL);

		if(g_softap_fac_nv->hard_dog_time == 0)
		{
			reload_value = WDT_DEFAULT_SEC * XY_UTC_CLK;
		}
		else
		{
			reload_value = g_softap_fac_nv->hard_dog_time * 60 * XY_UTC_CLK;
		}

		dsp_watchdog_value = HWREG(DSP_WATCHDOG_UPDATE);

		//DSP设置看门狗或单核模式或DSP在某种睡眠状态
		if(!DSP_IS_ALIVE() || (dsp_watchdog_value != s_dsp_watchdog_value)) 
		{
			WatchdogReloadSet(WDT_BASE,reload_value);
			s_dsp_watchdog_value = dsp_watchdog_value;
		}
		
		softap_printf(USER_LOG, WARN_LOG, "WDT: WDT_DATA:%d,WDT_CTL:%d,DSP COUNTER UPDATE:%d \n",count,ctrlvalue,dsp_watchdog_value);
		osDelay(WDT_TASK_SEC*1000);
	}
}

uint32_t wdt_task_init()
{
	uint32_t uwRet = pdFAIL;
	unsigned int reload_value;

	if(g_softap_fac_nv->watchdog_enable)
	{		
		//dsp core add this count
		HWREG(DSP_WATCHDOG_UPDATE) = 0;
		
		if(g_softap_fac_nv->off_debug == 0)
		{
			WatchdogResetDisable(WDT_BASE);
			WatchdogIntEnable();
		}
		else
		{
			WatchdogIntDisable();
			WatchdogResetEnable(WDT_BASE);
		}
		
		if(g_softap_fac_nv->hard_dog_time == 0)
		{
			reload_value = WDT_DEFAULT_SEC * XY_UTC_CLK;
		}
		else
		{
			//soft watchdog must little  hard watchdog
			if(g_softap_fac_nv->user_dog_time+1 > g_softap_fac_nv->hard_dog_time)
				reload_value = (g_softap_fac_nv->user_dog_time+1)* 60 * XY_UTC_CLK;
			else
				reload_value = g_softap_fac_nv->hard_dog_time * 60 * XY_UTC_CLK;
		}

		WatchdogReloadSet(WDT_BASE,reload_value);

		WatchdogEnable(WDT_BASE);

		//如果用户启用自动喂狗机制,则创建看门狗线程
		if(g_softap_fac_nv->hard_dog_mode != 1)
		{
			g_wdt_TskHandle = osThreadNew ((osThreadFunc_t)(wdt_task),NULL,"wdt_task",0x300,osPriorityLow);
			osThreadSetLowPowerFlag(g_wdt_TskHandle, osLowPowerProhibit);
		}
	}
	else
	{
		WatchdogResetDisable(WDT_BASE);
		WatchdogReloadSet(WDT_BASE,WDT_RELOAD_VAL_DEFAULT);
		WatchdogDisable(WDT_BASE);
	}
	
	return uwRet;
}

uint32_t wdt_task_deinit()
{
	uint32_t uwRet = pdFAIL;
	if(g_softap_fac_nv->watchdog_enable == 0)
	{
		WatchdogIntDisable();
		
		WatchdogResetDisable(WDT_BASE);
		WatchdogReloadSet(WDT_BASE,WDT_RELOAD_VAL_DEFAULT);
		WatchdogDisable(WDT_BASE);

		if(g_wdt_TskHandle != NULL)
		{
			osThreadTerminate(g_wdt_TskHandle);
			g_wdt_TskHandle = NULL;
		}
	}
	
	return uwRet;
}

void xy_watchdog_refresh(unsigned int sec)
{
	unsigned int reload_value = 0;

	//判断用户设置的时间是否有效
	if(sec < 1 || sec > 131071)
	{
		reload_value = XY_UTC_CLK * WDT_DEFAULT_SEC;
	}
	else
	{
		reload_value = XY_UTC_CLK * sec;
	}

	WatchdogReloadSet(WDT_BASE, reload_value);
}