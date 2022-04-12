#ifndef __usart_H
#define __usart_H

#include <stm32f10x.h>
#include "stdio.h" 
#include "string.h"
#include "led.h"


//外部引用串口接收数据完成的变量
extern u8 U2_RX_BUF[1024];  //接收缓冲区 
extern  u16  U2_RX_Cnt;    //接收计数
extern u8  U2_RX_Cmd ;      //接收命令到标志位 U2_RX_Cmd = 1
extern u8  U2_RX_Error;    // 接收到错误

void USART1_Init(u32 bound);
void USART2_Init(u32 bound);
void USART3_Init(u32 bound);

void UART1_SendString(char* s);
void UART2_SendString(char* s);
void UART3_SendString(char *pstr);


#endif


