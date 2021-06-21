
#ifndef _CDP_BACKUP_H
#define _CDP_BACKUP_H

#include <stdint.h>
#include <stdio.h>
#include "liblwm2m.h"
#include "agent_tiny_demo.h"
#include "rtc_tmr.h"

/*******************************************************************************
 *							   Macro definitions							   *
 ******************************************************************************/
#define CDP_BACKUP_ADDR 	NV_FLASH_NET_BASE
#define CDP_BACKUP_LEN 		NV_FLASH_NET_LEN
		
#define CDP_BACKUP_OBSERVE_MAX		4
#define CDP_BACKUP_WATCHERP_MAX		4

#define SET_CDP_REGINFO_PARAM(param,value)  \
        {  \
            if(g_cdp_regInfo != NULL ) \
            {  \
                g_cdp_regInfo->param = value;  \
            } \
        }

#define GET_CDP_REGINFO_PARAM(param) ({(g_cdp_regInfo != NULL)? g_cdp_regInfo->param : -1; })

extern osThreadId_t g_cdp_timeout_restart_TskHandle;
extern osThreadId_t g_cdp_resume_TskHandle;
extern osThreadId_t g_cdp_rtc_resume_TskHandle;
/*******************************************************************************
 *							   Type definitions 							   *
 ******************************************************************************/

typedef struct 
{
	bool active;
	bool update;		
	xy_lwm2m_media_type_t format;	
	unsigned char  	token[8];
	unsigned int  	tokenLen;
	unsigned int 	lastTime;
	uint32_t counter;
	uint16_t lastMid;
	union
	{
		int64_t asInteger;
		double	asFloat;
	} lastValue;
}cdp_lwm2m_watcher_t;

typedef struct 
{
	lwm2m_uri_t uri;					//observe uri
	unsigned int wather_count;			//observe water count
	cdp_lwm2m_watcher_t watcherList[CDP_BACKUP_WATCHERP_MAX];	//observe watcher list
} cdp_lwm2m_observed_t;



typedef struct {
    unsigned short  localPort;
    unsigned short  srvPort;        //server port
	char            ip_addr_str[16];		//server ip
    unsigned int    local_ip;       //need inet_pton
	int             regStatus;            	//server registration status
	int             resumeFlag;				//resume flag
	unsigned char   endpointname[16];		//endpointname,eg imei
	unsigned int    regtime;		//registration time
	unsigned int    lifetime;		//cdp lifetime
	unsigned int    location_len;	//server location len
	char            server_location[64];	//server location
	cdp_lwm2m_observed_t observed[CDP_BACKUP_OBSERVE_MAX];  //context observe list
	unsigned int    observed_count;	//context observe count
	int             cellID;					//cell id
	int             net_bearer;				//net bearer
	uint16_t	    nextMID;		//next message ID
} cdp_regInfo_t;

extern cdp_regInfo_t *g_cdp_regInfo;

void cdp_timeout_restart_process();
void cdp_attach_resume_process();
void cdp_netif_up_resume_process();
void cdp_rtc_resume_update_process();
void cdp_keeplive_update_process();
rtc_timeout_cb_t cdp_notice_update_process(void *para);
int cdp_resume_task();
bool cdp_handle_exist();
void cdp_resume_state_process(int code);

#endif //#ifndef _AT_ONENET_H

