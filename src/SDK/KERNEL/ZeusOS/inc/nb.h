#ifndef __NB_H_
#define __NB_H_
#include "zos_def.h"


/*******************************************************************************
 *                             Type definitions                                *
 ******************************************************************************/
/**
* @brief System power on reason,get from SoC registor, see  @ref get_sys_up_stat()
* @param POWER_ON		正常开机
* @param PIN_RESET		RESET硬件复位
* @param SOFT_RESET     软件复位
* @param UTC_WAKEUP     RTC超时唤醒 
* @param EXTPIN_WAKEUP	WAKEUP-PIN 唤醒
* @param WDT_RESET		硬件看门狗复位
* @param UNKNOWN_ON     未知错误，请查看硬件设计
*/
typedef enum
{
    NB_STATE_POWER_ON = 0,  ///  正常开机
    NB_STATE_PIN_RESET,     ///  RESET硬件复位
    NB_STATE_SOFT_RESET,    ///  软件复位
    NB_STATE_UTC_WAKEUP,    ///  RTC超时唤醒
    NB_STATE_EXTPIN_WAKEUP, ///  WAKEUP-PIN 唤醒
    NB_STATE_WDT_RESET,     ///  硬件看门狗复位
    NB_STATE_UNKNOWN_ON,    ///  未知错误，请查看硬件设计
} NB_START_STATE;

/**
* @brief reason for  SOFT_RESET, always unused 
* @param RB_BY_ASSERT	by assert
* @param RB_BY_NRB		by soft reboot,such as AT+NRB/xy_reboot/....
* @param RB_BY_RESET    by AT+RESET
* @param RB_BY_POWERTEST by	AT+NV=POWERTEST,unused
* @param RB_BY_FOTA     by  FOTA
* @param RB_BY_LOW_VOL  by xy_check_low_vBat for low vBAT
*/
typedef enum
{
    NB_RB_BY_ASSERT = 0,
    NB_RB_BY_NRB,
    NB_RB_BY_RESET,
    NB_RB_BY_POWERTEST,
    NB_RB_BY_FOTA,
    NB_RB_BY_LOW_VOL,
    NB_RB_MAX,
} NB_REBOOT_TYPE_T;

typedef enum {
	OS_OK					        =  0,
	OS_ERR					      = -1, 		  //normal err
	OS_Err_Timeout			  = -2, 		  // Operation not completed within the timeout period.
	OS_Err_NoConnected		= -3, 		  // tcpip newwork unusable,such as  PDP not activate
	OS_Err_Parameter		  = -4, 		  // input parameter error.
	OS_Err_NoMemory 		  = -5, 		  // System is out of memory: it was impossible to allocate or reserve memory for the operation.
	OS_Err_NotAllowed		  = -6, 		  // api not allowed,such as SIM card not insert,and do something about 3GPP operation.
	OS_Err_LowVbat			  = -7, 		  //low Vbat,can not do flash write
	OS_Err_Reserved 		  = 0x7FFFFFFF
} os_ret_Status_t;


typedef struct nb_serving_cell_info
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
} nb_serving_cell_info_t;

typedef struct nb_phy_info
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
} nb_phy_info_t;

typedef struct net_neighbor_cell_info
{
	int nc_earfcn;
	int nc_pci;
	int nc_rsrp;
} net_neighbor_cell_info_t;

typedef struct nb_neighbor_cell_info
{
	int nc_num;
	net_neighbor_cell_info_t neighbor_cell_info[5];
} nb_neighbor_cell_info_t;

typedef struct nb_radio_info
{
	nb_serving_cell_info_t serving_cell_info;
	nb_neighbor_cell_info_t neighbor_cell_info;
	nb_phy_info_t phy_info;
} nb_radio_info_t;



/**
 * @brief 指示芯片是否从深睡唤醒的，常用于支持深睡的模式，以识别是否为正常唤醒状态
 * 
 * @return bool
 * @note ZOS_EOK:深睡眠唤醒    ZOS_ERROR：不是深睡眠唤醒
 */
extern zos_err_t nb_get_powenon_from_deepsleep(void);

/**
* @brief 获取开机类型
* @return 开机类型详见 NB_START_STATE.
*/
extern uint32_t nb_get_power_type(void);


/**
* @brief 获取复位类型
* @return 复位类型详见 NB_REBOOT_TYPE_T.
*/
extern uint32_t nb_get_reset_type(void);


/**
 * @brief  关闭standby浅睡模式，接口内部实现了累加操作，以供多外设配对使用。
 * @warning 平台默认开启standby睡眠功能，当客户使用外设时，由于standby期间会关闭外设时钟，所以需要临时关闭standby睡眠，以防止丢数据。 \n
 *  该接口必须配对使用，即在准备使用外设时调用xy_standby_lock()关闭standby,使用完毕外设时调用xy_standby_unlock()开启standby。
 * @attention  由于存在多个外设同时工作情况，进而要求必须配对开关standby，否则会影响其他外设工作
 */
