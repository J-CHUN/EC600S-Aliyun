#include "led.h"


void LED_init(void)	//要初始化LED灯，即为初始化LED灯的控制引脚（PB0）
{
	//0,GPIO_Init函数需要一个结构指针参数，所以我们定义一个结构体
	GPIO_InitTypeDef GPIO_InitStruct;
	
	//1，开启GPIOC时钟
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);
	
	//2，对PB0引脚进行初始化
	GPIO_InitStruct.GPIO_Mode  = GPIO_Mode_Out_PP;	//对这个GPIOx_CRL进行配置
	GPIO_InitStruct.GPIO_Pin   = GPIO_Pin_13;// | 是位或
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOC, &GPIO_InitStruct);	//对PC13进行输出初始化
	
	//3，控制PC13的初始状态
//	GPIO_SetBits(GPIOC, GPIO_Pin_13);
	LED0 = 1;
}

