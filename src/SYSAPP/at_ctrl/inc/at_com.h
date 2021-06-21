#pragma once

/*******************************************************************************
 *                           Include header files                              *
 ******************************************************************************/
 #include "xy_at_api.h"
 #include "xy_utils.h"

/*******************************************************************************
 *                             Macro definitions                               *
 ******************************************************************************/
#define AT_CMD_MAX_LEN 128   //uart receive  max size once
#define AT_WAKEUP_DROP_LEN 4 //approximate string matching,to solve the problem of losing front strings by standby

/*******************************************************************************
 *                             Type definitions                                *
 ******************************************************************************/
struct at_serv_proc_e
{
	char *at_prefix;
	ser_req_func proc;
};

struct at_user_serv_proc_e
{
	struct at_user_serv_proc_e *next;
	ser_req_func proc;
	char at_prefix[0];
};

struct at_inform_proc_e
{
	char *at_prefix;
	inform_act_func proc;
};

struct at_user_inform_proc_e
{
	struct at_user_inform_proc_e *next;
	inform_act_func proc;
	char at_prefix[0];
};

struct at_fifo_msg
{
	char data[0];
};

/*******************************************************************************
 *                       Global variable declarations                          *
 ******************************************************************************/
extern int g_urc_format_ctl;

/*******************************************************************************
 *                       Global function declarations                          *
 ******************************************************************************/
/**
 * @brief parse request at cmd and get its prefix and params
 * @param at_cmd [IN] at cmd data
 * @param at_prefix [OUT]record the at_prefix after parse, if prefix format invalid or parse error,return NULL
 * @param is_all [IN] value[0,1], 0:get prefix such as +WORKLOCK,1:get prefix such as AT+WORKLOCK
 * @note Notice the function may change g_req_type!!!
 * @return return params first char address
 */
char *at_get_prefix_and_param(char *at_cmd, char *at_prefix, char is_all);

/**
 * @brief  parase param value to pval memory
 * @param fmt  [IN] format,such as "%1d,%d,%s"
 * @param buf  [IN] AT string param head,such as "2,5,cmnet"
 * @param pval [OUT] result memory
 * @return parase result,0 is success,see  AT_XY_ERR
 * @par example:
 * @code
	char buf[]="1,2,\"test\"";
	char n1 = 0;
	int n2 = 0;
	char *n3 = xy_zalloc(strlen(buf));
	char *p[] = {&n1,&n2,n3};
	ret = at_parse_param_2("%1d,%d,%s", buf, (void**)p);
	//result : n1==1 n2==2 n3=="test"
	ret = at_parse_param_2("%1d,%d,%2s", buf, (void**)p);
	//result : n1==1 n2==2 n3=="te"
   @endcode
 * @warning  不建议使用该接口，统一使用at_parse_param接口
 */
int at_parse_param_2(char *fmt, char *buf, void **pval);

/**
 * @brief  增加成功解析的参数个数入参, 同时,该接口实现了对字符串中包含的转义字符解析功能
 * @brief  由于支持字符串中转义字符解析，输入的字符串必须用""包含！！
 * @param fmt  [IN] 格式化字符串,例如 "%1d,%d,%s"
 * @param buf  [IN] 待解析的字符串头地址,例如 "2,5,cmnet"
 * @param parse_num  [OUT] 实际解析的参数个数，例如AT+HTTPHEADER=1,"" parse_num=2，而AT+HTTPHEADER=1  parse_num=1
 * @return 解析结果 参考@AT_XY_ERR
 * @par 示例:
 * @code
	char buf[]="1,2,\"test\"";
	int  parse_num = 0;
	char param1[16] = {0};
	char param2 = 0;
	int  param3 = 0;
	ret = at_parse_param_3("%1d,%d", buf, parse_num, &param2, &param3);
	//result : param2=1 param3=2 parse_num=2
	ret = at_parse_param_3("%1d,%d,%s", buf, parse_num, &param2, &param3, param1);
	//result : param1="test" param2=1 param3=2 parse_num=3
	ret = at_parse_param_3(",,%s", buf, parse_num, param1);
	//result : param1="test" parse_num=1
	char buf[]="1,2,\101test\102"";
	ret = at_parse_param_3("%1d,%d,%s", buf, parse_num, &param2, &param3, param1);
	//result : param1="AtestB" param2=1 param3=2 parse_num=3
 * @note 字符串转义字符解析示例: "\101xinyin\x42" => "AxinyiB"
 * @note 字符串转义字符解析示例: "\r\nxinyi\?" = "'\r''\n'xinyi?" 原先字符串中\r占用两个字节，解析成功后转为'\r' ACSII字符占用一个字节
 * @warning  该接口目前仅用于芯翼内部，解决特殊的字符转译问题
 */
int at_parse_param_3(char *fmt, char *buf, int* parse_num,...);

/**
 * @brief shape_special_mid_rsp
 * @param prefix [IN] 
 * @param at_cmd [IN]
 * @note
 */
//char *shape_special_mid_rsp(char *prefix, char *at_cmd);

/**
 * @brief check at request command is critical or not
 * @param at_str [IN] at request command string
 * @return 1: critical command,0: nor critical command
 * @note critical Req must special proc,critical command can see
 */
int is_critical_req(char *at_str);

/**
 * @brief at_check_sms_fun
 * @param at_cmd_prefix [IN] 
 * @return
 * @note
 */
int at_check_sms_fun(char *at_cmd_prefix);

/**
 * @brief format urc command
 * @param at_str [IN] urc at command string
 * @param size [IN] size
 * @note
 */
char *format_urc_cmd(char *at_str, int *size);

int parse_param(char *fmt_param, char *buf, void **pval, int is_strict);

int parse_param_vp(char *fmt_param, char *buf, int is_strict, va_list *ap);

int drop_unused_urc(char *buf);

void at_standby_set();

void at_standby_unset();

void at_standby_clear();

int at_deepsleep_Check();

