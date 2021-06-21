/** 
* @file        at_server_demo.c
* @ingroup     at
* @brief      芯翼平台支持AT命令通道动态切换为透传输模式，目前常见的透传数据模式有两种，一种是发送数据的AT命令，先发送AT命令，\n
*告知平台后续要发送多长字节的码流，平台会切换为透传模式，待整个长度码流收完后，再切换为AT命令模式；\n
*另一种是类似PPP协议透传模式，通过特定命令，如ATD，通知平台切换成透传模式，然后外部MCU可以与芯翼芯片进行多次的透传数据交互，直至外部MCU发送“+++”切换回AT命令模式.
* @attention    芯翼平台通过g_at_transparent_data_hook回调接口，将底层驱动收到的字符串传递给用户处理，但无法保证字符串是一次接受完毕的，用户需要考虑拼接。
* @note         !!!该源文件默认不编译，用户不得直接在该文件中修改！！！\n
*               而应该是自行创建新的私有的源文件，将demo示例代码拷贝过去后再做修改。
*/


#include  "xy_api.h"

/** 
* @brief   指示当前已接收到多少字节的透传数据
*/
int   g_have_rcved_len = 0;

/** 
* @brief   指示需要接收的透传数据总长度，仅针对指定透传长度的AT命令有效，PPP模式该全局始终为0
*/
int   g_stream_sum_bytes = 0;  

/** 
* @brief   用于指定透传长度的AT命令的数据缓存，PPP模式不得使用该全局
*/
char *g_rcv_transparent_buff = NULL;


/** 
* @brief  缓存接收到的码流最后3个字节，以供匹配是否为+++结束标识
*/
char g_rcved_tail_buff[4] = {0};
int g_rcved_tail_len = 0;


/** 
* @brief  退出透传模式的特殊字符串，通常为PPP协议的“+++”
*/
char *g_Quit_transparent_str = "+++";

osTimerId_t g_fixed_length_timer = NULL;


/** 
* @brief  用户关注！供用户自行处理指定长度的透传数据
*/
void proc_rcved_data(char *buf, unsigned long data_len)
{
	xy_printf("at_rcv[%d]:%s", data_len, buf);
}

/** 
* @brief  用户关注！供用户处理PPP模式接收到的透传数据，需要客户自行拼接和缓存
*/
void proc_ppp_rcved_data(char *buf, unsigned long data_len)
{
	(void) data_len;

	xy_printf("ppp_rcv:%s",buf);
}


/** 
* @brief  用户关注！超时未接收完指定长度的透传数据的容错处理，建议客户切换到AT命令模式，并报错给外部MCU
*/
void fixed_length_timeout_proc(uint16_t *timer)
{
	(void)timer;
}

/**
 * @brief  用户关注！收到切换为透传模式AT指令后，调用该接口切换为透传模式接收状态
 * @param  rcv_len [IN] 待接收的透传数据长度，PPP模式下该值必须填0，以指示长度不确定
 * @param  func [IN] 接收到的透传数据的回调接口，用户实现该接口内容
 * @note   该接口内部实现不建议客户做任何修改，客户仅需关注func回调接口和超时时长及超时处理即可
 */
void goto_transparent_mode(int rcv_len, data_proc_func func)
{
	xy_assert(g_rcv_transparent_buff == NULL);
	
	//PPP mode
	if(rcv_len == 0)
	{
		g_rcv_transparent_buff = NULL;
	}
	//fixed length  mode
	else
	{
		g_rcv_transparent_buff = xy_zalloc(rcv_len + 1);
		if(g_fixed_length_timer == NULL)
			g_fixed_length_timer = osTimerNew((osTimerFunc_t)(fixed_length_timeout_proc), osTimerOnce, NULL, "trans");
	}
	
	g_stream_sum_bytes = rcv_len;
	g_have_rcved_len = 0;
	g_at_transparent_data_hook = func;
	
	xy_standby_lock();
}

/**
 * @brief  切换回AT命令模式，该接口调用点为完成透传数据的传递后，或者接收超时后。
 * @note   该接口内部实现不建议客户做任何修改
 */
void goto_AT_cmd_mode()
{
	if(g_rcv_transparent_buff != NULL)
		xy_free(g_rcv_transparent_buff);
	g_rcv_transparent_buff = NULL;
	g_have_rcved_len = 0;
	g_stream_sum_bytes = 0;
	g_at_transparent_data_hook = NULL;

	memset(g_rcved_tail_buff, 0, 4);
	g_rcved_tail_len = 0;

	if (g_fixed_length_timer != NULL)
	{
		osTimerStop(g_fixed_length_timer);		
	}	
	
	xy_standby_unlock();

	char *trans_end = xy_zalloc(50);
	sprintf(trans_end,"\r\nNO CARRIER\r\n\r\nOK\r\n");
	send_urc_to_ext(trans_end);
	xy_free(trans_end);
}

/**
 * @brief  对于不定长的透传模式，通过识别透传数据尾部的特殊字符串来退出透传模式，如“PPP”
 * @note   该接口内部实现不建议客户做任何修改
 */
