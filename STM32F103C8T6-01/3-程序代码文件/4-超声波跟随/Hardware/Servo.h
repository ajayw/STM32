#ifndef __SERVO_H
#define __SERVO_H

void Servo_Init(void);					//初始化舵机设置
void Servo_Move(float Compare);			//下舵机控制前轮函数
void Servo_Distance(float Compare);		//上舵机控制超声波函数

#endif
