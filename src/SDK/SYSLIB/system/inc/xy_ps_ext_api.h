/** 
* @file     	xy_ps_ext_api.h
* @brief     	该头文件声明的是一些偏门的3GPP相关的接口定义，例如回调机制的API接口等，不建议客户使用。
* @attention  	3GPP相关的通用API接口，在xy_ps_api.h中定义。
*/

#pragma once

#include <stdint.h>

/*******************************************************************************
 *                             Type definitions                                *
 ******************************************************************************/
 /** 
  * @brief 芯翼提供的PS注册事件，用于内部调用用户提供的回调接口
  *  同时也时用户注册回调函数时，注册事件的id
  */
typedef enum
{
	XY_PS_EVENT_MIN = 0,
	XY_PS_EVENT_PDP_STATE,	 //pdp状态事件
	XY_PS_EVENT_SIM_STATE,	 //sim卡状态事件
	XY_PS_EVENT_NETIF_STATE, //netif状态事件
	XY_PS_EVENT_MAX,
} XY_PS_EVNET_E;

/**
 * @brief 芯翼提供的pdp承载网络状态，用户可在回调函数中根据不同的状态做处理
 */
enum XY_PS_BEARER_STATS
{
	XY_PS_BEARER_ACT = 0,  //pdp激活
	XY_PS_BEARER_DEACT,    //pdp去激活
	XY_PS_BEARER_OOS,
	XY_PS_BEARER_IS,
	XY_PS_BEARER_IDLE,
};

/**
 * @brief 芯翼提供的netif状态，用户可在回调函数中根据不同的状态做处理
 */
enum XY_NETIF_STATS
{
	XY_NETIF_DOWN = 0,      //网络不可用
	XY_NETIF_UP, 			//网络可用
};

/**
 * @brief 芯翼提供的sim卡状态，用户可在回调函数中根据不同的状态做处理
 */
enum XY_SIM_STATS
{
	XY_SIM_REMOVED = 0, //无sim卡
	XY_SIM_READY,		//sim可用
};

/**
 * @brief pdp状态参数，用户可从回调函数的参数中获取
 * @param state pdp状态，see @ref XY_PS_BEARER_STATS
 * @param cid pdp承载网channel id
 * @note 只有eventID为XY_PS_EVENT_PDP_STATE的回调参数才可以处理该类型参数
 */
typedef struct xy_pdp_state
{
	unsigned char state;
	unsigned char cid;
} xy_pdp_state_t;

/**
 * @brief netif状态参数，用户可从回调函数的参数中获取
 * @param state netif状态，see @ref XY_NETIF_STATS
 * @note 只有eventID为XY_PS_EVENT_NETIF_STATE的回调参数才可以处理该类型参数
 */
typedef struct xy_netif_info
{
	unsigned char state;
} xy_netif_info_t;

/**
 * @brief sim卡状态参数，用户可从回调函数的参数中获取
 * @param state sim卡状态，see @ref XY_SIM_STATS
 * @note 只有eventID为XY_PS_EVENT_SIM_STATE的回调参数才可以处理该类型参数
 */
typedef struct xy_sim_state
{
	unsigned char state;
} xy_sim_state_t;

/**
 * @brief 用户回调函数传参类型
 * @param eventId 用户注册的PS回调事件，see @ref XY_PS_EVNET_E
 * @param data_len 数据大小
 * @param data 数据地址
 * @note 用户可根据不同的eventId,将data强转为对应的数据类型在进行处理，如pdp状态事件，data需转为xy_pdp_state_t
 */
typedef struct xy_ps_event_param
{
	unsigned int eventId;
	unsigned int data_len;
	void *data;
} xy_ps_event_param_t;

/**
 * @brief 	用户注册的回调函数的声明
 * @param param ps事件回调函数参数，定义可参考 see @ref xy_ps_event_param_t
 * @warning 该函数被芯翼平台内部调用，进而要求函数内部不得做长时间阻塞动作，以免造成平台运行异常
 * 如有阻塞行为,建议获取状态参数后交由其他线程处理
 */
typedef int (*xy_ps_event_callback_t)(xy_ps_event_param_t* param);

/*******************************************************************************
 *                       Global variable declarations                          *
 ******************************************************************************/
/**
 * @brief  注册用户某一类PS事件回调函数的声明
 * @param eventId   用户可注册的PS事件，see @ref XY_PS_EVNET_E
 * @param func  用户实现的相关回调函数,回调函数类型 see @ref xy_ps_event_callback_t
 * @warning  注册的回调函数内部不得使用阻塞机制，不得运行过长时间
 */
void xy_Reg_Ps_Event(uint32_t eventId, void* func);

/**
 * @brief 去注册用户某一类PS事件回调函数的声明
 * @param eventId 用户可注册的PS事件，see @ref XY_PS_EVNET_E
 * @note
 */
void xy_DeReg_Ps_Event(uint32_t eventId);
