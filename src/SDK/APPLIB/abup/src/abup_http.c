/*****************************************************************************
* Copyright (c) 2019 ABUP.Co.Ltd. All rights reserved.
* File name: abup_http.c
* Description: http protocol
* Author: WQH
* Version: v1.0
* Date: 20190303
*****************************************************************************/
#include "abup_http.h"
#include "abup_flash.h"
#include "abup_md5.h"
#include <stddef.h>
#include "softap_nv.h"
#include "xy_flash.h"
#include "xy_fota_utils.h"

#define ABUP_REGISTER_URL         "/register"
#define ABUP_HTTP_CONTENT_LENGTH  "Content-Type: application/json\r\nContent-Length: "
#define ABUP_SERVER_DOMAIN        "iotapi.adups.com"
#define ABUP_SERVER_PORT          "80"
#define ABUP_SN_VERSION           "2"

#define ABUP_DOWNLOAD_HOST_MAX_LEN  64
#define ABUP_DOWNLOAD_URI_MAX_LEN   128
#define ABUP_DOWNLOAD_PORT_MAX_LEN  16
#define ABUP_MD5_LENGTH             32
#define ABUP_DOWNLOAD_UNIT_BYTES    4096    //512
#define ABUP_HTTP_MSG_LEN           40

typedef struct
{
    char download_host[ABUP_DOWNLOAD_HOST_MAX_LEN];
    char download_url[ABUP_DOWNLOAD_URI_MAX_LEN];
    char download_md5[ABUP_MD5_LENGTH + 2];
    unsigned int download_delta_id;
    unsigned int download_delta_size;
    char download_port[ABUP_DOWNLOAD_PORT_MAX_LEN];
} download_uri_struct;

typedef struct abup_fota_stru_tag
{
    download_uri_struct download_uri;
    unsigned short download_result;
    int status;
    char msg[ABUP_HTTP_MSG_LEN + 1];
    unsigned short err_type;
    char fota_pid[34];
    char fota_psec[34];
    unsigned short download_unit_index;
    unsigned short download_unit_num;
    bool continue_read;
} ABUP_FOTA_STRU;

static ABUP_FOTA_STRU *g_abup_data = NULL;
static unsigned char abup_update_result = 0xFF;
static ABUP_FOTA_STATUS abup_fota_status = 0;
static signed int delta_data_left_len = 0;
static signed int delta_data_current_num = 0;

signed int abup_get_delta_data_left_len(void)
{
	return delta_data_left_len;
}



bool abup_http_init(void)
{
    if (g_abup_data == NULL)
    {
        g_abup_data = (ABUP_FOTA_STRU *)xy_malloc(sizeof(ABUP_FOTA_STRU));
        if (g_abup_data == NULL)
        {
            xy_printf("ABUP-->abup_http_init: xy_malloc fail.");
            return false;
        }
        memset(g_abup_data, 0, sizeof(ABUP_FOTA_STRU));
    }

    return true;
}

void abup_http_deinit(void)
{
    if (g_abup_data != NULL)
    {
        xy_free(g_abup_data);
        g_abup_data = NULL;
    }
}

char *abup_get_host_domain(void)
{
    if (abup_fota_status == ABUP_FOTA_DL)
    {
        return g_abup_data->download_uri.download_host;
    }
    else
    {
        return ABUP_SERVER_DOMAIN;
    }
}

char *abup_get_host_port(void)
{
    if (abup_fota_status == ABUP_FOTA_DL)
    {
        return g_abup_data->download_uri.download_port;
    }
    else
    {
        return ABUP_SERVER_PORT;
    }
}

void abup_set_fota_status(ABUP_FOTA_STATUS status)
{
    abup_fota_status = status;
}

