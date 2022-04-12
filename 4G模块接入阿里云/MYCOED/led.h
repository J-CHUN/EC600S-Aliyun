#ifndef _LED_H
#define _LED_H

#include <stm32f10x.h>
#include "sys.h"

#define LED0_STA   GPIO_ReadOutputDataBit(GPIOC, GPIO_Pin_13)

// 定义LED引脚位操作的宏
#define LED0 	PCout(13)//定义LED的位操作宏，引脚为PB0，引脚为输出模式
#define LED1 	PBout(1)


//声明功能函数
void LED_init(void);

#endif
