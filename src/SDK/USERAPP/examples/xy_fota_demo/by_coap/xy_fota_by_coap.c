#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <ctype.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <lwip/netdb.h>
#include "cJSON.h"

#include "coap.h"
#include "lwip/sockets.h"
#include "xy_utils.h"
#include "xy_utils.h"
#include "xy_coap_api.h"
#if 0
static char imei[16] = "864948040254547";
static char version[32] = "V1100B00904R00C0603";
static char file_path[BUFSIZE];
static char file_name[BUFSIZE];
static int  file_size;
int  fotacheck = 7;//fota Start the cycle
int  client_state = UNREGISTER;//fota state

extern osMessageQueueId_t fota_demo_q;
void xy_fota_by_coap_hook(const uint8_t *data, size_t len)
{
    fota_demo_msg *msg;
    char *buf = xy_zalloc(len + 1);

    int i=0;
    for(i = 0; i < len; i++)
    {
        if(data[i] == '"')
            buf[i] = '\"';
        else
            buf[i] = data[i];    
    }

    //xy_printf("[king][append_to_output]%s", buf);
    if(client_state != DOWNLOADING)
        analy_json_string(buf);
    else
    {
        msg = xy_zalloc(sizeof(fota_demo_msg));
        msg->last_flag = 0;
        msg->data = xy_zalloc(len);
        memcpy(msg->data, buf, len + 1);
        msg->data_len = len;
        osMessageQueuePut(fota_demo_q, &msg, 0, osWaitForever);
    }
    xy_free(buf);
}
 

void client_static_update(char* jsonstr)
{
    cJSON * root = NULL;
    cJSON * item = NULL;//cjson对象

    root = cJSON_Parse(jsonstr); 
    if (!root) 
    {
        softap_printf(USER_LOG, WARN_LOG, "\n[King]Error before: [%s]\n",cJSON_GetErrorPtr());
    }

    cJSON_Delete(root);
}

void analy_json_string(char* jsonstr)
{
    cJSON * root = NULL;
    cJSON * item = NULL;//cjson对象

    root = cJSON_Parse(jsonstr); 
    if (!root) 
    {
        softap_printf(USER_LOG, WARN_LOG, "\n[King]Error before: [%s]\n",cJSON_GetErrorPtr());
    }
    
    xy_printf("%s\n\n", cJSON_Print(root));
    item = cJSON_GetObjectItem(root, "commandtype");
    if(!strcmp(item->valuestring, "REGISTER"))
    {
        item = cJSON_GetObjectItem(root, "cmdstatus");
        item = cJSON_GetObjectItem(item, "resultcode");
        if(item->valueint == 0)
        {
            softap_printf(USER_LOG, WARN_LOG, "Register success!\n");
            client_state = REGISTER;
        }
        else
        {
            softap_printf(USER_LOG, WARN_LOG, "Register error:%d!\n", item->valueint);
            quit = 1;
        }
        
        item = cJSON_GetObjectItem(root, "response");
        item = cJSON_GetObjectItem(item, "status");
        if(item->valueint == 0)
        {
            softap_printf(USER_LOG, WARN_LOG, "Client need update!\n");
            client_state = IDLE;
        }
        else
        {
            softap_printf(USER_LOG, WARN_LOG, "Client need not update!\n");
            client_state = UNREGISTER;
            quit = 1;
        }

        item = cJSON_GetObjectItem(item, "fotacheck");
        if(item->valueint != fotacheck)
        {
            softap_printf(USER_LOG, WARN_LOG, "Client fotacheck need update!\n");
            fotacheck = item->valueint;
        }
        else
        {
            softap_printf(USER_LOG, WARN_LOG, "Client fotacheck need not update!\n");
        }
    }
    else if(!strcmp(item->valuestring, "FOTA_STATUS_UPDATE"))
    {
        item = cJSON_GetObjectItem(root, "cmdstatus");
        item = cJSON_GetObjectItem(item, "resultcode");
        if(item->valueint == 0)
        {
            softap_printf(USER_LOG, WARN_LOG, "FOTA_STATUS_UPDATE success!\n");
        }
        else
            softap_printf(USER_LOG, WARN_LOG, "FOTA_STATUS_UPDATE error:%d!\n", item->valueint);

        
        item = cJSON_GetObjectItem(root, "response");
        item = cJSON_GetObjectItem(item, "status");
        if(item->valueint == 0)
        {
            softap_printf(USER_LOG, WARN_LOG, "Client fota status update success!\n");
            if(client_state == IDLE)
            {
                item = cJSON_GetObjectItem(root, "response");
                item = cJSON_GetObjectItem(item, "uri");
                strcpy(file_path, item->valuestring);
                softap_printf(USER_LOG, WARN_LOG, "[king][client_static_update]URI:%s\n", item->valuestring);

                item = cJSON_GetObjectItem(root, "response");
                item = cJSON_GetObjectItem(item, "file");
                strcpy(file_name, item->valuestring);
                softap_printf(USER_LOG, WARN_LOG, "[king][client_static_update]File:%s\n", item->valuestring);

                item = cJSON_GetObjectItem(root, "response");
                item = cJSON_GetObjectItem(item, "size");
                file_size = item->valueint;
                softap_printf(USER_LOG, WARN_LOG, "[king][client_static_update]Size:%d\n", item->valueint);
                
                client_state = DOWNLOADING;
            }

            if(client_static == DOWNLOADED)
            {                
                client_static = UPDATING;
            }

            if(client_static == UPDATING)
            {                
                softap_printf(USER_LOG, WARN_LOG, "Client start update\n");
                quit = 1;
            }
        }
        else
        {
            softap_printf(USER_LOG, WARN_LOG, "Client fota status update error!:%d\n", item->valueint);
            quit = 1;
        }
    }
    else
    {
        softap_printf(USER_LOG, WARN_LOG, "analy_json_string CommandType ERROR!\n");
    }

    cJSON_Delete(root);
}

