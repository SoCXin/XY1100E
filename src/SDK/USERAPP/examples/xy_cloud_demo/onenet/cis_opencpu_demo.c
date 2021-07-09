/** 
* @file        onenet_opencpu_demo.c
* @ingroup     cloud
* @brief       使用API实现Onenet与网络云平台交互的参考代码,demo完成注册和消息处理，发包操作需要新创建线程操作。
* @attention  
* @note         !!!该源文件默认不编译，用户不得直接在该文件中修改！！！\n
*               而应该是自行创建新的私有的源文件，将demo示例代码拷贝过去后再做修改。
*/

/**
 * @defgroup cloud Cloud
 */

#if DEMO_TEST
#include "xy_api.h"

#if MOBILE_VER
#include "xy_cis_api.h"

/********************************************************************************************************************************/
/*send_notify_data,used for Terminal report event data periodly,always into deep sleep,such as Meter, Smoke/Fire Alarm*/
/*read_pro/write_pro/excute_pro/send_notify_data,used for always power-on products,such as White Goods in Home Appliance*/
/*do_close_onenet, When user need close onenet,Call the func at the appropriate poin */
/********************************************************************************************************************************/

/******************user may modify************************/

/**
 * @brief Demo默认使用的onenet 生命周期
 */	
#define APIDEMO_LIFT_TIME        3000
/**
 * @brief Demo默认注册云平台服务器地址
 */	
#define CIS_SERVER_IP			"183.230.40.40"
/**
 * @brief Demo默认注册云平台服务器端口
 */	
#define CIS_SERVER_PORT			5683

/**
 * @brief Demo默认3311对象的资源数量上限
 */
#define  APIDEMO_RESOURCE_MAX    	4
/**
 * @brief Demo默认3311对象的资源ID
 */
#define  APIDEMO_RESOURCE_ID_1      5850
/**
 * @brief Demo默认3311对象的资源ID
 */
#define  APIDEMO_RESOURCE_ID_2      5851
/**
 * @brief Demo默认3311对象的资源ID
 */
#define  APIDEMO_RESOURCE_ID_3      5706
/**
 * @brief Demo默认3311对象的资源ID
 */
#define  APIDEMO_RESOURCE_ID_4      5805

/**
 * @brief Demo默认资源ID集合
 */
int  apidemo_res_id[APIDEMO_RESOURCE_MAX]={APIDEMO_RESOURCE_ID_1,APIDEMO_RESOURCE_ID_2,APIDEMO_RESOURCE_ID_3,APIDEMO_RESOURCE_ID_4};

/**
 * @brief Demo默认对象ID
 */
#define   APIDEMO_OBJ_ID      3311

/**
 * @brief Demo默认RTC定时器时长
 */
#define   APIDEMO_CIS_RTC_OFFSET   (55+osKernelGetTickCount()%10)


/******************user not care************************/

/**
 * @brief Demo发送上行数据默认ack ID
 */
static int  apidemo_ackid = 100;

/**
 * @brief Demo的工作状态; 0:working, 1:closing, 2:closed
 */
static int  apidemo_work_state = 0;   //0,working;1,closing,2,closed

/**
 * @brief Demo的任务消息队列
 */
osMessageQueueId_t cis_demo_q = NULL; //上下行报文消息队列

/**
 * @brief Demo的任务句柄
 */
osThreadId_t g_cis_apidemo_Handle = NULL;
/**
 * @brief Demo使用的RTC定时器回调函数
 */
static void cis_rtc_timeout_cb(void *para)
{
	(void) para;

	xy_printf("[CISAPIDEMO] Just user RTC callback\n");
}

/**
 * @brief 用户发送上行数据输处理函数
 *@param objId        	[IN]     发送数据的对象ID
 *@param insId        	[IN]     发送数据的对象实例ID
 *@param resId          [IN]     发送数据的资源ID
 *@param valueType      [IN]     发送数据的类型
 *@param value          [IN]     发送数据的内容
 *@param index      	[IN]     上行响应的index
 *@param flag           [IN]     上行响应的结束标志
 *@param valueLen       [IN]     发送数据的长度
 */
