/** 
* @file        xy_cis_api.h
* @brief       提供CIS基础功能的API，用户可根据需要自行调用相关接口实现与CIS服务器通讯，推荐了解CIS交互流程用户使用，提供保存恢复和快速RAI功能，具体参考cis_opencpu_demo.c。
* @attention   对于没有特殊要求的客户，建议使用xy_cis_ext_api.h接口来简化开发，具体可参看cis_opencpu_simple_demo.c。 \n
*  服务器的搭建，请按照文档《芯翼XY1100_中移云开发及调试指南_V1.1》操作
*/
#ifndef _CIS_API_H
#define _CIS_API_H

#include <cis_api.h>

/**
* @brief cis packet message queue
*/
typedef struct _cis_pkt_msg{
    int     type;       ///< message type
    int     flag;       ///< uri flag
    int     objId;      ///< uri object id
    int     insId;      ///< uri instance id
    int     resId;      ///< uri resource id
    int     evtId;      ///< event id
    int     index;      ///< message index
    int     data_len;   ///< message data length
    int     valueType;  ///< message data type
    char    data[0];    ///< message data content address
}cis_pkt_msg;

/**
* @brief 下行报文回调函数指针
*/
typedef void (*cis_downstream_callback)(et_callback_type_t type, void* context,cis_uri_t* uri,cis_mid_t mid, cis_evt_t eid, int
valueType, char* value, int valueLen);

/**
* @brief 内部全局声明，用户无需关注
*/
extern cis_downstream_callback g_cis_downstream_cb;
/*******************************************************************************
 *							   Type definitions 							   *
 ******************************************************************************/
/**
 *@brief 设置CIS 服务器IP地址、端口号、authcode.
 *@param ip_addr_str        [IN]     server ip
 *@param port     			[IN]     server port
 *@param ip_addr_str		[IN]	 device authcode on cloud server
 *@retval #int          XY_OK is succeed, or the error number is failed.
 */
int  cis_cloud_setting(char *ip_addr_str, int port, char *authcode);

/**
 *@ingroup cisapi
 *@brief 初始化CIS本地配置,开启云保存功能之后，会先尝试从NV恢复云会话数据
 *@param ip        [IN]     server ip
 *@param port      [IN]     server port
 *@param is_bs	   [IN]	 	bootstrap is enable or not
 *@param auth	   [IN]	 	device authcode on cloud server
 *@retval #int    success: XY_OK      fail:     XY_Err_NoConnected    网络未连接
 *                                              XY_Err_Parameter      参数错误
 *                                              XY_ERR                业务处理错误
 */
int  cis_create(char *ip, unsigned int port, int is_bs, char* auth);

/**
 *@ingroup cisapi
 *@brief 删除CIS本地配置，开启云保存功能之后，会先尝试从NV恢复云会话数据
 *@retval #int    success: XY_OK      fail:    XY_ERR               业务处理错误
 */
int  cis_delete(void);

/**
 *@ingroup cisapi
 *@brief 注册CIS，开启云保存功能之后，会先尝试从NV恢复云会话数据
 *@param lifetime        [IN]     onenet context lifetime
 *@retval #int    success: XY_OK         fail:  XY_Err_NoConnected   网络未连接
 *                                              XY_Err_Parameter     参数错误
 *                                              XY_Err_NotAllowed    业务线程已存在
 *                                              XY_ERR               业务处理错误
 */
int  cis_reg(int lifetime);

/**
 *@ingroup cisapi
 *@brief 添加object，开启云保存功能之后，会先尝试从NV恢复云会话数据
 *@param objID        [IN]     object id
 *@param insCount     [IN]     object instance count
 *@param insBitmap    [IN]     object instance bitmap
 *@param attrCount    [IN]     attribute count
 *@param actCount     [IN]     action count
 *@retval #int    success: XY_OK        fail:   XY_Err_NoConnected   网络未连接
 *                                              XY_Err_Parameter     参数错误
 *                                              XY_Err_NotAllowed    业务线程不存在，需要先注册
 */
int  cis_addobj(int objId, int insCount, char* insBitmap, int attrCount, int actCount);

/**
 *@ingroup cisapi
 *@brief 删除object，开启云保存功能之后，会先尝试从NV恢复云会话数据
 *@param objID        [IN]     object id
 *@retval #int    success: XY_OK        fail:   XY_Err_NoConnected   网络未连接
 *                                              XY_Err_Parameter     参数错误
 *                                              XY_Err_NotAllowed    业务线程不存在，需要先注册
 */
