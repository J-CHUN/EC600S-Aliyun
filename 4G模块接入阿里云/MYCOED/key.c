#include "key.h"


void key2_init(void)
{
	//0,GPIO_Init函数需要一个结构指针参数，所以我们定义一个结构体
	GPIO_InitTypeDef GPIO_InitStruct;
	
	//1，开启GPIOA时钟
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
	
	//2，对PA2引脚进行初始化
	GPIO_InitStruct.GPIO_Mode  = GPIO_Mode_IPU;	//选择上拉输入模式
	GPIO_InitStruct.GPIO_Pin   = GPIO_Pin_13;	//选择第二引脚
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStruct);	
}
