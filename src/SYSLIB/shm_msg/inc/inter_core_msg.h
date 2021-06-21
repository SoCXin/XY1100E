#pragma once

/*******************************************************************************
 *							Include header files				               *
 ******************************************************************************/
#include "ipcmsg.h"

/*******************************************************************************
 *                             Macro definitions                               *
 ******************************************************************************/
#define SHM_DATA_MAX 64
#define shm_msg_write(buf, len, msg_type) shmmsg_write_channel(buf, len, msg_type, IpcMsg_Normal)

/*******************************************************************************
 *                             Type definitions                                *
 ******************************************************************************/
typedef struct
{
    unsigned char src_id;
    unsigned char log_level;
    unsigned int time;
    unsigned char buf[0];
} AP_LOG_T;

typedef struct 
{
	int         msg_id;
	int         len;
    void   		*buf;
} T_PS_info_copy;

typedef struct 
{
	int         msg_id;
	int         len;
    char   		*buf;
} T_PS_param_req;

/*******************************************************************************
 *                       Global variable declarations                          *
 ******************************************************************************/
extern int g_flow_control_below_size;

/*******************************************************************************
 *                       Global function declarations                          *
 ******************************************************************************/
int shmmsg_write_channel(void *buf, int len, unsigned int msg_type, int chID);
int shm_zero_copy(void *buf, int len, unsigned int msg_type);
void get_ext_msg_sync(char msg_id, int len, void *req, int *rsp_len, void *rsp);
void icm_task_init(void);