int find_Quit_str(char *buf, unsigned long data_len)
{	
	if(g_have_rcved_len <= (int)(strlen(g_Quit_transparent_str)))
	{
		memcpy(g_rcved_tail_buff + g_rcved_tail_len, buf, data_len);
		g_rcved_tail_len += data_len;
	}
	else
	{
		if(data_len >= strlen(g_Quit_transparent_str))
		{
			memset(g_rcved_tail_buff,0,sizeof(g_rcved_tail_buff));
			memcpy(g_rcved_tail_buff,buf+data_len-strlen(g_Quit_transparent_str),strlen(g_Quit_transparent_str));
			g_rcved_tail_len = strlen(g_rcved_tail_buff);
		}
		else
		{
			int recode_buf_len = 0;
			char *recode_buf_local = xy_zalloc(data_len+g_rcved_tail_len+1);
		
			memcpy(recode_buf_local,g_rcved_tail_buff,g_rcved_tail_len);
			recode_buf_len += g_rcved_tail_len;
			memcpy(recode_buf_local+recode_buf_len,buf,data_len);
			recode_buf_len += data_len;
			
			memset(g_rcved_tail_buff,0,sizeof(g_rcved_tail_buff));
			memcpy(g_rcved_tail_buff,recode_buf_local+recode_buf_len-strlen(g_Quit_transparent_str),strlen(g_Quit_transparent_str));
			g_rcved_tail_len = strlen(g_rcved_tail_buff);

			xy_free(recode_buf_local);	
		}		
	}

	if (!strcmp(g_rcved_tail_buff, g_Quit_transparent_str))
	{
		return 1;
	}
	return 0;
}

/**
 * @brief  处理从串口接收到的透传码流，由于码流需要多次才能接收完毕，需要进行超时监控
 * @note   内部区分了指定长度接收和不定长接收两个模式
 */
int rcved_transparent_stream_proc(char *buf, unsigned long data_len)
{
	//指定长度码流接收处理	
	if(g_rcv_transparent_buff != NULL)
	{	
		if(g_stream_sum_bytes != 0)
			xy_assert((unsigned long)(g_have_rcved_len+data_len) <= (unsigned long)(g_stream_sum_bytes));
		
		memcpy(g_rcv_transparent_buff+g_have_rcved_len,buf,data_len);
		g_have_rcved_len = g_have_rcved_len+data_len;
		
		if(g_fixed_length_timer != NULL)
			osTimerStart(g_fixed_length_timer, 1000);

		//have finish transparent data transfer
		if (g_stream_sum_bytes != 0 && g_have_rcved_len == g_stream_sum_bytes)
		{
			proc_rcved_data(g_rcv_transparent_buff, g_have_rcved_len);
			goto_AT_cmd_mode();
		}

		return 1;
	}
	//PPP模式的码流接收处理，需要识别+++结束符
	else 
	{
		proc_ppp_rcved_data(buf, data_len);
		g_have_rcved_len = g_have_rcved_len + data_len;

		if (find_Quit_str(buf, data_len) == 1)
		{
			goto_AT_cmd_mode();
		}
		return 1;
	}
	return 0;
}

/**
 * @brief  PPP协议中的ATD切换透传模式的处理函数
 * @note   
 */
int at_ATD_req(char *at_buf,char **prsp_cmd)
{
	//only for transparent
	if(g_req_type == AT_CMD_ACTIVE)
	{
		if(at_strnstr(at_buf,"*98",3) || at_strnstr(at_buf,"*99",3))
		{
			goto_transparent_mode(0, (data_proc_func)(rcved_transparent_stream_proc));
			*prsp_cmd = xy_zalloc(40);
			snprintf(*prsp_cmd, 40, "\r\nOK\r\n\r\nCONNECTING\r\n");
		}
		else
		{
			*prsp_cmd = AT_ERR_BUILD(ATERR_PARAM_INVALID);
		}
	}	
	return AT_END;
}

/**
 * @brief  对于指定长度的AT命令示例的处理入口，内部解析出指定的长度值，并切换透传模式
 * @note   对于socket、cdp、onenet等数据传输业务，皆可以仿造该命令来实现其他的AT命令透传模式
 * @note   命令格式AT+TRANSPARENTDEMO=<data len>
 */
int at_TRANSPARENTDEMO_req(char *at_buf,char **prsp_cmd)
{
	if(g_req_type == AT_CMD_REQ)
	{
		int data_len = 0;

		if (at_parse_param("%d", at_buf, &data_len) != AT_OK) 
		{
			*prsp_cmd = AT_ERR_BUILD(ATERR_PARAM_INVALID);
			return AT_END;
		}
		else
		{
			goto_transparent_mode(data_len, (data_proc_func)(rcved_transparent_stream_proc));
		}
		*prsp_cmd = xy_zalloc(40);
		snprintf(*prsp_cmd, 40, "\r\nOK\r\n\r\nCONNECT\r\n");
	}
	else
		*prsp_cmd = AT_ERR_BUILD(ATERR_PARAM_INVALID);
	return AT_END;
}


/**
 * @brief  透传模式的AT命令注册初始化接口，该接口在user_task_init被调用
 * @note   用户自行在user_task_init中添加初始化接口调用
 */
int init_transparent_demo(void)
{
	register_app_at_req("AT+TRANSPARENTDEMO",at_TRANSPARENTDEMO_req);
	register_app_at_req("ATD",at_ATD_req);

	return 0;
}

