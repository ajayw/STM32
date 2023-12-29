#include "stm32f10x.h"                  			// Device header
#include "Delay.h"
#include "Servo.h"
#include "HCSR04.h"
#include "Serial.h"            						//蓝牙通信模块
#include "PWM_Motor.h" 
#include "Tracking.h" 

uint16_t AT;  										//接收蓝牙发过来的指令
u8 Wheel_direction = 90;   							//轮子默认角度
u8 Ultrasonic_direction = 95;   					//超声波默认角度
u8 Car_MOD = 0;  									//小车功能模式
u8 Servo_MOD = 0;  									//前轮转向判定操作模式
u8 Gear = 1; 										//设置默认车速档位为1
uint16_t speedL = 60;								//左边电机速度变量
uint16_t speedR = 60;								//右边电机速度变量
u8 Move_MOD = 0;  									//前进/后退/停车模式

int leftDistance = 0, rightDistance = 0, middleDistance = 0;  	//定义超声波三个方向距离

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

void car_gear(){  //小车档位速度
  switch (Gear){
    case 1: speedL = 70;	speedR = 70;	break; //一档速度
    case 2: speedL = 120;	speedR = 120;	break; //二档速度
    case 3: speedL = 225;	speedR = 225;	break; //三挡速度
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
	if(Wheel_direction > 60){
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
//控制舵机旋转函数Servo_ctl()
void Servo_ctl(){ 				
	if(Servo_MOD == 1){			//车轮左转
		wheelServo_L();
	}else if(Servo_MOD == 2){ 	//车轮回正
		wheelServo_M();
	}else if(Servo_MOD == 3){ 	//车轮右转
		wheelServo_R();
	}else if(Servo_MOD == 4){	//视角舵机左转
		Visual_Angle_left();
	}else if(Servo_MOD == 5){	//视角舵机回正
		Visual_Angle_middle();
	}else if(Servo_MOD == 6){	//视角舵机右转
		Visual_Angle_Right();
	}
}

/*
*获取左/右侧超声波距离的函数
*改变舵机函数参数值XXX可调节旋转角度:Servo_Distance(XXX);//0<xxx<180，预留极限位置，取15<XXX<175
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

void Bluetooth(){
	if (Serial_GetRxFlag() == 1)//判断串口是否有信息
	{        
		AT = Serial_GetRxData();//接收蓝牙发送的指令
		switch(AT)
		{
			case 'S': Move_MOD = 0;  break;  	//停车
			case 'F': Move_MOD = 1;  break;  	//前进
			case 'B': Move_MOD = 2;  break;  	//后退
			case 'Q': Move_MOD = 3;  break;		//原地左转
			case 'E': Move_MOD = 4;  break;		//原地右转
			case 'H': Servo_MOD = 0;  break;  	//停止舵机转向
			case 'Z': Servo_MOD = 1;  break;  	//前轮方向向左转
			case 'X': Servo_MOD = 2;  break;  	//前轮方向转到中间
			case 'C': Servo_MOD = 3;  break;  	//前轮方向向右转
			case 'I': Servo_MOD = 4;  break;	//视角舵机方向左转
			case 'O': Servo_MOD = 5;  break;	//视角舵机方向回正
			case 'P': Servo_MOD = 6;  break;	//视角舵机方向右转
			case 'J': GPIO_ResetBits(GPIOB, GPIO_Pin_1); break;	//蜂鸣器低电平关
			case 'K': GPIO_SetBits(GPIOB, GPIO_Pin_1); break;	//蜂鸣器高电平开
			case '1': Gear = 1;   break;  	//车速1档 40
			case '2': Gear = 2;   break;  	//车速2档 70
			case '3': Gear = 3;   break;  	//车速3档 100
			case '4': Car_MOD = 0;	wheelServo_M();	 Visual_Angle_middle();	break;  //自由控制模式
			case '5': Car_MOD = 1; 	wheelServo_M();	 Visual_Angle_middle();	break;	//避障模式
			case '6': Car_MOD = 2;  wheelServo_M();	 Visual_Angle_middle();	break;  //寻迹模式
			case '7': Car_MOD = 3;  wheelServo_M();	 Visual_Angle_middle();	break;  //跟随模式
			case 'D':wheelServo_M();
			default: break;
		}
	}
}
/*
*避障函数：Obstacle_Avoidance()
*比较左右侧距离leftDistance 和rightDistance，往距离大一侧行驶，当进入死胡同，也就是两侧都小于25时就后退再获取左、右侧距离
*修改左转函数TurnLeft()后面Delay_ms(xxx)函数中的参数xxx可调节左转的时间，从而控制左转角度大小。
*/
void Obstacle_Avoidance(){
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

/*
*循迹函数:Tracking()
*三路循迹保存到变量TI/T2/T3
*判断变量==1为检测到黑线，只有左边传感器检测到黑线就左转急弯调整，中间及左边同时检测到就左转缓弯调整，右边同理，只有中间检测到就直走
*/
void Tracking(){
	int T1 = Get_State(1);
  	int T2 = Get_State(2);
  	int T3 = Get_State(3);
	
	//检测到黑线为1，检测到红外反光为0
	if(T1 == 1 && T2 == 1 && T3 == 1){			//前部识别到黑线
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

/*
*跟随函数:Follow()
*获取超声波距离保存到变量middleDistance
*判断距离小于15执行后退函数，距离在15~25时停车，25~45时小车跟随前进，其他距离小车保持停止
*/

void Follow(){							//跟随函数
	wheelServo_M();
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
	Serial_Init();		//初始化串口
	Servo_Init();		//初始化舵机
	PWM_Motor_Init();	//初始化电机
	HCSR04_Init();		//初始化超声波传感器引脚功能
	Buzzer_Init();		//初始化蜂鸣器引脚
	Track_Init();		//初始化寻迹模式引脚功能
	Servo_Move(Wheel_direction);			//初始化方向轮角度
	Servo_Distance(Ultrasonic_direction);	//初始化视角角度
	while (1)
	{
		Bluetooth();
		if(Car_MOD == 0)		//自由控制模式
		{
			Servo_ctl();  		//自由模式舵机控制
			car_gear();
			if(Move_MOD == 0){
				Stop();
			}else if(Move_MOD == 1){
				Forward(speedL,speedR);
			}else if(Move_MOD == 2){
				Backward(speedL,speedR);
			}else if(Move_MOD == 3){
				Servo_MOD = 2;				//车轮舵机回正
				TurnLeft(speedL,speedR);
			}else if(Move_MOD == 4){
				Servo_MOD = 2;				//车轮舵机回正
				TurnRight(speedL,speedR);
			}
		}else if(Car_MOD == 1){	//避障模式
			speedL = 60;	speedR = 60;	//避障模式车速
			Obstacle_Avoidance();
		}else if(Car_MOD == 2){	//循迹模式
			speedL = 40;	speedR = 40;	//循迹模式车速
			Tracking();
		}else if(Car_MOD == 3){	//跟随模式模式
			speedL = 50;	speedR = 50;	//跟随模式车速
			Follow();
		}
	}
}
