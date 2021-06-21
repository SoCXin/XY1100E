#ifndef __XYCLKCONFIG_H
#define __XYCLKCONFIG_H

#include "softap_macro.h"

extern volatile unsigned char g_XY_HCLK_DIV;				//4
extern volatile unsigned char g_XY_PCLK_DIV;				//1
extern volatile unsigned long g_XY_HCLK;
extern volatile unsigned long g_XY_PCLK;
extern volatile unsigned long g_OS_SYS_CLOCK;
extern volatile unsigned short g_XY_RC32K_CLK;


#if SAMPLE_RATE_MODE == 1
#define XY_BBPLL_CLK  	391680000
#else
#define XY_BBPLL_CLK  	368640000
#endif


#define XY_UTC_CLK  	g_XY_RC32K_CLK


#define XY_REF_CLK		(XY_BBPLL_CLK/2)

#define XY_HCLK_DIV  	g_XY_HCLK_DIV				

#define XY_HCLK			g_XY_HCLK

#define XY_PCLK_DIV		g_XY_PCLK_DIV				//1

#define XY_PCLK			g_XY_PCLK

#define OS_SYS_CLOCK	g_OS_SYS_CLOCK

#endif /* __XYCLKCONFIG_H */
