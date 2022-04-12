/*
 * @Author: your name
 * @Date: 2020-04-06 02:16:49
 * @LastEditTime: 2020-04-06 02:18:40
 * @LastEditors: your name
 * @Description: In User Settings Edit
 * @FilePath: \undefinedc:\Users\Administrator\Desktop\sunjianping\HARDWARE\BEEP\beep.c
 */

#include "EC600S.h"
 #include "usart.h"
#include "delay.h"
#include "usart.h"	
char AtStrBuf[255];   								//打印缓存器
#define BUFLEN 255
unsigned short U2_RX_CntPre = 0;

//BEEP IO初始化
void EC600S_Init(void)
{
	//1)AT指令入网、模块测试	
	while(EC600S_SendCmd("AT\r\n", "OK", 200))          //测试指令，若返回OK，则模块可正常通信
		delay_ms(500);
	while(EC600S_SendCmd("AT+CPIN?\r\n", "READY", 200))  //检查SIM卡是否在位
		delay_ms(500);
	while(EC600S_SendCmd("AT+CSQ\r\n", "+CSQ", 200))     //查询信号质量
		delay_ms(500);
	while(EC600S_SendCmd("AT+CREG?\r\n", "0,1", 200))    //查询入网状态
		delay_ms(500);
}

void EC600S_CONNECT_MQTT_SERVER(char *PRODUCTKEY,char *DEVICENAME,char *DEVICESECRET)
{
	//关闭当前连接
	while(EC600S_SendCmd("AT+QIDEACT=1\r\n", "OK", 200))
		delay_ms(500);
	
	//配置为接收模式
	while(EC600S_SendCmd("AT+QMTCFG=\"recv/mode\",0,0,1\r\n", "OK", 200))
		delay_ms(500);
	
	//配置阿里云上的设备信息
	memset(AtStrBuf,0,255);
    sprintf(AtStrBuf,"AT+QMTCFG=\"ALIAUTH\",0,\"%s\",\"%s\",\"%s\"\r\n",PRODUCTKEY,DEVICENAME,DEVICESECRET);
	while(EC600S_SendCmd(AtStrBuf,"OK",200))	
		delay_ms(500);
	
	//打开一个MQTT客户端网络
	while(EC600S_SendCmd("AT+QMTOPEN=0,\"iot-as-mqtt.cn-shanghai.aliyuncs.com\",1883\r\n", "+QMTOPEN: 0,0", 300))
		delay_ms(500);
	
	//连接MQTT服务器
	memset(AtStrBuf,0,255);
    sprintf(AtStrBuf,"AT+QMTCONN=0,\"%s\"\r\n",DEVICENAME);
	while(EC600S_SendCmd(AtStrBuf,"+QMTCONN: 0,0,0",200)) 
		delay_ms(500);
	
	//订阅服务器发布的主题
	memset(AtStrBuf,0,255);
	sprintf(AtStrBuf,"AT+QMTSUB=0,1,\"/sys/%s/%s/thing/service/property/set\",0\r\n",PRODUCTKEY,DEVICENAME);
	while(EC600S_SendCmd(AtStrBuf,"+QMTSUB: 0,1,0,1",200)) 
		delay_ms(500);
	
	printf("设备已经连接到阿里云,准备发送数据 [..]\r\n");
}
 
void EC600S_MQTT_SEND_DATA(char *PRODUCTKEY,char *DEVICENAME,char *DATA)
{
    memset(AtStrBuf,0,BUFLEN);
    sprintf(AtStrBuf,"AT+QMTPUB=0,0,0,0,\"/sys/%s/%s/thing/event/property/post\"\r\n",PRODUCTKEY,DEVICENAME);
	EC600S_SendCmd(AtStrBuf,">",200);   
	
	UART2_SendString(DATA);     //发送的JSON数据
	
	USART_SendData(USART2, 0x1A); //数据结束符，需采用HEX形式发送
	//printf("用户数据发送成功  [OK]\r\n");
 
  
}

//==========================================================
//	函数名称：	EC600S_WaitRecive
//
//	函数功能：	等待接收完成
//
//	入口参数：	无
//
//	返回参数：	REV_OK-接收完成		REV_WAIT-接收超时未完成
//
//	说明：		循环调用检测是否接收完成
//==========================================================
_Bool EC600S_WaitRecive(void)
{
	if(U2_RX_Cnt == 0) 							//如果接收计数为0 则说明没有处于接收数据中，所以直接跳出，结束函数
		return REV_WAIT;
		
	if(U2_RX_Cnt == U2_RX_CntPre)				//如果上一次的值和这次相同，则说明接收完毕
	{
		U2_RX_Cnt = 0;							//清0接收计数
			
		return REV_OK;								//返回接收完成标志
	}
		
	U2_RX_CntPre = U2_RX_Cnt;					//置为相同
	
	return REV_WAIT;								//返回接收未完成标志
}


//==========================================================
//	函数名称：	EC600S_Clear
//
//	函数功能：	清空缓存
//
//	入口参数：	无
//
//	返回参数：	无
//
//	说明：		
//==========================================================
void EC600S_Clear(void)
{
	memset(U2_RX_BUF, 0, sizeof(U2_RX_BUF));
	U2_RX_Cnt = 0;

}


//==========================================================
//	函数名称：	EC600S_SendCmd
//
//	函数功能：	发送命令
//
//	入口参数：	cmd：命令
//				res：需要检查的返回指令
//
//	返回参数：	0-成功	1-失败
//
//	说明：		
//==========================================================
_Bool EC600S_SendCmd(char *cmd, char *res, u16 time)
{	
	UART2_SendString(cmd);

	while(time--)
	{	
		if(EC600S_WaitRecive() == REV_OK)							//如果收到数据
		{	
			if(strstr((const char *)U2_RX_BUF, res) != NULL)		//如果检索到关键词
			{
				EC600S_Clear();									//清空缓存
				return 0;
			}
		}
		delay_ms(10);
	}
	
	return 1;
}

/*
发送短信函数
参数1：接收号码
参数2：信息内容
*/
void EC600S_SendMsg(char *phone,char *txt)
{	
	char dataBuf[24] = {0};
	//设置短信格式  0表示短信为 PDU格式, 1 表示短信为文本格式
	while(EC600S_SendCmd("AT+CMGF=1\r\n","OK", 200))  
		delay_ms(500);
	//接收短信号码
	sprintf(dataBuf,"AT+CMGS=\"%s\"\r\n",phone);
	while(EC600S_SendCmd(dataBuf,">", 200))  
		delay_ms(500);
	//发送信息
	EC600S_SendCmd(txt,NULL,0);
	//数据结束符，需采用HEX形式发送
	USART_SendData(USART2, 0x1A); 
	printf("短信成功发送\r\n");
}

/*
打电话函数
参数1：拨打的号码
*/
void EC600S_CallPhone(char *phone)
{	
	char dataBuf[24] = {0};
	sprintf(dataBuf,"ATD%s;\r\n",phone);
	EC600S_SendCmd(dataBuf,NULL,0);
 
	printf("电话已拨打\r\n");
}
