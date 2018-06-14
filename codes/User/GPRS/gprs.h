#ifndef __GPRS_H
#define __GPRS_H

#include "stm32f10x.h"


void GPRS_Send_help(void);
void GPRS_Send_GPS(float lo, float la);
void Timer2_Init_Config(void);
int send_pdu_message(char *content);
int check_status(void);
#endif
