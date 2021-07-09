/*******************************************************************************
 *							 Include header files							   *
 ******************************************************************************/
#include "sys_init.h"
#include "xy_utils.h"
#include "hw_memmap.h"
#include "hw_csp.h"
#include "gpio.h"
#include "hw_types.h"
#include "bitband.h"
#include "rf_drv.h"
#include "at_global_def.h"

#define DIRTY_DATA_DEBUG      1
#define DIRTY_WAKEUP_REASON   1
#define DIRTY_DEBUG_ASSERT    0

// current used peripheral
#define CUR_CSP_UART_BASE       AT_UART_CSP

#define ONE_BIT_SAMPLE_NUM      8
// 1 start + 8 data + 1 stop + 1 idle
#define CSP_DATA_BITS           10
//max sample times, 3 characters, 11 bits in every character, 4 times sample in every bit
#define MAX_SAMPLE_NUM          (3 * CSP_DATA_BITS * ONE_BIT_SAMPLE_NUM)

// detected data, the third character of AT command
//extern char g_AT_Req_third_chars[];
extern char *p_AT_Req_third_char;
extern uint32_t g_third_char_num;



typedef struct
{
	volatile uint8_t  special_id;  // use for standby wakeup dirty data, in at_farps_channel.c judge and clear
	                               // see variable g_AT_Req_third_chars at at_hook_func.c
	volatile uint8_t  bit_num_cnt; // current sample bit counter
	volatile uint16_t sample_flag; // bit0, start flag, if set to 1, it means detected falling edge, maybe it a start bit
	                               // bit1, end flag, if set to 1, end sampling
	                               // bit2, last sample bit, compare to current sample
	                               // bit3, sample use clock, 0 is crystal, 1 is pll
	                               // bit4, give sem flag, if set to 1, need give semaphore after RestoreIntEnable()
	                               // bit5, if set 1, it means current wakeup enter soft sample
	                               // bit6, soft sample failure flag, need response 8003
	                               // bit7, utc sample success flag, if set 1, it means utc sample success
	                               // bit8, if set to 1, is means enter crystal sample, use for respons 8003
	
	volatile uint8_t  wakeup_rea;  // wakeup reason, 0: null  1: gpi  2: utc  3. others
	volatile uint8_t  wakeup_sta;  // record the the parameter of wakeup status
	volatile uint8_t  fail_rea[2]; // sample fail reason
	                               // if set 0, it means sample success
	                               // if set 1, it means sample failuer and current sample has no data
	                               // if set 2, it means sample failuer and current sample has received data
								   
	volatile uint32_t rcv_dat;	   // software receive data

	volatile uint16_t sample_cnt;  // in every timer interrupt, sample_cnt++, include invalid sample
	volatile uint16_t start_cnt;   // if detected a falling edge, count start, Record all samples after that

	volatile uint8_t  data_cnt;    // erver falling edge and rasing edge clear this bit
	volatile uint8_t  backup_special_id;   // special_id's backup, id will use before print, need backup to print
	volatile uint8_t  backup_sys_div;      // sys clock div's backup, backup HWREGB(0xA011000C)
	volatile uint8_t  reserved;
	
	volatile uint32_t backup_csp_mode;     // csp mode register's backup, backup HWREG(CUR_CSP_UART_BASE+CSP_MODE1)

#if DIRTY_DATA_DEBUG
	volatile uint16_t enter_cnt[4];   // the times to enter sample, member0:   gpi member1: utc
	volatile uint16_t success_cnt[4]; // the times to sample success, member0:   gpi member1: utc
#endif
} soft_sample_t;

soft_sample_t gpi_utc = {0};
//soft_sample_t *cur_sam = &gpi_utc;

