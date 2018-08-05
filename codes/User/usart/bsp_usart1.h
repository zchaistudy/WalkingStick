#ifndef __USART1_H
#define	__USART1_H

#include "stm32f10x.h"
#include <stdio.h>
#define	MAX_SEND 10

//#define __HLOG__  

#ifdef __HLOG__  
#define DEBUG(format,...) printf (format, ##__VA_ARGS__)   //' ## '的意思是，如果可变参数被忽略或为空，将使预处理器（ preprocessor ）去除掉它前面的那个逗号。
#else  
#define DEBUG(format,...)  
#endif  

void UART1_SendString(char* s);


extern void USART1_Config(u32 bound);
extern int  fputc(int ch, FILE *f);
extern int  fgetc(FILE *f);
void NVIC_Configuration(void);
extern void bsp_USART1_IRQHandler(void);
extern char *get_rebuff(uint8_t *len);
extern void clean_rebuff(void);
void SendGlasses(int* p,int cnt); //发送超声波数据到眼镜

#endif /* __USART1_H */