int  cis_delobj(int objId);

/**
 *@ingroup cisapi
 *@brief 发送上行数据,同步接口，会阻塞等待收到应答报文或者超时，开启云保存功能之后，会先尝试从NV恢复云会话数据，推荐使用
 *@param msg_id        [IN]     message id
 *@param objId         [IN]     object id
 *@param insId         [IN]     instance id
 *@param resId         [IN]     resource id
 *@param valueType     [IN]     data type
 *@param len           [IN]     data len
 *@param value         [IN]     data content
 *@param index         [IN]     data index
 *@param flag          [IN]     index flag
 *@param ackid         [IN]     send data with ack id
 *@retval #int    success: XY_OK        fail:   XY_Err_NoConnected   网络未连接
 *                                              XY_Err_Parameter     参数错误
 *                                              XY_Err_NotAllowed    业务线程不存在，需要先注册
 *                                              XY_ERR               业务处理错误
 */
int  cis_notify_sync(int msg_id, int objId, int insId, int resId, int valueType, int len, char* value, int index, int flag, int
ackid);

/**
 *@ingroup cisapi
 *@brief 发送上行数据,报文中携带快速释放连接(RAI)标志位,同步接口，会阻塞等待收到应答报文或者超时，开启云保存功能之后，会先尝试从NV恢复云会话数据，推荐使用
 *@param msg_id        [IN]     message id
 *@param objId         [IN]     object id
 *@param insId         [IN]     instance id
 *@param resId         [IN]     resource id
 *@param valueType     [IN]     data type
 *@param len           [IN]     data len
 *@param value         [IN]     data content
 *@param index         [IN]     data index
 *@param flag          [IN]     index flag
 *@param ackid         [IN]     send data with ack id
 *@retval #int    success: XY_OK        fail:   XY_Err_NoConnected   网络未连接
 *                                              XY_Err_Parameter     参数错误
 *                                              XY_Err_NotAllowed    业务线程不存在，需要先注册
 *                                              XY_ERR               业务处理错误
 *@warning   当用户认为后续没有报文交互需求时，调用该接口来伴随触发RAI，以降低功耗。必须设置出厂NV参数close_rai为1，否则在用户释放最后一把锁后会由芯翼平台自触发RAI
 */
int  cis_notify_sync_with_rai(int msg_id, int objId, int insId, int resId, int valueType, int len, char* value, int index, int flag, int
ackid);

/**
 *@ingroup cisapi
 *@brief 发送上行数据，异步接口，发完数据不阻塞等待返回结果，需要在接受数据的线程处理相关应答结果，开启云保存功能之后，会先尝试从NV恢复云会话数据，不推荐使用
 *@param msg_id        [IN]     message id
 *@param objId         [IN]     object id
 *@param insId         [IN]     instance id
 *@param resId         [IN]     resource id
 *@param valueType     [IN]     data type
 *@param len           [IN]     data len
 *@param value         [IN]     data content
 *@param index         [IN]     data index
 *@param flag          [IN]     index flag
 *@param ackid         [IN]     send data with ack id
 *@retval #int    success: XY_OK        fail:   XY_Err_NoConnected   网络未连接
 *                                              XY_Err_Parameter     参数错误
 *                                              XY_Err_NotAllowed    业务线程不存在，需要先注册
 *                                              XY_ERR               业务处理错误
 */
int  cis_notify_asyn(int msg_id, int objId, int insId, int resId, int valueType, int len, char* value, int index, int flag, int
ackid);
/**
 *@ingroup cisapi
 *@brief 发送上行数据,报文中携带快速释放连接(RAI)标志位,异步接口，发完数据不阻塞等待返回结果，需要在接受数据的线程处理相关应答结果，不推荐使用
 *@param msg_id        [IN]     message id
 *@param objId         [IN]     object id
 *@param insId         [IN]     instance id
 *@param resId         [IN]     resource id
 *@param valueType     [IN]     data type
 *@param len           [IN]     data len
 *@param value         [IN]     data content
 *@param index         [IN]     data index
 *@param flag          [IN]     index flag
 *@param ackid         [IN]     send data with ack id
 *@retval #int    success: XY_OK        fail:   XY_Err_NoConnected   网络未连接
 *                                              XY_Err_Parameter     参数错误
 *                                              XY_Err_NotAllowed    业务线程不存在，需要先注册
 *                                              XY_ERR               业务处理错误
 *@warning   当用户认为后续没有报文交互需求时，调用该接口来伴随触发RAI，以降低功耗。必须设置出厂NV参数close_rai为1，否则在用户释放最后一把锁后会由芯翼平台自触发RAI
 */
