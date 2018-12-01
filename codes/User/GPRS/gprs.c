#include "stm32f10x.h"
#include "bsp_usart.h"
#include "bsp_SysTick.h"
#include "string.h"
#include "GA6_module_errors.h"
#include "bsp_usart4.h"
#include <stdio.h>
#include "gprs.h"

char *AT_CMGS = "AT+CMGS=71";	
char *pdu_content = "0011000D91685112584368F90008AA3851736CE85BF98C6151FA73B07D27602560C551B5FF0C8BF7901A8FC7624B673A006100700070786E8BA476F24EBA4F4D7F6EFF01FF01FF01";
#define Buf4_Max 	  100 //����2���泤��
#define STABLE_TIMES  10  //�ȴ�ϵͳ�ϵ����ȶ�

/*************	���س�������	**************/

/*************  ���ر�������	**************/
char Uart4_Buf[Buf4_Max];//����2���ջ���
u8 First_Int = 0;
char error_result[20];

char server_ip_port_cmd[45];


/*************	���غ�������	**************/
void CLR_Buf_4(void);     //�������2���ջ���
u8 Wait_CREG(u8 query_times);    //�ȴ�ģ��ע��ɹ�
u8 Find(char *a);

/*************  �ⲿ�����ͱ�������*****************/

int check_status(void)
{
	int ret;
	char esc_char[2];
	
	esc_char[0] = 0x1B;//�˳��ַ�
	esc_char[1] = '\0';
//	
	ret = UART4_Send_AT_Command("AT","OK",5,50);//����ͨ���Ƿ�ɹ�

	if(ret == 0)
	{	
		UART4_SendString(esc_char);//��һ����>״̬����ô�÷���һ��ESC�ַ�
		return COMMUNITE_ERROR;
	}
    ret = UART4_Send_AT_Command("ATE0","OK",2,50);//�رջ��Թ���
	if(ret == 0)
	{
		return EAT0_ERROR;
	}
    
	return 1;
}

/***************************************************************
ע����Ȼ����Է�������ֵ����ȷ����������һ������ָ�����ʧ���ˡ�
****************************************************************/
int send_data_to_server(char *server_IP_and_port,char *content)
{
	u8 ret;
	char end_char[2];
	
	end_char[0] = 0x1A;//�����ַ�
	end_char[1] = '\0';
	
	ret = UART4_Send_AT_Command("AT+CIPSTATUS","CONNECT OK",3,50*2);//��ѯ����״̬
	if(ret == 1)//˵����������������״̬
	{

		ret = UART4_Send_AT_Command("AT+CIPSEND",">",3,50);           //������������
		if(ret == 0)
		{
			return AT_CIPSEND_ERROR;
		}
		
		UART4_SendString(content);
		
		ret = UART4_Send_AT_Command_End(end_char,"SEND OK",1,250);//���ͽ��������ȴ�����ok,�ȴ�5S��һ�Σ���Ϊ��������ʱ����ܽϳ�
		if(ret == 0)
		{
			return END_CHAR_ERROR;
		}
		
		return 1;
	}
	else
	{
		ret = UART4_Send_AT_Command("AT+CGATT=1","OK",3,50*7);//��������
		if(ret == 0)
		{
			return AT_CGATT_ERROR;
		}
		
		ret = UART4_Send_AT_Command("AT+CGACT=1,1","OK",3,50*2);//��������
		if(ret == 0)
		{
			return AT_CGATT1_ERROR;
		}

		memset(server_ip_port_cmd,'\0',45);
		strcpy(server_ip_port_cmd,"AT+CIPSTART=\"TCP\",");
		strcat(server_ip_port_cmd,server_IP_and_port);
		
		ret = UART4_Send_AT_Command(server_ip_port_cmd,"CONNECT OK",3,50*2);//���ӷ�����
		
		if(ret == 0)
		{
			return AT_CIPSTART_ERROR;
		}
		
		delay_ms(50);
//		UART1_SendString(Uart4_Buf);//MY �״����ӣ���ʾ���������������ӳɹ���Ϣ
		
		
		ret = UART4_Send_AT_Command("AT+CIPSEND",">",3,50);//��ʼ��������
		if(ret == 0)
		{
			return AT_CIPSEND_ERROR;
		}
		
		UART4_SendString(content);
		ret = UART4_Send_AT_Command_End(end_char,"SEND OK",1,250);//���ͽ��������ȴ�����ok,�ȴ�5S��һ�Σ���Ϊ��������ʱ����ܽϳ�
			
		if(ret == 0)
		{
			return END_CHAR_ERROR;
		}
		return 1;
	}
}


