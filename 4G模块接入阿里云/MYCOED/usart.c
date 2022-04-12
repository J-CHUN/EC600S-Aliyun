#include "usart.h"		 
#include "delay.h"
#include <stdbool.h>
#include "EC600S.h"
//下述程序将 stdio.h 中 printf函数进行修改，
//将printf函数的打印端重定义到串口中
#pragma import(__use_no_semihosting)             
//标准库需要的支持函数                 
struct __FILE 
{ 
	int handle; 
}; 

FILE __stdout;       
//定义_sys_exit()以避免使用半主机模式    
int _sys_exit(int x) 
{ 
	x = x; 
	return 0;
} 
//重定义fputc函数   printf 是一个宏
int fputc(int ch, FILE *f)
{ 	
	USART_SendData(USART1, ch);  //通过串口发送数据
	while(USART_GetFlagStatus(USART1, USART_FLAG_TXE)==RESET);      
	return ch;
}


/*******************************************************************************
* 函 数 名         : USART1_Init
* 函数功能		   : USART1初始化函数
* 输    入         : bound:波特率
* 输    出         : 无
*******************************************************************************/ 
void USART1_Init(u32 bound)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	
	//1，使能 GPIOA，USART1，AFIO 时钟
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1,ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO,ENABLE);	 	//GPIO复用时钟，外部中断的GPIO桥接时钟 
	
	//2，初始化GPIO引脚， PA9初始化为推挽复用	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;//TX			//串口输出PA9
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;	     	//推挽复用模式
	GPIO_Init(GPIOA, &GPIO_InitStructure); 
	//PA10初始化为模拟输入
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;//RX			//串口输入PA10
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;	//浮空输入
	GPIO_Init(GPIOA, &GPIO_InitStructure); 
	

	//3，初始化 USART1 
	USART_InitStructure.USART_BaudRate = bound;	//波特率设置
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;//字长为8位数据格式
	USART_InitStructure.USART_StopBits = USART_StopBits_1;//一个停止位
	USART_InitStructure.USART_Parity = USART_Parity_No;//无奇偶校验位
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//无硬件数据流控制
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;	//收发模式，使用“|”进行同时选择
	USART_Init(USART1, &USART_InitStructure); //初始化串口1  
		
	//初始化 Usart1 的NVIC
	NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;		//串口1中断通道，“stm32f10x.h”
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=2; //抢占优先级3
	NVIC_InitStructure.NVIC_IRQChannelSubPriority =2;		//子优先级3
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			//IRQ通道使能
	NVIC_Init(&NVIC_InitStructure);			
	
	//使能串口中断
	USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);	
	
	//使能串口1
	USART_Cmd(USART1, ENABLE); 
}

//初始化串口2
void USART2_Init(u32 bound)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	
	//1，使能 GPIOA，USART2，AFIO 时钟
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2,ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO,ENABLE);	 	//GPIO复用时钟，外部中断的GPIO桥接时钟 
	
	
	//2，初始化GPIO引脚， PA2初始化为推挽复用	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;//TX			//串口输出PA2
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;	     	//推挽复用模式
	GPIO_Init(GPIOA, &GPIO_InitStructure); 
	//PA3初始化为模拟输入
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3;//RX			//串口输入PA3
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;	//浮空输入
	GPIO_Init(GPIOA, &GPIO_InitStructure); 
	

	//3，初始化 USART2
	USART_InitStructure.USART_BaudRate = bound;	//波特率设置
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;//字长为8位数据格式
	USART_InitStructure.USART_StopBits = USART_StopBits_1;//一个停止位
	USART_InitStructure.USART_Parity = USART_Parity_No;//无奇偶校验位
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//无硬件数据流控制
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;	//收发模式，使用“|”进行同时选择
	USART_Init(USART2, &USART_InitStructure); //初始化串口 2
		
	//初始化 Usart2 的NVIC
	NVIC_InitStructure.NVIC_IRQChannel = USART2_IRQn;		//串口1中断通道，“stm32f10x.h”
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority= 2; //抢占优先级2
	NVIC_InitStructure.NVIC_IRQChannelSubPriority =2;		//子优先级2
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			//IRQ通道使能
	NVIC_Init(&NVIC_InitStructure);			
	
	//使能串口中断
	USART_ITConfig(USART2, USART_IT_RXNE, ENABLE);	
	
	//使能串口2
	USART_Cmd(USART2, ENABLE); 
}

void USART3_Init(u32 bound)
{
   //GPIO端口设置
	GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB,ENABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART3,ENABLE);
 
	
	/*  配置GPIO的模式和IO口 */
	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_10;//TX			   //串口输出
	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_AF_PP;	    //复用推挽输出
	GPIO_Init(GPIOB,&GPIO_InitStructure);  /* 初始化串口输入IO */
	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_11;//RX			 //串口输入
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_IN_FLOATING;		  //模拟输入
	GPIO_Init(GPIOB,&GPIO_InitStructure); /* 初始化GPIO */
	

   //USART3 初始化设置
	USART_InitStructure.USART_BaudRate = bound;//波特率设置
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;//字长为8位数据格式
	USART_InitStructure.USART_StopBits = USART_StopBits_1;//一个停止位
	USART_InitStructure.USART_Parity = USART_Parity_No;//无奇偶校验位
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//无硬件数据流控制
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;	//收发模式
	USART_Init(USART3, &USART_InitStructure); //初始化串口3
	
	USART_Cmd(USART3, ENABLE);  //使能串口3 
	
	USART_ClearFlag(USART3, USART_FLAG_TC);
		
	USART_ITConfig(USART3, USART_IT_RXNE, ENABLE);//开启相关中断

	//Usart1 NVIC 配置
	NVIC_InitStructure.NVIC_IRQChannel = USART3_IRQn;//串口3中断通道
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=2;//抢占优先级2
	NVIC_InitStructure.NVIC_IRQChannelSubPriority =2;		//子优先级2
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			//IRQ通道使能
	NVIC_Init(&NVIC_InitStructure);	//根据指定的参数初始化VIC寄存器、	
}


