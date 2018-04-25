#ifndef __UltrasonicWave_H
#define	__UltrasonicWave_H

#include "stm32f10x.h"
#include "bsp_delay.h"
#include "bsp_usart1.h"
#include "bsp_GeneralTim.h"






#define	TRIG_PORT1     			  GPIOC		//TRIG       
#define	TRIG_PIN1      			  GPIO_Pin_0   //TRIG       

#define	TRIG_PORT2     			  GPIOC		//TRIG       
#define	TRIG_PIN2      			  GPIO_Pin_1   //TRIG     

#define	TRIG_PORT3     			  GPIOC		//TRIG       
#define	TRIG_PIN3      			  GPIO_Pin_2   //TRIG       

#define	TRIG_PORT4     			  GPIOC		//TRIG       
#define	TRIG_PIN4      			  GPIO_Pin_3   //TRIG     

#define	TRIG_PORT5     			  GPIOC		//TRIG       
#define	TRIG_PIN5     			  GPIO_Pin_4   //TRIG 

#define	TRIG_PORT6     			  GPIOC		//TRIG       
#define	TRIG_PIN6      			  GPIO_Pin_5   //TRIG 


#define RCC_APB2Periph_GPIOx  RCC_APB2Periph_GPIOC

#define AVER_NUM        6            //超声波数量

extern int8_t  IT_TAG;

int getDistance(void);
int minusDistance(int distance);
int addDistance(int distance);
void UltrasonicWave_Configuration(void);               //对超声波模块初始化
void UltrasonicWave(int* num);
void dealTIM_ICUserValueStructureData(TIM_ICUserValueTypeDef TIM_ICUserValueStructurex, int i);
#endif /* __UltrasonicWave_H */

