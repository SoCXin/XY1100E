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
extern uint8_t g_user_have_free_lock;

/*******************************************************************************
 *						Global function declarations 					       *
 ******************************************************************************/

/**
 * @brief 设置延迟深睡机制，接口内部会根据NV参数deepsleep_delay来设置延迟深睡的时长
 *  
 * @warning  该接口慎用！目前平台内部会使用该接口，用于外部WAKEUP-PIN中断唤醒的延迟深睡，以防止外部异常唤醒
 */
void xy_deepsleep_delay();

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
 * @note
 */
int decrease_worklock(int type);

/**
 * @brief	获取某类型锁的数量
 * @para	锁类型, 0: opencpu用户或者芯翼云业务持有的锁，1：底板MCU用户持有的锁
 * @return	返回某种类型锁的数量
 * @note
 */
int get_locknum_by_type(int type);

/**
 * @brief 接收到at命令后,增加特殊的worklock锁
 * @note 针对模组客户,并且没有配对使用用户worklock锁的场景
 */
void set_special_worklock(void);

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

void create_send_rai_task();
