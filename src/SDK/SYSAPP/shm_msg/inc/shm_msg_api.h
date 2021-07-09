#pragma once


/*******************************************************************************
 *                             Type definitions                                *
 ******************************************************************************/
typedef enum
{
	SHM_MSG_INVALID = -1,
	SHM_MSG_PLAT_BASE = 0,
	SHM_MSG_PLAT_WORKLOCK = SHM_MSG_PLAT_BASE,

	SHM_MSG_PLAT_MAX = 2000,//other see  T_PS_SHM_MSG_ID

} T_PLAT_SHM_MSG_ID;

typedef struct 
{
	int         msg_id;
	int         len;
    void   		*buf;
} T_SHM_MSG_TYPE;

/**
 * @brief 收到对方核消息的hook处理函数,其中msg_id入参用户无需关注，仅用于send_shm_msg_sync同步接口的对方应答，由芯翼后台填写该入参
 */
typedef int (*shmmsg_req_func)(int msg_id, char *data_buf, int data_len);

struct shmmsg_proc_e
{
	struct shmmsg_proc_e *next;
	shmmsg_req_func proc;
	int shmmsg_id;
};

/*******************************************************************************
 *                       Global function declarations                          *
 ******************************************************************************/
int rcv_shm_msg_process(void *shm_msg);

/**
 * @brief 发送请求给DSP核，并阻塞等待对方核的应答结果;如果应答消息是空消息，则入参rsp_buf和rsp_buf_len填0即可
 */
int send_shm_msg_sync(int msg_id, void *req_param, int req_param_len, void *rsp_buf, int rsp_buf_len);

/**
 * @brief 仅发送消息给对方核，但不阻塞等待对方应答
 */
int send_shm_msg(int msg_id, void *req_param, int req_param_len);

/**
 * @brief 注册对方发送来的消息对应的HOOK函数，该回调工作在核间线程中，严禁内部执行阻塞动作或耗时长的动作.一个消息仅能注册一次
 */
void regist_rcved_msg_callback(int msg_id, shmmsg_req_func func);

