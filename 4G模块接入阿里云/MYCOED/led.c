#include "led.h"


void LED_init(void)	//Ҫ��ʼ��LED�ƣ���Ϊ��ʼ��LED�ƵĿ������ţ�PB0��
{
	//0,GPIO_Init������Ҫһ���ṹָ��������������Ƕ���һ���ṹ��
	GPIO_InitTypeDef GPIO_InitStruct;
	
	//1������GPIOCʱ��
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);
	
	//2����PB0���Ž��г�ʼ��
	GPIO_InitStruct.GPIO_Mode  = GPIO_Mode_Out_PP;	//�����GPIOx_CRL��������
	GPIO_InitStruct.GPIO_Pin   = GPIO_Pin_13;// | ��λ��
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOC, &GPIO_InitStruct);	//��PC13���������ʼ��
	
	//3������PC13�ĳ�ʼ״̬
//	GPIO_SetBits(GPIOC, GPIO_Pin_13);
	LED0 = 1;
}