// define bitband operation
#define SAMPLE_START_BIT          0    // if set to 1, it means detected falling edge, maybe it a start bit
#define SAMPLE_END_BIT            1    // if set to 1, end sampling
#define SAMPLE_LAST_LEVEL_BIT     2    // the last sample level
#define SAMPLE_REASON_BIT         3    // 0: crystal sample, 1: pll sample
#define SAMPLE_GIVE_SEM_BIT       4    // 1: need give semaphore after RestoreIntEnable()
#define SAMPLE_WAKEUP_BIT         5    // if set 1, it means current wakeup enter soft sample
#define SAMPLE_GPI_FAIL_BIT       6    // 1: current sample failure and has receive data, need response 8003
#define SAMPLE_UTC_SUC_BIT        7    // utc wakeup sample success flag, 1: utc sample success
#define SAMPLE_FIRST_ENTER_BIT    8    // enter crystal sample flag, 1: crystal sample enter
#if 0
#define SAMPLE_START_FLAG          MEM_BIT_ADDR(&cur_sam->sample_flag, SAMPLE_START_BIT)
#define SAMPLE_END_FLAG            MEM_BIT_ADDR(&cur_sam->sample_flag, SAMPLE_END_BIT)
#define SAMPLE_LAST_BIT_LEVEL      MEM_BIT_ADDR(&cur_sam->sample_flag, SAMPLE_LAST_LEVEL_BIT)
#define SAMPLE_REASON_FLAG         MEM_BIT_ADDR(&cur_sam->sample_flag, SAMPLE_REASON_BIT)
#define SAMPLE_GIVE_SEM_FLAG       MEM_BIT_ADDR(&cur_sam->sample_flag, SAMPLE_GIVE_SEM_BIT)
#define SAMPLE_WAKEUP_FLAG         MEM_BIT_ADDR(&cur_sam->sample_flag, SAMPLE_WAKEUP_BIT)
#define SAMPLE_GPI_FAIL_FLAG       MEM_BIT_ADDR(&cur_sam->sample_flag, SAMPLE_GPI_FAIL_BIT)
#define SAMPLE_UTC_SUC_FLAG        MEM_BIT_ADDR(&cur_sam->sample_flag, SAMPLE_UTC_SUC_BIT)
#define SAMPLE_FIRST_ENTER_FLAG    MEM_BIT_ADDR(&cur_sam->sample_flag, SAMPLE_FIRST_ENTER_BIT)
#else
#define SAMPLE_START_FLAG          MEM_BIT_ADDR(&gpi_utc.sample_flag, SAMPLE_START_BIT)
#define SAMPLE_END_FLAG            MEM_BIT_ADDR(&gpi_utc.sample_flag, SAMPLE_END_BIT)
#define SAMPLE_LAST_BIT_LEVEL      MEM_BIT_ADDR(&gpi_utc.sample_flag, SAMPLE_LAST_LEVEL_BIT)
#define SAMPLE_REASON_FLAG         MEM_BIT_ADDR(&gpi_utc.sample_flag, SAMPLE_REASON_BIT)
#define SAMPLE_GIVE_SEM_FLAG       MEM_BIT_ADDR(&gpi_utc.sample_flag, SAMPLE_GIVE_SEM_BIT)
#define SAMPLE_WAKEUP_FLAG         MEM_BIT_ADDR(&gpi_utc.sample_flag, SAMPLE_WAKEUP_BIT)
#define SAMPLE_GPI_FAIL_FLAG       MEM_BIT_ADDR(&gpi_utc.sample_flag, SAMPLE_GPI_FAIL_BIT)
#define SAMPLE_UTC_SUC_FLAG        MEM_BIT_ADDR(&gpi_utc.sample_flag, SAMPLE_UTC_SUC_BIT)
#define SAMPLE_FIRST_ENTER_FLAG    MEM_BIT_ADDR(&gpi_utc.sample_flag, SAMPLE_FIRST_ENTER_BIT)
#endif

// use for soft_sample_t's fail_rea[2]
#define SAMPLE_NOT_ENTER            0
#define SAMPLE_SUCCESS              1
#define SAMPLE_FAIL_ALL_IDLE        2
#define SAMPLE_FAIL_WITH_DATA       3

// use fo soft_sample_t's wakeup_rea
#define WAKEUP_FROM_GPI0            1
#define WAKEUP_FROM_UTC             2
#define WAKEUP_FROM_OTHERS          3
#define WAKEUP_FROM_GPI1            4

//use for SAMPLE_REASON_FLAG
#define SAMPLE_USE_CRYSTAL          0
#define SAMPLE_USE_PLL              1

// use for SAMPLE_FIRST_ENTER_FLAG
#define CRYSTAL_SAMPLE_NOT_ENTER    0
#define CRYSTAL_SAMPLE_ENTER        1


#ifdef __CC_ARM

