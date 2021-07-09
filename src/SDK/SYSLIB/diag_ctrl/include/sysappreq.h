#ifndef SYSAPPREQ_H
#define SYSAPPREQ_H
#include "TypeDefine.h"
#include "xy_nbiot_msg_define.h"


enum
{
    XY_SUBAPPREQ_BASE = XY_SYSAPP_REQ << 16,
    MSG_SHOW_NAME(XY_SYSAPP_DETECT_REQ, nullStruct),
    MSG_SHOW_NAME(XY_SYSAPP_READNV_REQ, nullStruct),
    MSG_SHOW_NAME(XY_SYSAPP_WRITENV_REQ, nullStruct),
    MSG_SHOW_NAME(XY_SYSAPP_MEMREADY_REQ, nullStruct),
	MSG_SHOW_NAME(XY_SYSAPP_ASSERT_REQ, nullStruct),
	MSG_SHOW_NAME(XY_SYSAPP_FILTER_REQ, nullStruct),
	MSG_SHOW_NAME(XY_SYSAPP_MAXLEN_REQ, nullStruct), 
	MSG_SHOW_NAME(XY_SYSAPP_HEART_REQ, nullStruct), 	
};
#endif // SYSAPPREQ_H