extern void nb_standby_lock(void);

/**
 * @brief  打开standby浅睡模式，接口内部实现了累减操作，以供多外设配对使用。
 * @warning 平台默认开启standby睡眠功能，当客户使用外设时，由于standby期间会关闭外设时钟，所以需要临时关闭standby睡眠，以防止丢数据。 \n
 *  该接口必须配对使用，即在准备使用外设时调用xy_standby_lock()关闭standby,使用完毕外设时调用xy_standby_unlock()开启standby。
 * @attention  由于存在多个外设同时工作情况，进而要求必须配对开关standby，否则会影响其他外设工作
 */
extern void nb_standby_unlock(void);

/*
* @brief 清空计数，可进入standby睡眠
* @warning  该接口慎用！强行清空计数，可能会造成正在工作的外设因为进了standby睡眠而数据通信异常。
*/
extern void nb_standby_clear(void);


/**
 * @brief 当用户业务需要使用芯翼芯片时，通过该接口来申请锁
 * @param lock_type  锁类型，用户默认使用0，不得使用其他值
 * @warning 必须与 xy_work_unlock配对使用，否则会造成深睡异常
 * @note  理论存在多个用户冲突使用芯翼芯片的可能，只要每个业务配对使用工作锁，就可以保证流程正常
 */
extern void nb_work_lock(void);

/**
 * @brief 当用户业务做完后，通过该接口通知芯翼平台准备进入深睡. 内部可选触发RAI快速链接释放。
 * @param lock_type  锁类型，用户默认使用0，不得使用其他值
 * @attention  芯翼芯片的深睡条件是没有用户申请锁的前提下，若内部检测出超过2秒的空闲时间，就会进入深睡； \n
 *  所以，并不是说用户释放锁后，就会很快睡眠，还要取决于3GPP能否调度出2秒以上的空闲，例如若在1.28S的寻呼状态下，永远都不会进入深睡。
 * @note  如果用户确定整机无需继续工作，且下次上电后重新执行attach流程，可以使用xy_fast_power_off来更快速深睡，类似直接断电，做到一秒内进深睡。
 * @warning 必须与xy_work_lock配套使用，否则可能造成其他业务因为提前深睡而造成流程异常。\n
 *  默认释放锁后内部会通过PS触发RAI，如果用户想自行控制RAI的发送类型和发送时机，将出厂NV参数close_rai设为1即可。
 */
extern void nb_work_unlock(void);

/**
 * @brief 强制芯片进入深睡，不关心当前持有锁的情况，也不关心当前3GPP是否能够调度出4秒以上的空闲时间。平台内部仅保存NV，进而要求下次上电后3GPP必须执行attach流程。
 * @param abnormal_off    缺省值为0
* @warning 该接口不与基站进行握手通信，类似直接断电，可达到1秒内进深睡。仅能在两个场景下使用：  \n
* 1) 当用户启用的软件看门狗超时后，可以认为当前3GPP状态异常，调用该接口进行强制深睡.\n
* 2) 部分客户为了健壮性考虑，要求不使用PSM模式，每次工作完毕后，调用该接口快速秒级深睡，下次再上电执行attach流程.
 */
extern void nb_fast_power_off(void);

/**
 * @brief 软重启接口，内部进行相关NV的保存后，触发芯片软重启.
 * @attention 软重启后RTC定时器及当前世界时间保持不变，若重启前已经获取了当前世界时间，则重启后仍然有效
 */
extern void nb_reboot(void);


/**
 * @brief 			Corresponding "AT+CFUN" operation
 * @param status  	see  @ref NET_CFUN0  NET_CFUN1
 * @return 			ZOS_OK is success,other value is fail
 * @warning         AT+CFUN=1成功返回OK后，并不代表此时PDP已激活，需要调用nb_wait_tcpip_ok接口等待PDP激活后，才能进行网络通信业务
 */
extern zos_err_t nb_set_cfun(int status);
/**
* @brief 			This is funtion user Read the value of CFUN
* @param 			存储cfun的int指针
* @return			ZOS_ERROR获取失败, ZOS_OK获取成功
*/
extern zos_err_t nb_get_cfun(int *cfun);

/**
* @brief 			This is funtion user Read check whether the network is registered
* @param 			存储cgact的int指针
* @return			ZOS_ERROR获取失败, ZOS_OK获取成功
*/
extern zos_err_t nb_get_CGACT(int *cgact);

