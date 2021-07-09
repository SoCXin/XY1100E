/** 
* @file        xy_at_api.h
* @brief       用户开发使用的AT相关接口及定义
*/
#pragma once
#include <stdint.h>

/*******************************************************************************
 *                             Macro definitions                               *
 ******************************************************************************/
/**
* @brief  AT错误码构建宏定义，内部组装为“+CME ERROR: XXX”
* @param  a 错误码，参见@ref AT_XY_ERR
* @return  返回字符串堆指针，由芯翼平台内部负责释放空间
*/

#define AT_ERR_BUILD(a) at_err_build_info(a, __FILE__, __LINE__)

/*******************************************************************************
 *                             Type definitions                                *
 ******************************************************************************/
/**
* @brief AT process result macro
* @param AT_OK			         at_parse_param_2 success
* @param ATERR_XY_ERR			 fatal error need reset
* @param ATERR_PARAM_INVALID     param  invalid
* @param ATERR_NOT_ALLOWED       operate not allow
* @param ATERR_DROP_MORE		 because sleep,at string head drop too more
* @param ATERR_DOING_FOTA		 For the duration of FOTA,cannot proc any AT req
* @param ATERR_MORE_PARAM        too more param,can not parase this param,only used for strict check
* @param ATERR_WAIT_RSP_TIMEOUT  wait AT response result timeout
* @param ATERR_CHANNEL_BUSY  	 now have one req at working,receive new  req at 
* @param ATERR_DSP_NOT_RUN       dsp not run
* @param ATERR_NOT_NET_CONNECT   NB PS can not PDP activate within the agreed time
* @param ATERR_NEED_OFFTIME      extern MCU power off XY Soc,and not send time bias extensional AT CMD when power up again
* @param ATERR_CONN_NOT_CONNECTED  connecting socket be resetted by server for unknown case,user must close current socket
* @param ATERR_INVALID_PREFIX 	 invalid prefix
* @param ATERR_LOW_VOL           voltage  too low,and SoC can not work normal
* @param USER_EXTAND_ERR_BASE    user can add self  err number from  here!
*/
enum AT_XY_ERR
{
	AT_OK = 0,

	ATERR_XY_ERR = 8000,
	ATERR_PARAM_INVALID,         //8001
	ATERR_NOT_ALLOWED,           //8002
	ATERR_DROP_MORE,             //8003
	ATERR_DOING_FOTA,            //8004
	ATERR_MORE_PARAM,            //8005
	ATERR_WAIT_RSP_TIMEOUT,      //8006
	ATERR_CHANNEL_BUSY,          //8007
	ATERR_DSP_NOT_RUN,           //8008
	ATERR_NOT_NET_CONNECT,       //8009
	ATERR_NEED_OFFTIME,          //8010
	ATERR_CONN_NOT_CONNECTED,    //8011
	ATERR_INVALID_PREFIX,	     //8012
	ATERR_LOW_VOL,               //8013

	USER_EXTAND_ERR_BASE = 9000,
};

/**
* @brief AT STR process result
* @param AT_END			reply rsp cmd  by at_ctl dirextly
* @param AT_FORWARD		forward  at req to  external processor
*/
enum at_cmd_route
{
	AT_END = 0,
	AT_FORWARD,
	AT_ROUTE_MAX,
};

/**
* @brief AT动作的种类，用全局g_req_type来记录
*/
enum AT_REQ_TYPE
{
	AT_CMD_REQ = 0, //AT+xxx=param
	AT_CMD_ACTIVE,	//AT+xxx,not include param
	AT_CMD_QUERY,	//AT+XXX?
	AT_CMD_TEST,	//AT+XXX=?
};

/**
* @brief  请求类AT命令的注册回调函数声明
* @param  at_paras [IN] at cmd param head,such as "1,5,CMNET"
* @param  rsp_cmd  [OUT] double pointer,response result string,such as "+ERROR:8009"
* @return always is AT_END,,see  @ref  at_cmd_route
* @note    if have response result string ,must malloc memory for rsp_cmd,and set value
*/
typedef int (*ser_req_func)(char *at_paras, char **rsp_cmd);

