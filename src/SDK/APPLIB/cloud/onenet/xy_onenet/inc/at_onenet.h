#ifndef _AT_ONENET_H
#define _AT_ONENET_H

#include <cis_def.h>
#include <cis_api.h>
#include <cis_if_sys.h>
#include <cis_def.h>
#include "std_object.h"
#include "xy_cis_api.h"

/*******************************************************************************
 *							   Macro definitions							   *
 ******************************************************************************/
#define STR_VALUE_LEN         NET_MAX_USER_DATA_LEN
#define OPAQUE_VALUE_LEN      (1024)
#define FLOAT_VALUE_LEN       (160+1)
#define INT_VALUE_LEN         (20+1)
#define ON_WRITE_LEN          (70)
#define ONENET_STACK_SIZE     1024*4
#define MAX_ONE_NET_AT_SIZE   120
#define MAX_SET_PARAM_AT_SIZE 150
#define MAX_SET_PARAM_SIZE    80
#define CIS_ERROR_STR_LEN     30
#define CIS_PARAM_ERROR       601    
#define CIS_STATUS_ERROR      602   
#define CIS_UNKNOWN_ERROR     100   
#define CIS_REF				  0

#define CIS_REF_MAX_NUM       1

#define onet_sizeof(TYPE, MEMBER) (sizeof(((TYPE *)0)->MEMBER))
#define ERR_CIS_IT(err) \
	{                  \
		err, #err      \
	}
/*******************************************************************************
 *							   Type definitions 							   *
 ******************************************************************************/
struct onenet_conf {
	int total_size;
	char *cur_config;
	int index;
	int cur_size;
	int flag;
};

struct onenet_addobj {
	int ref;
	int objId;
	int insCount;
	char *insBitmap;
	int attrCount;
    int actCount;
};

struct onenet_delobj {
	int ref;
	int objId;
};

struct onenet_read {
	int ref;
	int msgId;
    int result;
	int objId;
	int insId;
	int resId;
	int value_type;
	int len;
	char *value;
	int index;
	int flag;
	uint8_t raiflag;
};

struct onenet_notify {
	int ref;
	int msgId;
    int result;
	int objId;
	int insId;
	int resId;
	int value_type;
	int len;
	char *value;
	int index;
	int flag;
	int ackid;
	uint8_t raiflag;
};


struct onenet_write_exe {
	int ref;
	int msg_id;
	int result;
	uint8_t raiflag;
};
struct onenet_discover_rsp {
	int ref;
	int msgId;
	int result;
    int length;
    char *value;
    uint8_t raiflag;
};
struct onenet_parameter_rsp {
	int ref;
	int msg_id;
	int result;
    uint8_t raiflag;
};
struct onenet_update {
	int ref;
	int lifetime;
	int withObjFlag;
	uint8_t raiflag;
};
struct result_code_map{ 
    unsigned char at_result_code;
    unsigned char coap_result_code;
};

enum onenet_rsp_type{
	RSP_READ = 0,
	RSP_WRITE,
	RSP_EXECUTE,
	RSP_OBSERVE,
	RSP_SETPARAMS,
	RSP_DISCOVER,
};

typedef struct onenet_context_config_s {
	char *config_hex;
    int total_len;
    int offset;
    int index; // in descend order, 0 represents last config message
    int isDM;
} onenet_context_config_t;

typedef struct onenet_context_reference_s {
	unsigned int ref;
	st_context_t *onenet_context;
    osThreadId_t onet_at_thread_id;
    int thread_quit;
    onenet_context_config_t *onenet_context_config;
} onenet_context_reference_t;

typedef struct
{
	int errcode;
	char *errname;
} cis_err_msg_t;

