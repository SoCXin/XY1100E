#ifndef BASETYPE
#define BASETYPE
#include "softap_macro.h"
#if IS_DSP_CORE
#include "xy_typedef.h"


#ifndef S8
#define         S8 signed char
#endif

#ifndef S16
#define         S16 signed short
#endif

#ifndef S32
#define          S32 signed int 
#endif

#ifndef U16
#define      U16  unsigned short
#endif

#ifndef U32
#define         U32  unsigned int
#endif
#endif

#define LOG_TRUE 1
#define LOG_FALSE 0	

#endif // BASETYPE