void abup_get_http_content(char *out_buf, int len)
{
    unsigned char *signptr = 0;

    if (out_buf == NULL)
    {
        xy_printf("ABUP-->abup_get_http_content:out buffer null!");
        return;
    }

    bzero(out_buf, len);

    switch (abup_fota_status)
    {
        case ABUP_FOTA_RG:
            xy_printf("ABUP-->abup_get_http_data:register.");
            signptr = (unsigned char *)abup_hmd5_did_pid_psecret(
                          (char *)abup_get_device_mid(),
                          (char *)abup_get_product_id(),
                          (char *)abup_get_product_sec(),
                          (unsigned int)abup_get_utc_time());
            sprintf((char *)out_buf,
                    "{\"mid\":\"%s\",\"oem\":\"%s\",\"models\":\"%s\",\"platform\":\"%s\",\"deviceType\":\"%s\",\"timestamp\":%d,\"sign\":\"%s\",\"sdkversion\":\"%s\",\"appversion\":\"%s\",\"version\":\"%s\",\"networkType\":\"%s\"}",
                    abup_get_device_mid(),
                    abup_get_manufacturer(),
                    abup_get_model_number(),
                    abup_get_platform(),
                    abup_get_device_type(),
                    abup_get_utc_time(),
                    (signptr == NULL? (char *)"NULL":(char *)signptr),
                    abup_get_sdk_version(),
                    abup_get_app_version(),
                    abup_get_firmware_version(),
                    abup_get_network_type());
            break;

        case ABUP_FOTA_CV:
            xy_printf("ABUP-->abup_get_http_data:checkVersion.");
            signptr = (unsigned char *)abup_hmd5_did_pid_psecret(
                          (char *)g_abup_data->fota_pid,
                          (char *)abup_get_product_id(),
                          (char *)g_abup_data->fota_psec,
                          (unsigned int)abup_get_utc_time());
            sprintf((char *)out_buf,
                    "{\"mid\":\"%s\",\"version\":\"%s\",\"timestamp\":%d,\"sign\":\"%s\"}",
                    abup_get_device_mid(),
                    abup_get_firmware_version(),
                    abup_get_utc_time(),
                    (signptr == NULL? (char *)"NULL":(char *)signptr));
            break;

        case ABUP_FOTA_DL:
            break;

        case ABUP_FOTA_RD:
            xy_printf("ABUP-->abup_get_http_data:reportDownResult.");
            signptr = (unsigned char *)abup_hmd5_did_pid_psecret(
                          (char *)g_abup_data->fota_pid,
                          (char *)abup_get_product_id(),
                          (char *)g_abup_data->fota_psec,
                          (unsigned int)abup_get_utc_time());
            sprintf((char *)out_buf,
                    "{\"mid\":\"%s\",\"deltaID\":\"%d\",\"downloadStatus\":%d,\"downStart\":%d,\"downEnd\":%d,\"downSize\":%d,\"timestamp\":%d,\"sign\":\"%s\"}",
                    abup_get_device_mid(),
                    g_abup_data->download_uri.download_delta_id,
                    g_abup_data->download_result,
                    0,
                    20,
                    g_abup_data->download_uri.download_delta_size,
                    abup_get_utc_time(),
                    (signptr==NULL?(char *)"NULL":(char *)signptr));
            break;

        case ABUP_FOTA_RU:
        {
            unsigned char abup_update = 0xFF;
            unsigned int ru_delta_id = 0;

            abup_update = abup_check_upgrade();
            abup_read_nv((char *)&ru_delta_id, 128, 4);

            signptr = (unsigned char *)abup_hmd5_did_pid_psecret(
                          (char *)g_abup_data->fota_pid,
                          (char *)abup_get_product_id(),
                          (char *)g_abup_data->fota_psec,
                          (unsigned int)abup_get_utc_time());

            sprintf((char *)out_buf,
                    "{\"mid\":\"%s\",\"deltaID\":\"%d\",\"updateStatus\":%d,\"timestamp\":%d,\"sign\":\"%s\"}",
                    abup_get_device_mid(),
                    ru_delta_id,
                    abup_update,
                    abup_get_utc_time(),
                    (signptr==NULL?(char *)"NULL":(char *)signptr));
        }
        break;

        case ABUP_FOTA_UA:
            break;

        default:
            break;
    }
}

