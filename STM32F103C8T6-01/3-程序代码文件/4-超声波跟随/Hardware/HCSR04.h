#ifndef __HCSR04_H
#define __HCSR04_H

#define Trig_Port 		GPIOA
#define Trig_Pin 		GPIO_Pin_0
#define Trig_RCC		RCC_APB2Periph_GPIOA

#define Echo_Port 		GPIOA
#define Echo_Pin 		GPIO_Pin_1
#define Echo_RCC		RCC_APB2Periph_GPIOA

void Timer_Init(void);				//中断初始化设置
void HCSR04_Init(void);				//超声波初始设置
uint16_t HCSR04_GetValue(void);		//获取超声波传感器检测距离的函数
#endif
