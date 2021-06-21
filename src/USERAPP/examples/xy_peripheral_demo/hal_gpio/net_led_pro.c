/**
* @file        net_led_pro.c
* @ingroup     peripheral
* @brief       LED指示灯，指示当前驻网情况
* @attention   1，由于硬件上有些限制，如果要使用，请咨询我们的硬件FAE;
*              2，目前默认未使用该源文件;
*              3，如用户使用该源文件，需要修改NV参数g_softap_fac_nv->peri_remap_enable & 0x0100为0时，用户方可自行控制LED灯
* @par	点灯规则：
* @par							亮                        灭
* @par			上电默认注网：	   64ms    800ms
* @par			注网成功		   64ms    2000ms
* @par			idle		   0ms	   ~ms
* @par			PSM			   0ms	   ~ms
* @par			PDP deact      64ms    800ms
* @par			PDP act		   64ms    2000ms
***********************************************************************************/

#include "xy_api.h"

struct led_msg {
	int		msg_id;
};

osTimerId_t timer1 = NULL;
osTimerId_t timer2 = NULL;
osThreadId_t g_user_led_Handle = NULL;
osMessageQueueId_t at_led_q = NULL;

void callback1(void)
{
	if((g_softap_fac_nv != NULL) && (!(g_softap_fac_nv->peri_remap_enable & 0x0100)) && (g_softap_fac_nv->led_pin <= HAL_GPIO_PIN_NUM_63))
	{
		//LED SetOFF
		HAL_GPIO_WritePin(g_softap_fac_nv->led_pin,HAL_GPIO_PIN_RESET);
	}
	osTimerStart(timer2, 0);
}

void callback2(void)
{
	if((g_softap_fac_nv != NULL) && (!(g_softap_fac_nv->peri_remap_enable & 0x0100)) && (g_softap_fac_nv->led_pin <= HAL_GPIO_PIN_NUM_63))
	{
		//LED SetON
		HAL_GPIO_WritePin(g_softap_fac_nv->led_pin,HAL_GPIO_PIN_SET);
	}
	osTimerStart(timer1, 0);
}

void net_light_led(uint32_t timer1_time, uint32_t timer2_time)
{
	if((g_softap_fac_nv != NULL) && (!(g_softap_fac_nv->peri_remap_enable & 0x0100)) && (g_softap_fac_nv->led_pin <= HAL_GPIO_PIN_NUM_63))
	{
		//LED SetOFF
		HAL_GPIO_WritePin(g_softap_fac_nv->led_pin,HAL_GPIO_PIN_RESET);
	}

	//xy_assert(timer1 == NULL && timer2 == NULL);
	if(timer1 == NULL)
	{
		timer1 = osTimerNew((osTimerFunc_t)(callback1), osTimerOnce, NULL, "led_tmr1");
		xy_assert(timer1 != NULL);
		//timer maybe delay
		osTimerSetLowPowerFlag(timer1, osLowPowerProhibit);
		osTimerStart(timer1, timer1_time);
	}
	else
	{
		osTimerStart(timer1, timer1_time);
	}

	if(timer2 == NULL)
	{
		timer2 = osTimerNew((osTimerFunc_t)(callback2), osTimerOnce, NULL, "led_tmr2");
		xy_assert(timer2 != NULL);
		//timer maybe delay
		osTimerSetLowPowerFlag(timer2, osLowPowerProhibit);
		osTimerStart(timer2, timer2_time);
		osTimerStop(timer2);
	}
	else
	{
		osTimerStart(timer2, timer2_time);
		osTimerStop(timer2);
	}
//	xy_assert(XY_OK == LOS_SwtmrCreate(timer1_time, TIMER_AUTO_RELOAD, (SWTMR_PROC_FUNC)callback1, (uint32_t*)&timer1, NULL, OS_SWTMR_ROUSES_IGNORE, OS_SWTMR_ALIGN_SENSITIVE));
//	xy_assert(XY_OK == LOS_SwtmrCreate(timer2_time, TIMER_AUTO_RELOAD, (SWTMR_PROC_FUNC)callback2, (uint32_t*)&timer2, NULL, OS_SWTMR_ROUSES_IGNORE, OS_SWTMR_ALIGN_SENSITIVE));

}

void deinit(void)
{
	if((g_softap_fac_nv != NULL) && (!(g_softap_fac_nv->peri_remap_enable & 0x0100)) && (g_softap_fac_nv->led_pin <= HAL_GPIO_PIN_NUM_63))
	{
		//LED SetOFF
		HAL_GPIO_WritePin(g_softap_fac_nv->led_pin,HAL_GPIO_PIN_RESET);
	}
	
	xy_assert(timer1 != NULL && timer2 != NULL);

	if(timer1 != NULL)
	{
		osTimerDelete(timer1);
		timer1 = NULL;
	}

	if(timer2 != NULL)
	{
		osTimerDelete(timer2);
		timer2 = NULL;
	}	
}


