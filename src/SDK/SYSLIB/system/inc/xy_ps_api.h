/** 
* @file     	xy_ps_api.h
* @brief     	3GPP相关的API接口，目前提供了常用的API接口，以满足普通客户的二次开发。\n
*  如果个别客户有更细节的3GPP信息需求，请参阅<芯翼XY1100 AT命令手册>中具体AT命令介绍，并使用平台提供的接口进行查阅，可以参考at_NUESTATS_demo.c
* @attention  	3GPP相关的API接口，与PS协议栈内部状态紧藕，存在返回失败或值异常的可能，需要调用者充分考虑调用点及相关值的容错处理
*
*/

#pragma once
#include <stdint.h>

/*******************************************************************************
 *                             Macro definitions                               *
 ******************************************************************************/
/**
 * @brief 			Corresponding "AT+CFUN" operation
 * @warning 
 */
#define NET_CFUN0 		0 ///< will  AT+CFUN=0
#define NET_CFUN1 		1 ///< will  AT+CFUN=1
#define IMEI_LEN 		(15 + 1)
#define IMSI_LEN 		(15 + 1)
#define APN_LEN 		100
#define UCCID_LEN 		(20 + 1)
#define SN_LEN 			(64 + 1)
#define IPADDR_LEN 		16
#define PDPTYPE_LEN 	7
#define HARDVER_LEN 	20
#define VERSIONEXT_LEN 	28
#define MODULVER_LEN 	20
#define EDRX_BASE_TIME 	10.24
#define PTW_BASE_TIME 	2.56

/*******************************************************************************
 *                             Type definitions                                *
 ******************************************************************************/
typedef struct ril_serving_cell_info
{
	int Signalpower;
	int Totalpower;
	int TXpower;
	int CellID;
	int ECL;
	int SNR;
	int EARFCN;
	int PCI;
	int RSRQ;
	char tac[5];
	int sband;
} ril_serving_cell_info_t;

typedef struct neighbor_cell_info
{
	int nc_earfcn;
	int nc_pci;
	int nc_rsrp;
} neighbor_cell_info_t;

typedef struct ril_phy_info
{
	//BLER
	int RLC_UL_BLER;
	int RLC_DL_BLER;
	int MAC_UL_BLER;
	int MAC_DL_BLER;
	int MAC_UL_total_bytes;
	int MAC_DL_total_bytes;
	int MAC_UL_total_HARQ_TX;
	int MAC_DL_total_HARQ_TX;
	int MAC_UL_HARQ_re_TX;
	int MAC_DL_HARQ_re_TX;
	//THP
	int RLC_UL_tput;
	int RLC_DL_tput;
	int MAC_UL_tput;
	int MAC_DL_tput;
} ril_phy_info_t;

typedef struct ril_neighbor_cell_info
{
	int nc_num;
	neighbor_cell_info_t neighbor_cell_info[5];
} ril_neighbor_cell_info_t;

typedef struct ril_radio_info
{
	ril_serving_cell_info_t serving_cell_info;
	ril_neighbor_cell_info_t neighbor_cell_info;
	ril_phy_info_t phy_info;
} ril_radio_info_t;

/*******************************************************************************
 *                       Global function declarations                          *
 ******************************************************************************/
/**
 * @brief 			Corresponding "AT+CFUN" operation
 * @param status  	see  @ref NET_CFUN0  NET_CFUN1
 * @return 			XY_OK is success,other value is fail
 * @warning         AT+CFUN=1成功返回OK后，并不代表此时PDP已激活，需要调用xy_wait_tcpip_ok接口等待PDP激活后，才能进行网络通信业务
 */
int xy_cfun_excute(int status);

/**
* @brief 			This is funtion user Read the value of CFUN
* @param 			存储cfun的int指针
* @return			XY_ERR获取失败, XY_OK获取成功
*/
int xy_cfun_read(int *cfun);

/**
* @brief 			This is funtion user Read check whether the network is registered
* @param 			存储cgact的int指针
* @return			XY_ERR获取失败, XY_OK获取成功
*/
int xy_get_CGACT(int *cgact);

/**
* @brief 			查询CEREG配置的n值
* @param 			存储cereg的int指针，获取成功时，该值的具体含义请查阅《芯翼XY1100 AT命令手册》中的CEREG中n参数值
* @return			XY_ERR获取失败, XY_OK获取成功
* @warning          如果需要查询当前是否已小区驻留，可以使用AT+NUESTATS=CELL命令查询当前的服务小区
*/
int xy_cereg_read(int *cereg);

