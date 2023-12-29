//�궨�壬ʹ�ø��ķ��㣨�ٴ��֣�    �˴����ſ����Լ�ѡ�񣬸���stm32���Ŷ����
#include "stm32f10x.h"                  // Device header 
#define left  GPIO_Pin_13               //������ߵ�ģ���C13
#define middle GPIO_Pin_14              //�����м��ģ���C14   
#define right  GPIO_Pin_15              //�����ұߵ�ģ���C15
 
//----------��ʼ��Ѱ��ģ��Ĳ���-------- 
void Tracing_Init(void)
{
	//�ṹ���ʼ��
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC,ENABLE);              //��������ʱ��B
	GPIO_InitTypeDef GPIO_InitTypeStructure;
 
	GPIO_InitTypeStructure.GPIO_Mode=GPIO_Mode_IPU;                   //ʹ����������ģʽ
	GPIO_InitTypeStructure.GPIO_Pin=left|middle|right ;               //������
	GPIO_InitTypeStructure.GPIO_Speed=GPIO_Speed_50MHz;               //����IO���ٶ�
	GPIO_Init(GPIOC,&GPIO_InitTypeStructure);                         //��Bͨ��
}
 
//----------ͨ��ʶ��ڰ���������ߵ͵�ƽ�Ĳ���-------- 
int Get_State(uint16_t choice)
{
	uint16_t get=0;                                           
	switch(choice)
	{
		case(1): get= GPIO_ReadInputDataBit(GPIOC,left);break;    //��ȡ��ߵĵ�ƽ
		case(2): get=GPIO_ReadInputDataBit(GPIOC,middle);break;   //��ȡ�м�ĵ�ƽ
		case(3): get=GPIO_ReadInputDataBit(GPIOC,right);break;    //��ȡ�ұߵĵ�ƽ
	}
	return get;
	
}
