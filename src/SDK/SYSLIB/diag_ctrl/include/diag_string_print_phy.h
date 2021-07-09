#ifndef _DIAG_STRING_PRINT_PHY_H_
#define _DIAG_STRING_PRINT_PHY_H_
#include "basetype.h"
#include "TypeDefine.h"
#include <stdarg.h>

U32 diag_pyh_printk4_ram(int dyn_id,
							 u8 src_id,
							 u8 lev,
							 char *fmt, 
							 U32 v1, 
							 U32 v2, 
							 U32 v3,
							 U32 v4);

U32 diag_pyh_printk8_ram(int dyn_id,
		 	 	 	 	 	 u8 src_id,
							 u8 lev,
						  	 char *fmt,
						     U32 v1,
						  	 U32 v2,
						  	 U32 v3,
						     U32 v4,
						     U32 v5,
						     U32 v6,
						     U32 v7,
						     U32 v8);

U32 diag_pyh_printk12_ram(int dyn_id,
						  u8 src_id,
						  u8 lev,
						  char *fmt,
						  U32 v1,
						  U32 v2,
						  U32 v3,
						  U32 v4,
						  U32 v5,
						  U32 v6,
						  U32 v7,
						  U32 v8,
						  U32 v9,
						  U32 v10,
						  U32 v11,
						  U32 v12);


U32 diag_pyh_printk16_ram(int dyn_id,
						  u8 src_id,
						  u8 lev,
						  char *fmt,
						  U32 v1,
						  U32 v2,
						  U32 v3,
						  U32 v4,
						  U32 v5,
						  U32 v6,
						  U32 v7,
						  U32 v8,
						  U32 v9,
						  U32 v10,
						  U32 v11,
						  U32 v12,
						  U32 v13,
						  U32 v14,
						  U32 v15,
						  U32 v16);


U32 diag_static_log(u8 src_id,u8 lev, const char*fmt,va_list args);


U32 diag_dynamic_log(int dyn_id, u8 src_id,u8 lev, char *fmt, va_list args);

U32 diag_pyh_printk4(int dyn_id,
							 u8 src_id,
							 u8 lev,
							 char *fmt, 
							 U32 v1, 
							 U32 v2, 
							 U32 v3,
							 U32 v4);

U32 diag_pyh_printk8(int dyn_id,
		 	 	 	 	 	 u8 src_id,
							 u8 lev,
						  	 char *fmt,
						     U32 v1,
						  	 U32 v2,
						  	 U32 v3,
						     U32 v4,
						     U32 v5,
						     U32 v6,
						     U32 v7,
						     U32 v8);

U32 diag_pyh_printk12(int dyn_id,
						  u8 src_id,
						  u8 lev,
						  char *fmt,
						  U32 v1,
						  U32 v2,
						  U32 v3,
						  U32 v4,
						  U32 v5,
						  U32 v6,
						  U32 v7,
						  U32 v8,
						  U32 v9,
						  U32 v10,
						  U32 v11,
						  U32 v12);


U32 diag_pyh_printk16(int dyn_id,
						  u8 src_id,
						  u8 lev,
						  char *fmt,
						  U32 v1,
						  U32 v2,
						  U32 v3,
						  U32 v4,
						  U32 v5,
						  U32 v6,
						  U32 v7,
						  U32 v8,
						  U32 v9,
						  U32 v10,
						  U32 v11,
						  U32 v12,
						  U32 v13,
						  U32 v14,
						  U32 v15,
						  U32 v16);

#ifdef PHY_RAW_DATA_PRINT
U32 print_phy_primitive(char*pMsg, int nLen, u8 ucSN);
#endif
#endif