/**
 * @brief 			This is funtion user Read the SN
 * @param sn		由调用者申请内存，接口内部赋值
 * @param len		入参sn指针的长度，由调用者赋值，但不能小于@ref SN_LEN 
 * @return 			XY_ERR获取失败, XY_OK获取成功
 * @warning         注意入参内存申请长度，否则会造成内存越界
 */
int xy_get_SN(char *sn, int len);

/**
* @brief 			获取信号强度
* @param 			存储rssi的int指针
* @return			XY_ERR获取失败, XY_OK获取成功
*/
int xy_get_RSSI(int *rssi);

/**
 * @brief			This is funtion user get IMEI
 * @param imei		由调用者申请内存，接口内部赋值
 * @param len		入参imei指针的长度，由调用者赋值，但不能小于16
 * @return 			XY_ERR获取失败, XY_OK获取成功
 * @warning         注意入参内存申请长度，否则会造成内存越界
 */
int xy_get_IMEI(char *imei, int len);

/**
 * @brief			This is funtion user get IMSI
 * @param imsi		由调用者申请内存，接口内部赋值
 * @param len		入参imsi指针的长度，由调用者赋值，但不能小于16
 * @return 			XY_ERR is fail, XY_OK is success
 * @warning         注意入参内存申请长度，否则会造成内存越界
 *                  若尚未检测到SIM卡，imsi指针未被赋值，需要客户自行识别是否已经识别到SIM卡
 */
int xy_get_IMSI(char *imsi, int len);

/**
 * @brief			This is funtion user get cellid
 * @param 			存储cellid的int指针
 * @return			XY_ERR获取失败, XY_OK获取成功
 * @warning         小区未驻留之前，无法获取小区ID
 */
int xy_get_CELLID(int *cell_id);

/**
 * @brief 			Corresponding get NCCID operation
 * @param ccid      由调用者申请内存，接口内部赋值
 * @param len		入参ccid指针的长度，由调用者赋值，但不能小于23 
 * @return 			XY_ERR is fail, XY_OK is success
 * @warning         注意入参内存申请长度，否则会造成内存越界
 *                  若尚未检测到SIM卡，imsi指针未被赋值，需要客户自行识别是否已经识别到SIM卡
 */
int xy_get_NCCID(char *ccid, int len);

/**
 * @brief 			This is funtion user get APN
 * @param apn_buf	由调用者申请内存，接口内部赋值
 * @param len		入参apn_buf指针的长度，由调用者赋值，但不能小于100 
 * @param query_cid	PDP激活对应的哪一路cid，调用者若不关心则填-1，接口内部会自动赋值，即当前网络激活承载的cid路数，一般为0
 * @return 			XY_ERR is fail,XY_OK is success.
 * @warning         注意入参内存申请长度，否则会造成内存越界
 *                  若尚未配置APN，apn_buf不会被赋值
 					若query_cid要在驻网之后才能填-1，否则返回空
 */
int xy_get_PDP_APN(char *apn_buf, int len, int query_cid);

/**
 * @brief			获取当前网络激活承载的为哪路cid，PDP未激活时默认-1
 * @return 			当前网络激活承载的为哪路cid，默认值为0
 */
int xy_get_working_cid();

/**
 * @brief 			This is funtion user get IP Addr
 * @param ipAddr	由调用者申请内存，接口内部赋值
 * @param addrLen	入参ipAddr指针的长度，由调用者赋值，但不能小于17 
 * @return 			XY_ERR is fail,XY_OK is success.
 * @warning         注意入参内存申请长度，否则会造成内存越界
 *                  若尚未PDP激活成功，ipAddr不会被赋值
 * @note            该接口得到的是字符串类型地址，如需要转成int型，需要自行调用inet_aton接口
 */
int xy_getIP4Addr(char *ipAddr, int addrLen);

/**
 * @brief 			获取IPV4的整形地址
 * @param 			存储ip4addr的unsigned int指针
 * @return			XY_ERR获取失败, XY_OK获取成功
 * @warning         若尚未PDP激活成功，返回XY_ERR
 */
int xy_get_ip4addr(unsigned int *ip4addr);

/**
 * @brief 			This is funtion user get 3412 tau_time
 * @param 			存储tau的int指针
 * @return			XY_ERR获取失败, XY_OK获取成功
 */
