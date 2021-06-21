#ifndef __HW_INTS_H__
#define __HW_INTS_H__

//*****************************************************************************
//
// The following are defines for the fault assignments.
//
//*****************************************************************************
#define FAULT_NMI               2       // NMI fault
#define FAULT_HARD              3       // Hard fault
#define FAULT_MPU               4       // MPU fault
#define FAULT_BUS               5       // Bus fault
#define FAULT_USAGE             6       // Usage fault
#define FAULT_SVCALL            11      // SVCall
#define FAULT_DEBUG             12      // Debug monitor
#define FAULT_PENDSV            14      // PendSV
#define FAULT_SYSTICK           15      // System Tick

//*****************************************************************************
//
// The following are defines for the interrupt assignments.
//
//*****************************************************************************
#define INT_CSP1		        16	    // 
#define INT_I2C1		        17	    // 
#define INT_SPI			        18	    // 
#define INT_TIMER1		        19	    // 
#define INT_UART1		        20	    // 
#define INT_UTC			        21	    // 
#define INT_WDT			        22	    // 
#define INT_MEASURECNT		    23	    // 
#define INT_TIMER2		        24	    // 
#define INT_CSP2		        25	    // 
#define INT_ROOTCLK_XTAL	    26	    // 
#define INT_ROOTCLK_PLL		    27	    // 
#define INT_QSPI		        28	    // 
#define INT_SEMAPHORE		    29	    // 
#define INT_TIMER3		        30	    // 
#define INT_TIMER4		        31	    // 
#define INT_DMAC0		        32	    // 
#define INT_DMAC1		        33	    // 
#define INT_DMAC2		        34	    // 
#define INT_DMAC3		        35	    // 
#define INT_WAKEUP		        36	    // 
#define INT_GPIO                37      //
#define INT_I2C2                38      //
#define INT_CSP3                39      //
#define INT_CSP4                40      //
#define INT_DFEBUF              41      //


//*****************************************************************************
//
// The following are defines for the total number of interrupts.
//
//*****************************************************************************
#define NUM_INTERRUPTS          42

//*****************************************************************************
//
// The following are defines for the total number of priority levels.
//
//*****************************************************************************
#define NUM_PRIORITY            8
#define NUM_PRIORITY_BITS       3

#endif // __HW_INTS_H__
