#if XY_FOTA && BLE_ENABLE
#include <ctype.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <lwip/netdb.h>

#include "xy_utils.h"
#include "xy_fota.h"
#include "dma.h"
#include "osal.h"
#include "flash_adapt.h"
#include "xy_flash.h"
#if BLE_ENABLE
#include "ble_fota.h"
#endif

/**
* @brief message queue for fota demo
*/
typedef struct _ble_fota_msg {
    int     last_flag;  ///< last fota package flag;0:not,1:yes
    int     data_len;	///< fota package len
    uint8_t    *data;	///< fota package data
}ble_fota_msg;


#define LOCAL_FOTA_TIMEOUT (96 * 1000)
#define LOCAL_FOTA_DRAM_START DSP_HEAP_DRAM_START


osThreadId_t g_xy_local_fota_Handle = NULL;
osMessageQueueId_t local_fota_demo_q = NULL;
ota_context_t *g_ble_fota_context = NULL;



int xy_local_fota_hook(u8_t *data, int size, int last_flag)
{
	ble_fota_msg *msg = NULL;
    
    msg = xy_zalloc(sizeof(ble_fota_msg));

    if(last_flag == 1)
    {
        msg->last_flag = 1;
        /*if(testnum)
        {
        	testnum = 0;
        	*data = ~(*data);
        }*/
		msg->data =0;
    	msg->data_len = 0;
    }
    else
	{
        msg->last_flag = 0;
		msg->data = xy_malloc(size);
    	memcpy(msg->data, data, size);
    	msg->data_len = size;
	}
    
	osMessageQueuePut(local_fota_demo_q, &msg, 0, osWaitForever);

	return 0;
}

//suds
void xy_local_fota_task(void)
{
    int last_flag = 0;
    ble_fota_msg *msg = NULL;
    unsigned char *buf = NULL;
    int tmp = 0;

    volatile unsigned int buf_data_size = 0;
    buf = xy_zalloc(1024*4);
    //memset(buf, 0x00, 64);
    //Get the subcontract data written to Flash
    while(!last_flag)
    {
    	if(osMessageQueueGet(local_fota_demo_q, &msg, NULL, LOCAL_FOTA_TIMEOUT) != osOK)
        {
            softap_printf(USER_LOG, WARN_LOG, "[king][xy_locat_fota_task] get data timeout!");
            ble_fota_transmit_not_done();
            goto end;
        }
		//0x3ffc0000

		DMAChannelTransfer_withMutex((unsigned long)(LOCAL_FOTA_DRAM_START+buf_data_size), (unsigned long)(msg->data), (unsigned long)(msg->data_len), (unsigned long)(FLASH_DMA_CHANNEL));
		buf_data_size+=msg->data_len;
		if(1 == msg->last_flag)
			last_flag = 1;

/*
		if((buf_data_size + msg->data_len) >= 512)
		{
			ota_update_Hmac_by_packet(buf, buf_data_size);
			ota_write_packet_to_flash(buf, buf_data_size);

			ota_update_Hmac_by_packet(msg->data,  msg->data_len);
			ota_write_packet_to_flash(msg->data,  msg->data_len);
			buf_data_size = 0;
		}
		else
		{
			memcpy(buf+buf_data_size,msg->data,msg->data_len);
			buf_data_size+=msg->data_len;
		}


		
		if(1 == msg->last_flag && buf_data_size)
		{
			ota_update_Hmac_by_packet(buf, buf_data_size);
			ota_write_packet_to_flash(buf, buf_data_size);
			buf_data_size = 0;
		}
*/
        if(msg != NULL)
        {  
            if(msg->data != NULL)
                xy_free(msg->data);

            xy_free(msg);
            msg = NULL;
        }

    }
	while(buf_data_size)
	{

		DMAChannelTransfer_withMutex( (unsigned long)(buf),(unsigned long)(LOCAL_FOTA_DRAM_START+tmp),1024*4, (unsigned long)(FLASH_DMA_CHANNEL));
		if(buf_data_size>=FLASH_SECTOR_LENGTH)
		{
			if(ota_downlink_packet_proc(g_ble_fota_context, (char *)(buf), 1024*4))
                goto end;
			tmp+=1024*4;
			buf_data_size-=FLASH_SECTOR_LENGTH;
		}
		else
		{
            if(ota_downlink_packet_proc(g_ble_fota_context, (char *)(buf), buf_data_size))
                goto end;
						
			buf_data_size=0;
		}
	}

	g_xy_local_fota_Handle = (void *)(-1);
	xy_free(buf);
    osThreadExit();

end: 
    //fota resources deinit
    ota_update_deInit(g_ble_fota_context);
    g_xy_local_fota_Handle = NULL;
	xy_free(buf);
    osThreadExit();
}


char xy_local_fota_init(void)
{
	//fota resources init
	g_ble_fota_context = ota_update_Init();
	if(g_ble_fota_context == NULL)
	{
		softap_printf(USER_LOG, WARN_LOG, "[king][xy_locat_fota_task] init error!");
		return 0;
	}

	// init demo queue
	if(NULL == local_fota_demo_q)
		local_fota_demo_q = osMessageQueueNew(16, sizeof(void *));

    //Create xy_fota demo
	if(NULL == g_xy_local_fota_Handle)
		g_xy_local_fota_Handle = osThreadNew((osThreadFunc_t)(xy_local_fota_task), NULL, "xy_local_fota_task", 2048, XY_OS_PRIO_NORMAL1);

	return 1;
}


#endif


