 /**
* @file     xy_api.h
* @brief    芯翼提供给外部客户使用的所有头文件，客户只能使用对应头文件中的声明，不得私自调用其他接口
* @note  
		xy_api.h		芯翼平台提供的总的API接口，客户开发自己的源代码时，仅需包含该接口就可以使用对外的所有声明 \n
		xy_sys_hook.h	芯翼平台提供给客户实现的系统级HOOK函数接口，具体实现参看xy_sys_hook.c \n
		xy_app_hook.h   芯翼平台提供给客户实现的应用级HOOK函数接口，具体实现参看xy_app_hook.c \n
		xy_memmap.h 	内存和flash的配置相关，用户禁止修改 \n
		osal.h			操作系统适配API \n
		xy_utils.h		芯翼提供的工具类接口，如打印、断言、字符串处理等 \n
		xy_system.h 	系统级接口，如软重启、睡眠等 \n
		xy_flash.h		flash相关接口 \n
		xy_rtc_api.h	RTC硬定时相关接口 \n
		xy_at_api.h		AT命令二次开发相关API接口 \n
		xy_ps_api.h 	3GPP相关的API接口 \n
		xy_ps_ext_api.h 3GPP相关的订阅回调API接口 \n
		sockets.h		lwip的socket套接字接口 \n
		xy_net_api.h	tcpip基础API接口，如DNS等 \n
		xy_mqtt_api.h	MQTT的API接口 \n
		xy_coap_api.h	coap相关的API接口 \n
		xy_cis_api.h 	onenet的API接口 \n
		xy_cdp_api.h		CDP的API接口 \n
		xy_fota.h		FOTA相关的API接口，仅限于芯翼自研的FOTA功能 \n

*/

#pragma once

#include "xy_utils.h"
#include "osal.h"
#include "xy_memmap.h"
#include "xy_sys_hook.h"
#include "xy_app_hook.h"
#include "xy_system.h"
#include "xy_flash.h"
#include "xy_rtc_api.h"
#include "xy_at_api.h"
#include "xy_ps_api.h"
#include "xy_ps_ext_api.h"


//lwip socket inc
#include "lwip/sockets.h"
#include "lwip/netdb.h"
#include "xy_net_api.h"

#if XY_DTLS
#include "xy_aes.h"
#endif

#if MQTT
#include "xy_mqtt_api.h"
#endif

#if LIBCOAP
#include "xy_coap_api.h"
#endif

#if MOBILE_VER
#include "xy_cis_api.h"
#include "xy_cis_ext_api.h"
#endif

#if TELECOM_VER
#include "xy_cdp_api.h"
#endif

#if XY_FOTA
#include "xy_fota.h"
#endif


//hal inc
#include "hal_adc.h"
#include "hal_gpio.h"
#include "hal_i2c.h"
#include "hal_spi.h"
#include "hal_tim.h"
#include "hal_uart.h"
#include "hal_def.h"
#include "hal_cortex.h"
#include "hal_csp.h"





