#ifndef _ONENET_BACKUP_PROC_H
#define _ONENET_BACKUP_PROC_H
#include "at_onenet.h"
#include "rtc_tmr.h"

#define SET_ONENET_REGINFO_PARAM(param,value)  \
        {  \
            if(g_onenet_regInfo != NULL ) \
            {  \
                g_onenet_regInfo->param = value;  \
            } \
        }

#define GET_ONENET_REGINFO_PARAM(param) ({(g_onenet_regInfo != NULL)? g_onenet_regInfo->param : -1; })

extern osThreadId_t g_onenet_exception_recovery_TskHandle;
extern osThreadId_t g_onenet_resume_TskHandle;
extern osThreadId_t g_onenet_rtc_resume_TskHandle;

typedef struct
{
	unsigned short  localPort;
	unsigned short  remotPort;	
	unsigned char   remote_ip[16];

    unsigned int regStatus;					//onenet registration status
    unsigned int ref;						//onenet context ref index
    int flag;								//resume flag
    unsigned char imei[16];					//imei
    unsigned char imsi[16];					//imsi
    unsigned char location[24];				//server location
    unsigned int object_count;				//context object count
    onenet_object_t onenet_object[OBJECT_BACKUP_MAX];	//context object list
    // only record the security instance that describes lwm2m server, basically there is only one
    onenet_security_instance_t onenet_security_instance;	//std secruity instance
    unsigned int onenet_config_len;			//config data len
    unsigned char onenet_config_hex[120];	//config data
    unsigned int observed_count;			//observe count
    onenet_observed_t observed[OBSERVE_BACKUP_MAX];		//observe list
    int last_update_time;					//the newest update time
    int life_time;							//onenet life time
    unsigned short *nextMid;				//the newest message id
	unsigned short *nextObserveNum;
	unsigned int *nextNotifyId;				//the newest notify id
} onenet_regInfo_t;

extern onenet_regInfo_t *g_onenet_regInfo;

void onenet_resume_task();
void onenet_exception_recovery_process();
void onenet_rtc_resume_process();
void onenet_netif_up_resume_process();
void onenet_keeplive_update_process();
rtc_timeout_cb_t onenet_notice_update_process(void *para);
void onenet_resume_state_process(int code);
#endif //#ifndef _ONENET_BACKUP_PROC_H

