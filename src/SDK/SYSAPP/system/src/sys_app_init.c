#include "rtc_tmr.h"
#include "hw_utc.h"
#include "ipcmsg.h"
#include "low_power.h"
#include "patch.h"
#include "hw_csp.h"
#include "dma.h"
#include "hw_prcm.h"
#include "watchdog.h"
#include "at_ctl.h"
#include "at_worklock.h"
#include "xy_hwi.h"
#include "xy_system.h"
#include "cloud_utils.h"
#include "xy_utils.h"
#include "xy_proxy.h"
#include "inter_core_msg.h"
#include "netif_mgmt.h"
#include "lwip/tcpip.h"
#include "xy_log_send.h"
#include "sys_init.h"
#include "at_uart.h"
#include "xy_watchdog.h"
#include "net_app_resume.h"
#if ABUP_FOTA
#include "abup_fota.h"
#endif
#if XY_HTTP
#include "at_http.h"
#endif

#if TELECOM_VER	
#include "agent_tiny_demo.h"
#endif

#if BLE_ENABLE
#include "ble_config.h"
#endif

#if AT_SOCKET
#include "at_socket_context.h"
#endif

#if XY_DM
extern void dm_ctl_init();
#endif

#if MOBILE_VER
#include "at_onenet.h"
#endif

#if MQTT
extern void at_mqtt_init();
#endif

#if LIBCOAP
extern void at_coap_init();
#endif

extern void shm_msg_init();

extern void passthr_init();

void network_task_init()
{
    static int network_task_inited = 0;

    if(network_task_inited == 0)//只初始化一次
    {
        netif_mgmt_init();

        tcpip_init(NULL,NULL);
#if AT_SOCKET
        socket_init();
#endif //AT_SOCKET

#if XY_HTTP
        at_http_init();
#endif //XY_HTTP

#if XY_DM
        dm_ctl_init();
#endif //XY_DM

        cloud_fota_init();
        cloud_resume_init();
#if TELECOM_VER
        cdp_lwm2m_init();
#endif //TELECOM_VER

#if MOBILE_VER
        at_onenet_init();
#endif //MOBILE_VER

#if MQTT
        at_mqtt_init();
#endif //MQTT

#if LIBCOAP
        at_coap_init();
#endif //LIBCOAP

#if ABUP_FOTA
        abup_init();
#endif //ABUP_FOTA

        network_task_inited = 1;
    }
}

int sys_app_init_before_OFFTIME()
{
	xy_log_task();

	xy_user_dog_set(g_softap_fac_nv->user_dog_time * 60);

	if (g_softap_fac_nv->close_at_uart == 0)
		creat_AT_uart_rx_task();

	icm_task_init();

	worklock_init();

        shm_msg_init();

        at_init();

	xy_proxy_init();

	dma_transfer_mutex = osMutexNew(osMutexPrioInherit);

	memset((void *)DSP_LPM_MSG, 0, sizeof(LPM_MSG_T));

#if BLE_ENABLE
	if(g_softap_fac_nv->chip_with_ble)
	{
		ble_task_init();
		chip_work_type_init();
	}
#endif

	return 0;
}

int sys_app_init_after_OFFTIME()
{
	rtc_task_init();

        passthr_init();

        if (g_softap_fac_nv->work_mode == 0) //非单核模式，初始化网络业务线程
        {
                network_task_init();
    }

	wdt_task_init();
	
	return 0;
}