int opencpu_send_data(int objId, int insId, int resId, int valueType, char* value, int index, int flag)
{
	//malloc massage memory
	cis_pkt_msg *msg =NULL;
	msg = xy_zalloc(sizeof(cis_pkt_msg) + strlen(value) + 1);
	msg->type = CALLBACK_TYPE_NOTIFY;    //Send upload data must be set notify type; and unlock to sleep until the hook evtid=26
	msg->objId = objId;
	msg->insId = insId;
	msg->resId = resId;
	msg->index = index;
	msg->flag = flag;
	msg->valueType = valueType;
	memcpy(msg->data, value, strlen(value));
	
	if(cis_demo_q == NULL)
		cis_demo_q = osMessageQueueNew(50, sizeof(void *));
	xy_assert(cis_demo_q != NULL);
	//Put data into cis_demo_q, the demo task will read and notify it.
	osMessageQueuePut(cis_demo_q, &msg, 0, osWaitForever);

	return 0;
}

/**
 * @brief 用户下行数据输处理函数，不能阻塞
 *@param type        	[IN]     云平台下发请求类型
 *@param context        [IN]     云平台下发请求的上下文
 *@param uri            [IN]     云平台下发请求URI
 *@param mid            [IN]     云平台下发请求消息ID
 *@param eid            [IN]     云平台下发请求事件ID
 *@param valueType      [IN]     云平台下发请求数据类型
 *@param value          [IN]     云平台下发请求数据内容
 *@param valueLen       [IN]     云平台下发请求数据长度
 */
void cis_demo_downstream_cb(et_callback_type_t type, void* context,cis_uri_t* uri,cis_mid_t mid, cis_evt_t eid, int valueType,
char* value, int valueLen)
{
	(void) context;
	(void) mid;
	(void) valueType;

    if(cis_demo_q == NULL)
    {
        return;
    }
    
	cis_pkt_msg *msg =NULL;
	msg = xy_zalloc(sizeof(cis_pkt_msg) + valueLen + 1);
	msg->type = type;
	msg->flag = uri->flag;
	msg->objId = uri->objectId;
	msg->insId = uri->instanceId;
	msg->resId = uri->resourceId;
	msg->evtId = eid;

    if(value !=NULL && valueLen > 0)
    {
        memcpy(msg->data, value, valueLen);
        msg->data_len = valueLen;
    }

	if(cis_demo_q == NULL)
		cis_demo_q = osMessageQueueNew(16, sizeof(void *));
	xy_assert(cis_demo_q != NULL);

	osMessageQueuePut(cis_demo_q, &msg, 0, osWaitForever);
}

/**
 * @brief 向云平台发送指定对象指定实例的指定资源信息
 *@param msgId        	[IN]     云平台下发请求消息ID
 *@param insId        	[IN]     发送数据的对象实例ID
 *@param resId          [IN]     发送数据的资源ID
 *@param index      	[IN]     上行响应的index
 *@param flag           [IN]     上行响应的结束标志
 *@retval #int   0 
 */
static int opencpu_notify_inst_res_pro(int msgId,int insId,int resId,int index,int flag)
{
	int errorcode = CIS_RET_OK;
	char *at_str = xy_zalloc(100);
	//int result = 1;
	apidemo_ackid += 1;

	if(resId==APIDEMO_RESOURCE_ID_1)
	{
		int valuetype = 5;
		int data = 0;
		sprintf(at_str,"%d",data);
		errorcode = cis_notify_sync(msgId, APIDEMO_OBJ_ID, insId, resId, valuetype, 1, at_str,
		index, flag, apidemo_ackid);

	}
	else if(resId==APIDEMO_RESOURCE_ID_2)
	{
		int valuetype = 3;
		//int datalen = 4;
		int data = 65416;
		sprintf(at_str,"%d",data);
		errorcode = cis_notify_sync(msgId, APIDEMO_OBJ_ID, insId, resId, valuetype, 4, at_str,
		index, flag, apidemo_ackid);

	}
	else if(resId==APIDEMO_RESOURCE_ID_4)
	{
		int valuetype = 4;
		//int datalen = 4;
		float data = 125.001542;
		sprintf(at_str,"%f",data);
		errorcode = cis_notify_sync(msgId, APIDEMO_OBJ_ID, insId, resId, valuetype, 4, at_str,
		index, flag, apidemo_ackid);

	}
	else if(resId==APIDEMO_RESOURCE_ID_3)
	{
		int valuetype = 1;
		char data[10] = "xytest";
		errorcode = cis_notify_sync(msgId, APIDEMO_OBJ_ID, insId, resId, valuetype, strlen(data), data,
		index, flag, apidemo_ackid);


	}
	xy_printf("AP:%s",at_str);
	if (errorcode != CIS_RET_OK)
		xy_assert(0);
	xy_free(at_str);
	return 0;
}

