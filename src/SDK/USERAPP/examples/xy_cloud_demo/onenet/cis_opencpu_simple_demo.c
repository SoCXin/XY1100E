/** 
* @file        onenet_opencpu_simple_demo.c
* @ingroup     cloud
* @brief       使用简化API实现Onenet与网络云平台交互的参考代码，demo创建线程完成注册和消息处理，发包和去注册在demo线程操作。
* @attention  
* @note         !!!该源文件默认不编译，用户不得直接在该文件中修改！！！\n
*               而应该是自行创建新的私有的源文件，将demo示例代码拷贝过去后再做修改。
*/

/**
 * @defgroup cloud Cloud
 */
#include "xy_api.h"

#if DEMO_TEST
#if MOBILE_VER
#include "xy_cis_api.h"
extern void cis_write_type_downstream_cb(char *data, int data_len);
/**
 * @brief Demo默认注册云平台服务器地址
 */	
#define CIS_SERVER_IP			"183.230.40.40"
/**
 * @brief Demo默认注册云平台服务器端口
 */	
#define CIS_SERVER_PORT			5683

/**
 * @brief Demo的任务句柄
 */
osThreadId_t      g_cis_simple_demo_Handle = NULL;

/**
 * @brief Demo任务的处理入口
 * @note 
 Demo任务自动创建onenet套件，添加了默认的3311对象和一个实例，并使用默认参数向云平台发起注册；
 注册成功后自动向云平台发送上行数据，发送成功后自动执行去注册操作
 */
static void cis_simple_demo_task(void *args)
{
	(void) args;

    xy_printf("[CIS_SIMPLE_OPENCPU]start api demo");
	cis_cloud_setting(CIS_SERVER_IP, CIS_SERVER_PORT, NULL);

    xy_printf("[CIS_SIMPLE_OPENCPU]start REGISTER");
    if(cis_ext_reg(30))
    {
        xy_printf("[CIS_SIMPLE_OPENCPU]REGISTER failed");
        goto out;
    }

    xy_printf("[CIS_SIMPLE_OPENCPU]start senddata");
    if(cis_ext_send("A1B1C1D1",strlen("A1B1C1D1"), 30))
    {
        xy_printf("[CIS_SIMPLE_OPENCPU]senddata failed");
        goto out;
    }

    xy_printf("[CIS_SIMPLE_OPENCPU]start DEREGISTER");
    cis_ext_dereg();

    xy_printf("[CIS_SIMPLE_OPENCPU]cis simple api demo task end");	

out:
    //删除write type类型下行报文(下行数据)回调函数
    cis_set_write_type_downstream_cb(NULL);
	osThreadExit();
}

/**
 * @brief Demo任务的创建
 */
void cis_simple_demo_task_init()
{
	xy_printf("cis simple api demo task create");

    //注册write type类型下行报文(下行数据)回调函数
	cis_set_write_type_downstream_cb(cis_write_type_downstream_cb);

    if(g_cis_simple_demo_Handle == NULL)
		g_cis_simple_demo_Handle = osThreadNew ((osThreadFunc_t)(cis_simple_demo_task),NULL,"cis_simple_demo_task",0x800,osPriorityNormal);
    else
        xy_printf("cis simple api demo task running");
}

#endif
#endif
