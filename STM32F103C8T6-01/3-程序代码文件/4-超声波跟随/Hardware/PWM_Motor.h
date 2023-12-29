#ifndef __PWM_H
#define __PWM_H

void PWM_Motor_Init(void);			//初始化电机pwm设置
void PWM_Motor_L(uint16_t Compare);	//控制左侧电机
void PWM_Motor_R(uint16_t Compare);	//控制右侧电机

#endif
