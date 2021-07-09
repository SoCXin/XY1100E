/** 
* @file        cdp_opencpu_demo.c
* @ingroup     cloud
* @brief       使用API实现CDP与网络云平台交互的参考代码
* @attention  
* @note         !!!该源文件默认不编译，用户不得直接在该文件中修改！！！\n
*               而应该是自行创建新的私有的源文件，将demo示例代码拷贝过去后再做修改。
*/

#include "zos.h"


#include "xy_rtc_api.h"
#include "xy_utils.h"
#include "xy_demo.h"

/**
 * @brief 任务重试间隔时间
 */	
#define  WAIT_AGAIN_TIME              (5*1000)
/**
 * @brief 任务重试次数上限
 */	
#define  DO_AGAIN_TIMES               3

/**
 * @brief 简化demo的信号量
 */
zos_sem_t cdp_opencpu_demo_sem = NULL;
/**
 * @brief 简化demo的任务句柄
 */	
zos_task_t  g_cdp_opencpu_demo_Handle =NULL;
zos_uint8_t buf[100]="hello ZusOS";
char *str = ZOS_NULL;

/*下行数据的回调函数，平台下发的数据都会在此处理；
严禁用户在此回调函数中阻塞或耗时较长的处理数据，应当在获取到数据后在另一线程处理*/
void  cdp_demo_downstream_hook(char *data, int data_len)
{
	(void) data_len;
    memset(str,0,strlen(str));
    strncpy(str,data,data_len);
    zos_printf("data=%s",str);
    //if first data is 0xFF,That means there's nothing to do
    if(data[0] == (char)(0x01))
    {
        if(cdp_opencpu_demo_sem != NULL)
        	zos_sem_post(cdp_opencpu_demo_sem);
    }
    else
    {
        //Users can process the downlink data according to the actual demand
    }

    return;
}

/**
 *@brief 简化demo的任务处理流程
 *@note 简化demo内部实现了简单的云平台的注册和发送上行数据流程
 *@warning 简化demo并未执行去注册操作，同时接收下行数据时用户必须实现回调接口cdp_DownStream_Hook
 */	
void cdp_opencpu_demo_task(void *args)
{
    zos_uint8_t i=30,cgact;
    int again_num = 0;
    (void) args;



    if(cdp_opencpu_demo_sem == NULL)
    	cdp_opencpu_demo_sem = zos_sem_create(0xFFFF, 0);

	str = zos_malloc(50);
	zos_printf("nb power type:%d\r\n",nb_get_power_type());
	zos_printf("nb reset type:%d\r\n",nb_get_reset_type());
	nb_get_IMEI(str);
	zos_printf("IMEI = %s\r\n",str);

	nb_get_IMSI(str);
	zos_printf("IMSI = %s\r\n",str);

	nb_get_NCCID(str);
	zos_printf("ICCID = %s\r\n",str);

    while (i--)
    {
        if(nb_get_CGACT(&cgact) == ZOS_EOK)
		{
			zos_printf("cgact=%d\r\n",cgact);
			if(cgact == 1)
			{
				break;
			}
		}
		zos_task_delay(1000);
    }
    if(i>0)
	{
        zos_printf("Module networking successful\r\n");
    }
    zos_printf("cdp opencpu demo start");
    //设置云平台的IP和PORT，云平台的IP和PORT会存储到NV中
    //CTWing:221.229.214.202;CDP:49.4.85.232
    if(cdp_cloud_setting("221.229.214.202", 5683) != ZOS_EOK)
    {
       ZOS_ASSERT(0);
    }

    //执行云平台注册流程
    while(cdp_register(86400, 30)  != ZOS_EOK)
    { 
        again_num++;
        if(again_num > DO_AGAIN_TIMES)
        {
            zos_printf("register fail!!!");
            goto task_exit;
        }
        zos_task_delay(WAIT_AGAIN_TIME);
    }
    zos_printf("register success!!!\r\n");
    //注册成功后，发送上行数据
    again_num = 0;

    while(1)
    {
        if(cdp_send_syn(buf, strlen(buf), 0) != ZOS_EOK)
        {
            again_num++;
            zos_printf("cdp_senddata fail!!!");
            if(again_num > DO_AGAIN_TIMES)
            {
                zos_printf("cdp_senddata fail!!!");
                goto task_exit;
            }
        }

        zos_task_delay(WAIT_AGAIN_TIME);
    }

    
    //如果有云平台下发的下行数据，用户可以在回调函数中进行处理
    //阻塞等待云平台的下行数据
    if(cdp_opencpu_demo_sem == NULL)
    	cdp_opencpu_demo_sem = zos_sem_create(0xFFFF, 0);
    if(zos_sem_wait(cdp_opencpu_demo_sem, 180000) != ZOS_EOK)
    {
        zos_printf("wait for over data, timeout!");
        goto task_exit;
    }

    zos_printf("cdp opencpu demo end!");

task_exit:
	osThreadExit();
}

/**
 * @brief 简化demo的任务初始化
 */	
void cdp_opencpu_demo_init()
{
    //注册下行数据的回调函数
    cdp_callbak_set(cdp_demo_downstream_hook, ZOS_NULL);

    //创建CDP的demo线程
	g_cdp_opencpu_demo_Handle = zos_task_create ("cdp_opencpu_demo",cdp_opencpu_demo_task,ZOS_NULL,0x400,task_Priority_Normal);
}


