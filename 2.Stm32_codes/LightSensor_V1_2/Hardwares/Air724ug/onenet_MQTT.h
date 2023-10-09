#ifndef __ONENET_MQTT_H
#define __ONENET_MQTT_H

#include "main.h" 

void onenet_MQTTConnect(void);
void onenet_TCPDisConnect(void);
void onenet_Data_MPUB(char * strsend);
void onenet_CmdResponse_MPUB(char *CmdID,char * strsend);
uint8_t Connect_Check(void);

extern const char * mqtturl;
extern const char * mqttport;

#endif
