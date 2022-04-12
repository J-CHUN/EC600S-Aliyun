#ifndef __EXTI_H
#define __EXTI_H	 

#include "led.h"
#include "delay.h"					    

#define STATE 	PAin(4)    //PA4接蓝牙模块的STATE引脚
void EXTIX_PA4_Init(void);//IO初始化
				    
#endif

