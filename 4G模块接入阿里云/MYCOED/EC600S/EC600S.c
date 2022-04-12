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
char AtStrBuf[255];   								//��ӡ������
#define BUFLEN 255
unsigned short U2_RX_CntPre = 0;

//BEEP IO��ʼ��
void EC600S_Init(void)
{
	//1)ATָ��������ģ�����	
	while(EC600S_SendCmd("AT\r\n", "OK", 200))          //����ָ�������OK����ģ�������ͨ��
		delay_ms(500);
	while(EC600S_SendCmd("AT+CPIN?\r\n", "READY", 200))  //���SIM���Ƿ���λ
		delay_ms(500);
	while(EC600S_SendCmd("AT+CSQ\r\n", "+CSQ", 200))     //��ѯ�ź�����
		delay_ms(500);
	while(EC600S_SendCmd("AT+CREG?\r\n", "0,1", 200))    //��ѯ����״̬
		delay_ms(500);
}

void EC600S_CONNECT_MQTT_SERVER(char *PRODUCTKEY,char *DEVICENAME,char *DEVICESECRET)
{
	//�رյ�ǰ����
	while(EC600S_SendCmd("AT+QIDEACT=1\r\n", "OK", 200))
		delay_ms(500);
	
	//����Ϊ����ģʽ
	while(EC600S_SendCmd("AT+QMTCFG=\"recv/mode\",0,0,1\r\n", "OK", 200))
		delay_ms(500);
	
	//���ð������ϵ��豸��Ϣ
	memset(AtStrBuf,0,255);
    sprintf(AtStrBuf,"AT+QMTCFG=\"ALIAUTH\",0,\"%s\",\"%s\",\"%s\"\r\n",PRODUCTKEY,DEVICENAME,DEVICESECRET);
	while(EC600S_SendCmd(AtStrBuf,"OK",200))	
		delay_ms(500);
	
	//��һ��MQTT�ͻ�������
	while(EC600S_SendCmd("AT+QMTOPEN=0,\"iot-as-mqtt.cn-shanghai.aliyuncs.com\",1883\r\n", "+QMTOPEN: 0,0", 300))
		delay_ms(500);
	
	//����MQTT������
	memset(AtStrBuf,0,255);
    sprintf(AtStrBuf,"AT+QMTCONN=0,\"%s\"\r\n",DEVICENAME);
	while(EC600S_SendCmd(AtStrBuf,"+QMTCONN: 0,0,0",200)) 
		delay_ms(500);
	
	//���ķ���������������
	memset(AtStrBuf,0,255);
	sprintf(AtStrBuf,"AT+QMTSUB=0,1,\"/sys/%s/%s/thing/service/property/set\",0\r\n",PRODUCTKEY,DEVICENAME);
	while(EC600S_SendCmd(AtStrBuf,"+QMTSUB: 0,1,0,1",200)) 
		delay_ms(500);
	
	printf("�豸�Ѿ����ӵ�������,׼���������� [..]\r\n");
}
 
void EC600S_MQTT_SEND_DATA(char *PRODUCTKEY,char *DEVICENAME,char *DATA)
{
    memset(AtStrBuf,0,BUFLEN);
    sprintf(AtStrBuf,"AT+QMTPUB=0,0,0,0,\"/sys/%s/%s/thing/event/property/post\"\r\n",PRODUCTKEY,DEVICENAME);
	EC600S_SendCmd(AtStrBuf,">",200);   
	
	UART2_SendString(DATA);     //���͵�JSON����
	
	USART_SendData(USART2, 0x1A); //���ݽ������������HEX��ʽ����
	//printf("�û����ݷ��ͳɹ�  [OK]\r\n");
 
  
}

//==========================================================
//	�������ƣ�	EC600S_WaitRecive
//
//	�������ܣ�	�ȴ��������
//
//	��ڲ�����	��
//
//	���ز�����	REV_OK-�������		REV_WAIT-���ճ�ʱδ���
//
//	˵����		ѭ�����ü���Ƿ�������
//==========================================================
_Bool EC600S_WaitRecive(void)
{
	if(U2_RX_Cnt == 0) 							//������ռ���Ϊ0 ��˵��û�д��ڽ��������У�����ֱ����������������
		return REV_WAIT;
		
	if(U2_RX_Cnt == U2_RX_CntPre)				//�����һ�ε�ֵ�������ͬ����˵���������
	{
		U2_RX_Cnt = 0;							//��0���ռ���
			
		return REV_OK;								//���ؽ�����ɱ�־
	}
		
	U2_RX_CntPre = U2_RX_Cnt;					//��Ϊ��ͬ
	
	return REV_WAIT;								//���ؽ���δ��ɱ�־
}


//==========================================================
//	�������ƣ�	EC600S_Clear
//
//	�������ܣ�	��ջ���
//
//	��ڲ�����	��
//
//	���ز�����	��
//
//	˵����		
//==========================================================
void EC600S_Clear(void)
{
	memset(U2_RX_BUF, 0, sizeof(U2_RX_BUF));
	U2_RX_Cnt = 0;

}


//==========================================================
//	�������ƣ�	EC600S_SendCmd
//
//	�������ܣ�	��������
//
//	��ڲ�����	cmd������
//				res����Ҫ���ķ���ָ��
//
//	���ز�����	0-�ɹ�	1-ʧ��
//
//	˵����		
//==========================================================
_Bool EC600S_SendCmd(char *cmd, char *res, u16 time)
{	
	UART2_SendString(cmd);

	while(time--)
	{	
		if(EC600S_WaitRecive() == REV_OK)							//����յ�����
		{	
			if(strstr((const char *)U2_RX_BUF, res) != NULL)		//����������ؼ���
			{
				EC600S_Clear();									//��ջ���
				return 0;
			}
		}
		delay_ms(10);
	}
	
	return 1;
}

/*
���Ͷ��ź���
����1�����պ���
����2����Ϣ����
*/
void EC600S_SendMsg(char *phone,char *txt)
{	
	char dataBuf[24] = {0};
	//���ö��Ÿ�ʽ  0��ʾ����Ϊ PDU��ʽ, 1 ��ʾ����Ϊ�ı���ʽ
	while(EC600S_SendCmd("AT+CMGF=1\r\n","OK", 200))  
		delay_ms(500);
	//���ն��ź���
	sprintf(dataBuf,"AT+CMGS=\"%s\"\r\n",phone);
	while(EC600S_SendCmd(dataBuf,">", 200))  
		delay_ms(500);
	//������Ϣ
	EC600S_SendCmd(txt,NULL,0);
	//���ݽ������������HEX��ʽ����
	USART_SendData(USART2, 0x1A); 
	printf("���ųɹ�����\r\n");
}

/*
��绰����
����1������ĺ���
*/
void EC600S_CallPhone(char *phone)
{	
	char dataBuf[24] = {0};
	sprintf(dataBuf,"ATD%s;\r\n",phone);
	EC600S_SendCmd(dataBuf,NULL,0);
 
	printf("�绰�Ѳ���\r\n");
}
