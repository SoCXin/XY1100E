/*******************************************************************************
 *                           Include header files                              *
 ******************************************************************************/
#include <coap.h>
#include "lwip/sockets.h"
#include "xy_utils.h"
#include "at_global_def.h"
#include "xy_at_api.h"
extern bool xy_coap_messgae_process_can_quit(coap_context_t *ctx);
extern void xy_coap_clear_option();

typedef struct CoapOptionData
{
    unsigned char optNum;
    unsigned char *optValue;
} CoapOptionData;

/*******************************************************************************
 *                        Global variable definitions                          *
 ******************************************************************************/
coap_context_t* coap_client= NULL;

osThreadId_t g_coaprecvpacket_handle = NULL;
int g_stop_recv_packet_flag = 0;

/*******************************************************************************
 *                      Global function implementations                        *
 ******************************************************************************/

/*******************************************************************************************
 Function    : coap_recv_packet_task
 Description : receive COAP packet function task
 Input       : void
 Return      : void
 *******************************************************************************************/
void coap_recv_packet_task()
{
    softap_printf(USER_LOG, WARN_LOG,"[COAP] recv packet thread begin\n");
    //wait PDP active
    if(xy_wait_tcpip_ok(2*60) == XY_ERR)
        xy_assert(0);

    xy_coap_asy_receive(coap_client,&g_stop_recv_packet_flag);

	g_coaprecvpacket_handle = NULL;
	osThreadExit();

    softap_printf(USER_LOG, WARN_LOG,"[COAP] recv packet thread end\n");
}

/*******************************************************************************************
 Function    : coap_task_create
 Description : create recv COAP packet task
 Input       : void
 Return      : void
 *******************************************************************************************/
void coap_task_create()
{
    if (g_coaprecvpacket_handle != NULL)
        return;

    g_coaprecvpacket_handle = osThreadNew((osThreadFunc_t)(coap_recv_packet_task), NULL, "coap_recv_packet", 0x1000, XY_OS_PRIO_NORMAL1);
}
/*****************************************************************************
 Function    : at_COAPCREATE_req
 Description : create COAP client
 Input       : at_buf   ---data buf
               prsp_cmd ---response cmd
 Output      : None
 Return      : AT_END
 Eg          : +COAPCREATE=<server>,<port>
 *****************************************************************************/
int at_COAPCREATE_req(char *at_buf, char **prsp_cmd)
{
    unsigned short port = 0;
    char * remote_ip    = xy_zalloc(strlen(at_buf));
    void *p[] = {remote_ip,&port};

    softap_printf(USER_LOG, WARN_LOG,"[COAP] CREATE BEGIN\n");

    if(g_req_type != AT_CMD_REQ)
    {
        *prsp_cmd = AT_ERR_BUILD(ATERR_PARAM_INVALID);
        goto ERR_PROC;
    }

    if (!ps_netif_is_ok()) {
        *prsp_cmd = AT_ERR_BUILD(ATERR_NOT_NET_CONNECT);
        goto ERR_PROC;
    }

    if (at_parse_param_2("%s,%d,", at_buf, p) != AT_OK || !strcmp(remote_ip,"") || port < 1 ||port > 65535 || coap_client != NULL)
    {
        *prsp_cmd = AT_ERR_BUILD(ATERR_PARAM_INVALID);
        goto ERR_PROC;
    }

    coap_client = xy_coap_client_create(remote_ip,port);

    if (coap_client == NULL ) /*create failed*/
    {
        *prsp_cmd = AT_ERR_BUILD(ATERR_NOT_ALLOWED);
        goto ERR_PROC;
    }

    coap_task_create();
    softap_printf(USER_LOG, WARN_LOG,"[COAP] CREATE END\n");

ERR_PROC:
    if(remote_ip)
        xy_free(remote_ip);
    return AT_END;
}

/*****************************************************************************
 Function    : at_COAPDEL_req
 Description : delete COAP client
 Input       : at_buf   ---data buf
               prsp_cmd ---response cmd
 Output      : None
 Return      : AT_END
 Eg          : +COAPDEL
 *****************************************************************************/