int  cis_notify_asyn_with_rai(int msg_id, int objId, int insId, int resId, int valueType, int len, char* value, int index, int flag, int
ackid);

/**
 *@ingroup cisapi
 *@brief 应答 服务器read 请求，开启云保存功能之后，会先尝试从NV恢复云会话数据，
 *@param msg_id        [IN]     message id
 *@param result        [IN]     read data result
 *@param objId         [IN]     object id
 *@param insId         [IN]     instance id
 *@param resId         [IN]     resource id
 *@param valueType     [IN]     data type
 *@param len           [IN]     data len
 *@param value         [IN]     data content
 *@param index         [IN]     data index
 *@param flag          [IN]     index flag
 *@retval #int    success: XY_OK        fail:   XY_Err_NoConnected   网络未连接
 *                                              XY_Err_Parameter     参数错误
 *                                              XY_Err_NotAllowed    业务线程不存在，需要先注册
 *                                              XY_ERR               业务处理错误
 */
int  cis_read_rsp(int msg_id, int result, int objId, int insId, int resId, int valueType, int len, char* value, int index, int flag);
/**
 *@ingroup cisapi
 *@brief 应答 服务器read 请求,报文中携带快速释放连接(RAI)标志位，开启云保存功能之后，会先尝试从NV恢复云会话数据，
 *@param msg_id        [IN]     message id
 *@param result        [IN]     read data result
 *@param objId         [IN]     object id
 *@param insId         [IN]     instance id
 *@param resId         [IN]     resource id
 *@param valueType     [IN]     data type
 *@param len           [IN]     data len
 *@param value         [IN]     data content
 *@param index         [IN]     data index
 *@param flag          [IN]     index flag
 *@retval #int    success: XY_OK        fail:   XY_Err_NoConnected   网络未连接
 *                                              XY_Err_Parameter     参数错误
 *                                              XY_Err_NotAllowed    业务线程不存在，需要先注册
 *                                              XY_ERR               业务处理错误
 *@retval *@warning   当用户认为后续没有报文交互需求时，调用该接口来伴随触发RAI，以降低功耗。必须设置出厂NV参数close_rai为1，否则在用户释放最后一把锁后会由芯翼平台自触发RAI
 */
int  cis_read_rsp_with_rai(int msg_id, int result, int objId, int insId, int resId, int valueType, int len, char* value, int index, int flag);

/**
 *@ingroup cisapi
 *@brief 应答服务器write请求，开启云保存功能之后，会先尝试从NV恢复云会话数据，
 *@param msg_id        [IN]     message id
 *@param result        [IN]     write result
 *@retval #int    success: XY_OK        fail:   XY_Err_NoConnected   网络未连接
 *                                              XY_Err_Parameter     参数错误
 *                                              XY_Err_NotAllowed    业务线程不存在，需要先注册
 *                                              XY_ERR               业务处理错误
 */
int  cis_write_rsp(int msg_id, int result);

/**
 *@ingroup cisapi
 *@brief 应答服务器write请求,报文中携带快速释放连接(RAI)标志位，开启云保存功能之后，会先尝试从NV恢复云会话数据，
 *@param msg_id        [IN]     message id
 *@param result        [IN]     write result
 *@retval #int    success: XY_OK        fail:   XY_Err_NoConnected   网络未连接
 *                                              XY_Err_Parameter     参数错误
 *                                              XY_Err_NotAllowed    业务线程不存在，需要先注册
 *                                              XY_ERR               业务处理错误
 *@warning   当用户认为后续没有报文交互需求时，调用该接口来伴随触发RAI，以降低功耗。必须设置出厂NV参数close_rai为1，否则在用户释放最后一把锁后会由芯翼平台自触发RAI
 */
int  cis_write_rsp_with_rai(int msg_id, int result);

/**
 *@ingroup cisapi
 *@brief 应答服务器execute请求，开启云保存功能之后，会先尝试从NV恢复云会话数据，
 *@param msg_id        [IN]     message id
 *@param result        [IN]     execute result
 *@retval #int    success: XY_OK        fail:   XY_Err_NoConnected   网络未连接
 *                                              XY_Err_Parameter     参数错误
 *                                              XY_Err_NotAllowed    业务线程不存在，需要先注册
 *                                              XY_ERR               业务处理错误
 */