void abup_get_http_path(char *out_buf, int len)
{
    unsigned char *signptr = 0;

    if (out_buf == NULL)
    {
        xy_printf("ABUP-->abup_get_http_path:out buffer should not null!");
        return;
    }

    bzero(out_buf, len);

    switch (abup_fota_status)
    {
        case ABUP_FOTA_RG:
            sprintf((char *)out_buf, "%s/%s", ABUP_REGISTER_URL, abup_get_product_id());
            break;

        case ABUP_FOTA_CV:
            sprintf((char *)out_buf, "/product/%s/%s/ota/checkVersion", abup_get_product_id(), g_abup_data->fota_pid);
            break;

        case ABUP_FOTA_DL:
            if (strlen((char *)g_abup_data->fota_pid) == 0)
            {
                sprintf((char *)out_buf, "%s", g_abup_data->download_uri.download_url);
            }
            else
            {
                sprintf((char *)out_buf, "%s?%s", g_abup_data->download_uri.download_url, g_abup_data->fota_pid);
            }
            break;

        case ABUP_FOTA_RD:
            sprintf((char *)out_buf, "/product/%s/%s/ota/reportDownResult", abup_get_product_id(), g_abup_data->fota_pid);
            break;

        case ABUP_FOTA_RU:
            sprintf((char *)out_buf, "/product/%s/%s/ota/reportUpgradeResult", abup_get_product_id(), g_abup_data->fota_pid);
            break;

        case ABUP_FOTA_UA:
            sprintf((char *)out_buf, "/ota/%s/authentication?mid=%s&version=%s", abup_get_product_id(), abup_get_device_mid(), ABUP_SN_VERSION);
            break;

        default:
            break;
    }
}

void abup_get_http_header(char *out_buf, int len, int content_len)
{
    unsigned char *signptr = 0;

    if (out_buf == NULL)
    {
        xy_printf("ABUP-->abup_get_http_header:out buffer should not null!");
        return;
    }

    bzero(out_buf, len);

    if (content_len > 0)
    {
        sprintf((char *)out_buf, "%s%d\r\n", ABUP_HTTP_CONTENT_LENGTH, content_len);
    }
    else if (abup_get_fota_status() == ABUP_FOTA_DL)
    {
        sprintf((char *)out_buf, "Range: bytes=%d-%d\r\n",
				delta_data_current_num, g_abup_data->download_unit_num * ABUP_DOWNLOAD_UNIT_BYTES);
		xy_printf("ABUP-->download Range: bytes=%d-%d", delta_data_current_num, g_abup_data->download_unit_num * ABUP_DOWNLOAD_UNIT_BYTES);
    }
}

static void abup_save_pid_psec(void)
{
    char *save_data = NULL;

    xy_printf("ABUP-->abup_save_pid_psec.");
    save_data = (char *)xy_zalloc(32*3+3);
    if (save_data == NULL)
    {
        xy_printf("ABUP-->abup_save_pid_psec xy_zalloc fail.");
        return;
    }

    memcpy(save_data, g_abup_data->fota_pid, 32);
    memcpy(save_data + 32, g_abup_data->fota_psec, 32);
    memcpy(save_data + 64, abup_hmd5_pid_psec_mid((char *)g_abup_data->fota_pid, (char *)g_abup_data->fota_psec, (char *)abup_get_device_mid()), 32);
    abup_write_nv(save_data, 16+16, 96);

    xy_free(save_data);
    save_data = NULL;
}

void abup_string_to_hex(char *buf, unsigned char *hex, int hex_len)
{
    int i;

    for (i = 0; i < hex_len; i++)
    {
        unsigned hi = 0, lo = 0;

        if (buf[i * 2] >= '0' && buf[i * 2] <= '9')
        {
            hi = buf[i * 2] - '0';
        }
        else if (buf[i * 2] >= 'A' && buf[i * 2] <= 'F')
        {
            hi = buf[i * 2] - 'A' + 0x0A;
        }

        if (buf[i * 2 + 1] >= '0' && buf[i * 2 + 1] <= '9')
        {
            lo = buf[i * 2 + 1] - '0';
        }
        else if (buf[i * 2 + 1] >= 'A' && buf[i * 2 + 1] <= 'F')
        {
            lo = buf[i * 2 + 1] - 'A' + 0x0A;
        }

        hex[i] = (hi << 4) | lo;
    }
}

static void abup_save_sn_code(char *sn_code)
{
    unsigned char sn_hex[16] = {0};

    xy_printf("ABUP-->abup_save_sn_code.");

    abup_string_to_hex(sn_code, sn_hex, 16);
    abup_write_nv(sn_hex, 0, 16);
    abup_write_nv(abup_get_device_mid(), 16, 16);
}

