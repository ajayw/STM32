#include "stm32f10x.h"  // Device header
#include "delay.h"
#include "PWM_Motor.h"
#include "Servo.h"
#include "Tracking.h"

u8 Wheel_direction = 90;   	//初始化方向盘舵机角度
u8 speedL = 40;				//左边电机速度变量(0~100)
u8 speedR = 40;				//右边电机速度变量

void Forward(u8 sp_L,u8 sp_R){	//小车前进
	PWM_Motor_L(sp_L);
	PWM_Motor_R(sp_R);
	GPIO_SetBits(GPIOB, GPIO_Pin_8);		//左轮
	GPIO_ResetBits(GPIOB, GPIO_Pin_9);
	GPIO_ResetBits(GPIOA, GPIO_Pin_4);		//右轮
	GPIO_SetBits(GPIOB, GPIO_Pin_5);
}

void Backward(u8 sp_L,u8 sp_R){	//小车后退
	PWM_Motor_L(sp_L);
	PWM_Motor_R(sp_R);
	GPIO_ResetBits(GPIOB, GPIO_Pin_8);		//左轮
	GPIO_SetBits(GPIOB, GPIO_Pin_9);
	GPIO_SetBits(GPIOA, GPIO_Pin_4);		//右轮
	GPIO_ResetBits(GPIOB, GPIO_Pin_5);
}

void TurnLeft(u8 sp_L,u8 sp_R){	//小车左转
	PWM_Motor_L(sp_L);
	PWM_Motor_R(sp_R);
	GPIO_ResetBits(GPIOB, GPIO_Pin_8);		//左轮
	GPIO_SetBits(GPIOB, GPIO_Pin_9);
	GPIO_ResetBits(GPIOA, GPIO_Pin_4);		//右轮
	GPIO_SetBits(GPIOB, GPIO_Pin_5);
}

void TurnRight(u8 sp_L,u8 sp_R){	//小车右转
	PWM_Motor_L(sp_L);
	PWM_Motor_R(sp_R);
	GPIO_SetBits(GPIOB, GPIO_Pin_8);		//左轮
	GPIO_ResetBits(GPIOB, GPIO_Pin_9);
	GPIO_SetBits(GPIOA, GPIO_Pin_4);		//右轮
	GPIO_ResetBits(GPIOB, GPIO_Pin_5);
}

void Stop(){	//停止
	GPIO_ResetBits(GPIOB, GPIO_Pin_8);		//左轮
	GPIO_ResetBits(GPIOB, GPIO_Pin_9);	
	GPIO_ResetBits(GPIOA, GPIO_Pin_4);		//右轮
	GPIO_ResetBits(GPIOB, GPIO_Pin_5);
}
/*
*function:循迹函数Tracking()
*三路循迹保存到变量TI/T2/T3
*当小车左边传感器检测到黑线，代表小车偏右较多，需要急左转调整。而中间和左边同时检测到黑线，
*代表小车偏右较小，需要缓左转调整。而只有中间传感器检测到黑线，代表小车正在黑线中间，一边保持直走，右边传感器也同理
*/

void Tracking(){
	int T1 = Get_State(1);
  	int T2 = Get_State(2);
  	int T3 = Get_State(3);
	
	//检测到黑线为1，检测到红外反光为0
	if(T1 == 1 && T2 == 1 && T3 == 1){	//全部识别到黑线或拿起超出距离范围
		Stop();
	}else if(T1 == 0 && T2 == 1 && T3 == 0){	//中间识别到黑线
		Servo_Move(90);
		Forward(speedL,speedR);
	}else if(T1 == 1 && T2 == 0 && T3 == 0){
		Servo_Move(130);
		Forward(speedL-30,speedR+25);
	}else if(T1 == 1 && T2 == 1 && T3 == 0){
		Servo_Move(130);
		Forward(speedL-30,speedR+25);
	}else if(T1 == 0 && T2 == 1 && T3 == 1){
		Servo_Move(50);
		Forward(speedL+25,speedR-30);
	}else if(T1 == 0 && T2 == 0 && T3 == 1){
		Servo_Move(50);
		Forward(speedL+25,speedR-30);
	}else if(T1 == 0 && T2 == 0 && T3 == 0){	//全部识别到反光
		Forward(speedL,speedR);
	}
}

int main(void)
{
	PWM_Motor_Init();						//PWM：PB6,PB7控制速度	电机：PA4,PB5,PB8,PB9，控制旋转方向
	Servo_Init();
	Track_Init();
	Stop();
	Servo_Move(Wheel_direction);			//轮子舵机复位
	Delay_ms(3000);
	while (1){
    	Tracking();
  	}
}