/**
* @brief 			查询CEREG配置的n值
* @param 			存储cereg的int指针，获取成功时，该值的具体含义请查阅《芯翼XY1100 AT命令手册》中的CEREG中n参数值
* @return			ZOS_ERROR获取失败, ZOS_OK获取成功
* @warning          如果需要查询当前是否已小区驻留，可以使用AT+NUESTATS=CELL命令查询当前的服务小区
*/
extern zos_err_t nb_get_cereg(int *cereg);

/**
 * @brief 			This is funtion user Read the SN
 * @param sn		由调用者申请内存，接口内部赋值
 * @param len		入参sn指针的长度，由调用者赋值，但不能小于@ref SN_LEN
 * @return 			ZOS_ERROR获取失败, ZOS_OK获取成功
 * @warning         注意入参内存申请长度，否则会造成内存越界
 */
extern zos_err_t nb_get_SN(char *sn, int len);

/**
* @brief 			获取信号强度
* @param 			存储rssi的int指针
* @return			ZOS_ERROR获取失败, ZOS_OK获取成功
*/
extern zos_err_t nb_get_csq(int *rssi);

/**
 * @brief			This is funtion user get IMEI
 * @param imei		由调用者申请内存，接口内部赋值
 * @param len		入参imei指针的长度，由调用者赋值，但不能小于16
 * @return 			ZOS_ERROR获取失败, ZOS_OK获取成功
 * @warning         注意入参内存申请长度，否则会造成内存越界
 */
extern zos_err_t nb_get_IMEI(char *imei);

/**
 * @brief			This is funtion user get IMSI
 * @param imsi		由调用者申请内存，接口内部赋值
 * @param len		入参imsi指针的长度，由调用者赋值，但不能小于16
 * @return 			ZOS_ERROR is fail, ZOS_OK is success
 * @warning         注意入参内存申请长度，否则会造成内存越界
 *                  若尚未检测到SIM卡，imsi指针未被赋值，需要客户自行识别是否已经识别到SIM卡
 */
extern zos_err_t nb_get_IMSI(char *imsi);

/**
 * @brief			This is funtion user get cellid
 * @param 			存储cellid的int指针
 * @return			ZOS_ERROR获取失败, ZOS_OK获取成功
 * @warning         小区未驻留之前，无法获取小区ID
 */
extern zos_err_t nb_get_CELLID(int *cell_id);

/**
 * @brief 			Corresponding get NCCID operation
 * @param ccid      由调用者申请内存，接口内部赋值
 * @param len		入参ccid指针的长度，由调用者赋值，但不能小于23
 * @return 			ZOS_ERROR is fail, ZOS_OK is success
 * @warning         注意入参内存申请长度，否则会造成内存越界
 *                  若尚未检测到SIM卡，imsi指针未被赋值，需要客户自行识别是否已经识别到SIM卡
 */
extern zos_err_t nb_get_NCCID(char *ccid);

/**
 * @brief 			This is funtion user get APN
 * @param apn_buf	由调用者申请内存，接口内部赋值
 * @param len		入参apn_buf指针的长度，由调用者赋值，但不能小于100
 * @param query_cid	PDP激活对应的哪一路cid，调用者若不关心则填-1，接口内部会自动赋值，即当前网络激活承载的cid路数，一般为0
 * @return 			ZOS_ERROR is fail,ZOS_OK is success.
 * @warning         注意入参内存申请长度，否则会造成内存越界
 *                  若尚未配置APN，apn_buf不会被赋值
 					若query_cid要在驻网之后才能填-1，否则返回空
 */
extern zos_err_t nb_get_PDP_APN(char *apn_buf, int len, int query_cid);

/**
 * @brief			获取当前网络激活承载的为哪路cid，PDP未激活时默认-1
 * @return 			当前网络激活承载的为哪路cid，默认值为0
 */
extern zos_err_t nb_get_working_cid();

/**
 * @brief 			This is funtion user get IP Addr
 * @param ipAddr	由调用者申请内存，接口内部赋值
 * @param addrLen	入参ipAddr指针的长度，由调用者赋值，但不能小于17
 * @return 			ZOS_ERROR is fail,ZOS_OK is success.
 * @warning         注意入参内存申请长度，否则会造成内存越界
 *                  若尚未PDP激活成功，ipAddr不会被赋值
 * @note            该接口得到的是字符串类型地址，如需要转成int型，需要自行调用inet_aton接口
 */
extern zos_err_t nb_getIP4Addr(char *ipAddr, int addrLen);

/**
 * @brief 			获取IPV4的整形地址
 * @param 			存储ip4addr的unsigned int指针
 * @return			ZOS_ERROR获取失败, ZOS_OK获取成功
 * @warning         若尚未PDP激活成功，返回XY_ERR
 */
extern zos_err_t nb_get_ip4addr(unsigned int *ip4addr);

/**
 * @brief 			This is funtion user get 3412 tau_time
 * @param 			存储tau的int指针
 * @return			ZOS_ERROR获取失败, ZOS_OK获取成功
 */
