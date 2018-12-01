/**
  ******************************************************************************
  * @file    bsp_usart1.c
  * @author  fire
  * @version V1.0
  * @date    2013-xx-xx
  * @brief   ����c��printf������usart�˿�
  ******************************************************************************
  * @attention
  *
  * ʵ��ƽ̨:Ұ�� iSO STM32 ������
  * ��̳    :http://www.chuxue123.com
  * �Ա�    :http://firestm32.taobao.com
  *
  ******************************************************************************
  */

#include "bsp_usart.h"
#include "debug.h"

char YY[10]={'\0'};
/**
 * @brief  USART1 GPIO ����,����ģʽ���á�115200 8-N-1
 * @param  ��
 * @retval ��
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
		NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=3 ;//��ռ���ȼ�3
		NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;		//�����ȼ�3
		NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			//IRQͨ��ʹ��
		NVIC_Init(&NVIC_InitStructure);	//����ָ���Ĳ�����ʼ��VIC�Ĵ���
		
    USART_Init(USART1, &USART_InitStructure);
		USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);	//�������ڽ����ж�
    USART_Cmd(USART1, ENABLE);
}



struct __FILE 
{ 
	int handle; 

}; 

FILE __stdout;       
//����_sys_exit()�Ա���ʹ�ð�����ģʽ    
void _sys_exit(int x) 
{ 
	x = x; 
} 

///�ض���c�⺯��printf��USART1
int fputc(int ch, FILE *f)
{
    /* ����һ���ֽ����ݵ�USART1 */
    USART_SendData(USART1, (uint8_t) ch);

    /* �ȴ�������� */
    while (USART_GetFlagStatus(USART1, USART_FLAG_TXE) == RESET);

	
/***********************************/
#if DEBUG_USART3_USART1_ENABLE == 1    //ͨ����������ӡ����
	USART_SendData(USART3, (uint8_t) ch);
    /* �ȴ�������� */
    while (USART_GetFlagStatus(USART3, USART_FLAG_TXE) == RESET);	
	
#endif	
/***********************************/	
	
    return (ch);
}

///�ض���c�⺯��scanf��USART1
int fgetc(FILE *f)
{
    /* �ȴ�����1�������� */
    while (USART_GetFlagStatus(USART1, USART_FLAG_RXNE) == RESET);

    return (int)USART_ReceiveData(USART1);
}


/*******************************************************************************
* ������  : UART1_SendString
* ����    : USART1�����ַ���
* ����    : *s�ַ���ָ��
* ���    : ��
* ����    : �� 
* ˵��    : ��
*******************************************************************************/
void UART1_SendString(char* s)
{
	while(*s)//����ַ���������
	{
		while(USART_GetFlagStatus(USART1, USART_FLAG_TC)==RESET); 
		USART_SendData(USART1 ,*s++);//���͵�ǰ�ַ�
	}
}

/*******************************************************************************
* ������  : SendGlasses
* ����    : USART1���۾����͹��ȳ���������
* ����    : *sָ��
* ���    : ��
* ����    : �� 
* ˵��    : ��
*******************************************************************************/
void SendGlasses(int* p,int cnt)
{
	int i,aa;
//		YY[0]='!';
//		for(i=0;i<5;i++)                   //ֱ��ʹ��forѭ�������λ�ô��ҡ�������
//		{
//			YY[i+1]=p[i]+'0';			
//		}
//		YY[i+1]='\0';											//USART_SendData���͵������ݣ����ڵ��������յ������������ݣ��������жϽ��յ���ȴ˳�����
//		printf("%s",YY);									//ʹ��printf�����ݣ����ڽ��յ��Ķ����ַ���   ch = USART_ReceiveData(USART1); ��ʱch=49
		printf("!");
			for(i=0;i<5;i++)                  
		{
			aa=*p++;
			printf("%d%d%d",aa/100,(aa%100)/10,aa%10);    //�������ߵ����Σ����Խ�һ����λ�������ݣ�������һ���ַ�һ���ַ����͵ķ�ʽ
		}
}

/*******************************************************************************
* ������  : SendAngle
* ����    : USART1���۾����͹��ȷ�λ��Ϣ
* ����    : *sָ��
* ���    : ��
* ����    : �� 
* ˵��    : ��
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
		NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=2 ;//��ռ���ȼ�2
		NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2;		//�����ȼ�3
		NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			//IRQͨ��ʹ��
		NVIC_Init(&NVIC_InitStructure);	//����ָ���Ĳ�����ʼ��VIC�Ĵ���
		
		USART_Init(USART3, &USART_InitStructure); 
//		USART_ITConfig(USART3, USART_IT_RXNE, ENABLE);//�������ڽ����ж�
		USART_Cmd(USART3, ENABLE);
}



//ͨ������3����һ���ַ�
//ch �������ַ�
char Usart3Send(char ch)
{
    /* ����һ���ֽ����ݵ�USART3 */
    USART_SendData(USART3, (uint8_t) ch);

    /* �ȴ�������� */
    while (USART_GetFlagStatus(USART3, USART_FLAG_TXE) == RESET);

    return (ch);
}

//ͨ������3����һ���ַ���
//s �������ַ���
void Usart3SendString(char* s)
{
	while(*s)//����ַ���������
	{
		 while (USART_GetFlagStatus(USART3, USART_FLAG_TXE) == RESET);
		USART_SendData(USART3 ,*s++);//���͵�ǰ�ַ�
	}
}



