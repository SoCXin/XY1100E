/** 
* @file        at_NFTCIMEI_demo.c
* @ingroup     at
* @brief       部分客户需要定制一些自己的3GPP扩展AT请求命令，需要涉及与3GPP的AT命令交互，可参看此DEMO
* @attention   客户仅能调用at_api.h中的接口进行AT相关开发
* @note         !!!该源文件默认不编译，用户不得直接在该文件中修改！！！\n
*               而应该是自行创建新的私有的源文件，将demo示例代码拷贝过去后再做修改。
*/

#if DEMO_TEST

#include "xy_api.h"

int at_proc_NFTCIMEI_req(char *at_buf, char **prsp_cmd)
{
	if(g_req_type == AT_CMD_REQ)
	{
		char *imei_user = NULL;
		imei_user = xy_zalloc(strlen(at_buf));
		int res = -1;

		if (at_parse_param("%s", at_buf, imei_user) != AT_OK) 
		{
			res = ATERR_PARAM_INVALID;
			goto END_PROC;
		}
		char imei_serch[16] = {0};
		
		if (at_ReqAndRsp_to_ps("AT+CGSN=1\r\n","%s", 5, imei_serch)!=0)
		{
			xy_printf("imei find fail,call dsp!!!");
			res = ATERR_PARAM_INVALID;
			goto END_PROC;
		}
		
		*prsp_cmd = xy_zalloc(32);
		int state = 0;
		
		if(strcmp(imei_serch,imei_user))
		{			
			state = 0;
		}
		else
		{
			state = 1;
		}
		snprintf(*prsp_cmd, 32, "\r\n+NFTCIMEI:%d\r\n\r\nOK\r\n",state);
		xy_free(imei_user);
		return AT_END;
END_PROC:
		if(imei_user != NULL)
			xy_free(imei_user);
		*prsp_cmd = AT_ERR_BUILD(res);
		return AT_END;
	}
	else
	{
		*prsp_cmd = AT_ERR_BUILD(ATERR_PARAM_INVALID);
		return AT_END;
	}
}

//call by user_task_init
int init_NFTCIMEI_demo()
{
	register_app_at_req("AT+NFTCIMEI",	at_proc_NFTCIMEI_req);

	return 0;
}

#endif