/**
 * @brief 向云平台发送指定对象的所有实例和资源信息
 *@param msg_id        	[IN]     云平台下发请求消息ID
 *@retval #int   0 
 */
static int opencpu_notify_obj_all_pro(int msg_id)
{
	int index_id,i,index,inst_id,flag,resource_id;
	index = 7;
	for(inst_id=0;inst_id<2;inst_id++)
	{
		if(index==7)
			flag=1;
		else if(index==0)
			flag=0;
		else
			flag=2;
		
		for(i=3;i>=0;i--)
		{
			index_id = 3-i;
			resource_id=apidemo_res_id[index_id];
			opencpu_notify_inst_res_pro(msg_id,inst_id,resource_id,index,flag);
			index--;
		}
	}
	return 0;
}

/**
 * @brief 向云平台发送指定对象实例的所有资源信息
 *@param msg_id        	[IN]     云平台下发请求消息ID
 *@param inst_id       	[IN]     发送数据的对象实例ID
 *@retval #int   0 
 */
static int opencpu_notify_inst_all_pro(int msg_id,int inst_id)
{
	int index,index_id,flag,resource_id;
	for(index=3; index>=0; index--)
	{
		if(index==3)
			flag=1;
		else if(index==0)
			flag=0;
		else
			flag=2;
		index_id = 3-index;
		resource_id=apidemo_res_id[index_id];
		opencpu_notify_inst_res_pro(msg_id,inst_id,resource_id,index,flag);
	}
	return 0;
}

/**
 * @brief 读取指定对象实例的指定资源信息
 *@param msgId        	[IN]     云平台下发请求消息ID
 *@param insId        	[IN]     发送数据的对象实例ID
 *@param resId          [IN]     发送数据的资源ID
 *@param index      	[IN]     上行响应的index
 *@param flag           [IN]     上行响应的结束标志
 *@retval #int   0 
 */
static int opencpu_read_inst_res_pro(int msgId,int insId,int resId,int index,int flag)
{
	int errorcode = CIS_RET_ERROR;
	char *at_str = xy_zalloc(100);
	int result = 1;

	if(resId==APIDEMO_RESOURCE_ID_1)
	{
		int valuetype = 5;
		int data = 0;
		sprintf(at_str,"%d",data);
		errorcode = cis_read_rsp(msgId, result, APIDEMO_OBJ_ID, insId, resId, valuetype, 1, at_str,
		index, flag);

	}
	else if(resId==APIDEMO_RESOURCE_ID_2)
	{
		int valuetype = 3;
		//int datalen = 4;
		int data = 65416;
		sprintf(at_str,"%d",data);
		errorcode = cis_read_rsp(msgId, result, APIDEMO_OBJ_ID, insId, resId, valuetype, 4, at_str,
		index, flag);

	}
	else if(resId==APIDEMO_RESOURCE_ID_4)
	{
		int valuetype = 4;
		//int datalen = 4;
		float data = 125.001542;
		sprintf(at_str,"%f",data);
		errorcode = cis_read_rsp(msgId, result, APIDEMO_OBJ_ID, insId, resId, valuetype, 4, at_str,
		index, flag);

	}
	else if(resId==APIDEMO_RESOURCE_ID_3)
	{
		//int valuetype = 1;
		char data[10] = "xytest";
		errorcode = cis_read_rsp(msgId, result, APIDEMO_OBJ_ID, insId, resId, (int)(data), 5, at_str,
		index, flag);

	}
	xy_printf("AP:%s",at_str);
	if (errorcode != CIS_RET_OK)
		xy_assert(0);
	xy_free(at_str);
	return 0;
}

/**
 * @brief 读取指定对象的所有实例和资源信息
 *@param msg_id        	[IN]     云平台下发请求消息ID
 *@retval #int   0 
 */
