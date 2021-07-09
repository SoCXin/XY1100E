#pragma once
#include "shm_msg_api.h"

/*******************************************************************************
 *                             Type definitions                                *
 ******************************************************************************/

/*通过send_shm_msg和regist_rcved_msg_callback接口进行消息的收发处理，但由于消息接收是工作在核间线程中，进而
要求注册的回调函数必须是非阻塞的，否则会造成核间通信中断异常*/
typedef enum
{
    SHM_MSG_PS_BASE = 2001,
	SHM_MSG_PS_IMEI,
	SHM_MSG_PS_IMSI,
	SHM_MSG_PS_APN,
	SHM_MSG_PS_CELLID,
	SHM_MSG_PS_T_ACT,
	SHM_MSG_PS_T_TAU,
	SHM_MSG_PS_UICC_TYPE,
	SHM_MSG_PS_RSSI,
	SHM_MSG_PS_RAI,

	
    SHM_MSG_PS_MAX = 4000,
} T_PS_SHM_MSG_ID;