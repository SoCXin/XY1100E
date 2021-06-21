/**
* @file        hal_rtc_demo.c
* @ingroup     peripheral
* @brief       本DEMO为用户使用RTC的DEMO
			   在这个DEMO中，我们将一个回调函数与USER1事件进行绑定，并设定在30s后
*              此回调函数会被触发。在注册回调函数间隔一秒后，读取了当前时间距离触发回调函数的时间。
*              因系统会自动使能RTC，所以不需要用户进行RTC的配置。
********************************************************************************************************************/
#if DEMO_TEST && MOBILE_VER

#include "xy_api.h"

#define RTC_TIMER_SEND_OFFSET   90

#define RTC_TIMER_GATHER_OFFSET 60
	
#define CIS_SERVER_IP			"183.230.40.40"
#define CIS_SERVER_PORT			5683

osSemaphoreId_t rtc_uart_sem = NULL;

HAL_CSP_HandleTypeDef csp_uart_handle;

int  g_do_send = 0;
int  g_do_gather = 0;

struct THdata
{
	uint32_t address;
	uint8_t length;
};

static void send_rtc_timeout_callback(void *para)
{
	(void) para;
	//用户可在这个函数内部添处理流程。
	xy_printf("[demo]send_rtc_timeout_callback\n");
	//设置下次唤醒点
	xy_rtc_timer_create(RTC_TIMER_USER2, RTC_TIMER_SEND_OFFSET, send_rtc_timeout_callback, NULL);

	if(rtc_uart_sem == NULL)
	{
		rtc_uart_sem = osSemaphoreNew(0xFFFF, 0);
	}
	g_do_send = 1;
	osSemaphoreRelease(rtc_uart_sem);
}

static void do_send_data()
{
	uint32_t flashAddr = USER_FLASH_BASE;
	struct THdata* pTHdata = NULL;
	uint8_t *data;
	
	//用户可在这个函数内部添处理流程。
	xy_printf("[demo]start send data!\n");
	
	xy_work_lock(0);
	//设置下次唤醒点
	xy_rtc_timer_create(RTC_TIMER_USER2, RTC_TIMER_SEND_OFFSET, send_rtc_timeout_callback, NULL);

	cis_cloud_setting(CIS_SERVER_IP, CIS_SERVER_PORT, NULL);

    xy_printf("[demo]start REGISTER");
    if(cis_ext_reg(30))
    {
        xy_printf("[demo]REGISTER failed");
        goto FAIL_PROC;
    }	

	pTHdata = (struct THdata*)flashAddr;
	xy_printf("[demo]send len:%d",pTHdata->length);
	if(pTHdata->length != 0)
	{
		data = xy_malloc(pTHdata->length);
		
		//从flash中读取保存数据的首地址和已保存的长度
		xy_flash_read(flashAddr + 0x20, data, pTHdata->length);
		xy_printf("[demo]send:%s",data);
		
	    if(cis_ext_send((char*)data,pTHdata->length, 30))
	    {
	        xy_printf("[demo]senddata failed");
	        goto FAIL_PROC;
	    }
		xy_flash_erase(flashAddr,0X1000);
		xy_free(data);
	}

FAIL_PROC:

    cis_ext_dereg();

    xy_printf("[demo]send task end,goto deep sleep\n");	
	
	xy_work_unlock(0);
}

static void gather_rtc_timeout_callback(void *para)
{
	(void) para;
	//用户可在这个函数内部添处理流程。
	xy_printf("[demo]gather_rtc_timeout_callback\n");
	//设置下次唤醒点
	xy_rtc_timer_create(RTC_TIMER_USER1, RTC_TIMER_GATHER_OFFSET, gather_rtc_timeout_callback, NULL);
	
	if(rtc_uart_sem == NULL)
	{
		rtc_uart_sem = osSemaphoreNew(0xFFFF, 0);
	}
	g_do_gather = 1;
	osSemaphoreRelease(rtc_uart_sem);
}
void hal_csp_uart_demo_init(void)
{
	HAL_GPIO_InitTypeDef hal_csp_uart_gpio_init;

	//映射UART的TX引脚
	hal_csp_uart_gpio_init.Pin		= HAL_GPIO_PIN_NUM_6;
	hal_csp_uart_gpio_init.Mode		= GPIO_MODE_AF_PER;
	hal_csp_uart_gpio_init.Pull		= GPIO_NOPULL;
	hal_csp_uart_gpio_init.Alternate	= HAL_REMAP_CSP4_TXD;
	HAL_GPIO_Init(&hal_csp_uart_gpio_init);

	//映射UART的RX引脚
	hal_csp_uart_gpio_init.Pin		= HAL_GPIO_PIN_NUM_7;
	hal_csp_uart_gpio_init.Mode 		= GPIO_MODE_AF_INPUT;
	hal_csp_uart_gpio_init.Pull		= GPIO_PULLUP;
	hal_csp_uart_gpio_init.Alternate 	= HAL_REMAP_CSP4_RXD;
	HAL_GPIO_Init(&hal_csp_uart_gpio_init);

	//初始化CSP为UART
	csp_uart_handle.Instance				= HAL_CSP4;
	csp_uart_handle.Init.UART_BaudRate	= 9600;
	csp_uart_handle.Init.UART_DataBits	= HAL_CSP_DATA_BITS_8;
	csp_uart_handle.Init.UART_StopBits	= HAL_CSP_STOP_BITS_1;
	HAL_CSP_UART_Init(&csp_uart_handle);
	
	//使能CSP
	HAL_CSP_Ctrl(&csp_uart_handle, HAL_ENABLE);
}


