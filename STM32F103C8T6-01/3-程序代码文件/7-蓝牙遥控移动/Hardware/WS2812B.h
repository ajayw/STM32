#ifndef __WS2812B_H
#define __WS2812B_H	 
#include "sys.h"

#define RGB_PIN PBout(0)	// PB0

void RGB_Init(void);    //��ʼ��
void Right_arrow(void); //��ת��ͷ��������
void Left_arrow(void);  //��ת��ͷ��������
void on_the_headlights(void); //�ر���ת��ͷ��������
void OFF_RGB(void); //�رյƹ�
void Rainbow_mbp(void); //�ʺ�ɫ
void Left_arrow_OFF_LIGHT(void); //����ת��Ʋ��رճ���
void Right_arrow_OFF_LIGHT(void); //����ת��ͷ���رճ���
void OB_mbp(void); //����ģʽ����ɫ�ƹ�
void tracking_mbp(void); // Ѱ��ģʽ����ɫ�ƹ�
void Follow_mbp(void); //����ģʽ����ɫ�ƹ�
 
#endif
