/**
 * @file xy_lib_stub.c
 * @brief 用于解决封库模块与外部模块的耦合,外部模块的头文件包含与代码实现均用宏控制
 * @version 1.0
 * @date 2021-02-26
 * @copyright Copyright (c) 2021  芯翼信息科技有限公司
 * 
 */

/*******************************************************************************
 *					        Include header files							   *
 ******************************************************************************/
#include "xy_lib_stub.h"
#include "softap_macro.h"
#include "xy_utils.h"
#if AT_SOCKET
#include "at_socket.h"
#endif //AT_SOCKET

/*******************************************************************************
 *						Global function implementations 					   *
 ******************************************************************************/
void do_xdsend_cmd(char *at_data)
{
#if AT_SOCKET
    /** 中移socket灌包命令对处理时间有要求，xdsend命令需尽快处理*/
    if (!at_strncasecmp(at_data, "AT+XDSEND=", strlen("AT+XDSEND=")))
    {
        at_XDSEND_req(at_data + strlen("AT+XDSEND="));
        return;
    }
#else
    (void)at_data;
    return;
#endif
}

int find_xdsend_cmd(char *at_cmd)
{
#if AT_SOCKET
    char *str = at_cmd;
    while (*str == '\r' || *str == '\n')
        str++;
    return !memcmp(str, "AT+XDSEND=", strlen("AT+XDSEND="));
#else
    (void)at_cmd;
    return 0;
#endif
}