static void abup_save_delta_id(void)
{
    char *save_data = NULL;
    extern uint32_t abup_delta_page_num;

    save_data = (char *)xy_zalloc(10);
    if (save_data == NULL)
    {
        xy_printf("ABUP-->abup_save_delta_id xy_zalloc fail.");
        return;
    }

    memcpy(save_data, &g_abup_data->download_uri.download_delta_id, 4);
    memcpy(save_data + 4, &abup_delta_page_num, 4);
    abup_write_nv(save_data, 128, 8);

    xy_free(save_data);
    save_data = NULL;
}

static void abup_save_version(void)
{
    char *save_data = NULL;
    char ver_len = strlen(abup_get_firmware_version());

    if (ver_len >= (60 - 1))
    {
        xy_printf("ABUP-->warning, abup_save_version version len too long!!");
        return;
    }
    save_data = (char *)xy_zalloc(ver_len + 1);
    if (save_data == NULL)
    {
        xy_printf("ABUP-->abup_save_version xy_zalloc fail.");
        return;
    }

    memcpy(save_data, abup_get_firmware_version(), ver_len);
    abup_write_nv(save_data, 140, ver_len);

    xy_free(save_data);
    save_data = NULL;
}

void abup_clear_version(void)
{
    char *save_data = NULL;

    save_data = (char *)xy_zalloc(60);
    if (save_data == NULL)
    {
        xy_printf("ABUP-->abup_clear_version xy_zalloc fail.");
        return;
    }

    abup_write_nv(save_data, 140, 59);

    xy_free(save_data);
    save_data = NULL;
}

static void abup_set_download_flag(void)
{
    unsigned char flag[4] = "ABUP";
    abup_write_backup(flag, 0, 4);
}

static void abup_clear_download_flag(void)
{
    abup_erase_backup();
}

static void abup_save_block_id(signed int cur_len)
{
    unsigned char *save_data = NULL;
	
    save_data = (unsigned char *)xy_zalloc(5);
    if (save_data == NULL)
    {
        xy_printf("ABUP-->abup_save_block_id xy_zalloc fail.");
        return;
    }

	delta_data_current_num = cur_len;
    memcpy(save_data, &cur_len, 4);
    abup_write_nv(save_data, 136, 4);

    xy_printf("ABUP-->abup_save_block_id %d.", cur_len);

    xy_free(save_data);
    save_data = NULL;
}

static void abup_check_block_id(void)
{
    signed int block_id = 0;

    abup_read_nv((char *)&block_id, 136, 4);

    xy_printf("ABUP-->abup_check_block_id %d.", block_id);

	if (block_id <= 0)
		delta_data_current_num = 0;
	else
		delta_data_current_num = block_id;

	delta_data_left_len = g_abup_data->download_uri.download_delta_size - delta_data_current_num;
}

void abup_start_download(void)
{
    char backup_str[4] = {0};

    abup_fota_status = ABUP_FOTA_DL;

    if (g_abup_data->download_uri.download_delta_size % ABUP_DOWNLOAD_UNIT_BYTES == 0)
    {
        g_abup_data->download_unit_num = g_abup_data->download_uri.download_delta_size / ABUP_DOWNLOAD_UNIT_BYTES;
    }
    else
    {
        g_abup_data->download_unit_num = g_abup_data->download_uri.download_delta_size / ABUP_DOWNLOAD_UNIT_BYTES + 1;
    }
    xy_printf("ABUP-->host:%s,port:%s",
              g_abup_data->download_uri.download_host,
              g_abup_data->download_uri.download_port);
    xy_printf("ABUP-->delta_size:%d,delta_id:%d",
              g_abup_data->download_uri.download_delta_size,
              g_abup_data->download_uri.download_delta_id);
    xy_printf("ABUP-->md5:%s", g_abup_data->download_uri.download_md5);
    //xy_printf("ABUP-->url:%s", g_abup_data->download_uri.download_url);

    abup_set_download_flag();
    abup_read_backup(backup_str, 0, 4);
    if (backup_str[0] != 'A' || backup_str[1] != 'B' || backup_str[2] != 'U' || backup_str[3] != 'P')
    {
        xy_printf("ABUP-->Flag write fail.");
        g_abup_data->err_type = ABUP_ERROR_TYPE_WRITE_FLASH;
        g_abup_data->download_result = 100;
        abup_fota_status = ABUP_FOTA_RD;
        return;
    }

    if (g_abup_data->download_uri.download_delta_size > abup_delta_region_size())
    {
        xy_printf("ABUP-->Not enough space.");
        g_abup_data->err_type = ABUP_ERROR_TYPE_NOT_ENOUGH_SPACE;
        g_abup_data->download_result = 99;
        abup_fota_status = ABUP_FOTA_RD;
        return;
    }
}