char *get_json_string(cmd_type cmdtype, char *message)
{
    cJSON * root =  cJSON_CreateObject();
    cJSON * item =  cJSON_CreateObject();

    cJSON_AddItemToObject(root, "imei", cJSON_CreateString(imei));
    
    if(cmdtype == REGISTER)
    {
        cJSON_AddItemToObject(root, "commandtype", cJSON_CreateString("REGISTER"));
        cJSON_AddItemToObject(item, "version", cJSON_CreateString(version));
        cJSON_AddItemToObject(item, "energy", cJSON_CreateNumber(30));
        cJSON_AddItemToObject(item, "txpower", cJSON_CreateNumber(16));
        cJSON_AddItemToObject(item, "fotacheck", cJSON_CreateNumber(fotacheck));
    }
    else
    {
        cJSON_AddItemToObject(root, "commandtype", cJSON_CreateString("FOTA_STATUS_UPDATE"));
        cJSON_AddItemToObject(item, "status", cJSON_CreateNumber((int)cmdtype));
        cJSON_AddItemToObject(item, "message", cJSON_CreateString(message));
    }

    cJSON_AddItemToObject(root, "command", item);//root节点下添加Command节点

    //xy_printf("%s\n\n", cJSON_PrintUnformatted(root));
    return cJSON_PrintUnformatted(root);

}

static coap_context_t *ctx = NULL;
int chat_with_cloud_start()
{
    int count = -1;

    //创建coap客户端
    ctx = xy_coap_client_create("ota.simteek.com", 5683);
    if ( !ctx ) {
        softap_printf(USER_LOG, WARN_LOG, "cannot create context\n" );
        goto end;
    }

    //全局变量初始化
    xy_config_coap_head(6666, NULL, 0);
    while((client_state != UPDATING) && !quit)
    {
        if(count++ > 3)
        {
            softap_printf(USER_LOG, WARN_LOG, "Chat with cloud error\n" );
            goto end;
        }
        
        softap_printf(USER_LOG, WARN_LOG, "[king][xibo_fota_tsak]client_state:%d", client_state);
        if(client_state == UNREGISTER)
        {
            xy_config_coap_option(LIBCOAP_COAP_OPTION_URI_PATH, "register");
            xy_coap_send(ctx, "post", get_json_string(REGISTER, NULL));
        }

        if(client_state == REGISTER)
            goto end;

        if(client_state == IDLE)
        {
            xy_config_coap_option(LIBCOAP_COAP_OPTION_URI_PATH, "update");
            xy_coap_send(ctx, "post", get_json_string(IDLE, NULL));
        }

        if(client_state == DOWNLOADING)
        {
            xy_config_coap_option(LIBCOAP_COAP_OPTION_URI_PATH, "update");
            xy_coap_send(ctx, "post", get_json_string(DOWNLOADING, NULL));
            xy_config_coap_option(0, NULL);
            xy_coap_send(ctx, "get", file_name);                
        }

        osDelay(1000);
    }
    
    return 0;

end: 
    if(ctx != NULL)
    {
        xy_coap_client_delete(ctx);
        ctx = NULL;
    }
    return 1;    
}

void chat_with_cloud_end(int result)
{
    if(0 == result)
    {
        if(client_state == DOWNLOADED)
        {
            xy_coap_send(ctx, "post", get_json_string(DOWNLOADED, NULL));
        }

        if(client_state == UPDATING)
            xy_coap_send(ctx, "post", get_json_string(UPDATING, NULL));
    }
    else
    {
        xy_coap_send(ctx, "post", get_json_string(DOWNLOADED, "Dtle error!"));
    }

    if(ctx != NULL)
    {
        xy_coap_client_delete(ctx);
        ctx = NULL;
    }
}

#endif

