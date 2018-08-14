#ifndef __ANGLE_H
#define	__ANGLE_H
#include "bsp_angle_define.h"

/* º¯ÊýÉêÃ÷ -----------------------------------------------*/
void RCC_Configuration(void);
void GPIO_Configuration(void);
void NVIC_Configuration(void);
void USART1_Configuration(void);
void I2C_GPIO_Config(void);
void Delay(u32 nTime);
void Delayms(vu32 m);  
void InitLSM303D(void);
Statue Multiple_read(unsigned char SlaveAddress,unsigned char REG_Address);
char getAngle(void);
#endif