static void abup_get_string(char *src, char *offset, char *out, unsigned int len)
{
    char *p1 = NULL;
    char *p2 = NULL;

    if ((offset == NULL) || (src == NULL))
    {
        return;
    }

    p1 = (char *)strstr((char *)src, (char *)offset);
    if (p1 != NULL)
    {
        p1 += strlen((char *)offset);
        p2 = (char *)strstr((char *)p1, (char *)"\"");

        if (p2 != NULL)
        {
            if ((p2 - p1) > len)
            {
                strncpy((char *)out, (char *)p1, len);
            }
            else
            {
                strncpy((char *)out, (char *)p1, (p2 - p1));
            }
        }
    }
}

static void abup_get_int(char *src, char *offset, int *out)
{
    char *p1 = NULL;
    char *p2 = NULL;
    char p3[12] = {0};

    *out = 0;
    if ((offset == NULL) || (src == NULL))
    {
        return;
    }

    p1 = (char *)strstr((char *)src, (char *)offset);
    if (p1 != NULL)
    {
        p1 += strlen((char *)offset);
        p2 = (char *)strstr((char *)p1, (char *)",");

        if (p2 != NULL)
        {
            if ((p2 - p1) <= 11)
            {
                strncpy((char *)p3, (char *)p1, (p2 - p1));
                *out = atoi((char *)p3);
            }
        }
    }
}

void abup_change_ip(void)
{
    switch (abup_fota_status)
    {
        case ABUP_FOTA_CV:
            abup_start_download();
            //abup_set_fota_ratio(100 / (g_abup_data->download_unit_num + 2));
            break;
        case ABUP_FOTA_DL:
            abup_fota_status = ABUP_FOTA_RD;
            break;
        default:
            xy_printf("ABUP-->Error, invalid op to change ip.");
            break;
    }
}

