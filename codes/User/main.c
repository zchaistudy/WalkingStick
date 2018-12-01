/**
  ******************************************************************************
  * @file    main.c
  * @author  chzh
  * @version V1.0
  * @date    2018-10-14
  * @brief   ���ܹ���������
  ******************************************************************************
	*/
#include "UltrasonicWave.h"
#include "bsp_delay.h"
#include "stm32f10x.h"
#include "bsp_usart.h"
#include <string.h>
#include "bsp_SysTick.h"
#include "bsp_angle.h"
#include "bsp_exti.h"
#include "gps.h"
#include "bsp_usart4.h"
#include "gprs.h"
#include "bsp_usart.h"
#include "debug.h"
#include "shake.h"

extern char *pdu_content ;
extern int8_t  MEASURE_FLAG;
uint8_t direction_flag;
uint8_t gprs_flag;
extern int UltrasonicWave_Distance[];     
extern int HelpFlag;
extern GPSData SendGPS;

//GPRS��������ʼ������
static void Periph_GPRS_Init()
{
	UART4_Init_Config(115200);					//GPRSʹ�ô���4��ʼ��
	SysTick_Init_Config();							//�δ�ʱ����ʼ��������GPRS��ʱ��ʱ
}

void delay_0(u32 time)
{
  while(time--);
}

int main(void)
{
	int angle;
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);       //�ж����ȼ�����ͳһʹ��NVIC_PriorityGroup_2
	
	MEASURE_FLAG=0;                              //�����Ȼ�ô��۾������Ĺ��ȳ�������������ʱ�����˹��ȳ�������־λ��1�����ȿ�ʼ��ȡ�����ϳ��������ݣ������͸��۾���
  usart3Config();                              //����3��ʼ�������ڵ���
	USART1_Config(38400);                        //����1��ʼ��,�������۾�ͨѶ
	I2C_GPIO_Config();									         //I2CͨѶ�ĳ�ʼ�������ڷ�λģ��
  InitLSM303D();											         //��λģ��ĳ�ʼ��
	WaveConfig();
	USART2_Config();										         //����2������gps
	EXTI_PA0_Config(); 									         //��λ�����������¸ð����󣬽����ⲿ�жϣ���ȡ��λģ�����ݣ��������жϳ�ʼ��
	Periph_GPRS_Init();	                         //GPRS��������ʼ������,ͬʱ��ʼ���˵δ�ʱ�����ڼ�ʱ
	TIM2_Breathing_Init();                       //��ʱ��2��ʼ����ʹ��PWM������ģ�����Ƶ��
//	TIM6_TIM_Mode_Config();										 //��ʱ��6��ʼ�������ڼ�ʱ��һ��ʼ�ƻ�Ϊ��ʱ��ȡGPS�źţ����Ƿ�����Ҫת�ز���ϵͳ������ɣ�
	my_printf("ϵͳ����......");
	 while(1)                                    
		 
	{
//			parseGpsBuffer();		                   //��ȡgps��Ϣ���ڿ���̨��ӡλ����Ϣ		
//�����·�λ��ť�����ⲿ�жϺ󣬽�direction_flag��1����ʱ���Ի�ȡ��λ��Ϣ		
			if(direction_flag)										   
			{
					angle=getAngle();
					direction_flag=0;
					my_printf("��ǰ��λΪ��");
					my_printf("%c",angle);
				  SendAngle(angle);										//��������ģ��λ���۾��ϣ��ʽ���λ��Ϣ���͸��۾�
			}
			if(HelpFlag)
			{
				GPRS_Send_GPS(SendGPS.lo, SendGPS.la);	//ʹ��GPRS���͵�ǰλ������
//					GPRS_Send_help();					//ʹ��GPRS��������ź�  ����һ��ʱ�����ʱ�����Ե������ݲ���ܿ챻���յ�
					HelpFlag=0;
			}
			if( IsFinishMeasure() )   //������ģ�����ݲɼ����
			{
				SendGlasses(UltrasonicWave_Distance,ULTR_NUM);           //�������� 	
				MEASURE_FLAG = 0;			
			}

	}
}

/*********************************************END OF FILE**********************/


