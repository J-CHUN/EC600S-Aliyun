#ifndef __usart_H
#define __usart_H

#include <stm32f10x.h>
#include "stdio.h" 
#include "string.h"
#include "led.h"


//�ⲿ���ô��ڽ���������ɵı���
extern u8 U2_RX_BUF[1024];  //���ջ����� 
extern  u16  U2_RX_Cnt;    //���ռ���
extern u8  U2_RX_Cmd ;      //���������־λ U2_RX_Cmd = 1
extern u8  U2_RX_Error;    // ���յ�����

void USART1_Init(u32 bound);
void USART2_Init(u32 bound);
void USART3_Init(u32 bound);

void UART1_SendString(char* s);
void UART2_SendString(char* s);
void UART3_SendString(char *pstr);


#endif