static int opencpu_read_obj_all_pro(int msg_id)
{
	int index_id,i,index,inst_id,flag,resource_id;
	index = 7;
	for(inst_id=0;inst_id<2;inst_id++)
	{
		if(index==7)
			flag=1;
		else if(index==0)
			flag=0;
		else
			flag=2;
		for(i=3;i>=0;i--)
		{
			if(index==7)
				flag=1;
			else if(index==0)
				flag=0;
			else
				flag=2;
			index_id = 3-i;
			resource_id=apidemo_res_id[index_id];
			opencpu_read_inst_res_pro(msg_id,inst_id,resource_id,index,flag);
			index--;
		}
	}
	return 0;
}

/**
 * @brief 读取指定对象实例的所有资源信息
 *@param msgId        	[IN]     云平台下发请求消息ID
 *@param insId        	[IN]     发送数据的对象实例ID
 *@retval #int   0 
 */
static int opencpu_read_inst_all_pro(int msg_id,int inst_id)
{
	int index,index_id,flag,resource_id;
	for(index=3;index>0;index++)
	{
		if(index==3)
			flag=1;
		else if(index==0)
			flag=0;
		else
			flag=2;
		index_id = 3-index;
		resource_id=apidemo_res_id[index_id];
		opencpu_read_inst_res_pro(msg_id,inst_id,resource_id,index,flag);
	}
	return 0;
}

/**
 * @brief 处理平台下发数据进行event上报
 *@param event_num        	[IN]     上报的事件ID
 *@retval #int   0 is success; other value is error
 */
static int opencpu_event_proc(int event_num)
{	
	int ret = CIS_RET_OK;
	if(event_num == CIS_EVENT_UNREG_DONE)
	{
		apidemo_work_state=2;
		xy_printf("AP close success");
	}
	else if(event_num == CIS_EVENT_UPDATE_SUCCESS)
		xy_printf("AP updata success");
	else if(event_num == CIS_EVENT_UPDATE_FAILED)
		xy_printf("AP updata fail");
	else if(event_num == CIS_EVENT_REG_TIMEOUT)
		xy_printf("AP register timeout");
	else if(event_num == CIS_EVENT_REG_SUCCESS)
		xy_printf("AP register success");
	else if(event_num == CIS_EVENT_NOTIFY_SUCCESS)
		xy_printf("AP notify success");
	else if(event_num == CIS_EVENT_NOTIFY_FAILED)
		xy_printf("AP notify fail");
	else if(event_num == CIS_EVENT_UPDATE_NEED)
	{
		xy_printf("AP need updata");
		if ((ret=cis_updatelife(7200, 0)) != CIS_RET_OK)
		{
			xy_assert(0);
		}		
	}

	return ret;
}

/**
 * @brief 处理平台下发的read请求，并向平台发送响应结果
 *@param msgId        	[IN]     云平台下发请求消息ID
 *@param flag        	[IN]     发送数据的URI flag
 *@param objId          [IN]     发送数据的对象ID
 *@param insId          [IN]     发送数据的实例ID
 *@param resId          [IN]     发送数据的资源ID
 *@param result      	[IN]     Read请求的响应结果
 *@retval #int   0 
 */
static int opencpu_read_rsp(int msgId, int flag, int objId, int insId, int resId, int result)
{
	(void) result;

	xy_printf("AP read start");

	if(objId != APIDEMO_OBJ_ID)
		xy_assert(0);
	if((flag & URI_FLAG_INSTANCE_ID) == 0)
	{
		opencpu_read_obj_all_pro(msgId);
	}
	else if((flag & URI_FLAG_RESOURCE_ID) == 0)
	{
		opencpu_read_inst_all_pro(msgId, insId);
	}
	else
	{
		opencpu_read_inst_res_pro(msgId, insId, resId, 0, 0);
	}
	return 0;
}

/**
 * @brief 处理平台下发的observe请求，并向平台发送响应结果
 *@param msgId        	[IN]     云平台下发请求消息ID
 *@param flag        	[IN]     发送数据的URI flag
 *@param objId          [IN]     发送数据的对象ID
 *@param insId          [IN]     发送数据的实例ID
 *@param resId          [IN]     发送数据的资源ID
 *@param result      	[IN]     Observe请求的响应结果
 *@retval #int   0 
 */
