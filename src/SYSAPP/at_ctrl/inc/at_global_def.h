#pragma once

/*******************************************************************************
 *				              Include header files							  *
 ******************************************************************************/
#include "xy_at_api.h"
#include "hw_prcm.h"

/*******************************************************************************
 *                             Macro definitions                               *
 ******************************************************************************/
#define ERR_AT_IT(err) \
	{                  \
		err, #err      \
	}

#define AT_UART_CSP        CSP2_BASE
#define AT_UART_CSP_PRCM   PRCM_CKG_CTL_CSP2_EN


#define AT_RSP_OK          "\r\nOK\r\n"
#define REMOTE_AT_CTRL     "AT+REMOTECTL"

#define NETDOG_AT_STATICS(a) \
	do                       \
	{                        \
		((&at_netdog)->a);   \
	} while (0)

/*兼容DSP diag代码*/
#define AT_SEDN_TO_EXT(a)  send_urc_to_ext(a)

/*******************************************************************************
 *                             Type definitions                                *
 ******************************************************************************/
/**
* @brief Nearps receive callback declaration
* @param buf [IN] at_cmd progress result or response;
* @param len [IN] size of at_cmd progress result or response;
* @return  see specific func
* @note use callback for decouple at_ctl and shm module
*/
typedef void (*recv_from_nearps_callback)(void *buf, unsigned int len);

/**
* @brief send msg to control module that in the system inner
* @param type [IN] msg type,see @AT_MSG_ID
* @param data [IN] msg data
* @param size [IN] msg data size
* @param fd [IN] at source fd,see @AT_SRC_FD
* @return  see specific func,now we only use send_msg_2_atctl
* @note now only send to atctl
*/
typedef int (*send_at_msg_callaback)(int type, void *data, int size, int fd);

/**
* @brief broadcast at info/response to farps callback
* @param buf [IN] data address
* @param size [IN] data size
* @return  see specific func
* @note callback can use in other module,such as nv
*/
typedef int (*at_broadcast_callback)(void *buf, int size);

typedef struct
{
	recv_from_nearps_callback recv_from_nearps_cbk;
	send_at_msg_callaback send_at_msg_cbk;
	at_broadcast_callback at_broadcast_cbk;
} at_ctrl_callback;

//NETDOG_AT_STATICS
struct at_debug_stat
{
    int dbg_drop_nearps_num; //err,because context state err,drop AT cmd from PS
    int dirty_sum;           //recved AT req sum,find "\r",do ++
    int tail_dirty_sum;      //err,tail dirty sum,"WORKL***"
    int all_dirty_sum;       //err,recved at not include '\r'
    char dbg_pdp_act_succ;
    char dbg_pdp_act_fail;     //err,PDP act too long,maybe  power off or reset
    char dbg_pdp_deact_num;    //rcv  PDN DEACT  num  sended  by  PS
    char dbg_drop_up_sum;      //test drop up ip data sum
    char dbg_drop_down_sum;    //test drop down ip data sum
    char dbg_dm_no_need_start_num;
    char dbg_dm_no_get_iccid_fail;
    char dbg_dm_success_num;
    char dbg_dm_failed_num;
    char dbg_dm_count_num;
};

typedef struct
{
	int errno;
	char *errname;
} at_err_msg_t;

enum AT_MSG_ID
{
	AT_MSG_RCV_STR_FROM_FARPS,	//farps send  at cmd to  at_ctl
	AT_MSG_RCV_STR_FROM_NEARPS, //ps(dsp/user rsp) send at cmd to at_ctl
	AT_MSG_REQ_TO_FARPS,
	AT_LOAD_DSP, //when PS rtc wakeup,and workmode==1,load DSP
};

enum AT_SRC_FD
{
	PS_SYS_MIN = 0,
	PS_FD_NOT_AVAIL = PS_SYS_MIN,
	//Nearps
	NEARPS_DSP_FD,
	NEARPS_USER_FD,
	//Farps
	FARPS_UART_FD, //串口终端
	FARPS_BLE_FD, //蓝牙
	FARPS_M3_FD,  //M3核
	PS_SYS_MAX,
	//Farps user
	FARPS_USER_MIN = 0x8,
	FARPS_USER_PROXY = FARPS_USER_MIN, //proxy use fixed fd to tell at_ctl not transfer to proxy itself
	FARPS_USER_MAX = 0xC, //目前最多支持4路user task并发执行at_ReqAndRsp_to_ps
};

/*******************************************************************************
 *						  Global variable declarations						   *
 ******************************************************************************/
extern int g_FOTAing_flag;
extern int g_erron_mode;
extern int g_Echo_mode;
extern int g_NITZ_mode;
#ifdef WAKEUP_MCU_BY_URC
extern int g_ring_enable;
extern int g_duration;
#endif
extern struct at_debug_stat at_netdog;
extern at_ctrl_callback g_at_ctrl_cbk;

/*******************************************************************************
 *                       Global function declarations                          *
 ******************************************************************************/
/**
 * @brief Get at error descriptor with string
 * @param errno [IN] error can see @AT_XY_ERR
 * @return error descriptor with string
 * @note 
 */
char *get_at_err_string(int errno);

/**
 * @brief judge at request is offtime_related or not
 * @param at_prefix [IN] error no
 * @return 1:related,0: not related
 */
int is_offtime_related_req(char *at_prefix);

int xy_Remote_AT_Req(char *req_at);

int xy_Remote_AT_Rsp(char **rsp_data, int *len);
