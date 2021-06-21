/*******************************************************************************
 *							 Include header files							   *
 ******************************************************************************/
#include "xy_system.h"
#include "at_ps_cmd.h"
#include "at_worklock.h"
#include "xy_at_api.h"
#include "low_power.h"
#include "oss_nv.h"
#include "hal_adc.h"
#include "xy_utils.h"
#include "xy_hwi.h"
#include "utc.h"
#include "ps_netif_api.h"
#include "sys_init.h"
#include "hw_prcm.h"
#include "flash_adapt.h"
#include "osal.h"
#include "ipcmsg.h"
#include "csp.h"
#include "xy_sys_hook.h"
#include "shm_msg_api.h"
#include "at_ctl.h"
#include "dsp_process.h"
#include "xy_watchdog.h"
#include "at_uart.h"
#include "rtc_tmr.h"

/*******************************************************************************
 *						  Global variable definitions						   *
 ******************************************************************************/
//++ or --,default into standby
int g_user_off_standby = 0;


//0,save fac/var/invar NV,and into deepsleep directly,can wakeup by WAKEUP_PIN;1,save fac/var/invar NV,and not into DEEPSLEEP,wait MCU to OFF power for slow discharge of capacitor
int  g_fast_off = -1; 

osTimerId_t g_xy_user_dog_timer = NULL;


/*******************************************************************************
 *						Local function implementations						   *
 ******************************************************************************/
uint32_t get_sys_up_stat()
{
	
	static uint32_t g_sys_up_stat=0xffffffff;  

	if(g_sys_up_stat==0xffffffff)
	{
		g_sys_up_stat = sys_upstate_get();
		xy_assert(g_sys_up_stat != UNKNOWN_ON);
		HWREG(SYS_UP_REASON) = g_sys_up_stat;

		if(g_sys_up_stat == SOFT_RESET)
		{
			HWREG(SOFT_RESET_REASON) = HWREG(BACKUP_MEM_RESET_FLAG);
		}
		HWREG(BACKUP_MEM_RESET_FLAG) = 0xffffffff;// clear this flag 
	}

	return g_sys_up_stat;
}

uint32_t get_softreset_type()
{
    return HWREG(SOFT_RESET_REASON);
}

void xy_work_lock(int lock_type)
{
	if(1 != increase_worklock(lock_type))
	{
		xy_assert(0);
	}
}

void xy_work_unlock(int lock_type)
{
	//stop to use NB protocol stack
	if(decrease_worklock(lock_type)!= 1)
	{
		xy_assert(0);
	}	
}

//0，仅保存NV后，正常进入DEEPSLEEP；1，保存NV后，lpm_func内通过判断g_fast_off进行锁中断，等待客户断电，解决电容放电问题
void xy_fast_power_off(int abnormal_off)
{
    HAVE_FREE_LOCK = 1;
    g_user_have_free_lock = 1;
    int req_param = 0;
    int i = RTC_TIMER_AP_BASE;

    g_fast_off = abnormal_off;

    if (!DSP_IS_NOT_LOADED())
    {
        //执行fastoff情况下，删除芯翼自己的RTC事件，否则powerdown仍有RTC相关打印
        for (; i < RTC_TIMER_AP_USER_BASE; i++)
            rtc_event_delete(i);

    	req_param = 2;

		at_ReqAndRsp_to_ps("AT+CFUN=5\r\n",NULL,10);
        //such as AT+WORKLOCK=0,but not send RAI to NB base,and not care lock num++
        softap_printf(USER_LOG, WARN_LOG, "do AT+FASTOFF");
		send_shm_msg(SHM_MSG_PLAT_WORKLOCK,&req_param,sizeof(int));
    }
}

//user reboot Soc,param specify which NV to save,can not call by time/RTC hook
//RTC timer still valid
void xy_reboot()
{
    send_debug_str_to_at_uart("+DBGINFO:USER call reboot\n");
    reboot_nv();

    HWREG(PS_START_SYNC) = 0;

    wait_for_csp_tx_allout();

    soft_reset_by_wdt(RB_BY_NRB);
}

