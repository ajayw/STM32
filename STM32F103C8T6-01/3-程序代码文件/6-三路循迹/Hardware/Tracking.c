//宏定义，此处引脚可以自己选择，根据stm32引脚定义表
#include "stm32f10x.h"                  // Device header
#include "Tracking.h"

#define track_left  GPIO_Pin_13               //定义左边的模块接C13
#define track_middle GPIO_Pin_14              //定义中间的模块接C14   
#define track_right  GPIO_Pin_15              //定义右边的模块接C15
 
//----------初始话寻迹模块的部分-------- 
void Track_Init(void)
{
	//结构体初始化
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC,ENABLE);              //开启外设时钟B
	GPIO_InitTypeDef GPIO_InitTypeStructure;
 
	GPIO_InitTypeStructure.GPIO_Mode=GPIO_Mode_IPU;                   //使用上拉输入模式
	GPIO_InitTypeStructure.GPIO_Pin=track_left|track_middle|track_right ;               //绑定引脚
	GPIO_InitTypeStructure.GPIO_Speed=GPIO_Speed_50MHz;               //设置IO口速度
	GPIO_Init(GPIOC,&GPIO_InitTypeStructure);                         
}
 
//----------通过识别黑白线来输入高低电平的部分-------- 
int Get_State(uint16_t choice)
{
	uint16_t get=0;                                           
	switch(choice)
	{
		case(1): get = GPIO_ReadInputDataBit(GPIOC,track_left);break;    //获取左边的电平
		case(2): get = GPIO_ReadInputDataBit(GPIOC,track_middle);break;   //获取中间的电平
		case(3): get = GPIO_ReadInputDataBit(GPIOC,track_right);break;    //获取右边的电平
	}
	return get;
}
