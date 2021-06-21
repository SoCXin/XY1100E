#pragma once

/**
* @file         xy_utils.h
* @brief        一些基础接口声明和OS的扩展API声明
* @attention    用户不得直接调用freertos提供的API，只准调用osal.h和xy_utils.h中声明
*/

#include "softap_macro.h"
#include "sys_debug.h"
#include "at_utils.h"
#include "xy_printf.h"
#include "xy_mem.h"
#include "xy_memmap.h"
#include "osal.h"
#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <stddef.h>
#include <float.h>
#include <stdarg.h>
#include <ctype.h>

/*******************************************************************************
 *                             Macro definitions                               *
 ******************************************************************************/
/**
 * @brief 堆内存申请释放接口，其中xy_malloc_Align/xy_zalloc_Align为cache行对齐申请接口,凡是使用到flash、DMA、核间通信操作的都要注意cache行对齐
 * 
 * @note zalloc内部会执行memset 0的操作，进而会影响性能；对于大内存的申请，务必注意是否需要初始化为0。 \n
 *  一般的，用于存储AT命令的内存申请建议使用xy_zalloc，其他的堆内存申请建议使用xy_malloc。
 */
#define xy_zalloc(ulSize)   	    XY_ZALLOC(ulSize)
#define xy_malloc(ulSize)   		XY_MALLOC(ulSize)
#define xy_zalloc_Align(ulSize)   	XY_ZALLOC_ALIGN(ulSize)
#define xy_malloc_Align(ulSize)   	XY_MALLOC_ALIGN(ulSize)
#define xy_free(mem)				xy_MemFree(mem)

/**
 * @brief use this API to send log,and open it by LogView
 * 
 * @note 用户可以通过AT+NV=SET,LOG,<val>命令来修改输出的log内容，以方便客户调试开发。  \n
 * 其中0表示关闭log输出，常用于产线发货；1表示抓取所有log；2表示仅抓取M3核log，不包含3GPP任何log;3表示仅抓取使用该接口输出的用户log
 * @attention 通过NV修改log输出等级，必须重启后才能生效. \n
 *   该接口不可以在锁中断期间调用，否则会断言。
 * @warning 用户须注意,当内核没有调度时，系统不会输出log！并且该接口不可以在锁中断或者临界区中的调用，否则会断言！
 * 如果初始化时需要输出，可以使用send_debug_str_to_at_uart接口从AT口输出，这个接口在release模式下会立即退出，不会影响产品功耗
 */
#define xy_printf(fmt, args...) xy_log_print(USER_LOG, WARN_LOG, fmt, ##args)

/**
 * @brief use this API to send stream
 * @param buf  码流存放的缓存地址
 * @param len  码流的长度，以字节为单位
 * @return 	   实际输出的码流长度，以字节为单位
 * @attention  该接口不可以在锁中断期间调用，否则会断言。
 */
int xy_print_stream(uint8_t * buf, uint32_t len);

/**
 * @brief system assert API,and will output FILE/LINE and dump ram into flash 
 * @note 用于软件断言。死机后，会导出dump文件以供软件开发定位问题，详情参阅《XY1100 Trace32使用及死机定位指导》\n
 *  当off_debug=0，即debug模式是，断言不会触发重启；当off_debug=1，即release模式时，断言后会尝试导出，超时后触发重启
 * @warning  客户调试过程中遇到断言时，建议通过AT+NV=SET,CLOSEDEBUG,0开启debug模式复现，以抓取死机dump文件。
 */
#define xy_assert(a)	Sys_Assert(a)

/*******************************************************************************
 *                             Type definitions                                *
 ******************************************************************************/
/**
 * @brief Status code values returned by XinYi-OpenCpu functions.
*/
typedef enum {
	XY_OK					        =  0, 		
	XY_ERR					      = -1, 		  //normal err
	XY_Err_Timeout			  = -2, 		  // Operation not completed within the timeout period.
	XY_Err_NoConnected		= -3, 		  // tcpip newwork unusable,such as  PDP not activate
	XY_Err_Parameter		  = -4, 		  // input parameter error.
	XY_Err_NoMemory 		  = -5, 		  // System is out of memory: it was impossible to allocate or reserve memory for the operation.
	XY_Err_NotAllowed		  = -6, 		  // api not allowed,such as SIM card not insert,and do something about 3GPP operation.
	XY_Err_LowVbat			  = -7, 		  //low Vbat,can not do flash write
	XY_Err_Reserved 		  = 0x7FFFFFFF	
} xy_ret_Status_t;