/*******************************************************************************
* ������  : USART2_IRQHandler
* ����    : ����1�жϷ������
* ����    : ��
* ����    : �� 
* ˵��    : 
*******************************************************************************/
void UART4_IRQHandler(void)                	
{
	u8 Res=0;
	Res = USART_ReceiveData(UART4);
	Uart4_Buf[First_Int] = Res;  	  		//�����յ����ַ����浽������
	First_Int++;                	  		//����ָ������ƶ�
	if(First_Int > Buf4_Max)       	  	//���������,������ָ��ָ�򻺴���׵�ַ
	{
		First_Int = 0;
	}
} 	

/*******************************************************************************
* ������  : TIM2_IRQHandler
* ����    : ��ʱ��2�ж϶Ϸ�����
* ����    : ��
* ���    : ��
* ����    : �� 
* ˵��    : ��
*******************************************************************************/
void TIM2_IRQHandler(void)   //TIM2�ж�
{
	static u8 flag =1;

	if (TIM_GetITStatus(TIM2, TIM_IT_Update) != RESET)  //���TIM2�����жϷ������
	{
		
		TIM_ClearITPendingBit(TIM2, TIM_IT_Update);  //���TIM2�����жϱ�־ 
	
		if(flag)
		{
			//LED4_ON(); 
			flag=0;
		}
		else
		{
			//LED4_OFF(); 
			flag=1;
		}
	}	
}

/*******************************************************************************
* ������ : CLR_Buf
* ����   : �������4��������
* ����   : 
* ���   : 
* ����   : 
* ע��   : 
*******************************************************************************/
void CLR_Buf_4(void)
{
	u16 k;
	for(k=0;k<Buf4_Max;k++)      //��������������
	{
		Uart4_Buf[k] = 0x00;
	}
    First_Int = 0;              //�����ַ�������ʼ�洢λ��
}

/*******************************************************************************
* ������ : Wait_CREG
* ����   : �ȴ�ģ��ע��ɹ�
* ����   : 
* ���   : 
* ����   : 
* ע��   : 
*******************************************************************************/
u8 Wait_CREG(u8 query_times)
{
	u8 i;
	u8 k;
	u8 j;
	i = 0;
	CLR_Buf_4();
	while(i == 0)        			
	{

		UART4_Send_Command("AT+CREG?");
		delay_ms(100); 
		
		for(k=0;k<Buf4_Max;k++)      			
		{
			if((Uart4_Buf[k] == '+')&&(Uart4_Buf[k+1] == 'C')&&(Uart4_Buf[k+2] == 'R')&&(Uart4_Buf[k+3] == 'E')&&(Uart4_Buf[k+4] == 'G')&&(Uart4_Buf[k+5] == ':'))
			{
					 
				if((Uart4_Buf[k+7] == '1')&&((Uart4_Buf[k+9] == '1')||(Uart4_Buf[k+9] == '5')))
				{
					i = 1;
					return 1;
				}
				
			}
		}
		j++;
		if(j > query_times)
		{
			return 0;
		}
		
	}
	return 0;
}

/*******************************************************************************
* ������ : Find
* ����   : �жϻ������Ƿ���ָ�����ַ���
* ����   : 
* ���   : 
* ����   : unsigned char:1 �ҵ�ָ���ַ���0 δ�ҵ�ָ���ַ� 
* ע��   : 
*******************************************************************************/

u8 Find(char *a)
{ 
	if(strstr(Uart4_Buf, a)!=NULL)
	{
		return 1;
	}	
	else
	{
		return 0;
	}
		
}


/*******************************************************************************
* ������ : GPRS_Send_help
* ����   : GPRS_Send_help����
* ����   : 
* ���   : 
* ����   : 
* ע��   : ����2������GA6ģ��ͨ�ţ�����1���ڴ��ڵ��ԣ�
*******************************************************************************/
void GPRS_Send_help(void)
{
	int  ret;
		
	my_printf("GPRS����help��������.......................\r\n");
	
	memset(error_result,'\0',20);
		

	ret = send_data_to_server("\"39.108.110.121\",10001","help");//�������ݵ�������					
	
	if(ret == 1)
	{
		sprintf(error_result,"�ɹ����͵�server\r\n");
		my_printf("%s",error_result);
		my_printf("�յ��ظ���\r\n");  //received:��������ݲ��ǻظ�����������
		my_printf("%s",Uart4_Buf);
		my_printf("\r\n");
	} 
	else //���ݷ���ʧ�ܣ���ʱ��������ʱ��������µ�ʧ�ܣ������ͶϿ����ӣ�Ȼ��Ϳ��Լ�����������
	{
		sprintf(error_result,"������� : %d\r\n",ret);
		my_printf("%s",error_result);
		UART4_Send_AT_Command("AT+CIPCLOSE","OK",3,150);//�ر�����
	}
}

