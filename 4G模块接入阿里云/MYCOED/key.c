#include "key.h"


void key2_init(void)
{
	//0,GPIO_Init������Ҫһ���ṹָ��������������Ƕ���һ���ṹ��
	GPIO_InitTypeDef GPIO_InitStruct;
	
	//1������GPIOAʱ��
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
	
	//2����PA2���Ž��г�ʼ��
	GPIO_InitStruct.GPIO_Mode  = GPIO_Mode_IPU;	//ѡ����������ģʽ
	GPIO_InitStruct.GPIO_Pin   = GPIO_Pin_13;	//ѡ��ڶ�����
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStruct);	
}
