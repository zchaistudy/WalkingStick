#ifndef __USART1_H
#define	__USART1_H

#include "stm32f10x.h"
#include <stdio.h>


extern void USART1_Config(u32 bound);
extern int  fputc(int ch, FILE *f);
extern int  fgetc(FILE *f);
void NVIC_Configuration(void);
extern void bsp_USART1_IRQHandler(void);
extern char *get_rebuff(uint8_t *len);
extern void clean_rebuff(void);

#endif /* __USART1_H */
