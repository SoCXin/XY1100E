#ifndef _SOFTAP_MACRO_H
#define _SOFTAP_MACRO_H

#ifndef __XYFILE__
#define __XYFILE__ "xinyi"
#endif

/*user can close these macro by own platform needs*/


#define  SELF_TEST            0    //platform test
#define  IS_DSP_CORE          0    //1:DSP+freertos;0:M3+liteos
#define  SAMPLE_RATE_MODE     1    //Sample Rate: 30.72MHz +- 1.92MHz

#if XY_FOTA
#define  CONFIG_FEATURE_FOTA  1    //公有云FOTA的开关
#endif

#define  NET_MAX_USER_DATA_LEN      1400   //socket/onenet/cdp/ctwing/http...data must  < 1400
#define  PS_PDP_CID_MAX             2
#define  SOCK_RECV_TIMEOUT          1 //seconds
#define  SOCK_RCV_BUF_MAX           4000  //jinka recv buff,BYTES
#define  REMOTE_SERVER_LEN          100  // REMOTE_SERVER_LEN include ip len or domain len

//for common git of TCPIP
#define  TCPIP_THREAD_STACK_SIZE    0x600   //tcpip thread stack size, is different than its in dsp core 
#define  PING_THREAD_STACK_SIZE     0x400   //ping thread stack size, is different than its in dsp core 
#define  PING6_THREAD_STACK_SIZE    0xc00   //ipV6 ping thread stack size, is different than its in dsp core 


#define  WDT_TASK_SEC        (2) //watch dog will be feed every WDT_TASK_SEC

#define  PLL_DEBUG              0

#define  DSP_DYNAMIC_ON_OFF     1

#define INTERCORE_DEBUG 		0	// intercore异常定位代码, 仅bug定位需要

#define WAIT_UNTIL_ATEND     0   //控制AT串口收命令模式。0：默认标准版本 1：死等'\r'


#endif 