static int opencpu_observe_rsp_and_notify(int msgId, int flag, int objId, int insId, int resId, int result)
{
	(void) objId;

	//response observe result
	cis_observe_rsp(msgId, result);

	
	//Notify all instance and all resource of the specified object id
	if((flag & URI_FLAG_INSTANCE_ID) == 0)
	{
		opencpu_notify_obj_all_pro(msgId);
	}
	//Notify all resource of the specified objectID and instance id
	else if((flag & URI_FLAG_RESOURCE_ID) == 0)
	{
		opencpu_notify_inst_all_pro(msgId, insId);
	}
	//Notify the specified resource id
	else
	{
		opencpu_notify_inst_res_pro(msgId, insId, resId, 0, 0);
	}

	return 0;
}

/**
 * @brief 创建onenet套件，添加了默认的3311对象并使用默认参数向云平台发起注册
 *@retval #int   0 is success; -1 is error
 */
static int cis_context_init()
{
	int ret = CIS_RET_ERROR;
	cis_cloud_setting(CIS_SERVER_IP, CIS_SERVER_PORT, NULL);

	if(strlen((const char *)(g_softap_fac_nv->cloud_server_ip)) == 0)
	{
		xy_printf("[CISDEMO]Err: server ip is empty. Can not create cis");
		return -1;
	}	
	ret = cis_create((char *)(g_softap_fac_nv->cloud_server_ip), (unsigned int)(g_softap_fac_nv->cloud_server_port), 0, NULL);
	if(ret != CIS_RET_OK)
	{
		xy_printf("[CISDEMO]Err: Create cis failed");
		goto failed_out;
	}

	ret = cis_addobj(APIDEMO_OBJ_ID, 2, "11", 4, 0);
	if(ret != CIS_RET_OK)
	{
		xy_printf("[CISDEMO]Err: Add obj(%d) failed", APIDEMO_OBJ_ID);
		goto failed_out;
	}

	ret = cis_reg(APIDEMO_LIFT_TIME);
	if(ret != CIS_RET_OK)
	{
		xy_printf("[CISDEMO]Err: register cis failed");
		goto failed_out;
	}

	return 0;

failed_out:
	cis_delete();
	return -1;
}

/**
 * @brief Demo任务的详细处理流程
 * @note 
 注册完成后会从消息队列中读取消息，根据不同的消息类型进行响应，支持的消息类型请参考et_callback_type_t
 *@retval #int   0
 */