/*******************************************************************************
* 函数名  : UART1_SendString
* 描述    : USART1发送字符串
* 输入    : *s字符串指针
* 输出    : 无
* 返回    : 无 
* 说明    : 无
*******************************************************************************/
void UART1_SendString(char* s)
{
	while(*s)//检测字符串结束符
	{
		while(USART_GetFlagStatus(USART1, USART_FLAG_TC)==RESET); 
		USART_SendData(USART1 ,*s++);//发送当前字符
	}
}


//串口2发送字符串函数
void UART2_SendString(char* s)
{
	while(*s)//检测字符串结束符
	{
		while(USART_GetFlagStatus(USART2, USART_FLAG_TC)==RESET); 
		USART_SendData(USART2 ,*s++);//发送当前字符
	}
	while(USART_GetFlagStatus(USART2, USART_FLAG_TC)==RESET);
}

//串口3发送字符串函数
void UART3_SendString(char *pstr)
{
	char *p = pstr;
	while(*p!='\0')
	{
		USART_SendData(USART3, *p);
		while( USART_GetFlagStatus(USART3, USART_FLAG_TXE)==RESET);//等待发送完成
		USART_ClearFlag(USART3,USART_FLAG_TXE);//清空标志位、
		p++;  //指针偏移
	}
}


void USART1_IRQHandler(void)
{
//	uint8_t d;

	//检测标志位
	if(USART_GetITStatus(USART1,USART_IT_RXNE) == SET)
	{
	//	d=USART_ReceiveData(USART1);	//接收数据
		
		USART_ClearITPendingBit(USART1,USART_IT_RXNE);	//清空标志位
	}
}

u8  U2_RX_BUF[1024]={0};//串口接收缓存区 	
u16  U2_RX_Cnt=0;
u8  U2_RX_Cmd = 0;
u8  U2_RX_Error = 0;
void USART2_IRQHandler(void)
{
	uint8_t d;
	//检测标志位
	if(USART_GetITStatus(USART2,USART_IT_RXNE) == SET)
	{
		//接收数据
		d=USART_ReceiveData(USART2); //接收数据
		
		if(U2_RX_Cnt>1024) U2_RX_Cnt = 0;
		U2_RX_BUF[U2_RX_Cnt++] = d;//记录接收到的值
		//下发命令时，会收到：
	/*
	+QMTRECV: 0,0,"/sys/a1VPyJEJRjJ/test01/thing/service/property/set",92,"{"method":"thing.service.property.set","id":"44777986","params":{"LED":1},"version":"1.0.0"}"
	*/
		//判断有没有接收到命令，version
		if(U2_RX_BUF[U2_RX_Cnt-7]=='v'&&U2_RX_BUF[U2_RX_Cnt-6]=='e'&&U2_RX_BUF[U2_RX_Cnt-5]=='r'&&U2_RX_BUF[U2_RX_Cnt-4]=='s'&&U2_RX_BUF[U2_RX_Cnt-3]=='i'&&U2_RX_BUF[U2_RX_Cnt-2]=='o'&&d == 'n')
		{
			U2_RX_Cmd = 1;  //接收到命令
			U2_RX_Cnt = 0;
		}
		//判断有没有发生错误，ERROR
		if(U2_RX_BUF[U2_RX_Cnt-5]=='E'&&U2_RX_BUF[U2_RX_Cnt-4]=='R'&&U2_RX_BUF[U2_RX_Cnt-3]=='R'&&U2_RX_BUF[U2_RX_Cnt-2]=='O'&&d == 'R')
		{
			U2_RX_Error = 1;  //接收到错误
			U2_RX_Cnt = 0;
		}
	
		//将接收到的数据，通过串口1返发给电脑
		USART_SendData(USART1, d);
	    while( USART_GetFlagStatus(USART1, USART_FLAG_TXE)==RESET); //等待发送完成
		USART_ClearFlag(USART1,USART_FLAG_TXE);//清空标志位
		
		USART_ClearITPendingBit(USART2,USART_IT_RXNE);	//清空标志位
	}
}


void USART3_IRQHandler(void)                	//串口3中断服务程序
{
	uint8_t d;
	//检测标志位
	if(USART_GetITStatus(USART3, USART_IT_RXNE)==SET)
	{
		//接收数据
		d=USART_ReceiveData(USART3);
		
		//将接收到的数据，通过串口1返发给PC
		USART_SendData(USART1, d);
	    while( USART_GetFlagStatus(USART1, USART_FLAG_TXE)==RESET); //等待发送完成
		USART_ClearFlag(USART1,USART_FLAG_TXE);//清空标志位
		USART_ClearITPendingBit(USART3,USART_IT_RXNE);	//清空标志位
	}	
} 	


