#include "exti.h"//�ж�
#include "usart.h"

#include <stdbool.h>
//������������״̬��STATE����Ƭ��PA4������ߵ�ƽΪ�����ӣ�����״̬Ϊ�͵�ƽ
//�ⲿ�жϳ�ʼ������
void EXTIX_PA4_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
 	EXTI_InitTypeDef EXTI_InitStructure;
 	NVIC_InitTypeDef NVIC_InitStructure;

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO,ENABLE);//�ⲿ�жϣ���Ҫʹ��AFIOʱ��
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);//ʹ��PORTAʱ��
	
	GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_4;//PA4
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU; //���ó���������
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
 	GPIO_Init(GPIOA, &GPIO_InitStructure);//��ʼ��GPIOA
	
	//ѡ������EXTI�ߵ�GPIO����
	GPIO_EXTILineConfig(GPIO_PortSourceGPIOA,GPIO_PinSource4);
	//�жϳ�ʼ������ 
	EXTI_InitStructure.EXTI_Line=EXTI_Line4;
	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;	
	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising_Falling;
	EXTI_InitStructure.EXTI_LineCmd = ENABLE;
	EXTI_Init(&EXTI_InitStructure);		//����EXTI_InitStruct��ָ���Ĳ�����ʼ������EXTI�Ĵ���
	
	NVIC_InitStructure.NVIC_IRQChannel = EXTI4_IRQn;			//ʹ�ܰ������ڵ��ⲿ�ж�ͨ��
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x03;	//��ռ���ȼ�1 
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x03;			//�����ȼ�1
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;					//ʹ���ⲿ�ж�ͨ��
	NVIC_Init(&NVIC_InitStructure);  	  //����NVIC_InitStruct��ָ���Ĳ�����ʼ������NVIC�Ĵ���
}


extern int g_isBreak;
extern int g_handle;  //�Ͽ�����
void EXTI4_IRQHandler(void)    //�ⲿ�жϴ�������
{
	EXTI_ClearITPendingBit(EXTI_Line4);    //���LINE4�ϵ��жϱ�־λ 

}




