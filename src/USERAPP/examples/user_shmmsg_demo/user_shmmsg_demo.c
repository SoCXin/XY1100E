/** 
* @file        user_shmmsg_demo.c
* @ingroup     opencpu
* @brief       跨核的消息交互参考代码，当客户需要在DSP和M3核两边进行二次开发，并需要进行跨核消息交互时，请参考该DEMO。
* @note         !!!该源文件默认不编译，用户不得直接在该文件中修改！！！\n
*               而应该是自行创建新的私有的源文件，将demo示例代码拷贝过去后再做修改。
*/


#include "shm_msg_api.h"
#include "xy_utils.h"
#include "xy_system.h"
#include "user_shmmsg_demo.h"


static osThreadId_t  g_user_shm_task_Handle = NULL;



/**
 * @brief 接收到对方请求消息，做出对应的应答发送给对方核
 */
int shmmsg_req2_func(int msg_id,char *data_buf, int data_len)
{
	(void) msg_id;
	(void) data_buf;
	(void) data_len;
	char *str = xy_zalloc(100);
	memcpy(str,"Zhou Ming is shuai!!!",strlen("Zhou Ming is shuai!!!"));
	syn_reply_shmmsg_rsp(str,strlen(str));
	xy_free(str);
	return 0;
}

/**
 * @brief 发送请求给对方核，并死等对方应答结果
 */
static void user_shm_task_demo(void *args)
{
	(void) args;

	char *rsp_buf = xy_zalloc(100);
	
	syn_send_shmmsg_req(USER_MSG_SHM_REQ1,NULL,0,rsp_buf,100);
	if(strlen(rsp_buf)==0)
		xy_assert(0);
	
	xy_printf("get info arm:%s",rsp_buf);
	xy_free(rsp_buf);
	g_user_shm_task_Handle = NULL;
	osThreadExit();
}

/**
 * @brief 注册一个服务端应答HOOK，接收对方核发送来的请求并处理，在user_task_init中添加
 * @attention   
 */
void user_shmmsg_demo_init()
{
	register_shmmsg_proc_req(USER_MSG_SHM_REQ2,shmmsg_req2_func);
}

/**
 * @brief 创建一个客户端任务，与对方核进行握手通信，在user_task_init中添加
 * @attention   
 */
void test1()
{
	if(g_user_shm_task_Handle == NULL)
		g_user_shm_task_Handle = osThreadNew ((osThreadFunc_t)(user_shm_task_demo),NULL,"user_shm_task_demo",0x400,osPriorityNormal); 
}


