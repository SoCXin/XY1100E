/*****************************************************************************
* Copyright (c) 2019 ABUP.Co.Ltd. All rights reserved.
* File name: abup_common.h
* Description:
* Author: WQH
* Version: v1.0
* Date: 20190303
*****************************************************************************/
#ifndef __ABUP_COMMON_H__
#define __ABUP_COMMON_H__

void abup_info_point_deinit(void);
void abup_info_point_init(void);

void abup_set_service_info(void);

char *abup_get_device_mid(void);
char *abup_get_product_id(void);
char *abup_get_product_sec(void);
unsigned int abup_get_life_time(void);
char *abup_get_manufacturer(void);
char *abup_get_model_number(void);
char *abup_get_platform(void);
char *abup_get_device_type(void);
char *abup_get_sdk_version(void);
char *abup_get_app_version(void);
char *abup_get_firmware_version(void);
char *abup_get_network_type(void);
unsigned int abup_get_utc_time(void);

#endif

