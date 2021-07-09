/**
 * @file user_ps_demo.c
 * @brief 用户PS操作处理DEMO
 * 用户需调用芯翼提供的xy_Reg_Ps_Event接口将需要监听的PS事件及回调的函数注册到芯翼内部
 * 当所监听的事件发生变化时，芯翼内部会调用用户提供的回调函数体
 * 如果在回调中有阻塞的操作，用户需创建额外的线程进行处理，在回调中将PS信息传递给该线程处理
 * 如果在回调中无阻塞操作，可以不用创建线程，直接在回调中完成。
 * 回调函数中不同的子状态，如pdp激活/去激活状态，用户可参考xy_ps_ext_api.h中pdp/sim/netif状态定义
 * @version 1.0
 * @date 2021-04-14
 * @copyright Copyright (c) 2021  芯翼信息科技有限公司
 * 
 */
#include "xy_api.h"

#if DEMO_TEST

typedef enum USER_PS_TYPE
{
    PS_PDP = 0,
    PS_SIM,
    PS_NETIF,
} user_ps_type_e;

//用户自定义ps消息结构体，用于将数据从PS回调函数中传递给用户线程
typedef struct user_ps_info
{
    union
    {
        xy_pdp_state_t pdp_info;
        xy_sim_state_t sim_info;
        xy_netif_info_t netif_info;
    };
    user_ps_type_e ps_type;

} user_ps_msg_t;

osMessageQueueId_t user_ps_q = NULL;

/**
 * 用户在调用芯翼提供的ps操作接口，需要根据接口的返回结果进行处理
 * 例如xy_get_PDP_APN接口，需要判断返回值是否为XY_OK/XY_ERR 进而做不同的处理
 * 返回值可以参考xy_ps_api.h中接口说明
 */
void user_get_apn_info(int query_cid)
{
    char *apn_buffer = xy_zalloc(101);
    if (xy_get_PDP_APN(apn_buffer, 101, query_cid) != XY_OK)
    {
        xy_printf("[ps demo]user get apn info fail");
    }
    else
    {
        xy_printf("[ps demo]user get apn info success:%s", apn_buffer);
    }
    xy_free(apn_buffer);
}

/**
 * 用户在调用芯翼提供的ps操作接口，需要根据接口的返回结果进行处理
 * 例如xy_get_CELLID接口，需要判断返回值是否为0 进而做不同的处理
 * 返回值可以参考xy_ps_api.h中接口说明
 */
void user_get_cellid(void)
{
    int cellid = -1;
	xy_get_CELLID(&cellid);
    if (cellid == -1)
    {
        xy_printf("[ps demo]user get cellid fail");
    }
    else
    {
        xy_printf("[ps demo]user get cellid success:%d", cellid);
    }
    
}

//发送自定义消息到用户线程
void send_ps_msg_to_user_task(user_ps_msg_t * msg)
{
    xy_assert(msg != NULL);
    if (user_ps_q != NULL)
        osMessageQueuePut(user_ps_q, &msg, 0, osWaitForever);
}

/**
* pdp状态变化回调函数体demo
* 该回调由芯翼内部调用，函数由用户实现，调用xy_Reg_Ps_Event接口注册到芯翼内部
* 如果在回调中有阻塞的操作，用户需创建额外的线程进行处理，在回调中将PS信息传递给该线程处理
* 如果在回调中无阻塞操作，可以不用创建线程，直接在回调中完成。
*/
int do_ps_pdp_state_changed(xy_ps_event_param_t* event)
{
    xy_assert(event != NULL);
    xy_pdp_state_t *user_pdp_state = (xy_pdp_state_t *)event->data;
    xy_assert(user_pdp_state != NULL);
    switch (user_pdp_state->state)
	{
    case XY_PS_BEARER_DEACT: 
    {
        //DEMO示例：通知用户线程处理
        xy_printf("[ps demo]ps state deact:%d", user_pdp_state->cid);
        user_ps_msg_t *msg = (user_ps_msg_t *)xy_zalloc(sizeof(user_ps_msg_t));
        msg->pdp_info.state = user_pdp_state->state;
        msg->pdp_info.cid = user_pdp_state->cid;
        msg->ps_type = PS_PDP;
        send_ps_msg_to_user_task(msg);
    }
    break;
    case XY_PS_BEARER_ACT:
    {
        //DEMO示例：通知用户线程处理
        xy_printf("[ps demo]ps state act:%d", user_pdp_state->cid);
        user_ps_msg_t *msg = (user_ps_msg_t *)xy_zalloc(sizeof(user_ps_msg_t));
        msg->pdp_info.state = user_pdp_state->state;
        msg->pdp_info.cid = user_pdp_state->cid;
        msg->ps_type = PS_PDP;
        send_ps_msg_to_user_task(msg);
    }
    break;
    case XY_PS_BEARER_OOS:
        //DEMO示例：不需要通知用户线程，回调里直接处理
        xy_printf("[ps demo]ps state oos:%d", user_pdp_state->cid);
        break;
    case XY_PS_BEARER_IS:
        //DEMO示例：不需要通知用户线程，回调里直接处理
        xy_printf("[ps demo]ps state is：%d", user_pdp_state->cid);
        break;
	default:
		break;
	}
    return XY_OK;
}

