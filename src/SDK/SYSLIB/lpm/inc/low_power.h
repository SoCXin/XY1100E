#ifndef __LOW_POWER_H__
#define __LOW_POWER_H__
#include "xy_utils.h"
#include "lpm_config.h"

#define     PS_ABNORMAL_RESTORE      (2*60*60)


#define     AP_TICKLESS_MIN    20
#define     AP_TICKLESS_MIN_SIGNDLECORE    3

#define     AP_STANDBY_MIN     15
#define     AP_DEEPSLEEP_MIN     100

#define     AP_TICKLESS_MAX    3600000

typedef enum
{
	LPM_ACTIVE = 0,
	LPM_DEEPSLEEP,
	LPM_SLEEP,
	LPM_STANDBY,
	LPM_WFI,
}LPM_MODE;

typedef struct
{
	unsigned long long	msec;
	LPM_MODE 		    lpm_mode;
	void *              invar_nv;  //NON_VOLATILE_NV
	int                 invar_len;
	unsigned char       ps_state;   //1,TAU;2DRX/eDRX
	unsigned char       padding;  
	unsigned short 		rc32k_freq;
}LPM_MSG_T;

typedef struct 
{
	unsigned int ullpmtime_s;
	unsigned int ullpmtime_us;
}LPM_TIME_T;

typedef enum
{
	WKUP_DSP = 0,
	WKUP_TASK,
	WKUP_TMR,
}LPM_WK_TYPE;


typedef enum
{
	SHUTDOWN_DSP_NONE,
	SHUTDOWN_DSP_REQ,
	SHUTDOWN_DSP_DONE,
}SHUTDOWN_DSP_STATE_E;


/**
 * @brief 用户执行快速深睡，可达到1秒内进深睡，不会与空口进行交互 \n
 *  0,save fac/var/invar NV,and into deepsleep directly,can wakeup by WAKEUP_PIN; \n
 *  1,save fac/var/invar NV,and not into DEEPSLEEP,wait MCU to OFF power for slow discharge of capacitor
 * @warning  执行快速深睡，会锁中断，可能造成中断丢失。\n
 * 值为1时，必须执行断电操作，无法通过外部中断再次唤醒
 */
extern int g_fast_off;
extern volatile unsigned long g_deepsleep_dsp_ready;
extern volatile unsigned long g_standby_dsp_ready;
extern volatile unsigned char g_standby_lock;
extern volatile unsigned char g_plllock_check_enable;
extern volatile  long g_standby_wkup_taskid;
extern volatile  long g_standby_wkup_tmrid;
/**
 * dynamic close standby by "AT+STANDBY=0" / "AT+UARTSET" / at_standby_set,and user can vart its value directly,such as ++ -- */
extern int  g_at_off_standby;
extern int  g_user_off_standby;

void lpm_msg_process(LPM_MSG_T * pMsg);

void xy_standby_close();
void xy_standby_open();
void psm_standby_cb(void *para);
//void xy_osTickHandlerLoop(uint32_t uwElapseTicks);
uint64_t utc_cnt_get();
uint64_t utc_ms_get();
unsigned int sys_upstate_get();
void LPM_ARM_Entry_Deep_Sleep();
void lpm_string_output(void *buf, int size);
uint64_t global_timepoint_get(uint64_t ull_offset_ms);
void LPM_ARM_Entry_Deep_Sleep_fast_off();
void  force_into_deep_sleep();


unsigned int LPM_Deepsleep_Allow();

unsigned int LPM_Deepsleep_Allow_Critical();

unsigned int LPM_Standby_Allow();

unsigned int LPM_Standby_Allow_Critical();

unsigned int LPM_Standby_Allow_SingleCore();

unsigned int LPM_Standby_Allow_Critical_SingleCore();

unsigned int LPM_Wfi_Allow();

unsigned int LPM_Wfi_Allow_Critical();

int LPM_Fastoff_Check();

void LPM_Fastoff_Process();

void LPM_Deepsleep_Process();

void Switch_32k_Xtal();

void Check_ioldo_Bypass();

void LPM_Standby_Process();

void LPM_Standby_Process_SingleCore();

void LPM_Wfi_Process();

int shut_down_dspcore_process(void);

#endif
