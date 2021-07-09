#pragma once

/*******************************************************************************
 *                             Macro definitions                               *
 ******************************************************************************/
#define XY_PROXY_THREAD_NAME "xy_proxy_ctl"

/*******************************************************************************
 *                             Type definitions                                *
 ******************************************************************************/
enum XY_PROXY_MSG_ID
{
    PROXY_MSG_AT_RECV,                 //处理平台内部命令，如WORKLOCK等，避免阻塞at_ctl主线程
    PROXY_MSG_PS_PDP_ACT,              //pdp active
    PROXY_MSG_PS_PDP_DEACT,            //pdp deactive
    PROXY_MSG_INTER_CORE_WAIT_FREE,    //由于零拷贝内存释放机制，灌包测试场景可能会出现m3/dsp两侧写通道满，造成死锁，因此FREE的操作不能在inter_core_msg线程中执行
    PROXY_MSG_RESUME_CDP_EXCEPTION,    //resume CDP exception handling
    PROXY_MSG_RESUME_CDP_UPDATE,    // CDP update handling
    PROXY_MSG_RESUME_ONENET_UPDATE, // ONENET update handling
	PROXY_MSG_HTTP_RESPOND,
};

typedef struct xy_proxy_msg
{
    int msg_id;             //see XY_PROXY_MSG_ID
    int size;               //消息数据长度
    char data[0];           //消息数据
    
} xy_proxy_msg_t;

/*******************************************************************************
 *                       Global function declarations                          *
 ******************************************************************************/
/**
 * @brief xy proxy task initilation
 * @note create xy proxy msg queue,create semphore,create xy_proxy_ctl task
 */
void xy_proxy_init(void);

/**
 * @brief xy proxy ctl task entry
 * @note receive msg from at/cloud task and deal with it
 */
void xy_proxy_ctl(void);

/**
 * @brief 发送消息到主控线程
 * @param  msg_id  see XY_PROXY_MSG_ID
 * @param  buff    需要传递的数据地址
 * @param  len     需要传递的数据长度
 * @return int 
 */
int send_msg_2_proxy(int msg_id, void *buff, int len);