void abup_parse_http_data(char *p_data, unsigned int len)
{
    char *temp_buf = p_data;
    char *temp_buf1 = NULL;
    static unsigned char http_not_found_times = 0;

    if ((strstr((char *)temp_buf, (char *)"HTTP/1.1 301") != NULL) || (strstr((char *)temp_buf, (char *)"HTTP/1.1 302") != NULL) ||
        (strstr((char *)temp_buf, (char *)"HTTP/1.0 301") != NULL) || (strstr((char *)temp_buf, (char *)"HTTP/1.0 302") != NULL))
    {
        xy_printf("ABUP-->found 301 302");
        abup_fota_exit();
        return;
    }

    temp_buf1 = (char *)strstr((char *)temp_buf, (char *)"HTTP/1.1");
    if (temp_buf1 == NULL)
    {
		xy_printf("ABUP-->not found HTTP/1.1");
		if (abup_fota_status != ABUP_FOTA_DL)
		{
			return;
		}
    }
    http_not_found_times = 0;

    temp_buf1 = (char *)strstr((char *)temp_buf, (char *)"Connection:");
    if (temp_buf1 == NULL)
    {
        xy_printf("ABUP-->not found Connection");
        if (abup_fota_status != ABUP_FOTA_DL)
        {
            abup_fota_exit();
            return;
        }
    }

    //xy_printf("ABUP-->abup_parse_http_data len:%d", len);
    if (abup_fota_status != ABUP_FOTA_DL)
    {
        abup_get_int(temp_buf, "\"status\":", &g_abup_data->status);

        if (g_abup_data->status != FOTA_SUCCESS)
        {
            abup_get_string(temp_buf, "\"msg\":\"", g_abup_data->msg, ABUP_HTTP_MSG_LEN);
            xy_printf("ABUP-->status:%d", g_abup_data->status);
            xy_printf("ABUP-->%s", g_abup_data->msg);
			if (abup_fota_status == ABUP_FOTA_RU)
			{
				abup_clear_version();
			}
            abup_fota_exit();
            return;
        }
    }

    switch (abup_fota_status)
    {
        case ABUP_FOTA_RG:
            memset(g_abup_data->fota_pid, 0, 34);
            memset(g_abup_data->fota_psec, 0, 34);
            abup_get_string(temp_buf, "\"deviceId\":\"", g_abup_data->fota_pid, 32);
            abup_get_string(temp_buf, "\"deviceSecret\":\"", g_abup_data->fota_psec, 32);
            abup_save_pid_psec();
            abup_fota_status = ABUP_FOTA_CV;
            abup_task_message_send(ABUP_TASK_MSGID_SOCKET_WRITE);
            break;
        case ABUP_FOTA_CV:
        {
            char delta_id[12] = {0};
            char *p1 = NULL, *p2 = NULL;

            abup_get_string((char *)temp_buf, (char *)"\"deltaUrl\":\"", (char *)g_abup_data->download_uri.download_url, ABUP_DOWNLOAD_URI_MAX_LEN - 2);
            p1 = (char *)strstr((char *)g_abup_data->download_uri.download_url, (char *)"http://");
            if (p1 != NULL)
            {
                p1 += strlen((char *)"http://");
                p2 = (char *)strstr((char *)p1, (char *)"/");
                if (p2 != NULL && (p2 - p1) < ABUP_DOWNLOAD_HOST_MAX_LEN)
                {
                    strncpy((char *)g_abup_data->download_uri.download_host, (char *)p1, p2 - p1);
                    strcpy((char *)g_abup_data->download_uri.download_url, (char *)p2);
                }
            }
			update_last_OTA_check_time();
            abup_get_string(temp_buf, "\"deltaID\":\"", delta_id, 11);
            g_abup_data->download_uri.download_delta_id = (unsigned int)atoi((char *)delta_id);
            abup_get_int(temp_buf, (char *)"\"fileSize\":", (int *)&g_abup_data->download_uri.download_delta_size);
            abup_get_string(temp_buf, (char *)"\"md5sum\":\"", (char *)g_abup_data->download_uri.download_md5, ABUP_MD5_LENGTH);
            strcpy((char *)g_abup_data->download_uri.download_port, (char *)"80");

            abup_fota_flash_init(g_abup_data->download_uri.download_delta_size);
            abup_check_block_id();
            abup_save_delta_id();

            abup_task_message_send(ABUP_TASK_MSGID_CHANGE_IP);
            break;
        }
        case ABUP_FOTA_DL:
            temp_buf1 = (char *)strstr((char *)temp_buf, (char *)"Content-Range: bytes ");
            if (temp_buf1)
            {
                int s = 0, e = 0, t = 0;

                temp_buf1 += strlen("Content-Range: bytes ");
                sscanf((char *)temp_buf1, "%d-%d/%d", &s, &e, &t);
                if ((e - s) <= g_abup_data->download_uri.download_delta_size)
                {
                    xy_printf("ABUP-->abup_parse_http_data ABUP_FOTA_DL:%d-%d/%d ", s, e, t);
                }
                else
                {
                    xy_printf("ABUP-->Download size error!");
                    abup_fota_exit();
                    break;
                }

                temp_buf1 = (char *)strstr((char *)temp_buf1, (char *)"\r\n\r\n");
                if (temp_buf1)
                {
                	temp_buf1 += strlen("\r\n\r\n");
                }
                else
                {
                    xy_printf("ABUP-->abup_parse_http_data not find temp_buf1");
                }
				
				//xy_printf("ABUP-->abup_parse_http_data (temp_buf1-p_data):%d", temp_buf1 - p_data);
				delta_data_left_len = t;
				
                abup_write_flash(temp_buf1, 0, len);

				delta_data_left_len -= len;
            }
            else
            {
            	//xy_printf("ABUP-->download without Content-Range: bytes");
                abup_write_flash(temp_buf, g_abup_data->download_uri.download_delta_size - delta_data_left_len, len);
				
                delta_data_left_len -= len;
            }
			
            if (delta_data_left_len <= 0)
            {
                char delta_md5[ABUP_MD5_LENGTH + 2] = {0};
                xy_printf("ABUP-->download complete. md5:%s",g_abup_data->download_uri.download_md5);

				abup_save_block_id(0);
				delta_data_left_len = 0;
				
                abup_clear_download_flag();
                abup_delta_get_md5(delta_md5);
                if(strncmp((char *)delta_md5, (char *)g_abup_data->download_uri.download_md5, ABUP_MD5_LENGTH) == 0)
                {
                    xy_printf("ABUP-->DL MD5 OK.");
                    abup_save_version();
                    g_abup_data->download_result = 1;
                }
                else
                {
                    xy_printf("ABUP-->DL MD5 FAIL.");
                    abup_erase_flash(0, EFTL_PAGE_SIZE);
                    g_abup_data->err_type = ABUP_ERROR_TYPE_MD5_NOT_MATCH;
                    g_abup_data->download_result = 8;
                }

                abup_task_message_send(ABUP_TASK_MSGID_CHANGE_IP);
            }
            else
            {
            	xy_printf("ABUP-->download delta_data_left_len: %d", delta_data_left_len);
				
            	abup_save_block_id(g_abup_data->download_uri.download_delta_size - delta_data_left_len);
            }
			abup_set_fota_ratio((g_abup_data->download_uri.download_delta_size - delta_data_left_len)*100 / g_abup_data->download_uri.download_delta_size);
			break;
        case ABUP_FOTA_RD:
            abup_set_fota_ratio(100);
            xy_printf("ABUP-->Download report done!");
            if (g_abup_data->download_result == 1)
            {
                abup_task_message_send(ABUP_TASK_MSGID_SYSTEM_REBOOT);
            }
            else
            {
                xy_printf("ABUP-->Download fail(ret=%d)!", g_abup_data->download_result);
                abup_fota_exit();
            }
            break;
        case ABUP_FOTA_RU:
            xy_printf("ABUP-->Upgrade report done!");
            abup_clear_version();
            abup_fota_exit();
            break;
        case ABUP_FOTA_UA:
        {
            char sn_key[34] = {0};
            abup_get_string(temp_buf, "\"data\":\"", sn_key, 32);
            abup_save_sn_code(sn_key);
            abup_fota_status = ABUP_FOTA_RG;
            abup_task_message_send(ABUP_TASK_MSGID_SOCKET_WRITE);
            break;
        }
        default:
            break;
    }
}

