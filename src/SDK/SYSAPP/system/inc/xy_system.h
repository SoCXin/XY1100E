/** 
* @file     	xy_system.h
* @brief     	平台省电及系统级API接口。
* @attention  	用户不得使用非外部开发的接口及全局变量，否则可能造成系统运行异常
*
*/


#pragma once

/*******************************************************************************
 *							 Include header files							   *
 ******************************************************************************/
#include <stdint.h>
#include "osal.h"
#include "xy_mem.h"
#include "xy_memmap.h"
#include "dsp_process.h"

/*******************************************************************************
 *                             Type definitions                                *
 ******************************************************************************/
/**
* @brief System power on reason,get from SoC registor, see  @ref get_sys_up_stat()
* @param POWER_ON		normal power on
* @param PIN_RESET		reset by PIN button
* @param SOFT_RESET     reset by software 
* @param UTC_WAKEUP     wakeup  by  UTC(RTC)  
* @param EXTPIN_WAKEUP	wakeup by WAKEUP-PIN
* @param WDT_RESET		reset by hardware watchdog 
* @param UNKNOWN_ON     undefined power on,always is hardware error
*/
typedef enum
{
    POWER_ON = 0,  ///  normal power on
    PIN_RESET,     ///  RESET by PIN button
    SOFT_RESET,    ///  reset by software
    UTC_WAKEUP,    ///  UTC timeout wakeup
    EXTPIN_WAKEUP, ///  WAKEUP-PIN
    WDT_RESET,     ///  hardware watchdog
    UNKNOWN_ON,    ///  undefined error,always is hardware error
} SYS_START_STAT;

/**
* @brief reason for  SOFT_RESET, always unused 
* @param RB_BY_ASSERT	by assert
* @param RB_BY_NRB		by soft reboot,such as AT+NRB/xy_reboot/....
* @param RB_BY_RESET    by AT+RESET
* @param RB_BY_POWERTEST by	AT+NV=POWERTEST,unused
* @param RB_BY_FOTA     by  FOTA
* @param RB_BY_LOW_VOL  by xy_check_low_vBat for low vBAT
*/
typedef enum
{
    RB_BY_ASSERT = 0,
    RB_BY_NRB,
    RB_BY_RESET,
    RB_BY_POWERTEST,
    RB_BY_FOTA,
    RB_BY_LOW_VOL,
    RB_MAX,
} RB_TYPE_E;

/**
 * @brief DSP通过核间内存通知M3核省电，省电专用，业务层不准使用
 * @param DSP_NOT_LOADED DSP处于未启动状态
 * @param DSP_ALIVE DSP处于正常工作状态
 * @param DSP_WFI   DSP处于WFI状态
 * @param DSP_STANDBY_READY  DSP已经准备好进入standby模式
 * @param DSP_DEEPSLEEP_READY DSP已经准备好进入deepsleep模式
 */
typedef enum
{
	DSP_NOT_LOADED = 0,
	DSP_ALIVE,
	DSP_WFI,
	DSP_STANDBY_READY,
	DSP_DEEPSLEEP_READY,  //set this value when M3 work only
}DSP_LPM_STATE_D;

/**
 * @brief 平台提供的深睡相关的锁类型，用户默认使用LOCK_DEFAULT，不建议使用其他值
 * @param LOCK_DEFAULT   用户相关业务流程使用，如FOTA、用户云API等，通过xy_work_lock(0)调用，会触发3GPP快速深睡和RAI
 * @param LOCK_XY_LOCAL  业务平台锁机制，用于内部的云UPDATE更新等，会触发3GPP快速深睡，不触发RAI，以防止来不及处理伴随下行报文
 * @param LOCK_DELAY     延迟深睡锁，目前仅限于xy_deepsleep_delay使用，不受LOCK_XY_LOCAL影响，不能触发3GPP快速深睡和RAI
 * @param LOCK_SIM       仅芯翼内部使用，某种条件下不准进入深睡，例如当未检测到SIM卡时，直至用户主动释放LOCK_EXT或LOCK_DEFAULT锁才能进入深睡
 * @param LOCK_EXT       外部MCU使用，通过"AT+WORKLOCK=1"命令触发，会触发3GPP快速深睡和RAI
 */
typedef enum
{
	LOCK_DEFAULT,	
	LOCK_XY_LOCAL,	
	LOCK_DELAY,
	LOCK_SIM,
	LOCK_EXT, 		
	LOCK_MAX,
} WORK_LOCK_TYPE_E;

/*******************************************************************************
 *                       Global function declarations                          *
 ******************************************************************************/

static __INLINE int DSP_IS_NOT_LOADED()
{
	return (HWREG(DSP_LPM_STATE)==DSP_NOT_LOADED);
}

