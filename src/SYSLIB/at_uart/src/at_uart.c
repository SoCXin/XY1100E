#include "at_uart.h"
#include "oss_nv.h"
#include "xy_system.h"
#include "hw_gpio.h"
#include "hw_csp.h"
#include "replace.h"
#include "hw_memmap.h"
#include "csp.h"
#include "at_global_def.h"
#include "sys_init.h"
#include "gpio.h"
#include "at_ctl.h"
#include "xy_watchdog.h"
#if BLE_ENABLE
#include "ble_config.h"
#endif

typedef enum
{
	AUTOBAUD_STATE_NONE,
	AUTOBAUD_STATE_SKIP,	
	AUTOBAUD_STATE_INIT,
	AUTOBAUD_STATE_WAITINT_FIRST,
	AUTOBAUD_STATE_WAITINT_SECOND,
	AUTOBAUD_STATE_WAITINT_THIRD,
	AUTOBAUD_STATE_VERIFY,
	AUTOBAUD_STATE_DONE,

}AUTOBAUD_STATE;

#define AUTOBAUD_SUPPORT_NUM  7

unsigned int g_autobaud_table[AUTOBAUD_SUPPORT_NUM] = {2400,4800,9600,19200,38400,57600,115200};

unsigned int g_autobaud_state = AUTOBAUD_STATE_NONE;
unsigned int g_autobaud_tick[3] = {0};
unsigned char g_autobaud_stop_standby = 0;
unsigned int g_autobaud_rate = 0;
volatile uint8_t csp_out_flag = 0;
osSemaphoreId_t g_at_uart_sem = NULL;

int at_write_to_uart(char *buf,int size)
{
	 int i;
	 //bug5325
	if(size <= 0 || g_autobaud_state == 0 || ((HWREG(AT_UART_CSP+CSP_MODE1) & 0x20) == 0) || g_softap_fac_nv->close_at_uart == 1)
		return -1;

	csp_out_flag |= 0x01;

	HWREG(AT_UART_CSP+CSP_INT_STATUS) = CSP_INT_CSP_TX_ALLOUT;
	 
	for(i = 0 ; i < size ; i++ )
	{
		CSPCharPut( AT_UART_CSP, *((char *)buf + i));
	}
	return 0;
}

// return: 
// 0: csp write allout not complete
// 1: csp write allout complete
uint32_t csp_write_allout_state()
{
	// AT CSP all out
	if((csp_out_flag & 0x01) && ( ( HWREG(AT_UART_CSP + CSP_INT_STATUS) & CSP_INT_CSP_TX_ALLOUT) == 0 ) && (g_softap_fac_nv->close_at_uart == 0))
		return 0;
#if BLE_ENABLE
	if((csp_out_flag & 0x02) && ( ( HWREG(BLE_UART_PORT + CSP_INT_STATUS) & CSP_INT_CSP_TX_ALLOUT) == 0 ))
		return 0;
#endif
	
	csp_out_flag = 0;
	return 1;
}

void wait_for_csp_tx_allout(void)
{
	volatile unsigned int delaycnt = 0;
	//special process,bug4939
	if( csp_out_flag & 0x01 )
	{
		while(( HWREG(AT_UART_CSP + CSP_TX_FIFO_STATUS) & CSP_TX_FIFO_STATUS_EMPTY_Msk) == 0);
		for(delaycnt = 0;delaycnt<100000;delaycnt++);//for baudrate 2400,tx shifter,4ms

		csp_out_flag = 0;
	}
}

void xy_tick_delay_for_ATend(void)
{
	// tick_delay would not larger than 1ms,OS_SYS_CLOCK/1000
	uint32_t val_cur,val_target,tick_delay;

	osCoreEnterCritical();
#if IS_DSP_CORE
	tick_delay = 2 * 10 * OS_SYS_CLOCK / g_current_baudrate;	//2 bytes,10 bits

	val_cur = xthal_get_ccount();

	val_target = val_cur + tick_delay;

	if(val_target > val_cur)
	{
		while( (xthal_get_ccount() <= val_target) && (xthal_get_ccount() > val_cur) );
	}
	else if(val_target < val_cur )
	{
		while( (xthal_get_ccount() <= val_target) || (xthal_get_ccount() > val_cur) );
	}
	else
	{
		//impossible actually
		xy_assert(0);
	}
#else
	tick_delay = 2 * 10 * OS_SYS_CLOCK / g_current_baudrate;	//2 bytes,10 bits

	val_cur = SysTick->VAL;

	val_target = (val_cur + OS_SYS_CLOCK/(OS_TICK_RATE_HZ) - tick_delay ) % (OS_SYS_CLOCK/(OS_TICK_RATE_HZ));

	if(val_target > val_cur)
	{
		while( !(SysTick->VAL <= val_target && SysTick->VAL > val_cur) );
	}
	else if(val_target < val_cur )
	{
		while( !(SysTick->VAL <= val_target || SysTick->VAL > val_cur) );
	}
	else
	{
		//impossible actually
		xy_assert(0);
	}
#endif
	osCoreExitCritical();
}