extern zos_err_t nb_get_T_TAU(int *tau);

/**
 * @brief 			This is funtion user get 3324 act_time
 * @param 			存储t3324的int指针
 * @return			ZOS_ERROR获取失败, ZOS_OK获取成功
 */
extern zos_err_t nb_get_T_ACT(int *t3324);

/**
 * @brief 		用来通知PS发送空的UDP报文，并携带RAI指示给基站，以快速释放链接
 * @return 		ZOS_OK is success,ZOS_ERROR is fail
 * @note        当用户认为后续没有报文交互需求时，调用该接口来伴随触发RAI，以降低功耗。必须设置出厂NV参数close_rai为1，否则在用户释放最后一把锁后会由芯翼平台自触发RAI
 * @warning     该接口慎用！！！建议用户使用如cis_ext_send_with_rai、cdp_send_syn_with_rai等接口\n
 ***    PS会释放当前待发送的所有上行数据，进行需要用户确保调用该接口时已经没有待发送的上行数据。
 */
extern zos_err_t nb_send_rai();

/**
 * @brief   This is funtion user get modul_ver（厂商信息）
 * @param   modul_ver  存储modul_ver的内存buf，由调用者分配，buf长度不小于20；
 * @param   len     modul_ver的内存长度，不小于20
 * @return 	ZOS_ERROR is fail,return ZOS_OK is success.
**/
extern zos_err_t nb_get_MODULVER(char *modul_ver, int len);

/**
 * @brief   This is funtion user get versionExt（软件版本号）
 * @param   versionExt  存储versionExt的内存buf，由调用者分配，buf长度不小于28
 * @param   len   versionExt的内存长度，不小于28
 * @return 	ZOS_ERROR is fail,return ZOS_OK is success.
**/
extern zos_err_t nb_get_VERSIONEXT(char *versionExt, int len);

/**
 * @brief   This is funtion user get hardver（硬件版本号）
 * @param   hardver  存储hardver的内存buf，由调用者分配，buf长度不小于20；
 * @param   len    hardver的内存长度，不小于20
 * @return 	ZOS_ERROR is fail,return ZOS_OK is success.
**/
extern zos_err_t nb_get_HARDVER(char *hardver, int len);

/**
 * @brief   This is funtion user get uicc type
 * @param   uicc_type  存储uicc_type的int 指针
 * @return 	ZOS_ERROR is fail,return ZOS_OK is success.
*/
extern zos_err_t nb_get_UICC_TYPE(int *uicc_type);

/**
 * @brief   用以分别获取eDRX的寻呼周期和寻呼窗口周期大小
 * @param   eDRX_value	eDRX的寻呼周期，单位秒
 * @param   ptw_value	寻呼窗口周期，单位秒
 * @return  ZOS_ERROR is fail,return ZOS_OK is success.
*/
extern zos_err_t nb_get_eDRX_value(float *eDRX_value, float *ptw_value);

/**
 * @brief 	获取服务小区信息(对应AT命令为AT+NUESTATS=RADIO)
 * @param rcv_servingcell_info 用户主动malloc，并将地址传递进来，不可为NULL
 * @return  ZOS_ERROR is fail,return ZOS_OK is success.
 */
extern zos_err_t nb_get_servingcell_info(nb_serving_cell_info_t *rcv_servingcell_info);

/**
 * @brief 	获取邻小区信息(对应AT命令为AT+NUESTATS=CELL)
 * @param neighbor_cell_info 用户主动malloc，并将地址传递进来，不可为NULL
 * @return ZOS_ERROR is fail,return ZOS_OK is success.
 * @note 获取邻小区的earfcn，pci，rsrp与邻小区的数量，最多只返回5个邻小区的信息。
 */
extern zos_err_t nb_get_neighborcell_info(nb_neighbor_cell_info_t *neighbor_cell_info);

/**
 * @brief 	获取PHY信息	(对应AT命令为AT+NUESTATS=BLER和AT+NUESTATS=THP)
 * @param  rcv_phy_info 用户主动malloc，并将地址传递进来，不可为NULL
 * @return ZOS_ERROR is fail,return ZOS_OK is success.
 * @note
 */
extern zos_err_t nb_get_phy_info(nb_phy_info_t *rcv_phy_info);

/**
 * @brief 	获取NUESTATS中3GPP相关信息的API接口
 * @param rcv_radio_info 用户主动malloc，并将地址传递进来，不可为NULL
 * @return ZOS_ERROR is fail,return ZOS_OK is success.
 * @note  该接口内部分为几次发送不同的AT命令给PS，由于这条AT命令非标准，存在变化的可能，客户自行参考该DEMO实现即可
 */
extern zos_err_t nb_get_radio_info(nb_radio_info_t *rcv_radio_info);


#endif  /* __NB_H_ */