/**
* @brief  主动上报类AT命令的注册回调函数声明
* @param  at_paras  at cmd param head,such as "1,5,CMNET"
* @return see  @ref at_context_state,always is AT_END
* @note   always is AT_END,see  @ref  at_cmd_route
*/
typedef int (*inform_act_func)(char *at_paras);

/**
  * @brief  AT通道传输透传数据的回调函数声明
  */
typedef int (*data_proc_func)(char *data,int data_len);

/*******************************************************************************
 *                       Global variable declarations                          *
 ******************************************************************************/
extern char g_req_type;		//record request cmd type

/**
  * @brief  AT通道传输透传数据的回调函数指针全局声明
  * @note  默认赋值NULL，表示当前传输的是AT命令；否则为透传数据，具体函数实现需要用户完成，请参考at_transparent_mode.c
  */
extern data_proc_func g_at_transparent_data_hook;

/*******************************************************************************
 *                       Global function declarations                          *
 ******************************************************************************/
/**
 * @brief 仅用于调试，通过AT口发送调试类URC主动上报，such as "+DBGINFO:"
 * @param data [IN]is URC cmd,such as "\r\n+DBGINFO:NV ERROR\r\n"
 * @warning  该接口在idle线程或锁临界区情况下会直接返回，造成丢数据，所以该接口仅用于发送调试信息，不得发送正常功能性AT命令
 */
void send_debug_str_to_at_uart(char *buf);

/**
 * @brief 用于供用户发送应答结果和URC主动上报给底板MCU，常见于at_user_req_serv注册的具体AT请求的应答处理。该接口不可以在锁中断期间调用.
 * @param data [IN] is response result cmd,such as "\r\nOK\r\n"
 * @warning  该接口效率偏低，因为接口内部会发送给at_ctl主框架，由at_ctl最终写AT口
 */
void send_rsp_str_to_ext(void *data);

/**
 * @brief 仅用于供业务层发送主动上报URC给AT口，不得发送结果码
 * @param data [IN] is URC cmd,such as "\r\n+XXX:YYYY\r\n"
 * @warning  该接口慎用！  ！！该接口不能发送携带“OK”“ERROR”结果码的字符串，只能发送主动上报
 */
void send_urc_to_ext(void *data);

/**
 * @brief  register URC string hook func
 * @param  at_prefix  [IN] URC prefix,such as "+CGEV:"
 * @param  func [IN]  hook func to process URC,see @inform_act_func
 */
void register_app_at_urc(char *at_prefix, inform_act_func func);

/**
 * @brief  register request AT string proc hook func
 * @param  at_prefix [IN] URC prefix,such as "+CGEV:"
 * @param  func [IN] process  recved request  AT string ,see @ser_req_func
 * @note   used for server node to process received  request AT string 
 */
void register_app_at_req(char *at_prefix, ser_req_func func);

/**
 * @brief  get ASCII data from at_str
 * @param fmt_parm   [IN] format,such as ",,,,,,,,%s"
 * @param at_str     [IN] at string   
 * @param ascii_len	 [IN] dest param len
 * @param param_data [OUT]dest data 
 * @return parase result,0 is success,see  AT_XY_ERR
 * @warning   only used for ASCII string AT req,such as onenet or socket
 */
int get_ascii_data(char *fmt_parm, char *at_str, int ascii_len, char *param_data);

/**
 * @brief  parase param value to va_args assigned by user
 * @param fmt  [IN] format,such as "%1d,%d,%s"
 * @param buf  [IN] AT string param head,such as "2,5,cmnet"
 * @param va_args ... [IN/OUT] va_args assigned by user,should be pointer
 * @return parase result,0 is success,see  @ref  AT_XY_ERR
 * @par example:
 * @code
	char buf[]="1,2,\"test\"";
	char n1 = 0;
	int n2 = 0;
	char *n3 = xy_zalloc(strlen(buf));
	ret = at_parse_param("%1d,%d,%s", buf, &n1,&n2,n3);
	//result : n1==1 n2==2 n3=="test"
	ret = at_parse_param("%1d,%d,%2s", buf, &n1,&n2,n3);
	//result : n1==1 n2==2 n3=="te"
   @endcode
 */
int at_parse_param(char *fmt, char *buf, ...);

