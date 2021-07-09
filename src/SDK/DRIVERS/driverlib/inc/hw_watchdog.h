#ifndef __HW_WATCHDOG_H__
#define __HW_WATCHDOG_H__


//*****************************************************************************
//
// The following are defines for the Watchdog Timer register offsets.
//
//*****************************************************************************
#define WDT_CTL                 0x00000000  //  Watchdog Configure register
#define WDT_DAT                 0x00000004  //  Watchdog Data register
#define WDT_INT_STAT            0x00000008  //  Watchdog Interrupt Status register

//*****************************************************************************
//
// The following are defines for the bit fields in the WDT_CTL register.
//
//*****************************************************************************
#define WDT_CTL_WATCHDOG_EN     (1UL << 5)  //  Enable the Watchdog, active high
#define WDT_CTL_COUNTER_EN      (1UL << 3)  //  Enable the repeated counter timer
#define WDT_CTL_INT_EN          (1UL << 2)  //  Interrupt enable, active high
#define WDT_CTL_RST_EN          (1UL << 0)  //  Reset enable, active high


#endif // __HW_WATCHDOG_H__
