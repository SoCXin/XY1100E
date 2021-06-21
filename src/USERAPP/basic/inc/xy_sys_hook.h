/**
* @file        xy_sys_hook.h
* @brief       与芯翼平台系统运行相关的客户回调接口，客户必须自行实现自己的策略
* @attention   这些回调接口被芯翼平台内部某流程调用，客户需要阅读每个接口的具体功能.如有疑问，咨询芯翼FAE。
* @note    idle线程中执行的函数皆必须放在RAM上，否则会造成频繁的唤醒flash，功耗抬高约5mA
*/

#pragma once
#include <stdint.h>

/**
 * @brief  丢弃客户不需要的URC，以降低对底板MCU的干扰
 * @param  buf [IN] URC string,such as "+CGEV:0,1"
 * @return bool,if XY_OK, will be droped;if XY_ERR, will be sent to external MCU
 * @note   
 */
int drop_unused_urc_hook(char *buf);


/**
 * @brief 平台进入deepsleep前执行该钩子函数，只有当返回XY_OK时才可能进入深睡，该钩子函数作为通用工作锁机制的补充。
 * @return bool,see  @ref  xy_ret_Status_t
 * @warning 在成功进入深睡前可能会多次从idle线程中退出，例如外部中断，进而会造成该接口可能被执行多次，需要用户在接口实现时考虑到多次调用带来的影响。
 * @warning 该接口供用户追加进入深睡的定制条件。例如与外部MCU相连的某个引脚为高电平时，不允许深睡，就可以在此实现。 
 * @warning 该接口在idle线程中调用，因此内部不能使用信号量、互斥量、消息收发等阻塞或释放调度权的操作，例如不能进行flash的擦写操作、xy_printf打印等。
 */
int user_allow_deepsleep_hook();


/**
 * @brief 芯片进入deepsleep前会调用该函数，用户可以在此执行私有动作，也可以进行RTC事件异常的容错等
 * @warning 在成功进入深睡前可能会多次从idle线程中退出，例如外部中断，进而会造成该接口可能被执行多次，需要用户在接口实现时考虑到多次调用带来的影响。
 * @warning 该接口在idle线程中调用，因此内部不能使用信号量、互斥量、消息收发等阻塞或释放调度权的操作，例如不能进行flash的擦写操作、xy_printf打印等。
 */
void user_deepsleep_before_hook();



/**
 * @brief 平台进入standby前执行该钩子函数，只有当返回XY_OK时才可能进入standby
 * @return bool,see  @ref  xy_ret_Status_t
 * @warning 该接口通常用于无法通过API接口来进行standby睡眠控制的场景。例如与外部MCU相连的某个引脚为高电平时，不允许进standby，就可以在此实现。
 * @warning 该接口在idle线程中调用，因此内部不能使用信号量、互斥量、消息收发等阻塞或释放调度权的操作，例如不能进行flash的擦写操作、xy_printf打印等。
 */
int user_allow_standby_hook();


/**
 * @brief 供用户在standby睡眠之前执行一些私有动作，如PIN的重配置等
 * @warning 如果客户对功耗非常敏感，可以参照《芯翼XY1100_IO配置及引脚使用说明》对PIN进行重配置，以防止漏电或电流倒灌
 * @warning 该接口在idle线程中调用，因此内部不能使用信号量、互斥量、消息收发等阻塞或释放调度权的操作，例如不能进行flash的擦写操作、xy_printf打印等。
 */
void user_standby_before_hook();

/**
 * @brief 供用户在standby睡眠唤醒之后执行一些私有动作，如PIN的重配置等
 * @warning 如果客户对功耗非常敏感，可以参照《芯翼XY1100_IO配置及引脚使用说明》对PIN进行重配置，以防止漏电或电流倒灌 
 * @warning 该接口在idle线程中调用，因此内部不能使用信号量、互斥量、消息收发等阻塞或释放调度权的操作，例如不能进行flash的擦写操作、xy_printf打印等。
 */
void user_standby_after_hook();


/*
 * @brief  系统软看门狗超时回调接口，即由@ref xy_user_dog_set接口设置的软看门狗超时回调函数，内部由用户自行决定容错处理。
 * @warning  用户实现该函数内容时，需要充分考虑当前产品运行的状态，做出不同的容错处理，例如3GPP异常时，可以考虑软重启； \n
 *  云通信异常时，可以考虑放弃此次数据传输，过一天后再执行。无论何种容错，用户必须自行保证设备后续能够正常再次工作。
 */
void user_dog_timeout_hook();

/**
 * @brief 供用户通过代码在系统上电时修改某出厂NV参数，仅限于发货后无法直接修改出厂NV配置文件的场景，例如FOTA升级。
 * @warning 有些出厂NV参数在二级boot就会被使用，但是这个函数不会在二级boot过程中被调用，需慎重使用
 */
void xy_reset_fac_nv_hook();


/**
 * @brief   获取电池电量的回调接口
 * @return int    mAh
 * @warning 用户必须实现对电池电量的监控，并提供指示灯等方式提示更换电池 \n
 *     对于FOTA等耗时长的关键动作，必须检测电量是否足够完成该事务 
 * @note  该接口由用户二次开发实现，目前芯翼平台在xy_is_enough_Capacity接口中会调用，以裁决当前电量是否足够FOTA升级。\n
 *  用户根据自身的业务开发，也可以在适当点调用获取当前电量。
 */
unsigned int  xy_getVbatCapacity();


/**
 * @brief  检测电池电量是否足够的回调接口，常见于FOTA等特殊流程
 * @param state   unused
 * @return bool,see  @ref  xy_ret_Status_t
 * @warning  该接口必须配置出厂NV(min_mah)一起使用。目前该接口仅在FOTA升级之前调用，若发现电量不足，则平台会放弃此次升级。\n
 *  用户根据自身的业务开发，也可以适当调用该接口，以决定是否能执行某高功耗的动作。
 */
int  xy_is_enough_Capacity(int state);



/**
 * @brief  检测当前的电压是否达到芯翼芯片工作的最低门限，用户使用时要关注NV参数的设置
 * @param state   unused
 * @return bool,see  @ref  xy_ret_Status_t.if XY_ERR,Vbat is too low,and can not do something; 
 * @warning   目前仅考虑flash擦写动作时的电压检测
 * @note  检测VBAT电压是否过低。系统在开机初始化和flash擦写操作前，会调用接口识别是否为低压。 \n
 * 若是第一次识别为低压，则软重启系统，尝试恢复正常电压；若重启后仍然低压，则系统不再进行任何的flash擦写操作，并通过URC或全局告知用户进行策略动作。 \n
 * 该接口必须配置出厂NV(min_mVbat)一起使用，由用户根据自身的产品形态设置合理的低压工作门限。
 */
int  xy_check_low_vBat(int state);


/**
 * @brief 用户定制实现深睡的URC主动上报具体内容，若用户不实现该接口，将会默认上报“+POWERDOWN:”格式。
 * @note  at_str由芯翼平台负责申请内存并初始化为0，用户在函数内部直接填写有效的URC即可，长度不得超过40字节
 */
void user_powerdown_URC_Hook(char *at_str);

