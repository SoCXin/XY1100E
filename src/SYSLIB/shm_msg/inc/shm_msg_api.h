#ifndef __SHM_MSG_API_H__
#define __SHM_MSG_API_H__

typedef enum 
{
	SHM_MSG_PLAT_BASE = 0,
	SHM_MSG_PLAT_WORKLOCK,
	SHM_MSG_PLAT_PS_IMEI,
	SHM_MSG_PLAT_PS_IMSI,
	SHM_MSG_PLAT_PS_APN,
	SHM_MSG_PLAT_PS_CELLID,
	SHM_MSG_PLAT_PS_T_ACT,
	SHM_MSG_PLAT_PS_T_TAU,
	SHM_MSG_PLAT_PS_UICC_TYPE,
	SHM_MSG_PLAT_PS_RSSI,
	SHM_MSG_PLAT_PS_RAI,
	SHM_MSG_PLAT_PS_NETWORK_STATE,
	
	SHM_MSG_USER_BASE = 1001,
	SHM_MSG_MAX = 2000,
}SHM_MSG_ID;

typedef struct 
{
	int         msg_id;
	int         len;
    void   		*buf;
} T_SHM_MSG_TYPE;

/**
 * @brief 收到对方核请求消息的hook函数
 */
typedef int (*shmmsg_req_func)(int msg_id, char *data_buf, int data_len);

struct shmmsg_proc_e
{
	struct shmmsg_proc_e *next;
	shmmsg_req_func proc;
	int shmmsg_id;
};


int shmmsg_rsp_process(void *shm_msg);
int shmmsg_req_process(void *shm_msg);

/**
 * @brief 发送请求给DSP核，并阻塞等待DSP核的应答结果，应答结果不能为空
 */
int syn_send_shmmsg_req(int msg_id,void *req_param,int req_param_len,void *rsp_buf,int rsp_buf_len);

/**
 * @brief 仅发送消息给对方核，可以发送请求消息，但不阻塞等待对方应答；也可以发送应答消息
 */
int  send_shm_msg(int msg_id,void *req_param,int req_param_len);

/**
 * @brief 注册对方发送来的消息请求对应的HOOK函数
 */
void register_shmmsg_proc_req(int msg_id, shmmsg_req_func func);

/**
 * @brief 对某对方核发送来的请求，给出对应的应答结果，对方会通过app_send_shmmsg_req_to_extcore接口阻塞等待该结果
 */
int syn_reply_shmmsg_rsp(void *user_param,int user_param_len);

#endif
