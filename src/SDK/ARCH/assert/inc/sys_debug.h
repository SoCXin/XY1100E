#ifndef __SYS_DEBUG_H__
#define __SYS_DEBUG_H__

#ifdef __cplusplus
 extern "C" {
#endif


#include <stdio.h>
#include "print.h"


/*!< Function Declaration */
void print_assert_message(char *file, int line);
void sys_assert_proc(char *file, int line);

// assert definition
#define Sys_Assert(x)    do{if(!(x)) sys_assert_proc(__FILE__, __LINE__);}while(0)
// print definition
#define Sys_Print(...)   printf(__VA_ARGS__)


#ifdef __cplusplus
}
#endif

#endif  /* end of __SYS_DEBUG_H__ */
