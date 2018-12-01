/**
  ******************************************************************************
  * @file    bsp_usart1.c
  * @author  fire
  * @version V1.0
  * @date    2013-xx-xx
  * @brief   重现c库printf函数到usart端口
  ******************************************************************************
  * @attention
  *
  * 实验平台:野火 iSO STM32 开发板
  * 论坛    :http://www.chuxue123.com
  * 淘宝    :http://firestm32.taobao.com
  *
  ******************************************************************************
  */

#include "bsp_usart.h"
#include "debug.h"

char YY[10]={'\0'};
/**
 * @brief  USART1 GPIO 配置,工作模式配置。115200 8-N-1
 * @param  无
 * @retval 无
 */
void USART1_Config(u32 bound)
{
    GPIO_InitTypeDef GPIO_InitStructure;
    USART_InitTypeDef USART_InitStructure;
		NVIC_InitTypeDef NVIC_InitStructure;
    /* config USART1 clock */
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1 | RCC_APB2Periph_GPIOA , ENABLE);

    /* USART1 GPIO config */
    /* Configure USART1 Tx (PA.09) as alternate function push-pull */
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOA, &GPIO_InitStructure);

    /* Configure USART1 Rx (PA.10) as input floating */
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
    GPIO_Init(GPIOA, &GPIO_InitStructure);

    /* USART1 mode config */
    USART_InitStructure.USART_BaudRate = bound;
    USART_InitStructure.USART_WordLength = USART_WordLength_8b;
    USART_InitStructure.USART_StopBits = USART_StopBits_1;
    USART_InitStructure.USART_Parity = USART_Parity_No ;
    USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
    USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
		
		NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;
		NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=3 ;//抢占优先级3
		NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;		//子优先级3
		NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			//IRQ通道使能
		NVIC_Init(&NVIC_InitStructure);	//根据指定的参数初始化VIC寄存器
		
    USART_Init(USART1, &USART_InitStructure);
		USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);	//开启串口接受中断
    USART_Cmd(USART1, ENABLE);
}



struct __FILE 
{ 
	int handle; 

}; 

FILE __stdout;       
//定义_sys_exit()以避免使用半主机模式    
void _sys_exit(int x) 
{ 
	x = x; 
} 

///重定向c库函数printf到USART1
int fputc(int ch, FILE *f)
{
    /* 发送一个字节数据到USART1 */
    USART_SendData(USART1, (uint8_t) ch);

    /* 等待发送完毕 */
    while (USART_GetFlagStatus(USART1, USART_FLAG_TXE) == RESET);

	
/***********************************/
#if DEBUG_USART3_USART1_ENABLE == 1    //通过串口三打印数据
	USART_SendData(USART3, (uint8_t) ch);
    /* 等待发送完毕 */
    while (USART_GetFlagStatus(USART3, USART_FLAG_TXE) == RESET);	
	
#endif	
/***********************************/	
	
    return (ch);
}

///重定向c库函数scanf到USART1
int fgetc(FILE *f)
{
    /* 等待串口1输入数据 */
    while (USART_GetFlagStatus(USART1, USART_FLAG_RXNE) == RESET);

    return (int)USART_ReceiveData(USART1);
}


/*******************************************************************************
* 函数名  : UART1_SendString
* 描述    : USART1发送字符串
* 输入    : *s字符串指针
* 输出    : 无
* 返回    : 无 
* 说明    : 无
*******************************************************************************/
void UART1_SendString(char* s)
{
	while(*s)//检测字符串结束符
	{
		while(USART_GetFlagStatus(USART1, USART_FLAG_TC)==RESET); 
		USART_SendData(USART1 ,*s++);//发送当前字符
	}
}

/*******************************************************************************
* 函数名  : SendGlasses
* 描述    : USART1向眼镜发送拐杖超声波数据
* 输入    : *s指针
* 输出    : 无
* 返回    : 无 
* 说明    : 无
*******************************************************************************/
void SendGlasses(int* p,int cnt)
{
	int i,aa;
//		YY[0]='!';
//		for(i=0;i<5;i++)                   //直接使用for循环会出现位置错乱——逆序
//		{
//			YY[i+1]=p[i]+'0';			
//		}
//		YY[i+1]='\0';											//USART_SendData发送单个数据，串口调试助手收到的是正常数据，而串口中断接收到的却顺序错乱
//		printf("%s",YY);									//使用printf发数据，串口接收到的都是字符型   ch = USART_ReceiveData(USART1); 此时ch=49
		printf("!");
			for(i=0;i<5;i++)                  
		{
			aa=*p++;
			printf("%d%d%d",aa/100,(aa%100)/10,aa%10);    //由于上诉的无奈，所以将一个三位数的数据，采用了一个字符一个字符发送的方式
		}
}

/*******************************************************************************
* 函数名  : SendAngle
* 描述    : USART1向眼镜发送拐杖方位信息
* 输入    : *s指针
* 输出    : 无
* 返回    : 无 
* 说明    : 无
*******************************************************************************/
void SendAngle(char ch)
{
		printf("#");
		printf("%c",ch);
}

/*********************************************END OF FILE**********************/


void usart3Config(void)
{
		GPIO_InitTypeDef GPIO_InitStructure;
		USART_InitTypeDef USART_InitStructure;
		NVIC_InitTypeDef NVIC_InitStructure;
		/* config USART3 clock */
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
		RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART3, ENABLE);

		/* USART3 GPIO config */
		/* Configure USART3 Tx  as alternate function push-pull */
		GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
		GPIO_Init(GPIOB, &GPIO_InitStructure);
				
		/* Configure USART3 Rx  as input floating */
		GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11;
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
		GPIO_Init(GPIOB, &GPIO_InitStructure);
			
		/* USART2 mode config */
		USART_InitStructure.USART_BaudRate = 38400;                
		USART_InitStructure.USART_WordLength = USART_WordLength_8b;
		USART_InitStructure.USART_StopBits = USART_StopBits_1;
		USART_InitStructure.USART_Parity = USART_Parity_No ;
		USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
		USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;

		NVIC_InitStructure.NVIC_IRQChannel = USART3_IRQn;
		NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=2 ;//抢占优先级2
		NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2;		//子优先级3
		NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			//IRQ通道使能
		NVIC_Init(&NVIC_InitStructure);	//根据指定的参数初始化VIC寄存器
		
		USART_Init(USART3, &USART_InitStructure); 
//		USART_ITConfig(USART3, USART_IT_RXNE, ENABLE);//开启串口接受中断
		USART_Cmd(USART3, ENABLE);
}



//通过串口3发送一个字符
//ch 待发送字符
char Usart3Send(char ch)
{
    /* 发送一个字节数据到USART3 */
    USART_SendData(USART3, (uint8_t) ch);

    /* 等待发送完毕 */
    while (USART_GetFlagStatus(USART3, USART_FLAG_TXE) == RESET);

    return (ch);
}

//通过串口3发送一个字符串
//s 待发送字符串
void Usart3SendString(char* s)
{
	while(*s)//检测字符串结束符
	{
		 while (USART_GetFlagStatus(USART3, USART_FLAG_TXE) == RESET);
		USART_SendData(USART3 ,*s++);//发送当前字符
	}
}