static __INLINE int DSP_IS_STANDBY()
{
	return (HWREG(DSP_LPM_STATE)==DSP_STANDBY_READY);
}

static __INLINE int DSP_IS_WFI()
{
	return (HWREG(DSP_LPM_STATE)==DSP_WFI);
}

static __INLINE int DSP_IS_ALIVE()
{
	return (HWREG(DSP_LPM_STATE)==DSP_ALIVE);
}

static __INLINE int DSP_IS_DEEPSLEEP()
{
	return (HWREG(DSP_LPM_STATE)==DSP_DEEPSLEEP_READY);
}

/**
* @brief This function get  reason for power on.
* @returnThe reason of the Power on, see @ref SYS_START_STAT.
*/
uint32_t get_sys_up_stat();


/**
* @brief This function get softreset type.
* @returnThe reason of the software reset, see @ref RB_TYPE_E.
*/
uint32_t get_softreset_type();


/**
 * @brief  关闭standby浅睡模式，接口内部实现了累加操作，以供多外设配对使用。
 * @warning 平台默认开启standby睡眠功能，当客户使用外设时，由于standby期间会关闭外设时钟，所以需要临时关闭standby睡眠，以防止丢数据。 \n
 *  该接口必须配对使用，即在准备使用外设时调用xy_standby_lock()关闭standby,使用完毕外设时调用xy_standby_unlock()开启standby。
 * @attention  由于存在多个外设同时工作情况，进而要求必须配对开关standby，否则会影响其他外设工作
 */
void xy_standby_lock();

/**
 * @brief  打开standby浅睡模式，接口内部实现了累减操作，以供多外设配对使用。
 * @warning 平台默认开启standby睡眠功能，当客户使用外设时，由于standby期间会关闭外设时钟，所以需要临时关闭standby睡眠，以防止丢数据。 \n
 *  该接口必须配对使用，即在准备使用外设时调用xy_standby_lock()关闭standby,使用完毕外设时调用xy_standby_unlock()开启standby。
 * @attention  由于存在多个外设同时工作情况，进而要求必须配对开关standby，否则会影响其他外设工作
 */
void xy_standby_unlock();

/*
* @brief 清空计数，可进入standby睡眠
* @warning  该接口慎用！强行清空计数，可能会造成正在工作的外设因为进了standby睡眠而数据通信异常。
*/
void xy_standby_clear();

/**
 * @brief 当用户业务需要使用芯翼芯片时，通过该接口来申请锁
 * @param lock_type  锁类型，用户默认使用0，不得使用其他值
 * @warning 必须与 xy_work_unlock配对使用，否则会造成深睡异常
 * @note  理论存在多个用户冲突使用芯翼芯片的可能，只要每个业务配对使用工作锁，就可以保证流程正常
 */
void xy_work_lock(int lock_type);

/**
 * @brief 当用户业务做完后，通过该接口通知芯翼平台准备进入深睡. 内部可选触发RAI快速链接释放。
 * @param lock_type  锁类型，用户默认使用0，不得使用其他值
 * @attention  芯翼芯片的深睡条件是没有用户申请锁的前提下，若内部检测出超过2秒的空闲时间，就会进入深睡； \n
 *  所以，并不是说用户释放锁后，就会很快睡眠，还要取决于3GPP能否调度出2秒以上的空闲，例如若在1.28S的寻呼状态下，永远都不会进入深睡。
 * @note  如果用户确定整机无需继续工作，且下次上电后重新执行attach流程，可以使用xy_fast_power_off来更快速深睡，类似直接断电，做到一秒内进深睡。
 * @warning 必须与xy_work_lock配套使用，否则可能造成其他业务因为提前深睡而造成流程异常。\n
 *  默认释放锁后内部会通过PS触发RAI，如果用户想自行控制RAI的发送类型和发送时机，请参阅接口xy_send_rai。
 */
void xy_work_unlock(int lock_type);

/**
 * @brief 强制释放当前所有工作锁，通知芯翼平台准备进入正常深睡流程，可选触发3GPP执行RAI流程
 * @attention  该接口会执行正常的3GPP流程，直至进入深睡；而xy_fast_power_off接口则是强制让芯片执行深睡，不关心软件流程，包括3GPP的流程
 * @warning   该接口仅供使用多把锁的用户简化释放锁流程使用，用户必须确保在合适的点调用该接口，否则会造成用户任务被提前终止
 */
void xy_unlock_all();

/**
 * @brief 强制芯片进入深睡，不关心当前持有锁的情况，也不关心当前3GPP是否能够调度出4秒以上的空闲时间。平台内部仅保存NV，进而要求下次上电后3GPP必须执行attach流程。
 * @param abnormal_off    缺省值为0
*  @warning 该接口不与基站进行握手通信，类似直接断电，可达到1秒内进深睡。仅能在两个场景下使用：  \n
* 1) 当用户启用的软件看门狗超时后，可以认为当前3GPP状态异常，调用该接口进行强制深睡.\n
* 2) 部分客户为了健壮性考虑，要求不使用PSM模式，每次工作完毕后，调用该接口快速秒级深睡，下次再上电执行attach流程.
 */