int led_pro_fun(int net_ok)
{
	xy_printf("led net is %d\r\n",net_ok);
	if(net_ok == 2)
	{
		deinit();
		return 0;
	}
	
	if(net_ok == 1)
		net_light_led(64, 2000);
	else
		net_light_led(64, 800);
	return 0;
}

int write_led_pro(int msg_type)
{
	struct led_msg *msg =NULL;
	
	if(g_user_led_Handle == NULL)
		return 1;
	
	msg = xy_zalloc(sizeof(struct led_msg));
	msg->msg_id = msg_type;

	if(at_led_q == NULL)
		at_led_q = osMessageQueueNew(20, sizeof(void *));
	xy_assert(at_led_q != NULL);

	osMessageQueuePut(at_led_q, &msg, 0, osWaitForever);
	return 1;
	
}
void user_led_demo()
{
	struct led_msg *rcv_msg = NULL;
	
	while(1)
	{
		osMessageQueueGet(at_led_q, &rcv_msg, NULL, osWaitForever);
		if(rcv_msg == NULL)
		{
            continue;
		}

		switch(rcv_msg->msg_id)
		{
			case XY_PS_BEARER_OOS:
			case XY_PS_BEARER_DEACT:
				led_pro_fun(0);
				break;
			case XY_PS_BEARER_IS:
			case XY_PS_BEARER_ACT:
				led_pro_fun(1);
				break;
			case XY_PS_BEARER_IDLE:
				led_pro_fun(2);
				break;
			default:
				break;
		}

		xy_free(rcv_msg);		
	}
}

int at_CGEV_led_info(char *at_buf)
{
	char *at_paras = NULL;

	if ((at_paras = strstr(at_buf, "PDN ACT")) != NULL)
	{
		write_led_pro(XY_PS_BEARER_ACT);
	}
	else if ((at_paras = strstr(at_buf, "PDN DEACT")) != NULL)
	{
		write_led_pro(XY_PS_BEARER_DEACT);
	}
	else if ((at_paras = strstr(at_buf, "OOS")) != NULL)
	{
		write_led_pro(XY_PS_BEARER_OOS);
	}
	else if ((at_paras = strstr(at_buf, "IS")) != NULL)
	{
		write_led_pro(XY_PS_BEARER_IS);
	}
	return AT_END;
}

int at_CSCON_led_info(char *at_buf)
{
	int con_state = -1;

	if(strstr(at_buf,",") || strstr(at_buf,"-") || strstr(at_buf,")"))
		return AT_END;
	
	if (at_parse_param("%d", at_buf,&con_state) != AT_OK)
	{
		xy_printf("CSCON erro");
		return AT_END;
	}
	xy_printf("CSCON %d",con_state);
	if(con_state == 1)
		write_led_pro(XY_PS_BEARER_ACT);
	else if(con_state == 0)
		write_led_pro(XY_PS_BEARER_IDLE);
	return AT_END;
}


/**
 * @brief 用户任务初始化函数，在user_task_init中添加
 * @attention   
 */	
void user_led_demo_init(void)
{
	HAL_GPIO_InitTypeDef gpio_init;

	if(at_led_q != NULL)
		return;
	if (g_softap_fac_nv->peri_remap_enable & 0x0100)
	{
		xy_printf("peri_remap_enable  ERROR!!!");
		return ;
	}
	
	if((g_softap_fac_nv != NULL) && (g_softap_fac_nv->led_pin <= HAL_GPIO_PIN_NUM_63))
	{
		gpio_init.Pin		= (g_softap_fac_nv->led_pin);
		gpio_init.Mode		= GPIO_MODE_OUTPUT_PP;
		gpio_init.Pull		= GPIO_NOPULL;
		HAL_GPIO_Init(&gpio_init);
	}
	register_app_at_urc("+CGEV:",at_CGEV_led_info);
	register_app_at_urc("+CSCON:", at_CSCON_led_info);

	if(at_led_q == NULL)
		// xy_QueueCreate(&at_led_q, 20, sizeof(void *));
		at_led_q = osMessageQueueNew(20, sizeof(void *));
	
	g_user_led_Handle = osThreadNew ((osThreadFunc_t)(user_led_demo),NULL,"user_led_demo",0x400,osPriorityNormal);
	xy_printf("user_led_demo creat");

	if((g_softap_fac_nv != NULL) && (!(g_softap_fac_nv->peri_remap_enable & 0x0100)) && (g_softap_fac_nv->led_pin <= HAL_GPIO_PIN_NUM_63))
		write_led_pro(XY_PS_BEARER_DEACT);
}