extern int at_proc_miplconf_req(char *at_buf, char **prsp_cmd);
extern int at_onenet_create_req(char *at_buf, char **prsp_cmd);
extern int at_proc_miplcreate_req(char *at_buf, char **prsp_cmd);
extern int at_proc_mipldel_req(char *at_buf, char **prsp_cmd);
extern int at_proc_miplopen_req(char *at_buf, char **prsp_cmd);
extern int at_proc_mipladdobj_req(char *at_buf, char **prsp_cmd);
extern int at_proc_mipldelobj_req(char *at_buf, char **prsp_cmd);
extern int at_proc_miplclose_req(char *at_buf, char **prsp_cmd);
extern int at_proc_miplnotify_req(char *at_buf, char **prsp_cmd);
extern int at_proc_miplread_req(char *at_buf, char **prsp_cmd);
extern int at_proc_miplwrite_req(char *at_buf, char **prsp_cmd);
extern int at_proc_miplexecute_req(char *at_buf, char **prsp_cmd);
extern int at_proc_miplobserve_req(char *at_buf, char **prsp_cmd);
extern int at_proc_miplver_req(char *at_buf, char **prsp_cmd);
extern int at_proc_discover_req(char *at_buf, char **prsp_cmd);
extern int at_proc_setparam_req(char *at_buf, char **prsp_cmd);
extern int at_proc_update_req(char *at_buf, char **prsp_cmd);
extern int at_proc_rmlft_req(char *at_buf, char **prsp_cmd);

extern cis_coapret_t onet_on_read_cb(void* context,cis_uri_t* uri,cis_mid_t mid);
extern cis_coapret_t onet_on_write_cb(void* context,cis_uri_t* uri, const cis_data_t* value,cis_attrcount_t attrcount,cis_mid_t mid);
extern cis_coapret_t onet_on_execute_cb(void* context, cis_uri_t* uri, const uint8_t* value, uint32_t length,cis_mid_t mid);
extern cis_coapret_t onet_on_observe_cb(void* context, cis_uri_t* uri, bool flag, cis_mid_t mid);
extern cis_coapret_t onet_on_discover_cb(void* context,cis_uri_t* uri,cis_mid_t mid);
extern cis_coapret_t onet_on_parameter_cb(void* context, cis_uri_t* uri, cis_observe_attr_t parameters, cis_mid_t mid);
extern void onet_on_event_cb(void* context, cis_evt_t eid, void* param);

#define OBSERVE_BACKUP_MAX 8
#define OBJECT_BACKUP_MAX 8
#define ONENET_BACKUP_ADDR 		NV_FLASH_NET_BASE
#define ONENET_BACKUP_LEN 		NV_FLASH_NET_LEN

typedef struct st_security_instance_dup
{
	struct st_security_instance_dup *	next;        // matches lwm2m_list_t::next
	cis_listid_t					instanceId;  // matches lwm2m_list_t::id
	char *							host;        // ip address;
#if CIS_ENABLE_DTLS
	char *							identify;
	char *							secret;
	char *							public_key;
#endif
	bool                            isBootstrap;
	uint16_t						shortID;
	uint32_t						clientHoldOffTime;
	uint8_t							securityMode;
#if CIS_ENABLE_DTLS
#if CIS_ENABLE_PSK
	uint8_t							id_len;
	uint8_t							psk_len;
#endif
#endif
} security_instance_dup_t;


typedef struct
{
    st_uri_t          uri;			//observe uri
    unsigned int      msgid;		//observe message id
    unsigned int      token_len;	//observe request token len
    unsigned char     token[8];		//observe request token
    unsigned int      last_time;	//the newest observe request time
    unsigned int      counter;		//observe request count
    unsigned short    lastMid;		//the newest message id
    et_media_type_t   format;		//observe request data format
} onenet_observed_t;

typedef struct
{
    unsigned int					instanceId;  // matches lwm2m_list_t::id
    char							host[64];        // ip address;
#if CIS_ENABLE_DTLS
    char							identify[32];
    char							secret[64];
    char							public_key[64];
#endif
    int                             isBootstrap;
    unsigned short					shortID;
    unsigned int				    clientHoldOffTime;
    unsigned char				    securityMode;
#if CIS_ENABLE_DTLS
#if CIS_ENABLE_PSK
    unsigned char                   id_len;
    unsigned char                   psk_len;
#endif
#endif
} onenet_security_instance_t;

typedef struct
{
    cis_listid_t        objID;
    cis_instcount_t     instBitmapBytes;
    cis_instcount_t     instBitmapCount;
    uint8_t             instBitmapPtr[32]; // we assume instance count of one user-added object is no more than 32
    cis_instcount_t     instValidCount;

    cis_attrcount_t     attributeCount;
    cis_actcount_t      actionCount;
    //void *              userData;
} onenet_object_t;

int onet_resume_task();
int is_onenet_task_running(unsigned int ref);
int is_onenet_state_ready(unsigned int ref);
void onet_at_pump(void* param);
void at_onenet_init();
void resume_onenet(void* param);
void onenet_update(void *ctx);

#endif //#ifndef _AT_ONENET_H

