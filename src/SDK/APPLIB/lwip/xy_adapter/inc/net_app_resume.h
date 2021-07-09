#ifndef _NET_APP_RESUME_H
#define _NET_APP_RESUME_H

#include "lwip/sockets.h"
#include "lwip/ip4_addr.h"
#if AT_SOCKET
#include "at_socket_context.h"
#endif
#if TELECOM_VER
#include "cdp_backup.h"
#endif
#if MOBILE_VER
#include "onenet_backup_proc.h"
#endif

#define CLOUD_LIFETIME_DELTA(param)             ((param)*9/10)  //距离lifetime 超时的时间间隔(s)

#define SET_NET_RECOVERY_FLAG(param)  \
        {  \
            if(ret == CLOUD_SAVE_NEED_WRITE)  \
                g_app_recovery_type|=(1<<param);  \
        }

#define NET_NEED_RECOVERY(param)    ((g_app_recovery_type&(1<<param))==(1<<param))

#define OFFSET_NETINFO_PARAM(param) ((int)&(((app_deepsleep_infos_t *)0)->param))


typedef struct _app_deepsleep_infos_t{
unsigned int  recovery_type;
unsigned int  local_addr;

#if AT_SOCKET
udp_context_t udp_context;    
#endif

#if MOBILE_VER
onenet_regInfo_t onenet_regInfo;
#endif

#if TELECOM_VER
cdp_regInfo_t    cdp_regInfo;
#endif

}app_deepsleep_infos_t;


typedef struct _net_infos_t{
    unsigned short  local_port;
    unsigned short  remote_port;
    char            remote_ip[16];
}net_infos_t;

typedef enum {
    NO_TASK   = -2,
    UNKNOWN_IP = -1,
    SOCKET_TASK = 0,
    ONENET_TASK = 2,
    CDP_TASK,
}net_app_type_t;

typedef enum
{
    CLOUD_SAVE_NONEED_WRITE = -1,
    CLOUD_SAVE_NEED_WRITE,
}cloud_resume_save_type;

typedef enum
{
    IP_RECEIVE_ERROR = -2,
    IP_NO_CHANGED = -1,
    IP_IS_CHANGED = 0,
}IP_comparison_result_type;

typedef enum {
    LIFETIME_TIMEOUT = -1,    //lifetime已到期
    REG_CONFIG_ERROR = -2,     //配置信息错误
    STATE_ERROR = -3,          //状态机状态错误
}cloud_resume_error_type_t;

typedef enum {
    RESUME_SUCCEED = 0,
    RESUME_SWITCH_INACTIVE ,    //恢复开关未打开
    RESUME_READ_FLASH_FAILED ,  //读取flash异常
    RESUME_FLAG_ERROR,          //云业务标志位错误
    RESUME_OTHER_ERROR,         //异常错误
}cloud_resume_result_type_t;

extern osMutexId_t g_cloud_resume_mutex;
extern unsigned int g_app_recovery_type;
int  is_IP_changed();
int  resume_net_app(net_app_type_t type);
void save_net_app_infos(void);
int  resume_net_app_by_dl_pkt(void *data, unsigned short len);
void cloud_resume_init(void);
bool is_exist_resume_cloud_data();
#endif

