
#ifndef __EC600S_H
#define __EC600S_H	 
#include "sys.h"

#define BEEP PAout(5)	// PA5
	
#define REV_OK		0	//接收完成标志
#define REV_WAIT	1	//接收未完成标志

void EC600S_Init(void);//初始化
void EC600S_CONNECT_MQTT_SERVER(char *PRODUCTKEY,char *DEVICENAME,char *DEVICESECRET);
void EC600S_MQTT_SEND_DATA(char *PRODUCTKEY,char *DEVICENAME,char *DATA);	

_Bool EC600S_SendCmd(char *cmd, char *res, u16 time);
void EC600S_SendMsg(char *phone,char *txt);
void EC600S_CallPhone(char *phone);
#endif