void at_uart_init()
{

	g_at_uart_sem = osSemaphoreNew(0xFFFF, 0);

	if(g_softap_fac_nv->close_at_uart==0 && ((g_softap_fac_nv->peri_remap_enable & 0x2) != 0))
	{
		PRCM_Clock_Enable(PRCM_BASE,AT_UART_CSP_PRCM);

		HWREG(AT_UART_CSP + CSP_RX_FIFO_CTRL) = ((HWREG(AT_UART_CSP + CSP_RX_FIFO_CTRL) & 0x03)|((32-1)<<2));

		HWREG(AT_UART_CSP + CSP_AYSNC_PARAM_REG) = ((HWREG(AT_UART_CSP + CSP_AYSNC_PARAM_REG) & 0xffff0000) | 0x0a);

		CSPIntClear(AT_UART_CSP,CSP_INT_ALL);

		HWREG(AT_UART_CSP + CSP_RX_FIFO_OP) = CSP_RX_FIFO_OP_RESET;

		// Fifo Start
		HWREG(AT_UART_CSP + CSP_RX_FIFO_OP) = CSP_RX_FIFO_OP_START;
		
		CSPIntEnable(AT_UART_CSP,CSP_INT_RX_DONE);
		
	
		NVIC_IntRegister(CSP2_IRQn, at_uart_int_handler, 1);
		
	}
	else
	{
		ASSERT_PRINT_CLOSE();
	}
}


int at_uart_read( char *buf, unsigned long len)
{	
	long cspAtCdata = 0;
    
    unsigned long read_pos = 0;

	if (osOK == osSemaphoreAcquire(g_at_uart_sem, osWaitForever))
	{
		while(read_pos<len)
		{
			cspAtCdata = CSPCharGetNonBlocking(AT_UART_CSP);
			if(cspAtCdata == -1)
				break;
			*(buf + read_pos) = (char)cspAtCdata;
			read_pos++;
		}
		return read_pos;
	
	}

	return -1;
}

void at_uart_int_handler(void)
{
	if((CSPIntStatus(CSP2_BASE) & CSP_INT_RX_DONE) && (HWREG(CSP2_BASE + CSP_INT_ENABLE) & CSP_INT_RX_DONE))
	{
	    CSPIntDisable(CSP2_BASE,CSP_INT_RX_DONE);

	    CSPIntClear(CSP2_BASE,CSP_INT_RXFIFO_THD_REACH|CSP_INT_CSP_RX_TIMEOUT|CSP_INT_RX_DONE);

	 	CSPIntEnable(CSP2_BASE,CSP_INT_RXFIFO_THD_REACH|CSP_INT_CSP_RX_TIMEOUT);

	 	return;
	}

	if((CSPIntStatus(CSP2_BASE) & CSP_INT_CSP_RX_TIMEOUT) && (HWREG(CSP2_BASE + CSP_INT_ENABLE) & CSP_INT_CSP_RX_TIMEOUT))
	{
		CSPIntDisable(CSP2_BASE,CSP_INT_CSP_RX_TIMEOUT);
	}

	CSPIntDisable(CSP2_BASE,CSP_INT_RXFIFO_THD_REACH);

	CSPIntClear(CSP2_BASE,CSP_INT_RXFIFO_THD_REACH|CSP_INT_CSP_RX_TIMEOUT|CSP_INT_RX_DONE);

	osSemaphoreRelease(g_at_uart_sem);
}

