#ifndef _LED_H
#define _LED_H

#include <stm32f10x.h>
#include "sys.h"

#define LED0_STA   GPIO_ReadOutputDataBit(GPIOC, GPIO_Pin_13)

// ����LED����λ�����ĺ�
#define LED0 	PCout(13)//����LED��λ�����꣬����ΪPB0������Ϊ���ģʽ
#define LED1 	PBout(1)


//�������ܺ���
void LED_init(void);

#endif
