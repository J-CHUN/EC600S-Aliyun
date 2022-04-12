/*
4G模块EC600S接入阿里云--实现数据上传和命令下发
模块接线：
4G模块EC600S：
VCC--5V  GND--GND  TX--PA3  RX--PA2
光照强度传感器BH1750: 
VCC--3.3V  GND--GNG  SCL--PB6  SDA--PB7  ADDR--不接

*/
#include <stm32f10x.h>
#include <stdbool.h>
#include "led.h"
#include "usart.h"
#include "delay.h"
#include "bh1750.h"
#include "EC600S.h"

//三元组
#define PRODUCTKEY 		 "a1VPyJEJRjJ"
#define DEVICENAME  	 "test01"
#define DEVICESECRET     "e7da0d4274b9da4ded7b7e90ad4178be"

//手机号码和短信内容
#define Phone            "10086"
#define Message          "Thank"

int main(void)
{
	float light;     //光照值
	char dataBuf[36]; //存储数据
	unsigned short timeCount = 300;	//发送间隔变量
	
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2); //设置NVIC优先级分组
	SysTick_Init(72);  //初始化延时函数
	LED_init(); //初始化LED PC13
	BH1750_Init();//初始化光照传感器
	USART1_Init(115200);	//初始化串口1--调试用
	USART2_Init(115200);	//初始化串口1--EC600S使用

	EC600S_Init();    //初始化EC600S模块
	EC600S_CONNECT_MQTT_SERVER(PRODUCTKEY,DEVICENAME,DEVICESECRET); //连接阿里云服务器

	// EC600S_SendMsg(Phone,Message);  //发短信
	// EC600S_CallPhone(Phone);        //打电话
	
	while(1)
	{
		delay_ms(10);
		if(timeCount++ >= 300)	//发送间隔3s
		{
			timeCount = 0;
			light =  LIght_Intensity();	//读取光照强度的值
			
			memset(dataBuf,0,36);
			sprintf(dataBuf,"{params:{Light:%.1f}}",light);      //将数据打包json格式
			EC600S_MQTT_SEND_DATA(PRODUCTKEY,DEVICENAME,dataBuf); //发布主题消息
		}	
		if(U2_RX_Cmd==1)		//接收到数据
		{
			U2_RX_Cmd = 0;       //重新置为0
			printf("串口2:收到服务器下发数据:\r\n");
			if(strstr((const char*)U2_RX_BUF,(const char*)"{\"LED\":1}"))
			{
				LED0 = 0;  //低电平开灯
				printf("LED灯已打开\r\n");
			}
			else if(strstr((const char*)U2_RX_BUF,(const char*)"{\"LED\":0}"))
			{
				LED0 = 1;   //高电平关灯
				printf("LED灯已关闭\r\n");
			}
			memset(U2_RX_BUF, 0, sizeof(U2_RX_BUF));  //清空串口2缓冲区
			
			//上传LED灯的状态
			memset(dataBuf,0,36);
			sprintf(dataBuf,"{params:{LED:%d}}",!LED0_STA);     //将数据打包json格式
			EC600S_MQTT_SEND_DATA(PRODUCTKEY,DEVICENAME,dataBuf);//发布主题消息
		}			
		
		//接收到错误，重连阿里云
		if(U2_RX_Error==1)
		{
			U2_RX_Error = 0;   //重新置为0
			printf("接收到错误，重连阿里云....\r\n");
			EC600S_CONNECT_MQTT_SERVER(PRODUCTKEY,DEVICENAME,DEVICESECRET);//连接阿里云服务器
		}
	}
}