/**
 * @brief  由于3GPP相关线程优先级较高，进而用户可用的优先级只能在该枚举范围内，建议默认使用XY_OS_PRIO_NORMAL
*/
typedef enum
{
  XY_OS_PRIO_LOW = osPriorityLow,
  XY_OS_PRIO_LOW1,
  XY_OS_PRIO_LOW2 ,
  XY_OS_PRIO_NORMAL = osPriorityNormal,
  XY_OS_PRIO_NORMAL1,
  XY_OS_PRIO_NORMAL2,
  XY_OS_PRIO_HIGH = osPriorityHigh,
  XY_OS_PRIO_HIGH1,
  XY_OS_PRIO_HIGH2
} xy_os_Prio_t;

/*******************************************************************************
 *                       Global function declarations                          *
 ******************************************************************************/
/**
  * @brief convort HEX ASCII string to binary stream 
  * @param src		 string ASCII data  address,such as "AB23"
  * @param src_len	 string ASCII data len
  * @param dst		 binary stream  address,such as 0XAB23
  * @param dst_size	 binary stream memory len
  * @return successful binary stream len
  * @note  such as "AB23"---->AB23(2 BYTES)
  */
int hexstr2bytes(char* src, int src_len, char* dst, int dst_size);

/**
  * @brief  convert  binary stream   to  HEX ASCII string
  * @param src		 binary stream address,such as 0XAB23
  * @param src_len	 binary stream len
  * @param dst		 string ASCII data,such as "AB23"
  * @param dst_size	 string ASCII data len
  * @return successful string ASCII data len
  * @note  such as  0XAB23(2 BYTES)---->"AB23" 
  */
int bytes2hexstr(unsigned char* src, signed long src_len, char* dst, signed long dst_size);

/**
 * @brief 赋值随机因子，必须保证每次上电后赋值的随机因子皆不同
 * 
 * @return  bool,see  @ref  xy_ret_Status_t.
 * @note 
 */
int xy_srand(uint32_t key);

/**
 * @brief xy_srand随机值种子设置，xy_rand获取随机值.
 * @return uint32_t 
 * @note  芯翼平台在开机main函数中读取RTC的count计数器作为种子；一旦联网后获取到世界时间，则用最新的世界时间作为种子
 * @warning  刚开机时获取RTC的计数器作为种子，可能造成每次开机种子值一样，建议客户在PDP激活后再播种
 */
int xy_rand();

/**
 * @brief Get the GUID，可以作为芯片的唯一ID,常用于云平台的终端唯一ID识别
 * 
 * @return  16字节的字符串首地址，调用者无需释放该地址
 * @note 
 */
unsigned char *xy_getGUID();

/**
 * @brief 四字节的翻转
 * 
 * @return  bool,see  @ref  xy_ret_Status_t.
 * @note 
 */
int xy_rearrange_DWORD(unsigned char *data, int data_len);

/**
 * @brief checksum计算，返回的是32位值
 * 
 * @return  32位结果
 * @note 
 */
unsigned int xy_chksum(const void *dataptr, int len);

/**
 * @brief 将当前队列中缓存的消息体释放，以清空队列
 * @param pMsgQueueId  [IN] ID
 * @attention 该接口仅适用于消息传递的是消息体指针情况，若不是，直接调用osMessageQueueReset即可 \n
 * 通常与osMessageQueueNew、osMessageQueuePut、osMessageQueueGet捆绑使用。
 */
void xy_queue_clear(osMessageQueueId_t pMsgQueueId);

/**
 * @brief 先读取当前队列中缓存的所有消息体，并释放内存，再删除该消息队列
 * @param pMsgQueueId  [IN] ID
 * @attention 该接口仅适用于消息传递的是消息体指针情况，若不是，直接调用osMessageQueueDelete即可 \n
 * 通常与osMessageQueueNew、osMessageQueuePut、osMessageQueueGet捆绑使用。
 */
void xy_queue_Delete(osMessageQueueId_t pMsgQueueId);

/**
 * @brief    延迟若干tick后继续执行当前线程，等同于sleep。目前单位为毫秒
 * @warning  该接口区别与osDelay接口，该接口内部临时屏蔽standby睡眠的影响，确保准时。  \n
 *           该接口不建议客户使用，如需要，请联系FAE!
 */
osStatus_t osDelay_stict(uint32_t ticks);