ABUP_FOTA_STATUS abup_get_fota_status(void)
{
    return abup_fota_status;
}

void abup_fota_exit(void)
{
    xy_printf("ABUP-->abup_fota_exit executing");
    abup_task_message_send(ABUP_TASK_MSGID_EXIT);
}

void abup_delta_get_md5(char *file_md5)
{
    unsigned int i = 0;
    unsigned char tmp_buffer[512] = {0};
    unsigned int total = 0;
    ABUP_MD5_CTX abup_md5;
    unsigned char abup_md5_result[16] = {0};
    char abup_md5_info[33] = {0};

    total = g_abup_data->download_uri.download_delta_size;

    abup_MD5_Init(&abup_md5);
    while (total > 0)
    {
        memset(tmp_buffer,0,512);
        if (total > 512)
        {
            //xy_printf("ABUP-->calc file md5 i = %d,total = %d", i, total);
            abup_read_flash(tmp_buffer, i * 512, 512);
            abup_MD5_Update(&abup_md5, (void *)tmp_buffer, 512);
            i += 1;
            total -= 512;
        }
        else
        {
            //xy_printf("ABUP-->calc file md5 i = %d,total = %d", i, total);
            abup_read_flash(tmp_buffer, i * 512, total);
            abup_MD5_Update(&abup_md5, (void *)tmp_buffer, total);
            total = 0;
        }
    }
    if (total == 0)
    {
        memset(abup_md5_result, 0, 16);
        memset(abup_md5_info, 0, 33);
        abup_MD5_Final(abup_md5_result, &abup_md5);
        abup_MD5_Encode(abup_md5_info, abup_md5_result, 16);
        xy_printf("ABUP-->md calc = %s", abup_md5_info);
        strncpy((char *)file_md5,(char *)abup_md5_info, 32);
    }
}

