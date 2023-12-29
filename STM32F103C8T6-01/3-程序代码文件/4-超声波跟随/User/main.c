#include "stm32f10x.h"  // Device header
#include "Delay.h"
#include "PWM_Motor.h"
#include "HCSR04.h"
#include "Servo.h"

int middleDistance = 0;  					//定义超声波测距变量
u8 Wheel_direction = 90;   				//初始化方向盘舵机角度
u8 Ultrasonic_direction = 95;   			//初始化超声波下舵机角度
u8 speedL,speedR;							//左右侧电机速度变量(0~100)

//GPIO_SetBits();引脚置高电平
//GPIO_ResetBits();引脚置低电平
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

void TurnRight(u8 sp_L,u8 sp_R){//小车右转
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
*跟随函数:Follow()
*获取超声波距离保存到变量middleDistance
*判断距离小于15执行后退函数，距离在25~45时小车跟随前进，其他距离小车保持停止
*/

void Follow(){							//跟随函数
	middleDistance = HCSR04_GetValue();	//获取正前方距离值
	int Fmod; 							//前后方向刹车判断,0：在后退情况下执行一下前进以达到急停效果，1：在前进情况下执行一下后退以达到急停效果
	int flag;							//执行状态，0:没有执行过刹车，1：已经执行过刹车
	if(middleDistance <= 15){			//如果距离小于等于15cm，执行后退
		Backward(speedL,speedR);		//后退
		Fmod = 0;						//标记目前属于后退
		flag = 0;						//标记目前没执行急刹车
	}else if(middleDistance > 15 && middleDistance <= 25){	//如果距离大于15.且小于等于25，停车
		if(Fmod == 0){
			if(flag == 0){
				Forward(speedL,speedR);	//执行50ms前进，急刹车
				Delay_ms(20);
				flag = 1;				//更改执行状态，防止再次执行
			}
		}else if(Fmod == 1){
			if(flag == 0){
				Backward(speedL,speedR);//执行50ms后退，急刹车
				Delay_ms(20);
				flag = 1;				//更改执行状态，防止再次执行
			}
		}
		Stop();							//停车
	}else if(middleDistance > 25 && middleDistance <= 45){
		Forward(speedL,speedR);			//前进
		Fmod = 1;
		flag = 0;
	}else{
		if(Fmod == 0){
			if(flag == 0){
				Forward(speedL,speedR);	//执行50ms前进，急刹车
				Delay_ms(20);
				flag = 1;				//更改执行状态，防止再次执行
			}
		}else if(Fmod == 1){			
			if(flag == 0){				
				Backward(speedL,speedR);//执行50ms后退，急刹车
				Delay_ms(20);
				flag = 1;				//更改执行状态，防止再次执行
			}
		}
		Stop();
	}
}

int main(void)
{
	PWM_Motor_Init();	//PWM：PB6,PB7控制速度	电机：PA4,PB5,PB8,PB9，控制旋转方向
	Servo_Init();		//初始化舵机设置
	HCSR04_Init();		//初始化超声波设置
	Stop();				//执行一次停止
	Servo_Move(Wheel_direction);			//轮子舵机复位
	Servo_Distance(Ultrasonic_direction);	//超声波舵机复位
	Delay_ms(3000);
	while (1){
    	Follow();
  	}
}