void xy_fast_power_off(int abnormal_off);

/**
 * @brief 芯翼平台提供了软件看门狗功能，以防止异常情况下芯片工作过长造成功耗过高。当平台的软件看门狗超时后，会执行user_dog_timeout_hook回调函数。
 * @param sec [IN]  软看门狗时长，即产品用户认为系统工作多长时间后仍未深睡，即判为系统运行异常
 * @note   慎用该接口！\n
 *   芯翼平台提供了user_dog_time配置NV供用户设置，有效时由后台调用该接口进行软看门狗的启动。
 * @attention  对于耗时长的业务，如FOTA差分包下载，用户可以通过该接口重设软看门狗的时长。
 */
void xy_user_dog_set(int sec);

/**
 * @brief  杀掉异常监控的软看门狗，与@ref xy_user_dog_set接口配套使用。
 * @attention  慎用该接口！杀掉软看门狗将会造成芯翼软件没有监控工作异常的能力，需要用户自行设计监控机制，例如在底板MCU上进行监控。
 */
void xy_kill_user_dog();

/**
 * @brief 软重启接口，内部进行相关NV的保存后，触发芯片软重启.
 * @attention 软重启后RTC定时器及当前世界时间保持不变，若重启前已经获取了当前世界时间，则重启后仍然有效
 */
void xy_reboot();

/**
* @brief 当为M3单核工作模式时，若用户想动态使用3GPP进行数据通信，通过该接口实现动态加载DSP核，并启动NBiOT
* @warning 对于OPENCPU产品，进行数据通信之前，先调用该接口启动DSP；对于底板MCU客户，只需发送“AT\r\n”给芯翼芯片就可以动态加载DSP核
*/
#define   xy_Dynamic_load_DSP     dynamic_load_up_dsp(0)

/**
 * @brief 动态关闭DSP核，用于OPENCPU的单M3核工作场景
 * @note  该接口慎用！M3核用户发现不再使用DSP核时，调用该接口强行关闭DSP核，以更低功耗；下次准备再使用DSP核时，调用dynamic_load_up_dsp接口即可。
 */
int shut_down_dsp_core(void);

/**
 * @brief 指示芯片是否从深睡唤醒的，常用于支持深睡的模式，以识别是否为正常唤醒状态
 * 
 * @return bool
 * @note 该接口慎用！
 */
int is_powenon_from_deepsleep();

/**
 * @brief Get the VBAT pin voltage of the chip. 
 * @note The working voltage range of the normal version is 3.1V~4.2V.
 *       The wide voltage version working voltage range is 2.2V~4.2V.
 * 
 * @return unsigned int. unit mV
 */
unsigned int xy_getVbat();

/**
 * @brief 供用户设置硬看门狗的喂狗时长，若未在规定时间内喂狗，会触发硬件重启。
 * @attention  该接口慎用！ 芯翼平台提供了NV配置项供用户设置，有效时由后台调用该接口进行硬看门狗的启动。\n
 *  当NV参数值hard_dog_mode设置为1，芯翼平台会根据NV参数值hard_dog_time的时长进行看门狗设置，一旦超时，则触发硬重启。\n
 * @note  对于不对芯翼芯片进行断电的产品，用户必须设置好软硬件看门狗的时长，以防止流程异常造成的功耗过高或产品工作异常
 */
void xy_watchdog_refresh(unsigned int sec);

/**
 * @brief   This is funtion user get modul_ver（厂商信息）
 * @param   modul_ver  存储modul_ver的内存buf，由调用者分配，buf长度不小于20；  
 * @param   len     modul_ver的内存长度，不小于20
 * @return 	XY_ERR is fail,return XY_OK is success.
**/
int xy_get_MODULVER(char *modul_ver, int len);

/**
 * @brief   This is funtion user get versionExt（软件版本号）
 * @param   versionExt  存储versionExt的内存buf，由调用者分配，buf长度不小于28
 * @param   len   versionExt的内存长度，不小于28
 * @return 	XY_ERR is fail,return XY_OK is success.
**/
int xy_get_VERSIONEXT(char *versionExt, int len);

/**
 * @brief   This is funtion user get hardver（硬件版本号）
 * @param   hardver  存储hardver的内存buf，由调用者分配，buf长度不小于20；
 * @param   len    hardver的内存长度，不小于20
 * @return 	XY_ERR is fail,return XY_OK is success.
**/
int xy_get_HARDVER(char *hardver, int len);