int  cis_execute_rsp(int msg_id, int result);
/**
 *@ingroup cisapi
 *@brief 应答服务器execute请求,报文中携带快速释放连接(RAI)标志位，开启云保存功能之后，会先尝试从NV恢复云会话数据，
 *@param msg_id        [IN]     message id
 *@param result        [IN]     execute result
 *@retval #int    success: XY_OK        fail:   XY_Err_NoConnected   网络未连接
 *                                              XY_Err_Parameter     参数错误
 *                                              XY_Err_NotAllowed    业务线程不存在，需要先注册
 *                                              XY_ERR               业务处理错误
 *@warning   当用户认为后续没有报文交互需求时，调用该接口来伴随触发RAI，以降低功耗。必须设置出厂NV参数close_rai为1，否则在用户释放最后一把锁后会由芯翼平台自触发RAI
 */
int  cis_execute_rsp_with_rai(int msg_id, int result);

/**
 *@ingroup cisapi
 *@brief 应答服务器observe请求，开启云保存功能之后，会先尝试从NV恢复云会话数据，
 *@param msg_id        [IN]     message id
 *@param result        [IN]     observe result
 *@retval #int    success: XY_OK        fail:   XY_Err_NoConnected   网络未连接
 *                                              XY_Err_Parameter     参数错误
 *                                              XY_Err_NotAllowed    业务线程不存在，需要先注册
 *                                              XY_ERR               业务处理错误
 */
int  cis_observe_rsp(int msg_id, int result);

/**
 *@ingroup cisapi
 *@brief 应答服务器discover请求,报文中携带快速释放连接(RAI)标志位，开启云保存功能之后，会先尝试从NV恢复云会话数据，
 *@param msg_id        [IN]     message id
 *@param objId         [IN]     object id
 *@param result        [IN]     discover result
 *@param length        [IN]     response data length
 *@param result        [IN]     response data content
 *@retval #int    success: XY_OK        fail:   XY_Err_NoConnected   网络未连接
 *                                              XY_Err_Parameter     参数错误
 *                                              XY_Err_NotAllowed    业务线程不存在，需要先注册
 *                                              XY_ERR               业务处理错误
 *@warning   当用户认为后续没有报文交互需求时，调用该接口来伴随触发RAI，以降低功耗。必须设置出厂NV参数close_rai为1，否则在用户释放最后一把锁后会由芯翼平台自触发RAI
 */
int  cis_observe_rsp_with_rai(int msg_id, int result);

/**
 *@ingroup cisapi
 *@brief 应答服务器discover请求，开启云保存功能之后，会先尝试从NV恢复云会话数据，
 *@param msg_id        [IN]     message id
 *@param objId         [IN]     object id
 *@param result        [IN]     discover result
 *@param length        [IN]     response data length
 *@param result        [IN]     response data content
 *@retval #int    success: XY_OK        fail:   XY_Err_NoConnected   网络未连接
 *                                              XY_Err_Parameter     参数错误
 *                                              XY_Err_NotAllowed    业务线程不存在，需要先注册
 *                                              XY_ERR               业务处理错误
 */
int  cis_discover_rsp(int msg_id, int objId, int result, int length, char* valueStr);

/**
 *@ingroup cisapi
 *@brief 应答服务器discover请求,报文中携带快速释放连接(RAI)标志位，开启云保存功能之后，会先尝试从NV恢复云会话数据，
 *@param msg_id        [IN]     message id
 *@param result        [IN]     set parameter result
 *@retval #int    success: XY_OK        fail:   XY_Err_NoConnected   网络未连接
 *                                              XY_Err_Parameter     参数错误
 *                                              XY_Err_NotAllowed    业务线程不存在，需要先注册
 *                                              XY_ERR               业务处理错误
 *@warning   当用户认为后续没有报文交互需求时，调用该接口来伴随触发RAI，以降低功耗。必须设置出厂NV参数close_rai为1，否则在用户释放最后一把锁后会由芯翼平台自触发RAI
 */
int  cis_discover_rsp_with_rai(int msg_id, int objId, int result, int length, char* valueStr);

