#include "stm32f10x.h"                  // Device header
#include "Delay.h"
#include "Servo.h"
#include "Serial.h"            			//蓝牙通信模块
#include "PWM_Motor.h" 

uint16_t AT;  //接收蓝牙发过来的指令
  
u8 Wheel_direction = 90;   							//轮子默认角度
u8 Ultrasonic_direction = 95;   					//超声波默认角度
u8 Servo_MOD = 0;  									//舵机判定操作模式
u8 Gear = 1; 										//设置默认车速档位为1
uint16_t speedL = 60;								//左边电机速度变量
uint16_t speedR = 60;								//右边电机速度变量
u8 Move_MOD = 0;  									//前进/后退/停车等运动模式
u8 carlight_mod = 1;								//LED开关

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

void TurnLeft(u8 sp_L,u8 sp_R){	//小车原地左转
	PWM_Motor_L(sp_L);
	PWM_Motor_R(sp_R);
	GPIO_ResetBits(GPIOB, GPIO_Pin_8);		//左轮
	GPIO_SetBits(GPIOB, GPIO_Pin_9);
	GPIO_ResetBits(GPIOA, GPIO_Pin_4);		//右轮
	GPIO_SetBits(GPIOB, GPIO_Pin_5);
}

void TurnRight(u8 sp_L,u8 sp_R){	//小车原地右转
	PWM_Motor_L(sp_L);
	PWM_Motor_R(sp_R);
	GPIO_SetBits(GPIOB, GPIO_Pin_8);		//左轮
	GPIO_ResetBits(GPIOB, GPIO_Pin_9);
	GPIO_SetBits(GPIOA, GPIO_Pin_4);		//右轮
	GPIO_ResetBits(GPIOB, GPIO_Pin_5);
}

void Stop(){	//停止
	PWM_Motor_L(0);
	PWM_Motor_R(0);
	GPIO_ResetBits(GPIOB, GPIO_Pin_8);		//左轮
	GPIO_ResetBits(GPIOB, GPIO_Pin_9);
	GPIO_ResetBits(GPIOA, GPIO_Pin_4);		//右轮
	GPIO_ResetBits(GPIOB, GPIO_Pin_5);
}

//初始化蜂鸣器引脚
void Buzzer_Init(void)	
{
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, &GPIO_InitStructure);
	GPIO_SetBits(GPIOB, GPIO_Pin_1);	//初始化为高电平，防止开机就响
}

void Bluetooth(){
	if (Serial_GetRxFlag() == 1)//判断串口是否有信息
	{
		AT = Serial_GetRxData();//接收蓝牙发送的指令  
      	switch(AT)
      	{
			case 'S': Move_MOD = 0;  break;  	//停车
			case 'F': Move_MOD = 1;  break;  	//前进
      		case 'B': Move_MOD = 2;  break;  	//后退
			case 'Q': Move_MOD = 3;  break;	//原地左转
			case 'E': Move_MOD = 4;  break;	//原地右转
			case 'H': Servo_MOD = 0;  break;  	//停止两个舵机，用来控制舵机点动
      		case 'Z': Servo_MOD = 1;  break;  	//前轮方向向左转
      		case 'X': Servo_MOD = 2;  break;  	//前轮方向转到中间
      		case 'C': Servo_MOD = 3;  break;  	//前轮方向向右转
			case 'I': Servo_MOD = 4;  break;	//视角舵机方向左转
			case 'O': Servo_MOD = 5;  break;	//视角舵机方向回正
			case 'P': Servo_MOD = 6;  break;	//视角舵机方向右转
			case 'J': GPIO_ResetBits(GPIOB, GPIO_Pin_1); break;	//蜂鸣器关
			case 'K': GPIO_SetBits(GPIOB, GPIO_Pin_1); break;	//蜂鸣器开
      		case '1': Gear = 1;   break;  	//车速1档 70
      		case '2': Gear = 2;   break;  	//车速2档 120
      		case '3': Gear = 3;   break;  	//车速3档 225
      		default: break;
      	}
	}
}

void car_gear(){  									//小车档位速度
  switch (Gear){
    case 1: speedL = 70;	speedR = 70;	break; 	//一档速度
    case 2: speedL = 120;	speedR = 120;	break; 	//二档速度
    case 3: speedL = 225;	speedR = 225;	break; 	//三挡速度
    default: break;
  }
}

void wheelServo_L(){ //车轮方向左转
  if(Wheel_direction < 130){
      Wheel_direction++;
      Servo_Move(Wheel_direction);
      Delay_ms(10);
    }
}

void wheelServo_M(){ //车轮方向回正
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

void wheelServo_R(){  //车轮方向右转
  if(Wheel_direction > 50){
    Wheel_direction--;
    Servo_Move(Wheel_direction);
    Delay_ms(10);
  }
}

void Visual_Angle_left(){	//视角左转
	if(Ultrasonic_direction < 165){
		Ultrasonic_direction++;
		Servo_Distance(Ultrasonic_direction);
		Delay_ms(10);
    }
}

void Visual_Angle_middle(){	//视角回正
	while(Ultrasonic_direction != 95){
		if(Ultrasonic_direction > 95){
			Ultrasonic_direction--;
			Servo_Distance(Ultrasonic_direction);
			Delay_ms(10);
		}else if(Ultrasonic_direction < 95){
			Ultrasonic_direction++;
			Servo_Distance(Ultrasonic_direction);
			Delay_ms(10);
		}
	}
}

void Visual_Angle_Right(){//视角右转
	if(Ultrasonic_direction > 25){
		Ultrasonic_direction--;
		Servo_Distance(Ultrasonic_direction);
		Delay_ms(10);
	}
}

void Servo_ctl(){ 		//舵机转向判断
  if(Servo_MOD == 1){ 		//车轮左转
    wheelServo_L();
  }else if(Servo_MOD == 2){ //车轮回正
    wheelServo_M();
  }else if(Servo_MOD == 3){ //车轮右转
    wheelServo_R();   
  }else if(Servo_MOD == 4){	//视角舵机左转
	Visual_Angle_left();
  }else if(Servo_MOD == 5){	//视角舵机回正
	Visual_Angle_middle();
  }else if(Servo_MOD == 6){	//视角舵机右转
	Visual_Angle_Right();
  }
}

int main(void)
{
	Serial_Init();		//初始化串口
	PWM_Motor_Init();	//电机初始化
	Servo_Init();		//初始化舵机
	Buzzer_Init();		//初始化蜂鸣器引脚
	Servo_Move(Wheel_direction);				//初始化方向轮角度
	Servo_Distance(Ultrasonic_direction);		//初始化视角角度

	while (1)
	{
		Bluetooth(); 	//接收蓝牙数据
		Servo_ctl();  	//自由模式舵机控制
		car_gear();		//档位
		if(Move_MOD == 0){
			Stop();
		}else if(Move_MOD == 1){
			Forward(speedL,speedR);		//前进
		}else if(Move_MOD == 2){
			Backward(speedL,speedR);	//后退
		}else if(Move_MOD == 3){
			TurnLeft(speedL,speedR);	//左转
		}else if(Move_MOD == 4){
			TurnRight(speedL,speedR);	//右转
		}				
	}
}
