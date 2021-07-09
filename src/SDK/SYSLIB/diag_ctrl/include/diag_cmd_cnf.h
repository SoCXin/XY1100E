#ifndef _DIAG_COMMAND_H_
#define _DIAG_COMMAND_H_
#include "SubSysCnf.h"
#include "TypeDefine.h"
#include "sysappcnf.h"

void diag_send_cmd(U32 type_id, U32 cmd_id, u8 src_id, char* data, int nLen);

void diag_wireshark_data(char* data, int nLen, u8 type);

void diag_wireshark_dataM3(char* data, int nLen, u8 type, u32 m3Time);


void diag_send_cmd_flash(U32 type_id, U32 cmd_id, u8 src_id, char* data, int nLen);

void diag_wireshark_data_flash(char* data, int nLen, u8 type);

void diag_wireshark_dataM3_flash(char* data, int nLen, u8 type, u32 m3Time);




#endif