/**
* sim卡状态变化回调函数体demo
* 该回调由芯翼内部调用，函数由用户实现，调用xy_Reg_Ps_Event接口注册到芯翼内部
* 如果在回调中有阻塞的操作，用户需创建额外的线程进行处理，在回调中将PS信息传递给该线程处理
* 如果在回调中无阻塞操作，可以不用创建线程，直接在回调中完成。
*/
int do_ps_sim_state_changed(xy_ps_event_param_t* event)
{
    xy_assert(event != NULL);
    xy_sim_state_t* sim_state = (xy_sim_state_t *)event->data;
    switch (sim_state->state)
	{
	case XY_SIM_REMOVED:
        //DEMO示例：不需要通知用户线程，回调里直接处理
		xy_printf("[ps demo]sim removed");
		break;
    case XY_SIM_READY:
    {
        //DEMO示例：通知用户线程处理
        xy_printf("[ps demo]sim ready");
        user_ps_msg_t *msg = (user_ps_msg_t *)xy_zalloc(sizeof(user_ps_msg_t));
        msg->sim_info.state = sim_state->state;
        msg->ps_type = PS_SIM;
        send_ps_msg_to_user_task(msg);
    }
    break;
	default:
		break;
	}

    return XY_OK;
}

/**
* netif状态变化回调函数体demo
* 该回调由芯翼内部调用，函数由用户实现，调用xy_Reg_Ps_Event接口注册到芯翼内部
* 如果在回调中有阻塞的操作，用户需创建额外的线程进行处理，在回调中将PS信息传递给该线程处理
* 如果在回调中无阻塞操作，可以不用创建线程，直接在回调中完成。
*/
int do_netif_state_changed(xy_ps_event_param_t* event)
{
    xy_assert(event != NULL);
    xy_netif_info_t *netif_info = (xy_netif_info_t *)event->data;
    xy_assert(netif_info != NULL);
    switch (netif_info->state)
    {
    case XY_NETIF_UP:
    {
        xy_printf("[ps demo]netif up");
        user_ps_msg_t *msg = (user_ps_msg_t *)xy_zalloc(sizeof(user_ps_msg_t));
        msg->netif_info.state = netif_info->state;
        msg->ps_type = PS_NETIF;
        send_ps_msg_to_user_task(msg);
    }
    break;
    case XY_NETIF_DOWN:
        xy_printf("[ps demo]netif down");
        break;
    default:
        break;
    }

    return XY_OK;
}

/**
 * 用户线程函数体，用于处理阻塞操作，
 * 比如pdp激活后调用xy_get_PDP_APN接口查询apn，这个接口有阻塞操作，不能在用户注册的回调函数中处理
 */
void user_ps_process(void)
{
    void *rcv_msg = NULL;
    user_ps_msg_t *msg = NULL;
    while (1)
	{
		osMessageQueueGet(user_ps_q, &rcv_msg, NULL, osWaitForever);
		if (rcv_msg == NULL)
		{
			xy_assert(0);
		}

		msg = ((user_ps_msg_t *)rcv_msg);

        //示例：根据pdp状态进行ps操作
        if (msg->ps_type == PS_PDP)
        {
            switch (msg->pdp_info.state)
            {
            case XY_PS_BEARER_DEACT:
                xy_printf("[ps demo]user task recv pdp deact msg");
                //去激活后如果继续执行ps操作会报错
                user_get_apn_info(msg->pdp_info.cid);
                break;
            case XY_PS_BEARER_ACT:
                xy_printf("[ps demo]user task recv pdp act msg");
                //在收到PDP激活的消息后，用户操作ps相关接口
                user_get_cellid();
                user_get_apn_info(msg->pdp_info.cid);
                break;
            default:
                break;
            }
        } 
        //示例：根据sim卡状态进行ps操作
        else if (msg->ps_type == PS_SIM)
        {        
            switch (msg->sim_info.state)
            {
            case XY_SIM_READY:
                xy_printf("[ps demo]user get sim ready");
                break;
            default:
                break;
            };
        }
        //示例：根据netif状态进行ps操作
        else if(msg->ps_type == PS_NETIF)
        {
            switch (msg->netif_info.state)
            {
            case XY_NETIF_UP:
            {
                //DEMO示例:网络可用后获取本机ipv4地址
                char *ip4addr = (char *)xy_zalloc(16);
                xy_getIP4Addr(ip4addr, 16);
                xy_printf("[ps demo]user get ip:%s", ip4addr);
                xy_free(ip4addr);
            }
            break;
            default:
                break;
            };

        }

        xy_free(msg);
    }
}

/**
 *  ps相关事件的状态上电后可能会立即上报，用户注册回调的时机需要考虑，建议放在main函数中
 *  如果注册的时机靠后，可能错过ps事件上报时间点，导致无法接收事件。
 *  用户注册的回调函数类型必须为xy_ps_event_callback_t
 */
void xy_ps_ext_demo_init()
{
	xy_Reg_Ps_Event(XY_PS_EVENT_PDP_STATE, (xy_ps_event_callback_t)do_ps_pdp_state_changed);
	xy_Reg_Ps_Event(XY_PS_EVENT_SIM_STATE, (xy_ps_event_callback_t)do_ps_sim_state_changed);
	xy_Reg_Ps_Event(XY_PS_EVENT_NETIF_STATE, (xy_ps_event_callback_t)do_netif_state_changed);

    //如果在回调中有阻塞的操作，用户创建额外的线程进行处理，在回调中将pdp或者sim卡信息传递给该线程处理
    //如果在回调中无阻塞操作，可以不用创建线程，直接在回调中完成。
    user_ps_q = osMessageQueueNew(10, sizeof(void *));
    osThreadNew((osThreadFunc_t)(user_ps_process), NULL, "user_ps_process", 0x800, XY_OS_PRIO_NORMAL);
}

#endif //DEMO_TEST