//external mcu must send "AT\r\n",A=0x41,T=0x54
void AutoBaudStart()
{
	volatile unsigned int int_status = 0;
	//unsigned int autobaud_measure_cnt = 0;
	unsigned int bits_len1 = 0,bits_len2 = 0;
	//unsigned int systick_reload_times = 0;
	volatile unsigned int systick_reload_flag = 0;
	unsigned int baudrate = 0;
	unsigned int checkvalue = 0;
	unsigned int edge_triggle_cnt = 0;
	unsigned long at_rxd_mask = 0;
	unsigned long last_time_autobaudrate = 0;
	//unsigned char autobaud_num = 0;
	volatile unsigned char softdelay = 0;
	unsigned int rxdpin_last;
	unsigned int rxdpin_current;
	
	if((g_softap_fac_nv->uart_rate & 0x1ff) !=0 || g_softap_fac_nv->close_at_uart ==1  \
		||(g_softap_fac_nv->peri_remap_enable & (1<<REMAP_CSP2)) == 0 || g_softap_fac_nv->csp2_rxd_pin == 0xff)
	{
		g_autobaud_state = AUTOBAUD_STATE_SKIP;
		return;
	}

	if(get_sys_up_stat() == UTC_WAKEUP || get_sys_up_stat() == EXTPIN_WAKEUP)
	{
		//if wakeup from deepsleep,use the baudrate last time got by autobaud
		last_time_autobaudrate = (g_softap_fac_nv->uart_rate >> 9)*2400;
		if(last_time_autobaudrate > 0 && last_time_autobaudrate <= g_autobaud_table[AUTOBAUD_SUPPORT_NUM-1])
		{
			CSPUARTModeSet(AT_UART_CSP,XY_PCLK,last_time_autobaudrate,8,1);

			if(last_time_autobaudrate > 9600)
			{
				g_autobaud_stop_standby = 1;
			}
			g_autobaud_rate = last_time_autobaudrate;
			g_autobaud_state = AUTOBAUD_STATE_SKIP;
			return;
		}
	}


	at_rxd_mask = (1 << g_softap_fac_nv->csp2_rxd_pin);
	
	g_autobaud_state = AUTOBAUD_STATE_INIT;
	
	while(1)
	{
		switch(g_autobaud_state)
		{
			case AUTOBAUD_STATE_INIT:
			{

				NVIC_DisableIRQ(GPIO_IRQn);

				//init gpio falling edge interrupt
				HWREG(GPIO_BASE + GPIO_PAD_CFT0) 		= 		at_rxd_mask;//clear
			    HWREG(GPIO_BASE + GPIO_CTRL0) 			&= 		~at_rxd_mask;//gpio mode
			    HWREG(GPIO_BASE + GPIO_INPUTEN0)  		|= 		at_rxd_mask;//input
			    HWREG(GPIO_BASE + GPIO_OUTPUTEN0) 		|= 		at_rxd_mask;
				HWREG(GPIO_BASE + GPIO_PULLUP0)   		&= 		~at_rxd_mask;//pullup
				HWREG(GPIO_BASE + GPIO_PULLDOWN0) 		|= 		at_rxd_mask;
				HWREG(GPIO_BASE + GPIO_INT_TYPE_LEVEL0) &= 		~at_rxd_mask;//edge
				HWREG(GPIO_BASE + GPIO_INT_BOTH_EDGE0)  &= 		~at_rxd_mask;//single edge
				HWREG(GPIO_BASE + GPIO_INT_FALL_EDGE0)  |= 		at_rxd_mask;//falling edge
				HWREG(GPIO_BASE + GPIO_INT_MASK0) 		&= 		~at_rxd_mask;//unmask
				HWREG(GPIO_BASE + GPIO_INTEN0) 	  		|= 		at_rxd_mask;//enable interrupt

				HWREG(GPIO_BASE + GPIO_CFGSEL0)		&=	(~at_rxd_mask);
				GPIOPeripheralPad(GPIO_CSP2_RXD,g_softap_fac_nv->csp2_rxd_pin);

				int_status = HWREG(GPIO_BASE + GPIO_INT_STAT0);//clear int status

				HWREG(AT_UART_CSP + CSP_MODE1) &= ~CSP_MODE1_CSP_EN;//disable csp2

				
				//init systick
				SysTick->LOAD  = (uint32_t)SysTick_LOAD_RELOAD_Msk;                         /* set reload register */
			 	SysTick->VAL   = 0UL;                                             /* Load the SysTick Counter Value */
			 	SysTick->CTRL  = SysTick_CTRL_CLKSOURCE_Msk |
			                     //SysTick_CTRL_TICKINT_Msk   |
			                     SysTick_CTRL_ENABLE_Msk;                         /* Enable SysTick IRQ and SysTick Timer */
				
				g_autobaud_state = AUTOBAUD_STATE_WAITINT_FIRST;	
				break;
			}
			
			case AUTOBAUD_STATE_WAITINT_FIRST:
			{
				do{
					int_status = HWREG(GPIO_BASE + GPIO_DIN0);
					g_autobaud_tick[0] = SysTick->VAL;
				}while((int_status & at_rxd_mask) != 0);
				
				for(softdelay=0;softdelay<10;softdelay++);

				//first falling edge
				g_autobaud_state = AUTOBAUD_STATE_WAITINT_SECOND;	
				break;
			}
				
			case AUTOBAUD_STATE_WAITINT_SECOND:
			{
				systick_reload_flag = SysTick->CTRL;//clear flag
				systick_reload_flag = 0;
				
				do{
					int_status = HWREG(GPIO_BASE + GPIO_DIN0);
					
					//g_autobaud_tick[1] = SysTick->VAL;
					if(SysTick->CTRL & SysTick_CTRL_COUNTFLAG_Msk)
					{
						systick_reload_flag++;
					}
					
				}while((int_status & at_rxd_mask) == 0 && (systick_reload_flag < 2));
				for(softdelay=0;softdelay<10;softdelay++);

				//second falling edge
				if(systick_reload_flag < 2)
				{
					systick_reload_flag = SysTick->CTRL;//clear flag
					systick_reload_flag = 0;

					do{
						int_status = HWREG(GPIO_BASE + GPIO_DIN0);
						
						g_autobaud_tick[1] = SysTick->VAL;
						if(SysTick->CTRL & SysTick_CTRL_COUNTFLAG_Msk)
						{
							systick_reload_flag++;
						}
						
					}while((int_status & at_rxd_mask) != 0 && (systick_reload_flag < 2));
					for(softdelay=0;softdelay<10;softdelay++);

					if(systick_reload_flag < 2)
					{
						g_autobaud_state = AUTOBAUD_STATE_WAITINT_THIRD;
					}
					else
					{
						g_autobaud_state = AUTOBAUD_STATE_WAITINT_FIRST;
					}
				}
				else
				{
					g_autobaud_state = AUTOBAUD_STATE_WAITINT_FIRST;	
				}
				
				break;
			}

			case AUTOBAUD_STATE_WAITINT_THIRD:
			{
				systick_reload_flag = SysTick->CTRL;//clear flag
				systick_reload_flag = 0;
				
				do{
					int_status = HWREG(GPIO_BASE + GPIO_DIN0);
					
					if(SysTick->CTRL & SysTick_CTRL_COUNTFLAG_Msk)
					{
						systick_reload_flag++;
					}
					
				}while((int_status & at_rxd_mask) == 0 && (systick_reload_flag < 2));
				for(softdelay=0;softdelay<10;softdelay++);
				
				//third falling edge
				if(systick_reload_flag < 2)
				{
					systick_reload_flag = SysTick->CTRL;//clear flag
					systick_reload_flag = 0;

					do{
						int_status = HWREG(GPIO_BASE + GPIO_DIN0);
						
						g_autobaud_tick[2] = SysTick->VAL;
						if(SysTick->CTRL & SysTick_CTRL_COUNTFLAG_Msk)
						{
							systick_reload_flag++;
						}
						
					}while((int_status & at_rxd_mask) != 0 && (systick_reload_flag < 2));

					if(systick_reload_flag < 2)
					{
						g_autobaud_state = AUTOBAUD_STATE_VERIFY;
					}
					else
					{
						g_autobaud_state = AUTOBAUD_STATE_WAITINT_FIRST;
					}

				}
				else
				{
					g_autobaud_state = AUTOBAUD_STATE_WAITINT_FIRST;	
				}
				
				break;
			}

			case AUTOBAUD_STATE_VERIFY:
			{
				edge_triggle_cnt = 0;
				//first falling to second falling
				bits_len1 = (g_autobaud_tick[0] + SysTick_LOAD_RELOAD_Msk - g_autobaud_tick[1]) % SysTick_LOAD_RELOAD_Msk;
				bits_len2 = (g_autobaud_tick[0] + SysTick_LOAD_RELOAD_Msk - g_autobaud_tick[2]) % SysTick_LOAD_RELOAD_Msk;
				checkvalue = (bits_len2*10)/bits_len1;
				
				if( checkvalue>36 && checkvalue<44)
				{
					baudrate = XY_HCLK*8/bits_len2;
					char i;
					for(i=AUTOBAUD_SUPPORT_NUM-1;i >= 0;i--)
					{
						if( (baudrate>g_autobaud_table[(int)(i)]*8/10) && (baudrate<g_autobaud_table[(int)(i)]*12/10) )
						{
							baudrate = g_autobaud_table[(int)(i)];
							break;
						}
							
					}
					
					if(i >= 0)
					{						
						systick_reload_flag = SysTick->CTRL;//clear flag
						systick_reload_flag = 0;

						rxdpin_last = HWREG(GPIO_BASE + GPIO_DIN0) & at_rxd_mask;

						do{
							rxdpin_current = HWREG(GPIO_BASE + GPIO_DIN0) & at_rxd_mask;
							if(rxdpin_last >0 && rxdpin_current ==0)
								edge_triggle_cnt++;
							
							if(SysTick->CTRL & SysTick_CTRL_COUNTFLAG_Msk)
							{
								systick_reload_flag++;
							}
							rxdpin_last = rxdpin_current;
							for(softdelay=0;softdelay<10;softdelay++);
							
						}while(edge_triggle_cnt < 10 && systick_reload_flag < 2);

						

						if(systick_reload_flag < 2)
						{
							while( (HWREG(GPIO_BASE + GPIO_DIN0) & at_rxd_mask) == 0);//wait till io high,idle
							HWREG(GPIO_BASE + GPIO_CTRL0)		|=	at_rxd_mask;//peripheral mode
							g_autobaud_state = AUTOBAUD_STATE_DONE;
							
							
						}
						else
						{
							g_autobaud_state = AUTOBAUD_STATE_INIT;	
						}
						
					}
					else
					{
						g_autobaud_state = AUTOBAUD_STATE_WAITINT_FIRST;
					}
					
				}
				else
				{
					g_autobaud_state = AUTOBAUD_STATE_WAITINT_FIRST;
				}
				break;
			}
				
			case AUTOBAUD_STATE_DONE:
				break;
			default:
				break;
		}

		if(g_autobaud_state == AUTOBAUD_STATE_DONE)
			break;//break while
			
	}

	HWREG(AT_UART_CSP + CSP_MODE1) 		&= ~CSP_MODE1_CSP_EN;
	CSPUARTModeSet(AT_UART_CSP,XY_PCLK,baudrate,8,1);
	
	char rsp[] = "\r\nOK\r\n";
	
	HWREG(AT_UART_CSP+CSP_INT_STATUS) = CSP_INT_ALL;
	for(unsigned char i = 0 ; i < strlen(rsp) ; i++ )
	{
		CSPCharPut( AT_UART_CSP, *((char *)rsp + i));
	}
	while((HWREG(AT_UART_CSP+CSP_INT_STATUS)&CSP_INT_CSP_TX_ALLOUT) == 0);
	HWREG(AT_UART_CSP+CSP_INT_STATUS) = CSP_INT_CSP_TX_ALLOUT;
	//HWREG(AT_UART_CSP + CSP_INT_STATUS) = CSP_INT_RX_DONE;
	
	NVIC_ClearPendingIRQ(GPIO_IRQn);

	if(baudrate > 9600)
		g_autobaud_stop_standby = 1;
	
	g_autobaud_rate = baudrate;	
	
	g_softap_fac_nv->uart_rate = (g_autobaud_rate/2400)<<9;

	SAVE_FAC_PARAM(uart_rate);
}

extern void soft_sample_fill_char(char *buf, int *data_len);
extern void soft_sample_print_wakeup_reason(void);

void AT_uart_rx_task(void)
{
    char aucATBuffer[100] = {0};
    int len = 0;

    while(1)
    {
		//memset(aucATBuffer,0,sizeof(aucATBuffer));
		len = at_uart_read(aucATBuffer,sizeof(aucATBuffer));
		CSPIntEnable(AT_UART_CSP,CSP_INT_RX_DONE);
		
		// bug 3706, standby wakeup response two consecutive 8003
		soft_sample_fill_char(aucATBuffer, &len);

		if((len >= 0) )//|| (farps_handle_once_flag == 1))
		{
			xy_assert(len <= 100);
            {
                at_recv_from_uart(aucATBuffer,len,FARPS_UART_FD);
				soft_sample_print_wakeup_reason();
			}
		}
    }
}


uint32_t creat_AT_uart_rx_task()
{
    uint32_t uwRet = pdFAIL;
	
	osThreadNew ((osThreadFunc_t)(AT_uart_rx_task),NULL,"AT_uart_rx",0x480,osPriorityNormal3);
	
    return uwRet;
}