static int cis_demo_pro()
{
	int ret = CIS_RET_OK;
    cis_pkt_msg *rcv_msg = NULL;
	int evtType = -1;
	while(apidemo_work_state != 2)
	{	
		osMessageQueueGet(cis_demo_q, &rcv_msg, NULL, osWaitForever);
		evtType	= (int)rcv_msg->type;
		xy_printf("[CIS_OPENCPU]onenet demo process...");

		xy_printf("[CIS_OPENCPU]work_state(%d),evtType(%d)", apidemo_work_state, evtType);

		//when do closing,only can proc CALLBACK_TYPE_EVENT type  
		if(apidemo_work_state == 1 && evtType != CALLBACK_TYPE_EVENT)
		{
			xy_free(rcv_msg);
			continue;
		}

		switch(evtType)
		{
			case CALLBACK_TYPE_DISCOVER:
				xy_printf("[CIS_OPENCPU]discover flag(%d)", rcv_msg->flag);
				ret = cis_discover_rsp(-1, rcv_msg->objId, 1, 19, "5850;5851;5706;5805");
				break;
			case CALLBACK_TYPE_OBSERVE:
				xy_printf("[CIS_OPENCPU]observe flag(%d),objId(%d),insId(%d),resId(%d)", rcv_msg->flag,rcv_msg->objId, rcv_msg->insId, rcv_msg->resId);
				ret = opencpu_observe_rsp_and_notify(-1, rcv_msg->flag, rcv_msg->objId, rcv_msg->insId, rcv_msg->resId, 1);
				break;
			case CALLBACK_TYPE_OBSERVE_CANCEL:
				//Process observe cancel event
				xy_printf("[CIS_OPENCPU]Read flag(%d),objId(%d),insId(%d),resId(%d)", rcv_msg->flag, rcv_msg->objId, rcv_msg->insId, rcv_msg->resId);
				cis_observe_rsp(-1, 1);
				break;
			case CALLBACK_TYPE_READ:
				//Process read event and send readRsp
				xy_printf("[CIS_OPENCPU]Read flag(%d),objId(%d),insId(%d),resId(%d)", rcv_msg->flag, rcv_msg->objId, rcv_msg->insId, rcv_msg->resId);
				ret = opencpu_read_rsp(-1, rcv_msg->flag, rcv_msg->objId, rcv_msg->insId, rcv_msg->resId, 1);
				break;
			case CALLBACK_TYPE_WRITE:
				//Process write event and send writeRsp
				xy_printf("[CIS_OPENCPU]Write ");
				ret = cis_write_rsp(-1, 1);
				break;
			case CALLBACK_TYPE_EXECUTE:
				//Process execute event and send executeRsp
				xy_printf("[CIS_OPENCPU]Execute ");
				ret = cis_execute_rsp(-1, 1);
				break;
			case CALLBACK_TYPE_OBSERVE_PARAMS:
				//Process parameter event and send parameterRsp
				xy_printf("[CIS_OPENCPU]Set Param ");
				ret = cis_rsp_withparam(-1, 1);
				break;
			case CALLBACK_TYPE_EVENT:
				xy_printf("[CIS_OPENCPU]Event(%d) ", rcv_msg->evtId);
				//Process event inform and try to update
				ret = opencpu_event_proc(rcv_msg->evtId);	
				break;
			case CALLBACK_TYPE_NOTIFY:
				xy_printf("[CIS_OPENCPU]Notify ");
				//Process send notify event
				ret = cis_notify_sync(-1, rcv_msg->objId, rcv_msg->insId, rcv_msg->resId, rcv_msg->valueType,
				rcv_msg->data_len,rcv_msg->data, rcv_msg->index, rcv_msg->flag, apidemo_ackid);	
				break;
			case CALLBACK_TYPE_QUIT:
				xy_printf("[CIS_OPENCPU]Quit ");
				//Process 
				apidemo_work_state = 1;
				//do delete
				if (cis_delete() != CIS_RET_OK)
				{
					xy_assert(0);
				}
				break;
			default:
				break;			

		}
			
		xy_free(rcv_msg);

		if(ret != CIS_RET_OK)
			xy_assert(0);
		
	}
	return 0;
}

/**
 * @brief Demo任务的处理流程入口
 * @note 参考任务自动创建onenet套件，添加了默认的3311对象并使用默认参数向云平台发起注册
 */
static void cis_demo_task(void *args)
{
	(void) args;

	xy_printf("[CIS_OPENCPU]start api demo");
	xy_work_lock(0);

	//wait PDP active
	if(xy_wait_tcpip_ok(2*60) == XY_ERR)
		xy_assert(0);

	xy_printf("[CIS_OPENCPU]cis opencpu demo task start");
	//start onenet task
	if(cis_context_init() != XY_OK)
	{
		goto out;
	}

	//onenet inform event process
	cis_demo_pro();

	xy_printf("[CIS_OPENCPU]cis opencpu demo task end");

	//for to reduce BSS stress,and improve communication success rate,user task must set rand UTC,see  xy_rtc_set_by_day
	xy_rtc_timer_create(RTC_TIMER_USER1,APIDEMO_CIS_RTC_OFFSET,cis_rtc_timeout_cb,NULL);

out:
	xy_work_unlock(0);
    //删除下行报文回调函数
    cis_set_downstream_cb(NULL);
	
	osThreadExit();
}

/**
 * @brief Demo任务的创建和消息队列初始化
 */
void cis_demo_task_init()
{
	xy_printf("cis api demo create");

	if(cis_demo_q == NULL)
		cis_demo_q = osMessageQueueNew(16, sizeof(void *));

	//注册下行报文回调函数
	cis_set_downstream_cb(cis_demo_downstream_cb);

    if(g_cis_apidemo_Handle == NULL)
		g_cis_apidemo_Handle = osThreadNew ((osThreadFunc_t)(cis_demo_task),NULL,"cis_demo_task",0x800,osPriorityNormal);
    else
        xy_printf("cis api demo task running");
}

#endif

#endif