int at_COAPDEL_req(char *at_buf, char **prsp_cmd)
{
    int  time = 0;

    (void) at_buf;

    softap_printf(USER_LOG, WARN_LOG,"[COAP] DEL BEGIN\n");

    if(g_req_type != AT_CMD_REQ && g_req_type != AT_CMD_ACTIVE)
    {
        *prsp_cmd = AT_ERR_BUILD(ATERR_PARAM_INVALID);
        goto ERR_PROC;
    }

    if (!ps_netif_is_ok()) {
        *prsp_cmd = AT_ERR_BUILD(ATERR_NOT_NET_CONNECT);
        goto ERR_PROC;
    }

    if(coap_client == NULL || !xy_coap_messgae_process_can_quit(coap_client))
    {
        *prsp_cmd = AT_ERR_BUILD(ATERR_NOT_ALLOWED);
        goto ERR_PROC;
    }

    g_stop_recv_packet_flag = 1;//stop recv thread
    while(g_coaprecvpacket_handle != NULL)
    {
        time +=200;
        osDelay(200);
        if (time > 2000)
        {
            *prsp_cmd = AT_ERR_BUILD(ATERR_NOT_ALLOWED);
            goto ERR_PROC;
        }

    }
    xy_coap_clear_option();
    xy_coap_client_delete(coap_client);
    coap_client = NULL;
    g_stop_recv_packet_flag = 0;
    softap_printf(USER_LOG, WARN_LOG,"[COAP] DEL END\n");
ERR_PROC:

    return AT_END;
}

/*****************************************************************************
 Function    : at_COAPHEAD_req
 Description : config COAP head
 Input       : at_buf   ---data buf
               prsp_cmd ---response cmd
 Output      : None
 Return      : AT_END
 Eg          : +COAPHEAD=<msgid>,<tkl>,<token>
 *****************************************************************************/
int at_COAPHEAD_req(char *at_buf, char **prsp_cmd)
{
    int  msgid = -1;
    int  tkl   = -1;
    char * token = NULL;
    token = xy_zalloc(strlen(at_buf));
    void *p[] = {&msgid,&tkl,token};

    softap_printf(USER_LOG, WARN_LOG,"[COAP] CONFIG HEAD BEGIN\n");

    if(g_req_type != AT_CMD_REQ)
    {
        *prsp_cmd = AT_ERR_BUILD(ATERR_PARAM_INVALID);
        goto ERR_PROC;
    }

    if (!ps_netif_is_ok()) {
        *prsp_cmd = AT_ERR_BUILD(ATERR_NOT_NET_CONNECT);
        goto ERR_PROC;
    }

    if (at_parse_param_2("%d,%d,%s", at_buf, p) != AT_OK  ||msgid < 0 || msgid > 65535||tkl < 0 || tkl > 8 ||strlen(token) != tkl)
    {
        *prsp_cmd = AT_ERR_BUILD(ATERR_PARAM_INVALID);
        goto ERR_PROC;
    }

    if (coap_client == NULL )
    {
        *prsp_cmd = AT_ERR_BUILD(ATERR_NOT_ALLOWED);
        goto ERR_PROC;
    }

    xy_config_coap_head(msgid,token,tkl);

    softap_printf(USER_LOG, WARN_LOG,"[COAP] CONFIG HEAD END\n");

ERR_PROC:
    if(token)
        xy_free(token);
    return AT_END;
}

/*****************************************************************************
 Function    : at_COAPOPTION_req
 Description : config COAP options
 Input       : at_buf   ---data buf
               prsp_cmd ---response cmd
 Output      : None
 Return      : AT_END
 Eg          : +COAPOPTION=<opt_count>,<opt_name>,<opt_value>
 *****************************************************************************/
