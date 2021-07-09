#ifndef _DIAG_FORMAT_H_
#define _DIAG_FORMAT_H_


#include <stdarg.h>
#include "basetype.h"

void diag_packet_header(char *pItemHeader,unsigned int buf_sz, unsigned int type_id, unsigned int item_id);
void diag_packet_header_m3(char *pItemHeader,unsigned int buf_sz, unsigned int type_id, unsigned int item_id, unsigned int m3Time);

#if BLE_ENABLE
int diag_packet_header_AT(char *data, int len, char **out_addr);
int diag_packet_header_user(int type_id, int item_id, char *data, int len, char **out_addr);

#endif

unsigned int diag_str2memory(const char*   pfmt,
							unsigned char strLen,
							unsigned char*        pArgBuf,
                            int        arg_buf_sz,
                            va_list       args
                           );

unsigned short get_Seq_num();

#endif