/**
* @brief	这个demo使用RTC进行定时，并注册了一个回调函数，这个回调函数在注册语句执行的30秒后被调用
*/
void RTC_Event_Demo_Loop(void)
{
	uint8_t ReadData[32] = {0};//3~7为温度，共5个byte,格式为xx.xx;17~20为湿度，共4个byte,格式为xx.x
	uint8_t writeBuf[32] = {0};
	uint8_t readBuf[32] = {0};
	uint32_t flashAddr = USER_FLASH_BASE;
	struct THdata* pTHdata = NULL;

	int ret;


	xy_assert(g_softap_fac_nv->deepsleep_enable==1 && g_softap_fac_nv->work_mode==0);

	hal_csp_uart_demo_init();

	while(1)
	{		
		//wait user RTC timeout,maybe happen after PS RTC wakeup
		if(rtc_uart_sem != NULL)
		{	
			ret = osSemaphoreAcquire(rtc_uart_sem, osWaitForever);

			//user RTC timeout
			if(ret==osOK && g_do_gather==1)
			{
				xy_printf("[demo]gather data start\n");	
				
				xy_work_lock(0);
				
				//数据采集，保持到flash
				//第一次发送AT只是为了唤醒温湿度传感器，收到的数据丢弃。如果直接发送查询温湿度AT命令，会丢失数据。
				HAL_CSP_Transmit(&csp_uart_handle, (uint8_t *)"AT\r\n", 4, 500);
				HAL_CSP_Receive(&csp_uart_handle, ReadData, 4, 500);
				xy_printf("[demo]ReadData:%s\n", ReadData);

				HAL_CSP_Transmit(&csp_uart_handle, (uint8_t *)"AT+T\r\n", 6, 500);
				ret = HAL_CSP_Receive(&csp_uart_handle, ReadData, 14, 500);
				xy_assert(ret == HAL_OK);
				xy_printf("[demo]ReadData:%s\n", ReadData);

				HAL_CSP_Transmit(&csp_uart_handle, (uint8_t *)"AT+H\r\n", 6, 500);
				ret = HAL_CSP_Receive(&csp_uart_handle, ReadData + 14, 13, 500);
				xy_assert(ret == HAL_OK);
				xy_printf("[demo]ReadData:%s\n", ReadData+14);

				//从flash中读取保存数据的首地址和已保存的长度
				xy_flash_read(flashAddr, readBuf, 32);

				pTHdata = (struct THdata*)readBuf;
				if(pTHdata->address != (flashAddr + 0x20))
				{
					pTHdata->address = flashAddr + 0x20;
					pTHdata->length = 0;
				}

				//将数据写到flash中
				memcpy(writeBuf, ReadData, 32);
				xy_flash_write((uint32_t)pTHdata->address + pTHdata->length, writeBuf, 32);

				//更新已写长度保存到flash
				pTHdata->length += 32;
				xy_flash_write(flashAddr, readBuf, 32);

				//将写到flash的数据读出来，验证
				xy_flash_read(flashAddr, readBuf, 32);
				pTHdata = (struct THdata*)readBuf;

				xy_flash_read(((uint32_t)pTHdata->address + pTHdata->length) - 32, readBuf, 32);
				xy_printf("[demo]check flash data:%s\n", readBuf);

				xy_printf("[demo]gather task end,goto deep sleep\n");	
							
				xy_work_unlock(0);
			}
			else if(ret==osOK && g_do_send==1)
			{
				do_send_data();
			}
			else
				xy_assert(0);
		}
		//first POWENON,only set next user RTC,not use NB PS
		else
		{
			xy_work_lock(0);	
			
			xy_printf("[demo]not gather RTC wakeup\n");
			rtc_uart_sem = osSemaphoreNew(0xFFFF, 0);	
			//wakeup by other cause,such as PS RTC/PIN
			if(xy_rtc_next_offset_by_ID(RTC_TIMER_USER2))
			{
				xy_printf("[demo]send RTC have setted\n");	
			}
			else
				xy_rtc_timer_create(RTC_TIMER_USER2, RTC_TIMER_SEND_OFFSET, send_rtc_timeout_callback, NULL);

			if(xy_rtc_next_offset_by_ID(RTC_TIMER_USER1))
			{
				xy_printf("[demo]gather RTC have setted\n");	
				continue;
			}
			
			xy_printf("[demo]first set gather RTC,goto deepsleep!\n");

			//for to reduce BSS stress,and improve communication success rate,user task must set rand UTC,see  xy_rtc_set_by_day
			xy_rtc_timer_create(RTC_TIMER_USER1, RTC_TIMER_GATHER_OFFSET, gather_rtc_timeout_callback, NULL);
							
			xy_work_unlock(0);
		}
	}

}

void rtc_uart_demo_init()
{
	osThreadId_t task_Handle =NULL;
	task_Handle = osThreadNew ((osThreadFunc_t)(RTC_Event_Demo_Loop),NULL,"rtc_csp",0x400,osPriorityNormal);
	xy_assert(task_Handle != NULL);

}

#endif
