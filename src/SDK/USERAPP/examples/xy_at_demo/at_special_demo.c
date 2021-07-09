/**
 * @file at_special_demo.c
 * @brief 特殊的AT命令定制，发送AT+CGSN和AT+CIMI直接获得imei号和imsi号，不带有+xxxx:的前缀。用户如有此类需求可参考此代码
 * @version 1.0
 * @date 2021-02-24
 * @attention  该demo仅供客户参考！！！
 * @copyright Copyright (c) 2021  芯翼信息科技有限公司
 * 
 */

#if DEMO_TEST

#include "xy_api.h"

//AT+CGSN
int at_special_CGSN_req(char *at_buf, char **prsp_cmd)
{
	char *imei_temp = xy_zalloc(16);

	(void) at_buf;

	if (g_req_type == AT_CMD_ACTIVE)
	{
		if (at_ReqAndRsp_to_ps("AT+CGSN=1\r\n", "%s", 5, imei_temp) != AT_OK)
		{
			xy_printf("get imei fail!!!");
			*prsp_cmd = AT_ERR_BUILD(ATERR_NOT_ALLOWED);
			goto end;
		}
		*prsp_cmd = xy_zalloc(32);
		sprintf(*prsp_cmd, "\r\n%s\r\n\r\nOK\r\n", imei_temp);
	}
	else
	{
		xy_free(imei_temp);
		return AT_FORWARD;
	}

end:	
	xy_free(imei_temp);
	return AT_END;
}

//AT+CIMI
int at_special_CIMI_req(char *at_buf, char **prsp_cmd)
{
	char *imsi_temp = xy_zalloc(16);

	(void) at_buf;

	if(g_req_type == AT_CMD_ACTIVE)
	{	
		if(at_ReqAndRsp_to_ps("AT+CIMI\r\n", "%s", 5, imsi_temp) != AT_OK)
		{
			xy_printf("get imsi fail!!!");		
			*prsp_cmd = AT_ERR_BUILD(ATERR_NOT_ALLOWED);
			goto end;
		}
		*prsp_cmd = xy_zalloc(32);
		sprintf(*prsp_cmd,"\r\n%s\r\n\r\nOK\r\n",imsi_temp);
	}
	else
	{
		xy_free(imsi_temp);
		return AT_FORWARD;
	}
	
end:	
	xy_free(imsi_temp);
	return AT_END;
}

void init_special_at_demo(void)
{
	register_app_at_req("AT+CGSN",	at_special_CGSN_req);
	register_app_at_req("AT+CIMI",	at_special_CIMI_req);
}

#endif //DEMO_TEST

