#include "stm32f10x.h"  // Device header
#include "delay.h"	
#include "PWM_Motor.h"	

void Forward(unsigned char sp_L,unsigned char sp_R){	//小车前进
	PWM_Motor_L(sp_L);
	PWM_Motor_R(sp_R);
	GPIO_SetBits(GPIOB, GPIO_Pin_8);		//左轮
	GPIO_ResetBits(GPIOB, GPIO_Pin_9);
	GPIO_ResetBits(GPIOA, GPIO_Pin_4);		//右轮
	GPIO_SetBits(GPIOB, GPIO_Pin_5);
}

void Backward(unsigned char sp_L,unsigned char sp_R){	//小车后退
	PWM_Motor_L(sp_L);
	PWM_Motor_R(sp_R);
	GPIO_ResetBits(GPIOB, GPIO_Pin_8);		//左轮
	GPIO_SetBits(GPIOB, GPIO_Pin_9);
	GPIO_SetBits(GPIOA, GPIO_Pin_4);		//右轮
	GPIO_ResetBits(GPIOB, GPIO_Pin_5);
}

void TurnLeft(unsigned char sp_L,unsigned char sp_R){	//小车原地左转
	PWM_Motor_L(sp_L);
	PWM_Motor_R(sp_R);
	GPIO_ResetBits(GPIOB, GPIO_Pin_8);		//左轮
	GPIO_SetBits(GPIOB, GPIO_Pin_9);
	GPIO_ResetBits(GPIOA, GPIO_Pin_4);		//右轮
	GPIO_SetBits(GPIOB, GPIO_Pin_5);
}

void TurnRight(unsigned char sp_L,unsigned char sp_R){	//小车原地右转
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

int main(void)
{
	PWM_Motor_Init();	//PWM：PB6,PB7控制速度	电机：PA4,PB5,PB8,PB9，控制旋转方向
	Delay_ms(3000);
	while (1)
	{		
		//调用移动函数，参数0~100，控制左右两边轮子速度
		Forward(80,80); //Forward(motor_left_speed,motor_right_speed)
		Delay_ms(2000);

		//后退
		Backward(80,80);
		Delay_ms(2000);
		
		//原地左转
		TurnLeft(80,80);
		Delay_ms(2000);

		//原地右转
		TurnRight(80,80);
		Delay_ms(2000);

		//停止
		Stop();
		Delay_ms(2000);
	}
}
