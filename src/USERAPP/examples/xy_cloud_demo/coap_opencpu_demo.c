/*********************************************************************************
* @file        coap_opencpu_demo.c
* @ingroup     cloud
* @brief       使用COAP API实现与COAP服务器的通信
* @attention
* @note         !!!该源文件默认不编译，用户不得直接在该文件中修改！！！\n
*               而应该是自行创建新的私有的源文件，将demo示例代码拷贝过去后再做修改。
*/
#include "xy_api.h"

#if DEMO_TEST && LIBCOAP
#include "xy_utils.h"
#include "xy_demo.h"
#include "xy_coap_api.h"

/*COAP connect information*/
#define COAP_SERVER_IP       "221.229.214.202"
#define COAP_SERVER_PORT     5683
#define COAP_LOCAL_PORT      5555

/*COAP message information*/
#define COAP_REQUEST_METHOD  "post"     /*include get post put delete */
#define COAP_REQUEST_TYPE    "con"     /*include con non ack rst */
#define COAP_MESSAGE_ID      1234
#define COAP_TOKEN           "abcdef"
#define COAP_OPTION_TYPE     11
#define COAP_OPTION_VALUE    "rd"
#define COAP_PAYLOAD         "616263646566"
#define COAP_PAYLOAD_LEN      6


static osThreadId_t g_coaptest_handle =NULL;
static osThreadId_t g_coapdownlink_handle = NULL;
static int coap_stop_flag = 0;
static coap_context_t* coap_client= NULL;


void coap_downlink_messgae_rev()
{
    xy_printf("[COAPdemo]downlink_messgae thread start");

    if(xy_wait_tcpip_ok(2*60) == XY_ERR)
        xy_assert(0);

    xy_coap_asy_receive(coap_client,&coap_stop_flag);

    g_coapdownlink_handle = NULL;

    xy_printf("[COAPdemo]downlink_messgae thread end");
	osThreadExit();

}

void coap_downlink_messgae_process_thread_create()
{
    if (g_coapdownlink_handle != NULL)
    {
        return;
    }
	g_coapdownlink_handle = osThreadNew ((osThreadFunc_t)(coap_downlink_messgae_rev),NULL,"coap_downlink_messgae_process",0x1000,osPriorityNormal);
}

static int xy_coap_demo()
{
    /* create COAP client */
    coap_client = xy_coap_client_create(COAP_SERVER_IP,COAP_SERVER_PORT);

    /* create task to receive COAP downlink message */
    coap_downlink_messgae_process_thread_create();

    /* config COAP message header */
    xy_config_coap_head(COAP_MESSAGE_ID,COAP_TOKEN,strlen(COAP_TOKEN));

    /* config COAP message option */
    xy_config_coap_option(COAP_OPTION_TYPE,COAP_OPTION_VALUE);

    /* config COAP message payload and send */
    xy_coap_asy_send(coap_client,COAP_REQUEST_TYPE, COAP_REQUEST_METHOD, COAP_PAYLOAD,COAP_PAYLOAD_LEN);
    xy_printf("[COAPdemo]coap message send");

    /* stop recv thread */
    coap_stop_flag = 1;

    /* delete COAP client and free memory */
    xy_coap_client_delete(coap_client);
    xy_printf("[COAPdemo]xy_coap_demo end");
    return 0;
}

void coap_test_demo(void)
{
    //wait PDP active
    if(xy_wait_tcpip_ok(2*60) == XY_ERR)
        xy_assert(0);

    xy_printf("[COAPdemo]coap_test_demo start");

    xy_coap_demo();

    xy_printf("[COAPdemo]coap_test_demo end");

	g_coaptest_handle = NULL;
	osThreadExit();
}
//can call by  user_app_init
void coap_opencpu_demo_init()
{
    if (g_coaptest_handle != NULL)
    {
        return;
    }
	g_coaptest_handle = osThreadNew ((osThreadFunc_t)(coap_test_demo),NULL,"coap_test_demo",0x1000,osPriorityNormal);
}

#endif
