#include "stm32f10x.h"                  // Device header
#include "Servo.h"						//导入舵机库文件

uint16_t Wheel_direction = 90;   		//轮子默认角度变量
uint16_t Ultrasonic_direction = 95;   	//超声波默认角度变量

int main(void)
{
	Servo_Init();							//初始化舵机
	Servo_Move(Wheel_direction);			//调用控制电机方向的舵机函数
	Servo_Distance(Ultrasonic_direction);	//调用控制超声波的舵机函数
}
