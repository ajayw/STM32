#include "stm32f10x.h"  // Device header
#include "Delay.h"
#include "PWM_Motor.h"
#include "HCSR04.h"
#include "Servo.h"
//u8--uint8_t--unsigned char 
int leftDistance = 0, rightDistance = 0, middleDistance = 0;  //定义超声波测距变量
u8 Wheel_direction = 90;   		//初始化方向盘舵机角度
u8 Ultrasonic_direction = 95;   //初始化超声波下舵机角度
u8 speedL = 60;					//左边电机速度变量(0~100)
u8 speedR = 60;					//右边电机速度变量

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
*function:方向盘居中函数wheel_Middle()
*舵机角度保存到变量Wheel_direction
*判断距离小于5执行后退函数，距离在10~30时小车跟随前进，其他距离小车保持停止
*/

void wheel_Middle(){ 						//方向盘居中函数
  while(Wheel_direction != 90){
    if(Wheel_direction > 90){
      Wheel_direction--;
      Servo_Move(Wheel_direction);
      Delay_ms(10);
    }else if(Wheel_direction < 90){
      Wheel_direction++;
      Servo_Move(Wheel_direction);
      Delay_ms(10);
      }
  }
}

/*
*获取左/右侧超声波距离的函数
*改变舵机函数参数值XXX可调节旋转角度:Servo_Distance(XXX);
*
*/

void L_R_distance(){	
	Stop(); 							//停车
  	Delay_ms(200); 						//延时200毫秒
	Servo_Distance(175);				//舵机带动超声波左转
	Delay_ms(300);
	leftDistance =	HCSR04_GetValue();	//获取左侧的距离
	Delay_ms(200);
	
	Servo_Distance(15);					//右转
	Delay_ms(300);
	rightDistance =	HCSR04_GetValue();	//获取右侧的距离
	Delay_ms(200);
	Servo_Distance(95);					//回正
}

/*
*避障函数：Obstacle_Avoidance()
*比较左右侧距离leftDistance 和rightDistance，往距离大一侧行驶，当进入死胡同，也就是两侧都小于25时就后退再获取左、右侧距离,循环重复判断
*/
void Obstacle_Avoidance(){
	wheel_Middle();
	middleDistance = HCSR04_GetValue();
	if(middleDistance <= 35){				//超声波距离小于等于35即为遇到障碍
		Backward(speedL,speedR);			//ִ执行后退函数刹车
		Delay_ms(50);
		L_R_distance();						//调用函数获取左、右侧距离
		if(leftDistance > 25 && leftDistance >= rightDistance){
			TurnLeft(speedL,speedR);
			Delay_ms(500);
		}else if(rightDistance >25 && rightDistance >= leftDistance){
			TurnRight(speedL,speedR);
      		Delay_ms(500);
		}else if(leftDistance <= 25 && rightDistance <= 25){	//进入死胡同。两边距离都小于25
			while(leftDistance <= 25 && rightDistance <= 25){	//一直后退，直到至少有一边大于25
				Backward(speedL,speedR);
				Delay_ms(150);
				L_R_distance();
			}
			if(leftDistance >= rightDistance){
				TurnLeft(speedL,speedR);
				Delay_ms(500);
			}else if(rightDistance > leftDistance){
				TurnRight(speedL,speedR);
				Delay_ms(500);
			}
		}
	}else{
		Forward(speedL,speedR);
	}
}

int main(void)
{
	PWM_Motor_Init();						//PWM：PB6,PB7控制速度	电机：PA4,PB5,PB8,PB9，控制旋转方向
	Servo_Init();							//初始化舵机设置
	HCSR04_Init();							//初始化超声波设置
	Stop();
	Servo_Move(Wheel_direction);			//轮子舵机复位
	Servo_Distance(Ultrasonic_direction);	//超声波舵机复位
	Delay_ms(3000);
	while (1){
    	Obstacle_Avoidance();
  	}
}
