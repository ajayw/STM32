#ifndef __WS2812B_H
#define __WS2812B_H	 
#include "sys.h"

#define RGB_PIN PBout(0)	// PB0

void RGB_Init(void);    //初始化
void Right_arrow(void); //右转箭头，带车灯
void Left_arrow(void);  //左转箭头，带车灯
void on_the_headlights(void); //关闭左转箭头，带车灯
void OFF_RGB(void); //关闭灯光
void Rainbow_mbp(void); //彩虹色
void Left_arrow_OFF_LIGHT(void); //打开左转向灯并关闭车灯
void Right_arrow_OFF_LIGHT(void); //打开右转箭头并关闭车灯
void OB_mbp(void); //避障模式，青色灯光
void tracking_mbp(void); // 寻迹模式，橙色灯光
void Follow_mbp(void); //跟随模式，粉色灯光
 
#endif
