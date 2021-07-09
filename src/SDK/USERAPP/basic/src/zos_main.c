/** 
* @file        zos_main.c
* @ingroup     zos_main
* @brief       主入口，用户在对应的接口中进行外设和私有应用任务的初始化，但不得长时间阻塞！
*/
/**
 * @defgroup zos main
 */




#include "zos.h"

//任务全局变量
zos_task_t user_main_task_Handle = NULL;


#define USER_MAIN_STACK_SIZE       1024

void user_main_task(void *parameter)
{
	zos_uint8_t i=20;
	zos_uint8_t rssi,cgact;
	struct nb_time_t time;
	nb_serving_cell_info_t rcv_servingcell_info;
	char *str = ZOS_NULL;
	str = zos_malloc(50);
	if(nb_get_powenon_from_deepsleep()==ZOS_EOK)
	{
		zos_printf("NB module starts from deepsleep\r\n");
	}
	else
	{
		zos_printf("NB module starts from power or reset\r\n");
	}
	
	zos_printf("nb power type:%d\r\n",nb_get_power_type());
	zos_printf("nb reset type:%d\r\n",nb_get_reset_type());
	nb_get_IMEI(str);
	zos_printf("IMEI = %s\r\n",str);

	nb_get_IMSI(str);
	zos_printf("IMSI = %s\r\n",str);

	nb_get_NCCID(str);
	zos_printf("ICCID = %s\r\n",str);
	//关闭深睡眠，如果想要深睡眠，后面直接调用nb_work_unlock() 就可以
	nb_work_lock();
	do{
		if(nb_get_csq(&rssi) == ZOS_EOK)
		{
			zos_printf("csq = %d\r\n",rssi);
			if(rssi<32)
			{
				break;
			}
		}
		zos_task_delay(1000);
	}while(i--);

	if(i>0)
	{
		do{
			if(nb_get_CGACT(&cgact) == ZOS_EOK)
			{
				zos_printf("cgact=%d\r\n",cgact);
				if(cgact == 1)
				{
					break;
				}
			}
			zos_task_delay(1000);
		}while(i--);
		zos_task_delay(1000);
		if(zos_rtc_get_time(&time) == ZOS_EOK)
		{
			zos_printf("date:%d-%d-%d time:%d-%d-%d\r\n",time.tm_year,time.tm_mon,time.tm_mday,time.tm_hour,time.tm_min,time.tm_sec);
		}
		nb_get_servingcell_info(&rcv_servingcell_info);
		zos_printf("cell info:%d,%d,%d,%d,%d,%d,%d\n",
        rcv_servingcell_info.EARFCN,
        rcv_servingcell_info.PCI,
        rcv_servingcell_info.CellID,
        rcv_servingcell_info.Signalpower,
        rcv_servingcell_info.RSRQ,
        rcv_servingcell_info.Totalpower,
        rcv_servingcell_info.SNR);
		zos_printf("Module networking successful\r\n");
		zos_printf("adc0= %d\r\n",zos_adc_read(ZOS_ADC_BUS_0));
		zos_printf("vbat= %d\r\n",zos_vbat_read());
		zos_printf("adc0= %d\r\n",zos_adc_read(ZOS_ADC_BUS_0));
	}
	else
	{
		zos_printf("Module networking failure\r\n");
	}
	i=0;
	while(1)
	{
		i++;
		zos_printf("i=%d\r\n",i);
		if(i == 50)
		{
			nb_work_unlock();
			zos_task_delay(10000);
		}
		else
		{
			zos_task_delay(1000);
		}

	}
}


int zos_main(void)
{
  	user_main_task_Handle = zos_task_create ("user_main_task",user_main_task,ZOS_NULL,USER_MAIN_STACK_SIZE,task_Priority_Normal);
}
