/** 
* @file        xy_net_api.h
* @brief       tcpip api,dns/socket/ping...
*
* @attention  User must use these APIs in the right process,and prevent state inconsistency caused by instantaneous change

* @warning     Socket related, please see lwip  POSIX Socket API "sockets.h"
*
*/
#pragma once
#include <stdint.h>
#include "lwip/ip_addr.h"

/**
 * @brief  set IPV4 dns server,and will save into NV
 * @ingroup tcpipapi
 * @param pridns   first DNS server,32 bit
 * @param secdns   second DNS server,32 bit
 * @return bool,see @ref  xy_ret_Status_t
 * @note  输入的IP地址为二进制网络字节序
 */
int xy_dns_set(ip4_addr_t *pridns,ip4_addr_t *secdns);

/**
 * @brief  set IPV4 dns server,and will save into NV
 * @ingroup tcpipapi
 * @param pri_dns   first DNS server,such as "114.114.114.114"
 * @param sec_dns   second DNS server,such as "114.114.114.114"
 * @return bool,see  @ref xy_ret_Status_t
 * @note   输入的IP地址为点分文本的IP地址，内部通过inet_pton接口转换为二进制网络字节序的IP地址
 */
int xy_dns_set2(char *pri_dns,char *sec_dns);

/**
 * @brief  阻塞等待PDP激活成功和TCPIP网路成功。由于平台内部串行处理，在收到“+CGEV: ME PDN ACT 0”时，tcpip协议栈的网路尚未配置完成，此时调用该接口时入参不能为0
 * 
 * @param timeout  0，非阻塞态，立即返回
 * @return  bool,see  @ref  xy_ret_Status_t
 * @attention  用户需要特别注意超时时间，一旦超时，表明3GPP无线环境异常，建议客户软重启后再尝试，若多次失败，建议睡眠一段时间后再尝试. \n
 *  若用户在开机后直接调用该接口，建议timeout设置大于30秒，以给3GPP足够的时长进行小区驻留。
 */
int xy_wait_tcpip_ok(int timeout);


