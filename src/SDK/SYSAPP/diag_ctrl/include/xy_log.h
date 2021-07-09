#ifndef _XY_LOG_H
#define _XY_LOG_H
#include "xy_nbiot_module_define.h"
#include "diag_string_print.h"
#include "softap_macro.h"
#include "diag_IsPrintLog.h"
#define XYLOG_MSG_Q_LENGTH 300
#define XYLOG_MSG_Q_ITEM_SIZE 4


#define PrintLogSt						diag_static_log_ram	
#define PrintLogStM3					diag_static_logM3_ram
#if XY_DY_LOG
#define PrintLog						diag_dynamic_log_ram
#else
#define PrintLog(dyn_id, src_id, lev, fmt, ...) diag_static_log_ram(src_id,lev, fmt,##__VA_ARGS__)

#endif //end XY_DY_LOG

#define PhyPrintf4(dyn_id, src_id, lev, fmt, v1, v2, v3, v4)                                                            diag_pyh_printk4_ram(dyn_id, src_id, lev, NULL, v1, v2, v3, v4)
#define PhyPrintf8(dyn_id, src_id, lev, fmt, v1, v2, v3, v4, v5, v6, v7, v8)                                            diag_pyh_printk8_ram(dyn_id, src_id, lev, NULL, v1, v2, v3, v4, v5, v6, v7, v8)
#define PhyPrintf12(dyn_id, src_id, lev, fmt, v1, v2, v3, v4, v5, v6, v7, v8, v9, v10, v11, v12)                        diag_pyh_printk12_ram(dyn_id, src_id, lev, NULL, v1, v2, v3, v4, v5, v6, v7, v8, v9, v10, v11, v12)
#define PhyPrintf16(dyn_id, src_id, lev, fmt, v1, v2, v3, v4, v5, v6, v7, v8, v9, v10, v11, v12, v13, v14, v15, v16)    diag_pyh_printk16_ram(dyn_id, src_id, lev, NULL, v1, v2, v3, v4, v5, v6, v7, v8, v9, v10, v11, v12, v13, v14, v15, v16)
#define PhyRawPrint(pMsg, len, ucSN)                                                                                    print_phy_primitive(pMsg, len, ucSN)//print_phy_primitive(src_id, pMsg, len)

#define OutputSNVersionMessage(outLevel, outString, ...) diag_static_log(outLevel, outString, ##__VA_ARGS__)


#endif

