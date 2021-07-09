#ifndef _DIAG_OUT_H_
#define _DIAG_OUT_H_
#include "softap_macro.h"
#if IS_DSP_CORE
#include "basetype.h"
#include "xy_nbiot_module_define.h"
#endif
#define ARG_MEMORY_LEN_MAX  80

//canloss is 0 do not check the msg buffer size
unsigned int diag_msg_out(const char*  buf,
		 unsigned int        buf_sz,
		 unsigned char canLoss);

void diag_send_data(const char*  buf,
		 unsigned int        buf_sz
                        );

void diag_send_header( unsigned short	trace_id,
		 unsigned int	class_id,
		 unsigned int 	buf_len);

int at_diag_info(char *at_buf,char **prsp_cmd);

extern void ResetMem();


#endif
