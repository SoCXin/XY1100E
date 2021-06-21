#ifndef _DIAG_STRING_PRINT_H_
#define _DIAG_STRING_PRINT_H_
#include "basetype.h"
#include "TypeDefine.h"
#include <stdarg.h>
/*

the functions success return 1, otherwise 0



*/

unsigned int diag_static_log_ram(unsigned char src_id,unsigned char lev, const char*fmt,...);

unsigned int diag_static_logM3_ram(unsigned char src_id,unsigned char lev, unsigned int m3Time, const char* log_buf);


unsigned int diag_dynamic_log_ram(int dyn_id, unsigned char src_id,unsigned char lev, char *fmt, ...);

#if !IS_DSP_CORE
unsigned int diag_static_logM3(unsigned char src_id, unsigned char lev, unsigned int m3Time, const char*fmt, va_list args);
#else
unsigned int diag_static_logM3(u8 src_id,u8 lev, u32 m3Time, const char* log_buf);
#endif

#endif