void GPRS_Send_GPS(float lo, float la)
{
	int  ret;
	char sendData[30]; //��Ҫ���͵�����
		
	my_printf("GPRS�������깦������.......................\r\n");	
	
	memset(error_result,'\0',20);
	memset(sendData,'\0',30);

		sprintf(sendData,"help!lo:%f,la:%f\r\n",lo, la);//��Ҫ���͵�����
		ret = send_data_to_server("\"39.108.110.121\",10001",sendData);          //�������ݵ�������					

	
	if(ret == 1)
	{
		sprintf(error_result,"�ɹ����͵�server\r\n");
				my_printf("%s",error_result);
		my_printf("�յ��ظ���\r\n");  //received:��������ݲ��ǻظ�����������
		my_printf("%s",Uart4_Buf);
		my_printf("\r\n");
	}
	else //���ݷ���ʧ�ܣ���ʱ��������ʱ��������µ�ʧ�ܣ������ͶϿ����ӣ�Ȼ��Ϳ��Լ�����������
	{
		sprintf(error_result,"������� : %d\r\n",ret);
		my_printf("%s",error_result);
		UART4_Send_AT_Command("AT+CIPCLOSE","OK",3,150);//�ر�����
	}
}

//������Ϣ����
int send_pdu_message(char *content)
{
	int ret;
	char end_char[2];
	
	end_char[0] = 0x1A;
	end_char[1] = '\0';
	
	//���ô洢λ�ã���������ֵ�ж���
	ret = UART4_Send_AT_Command("AT+CPMS=\"SM\",\"ME\",\"SM\"","OK",3,100);
	if(ret == 0)
	{
		return AT_CPMS_ERROR;
	}
		
	ret = UART4_Send_AT_Command("AT+CMGF=0","OK",3,50);//����ΪPDUģʽ
	if(ret == 0)
	{
		return AT_CMGF_ERROR;
	}
	
	ret = UART4_Send_AT_Command(AT_CMGS,">",3,50);//�����ַ�����ָ��
	
	if(ret == 0)
	{
		return AT_CMGS_ERROR;
	}
	
	UART4_SendString(content);
	ret = UART4_Send_AT_Command_End(end_char,"OK",3,250);//���ͽ��������ȴ�����ok,�ȴ�5S��һ�Σ���Ϊ���ųɹ����͵�״̬ʱ��Ƚϳ�
	if(ret == 0)
	{
		return END_CHAR_ERROR;
	}
	
	return 1;
}

/*
 * ��������TIM6_TIM_NVIC_Config
 * ����  �����ö�ʱ��6�����ȼ������ȼ�����ʹ��NVIC_PriorityGroup_2
 * ����  ����
 * ���  ����	
 */
void TIM6_TIM_NVIC_Config(void)
{
    NVIC_InitTypeDef NVIC_InitStructure; 
    // �����ж���Ϊ0
    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);		
		// �����ж���Դ
    NVIC_InitStructure.NVIC_IRQChannel = TIM6_IRQn ;	
		// ���������ȼ�Ϊ 0
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;	 
	  // ������ռ���ȼ�Ϊ3
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;	
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);
}

/*
 * ע�⣺TIM_TimeBaseInitTypeDef�ṹ��������5����Ա��TIM6��TIM7�ļĴ�������ֻ��
 * TIM_Prescaler��TIM_Period������ʹ��TIM6��TIM7��ʱ��ֻ���ʼ����������Ա���ɣ�
 * ����������Ա��ͨ�ö�ʱ���͸߼���ʱ������.
 *-----------------------------------------------------------------------------
 *typedef struct
 *{ TIM_Prescaler            ����
 *	TIM_CounterMode			     TIMx,x[6,7]û�У���������
 *  TIM_Period               ����
 *  TIM_ClockDivision        TIMx,x[6,7]û�У���������
 *  TIM_RepetitionCounter    TIMx,x[1,8,15,16,17]����
 *}TIM_TimeBaseInitTypeDef; 
 *-----------------------------------------------------------------------------
 */

/*
 * ��������TIM6_TIM_Mode_Config
 * ����  ����ʼ����ʱ��6
 * ����  ����
 * ���  ����	
 */
void TIM6_TIM_Mode_Config(void)
{
    TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
		
		// ������ʱ��ʱ��,���ڲ�ʱ��CK_INT=72M
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM6, ENABLE);
	
		// �Զ���װ�ؼĴ�����ֵ���ۼ�TIM_Period+1��Ƶ�ʺ����һ�����»����жϣ�һ���ж�ʱ��Ϊ1us��
    TIM_TimeBaseStructure.TIM_Period = 999;	

	  // ʱ��Ԥ��Ƶ��Ϊ
    TIM_TimeBaseStructure.TIM_Prescaler= 71;
	
	  // ��ʼ����ʱ��
    TIM_TimeBaseInit(TIM6, &TIM_TimeBaseStructure);
		
		// ����������жϱ�־λ
    TIM_ClearFlag(TIM6, TIM_FLAG_Update);
	  
		// �����������ж�
    TIM_ITConfig(TIM6,TIM_IT_Update,ENABLE);
		
		// ʹ�ܼ�����
    TIM_Cmd(TIM6, ENABLE);	
		
		TIM6_TIM_NVIC_Config();
}