int at_COAPOPTION_req(char *at_buf, char **prsp_cmd)
{
    int  i;
    int  opt_count = 0;
    char *form  =NULL;
    //char* test = ",%d,%c";
    char *format   =xy_zalloc(strlen(at_buf));
    void *p[] = {&opt_count};

    softap_printf(USER_LOG, WARN_LOG,"[COAP] CONFIG OPTION BEGIN\n");

    if(g_req_type != AT_CMD_REQ)
    {
        *prsp_cmd = AT_ERR_BUILD(ATERR_PARAM_INVALID);
        return AT_END;
    }

    if (!ps_netif_is_ok()) {
        *prsp_cmd = AT_ERR_BUILD(ATERR_NOT_NET_CONNECT);
        return AT_END;
    }

    if (at_parse_param_2("%d", at_buf, p) != AT_OK )
    {
        *prsp_cmd = AT_ERR_BUILD(ATERR_PARAM_INVALID);
        return AT_END;
    }

    softap_printf(USER_LOG, WARN_LOG,"[COAP] opt_count = %d\n",opt_count);
    /*dynamic parse param*/
    void *param[2*opt_count];
    CoapOptionData option[opt_count];
    memset (&option,0,sizeof(CoapOptionData));
    form =format;
    for(i=0;i <2*opt_count;i+=2)
    {
        softap_printf(USER_LOG, WARN_LOG,"[COAP] i = %d",i);
        param[i] =&option[i/2].optNum;
        option[i/2].optValue = xy_zalloc(strlen(at_buf));
        param[i+1] =option[i/2].optValue;

        *form++ = ',';
        *form++ = '%';
        *form++ = 'd';
        *form++ = ',';
        *form++ = '%';
        *form++ = 's';
    }

    *form++ = '\0';
    if (at_parse_param_2(format, at_buf, param) != AT_OK )
    {
        *prsp_cmd = AT_ERR_BUILD(ATERR_PARAM_INVALID);
        goto ERR_PROC;
    }

    if (coap_client == NULL )
    {
        *prsp_cmd = AT_ERR_BUILD(ATERR_NOT_ALLOWED);
        goto ERR_PROC;
    }

    xy_coap_clear_option();

    for(i=0;i <opt_count;i++)
    {
        xy_config_coap_option(option[i].optNum,option[i].optValue);
    }

    softap_printf(USER_LOG, WARN_LOG,"[COAP] CONFIG OPTION END\n");

ERR_PROC:
    for(i=0;i <opt_count;i++)
    {
        if(option[i].optValue)
            xy_free(option[i].optValue);
    }
    if(format)
        xy_free(format);
    return AT_END;
}

/*****************************************************************************
 Function    : at_COAPSEND_req
 Description : send COAP messages
 Input       : at_buf   ---data buf
               prsp_cmd ---response cmd
 Output      : None
 Return      : AT_END
 Eg          : +COAPSEND=<type>,<method>,<data_len>,<data>
 *****************************************************************************/
int at_COAPSEND_req(char *at_buf, char **prsp_cmd)
{
    int  data_len = 0;
    char *tans_data = NULL;
    char *method = xy_zalloc(strlen(at_buf));
    char *type   = xy_zalloc(strlen(at_buf));
    char *data   = xy_zalloc(strlen(at_buf));
    void *p[] = {type,method,&data_len,data};

    softap_printf(USER_LOG, WARN_LOG,"[COAP] SEND BEGIN\n");

    if(g_req_type != AT_CMD_REQ)
    {
        *prsp_cmd = AT_ERR_BUILD(ATERR_PARAM_INVALID);
        goto ERR_PROC;
    }

    if (!ps_netif_is_ok()) {
        *prsp_cmd = AT_ERR_BUILD(ATERR_NOT_NET_CONNECT);
        goto ERR_PROC;
    }

    if (at_parse_param_2("%s,%s,%d,%s", at_buf, p) != AT_OK ||strlen(method) > 6||strlen(type) > 3|| data_len > 1000 || data_len < 0 || strlen(data) != data_len * 2)
    {
        *prsp_cmd = AT_ERR_BUILD(ATERR_PARAM_INVALID);
        goto ERR_PROC;
    }

    if (coap_client == NULL )
    {
        *prsp_cmd = AT_ERR_BUILD(ATERR_NOT_ALLOWED);
        goto ERR_PROC;
    }

    if(data_len != 0)
    {
        tans_data = xy_zalloc(data_len + 1);
        if (hexstr2bytes(data, data_len * 2, tans_data, data_len) == -1)
        {
            *prsp_cmd = AT_ERR_BUILD(ATERR_PARAM_INVALID);
            goto ERR_PROC;
        }
        tans_data[data_len]= '\0';
        softap_printf(USER_LOG, WARN_LOG,"[COAP] tans_data = %d,%s\n",tans_data,tans_data);
    }

    if (0 != xy_coap_asy_send(coap_client, type,method, tans_data,data_len))
    {
        *prsp_cmd = AT_ERR_BUILD(ATERR_NOT_ALLOWED);
        goto ERR_PROC;
    }

    softap_printf(USER_LOG, WARN_LOG,"[COAP] SEND END\n");

ERR_PROC:
    if(method)
        xy_free(method);
    if(type)
        xy_free(type);
    if(data)
        xy_free(data);
    if(tans_data)
        xy_free(tans_data);
    return AT_END;
}

static uint16_t  s_coap_inited = 0;

void at_coap_init(void)
{
    if(!s_coap_inited)
    {
        s_coap_inited = 1;
    }
    return;
}
