#ifndef _KEY_H
#define _KEY_H

#include <stm32f10x.h>
#include "sys.h"

// 全局变量声明（外部引用）

// 定义按键的位操作
#define KEY2	PAin(13)

// 声明功能函数
void key2_init(void);

#endif
