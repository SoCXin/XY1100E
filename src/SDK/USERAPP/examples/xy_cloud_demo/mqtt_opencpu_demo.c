/*********************************************************************************
* @file        mqtt_opencpu_demo.c
* @ingroup     cloud
* @brief       使用MQTT API实现与MQTT服务器的通信
* @attention
* @note         !!!该源文件默认不编译，用户不得直接在该文件中修改！！！\n
*               而应该是自行创建新的私有的源文件，将demo示例代码拷贝过去后再做修改。
*/
#include "xy_api.h"

#if DEMO_TEST && MQTT
#include "xy_utils.h"
#include "xy_demo.h"
#include "MQTTClient.h"
#include "MQTTTimer.h"

/*authentication information*/
#define MQTT_ClientId       "57950b7ee9f842aab21a16b51c327568"
#define MQTT_UserName       "tr1234567"
#define MQTT_Password       "CK833bttbRSNbHpxdqq0EDtMj4CWn5YnedECF1nblQY"

/*MQTT message param*/
#define MQTT_Topic          "device_control"
#define MQTT_CleanSession   1
#define MQTT_QOS            1

/*will information*/
#define MQTT_Will_Flag      1
#define MQTT_Will_Topic     "device_control"
#define MQTT_Will_Message   "will message"
#define MQTT_Will_Qos       1
#define MQTT_Will_Retained  0

#define MQTT_Command_Timeout 5000
#define MQTT_KeepLive       600
#define MQTT_Sendbuf_Size   200
#define MQTT_Readbuf_Size   200
/*SELECT ERROR CODE*/
#define SELETE_TIMEOUT      -2
#define SELETE_NET_ERR      -1

static osThreadId_t g_mqtttest_handle = NULL;
static osThreadId_t g_mqttdownlink_handle = NULL;
static MQTTClient *pMQTT_test_client={NULL};
struct Demo_Options
{
    char* host;         /* server IP or domain name */
    int port;
    char* proxy_host;
    int proxy_port;
    int MQTTVersion;
} network_options =
{
    "mqtt.ctwing.cn",
    1883,
    "mqtt.ctwing.cn",
    1885,
    4,
};

void mqtt_user_defined_process(MessageData* md)
{
    MQTTMessage* m = md->message;

    xy_printf("[MQTTdemo]payload was %s", m->payload);//only show receive MQTT publish message content
}
void mqtt_downlink_buf_recv(MQTTClient *c)
{
    int ret = -1;
    int timeout   = 2000;
    struct timeval tv;
    fd_set read_fds,exceptfds;

    FD_ZERO(&read_fds);
    FD_ZERO(&exceptfds);

    FD_SET(c->ipstack->my_socket, &read_fds);
    FD_SET(c->ipstack->my_socket, &exceptfds);

    tv.tv_sec  = timeout / 1000;
    tv.tv_usec = (timeout % 1000) * 1000;
    ret = select(c->ipstack->my_socket+1, &read_fds, NULL, &exceptfds, &tv);

    if (ret == 0)
    {
        xy_printf("[MQTTdemo] select timeout");
        return ;
    }
    else if(ret < 0)
    {
        close(c->ipstack->my_socket);
        c->ipstack->my_socket =-1;
        xy_printf("[MQTTdemo] select error ret=%d,err %d", ret, errno);
        return ;
    }

    if(FD_ISSET(c->ipstack->my_socket, &exceptfds))
    {
        close(c->ipstack->my_socket);
        c->ipstack->my_socket =-1;
        return ;
    }

    if(FD_ISSET(c->ipstack->my_socket, &read_fds))
    {
        ret = recv(c->ipstack->my_socket, c->readbuf, c->readbuf_size, 0);

        if (ret < 0)
        {
            if (errno == EAGAIN || errno == EWOULDBLOCK || errno == EINTR)
            {
                xy_printf("[MQTTdemo] no data available for now");
                return ;
            }
            else
            {
                xy_printf("[MQTTdemo] error accured when recv: 0x%x", errno);
                close(c->ipstack->my_socket);
                c->ipstack->my_socket =-1;
                return ;
            }
        }
        else if (ret == 0)
        {
            xy_printf("[MQTTdemo] socket was closed by peer");
            close(c->ipstack->my_socket);
            c->ipstack->my_socket =-1;
            return ;
        }
        else
        {
            xy_printf("[MQTTdemo] packet arrived");
            xy_mqtt_downlink_process(pMQTT_test_client,mqtt_user_defined_process,1); //处理下行数据
        }

    }

    return ;
}
void mqtt_downlink_message_process()
{
    xy_printf("[MQTTdemo] process begin c=%p socket=%d",pMQTT_test_client->ipstack,pMQTT_test_client->ipstack->my_socket);
    while(1)
    {
        if(pMQTT_test_client->ipstack->my_socket < 0 || pMQTT_test_client->close_sock == 1)
            return;

        keepalive(pMQTT_test_client);
        //select downlink message
        mqtt_downlink_buf_recv(pMQTT_test_client);
    }

}

void mqtt_downlink_messgae_rev()
{
    xy_printf("[MQTTdemo]downlink_messgae thread start soc = %d",pMQTT_test_client->ipstack->my_socket);

    mqtt_downlink_message_process();

    g_mqttdownlink_handle = NULL;

    xy_printf("[MQTTdemo]downlink_messgae thread end");
	osThreadExit();

}