void xy_standby_lock()
{
	osCoreEnterCritical();
    g_user_off_standby++;
	osCoreExitCritical();
}

void xy_standby_unlock()
{
	osCoreEnterCritical();
    if(g_user_off_standby != 0)
    {
    	g_user_off_standby--;
    }
	osCoreExitCritical();
}

void xy_standby_clear()
{
	osCoreEnterCritical();
   	g_user_off_standby = 0;
	osCoreExitCritical();
}

//TAU not timeout,and IP addr still valid
int is_powenon_from_deepsleep()
{
	if(g_normal_wakeup == 1)
    	return  XY_OK;
	else
		return  XY_ERR;
}

//该接口内部禁止调用softap_printf，因为此接口可能在云保存过程中（IDLE线程中）被调用到，可调用send_debug_str_to_at_uart接口。
int xy_wait_tcpip_ok(int timeout)
{
    int timeout_compare = 0;
	if (osKernelIsRunningIdle() == osOK && timeout > 0)
    {
        timeout = 0;
    }
    while (ps_netif_is_ok() == 0)
    {
        if (g_check_valid_sim == 0)
        {
        	send_debug_str_to_at_uart("xy_wait_tcpip_ok ERR! NO SIM CARD");
            return  XY_ERR;
        }

        if (timeout == 0)
        {
        	send_debug_str_to_at_uart("xy_wait_tcpip_ok ERR! PDP not active");
            return XY_ERR;
        }
        else if (timeout == (int)(osWaitForever))
        {
            osDelay(100);
        }
        else
        {
            osDelay(100);
            timeout_compare += 100;
            if (timeout_compare >= timeout * 1000)
            {
            	send_debug_str_to_at_uart("xy_wait_tcpip_ok ERR! PDP not active");
                return XY_ERR;
            }
        }
    }
    return XY_OK;
}

//Get battery voltage(mv). XinYi SoC Vol is 3.1V-4.2V
unsigned int xy_getVbat()
{
    short real_val;    // actually read value
    short convert_val; // convert the real value
    short no_cal_vol;  // voltage without calibration
    short cal_vol;     // voltage with calibration
    HAL_ADC_HandleTypeDef ADC_HandleStruct;

    ADC_HandleStruct.Init.Mode = HAL_ADC_MODE_TYPE_VBAT;
	ADC_HandleStruct.Init.Speed = HAL_ADC_SPEED_TYPE_240K;
	//ADC_HandleStruct.Init.ADCRange = HAL_ADC_RANGE_TYPE_1V;

    HAL_ADC_Init(&ADC_HandleStruct);

    int average = 0;

    for (uint32_t i = 0; i < 10; i++)
    {
        real_val = HAL_ADC_GetValue(&ADC_HandleStruct);
        average += real_val;
    }

    HAL_ADC_DeInit(&ADC_HandleStruct);

    if (average >= 0)
        real_val = (average + 5) / 10;
    else
        real_val = (average - 5) / 10;

    convert_val = HAL_ADC_GetConverteValue(&ADC_HandleStruct,real_val);
    no_cal_vol = HAL_ADC_GetVoltageValue(&ADC_HandleStruct,convert_val);
    cal_vol = HAL_ADC_GetVoltageValue(&ADC_HandleStruct,convert_val);

    if (cal_vol != HAL_ADC_ERROR_NO_CALIBRATION)
        return cal_vol;
    else
        return no_cal_vol;
}

void xy_kill_user_dog()
{
	if(g_xy_user_dog_timer != NULL)
		osTimerDelete(g_xy_user_dog_timer);
	g_xy_user_dog_timer = NULL;
}

void xy_user_dog_set(int sec)
{
	if(g_softap_fac_nv->deepsleep_enable==0 || sec==0) 
		return;

	if(g_xy_user_dog_timer != NULL)
		xy_kill_user_dog();
	
	g_xy_user_dog_timer = osTimerNew((osTimerFunc_t)user_dog_timeout_hook, osTimerOnce, NULL,"dog");
	osTimerStart(g_xy_user_dog_timer,sec * 1000);
}


