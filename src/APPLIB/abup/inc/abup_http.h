/*****************************************************************************
* Copyright (c) 2019 ABUP.Co.Ltd. All rights reserved.
* File name: adups_http.h
* Description:
* Author: WQH
* Version: v1.0
* Date: 20190303
*****************************************************************************/
#ifndef __ABUP_HTTP_H__
#define __ABUP_HTTP_H__
#include "abup_fota.h"
#include <stdbool.h>

typedef enum
{
	FOTA_SUCCESS=1000,
	FOTA_PID_ERROR,
	FOTA_PROJECT_ERROR,
	FOTA_PARAM_INVAILD,
	FOTA_PARAM_LOST,
	FOTA_SYS_ERROR,
	FOTA_JSON_ERROR,
	FOTA_RG_SIGN_ERROR=2001,
	FOTA_CV_LAST_VERSION=2101,
	FOTA_CV_INVAILD_VERSION,
	FOTA_CV_UNREG_DEVICE,
	FOTA_DL_STATE_ERROR=2201,
	FOTA_DL_DELTAID_ERROR,
	FOTA_RP_DELTAID_ERROR=2301,
	FOTA_RP_UPGRADE_STATE_ERROR	
}FOTACODE;
	
void abup_clear_version(void);
char *abup_get_host_domain(void);
char *abup_get_host_port(void);
void abup_set_fota_status(ABUP_FOTA_STATUS status);
signed int abup_get_delta_data_left_len(void);
void abup_get_http_content(char *out_buf, int len);
void abup_get_http_path(char *out_buf, int len);
void abup_change_ip(void);
void abup_parse_http_data(char *p_data, unsigned int len);
ABUP_FOTA_STATUS abup_get_fota_status(void);
void abup_fota_exit(void);
void abup_delta_get_md5(char *file_md5);
bool abup_get_pid_psec(void);
bool abup_is_sn_exist(void);
void abup_set_continue_read(void);
void abup_reset_continue_read(void);
unsigned char abup_check_upgrade(void);
bool abup_get_sn_key(void);

#endif

