/** 
* @file        xy_mqtt_api.h
* @brief       This file is the MQTT API that users can call.
* @attention   请参考mqtt_opencpu_demo.c进行二次开发。
* @par  
*/
#ifndef _XY_MQTT_API_H
#define _XY_MQTT_API_H
#include <stdint.h>

#include "xy_utils.h"
#include "MQTTClient.h"
#include "MQTTConnect.h"

/**
 *@ingroup mqttapi
 *@brief Send MQTT connect message according to parameters in MQTTClient and MQTTPacket_connectData
 *@param c            [IN]     MQTT client configure,include network state,timer ,etc
 *@param options      [IN]     MQTT connect message parameter
 *@param synflag      [IN]     synchronous mode or asynchronous mode(0:synchronous mode,1:asynchronous mode)
 *@retval #int          0 is succeed, or the error number is failed.
 */
int xy_mqtt_connect(MQTTClient* c, MQTTPacket_connectData* options,int synflag);

/**
 *@ingroup mqttapi
 *@brief Send MQTT disconnect message according to parameters in MQTTClient
 *@param c            [IN]     MQTT client configure,include network state,timer ,etc
 *@param synflag      [IN]     synchronous mode or asynchronous mode(0:synchronous mode,1:asynchronous mode)
 *@retval #int          0 is succeed, or the error number is failed.
 */

int xy_mqtt_disconnect(MQTTClient* c,int synflag);

/**
 *@ingroup mqttapi
 *@brief Send MQTT subscribe message according to parameters in MQTTClient
 *@param c            [IN]     MQTT client configure,include network state,timer ,etc
 *@param topicFilter  [IN]     the topic will unsubscribe 
 *@param qos		  [IN]     quality of service
 *@param synflag      [IN]     synchronous mode or asynchronous mode(0:synchronous mode,1:asynchronous mode)
 *@retval #int          0 is succeed, or the error number is failed.
 */

int xy_mqtt_subscribe(MQTTClient* c, const char* topicFilter, int qos,int synflag);

/**
 *@ingroup mqttapi
 *@brief Send MQTT unsubscribe message according to parameters in MQTTClient
 *@param c            [IN]     MQTT client configure,include network state,timer ,etc
 *@param topicFilter  [IN]     the topic will unsubscribe
 *@param synflag      [IN]     synchronous mode or asynchronous mode(0:synchronous mode,1:asynchronous mode)
 *@retval #int          0 is succeed, or the error number is failed.
 */

int xy_mqtt_unsubscribe(MQTTClient* c, const char* topicFilter,int synflag);

/**
 *@ingroup mqttapi
 *@brief Send MQTT publish message according to parameters in MQTTClient and MQTTMessage
 *@param c            [IN]     MQTT client configure,include network state,timer ,etc
 *@param topicName    [IN]     the topic will publish
 *@param message      [IN]     MQTT publish message parameter
 *@param synflag      [IN]     synchronous mode or asynchronous mode(0:synchronous mode,1:asynchronous mode)
 *@retval #int          0 is succeed, or the error number is failed.
 */

int xy_mqtt_publish(MQTTClient* c, const char* topicName, MQTTMessage* message,int synflag);

/**
 *@ingroup mqttapi
 *@brief Receive and processing MQTT reply and publish message
 *@param c             [IN]     MQTT client configure,include network state,timer ,etc
 *@param messageHandler[IN]     function for receiving MQTT publish message of subscription topic
 *@param synflag       [IN]     synchronous mode or asynchronous mode(0:synchronous mode,1:asynchronous mode)
 *@retval #int          0 is succeed, or the error number is failed.
 */

int xy_mqtt_downlink_process(MQTTClient* c,messageHandler messageHandler,int synflag);

#endif
