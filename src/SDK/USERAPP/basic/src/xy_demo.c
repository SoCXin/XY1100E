/** 
* @file        xy_demo.c
* @ingroup     hook
* @brief       芯翼平台DEMO函数汇总
* @note
*/
#include  "xy_demo.h"

#if DEMO_TEST

extern void user_task_demo_init();
extern void user_task_demo2_init();
extern void user_task_demo3_init();
extern void user_task_volatile_demo_init();
#if TELECOM_VER
extern void cdp_opencpu_demo_init();
#endif
#if MOBILE_VER
extern void cis_demo_task_init();
extern void cis_simple_demo_task_init();
#endif
#if LIBCOAP
extern void coap_opencpu_demo_init();
#endif
#if MQTT
extern void mqtt_opencpu_demo_init();
#endif
extern void xy_ps_ext_demo_init();
extern void xy_socket_demo_init();
#if ASYNC_SKT_ENABLE
extern void app_tcpip_client_init();
#endif

demo_proc g_demo_proc_list[] = {
    /*驱动[1-80]*/
    
    /*基础平台[81-160]*/
    {81, xy_ps_ext_demo_init},
    {84, xy_socket_demo_init},
#if ASYNC_SKT_ENABLE
    {85, app_tcpip_client_init},
#endif
    {86, user_task_demo_init},
    {87, user_task_demo2_init},
    {88, user_task_demo3_init},
    {89, user_task_volatile_demo_init},
    /*上层业务平台[160-254]*/
#if TELECOM_VER
    {160, cdp_opencpu_demo_init},
#endif
#if MOBILE_VER
    {161, cis_demo_task_init},
    {162, cis_simple_demo_task_init},
#endif
#if LIBCOAP
    {163, coap_opencpu_demo_init},
#endif
#if MQTT
    {164, mqtt_opencpu_demo_init},
#endif
};

uint8_t g_demo_proc_list_len = sizeof(g_demo_proc_list)/sizeof(demo_proc);

#endif //DEMO_TEST
