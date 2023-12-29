#include "stm32f10x.h"                  // Device header

void PWM_Motor_Init(void)	//配置控制电机的PWM接口
{
	//使能TIM4和配置GPIOA、GPIOB的时钟。
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4, ENABLE); 
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
	
	//配置GPIOB的6、7脚
	GPIO_InitTypeDef GPIO_InitStructure1;
	GPIO_InitStructure1.GPIO_Mode = GPIO_Mode_AF_PP;//复用推挽输出模式
	GPIO_InitStructure1.GPIO_Pin = GPIO_Pin_6 | GPIO_Pin_7;
	GPIO_InitStructure1.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, &GPIO_InitStructure1);//指针GPIO_InitStructure1,将设置应用到GPIOB

	TIM_InternalClockConfig(TIM4);//内部定时TIM4初始化
	
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure;
	TIM_TimeBaseInitStructure.TIM_ClockDivision = TIM_CKD_DIV1;
	TIM_TimeBaseInitStructure.TIM_CounterMode = TIM_CounterMode_Up;	//复用输出
	TIM_TimeBaseInitStructure.TIM_Period = 100 - 1;					//ARR 周期为100-1，表示每隔0.1毫秒产生一次PWM输出
	TIM_TimeBaseInitStructure.TIM_Prescaler = 72 - 1;				//PSC PSC  预分频器为72-1，即时钟频率为72MHz，表示每个时钟周期划分为72个计数周期
	TIM_TimeBaseInitStructure.TIM_RepetitionCounter = 0;
	TIM_TimeBaseInit(TIM4, &TIM_TimeBaseInitStructure);
	
	TIM_OCInitTypeDef TIM_OCInitStructure;
	TIM_OCStructInit(&TIM_OCInitStructure);//执行一次默认设置
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;//PWM1模式
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;//输出极性为高电平有效
	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;//使能输出状态
	TIM_OCInitStructure.TIM_Pulse = 50;		//CCR  占空比通过修改TIM3->CCR1-4寄存器的值来实现
	TIM_OC1Init(TIM4, &TIM_OCInitStructure);
	TIM_OC2Init(TIM4, &TIM_OCInitStructure);
	TIM_Cmd(TIM4, ENABLE);
	
	GPIO_InitTypeDef GPIO_InitStructure2;
	GPIO_InitStructure2.GPIO_Pin = GPIO_Pin_5 | GPIO_Pin_8 | GPIO_Pin_9; 	//初始化PB5,PB8,PB9
	GPIO_InitStructure2.GPIO_Mode = GPIO_Mode_Out_PP;//设置推挽输出模式
	GPIO_InitStructure2.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, &GPIO_InitStructure2);
	
	GPIO_InitTypeDef GPIO_InitStructure3;
	GPIO_InitStructure3.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure3.GPIO_Pin = GPIO_Pin_4;	//初始化PA4
	GPIO_InitStructure3.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure3);
 	
}

void PWM_Motor_L(uint16_t Compare)	//左侧马达速度
{
	TIM_SetCompare2(TIM4, Compare);
}

void PWM_Motor_R(uint16_t Compare)	//右侧马达速度
{
	TIM_SetCompare1(TIM4, Compare);
}