void mqtt_downlink_messgae_process_thread_create()
{
    if (g_mqttdownlink_handle != NULL)
    {
        return;
    }
	g_mqttdownlink_handle = osThreadNew ((osThreadFunc_t)(mqtt_downlink_messgae_rev),NULL,"mqtt_downlink_messgae_process",0x1000,osPriorityNormal);
}

void mqtt_connect_param_init(MQTTPacket_connectData *data)
{
    data->MQTTVersion = network_options.MQTTVersion;
    data->clientID.cstring = MQTT_ClientId;
    data->username.cstring = MQTT_UserName;
    data->password.cstring = MQTT_Password;
    data->keepAliveInterval= MQTT_KeepLive;
    data->cleansession     = MQTT_CleanSession;

    data->willFlag               = MQTT_Will_Flag;
    data->will.message.cstring   = MQTT_Will_Message;
    data->will.qos               = MQTT_Will_Qos;
    data->will.retained          = MQTT_Will_Retained;
    data->will.topicName.cstring = MQTT_Will_Topic;
    return;
}

void mqtt_publish_param_init(MQTTMessage* pubmsg)
{
    pubmsg->payload = "abc";              // What to publish
    pubmsg->payloadlen = strlen(pubmsg->payload);
    pubmsg->qos = MQTT_QOS;
    pubmsg->retained = 0;
    pubmsg->dup = 0;
    return;
}
void free_demo_memory(MQTTClient* c)
{
    int i;
	if(c == NULL)
		return;
    for (i = 0; i < MAX_MESSAGE_HANDLERS; ++i)
    {
    	if(c->messageHandlers[i].topicFilter)
    	{
    		xy_free((void *)(c->messageHandlers[i].topicFilter));
			c->messageHandlers[i].topicFilter = NULL;
		}
    }
        
    if(c->ipstack)
        xy_free(c->ipstack);
    if(c->buf)
        xy_free(c->buf);
    if(c->readbuf)
        xy_free(c->readbuf);
    
    xy_free(c);
	c = NULL;
	return;
}

static int xy_mqtt_demo()
{
    int rc = 0;
    char *sendbuf = xy_zalloc(MQTT_Sendbuf_Size);
    char *readbuf = xy_zalloc(MQTT_Readbuf_Size);
	char* test_topic = xy_zalloc(strlen(MQTT_Topic)+1);
    pMQTT_test_client = xy_zalloc(sizeof(MQTTClient));
    pMQTT_test_client->ipstack = xy_zalloc(sizeof(Network));

    strncpy(test_topic,MQTT_Topic,strlen(MQTT_Topic));

    /*network init*/
    NetworkInit(pMQTT_test_client->ipstack);
    NetworkConnect(pMQTT_test_client->ipstack, network_options.host, network_options.port);
    MQTTClientInit((MQTTClient*)(pMQTT_test_client), pMQTT_test_client->ipstack, (unsigned int)(MQTT_Command_Timeout),
    		(unsigned char *)(sendbuf), (size_t)(MQTT_Sendbuf_Size), (unsigned char *)(readbuf), (size_t)(MQTT_Readbuf_Size));

    /*Create thread to receive downlink message asynchronously*/
    mqtt_downlink_messgae_process_thread_create();
    xy_printf( "[MQTTdemo]Starting mqtt_downlink_messgae_process_thread success");

    /*Init MQTT connect message Parameters*/
    MQTTPacket_connectData data = MQTTPacket_connectData_initializer;
    mqtt_connect_param_init(&data);

    /*send MQTT connect message*/
    rc = xy_mqtt_connect(pMQTT_test_client, &data,1);
    xy_assert(SUCCESS == rc);

    /*send MQTT subscribe message*/
    rc = xy_mqtt_subscribe(pMQTT_test_client, test_topic, MQTT_QOS,1);

    /*Init MQTT publish message Parameters*/
	MQTTMessage pubmsg = {0};
    mqtt_publish_param_init(&pubmsg);

    /*send MQTT publish message*/
    rc = xy_mqtt_publish(pMQTT_test_client, test_topic, &pubmsg,1);

    /*send MQTT unsubscribe message*/
    rc = xy_mqtt_unsubscribe(pMQTT_test_client, test_topic,1);

    /*send MQTT disconnect message */
    rc = xy_mqtt_disconnect(pMQTT_test_client,1);

    /*close receive downlink thread */
    pMQTT_test_client->close_sock = 1;

    osDelay(3000);
	xy_assert(g_mqttdownlink_handle == NULL);

    /* free MQTT client*/
    free_demo_memory(pMQTT_test_client);

    return SUCCESS;
}

void mqtt_test_demo(void)
{
    //wait PDP active
    if(xy_wait_tcpip_ok(2*60) == XY_ERR)
        xy_assert(0);

    xy_printf("[MQTTdemo]xy_mqtt_demo start");

    xy_mqtt_demo();

    xy_printf("[MQTTdemo]xy_mqtt_demo end");

    g_mqtttest_handle = NULL;
	osThreadExit();
}
//can call by  user_app_init
void mqtt_opencpu_demo_init()
{
    if (g_mqtttest_handle != NULL)
    {
        return;
    }
	g_mqtttest_handle = osThreadNew ((osThreadFunc_t)(mqtt_test_demo),NULL,"mqtt_test_demo",0x1000,osPriorityNormal1);
}

#endif
