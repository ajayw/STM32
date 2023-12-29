#include "stm32f10x.h"                  // Device header
#include "Delay.h"

int main(void)
{
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);
	
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_13;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOC, &GPIO_InitStructure);
	
	while (1)
	{	//三种方式实现LED闪烁
		GPIO_ResetBits(GPIOC, GPIO_Pin_13);				//将PC13引脚置低，即将LED灯关闭，然后等待500毫秒（半秒），再将PC13引脚置高，打开LED灯，接着再等待500毫秒
		Delay_ms(500);
		GPIO_SetBits(GPIOC, GPIO_Pin_13);
		Delay_ms(500);
		
		GPIO_WriteBit(GPIOC, GPIO_Pin_13, Bit_RESET);
		Delay_ms(500);
		GPIO_WriteBit(GPIOC, GPIO_Pin_13, Bit_SET);
		Delay_ms(500);
		
		GPIO_WriteBit(GPIOC, GPIO_Pin_13, (BitAction)0);//自带枚举：typedef enum{Bit_Bit_RESET=0,Bit_SET}BitAction;
		Delay_ms(500);
		GPIO_WriteBit(GPIOC, GPIO_Pin_13, (BitAction)1);
		Delay_ms(500);
	}
}
