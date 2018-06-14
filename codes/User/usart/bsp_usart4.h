#ifndef __USART4_H
#define	__USART4_H

#include "stm32f10x.h"

#define  DEBUG_USARTx                   UART4
#define  DEBUG_USART_CLK                RCC_APB1Periph_UART4
#define  DEBUG_USART_APBxClkCmd         RCC_APB1PeriphClockCmd
#define  DEBUG_USART_BAUDRATE           115200

// USART GPIO 引脚宏定义
#define  DEBUG_USART_GPIO_CLK           (RCC_APB2Periph_GPIOC)
#define  DEBUG_USART_GPIO_APBxClkCmd    RCC_APB2PeriphClockCmd
    
#define  DEBUG_USART_TX_GPIO_PORT         GPIOC   
#define  DEBUG_USART_TX_GPIO_PIN          GPIO_Pin_10
#define  DEBUG_USART_RX_GPIO_PORT       GPIOC
#define  DEBUG_USART_RX_GPIO_PIN        GPIO_Pin_11

#define  DEBUG_USART_IRQ                UART4_IRQn
#define  DEBUG_USART_IRQHandler         UART4_IRQHandler


void UART4_Init_Config(u32 bound);
void UART4_SendString(char* s);

void UART4_SendLR(void);
void UART4_Send_Command(char* s);
u8 UART4_Send_AT_Command(char *b,char *a,u8 wait_time,u32 interval_time);
void UART4_Send_Command_END(char* s); 
u8 UART4_Send_AT_Command_End(char *b,char *a,u8 wait_time,u32 interval_time);

//串口1发送回车换行
#define UART1_SendLR() UART1_SendString("\r\n")

//串口2发送回车换行
#define UART4_SendLR() UART4_SendString("\r\n")
//	

#endif /* __USART1_H */
