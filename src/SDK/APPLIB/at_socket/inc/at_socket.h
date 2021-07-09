
#pragma once
#include "softap_macro.h"

#if AT_SOCKET

/*
0x100 Exception Message: send message with high priority
0x200 Release Indicator: indicate release after next message
0x400 Release Indicator: indicate release after next message has been replied
*/
enum
{
	DATA_NORMAL,
	DATA_EXCEPTION,
	DATA_REL_UP,
	DATA_REL_DOWN,
};

int at_NSOCR_req(char *at_buf, char **prsp_cmd);
int at_NSOST_req(char *at_buf, char **prsp_cmd);
int at_NSOSTF_req(char *at_buf, char **prsp_cmd);
int at_NSORF_req(char *at_buf, char **prsp_cmd);
int at_NSOCL_req(char *at_buf, char **prsp_cmd);
int at_NSOCO_req(char *at_buf, char **prsp_cmd);
int at_NSOSD_req(char *at_buf, char **prsp_cmd);
int at_NSONMI_req(char *at_buf, char **prsp_cmd);
int at_NQSOS_req(char *at_buf, char **prsp_cmd);
int at_NSOSTATUS_req(char *at_buf, char **prsp_cmd);
int at_XDSEND_req(char *at_buf);

#endif


