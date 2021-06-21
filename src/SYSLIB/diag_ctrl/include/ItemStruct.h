#ifndef ITEMSTRUCTINF
#define ITEMSTRUCTINFO

#include "basetype.h"
#include "xy_nbiot_msg_define.h"
#include "xy_nbiot_module_define.h"
#define HEADERSIZE sizeof(unsigned int)
#define LENSIZE sizeof(unsigned short)

//
///
//
/*
̬log ItemHeader_t + DynamicLog_t
̬log ItemHeader_t + DynamicLog_t
AT ItemHeader_t + ATCmd_t
.
.
.
.
*/
//
typedef struct
{
	unsigned int u32Header:24;// 0x5AA5FEEF
	unsigned int uChipType:8;
	unsigned short u16Len; //
	unsigned short u16SeqNum;//log
    unsigned int u4TraceId:4;//item
    unsigned int u28ClassId:28;//
    unsigned int u32Time;//
    unsigned char u8Payload[0];
}ItemHeader_t; //sizeof = 16


//̬log
typedef struct
{
	ItemHeader_t itemHeader;
	unsigned char u8SrcId;
	unsigned char u8MsgLev;
	unsigned char u8ParamSize;
	unsigned char Pad;
	unsigned short u16MsgSize;
	unsigned short u16Pad;
	unsigned char u8Payload[0];
}StaticLog_t; //sizeof = 8

//ѹlog
typedef struct
{
	ItemHeader_t itemHeader;
	unsigned char u8SrcId;
	unsigned char u8MsgLev;
	unsigned short pad;
	unsigned short u16DynId;//
	unsigned short u16MsgSize;
	unsigned char u8Payload[0];//
}DynamicLog_t;

//AT
typedef struct
{
	ItemHeader_t itemHeader;
	unsigned short u16Len;
	unsigned short u16Pad;//
	unsigned char u8Paylod[0];//AT
}ATCmd_t; //sizeof 4

//og
typedef struct
{
	ItemHeader_t itemHeader;
}Message_t;//sizeof 4

//
typedef struct
{
	ItemHeader_t itemHeader;
	unsigned char u8Type;//
	unsigned char u8Ver;//
	unsigned short u16Len;//
	unsigned char u8Payload[0];//
}Protocol_t;//sizeof 8

typedef struct
{
	unsigned char u8Direction;
	unsigned char pad;
	unsigned short u16Len;//
	unsigned char u8Payload[0];//
}NasHeader_t;

typedef struct
{
	unsigned char u8MsgId;
	unsigned char pad;
	unsigned short u16Len;
	unsigned char u8Payload[0];//
}RrcHeader_t;

//
typedef struct
{
	unsigned short u8Seq:15;//
	unsigned short u8PEnd:1;//
	unsigned short u8Len;
	unsigned char u8Payload[0];//
}MemInfo_t;

typedef struct
{
	unsigned char u8Payload[0];//
}CommonReq_t;

typedef struct
{
	unsigned char type;
	unsigned char pad;
	unsigned short len;
	unsigned char u8Payload[0];//
}WireShark_t;


typedef struct
{
	ItemHeader_t itemHeader;
	unsigned char u8SrcId;
	unsigned char pad;
	unsigned short u16Len;//
	unsigned char u8Payload[0];//
}CommonCnf_t;

typedef struct
{
	unsigned int freq;
	unsigned int power;
}APC_Control_Id_t;

typedef struct
{
	unsigned char log_filter[XYLOG_MAX_BLOCK];
	unsigned char air_filter;
	unsigned char primitive_filter[MAX_PS_BLOCK][MAX_MSG_ID];
}filterInfo;

typedef  CommonReq_t SysAppReq_t;
typedef  CommonReq_t SubSysReq_t;
typedef  CommonCnf_t SysAppCnf_t;
typedef  CommonCnf_t SubSysCnf_t;

typedef enum
{
	Chip_1100 = 0xEF,
	Chip_1100E= 0xEE,
}ChipType_E;

#endif
