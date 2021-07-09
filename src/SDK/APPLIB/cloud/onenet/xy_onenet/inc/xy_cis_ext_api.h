/** 
* @file        xy_cis_ext_api.h
* @brief       简化API调用流程，仅支持一个object和resource，提供基本的注册、去注册、数据收发功能。
*              
* @attention   建议普通用户使用该头文件接口进行相关开发,具体可参看cis_opencpu_simple_demo.c。\n
*  服务器的搭建，请按照文档《芯翼XY1100_中移云开发及调试指南_V1.1》操作
*/
#ifndef _CIS_EXT_API_H
#define _CIS_EXT_API_H

#include <cis_api.h>


/**
* @brief 简化的API接口的默认配置，仅支持一个object和resource，用户可以自行修改lifetime时长和ID值
*/
#define  CIS_API_LIFT_TIME        3000   //second
#define  CIS_API_RESOURCE_ID      5850
#define  CIS_API_OBJ_ID           3311


/**
* @brief cis event message queue
*/
typedef struct _cis_event_msg{
    int     type;        ///< message type
    int     valueType;   ///< data type
    char    data[0];     ///< data content address
}cis_event_msg;

/**
* @brief 下行报文write类型回调函数指针
*/
typedef void (*cis_write_type_downstream_callback)(char *data, int data_len);

/**
* @brief 用户动态注册下行数据回调函数
*/
extern cis_write_type_downstream_callback g_cis_write_type_downstream_cb;
/**
 *@ingroup cisapi
 *@brief CIS简化版注册接口
 *@param timeout        [IN]     waiting time for register
 *@retval #int    XY_OK is enable to send data, XY_ERR is unenable
 */
int cis_ext_reg(int timeout);

/**
 *@ingroup cisapi
 *@brief CIS简化版去注册接口
 *@retval #int    XY_OK is enable to send data, XY_ERR is unenable
 */
int cis_ext_dereg();

/**
 *@ingroup cisapi
 *@brief CIS简化版发送数据接口，同步接口，会阻塞等待应答报文
 *@param data         		[IN]     data content
 *@param data_len           [IN]     data len
 *@param timeout         	[IN]     waitting time
 *@retval #int    XY_OK is success, XY_ERR is failed
 */
int cis_ext_send(char *data, int data_len, int timeout);

/**
 *@ingroup cisapi
 *@brief CIS简化版发送数据接口，同步接口，会阻塞等待应答报文,报文中携带快速释放连接(RAI)标志位
 *@param data         		[IN]     data content
 *@param data_len           [IN]     data len
 *@param timeout         	[IN]     waitting time
 *@retval #int    XY_OK is success, XY_ERR is failed
 *@warning   当用户认为后续没有报文交互需求时，调用该接口来伴随触发RAI，以降低功耗。必须设置出厂NV参数close_rai为1，否则在用户释放最后一把锁后会由芯翼平台自触发RAI
 */
int cis_ext_send_with_rai(char *data, int data_len, int timeout);

/**
 *@ingroup cisapi
 *@brief 设置处理CIS下行数据的回调函数
 *@param cis_write_type_downstream_callback	 [IN]  callback function
 */
void  cis_set_write_type_downstream_cb(cis_write_type_downstream_callback downstream_cb);

#endif

