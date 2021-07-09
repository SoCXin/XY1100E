#pragma once

/*******************************************************************************
 *							 Include header files							   *
 ******************************************************************************/
#include "lwip/ip_addr.h"
#include "osal.h"
#include "xy_ps_api.h"

/*******************************************************************************
 *							   Macro definitions							   *
 ******************************************************************************/
#define  AT_IPV4V6_MAX_SIZE (80)

/*******************************************************************************
 *                             Type definitions                                *
 ******************************************************************************/
struct dns_server_info { 
    ip4_addr_t dns4;
#if LWIP_IPV6
	ip6_addr_t dns6;
#endif
};

typedef struct
{
	unsigned char imsi[IMSI_LEN];	   //455201033543990
	unsigned char meid[IMEI_LEN];	   //460040772710417
	int      cell_id; //099F6298
} ps_info_t;

//sim card type,see  g_uicc_type
typedef enum
{
	UICC_UNKNOWN = 0,
	UICC_TELECOM, //telecom
	UICC_MOBILE,  //mobile
	UICC_UNICOM,  //unicom
} SIM_card_type_t;

/*******************************************************************************
 *                       Global variable declarations                          *
 ******************************************************************************/
extern char g_OOS_flag;
extern unsigned char *g_ueiccid;
extern short g_ps_pdn_state; //mark actived pdn cid
extern int g_null_udp_rai;
extern int g_working_cid;
extern int g_check_valid_sim; //^SIMST
extern int g_uicc_type;
extern osSemaphoreId_t g_out_OOS_sem;
extern ps_info_t query_psinfo;
extern struct dns_server_info dns46_info[2];

/*******************************************************************************
 *						Global function declarations 					       *
 ******************************************************************************/
int at_AT_req(char *at_buf, char **prsp_cmd);
int at_CGEV_info(char *at_buf);
int at_CSCON_info(char *at_buf);
int at_XYIPDNS_info(char *at_buf);
int at_SIMST_info(char *at_buf);
int at_PSINFO_info(char *at_buf);
int at_XYIMEIMSI_info(char *at_buf);
int at_XYRAI_req(char *at_buf, char **prsp_cmd);
int at_CMEE_req(char *at_buf, char **prsp_cmd);
int at_NSET_req(char *at_buf, char **prsp_cmd);
int at_ECHOMODE_req(char *at_buf, char **prsp_cmd);
int at_CTZEU_info(char *at_buf);
int send_null_udp_paket(char *remote_ip,unsigned short remote_port);
int proc_XYIPDNS_info(char *at_buf);

/**
 * @brief 		 芯翼平台内部通知用户的函数声明
 * @param eventParam  see XY_EVNET_E
 * @warning    该函数被芯翼平台内部调用，由于其会调用用户回调接口，进而要求函数内部不得做长时间阻塞动作，以免造成平台运行异常
 * @example notify_Ps_Event(XY_EVENT_PS_BEARED_STATE_CHANGED, &state, &cid, NULL, NULL);    
 */
void notify_Ps_Event(uint32_t eventId, void *data, uint32_t data_len);