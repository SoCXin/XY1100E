#pragma once

/*******************************************************************************
 *						      Include header files							   *
 ******************************************************************************/
#include "lwip/priv/sockets_priv.h"
#include "osal.h"

/*******************************************************************************
 *                             Macro definitions                               *
 ******************************************************************************/
#define ADD_NONIP_PROC_FUNC(uplink_func) (g_nonip_func = uplink_func)
#define RMV_NONIP_PROC_FUNC(uplink_func) \
	do                                   \
	{                                    \
		if (g_nonip_func == uplink_func) \
			g_nonip_func = NULL;         \
	} while (0)

/*******************************************************************************
 *                             Type definitions                                *
 ******************************************************************************/
typedef enum
{
	NON_IP,
	IPV4_TYPE,
	IPV6_TYPE,
	IPV46_TYPE,
	PDP_TYPR_NUM_FLAGS
} IP_ADDR_TYPE;

typedef struct
{
	ip4_addr_t ip;
	ip4_addr_t gateway;
	ip4_addr_t pri_dns;
	ip4_addr_t sec_dns;
} ip4_netif_into;

typedef struct
{
	unsigned char addr_type;
	unsigned char addr_stats;
	unsigned short mtu;
	unsigned short hop_limit;
	unsigned int base_reachable_time;
	unsigned int reachable_time;
	unsigned int retrans_timer;
	struct in6_addr ipv6_addr;
} ip6_netif_into;

/* activation information specific to the PS network, which cid used to identify different PS network port*/
struct pdp_active_info
{
	char c_id;
	char ip46flag; //see IP_ADDR_TYPE
	ip4_netif_into ip4_info;
#if LWIP_IPV6
	ip6_addr_t ipv6;
#endif
};

struct ps_netif
{
	unsigned char cid;  
	struct netif *ps_eth;
};

struct ipdata_info
{
	unsigned char cid;
	unsigned char rai;
	unsigned char data_type;
	unsigned char padding;
	unsigned short sequence;
	unsigned short data_len;
	void *data;
};

typedef int (*nonip_uplink_func)(void *data, int len);

/*******************************************************************************
 *                       Global variable declarations                          *
 ******************************************************************************/
extern nonip_uplink_func g_nonip_func;
extern int now_in_ping;
extern unsigned short g_udp_send_seq[NUM_SOCKETS];
extern unsigned char g_udp_send_rai[NUM_SOCKETS];
extern osMutexId_t g_udp_send_m;

/*******************************************************************************
 *                       Global function declarations                          *
 ******************************************************************************/
void send_packet_to_user(char cid, int len, char *data);
int send_nonip_packet_to_ps(char *data, int data_len, int rai);

/**
 * @brief 查找已经激活的网口
 * @return 返回已激活的网口，netif结构为lwip标准定义 
 * @note
 */
struct netif* find_active_netif();

/**
 * @brief PDP激活处理
 * @param pdp_info 参考pdp_active_info
 * @note
 */
void ps_netif_activate(struct pdp_active_info *pdp_info);

/**
 * @brief PDP去激活处理
 * @param c_id pdp链路ID
 * @param ip46flag IP类型，可以参考IP_ADDR_TYPE定义
 * @note
 */
int ps_netif_deactivate(int c_id, u16_t ip46flag);

/**
 * @brief 查询netif是否启动完成，返回1表示网口已启动，返回0表示未启动
 * @note
 */
int ps_netif_is_ok();

/**
 * @brief 定制命令，对CGATT命令返回结果做特殊处理
 * 平台收到XYIPDNS，但netif网口尚未打开，用户执行CGATT查询，协议栈会返回1，表示网络已通
 * 此时用户如果执行网络命令可能会失败
 * @param at_str CGATT命令返回数据
 * @note
 */
void proc_cgatt_rsp(char *at_str);

void acquire_udp_send_mutex(void);

void release_udp_send_mutex(void);

