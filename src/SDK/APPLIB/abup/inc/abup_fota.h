/*****************************************************************************
* Copyright (c) 2019 ABUP.Co.Ltd. All rights reserved.
* File name: abup_fota.h
* Description:
* Author: WQH
* Version: v1.0
* Date: 20190903
*****************************************************************************/
#ifndef __ABUP_FOTA_H__
#define __ABUP_FOTA_H__

#define ABUP_SOCKET_BUFFER_LEN		2048+640	//1152

typedef enum
{
	ABUP_FOTA_IDLE,
	ABUP_FOTA_RG,
	ABUP_FOTA_CV,
	ABUP_FOTA_DL,
	ABUP_FOTA_RD,
	ABUP_FOTA_RU,
	ABUP_FOTA_UA
}ABUP_FOTA_STATUS;

typedef enum
{
	ABUP_TASK_MSGID_SOCKET_PDP = 101,
	ABUP_TASK_MSGID_SOCKET_CONNECT, //102
	ABUP_TASK_MSGID_SOCKET_WRITE, //103
	ABUP_TASK_MSGID_SOCKET_READ, //104
	ABUP_TASK_MSGID_CHANGE_IP, //105
	ABUP_TASK_MSGID_SOCKET_CLOSE, //106
	ABUP_TASK_MSGID_EXIT, //107
	ABUP_TASK_MSGID_SYSTEM_REBOOT, //108
}ABUP_TASK_MSGID_ENUM;

typedef enum
{
    ABUP_ERROR_TYPE_NONE,
    ABUP_ERROR_TYPE_UART_TIMEOUT,// 1
    ABUP_ERROR_TYPE_NO_NETWORK,
    ABUP_ERROR_TYPE_DNS_FAIL,// 3
    ABUP_ERROR_TYPE_CREATE_SOCKET_FAIL,
    ABUP_ERROR_TYPE_NETWORK_ERROR,// 5
    ABUP_ERROR_TYPE_NO_NEW_UPDATE,
    ABUP_ERROR_TYPE_MD5_NOT_MATCH,// 7
    ABUP_ERROR_TYPE_NOT_ENOUGH_SPACE,
    ABUP_ERROR_TYPE_ERASE_FLASH,// 9
    ABUP_ERROR_TYPE_WRITE_FLASH,
    ABUP_ERROR_TYPE_NO_ACCESS_TIMES,// 11
    ABUP_ERROR_TYPE_UNKNOW,
}ABUP_ERROR_TYPE_ENUM;

void abup_create_fota_task(void);
void abup_free_dns(void);
void abup_close_socket(void);
void abup_set_fota_ratio(unsigned int ratio);
unsigned int abup_get_fota_ratio(void);
void abup_check_update(void);
void abup_check_version(void);
void abup_init(void);
void abup_task_message_send(unsigned int msg_id);
#endif