/**
 * @brief  parase param value to va_args assigned by user
 * @param fmt  [IN] format,such as "%1d,%d,%s"
 * @param buf  [IN] param head,such as "2,5,cmnet"
 * @param is_strict	 [IN] check param strictly,probit invalid param
 * @param va_args ... [IN/OUT] va_args assigned by user,should be pointer
 * @return parase result,0 is success,see  @ref  AT_XY_ERR
 * @par example:
 * @code
	char buf[]="1,2,\"test\"";
	char n1 = 0;
	int n2 = 0;
	char *n3 = xy_zalloc(strlen(buf));
	ret = at_parse_param_ext("%1d,%d,%s", buf, 1, &n1,&n2,n3);
	//result : n1==1 n2==2 n3=="test"
	ret = at_parse_param_ext("%1d,%d", buf, 1, &n1,&n2,n3);
	//result : ATERR_MORE_PARAM
 * @endcode
 */
int at_parse_param_ext(char *fmt_parm, char *buf, int is_strict, ...);

/**
 * @brief 用于发送AT请求给平台，并同步处理对应的应答结果。
 * @param req_at  [IN] req at str,such as "AT+WORKLOCK=0\r\n"
 * @param info_fmt [IN] response str format,such as "%1d,%d,%s"
 * @param timeout [IN] wait rsp time,if timeout,maybe ERROR!
 * @param va_args ... [IN/OUT] va_args assigned by user,should be pointer
 * @return parase result,0 is success,see  @ref  AT_XY_ERR
 * @par example:
 * @code
	int ret = 0;
	ret = at_ReqAndRsp_to_ps("AT+CFUN=1\r", NULL, 15);
	if(ret == 0)
		xy_printf("success");

	char *pstr = k_malloc(50);
	memset(pstr,0,50);
	ret = at_ReqAndRsp_to_ps("AT+CIMI\r", "%50s", 5, pstr);
	if(ret == 0)
		xy_printf("success");

	int n1 = 0;
	short n2 = 0;
	char n3 = 0;
	ret = at_ReqAndRsp_to_ps("AT+CCIOTOPT?\r", "%d,%2d,%1d", 5, &n1,&n2,&n3);
	if(ret == 0)
		xy_printf("success");
 * @endcode
 * @warning  由于接口内部是根据AT前缀来匹配中间结果的，所以对于非标准化的AT请求中间结果不适用，需要按照主动上报方式进行解析处理，参考register_app_at_urc接口即可。
 */
int at_ReqAndRsp_to_ps(char *req_at, char *info_fmt, int timeout, ...);

/**
 * @brief send at req to ext MCU,and parase reponse str to pval memory
 * @param req_at [IN] req at str,such as "AT+EXTATs=1,2,CMNET"
 * @param info_fmt [IN] response str format
 * @param pval [OUT] result memory
 * @param timeout [IN] wait rsp time,if timeout ,user need resend or process exception
 * @return parase result,0 is success,see  @ref  AT_XY_ERR
 * @note   暂未使用！！！
 * @par example
 * @code
	int ret = 0;
	ret = at_ReqAndRsp_to_ext("AT+EXTAT=1\r", NULL,  NULL, 0);
	if(ret == 0)
		xy_printf("success");

	char *pstr = k_malloc(50);
	memset(pstr,0,50);
	ret = at_ReqAndRsp_to_ext("AT+EXTAT\r", "%50s",  (void**)&pstr, 1);
	if(ret == 0)
		xy_printf("success");

	int n1 = 0;
	short n2 = 0;
	char n3 = 0;
	char *p2[] = {&n1,&n2,&n3};
	ret = at_ReqAndRsp_to_ext("AT+EXTAT?\r", "%d,%2d,%1d",	(void**)p2,0);
	if(ret == 0)
		xy_printf("success");
 * @endcode
 */
int at_ReqAndRsp_to_ext(char *req_at, char *info_fmt, void **pval,int timeout);

/**
 * @brief AT错误码构建函数
 * @param err_no [IN] 错误码，参见AT_XY_ERR
 * @param file [IN] 调用函数所在文件名
 * @param line [IN] 调用函数所在行
 * @return 返回字符串堆指针
 * @attention  慎用！建议客户直接使用@ref AT_ERR_BUILD
 */
char *at_err_build_info(int err_no, char *file, int line);


