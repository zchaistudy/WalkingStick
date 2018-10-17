#ifndef _SHAKE_H
#define _SHAKE_H

#include "stm32f10x.h"

#define First  '1'
#define Second '2'
#define Third  '3'
#define Forth  '4'

void TIM2_Breathing_Init(void);
void AdjustVibrationFrequencyWalking(char degree);

#endif  /*_SHAKE_H*/