/**
 *@ingroup cisapi
 *@brief 应答服务器设置策略参数请求，收到服务器SETPARAMS类型报文，才需调用，开启云保存功能之后，会先尝试从NV恢复云会话数据，
 *@param msg_id        [IN]     message id
 *@param result        [IN]     set parameter result
 *@retval #int    success: XY_OK        fail:   XY_Err_NoConnected   网络未连接
 *                                              XY_Err_Parameter     参数错误
 *                                              XY_Err_NotAllowed    业务线程不存在，需要先注册
 *                                              XY_ERR               业务处理错误
 */
int  cis_rsp_withparam(int msg_id, int result);
/**
 *@ingroup cisapi
 *@brief 应答服务器设置策略参数请求,报文中携带快速释放连接(RAI)标志位,收到服务器SETPARAMS类型报文，才需调用，开启云保存功能之后，会先尝试从NV恢复云会话数据，
 *@param msg_id        [IN]     message id
 *@param result        [IN]     set parameter result
 *@retval #int    success: XY_OK        fail:   XY_Err_NoConnected   网络未连接
 *                                              XY_Err_Parameter     参数错误
 *                                              XY_Err_NotAllowed    业务线程不存在，需要先注册
 *                                              XY_ERR               业务处理错误
 *@warning   当用户认为后续没有报文交互需求时，调用该接口来伴随触发RAI，以降低功耗。必须设置出厂NV参数close_rai为1，否则在用户释放最后一把锁后会由芯翼平台自触发RAI
 */
int  cis_rsp_withparam_with_rai(int msg_id, int result);

/**
 *@ingroup cisapi
 *@brief 更新CIS lifetime时间，开启云保存功能之后，会先尝试从NV恢复云会话数据，
 *@param lifetime        [IN]     onenet context lifetime
 *@param withObjFlag     [IN]     update object or not
 *@retval #int    success: XY_OK        fail:   XY_Err_NoConnected   网络未连接
 *                                              XY_Err_Parameter     参数错误
 *                                              XY_Err_NotAllowed    业务线程不存在，需要先注册
 *                                              XY_ERR               业务处理错误
 */
int  cis_updatelife(int lifetime, bool withObjFlag);
/**
 *@ingroup cisapi
 *@brief 更新CIS lifetime时间,报文中携带快速释放连接(RAI)标志位，开启云保存功能之后，会先尝试从NV恢复云会话数据，
 *@param lifetime        [IN]     onenet context lifetime
 *@param withObjFlag     [IN]     update object or not
 *@retval #int    success: XY_OK        fail:   XY_Err_NoConnected   网络未连接
 *                                              XY_Err_Parameter     参数错误
 *                                              XY_Err_NotAllowed    业务线程不存在，需要先注册
 *                                              XY_ERR               业务处理错误
 *@warning   当用户认为后续没有报文交互需求时，调用该接口来伴随触发RAI，以降低功耗。必须设置出厂NV参数close_rai为1，否则在用户释放最后一把锁后会由芯翼平台自触发RAI
 */
int  cis_updatelife_with_rai(int lifetime, bool withObjFlag);

/**
 *@ingroup cisapi
 *@brief 获取 CIS SDK 版本号
 *@param rsp_cmd        [OUT]     onenet sdk version
 */
void cis_miplver(char **rsp_cmd);

/**
 *@ingroup cisapi
 *@brief 检查是否观测成功指定的对象，实例，资源，开启云保存功能之后，会先尝试从NV恢复云会话数据，
 *@param objId         [IN]     object id
 *@param insId         [IN]     instance id
 *@param resId         [IN]     resource id
 *@retval #int    success: XY_OK        fail:   XY_Err_NotAllowed    业务线程不存在，需要先注册
 *                                              XY_ERR               业务处理错误
 */
int  cis_check_observe(int objId, int insId, int resId);

/**
 *@ingroup cisapi
 *@brief 获取本地CIS状态机运行状态，开启云保存功能之后，会先尝试从NV恢复云会话数据，
 *@retval #int    success: XY_OK        fail:   enum et_client_state_t  状态返回码
 */
et_client_state_t cis_get_context_state();

/**
 *@ingroup cisapi
 *@brief 获取CIS服务器运行状态，开启云保存功能之后，会先尝试从NV恢复云会话数据，
 *@retval #int    success: XY_OK        fail:   enum et_status_t  状态返回码
 */
et_status_t cis_get_server_status();

/**
 *@ingroup cisapi
 *@brief 设置CIS下行报文的回调函数
 *@param downstream_callback               [IN]     callback function
 */
void  cis_set_downstream_cb(cis_downstream_callback downstream_cb);

#endif

