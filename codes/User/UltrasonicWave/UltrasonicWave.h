#ifndef __UltrasonicWave_H
#define	__UltrasonicWave_H

#include "stm32f10x.h"
#include "bsp_delay.h"
#include "bsp_usart.h"
#include "waveConfig.h"

//#define ONLY_WALKINGSTICK             //拐杖单独测试

#define ULTR_NUM        5              //超声波数量
#define AVER_NUM        ULTR_NUM+1            //超声波数量+一位方位模块

extern int8_t  IT_TAG;
extern int UltrasonicWave_Distance[];
int getRange(void);
int minusDistance(int distance);
int addDistance(int distance);
void UltrasonicWave_Configuration(void);               //对超声波模块初始化
void UltrasonicWave(int);
int IsFinishMeasure(void);




#endif /* __UltrasonicWave_H */