bool abup_get_prev_version(char *prev_version, unsigned int ver_len)
{
    abup_read_nv(prev_version, 140, ver_len);

    if (prev_version[0] == -1 || strlen(prev_version) == 0)
    {
        return false;
    }

    return true;
}

bool abup_get_pid_psec(void)
{
    char pid_md5[34] = {0};

    abup_read_nv(g_abup_data->fota_pid, 32, 32);
    abup_read_nv(g_abup_data->fota_psec, 64, 32);
    abup_read_nv(pid_md5, 96, 32);
    if (strncmp((char *)abup_hmd5_pid_psec_mid((char *)g_abup_data->fota_pid, (char *)g_abup_data->fota_psec, (char *)abup_get_device_mid()), pid_md5, 32) == 0)
    {
        return true;
    }
    return false;
}

bool abup_is_sn_exist(void)
{
    unsigned char sn_code[18] = {0};
    char imei[16] = {0};

    abup_read_nv(sn_code, 0, 16);
    xy_printf("ABUP-->sn:%X %X %X %X",sn_code[0],sn_code[3],sn_code[4],sn_code[7]);
    if ((sn_code[0] == 0x0) || (sn_code[0] == 0xFF))
    {
        return false;
    }

    abup_read_nv(imei, 16, 15);
    xy_printf("ABUP-->mid:%s",imei);
    if (strncmp(imei, abup_get_device_mid(), 15) != 0)
    {
        return false;
    }

    return true;
}

void abup_set_continue_read(void)
{
    g_abup_data->continue_read = true;
}

void abup_reset_continue_read(void)
{
    g_abup_data->continue_read = false;
}

unsigned char abup_check_upgrade(void)
{
    char up_flag[4] = {0};

    if (abup_update_result != 0xFF)
    {
        return abup_update_result;
    }

    abup_read_backup(up_flag, 0, 4);
    xy_printf("ABUP-->backup:%X %X %X %X",up_flag[0],up_flag[1],up_flag[2],up_flag[3]);
    if (up_flag[0] == 'A' && up_flag[1] == 'B' && up_flag[2] == 'U' && up_flag[3] == 'P')
    {
        xy_printf("ABUP-->No Upgrade! version=%s.", abup_get_firmware_version());
        abup_update_result = 0;
    }
    else
    {
        char version[33] = {0};
        bool ret = false;

        ret = abup_get_prev_version(version, 32);
        if (ret)
        {
            xy_printf("ABUP-->Preview version=%s", version);
            xy_printf("ABUP-->Current version=%s", abup_get_firmware_version());
            if (strncmp(version, abup_get_firmware_version(), strlen(abup_get_firmware_version())) == 0)
            {
                xy_printf("ABUP-->Upgrade Fail!");
                abup_update_result = 99;
            }
            else
            {
                xy_printf("ABUP-->Upgrade Sucess!");
                abup_update_result = 1;
            }
        }
        else
        {
            xy_printf("ABUP-->No Upgrade! version=%s", abup_get_firmware_version());
            abup_update_result = 0;
        }
    }

    return abup_update_result;
}

void abup_fota_retry(void)
{
    xy_printf("ABUP-->retry fota status:%d.", abup_fota_status);
    switch (abup_fota_status)
    {
        case ABUP_FOTA_RG:
        case ABUP_FOTA_CV:
        case ABUP_FOTA_DL:
        case ABUP_FOTA_RU:
        case ABUP_FOTA_UA:
            abup_task_message_send(ABUP_TASK_MSGID_SOCKET_CONNECT);
            break;
        case ABUP_FOTA_RD:
            if (g_abup_data->download_result == 1)
            {
                abup_task_message_send(ABUP_TASK_MSGID_SYSTEM_REBOOT);
            }
            else
            {
                abup_task_message_send(ABUP_TASK_MSGID_SOCKET_CONNECT);
                xy_printf("ABUP-->Download fail(ret=%d)!", g_abup_data->download_result);
            }
            break;
        default:
            break;
    }
}
