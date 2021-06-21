/****************************************************************************************************
Copyright:   2018-2020, XinYi Info Tech Co.,Ltd.
File name:   MQTTTimer.c
Description: MQTT protocol API function
Author:  gaoj
Version:
Date:    2020.7.20
History:
 ****************************************************************************************************/
#ifndef MQTTTIMER_H_
#define MQTTTIMER_H_


#if defined(WIN32_DLL) || defined(WIN64_DLL)
  #define DLLImport __declspec(dllimport)
  #define DLLExport __declspec(dllexport)
#elif defined(LINUX_SO)
  #define DLLImport extern
  #define DLLExport  __attribute__ ((visibility ("default")))
#else
  #define DLLImport
  #define DLLExport
#endif
#include "xy_utils.h"
#include "lwip/sockets.h"

typedef struct Timer
{
    struct timeval end_time;
} Timer;

typedef struct Network
{
    int my_socket;
    int (*mqttread) (struct Network*, unsigned char*, int, int);
    int (*mqttwrite) (struct Network*, unsigned char*, int, int);
} Network;

void TimerInit(Timer*);
char TimerIsExpired(Timer*);
void TimerCountdownMS(Timer*, unsigned int);
void TimerCountdown(Timer*, unsigned int);
int TimerLeftMS(Timer*);
void xy_gettimeofday(struct timeval *time);
void xy_timersub(struct timeval *a, struct timeval *b, struct timeval *res);

int xy_mqtt_read(Network*, unsigned char*, int, int);
int xy_mqtt_write(Network*, unsigned char*, int, int);

DLLExport void NetworkInit(Network*);
DLLExport int NetworkConnect(Network*, char*, unsigned short);
DLLExport void NetworkDisconnect(Network*);

#endif
