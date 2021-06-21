#ifndef _DIAG_PIRMITIVE_PRINT_H_
#define _DIAG_PIRMITIVE_PRINT_H_
#include "basetype.h"
#include "TypeDefine.h"

U32 print_nas(u8 dirction, char *pMsg, int nLen);

U32 print_rrc(u8 msg_id, char *pMsg, int nLen);

U32 print_asn1_ber(u8 msg_id, char *pMsg, int nLen);

U32 print_nas_ram(u8 dirction, char *pMsg, int nLen);

U32 print_rrc_ram(u8 msg_id, char *pMsg, int nLen);

U32 print_asn1_ber_ram(u8 msg_id, char *pMsg, int nLen);

#endif