__asm volatile void BackupIntEnable(uint32_t *backup)
{
	// protect register
	PUSH {R1-R3}
	// backup[2] = HWREG(0xE000E100);
	LDR R1,=0xE000E100
	LDR R1, [R1]
	STR R1, [R0, #4]
	// HWREG(0xE000E180) = 0xFFFFFFFF;
	LDR R1, =0xFFFFFFFF
	LDR R2, =0xE000E180
	STR R1, [R2]
	// backup[2] = HWREG(0xE000E010);
	LDR R1, =0xE000E010
	LDR R2, [R1]
	STR R2, [R0, #8]
	// HWREG(0xE000E010) &= 0xFFFFFFFD;
	LDR R3, =0xFFFFFFFD
	AND R2, R3
	STR R2, [R1]
	// backup PRIMASK
	MRS R1, PRIMASK
	STR R1, [R0]
	CPSIE I
	// restore protect register
	POP {R1-R3}

	BX  LR
}

__asm volatile void RestoreIntEnable(uint32_t *backup)
{
	// protect register
	PUSH {R1-R2}
	// restore PRIMASK
	LDR R1, [R0]
	MSR PRIMASK, R1
	// HWREG(0xE000E180) = 0xFFFFFFFF;
	LDR R1, =0xE000E180
	LDR R2, =0xFFFFFFFF
	STR R2, [R1]
	// HWREG(0xE000E100) = *(backup + 1);
	LDR R1, =0xE000E100
	LDR R2, [R0, #4]
	STR R2, [R1]
	// HWREG(0xE000E010) = *(backup + 2);
	LDR R1, =0xE000E010
	LDR R2, [R0, #8]
	LDR R2, [R1]
	// restore protect register
	POP {R1-R2}

	BX  LR
}

#endif


#ifdef __GNUC__

void BackupIntEnable(uint32_t *backup)
{
	// backup PRIMASK, 0xE000E100, 0xE000E010,
	__asm__ __volatile__ (
			"MRS %[output], PRIMASK		\n\t"
			: [output] "=r"(backup[0])
		);
	backup[1] = HWREG(0xE000E100);	//nvic int enable
	backup[2] = HWREG(0xE000E010);	//systick ctrl
	backup[3] = HWREG(0xE000E014);  //systick reload
	backup[4] = (uint32_t)g_pfnVectors[15];  //interrupt vector

	// disable all external interrupt
	HWREG(0xE000E180) = 0xFFFFFFFF;

	// disable SysTick interrupt
	HWREG(0xE000E010) = 0;

	// enable global interrupt
	__asm__ __volatile__ ("CPSIE I");
}
//yus
void RestoreIntEnable(uint32_t *backup)
{
	// Restore PRIMASK
	__asm__ __volatile__ (
			"MSR PRIMASK, %[input]	\n\t"
			: : [input] "r"(backup[0])
		);

	// disable all external interrupt
	HWREG(0xE000E180) = 0xFFFFFFFF;

	// Restore 0xE000E100, 0xE000E010, 0xE000E014
	HWREG(0xE000E100) = backup[1];	//restore nvic int enable
	HWREG(0xE000E010) = backup[2];	//restore systick ctrl
	HWREG(0xE000E014) = backup[3];  //restore systick reload
	g_pfnVectors[15]  = backup[4];  //restore interrupt vector
}

#endif


// timer handler
void soft_sample_timer_handler(void)
{
	uint8_t rxd_level;

	// read current RXD level
	//rxd_level = (HWREG(CUR_CSP_UART_BASE + 0x1C) >> 2) & 1U;
	rxd_level = (HWREG(GPIO_BASE + GPIO_DIN0) >> (g_softap_fac_nv->csp2_rxd_pin)) & 1U;

	// sample counter
	gpi_utc.sample_cnt++;

	// the sampling cannot continue forever
	if((gpi_utc.sample_cnt > CSP_DATA_BITS * ONE_BIT_SAMPLE_NUM) && (gpi_utc.start_cnt == 0))
	{
		SAMPLE_END_FLAG = 1;
		gpi_utc.fail_rea[SAMPLE_REASON_FLAG] = SAMPLE_FAIL_ALL_IDLE;
	}
	if(gpi_utc.start_cnt > MAX_SAMPLE_NUM)
	{
		SAMPLE_END_FLAG = 1;
		gpi_utc.fail_rea[SAMPLE_REASON_FLAG] = SAMPLE_FAIL_WITH_DATA;
	}

	// sample is not end
	if(SAMPLE_END_FLAG == 0)
	{
		// wait a falling edge
		if((rxd_level == 1) && (SAMPLE_START_FLAG == 0))
		{
			SAMPLE_START_FLAG = 1;
			// save the idle bit
			gpi_utc.rcv_dat = 0x01;
			gpi_utc.bit_num_cnt = 1;
		}

		// wait the first low level
		if(SAMPLE_START_FLAG == 1)
		{
			// sample has started or it just the first low level
			if((gpi_utc.start_cnt != 0) || (rxd_level == 0))
			{
				gpi_utc.start_cnt++;

				// every falling edge or rising edge synchronization
				if(rxd_level != SAMPLE_LAST_BIT_LEVEL)
				{
					gpi_utc.data_cnt = 0;
				}

				// set last bit
				SAMPLE_LAST_BIT_LEVEL = rxd_level;
				// use to get valid data
				gpi_utc.data_cnt++;
			}
			
			// in every (ONE_BIT_SUMPLE_NUM / 2) times sample is valid
			if((gpi_utc.data_cnt % ONE_BIT_SAMPLE_NUM) == (ONE_BIT_SAMPLE_NUM / 2))
			{
				// bit band operation, write bit
				MEM_BIT_ADDR(&gpi_utc.rcv_dat, gpi_utc.bit_num_cnt) = rxd_level;
				
				// record sample bits
				gpi_utc.bit_num_cnt++;

				//user can add special char here
				if(gpi_utc.bit_num_cnt >= 10)
				{
					char idle_bit     = MEM_BIT_ADDR(&gpi_utc.rcv_dat, gpi_utc.bit_num_cnt - 10);
					char start_bit    = MEM_BIT_ADDR(&gpi_utc.rcv_dat, gpi_utc.bit_num_cnt - 9);
					//char stop_bit     = MEM_BIT_ADDR(&gpi_utc.rcv_dat, gpi_utc.bit_num_cnt - 1);

					if((idle_bit == 1) && (start_bit == 0))// && (stop_bit == 1))
					{
						char special_char = (char)((gpi_utc.rcv_dat >> (gpi_utc.bit_num_cnt - 8)) & 0xFF);

						for(uint32_t i=0; i<g_third_char_num; i++)
						{
							if(special_char == p_AT_Req_third_char[i])
							{
								gpi_utc.special_id = i + 1;
								break;
							}
							gpi_utc.special_id = 0;
						}
					}

					if(gpi_utc.special_id != 0)
					{
						// Restore the previous register status
						HWREGB(0xA011000C) = gpi_utc.backup_sys_div;

						// wait stop bit to enable csp
						while(((HWREG(GPIO_BASE + GPIO_DIN0) >> (g_softap_fac_nv->csp2_rxd_pin)) & 1U) == 0);

						// csp enable
						//HWREG(CUR_CSP_UART_BASE + CSP_MODE1) |= CSP_MODE1_CSP_EN;
						HWREG(CUR_CSP_UART_BASE + CSP_MODE1) = gpi_utc.backup_csp_mode;

						SAMPLE_END_FLAG = 1;  // set end

					#if DIRTY_DATA_DEBUG
						gpi_utc.success_cnt[gpi_utc.wakeup_rea]++;
						if(gpi_utc.wakeup_rea == WAKEUP_FROM_UTC)	//utc wakeup and sample success
							SAMPLE_UTC_SUC_FLAG = 1;
					#endif
					}
				}
			}
		}
	}
}

// sample success, not enter next sample
// sample success or sample failur with data, not enter next sample
#define USE_CONDITON   0

// current baudrate
extern volatile unsigned int g_current_baudrate;
void soft_sample_init(uint32_t type)
{
	uint32_t backup_IntDat[5];

	// Backup interrupt enable status
	BackupIntEnable(backup_IntDat);

	// Backup CSP and Clock register
	gpi_utc.backup_sys_div  = HWREGB(0xA011000C);
	if(g_softap_fac_nv->work_mode!=2)
		gpi_utc.backup_sys_div &= 0xFE;
	gpi_utc.backup_csp_mode = HWREG(CUR_CSP_UART_BASE + CSP_MODE1) | CSP_MODE1_CSP_EN;

	// set last bit level, init high to idle
	SAMPLE_LAST_BIT_LEVEL = 1;
	
	// clear all sample data
	gpi_utc.special_id  = 0;
	gpi_utc.bit_num_cnt = 0;
	SAMPLE_START_FLAG   = 0;
	SAMPLE_END_FLAG     = 0;
	gpi_utc.rcv_dat     = 0;
	gpi_utc.sample_cnt  = 0;
	gpi_utc.start_cnt   = 0;
	gpi_utc.data_cnt    = 0;

#if DIRTY_DATA_DEBUG
	gpi_utc.enter_cnt[gpi_utc.wakeup_rea]++;
#endif

	// save sample reason, timer handler use
	SAMPLE_REASON_FLAG = type;

	// Set RXD PIN IO mode and Input mode, set bit15 and bit20 to 1
	//HWREG(CUR_CSP_UART_BASE + 0x00) = 0x00108000;

	// crystal sample
	if(SAMPLE_REASON_FLAG == SAMPLE_USE_CRYSTAL)
	{
		// switch to xtal
		HWREGB(0xA011000C) = 0x01;

		// set crystal sample flag
		SAMPLE_FIRST_ENTER_FLAG = CRYSTAL_SAMPLE_ENTER;

		// clear sample fail reason
		gpi_utc.fail_rea[0] = 0;
		gpi_utc.fail_rea[1] = 0;
	}

	// if first sample not enter, need clear faile reason, respons 8003 needed
	if(SAMPLE_FIRST_ENTER_FLAG == CRYSTAL_SAMPLE_NOT_ENTER)
	{
		// clear sample fail reason
		gpi_utc.fail_rea[0] = 0;
		gpi_utc.fail_rea[1] = 0;
	}

	// clear systick count
	HWREG(0xE000E018) = 0;

	if(SAMPLE_REASON_FLAG == SAMPLE_USE_CRYSTAL)
	{
		HWREG(0xE000E014) = g_softap_fac_nv->xtal_freq / g_current_baudrate / ONE_BIT_SAMPLE_NUM;
	}
	else if(SAMPLE_REASON_FLAG == SAMPLE_USE_PLL)
	{
		HWREG(0xE000E014) = XY_HCLK / g_current_baudrate / ONE_BIT_SAMPLE_NUM;
	}

	// Interrupt register
	g_pfnVectors[15] = (uint32_t)(soft_sample_timer_handler);

	// Enable Systick Interrupt
	HWREG(0xE000E010) = 0x7;

	// wait to close timer
	while(SAMPLE_END_FLAG == 0);

	// disable systick interrupt
	HWREG(0xE000E010) = 0;

	// if sample failed, timer handler not restore
	if(gpi_utc.special_id == 0)
	{
		// Restore the previous register status
		HWREGB(0xA011000C) = gpi_utc.backup_sys_div;
		
#if USE_CONDITON
		// current sample is use PLL, restore csp
		if(SAMPLE_REASON_FLAG == SAMPLE_USE_PLL)
#else
		// sample failed, and crystal sample with data, or current sample is use PLL, restore csp
		if((gpi_utc.fail_rea[0] == SAMPLE_FAIL_WITH_DATA) || (SAMPLE_REASON_FLAG == SAMPLE_USE_PLL))
#endif
		{
			// csp enable
			//HWREG(CUR_CSP_UART_BASE + CSP_MODE1) |= CSP_MODE1_CSP_EN;
			HWREG(CUR_CSP_UART_BASE + CSP_MODE1) = gpi_utc.backup_csp_mode;
		}
	}
	else
	{
		// set success flag
		gpi_utc.fail_rea[SAMPLE_REASON_FLAG] = SAMPLE_SUCCESS;
	
		// set to give semaphore
		SAMPLE_GIVE_SEM_FLAG = 1;

		// bug 3692
		// if sample success, give semaphore now, it will schedule after exit critical, so don't need look standby
		//xy_standby_close();
	}

	// backup special id
	gpi_utc.backup_special_id = gpi_utc.special_id;

	// restore interrupt enable status
	RestoreIntEnable(backup_IntDat);

	// give semaphore, can't give before RestoreIntEnable(), otherwise it will be trigger interrupt(PendSV)
	if(SAMPLE_GIVE_SEM_FLAG == 1)
	{
		SAMPLE_GIVE_SEM_FLAG = 0;
		osSemaphoreRelease(g_at_uart_sem);
	}

#if DIRTY_DEBUG_ASSERT
	if(gpi_utc.special_id == 0)
	{
		xy_assert(0);
	}
#endif
}

// called at LPM_ARM_Entry_Standby() in lpm_func.c
void soft_sample_from_standby(uint32_t type, uint32_t wakeup_status)
{
	// write wakeup state to memory
	{
		gpi_utc.wakeup_sta = wakeup_status;

		if(wakeup_status & 0x04)		// wakeup from gpi0
			gpi_utc.wakeup_rea = WAKEUP_FROM_GPI0;
		else if(wakeup_status & 0x01)	// wakeup from utc
			gpi_utc.wakeup_rea = WAKEUP_FROM_UTC;
#if BLE_ENABLE
		else if(wakeup_status & 0x08)	// wakeup from gpi1
			gpi_utc.wakeup_rea = WAKEUP_FROM_GPI1;
#endif
		else							// other reason
			gpi_utc.wakeup_rea = WAKEUP_FROM_OTHERS;
	}
	
	SAMPLE_WAKEUP_FLAG = 1;

	if((g_current_baudrate <= 9600)
#if BLE_ENABLE
	&& (gpi_utc.wakeup_rea != WAKEUP_FROM_GPI1)
#endif
	)
	{
		if(type == 0)	//crystal
		{
			soft_sample_init(0);
		}
		else			//pll
		{
#if USE_CONDITON
			// first sample failur, enter second sample
			if(gpi_utc.special_id == 0)
#else
			// first sample failur and sample without data, or first sample not enter, enter second sample
			if((gpi_utc.fail_rea[0] == SAMPLE_FAIL_ALL_IDLE) || (SAMPLE_FIRST_ENTER_FLAG == CRYSTAL_SAMPLE_NOT_ENTER))
#endif
			{
				soft_sample_init(1);
			}
				
			// whatever the second sample is enter or not, clear this flag
			SAMPLE_FIRST_ENTER_FLAG = CRYSTAL_SAMPLE_NOT_ENTER;
		}

#if USE_CONDITON
		// four condition
		// first failure is idle, second failure is idle, wakeup because gpi
		// first failure is idle, second failure is no matching character
		// first failure is no matching character, second failure is idle
		// first failure is no matching character, second failure is no matching character
		if(	((gpi_utc.fail_rea[0] == SAMPLE_FAIL_ALL_IDLE)  && (gpi_utc.fail_rea[1] == SAMPLE_FAIL_ALL_IDLE)   && (gpi_utc.wakeup_rea == WAKEUP_FROM_GPI0))  || \
			((gpi_utc.fail_rea[0] == SAMPLE_FAIL_WITH_DATA) && (gpi_utc.fail_rea[1] == SAMPLE_FAIL_ALL_IDLE))  || \
			(gpi_utc.fail_rea[1] == SAMPLE_FAIL_WITH_DATA) )
#else
		// three condition
		// first failure is no matching character, second is not enter
		// first failure is idle, second failure is no matching character
		// first failure is idle, second failure is idle, wakeup because gpi
		if(	(gpi_utc.fail_rea[0] == SAMPLE_FAIL_WITH_DATA) || \
			(gpi_utc.fail_rea[1] == SAMPLE_FAIL_WITH_DATA) || \
			((gpi_utc.fail_rea[0] == SAMPLE_FAIL_ALL_IDLE) && (gpi_utc.fail_rea[1] == SAMPLE_FAIL_ALL_IDLE) && (gpi_utc.wakeup_rea == WAKEUP_FROM_GPI0)) )
#endif
		{
			//gpi_utc.fail_rea[0] = 0;
			//gpi_utc.fail_rea[1] = 0;
			SAMPLE_GPI_FAIL_FLAG = 1;
			osSemaphoreRelease(g_at_uart_sem);
		}
	}
	else
	{
		volatile uint32_t delay;

		CSPEnable(AT_UART_CSP);

		if(HWREGB(0xA011000D) & 0x01)
		{
			if(g_softap_fac_nv->work_mode!=2)
			{
				// switch to pll
				for(delay=0;delay<3000;delay++);
				PRCM_Clock_Mode_Auto();
				while((!(HWREGB(0xa011000D)&0x04))||(!(HWREGB(0xa011000D)&0x02)));
			}
			// high baudrate and wakeup from gpi, response 8003
			if(wakeup_status & 0x04)
			{
				SAMPLE_GPI_FAIL_FLAG = 1;
				osSemaphoreRelease(g_at_uart_sem);
			}
		}
	}
}

#if 1

/* text at flash */
//uint32_t soft_sample_get_special_id(void)   __attribute__((section(".flashtext")));
void soft_sample_fill_char(char *buf, int *data_len)        __attribute__((section(".flashtext")));
/* print log */
//void soft_sample_print_detect_data(void)    __attribute__((section(".flashtext")));
void soft_sample_print_string_hex(char *rcvd_men, int rcvd_len)     __attribute__((section(".flashtext")));
//void soft_sample_print_wakeup_reason(void)  __attribute__((section(".flashtext")));
void soft_sample_print_utc_data(void)       __attribute__((section(".flashtext")));
void soft_sample_print_8003_log(char *rcvd_mem, int rcvd_len) __attribute__((section(".flashtext")));

#endif


// called at at_recv_from_uart() in at_farps_channal.c
uint32_t soft_sample_get_special_id(void)
{
	return gpi_utc.special_id;
}

// called at AT_uart_rx_task() in sys_app_init.c
void soft_sample_fill_char(char *buf, int *data_len)
{
	int len = *data_len;
	
	if(gpi_utc.special_id != 0)
	{
		// all data right shifter 3 bytes, voidance space to save "AT+"
		for(int i=0; i<len; i++)
		{
			buf[len+3-1-i] = buf[len-1-i];
		}
		
		// add "AT+"
		buf[0] = 'A';
		buf[1] = 'T';
		buf[2] = p_AT_Req_third_char[gpi_utc.special_id - 1];

		// lenth add 3, soft add "AT+"
		*data_len = len + 3;

		// clear id
		gpi_utc.special_id = 0;
		
		return;
	}
	
	if(len > 0)
	{
		SAMPLE_GPI_FAIL_FLAG = 0;
		return;
	}
	
	if(SAMPLE_GPI_FAIL_FLAG == 1)
	{
		buf[0] = 'A';
		buf[1] = 'B';
		buf[2] = 'C';

		*data_len = 3;

		SAMPLE_GPI_FAIL_FLAG = 0;
		
		return;
	}
}


// called at current file
void soft_sample_print_detect_data(void)
{
	uint32_t reason = gpi_utc.wakeup_rea;

	if(reason == WAKEUP_FROM_GPI0)		//gpi
		softap_printf(USER_LOG, WARN_LOG, "gpi external value:\r\n");
	else if(reason == WAKEUP_FROM_UTC)	//utc
		softap_printf(USER_LOG, WARN_LOG, "utc external value:\r\n");
	else	//others
		softap_printf(USER_LOG, WARN_LOG, "others external value:\r\n");

#if DIRTY_DATA_DEBUG
	softap_printf(USER_LOG, WARN_LOG, "dirty external: %u,%u,%u,%u,0x%X,0x%X,0x%X,%u,%u,%u,%u\r\n",
			gpi_utc.sample_cnt, gpi_utc.start_cnt, gpi_utc.data_cnt, gpi_utc.bit_num_cnt, gpi_utc.sample_flag, gpi_utc.wakeup_sta,
			gpi_utc.rcv_dat, gpi_utc.fail_rea[0], gpi_utc.fail_rea[1], gpi_utc.enter_cnt[reason], gpi_utc.success_cnt[reason]);
#else
	softap_printf(USER_LOG, WARN_LOG, "dirty external: %u,%u,%u,%u,0x%X,0x%X,0x%X,%u,%u\r\n",
			gpi_utc.sample_cnt, gpi_utc.start_cnt, gpi_utc.data_cnt, gpi_utc.bit_num_cnt, gpi_utc.sample_flag, gpi_utc.wakeup_sta,
			gpi_utc.rcv_dat, gpi_utc.fail_rea[0], gpi_utc.fail_rea[1]);
#endif

	// csp interrupt state
	softap_printf(USER_LOG, WARN_LOG, "csp state: %X\r\n", HWREG(0xA0100018));
	HWREG(CUR_CSP_UART_BASE + 0x18) = 1 << 10;
}

// called at current file
void soft_sample_print_string_hex(char *rcvd_mem, int rcvd_len)
{
	//softap_printf(USER_LOG, WARN_LOG, "dirty string: %s\r\n", g_farps_rcv_mem);

	uint8_t *tmp_str = xy_zalloc(128);

	for (int i = 0; ((i < rcvd_len) && (i < 30)); i++)
	{
		sprintf((char *)(&tmp_str[i*3]), "%02X ", rcvd_mem[i]);
	}
	softap_printf(USER_LOG, WARN_LOG, "dirty string hex: %s\r\n", tmp_str);

	xy_free(tmp_str);
}

/* debug code */
// called at xy_idle_sleep() in low_power.c
void soft_sample_print_wakeup_reason(void)
{
	if(SAMPLE_WAKEUP_FLAG == 0)
		return;
	else
		SAMPLE_WAKEUP_FLAG = 0;
	
#if (DIRTY_DATA_DEBUG && DIRTY_WAKEUP_REASON)
	uint32_t reason = gpi_utc.wakeup_rea;

	if(reason == WAKEUP_FROM_GPI0)	// wakeup from utc
	{
		if((gpi_utc.fail_rea[0] == SAMPLE_SUCCESS) || (gpi_utc.fail_rea[1] == SAMPLE_SUCCESS))
		{
			softap_printf(USER_LOG, WARN_LOG, "gpi0 sample success from sample %d, and id is %d\r\n", SAMPLE_REASON_FLAG, gpi_utc.backup_special_id);
		}
		else
		{
			softap_printf(USER_LOG, WARN_LOG, "gpi0 sample failure\r\n");
			soft_sample_print_detect_data();
		}
	}
	else if(reason == WAKEUP_FROM_UTC)	// wakeup from utc
	{
		if((gpi_utc.fail_rea[0] == SAMPLE_SUCCESS) || (gpi_utc.fail_rea[1] == SAMPLE_SUCCESS))
		{
			softap_printf(USER_LOG, WARN_LOG, "utc sample success from sample %d, and id is %d\r\n", SAMPLE_REASON_FLAG, gpi_utc.backup_special_id);
		}
		else if((gpi_utc.fail_rea[0] == SAMPLE_FAIL_WITH_DATA) || (gpi_utc.fail_rea[1] == SAMPLE_FAIL_WITH_DATA))
		{
			softap_printf(USER_LOG, WARN_LOG, "utc wakeup with data sample failure");
			soft_sample_print_detect_data();
		}
		else
		{
			softap_printf(USER_LOG, WARN_LOG, "utc wakeup without data\r\n");
		}
	}
#if BLE_ENABLE
	else if(reason == WAKEUP_FROM_GPI1)	// wakeup from ble
	{
		softap_printf(USER_LOG, WARN_LOG, "gpi1 wakeup because ble receive data\r\n");
	}
#endif
	else	// wakeup from others
	{
		softap_printf(USER_LOG, WARN_LOG, "others wakeup status is 0x%X\r\n", gpi_utc.wakeup_sta);
		
		if((gpi_utc.fail_rea[0] == SAMPLE_SUCCESS) || (gpi_utc.fail_rea[1] == SAMPLE_SUCCESS))
		{
			softap_printf(USER_LOG, WARN_LOG, "others sample success from sample %d, with id is %d\r\n", SAMPLE_REASON_FLAG, gpi_utc.backup_special_id);
		}
		else
		{
			softap_printf(USER_LOG, WARN_LOG, "others sample failure\r\n");
			soft_sample_print_detect_data();
		}
	}
#endif
}

// called at at_farps_fd_write() in at_farps_channal.c
void soft_sample_print_utc_data(void)
{
	// utc wakeup sample success,
	if(SAMPLE_UTC_SUC_FLAG == 1)
	{
		SAMPLE_UTC_SUC_FLAG = 0;

		soft_sample_print_detect_data();
	}
}

// 8003 debug print
// called at at_farps_fd_write() in at_farps_channal.c
void soft_sample_print_8003_log(char *rcvd_mem, int rcvd_len)
{
	soft_sample_print_detect_data();

	soft_sample_print_string_hex(rcvd_mem,rcvd_len);
}

