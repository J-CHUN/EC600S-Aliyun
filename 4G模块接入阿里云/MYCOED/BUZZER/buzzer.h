#ifndef __BUZZER_H
#define __BUZZER_H	 
#include "sys.h"

#define BUZZERPORT	GPIOA	//����IO�ӿ�
#define BUZZER	GPIO_Pin_6	//����IO�ӿ�



void BUZZER_Init(void);//��ʼ��
void BUZZER_BEEP1(void);//��һ��
void MIDI_PLAY(void);
		 				    
#endif
