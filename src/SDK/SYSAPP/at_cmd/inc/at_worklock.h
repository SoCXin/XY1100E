#pragma once

/*******************************************************************************
 *							 Include header files							   *
 ******************************************************************************/
#include "xy_utils.h"

/*******************************************************************************
 *                             Type definitions                                *
 ******************************************************************************/
typedef struct worklock
{
	int type;
	int num;
} worklock_t;

/*******************************************************************************
 *                       Global variable declarations                          *
 ******************************************************************************/
extern osTimerId_t deepsleep_delay_timer;
extern uint16_t g_udp_latest_seq;
extern uint8_t g_user_lock_released;

/*******************************************************************************
 *						Global function declarations 					       *
 ******************************************************************************/

/**
 * @brief 设置延迟深睡机制，接口内部会根据NV参数deepsleep_delay来设置延迟深睡的时长
 * @warning  该接口慎用！目前平台内部会使用该接口，用于外部WAKEUP-PIN中断唤醒的延迟深睡，以防止外部异常唤醒
 * 另外当释放完用户锁后，系统尚未进入深睡时，此时外部mcu输入AT命令不会自动持有外部锁，但会申请延迟锁
 */
void xy_delaylock_active();

/**
 * @brief 移除芯翼延迟锁
 * @warning
 */
void xy_delaylock_suspend();

/**
 * @brief process AT+WORKLOCK cmd
 * @param at_buf [IN]
 * @param prsp_cmd [OUT]
 * @note AT+WORKLOCK=<enable>[,<type>]  enable:1 request lock, 0 release lock
 */
int at_WORKLOCK_req(char *at_buf, char **prsp_cmd);

/**
 * @brief increase worklock func
 * @param type [IN] see @ref WORK_LOCK_TYPE_E
 * @return 
 * @note
 */
int increase_worklock(int type);

/**
 * @brief decrease worklock func
 * @param type [IN] see @ref WORK_LOCK_TYPE_E
 * @return 
 */
int decrease_worklock(int type);

/**
 * @brief	获取某类型锁的数量
 * @param	锁类型, see @ref WORK_LOCK_TYPE_E
 * @return	返回某种类型锁的数量
 */
int get_locknum_by_type(int type);

/**
 * @brief 接收到at命令后,增加特殊的worklock锁
 * @note 使用场景包括 1.狭义模组类客户,并且没有配对使用用户worklock锁的场景
 * @note 使用场景包括 2.释放完用户锁后,系统尚未进入深睡时,再次输入外部AT命令，调用该接口申请延迟锁
 * @warning 蓝牙模式下，外部mcu输入at命令不会持有任何锁
 */
void set_lock_by_MCU_at(void);

/**
 * @brief worklock锁使用的全局变量初始化
 * @note
 */
void worklock_init();

/**
 * @brief 释放锁之后发送RAI
 * @note
 */
void send_rai_proxy(void);

/**
 * @brief 收到sequence的处理
 * @note
 */
int at_UDPRAI_info(char* at_buf);

/**
 * @brief 创建发送rai的线程
 * @note close_rai=1的情况下不触发，由用户触发
 */
void create_send_rai_task();

void clear_xy_worklock(void);