int xy_get_T_TAU(int *tau);

/**
 * @brief 			This is funtion user get 3324 act_time
 * @param 			存储t3324的int指针
 * @return			XY_ERR获取失败, XY_OK获取成功
 */
int xy_get_T_ACT(int *t3324);

/**
 * @brief 		用来通知PS发送空的UDP报文，并携带RAI指示给基站，以快速释放链接
 * @return 		XY_OK is success,XY_ERR is fail
 * @note        当用户认为后续没有报文交互需求时，调用该接口来伴随触发RAI，以降低功耗。必须设置出厂NV参数close_rai为1，否则在用户释放最后一把锁后会由芯翼平台自触发RAI
 * @warning     该接口慎用！！！建议用户使用如cis_ext_send_with_rai、cdp_send_syn_with_rai等接口\n
 ***    PS会释放当前待发送的所有上行数据，进行需要用户确保调用该接口时已经没有待发送的上行数据。
 */
int xy_send_rai();

/**
 * @brief   This is funtion user get modul_ver（厂商信息）
 * @param   modul_ver  存储modul_ver的内存buf，由调用者分配，buf长度不小于20；  
 * @param   len     modul_ver的内存长度，不小于20
 * @return 	XY_ERR is fail,return XY_OK is success.
**/
int xy_get_MODULVER(char *modul_ver, int len);

/**
 * @brief   This is funtion user get versionExt（软件版本号）
 * @param   versionExt  存储versionExt的内存buf，由调用者分配，buf长度不小于28
 * @param   len   versionExt的内存长度，不小于28
 * @return 	XY_ERR is fail,return XY_OK is success.
**/
int xy_get_VERSIONEXT(char *versionExt, int len);

/**
 * @brief   This is funtion user get hardver（硬件版本号）
 * @param   hardver  存储hardver的内存buf，由调用者分配，buf长度不小于20；
 * @param   len    hardver的内存长度，不小于20
 * @return 	XY_ERR is fail,return XY_OK is success.
**/
int xy_get_HARDVER(char *hardver, int len);

/**
 * @brief   This is funtion user get uicc type
 * @param   uicc_type  存储uicc_type的int 指针
 * @return 	XY_ERR is fail,return XY_OK is success.
*/
int xy_get_UICC_TYPE(int *uicc_type);

/**
 * @brief   用以分别获取eDRX的寻呼周期和寻呼窗口周期大小
 * @param   eDRX_value	eDRX的寻呼周期，单位秒
 * @param   ptw_value	寻呼窗口周期，单位秒
 * @return  XY_ERR is fail,return XY_OK is success.
*/
int xy_get_eDRX_value(float *eDRX_value, float *ptw_value);

/**
 * @brief 	获取服务小区信息(对应AT命令为AT+NUESTATS=RADIO)
 * @param rcv_servingcell_info 用户主动malloc，并将地址传递进来，不可为NULL
 * @return  XY_ERR is fail,return XY_OK is success. 
 */
int xy_get_servingcell_info(ril_serving_cell_info_t *rcv_servingcell_info);

/**
 * @brief 	获取邻小区信息(对应AT命令为AT+NUESTATS=CELL)
 * @param neighbor_cell_info 用户主动malloc，并将地址传递进来，不可为NULL
 * @return XY_ERR is fail,return XY_OK is success.
 * @note 获取邻小区的earfcn，pci，rsrp与邻小区的数量，最多只返回5个邻小区的信息。
 */
int xy_get_neighborcell_info(ril_neighbor_cell_info_t *neighbor_cell_info);

/**
 * @brief 	获取PHY信息	(对应AT命令为AT+NUESTATS=BLER和AT+NUESTATS=THP)
 * @param  rcv_phy_info 用户主动malloc，并将地址传递进来，不可为NULL
 * @return XY_ERR is fail,return XY_OK is success.
 * @note        
 */
int xy_get_phy_info(ril_phy_info_t *rcv_phy_info);

/**
 * @brief 	获取NUESTATS中3GPP相关信息的API接口
 * @param rcv_radio_info 用户主动malloc，并将地址传递进来，不可为NULL
 * @return XY_ERR is fail,return XY_OK is success.
 * @note  该接口内部分为几次发送不同的AT命令给PS，由于这条AT命令非标准，存在变化的可能，客户自行参考该DEMO实现即可      
 */
int xy_get_radio_info(ril_radio_info_t *rcv_radio_info);