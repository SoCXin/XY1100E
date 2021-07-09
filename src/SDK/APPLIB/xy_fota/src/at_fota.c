#if XY_FOTA
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <ctype.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <lwip/netdb.h>

#include "xy_utils.h"
#include "xy_at_api.h"
#include "xy_fota.h"
#include "at_global_def.h"
#include "at_com.h"
typedef enum
{
    AT_FOTA_IDLE,
    AT_FOTA_DOWNING,
    AT_FOTA_DOWNED,
    AT_FOTA_UPDATE
} at_fota_cmd_e;


static at_fota_cmd_e fota_status = AT_FOTA_IDLE;
static int block_num = 0;
static ota_context_t * ota_context = NULL;

static uint8_t chk_sum(const void*data,uint32_t length)
{
	uint8_t ret = 0;
	int   val = 0;
	unsigned char *tmp_ptr = (unsigned char *)data;
	unsigned char *tail;

	xy_assert(((int)data%4) == 0);
	while(length>0)
	{
		val ^= *tmp_ptr;
		tmp_ptr++;
		length--;
	}
	ret ^= *((unsigned char *)&val);
	ret ^= *((unsigned char *)&val+1);
	ret ^= *((unsigned char *)&val+2);
	ret ^= *((unsigned char *)&val+3);

	tail = (unsigned char *)tmp_ptr;
	while((uint32_t)tail < (int)data+length)
	{
		ret ^= *tail;
		tail++;
	}
	return ret;
}


int at_fota_Flash_Erase(void)
{
    ota_update_deInit(ota_context);
    ota_context = NULL;

    ota_context = ota_update_Init();
    block_num = 0;
    fota_status = AT_FOTA_DOWNING;
    return 0;
}

int at_fota_downing(char *at_buf)
{
    if(fota_status != AT_FOTA_DOWNING)
        return -1;

    int len = -1;
    int sn = -1;
    unsigned char crc_sum = 0;
    char *crc_data = xy_zalloc(3);
    char *save_data = NULL;
    char *src_data = xy_zalloc(strlen(at_buf));
    void *p[] = {NULL,&sn,&len,src_data,crc_data};
    
    if (at_parse_param_2("%d,%d,%d,%s,%s", at_buf, p) != AT_OK)
        goto ERR_PROC;

    if(sn != block_num)
        goto ERR_PROC;
        
    if(len>512 || len<=0 || strlen(src_data)!=len * 2)
        goto ERR_PROC;
    
    save_data = xy_zalloc(len);
    if (hexstr2bytes(src_data, len * 2, save_data, len) == -1)
        goto ERR_PROC;
    
    if (hexstr2bytes(crc_data, 2, (char *)(&crc_sum), 1) == -1)
        goto ERR_PROC;

    int sum = chk_sum(save_data, len);
    if(sum != crc_sum)
        goto ERR_PROC;
      
    if(ota_downlink_packet_proc(ota_context, save_data, len))
        goto ERR_PROC;

    block_num++;

    if(save_data)
        xy_free(save_data);
    if(src_data)
        xy_free(src_data);
    if(crc_data)
        xy_free(crc_data);

    return 0;

ERR_PROC:
    if(save_data)
        xy_free(save_data);
    if(src_data)
        xy_free(src_data);
    if(crc_data)
        xy_free(crc_data);

    return -1;


}

int at_fota_check(void)
{
    if((fota_status != AT_FOTA_DOWNING) || (block_num == 0))
        return -1;

    if(ota_delta_check_sync(ota_context))
        return -1;

    fota_status = AT_FOTA_DOWNED;
    return 0;
}

int at_fota_update(void)
{
    char *at_str = NULL;
    
    if(fota_status != AT_FOTA_DOWNED)
        return -1;
    
    fota_status = AT_FOTA_UPDATE;

        
    at_str = xy_zalloc(64);
    snprintf(at_str, 64, "\r\n+FIRMWARE UPDATING\r\n");
    send_rsp_str_to_ext(at_str);
    osDelay(50);
	xy_free(at_str);
    
    ota_update_start(ota_context);
    return 0;
}

/*****************************************************************************
 Function    : at_NFWUPD_req
 Description :   
 Input       : at_buf   ---data buf
               prsp_cmd ---response cmd
 Output      : None
 Return      : AT_END
 Eg          : AT+NFWUPD=<cmd>[,<sn>,<len>,<data>,<crc>]
               AT+NFWUPD=?
 *****************************************************************************/
int at_NFWUPD_req(char *at_buf, char **prsp_cmd)
{
    if(g_req_type == AT_CMD_REQ)
    {
        int cmd = -1;
        //int ret = -1;
        void *p[] = {&cmd};

        if (at_parse_param_2("%d", at_buf, p) != AT_OK)
        {
            *prsp_cmd = AT_ERR_BUILD(ATERR_PARAM_INVALID);
            return AT_END;
        }

        switch(cmd)
        {
        case 0:
            if(at_fota_Flash_Erase())
                *prsp_cmd = AT_ERR_BUILD(ATERR_PARAM_INVALID);
            break;
        case 1:
            if(at_fota_downing(at_buf))
                *prsp_cmd = AT_ERR_BUILD(ATERR_PARAM_INVALID);
            break;
        case 2:
            if(at_fota_check())
                *prsp_cmd = AT_ERR_BUILD(ATERR_PARAM_INVALID);
            break;
        case 5:
            if(at_fota_update())
                *prsp_cmd = AT_ERR_BUILD(ATERR_PARAM_INVALID);
            break;
        default:
            *prsp_cmd = AT_ERR_BUILD(ATERR_PARAM_INVALID);
        }

    }
    else if(g_req_type == AT_CMD_TEST)
    {
        *prsp_cmd = xy_zalloc(40);
        snprintf(*prsp_cmd, 40, "\r\n+NFWUPD:(0,1,2,5)\r\n\r\nOK\r\n");
    }
    else
        *prsp_cmd = AT_ERR_BUILD(ATERR_PARAM_INVALID);

    return AT_END;
}

#endif

