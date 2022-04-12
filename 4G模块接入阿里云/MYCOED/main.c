/*
4Gģ��EC600S���밢����--ʵ�������ϴ��������·�
ģ����ߣ�
4Gģ��EC600S��
VCC--5V  GND--GND  TX--PA3  RX--PA2
����ǿ�ȴ�����BH1750: 
VCC--3.3V  GND--GNG  SCL--PB6  SDA--PB7  ADDR--����

*/
#include <stm32f10x.h>
#include <stdbool.h>
#include "led.h"
#include "usart.h"
#include "delay.h"
#include "bh1750.h"
#include "EC600S.h"

//��Ԫ��
#define PRODUCTKEY 		 "a1VPyJEJRjJ"
#define DEVICENAME  	 "test01"
#define DEVICESECRET     "e7da0d4274b9da4ded7b7e90ad4178be"

//�ֻ�����Ͷ�������
#define Phone            "10086"
#define Message          "Thank"

int main(void)
{
	float light;     //����ֵ
	char dataBuf[36]; //�洢����
	unsigned short timeCount = 300;	//���ͼ������
	
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2); //����NVIC���ȼ�����
	SysTick_Init(72);  //��ʼ����ʱ����
	LED_init(); //��ʼ��LED PC13
	BH1750_Init();//��ʼ�����մ�����
	USART1_Init(115200);	//��ʼ������1--������
	USART2_Init(115200);	//��ʼ������1--EC600Sʹ��

	EC600S_Init();    //��ʼ��EC600Sģ��
	EC600S_CONNECT_MQTT_SERVER(PRODUCTKEY,DEVICENAME,DEVICESECRET); //���Ӱ����Ʒ�����

	// EC600S_SendMsg(Phone,Message);  //������
	// EC600S_CallPhone(Phone);        //��绰
	
	while(1)
	{
		delay_ms(10);
		if(timeCount++ >= 300)	//���ͼ��3s
		{
			timeCount = 0;
			light =  LIght_Intensity();	//��ȡ����ǿ�ȵ�ֵ
			
			memset(dataBuf,0,36);
			sprintf(dataBuf,"{params:{Light:%.1f}}",light);      //�����ݴ��json��ʽ
			EC600S_MQTT_SEND_DATA(PRODUCTKEY,DEVICENAME,dataBuf); //����������Ϣ
		}	
		if(U2_RX_Cmd==1)		//���յ�����
		{
			U2_RX_Cmd = 0;       //������Ϊ0
			printf("����2:�յ��������·�����:\r\n");
			if(strstr((const char*)U2_RX_BUF,(const char*)"{\"LED\":1}"))
			{
				LED0 = 0;  //�͵�ƽ����
				printf("LED���Ѵ�\r\n");
			}
			else if(strstr((const char*)U2_RX_BUF,(const char*)"{\"LED\":0}"))
			{
				LED0 = 1;   //�ߵ�ƽ�ص�
				printf("LED���ѹر�\r\n");
			}
			memset(U2_RX_BUF, 0, sizeof(U2_RX_BUF));  //��մ���2������
			
			//�ϴ�LED�Ƶ�״̬
			memset(dataBuf,0,36);
			sprintf(dataBuf,"{params:{LED:%d}}",!LED0_STA);     //�����ݴ��json��ʽ
			EC600S_MQTT_SEND_DATA(PRODUCTKEY,DEVICENAME,dataBuf);//����������Ϣ
		}			
		
		//���յ���������������
		if(U2_RX_Error==1)
		{
			U2_RX_Error = 0;   //������Ϊ0
			printf("���յ���������������....\r\n");
			EC600S_CONNECT_MQTT_SERVER(PRODUCTKEY,DEVICENAME,DEVICESECRET);//���Ӱ����Ʒ�����
		}
	}
}


