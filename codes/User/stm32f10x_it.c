/**
  ******************************************************************************
  * @file    Project/STM32F10x_StdPeriph_Template/stm32f10x_it.c
  * @author  MCD Application Team
  * @version V3.5.0
  * @date    08-April-2011
  * @brief   Main Interrupt Service Routines.
  *          This file provides template for all exceptions handler and
  *          peripherals interrupt service routine.
  ******************************************************************************
  * @attention
  *
  * THE PRESENT FIRMWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS
  * WITH CODING INFORMATION REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE
  * TIME. AS A RESULT, STMICROELECTRONICS SHALL NOT BE HELD LIABLE FOR ANY
  * DIRECT, INDIRECT OR CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING
  * FROM THE CONTENT OF SUCH FIRMWARE AND/OR THE USE MADE BY CUSTOMERS OF THE
  * CODING INFORMATION CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
  *
  * <h2><center>&copy; COPYRIGHT 2011 STMicroelectronics</center></h2>
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "stm32f10x_it.h"
#include "bsp_usart.h"
#include <stdio.h>
#include "waveConfig.h"
#include "UltrasonicWave.h"
#include "debug.h"
#include "gps.h" 
#include "gprs.h"
#include "shake.h"

int HelpFlag=0;
extern GPSData SendGPS;
extern void TimingDelay_Decrement(void);
extern uint8_t direction_flag;
extern _SaveData Save_Data;
extern int8_t  MEASURE_FLAG;          //��������־
extern uint8_t gprs_flag;             //gprs�������ݱ�־λ
int time= 0;
////////���Կ���//////////////
#ifdef DEBUG_ON_OFF 
#undef  DEBUG_ON_OFF
#define DEBUG_ON_OFF 0
#endif
//////////////////////////////

/** @addtogroup STM32F10x_StdPeriph_Template
  * @{
  */

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

/******************************************************************************/
/*            Cortex-M3 Processor Exceptions Handlers                         */
/******************************************************************************/

/**
  * @brief  This function handles NMI exception.
  * @param  None
  * @retval None
  */
void NMI_Handler(void)
{
}

/**
  * @brief  This function handles Hard Fault exception.
  * @param  None
  * @retval None
  */
void HardFault_Handler(void)
{
    /* Go to infinite loop when Hard Fault exception occurs */
  printf("\r\n hardfault err\r\n");
    while (1)
    {
    }
}

/**
  * @brief  This function handles Memory Manage exception.
  * @param  None
  * @retval None
  */
void MemManage_Handler(void)
{
    /* Go to infinite loop when Memory Manage exception occurs */
    while (1)
    {
    }
}

/**
  * @brief  This function handles Bus Fault exception.
  * @param  None
  * @retval None
  */
void BusFault_Handler(void)
{
    /* Go to infinite loop when Bus Fault exception occurs */
    while (1)
    {
    }
}

/**
  * @brief  This function handles Usage Fault exception.
  * @param  None
  * @retval None
  */
void UsageFault_Handler(void)
{
    /* Go to infinite loop when Usage Fault exception occurs */
    while (1)
    {
    }
}

/**
  * @brief  This function handles SVCall exception.
  * @param  None
  * @retval None
  */
void SVC_Handler(void)
{
}

/**
  * @brief  This function handles Debug Monitor exception.
  * @param  None
  * @retval None
  */
void DebugMon_Handler(void)
{
}

/**
  * @brief  This function handles PendSVC exception.
  * @param  None
  * @retval None
  */
void PendSV_Handler(void)
{
}

/**
  * @brief  This function handles SysTick Handler.
  * @param  None
  * @retval None
  */
extern void SysTickDelayTime_Counter(void);

void SysTick_Handler(void)
{
	SysTickDelayTime_Counter();
}

/******************************************************************************/
/*                 STM32F10x Peripherals Interrupt Handlers                   */
/*  Add here the Interrupt Handler for the used peripheral(s) (PPP), for the  */
/*  available peripheral interrupt handler's name please refer to the startup */
/*  file (startup_stm32f10x_xx.s).                                            */
/******************************************************************************/

/**
  * @brief  USART1_IRQHandler,���ڽ��������Ϣ�봫�������ź�
  * @param  None
  * @retval None
  */


void USART1_IRQHandler(void)
{
	u8 Res;
	float lo, la;			//���GPS����
	lo=22.2, la=33.3;
	if(USART_GetITStatus(USART1, USART_IT_RXNE) != RESET) 
	{
		my_printf("***************\r\n");
		Res =USART_ReceiveData(USART1);						//��ȡ���յ�������
		if(Res == '!')
		{
				HelpFlag=1;                   //�������ж����洦������ĺ���
				my_printf("�յ�������Ϣ\r\n");
		}
		else 
		if(Res == '#')               //���յ���ȡ������Ϣ���ź�
		{
			MEASURE_FLAG=1;
		}
		else
		{
			my_printf("Res = %c\r\n",Res);			
			AdjustVibrationFrequencyWalking( Res );  			
		}
 
	}
}


void GENERAL1_TIM_INT_FUN(void)
{
	// ��Ҫ��������źŵ����ڴ��ڶ�ʱ�������ʱʱ����ʱ���ͻ���������������ж�
	// ���ʱ��������Ҫ�������Ķ�ʱ���ڼӵ������źŵ�ʱ������ȥ
	if ( TIM_GetITStatus ( GENERAL1_TIM, TIM_IT_Update) != RESET )               
	{	
		TIM_ICUserValueStructure[0].Capture_CcrValue += GENERAL1_TIM_PERIOD;
		TIM_ICUserValueStructure[1].Capture_CcrValue += GENERAL1_TIM_PERIOD;	
		TIM_ICUserValueStructure[2].Capture_CcrValue += GENERAL1_TIM_PERIOD;	
		TIM_ICUserValueStructure[3].Capture_CcrValue += GENERAL1_TIM_PERIOD;			
		TIM_ClearITPendingBit ( GENERAL1_TIM, TIM_FLAG_Update ); 		
	}

//��һ
	if ( TIM_GetITStatus (GENERAL1_TIM, GENERAL1_TIM_IT_CC1 ) != RESET)
	{
		// ��һ�β���
		if ( TIM_ICUserValueStructure[0].Capture_StartFlag == 0 )
		{
      // �沶��ȽϼĴ�����ֵ�ı�����ֵ��0			
			TIM_ICUserValueStructure[0].Capture_CcrValue = 0;
            TIM_ICUserValueStructure[0].Capture_First  = GENERAL1_TIM_GetCapture1_FUN (GENERAL1_TIM);
			// ����һ�β���������֮�󣬾ͰѲ����������Ϊ�½���
			GENERAL1_TIM_OC1PolarityConfig_FUN(GENERAL1_TIM, TIM_ICPolarity_Falling);
      // ��ʼ�����׼��1			
			TIM_ICUserValueStructure[0].Capture_StartFlag = 1;			
		}
		// �½��ز����ж�
		else // �ڶ��β���
		{
			// ��ȡ����ȽϼĴ�����ֵ�����ֵ���ǲ��񵽵ĸߵ�ƽ��ʱ���ֵ
			TIM_ICUserValueStructure[0].Capture_CcrValue += GENERAL1_TIM_GetCapture1_FUN (GENERAL1_TIM) - TIM_ICUserValueStructure[0].Capture_First;

			// ���ڶ��β����½���֮�󣬾ͰѲ����������Ϊ�����أ��ÿ����µ�һ�ֲ���
			GENERAL1_TIM_OC1PolarityConfig_FUN(GENERAL1_TIM, TIM_ICPolarity_Rising);
      // ��ʼ�����־��0		
			TIM_ICUserValueStructure[0].Capture_StartFlag = 0;
      // ������ɱ�־��1			
			TIM_ICUserValueStructure[0].Capture_FinishFlag = 1;		
		}

		TIM_ClearITPendingBit (GENERAL1_TIM,GENERAL1_TIM_IT_CC1);	    
	}	

//�ڶ�
	if ( TIM_GetITStatus (GENERAL1_TIM, GENERAL1_TIM_IT_CC2 ) != RESET)
	{
		// ��һ�β���
		if ( TIM_ICUserValueStructure[1].Capture_StartFlag == 0 )
		{
      // �沶��ȽϼĴ�����ֵ�ı�����ֵ��0			
			TIM_ICUserValueStructure[1].Capture_CcrValue = 0;
            TIM_ICUserValueStructure[1].Capture_First  = GENERAL1_TIM_GetCapture2_FUN (GENERAL1_TIM);
			// ����һ�β���������֮�󣬾ͰѲ����������Ϊ�½���
			GENERAL1_TIM_OC2PolarityConfig_FUN(GENERAL1_TIM, TIM_ICPolarity_Falling);
      // ��ʼ�����׼��1			
			TIM_ICUserValueStructure[1].Capture_StartFlag = 1;			
		}
		// �½��ز����ж�
		else // �ڶ��β���
		{
			// ��ȡ����ȽϼĴ�����ֵ�����ֵ���ǲ��񵽵ĸߵ�ƽ��ʱ���ֵ
			TIM_ICUserValueStructure[1].Capture_CcrValue += GENERAL1_TIM_GetCapture2_FUN (GENERAL1_TIM) - TIM_ICUserValueStructure[1].Capture_First;

			// ���ڶ��β����½���֮�󣬾ͰѲ����������Ϊ�����أ��ÿ����µ�һ�ֲ���
			GENERAL1_TIM_OC2PolarityConfig_FUN(GENERAL1_TIM, TIM_ICPolarity_Rising);
      // ��ʼ�����־��0		
			TIM_ICUserValueStructure[1].Capture_StartFlag = 0;
      // ������ɱ�־��1			
			TIM_ICUserValueStructure[1].Capture_FinishFlag = 1;		
		}

		TIM_ClearITPendingBit (GENERAL1_TIM,GENERAL1_TIM_IT_CC2);	    
	}

//����
	if ( TIM_GetITStatus (GENERAL1_TIM, GENERAL1_TIM_IT_CC3 ) != RESET)
	{
		// ��һ�β���
		if ( TIM_ICUserValueStructure[2].Capture_StartFlag == 0 )
		{
      // �沶��ȽϼĴ�����ֵ�ı�����ֵ��0			
			TIM_ICUserValueStructure[2].Capture_CcrValue = 0;
            TIM_ICUserValueStructure[2].Capture_First  = GENERAL1_TIM_GetCapture3_FUN (GENERAL1_TIM);
			// ����һ�β���������֮�󣬾ͰѲ����������Ϊ�½���
			GENERAL1_TIM_OC3PolarityConfig_FUN(GENERAL1_TIM, TIM_ICPolarity_Falling);
      // ��ʼ�����׼��1			
			TIM_ICUserValueStructure[2].Capture_StartFlag = 1;			
		}
		// �½��ز����ж�
		else // �ڶ��β���
		{
			// ��ȡ����ȽϼĴ�����ֵ�����ֵ���ǲ��񵽵ĸߵ�ƽ��ʱ���ֵ
			TIM_ICUserValueStructure[2].Capture_CcrValue += GENERAL1_TIM_GetCapture3_FUN (GENERAL1_TIM) - TIM_ICUserValueStructure[2].Capture_First;

			// ���ڶ��β����½���֮�󣬾ͰѲ����������Ϊ�����أ��ÿ����µ�һ�ֲ���
			GENERAL1_TIM_OC3PolarityConfig_FUN(GENERAL1_TIM, TIM_ICPolarity_Rising);
      // ��ʼ�����־��0		
			TIM_ICUserValueStructure[2].Capture_StartFlag = 0;
      // ������ɱ�־��1			
			TIM_ICUserValueStructure[2].Capture_FinishFlag = 1;			
		}

		TIM_ClearITPendingBit (GENERAL1_TIM,GENERAL1_TIM_IT_CC3);	    
	}

//����
	if ( TIM_GetITStatus (GENERAL1_TIM, GENERAL1_TIM_IT_CC4 ) != RESET)
	{
		// ��һ�β���
		if ( TIM_ICUserValueStructure[3].Capture_StartFlag == 0 )
		{
      // �沶��ȽϼĴ�����ֵ�ı�����ֵ��0			
			TIM_ICUserValueStructure[3].Capture_CcrValue = 0;
            TIM_ICUserValueStructure[3].Capture_First  = GENERAL1_TIM_GetCapture4_FUN (GENERAL1_TIM);
			// ����һ�β���������֮�󣬾ͰѲ����������Ϊ�½���
			GENERAL1_TIM_OC4PolarityConfig_FUN(GENERAL1_TIM, TIM_ICPolarity_Falling);
      // ��ʼ�����׼��1			
			TIM_ICUserValueStructure[3].Capture_StartFlag = 1;			
		}
		// �½��ز����ж�
		else // �ڶ��β���
		{
			// ��ȡ����ȽϼĴ�����ֵ�����ֵ���ǲ��񵽵ĸߵ�ƽ��ʱ���ֵ
			TIM_ICUserValueStructure[3].Capture_CcrValue += GENERAL1_TIM_GetCapture4_FUN (GENERAL1_TIM) - TIM_ICUserValueStructure[3].Capture_First;

			// ���ڶ��β����½���֮�󣬾ͰѲ����������Ϊ�����أ��ÿ����µ�һ�ֲ���
			GENERAL1_TIM_OC4PolarityConfig_FUN(GENERAL1_TIM, TIM_ICPolarity_Rising);
      // ��ʼ�����־��0		
			TIM_ICUserValueStructure[3].Capture_StartFlag = 0;
      // ������ɱ�־��1			
			TIM_ICUserValueStructure[3].Capture_FinishFlag = 1;		
		}

		TIM_ClearITPendingBit (GENERAL1_TIM,GENERAL1_TIM_IT_CC4);	    
	}	
}


void GENERAL2_TIM_INT_FUN(void)
{
	// ��Ҫ��������źŵ����ڴ��ڶ�ʱ�������ʱʱ����ʱ���ͻ���������������ж�
	// ���ʱ��������Ҫ�������Ķ�ʱ���ڼӵ������źŵ�ʱ������ȥ
	if ( TIM_GetITStatus ( GENERAL2_TIM, TIM_IT_Update) != RESET )               
	{	
		TIM_ICUserValueStructure[4].Capture_CcrValue += GENERAL2_TIM_PERIOD;
        if(  TIM_ICUserValueStructure[4].Capture_StartFlag != 0 ) 		
			p_debug("capture 4:over");		
		TIM_ClearITPendingBit ( GENERAL2_TIM, TIM_FLAG_Update ); 		
	}

//��һ
	if ( TIM_GetITStatus (GENERAL2_TIM, GENERAL1_TIM_IT_CC3 ) != RESET)
	{
		// ��һ�β���
		if ( TIM_ICUserValueStructure[4].Capture_StartFlag == 0 )
		{
      // �沶��ȽϼĴ�����ֵ�ı�����ֵ��0			
			TIM_ICUserValueStructure[4].Capture_CcrValue = 0;
            TIM_ICUserValueStructure[4].Capture_First  = GENERAL2_TIM_GetCapture3_FUN (GENERAL2_TIM);
			// ����һ�β���������֮�󣬾ͰѲ����������Ϊ�½���
			GENERAL2_TIM_OC3PolarityConfig_FUN(GENERAL2_TIM, TIM_ICPolarity_Falling);
      // ��ʼ�����׼��1			
			TIM_ICUserValueStructure[4].Capture_StartFlag = 1;			
		}
		// �½��ز����ж�
		else // �ڶ��β���
		{
			// ��ȡ����ȽϼĴ�����ֵ�����ֵ���ǲ��񵽵ĸߵ�ƽ��ʱ���ֵ
			TIM_ICUserValueStructure[4].Capture_CcrValue += GENERAL2_TIM_GetCapture3_FUN (GENERAL2_TIM) - TIM_ICUserValueStructure[4].Capture_First;

			// ���ڶ��β����½���֮�󣬾ͰѲ����������Ϊ�����أ��ÿ����µ�һ�ֲ���
			GENERAL2_TIM_OC3PolarityConfig_FUN(GENERAL2_TIM, TIM_ICPolarity_Rising);
      // ��ʼ�����־��0		
			TIM_ICUserValueStructure[4].Capture_StartFlag = 0;
      // ������ɱ�־��1			
			TIM_ICUserValueStructure[4].Capture_FinishFlag = 1;		
//			printf ( "5\r\n��%d us\r\n", TIM_ICUserValueStructure[4].Capture_CcrValue);
		}

		TIM_ClearITPendingBit (GENERAL2_TIM,GENERAL2_TIM_IT_CC3);	    
	}	
}

#if 0
void TIM5_IRQHandler(void)
{
	int i;
	extern int8_t  MEASURE_FLAG;   // 1 �ɼ�����
	
	static int portNum = 0;      //ѡ����ͨ��
	
	if ( TIM_GetITStatus( TIM5, TIM_IT_Update) != RESET ) 
	{			
		p_debug("tim5");
		
#ifndef ONLY_WALKINGSTICK               //�۾�+����		
		if( MEASURE_FLAG)
		{	
			
			
			UltrasonicWave(portNum);    //�ɼ�һ��ģ������
			portNum++;
			if( portNum > ULTR_NUM)   //������ģ�����ݲɼ����
			{
			    portNum = 0; 	
				SendGlasses(UltrasonicWave_Distance,ULTR_NUM);           //�������� 	
				MEASURE_FLAG = 0;
							
			}
		}

#else
/****************���ȵ�������************************/
		UltrasonicWave(portNum);    //�ɼ�һ��ģ������
		portNum++;
		if( portNum > ULTR_NUM)   //������ģ�����ݲɼ����
		{
			portNum = 0; 
		}
#endif
		
		TIM_ClearITPendingBit(TIM5 , TIM_FLAG_Update);  		 
	}		
	
}

#else  /*************************����ѯ*********************************/
void TIM5_IRQHandler(void)
{
	
	if ( TIM_GetITStatus( TIM5, TIM_IT_Update) != RESET ) 
	{			
#ifndef ONLY_WALKINGSTICK               //�۾�+����		
		if( MEASURE_FLAG)
		{	
			UltrasonicWave(0);    //�ɼ�һ��ģ������
//			portNum++;
//			if( portNum > ULTR_NUM)   //������ģ�����ݲɼ����
//			{
//			    portNum = 0; 	
//				SendGlasses(UltrasonicWave_Distance,ULTR_NUM);           //�������� 	
//				MEASURE_FLAG = 0;
//							
//			}
			MEASURE_FLAG = 0;		
		}
		if( IsFinishMeasure() )   //������ģ�����ݲɼ����
		{
			p_debug("finish measure");
			SendGlasses(UltrasonicWave_Distance,ULTR_NUM);           //�������� 		
		}	
//		if( IsFinishMeasure() )   //������ģ�����ݲɼ����
//		{
//			SendGlasses(UltrasonicWave_Distance,ULTR_NUM);           //�������� 	
//			MEASURE_FLAG = 0;			
//		}
#else
/****************���ȵ�������************************/
		UltrasonicWave(0);    //�ɼ�һ��ģ������
//		portNum++;
//		if( portNum > ULTR_NUM)   //������ģ�����ݲɼ����
//		{
//			portNum = 0; 
//		}
#endif
		
		TIM_ClearITPendingBit(TIM5 , TIM_FLAG_Update);  		 
	}		
	
}

#endif


static void delay_t(u32 time)
{
  while(time--);
}

/**
  * @brief  �ⲿ�ж�0�����ڼ�ⰴ��
  * @param  None
  * @retval None
  */

void EXTI0_IRQHandler(void)
{
	if(EXTI_GetITStatus(EXTI_Line0) != RESET) //ȷ���Ƿ������EXTI Line�ж�
	{	
			delay_t(100);
		if( GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_0) == 1) 
		   {
				 direction_flag=1;												//��������־λ���б��
			 }
		EXTI_ClearITPendingBit(EXTI_Line0);     //����жϱ�־λ
	}  
}


/**
  * @brief  ����2�жϣ�����gps�����жϵĹ����о��Ѿ��ڲɼ�������
  * @param  None
  * @retval None
  */

void USART2_IRQHandler(void)                	//����2�жϷ������
{
	u8 Res;
	if(USART_GetITStatus(USART2, USART_IT_RXNE) != RESET) 
	{
		Res =USART_ReceiveData(USART2);						//��ȡ���յ�������
	
	if(Res == '$')
	{
		point1 = 0;	
	}

	  USART_RX_BUF[point1++] = Res;

	if(USART_RX_BUF[0] == '$' && USART_RX_BUF[4] == 'M' && USART_RX_BUF[5] == 'C')			//ȷ���Ƿ��յ�"GPRMC/GNRMC"��һ֡����
	{
		if(Res == '\n')									   
		{
			memset(Save_Data.GPS_Buffer, 0, GPS_Buffer_Length);      //��GPS���ݳ�ʼ��
			memcpy(Save_Data.GPS_Buffer, USART_RX_BUF, point1); 		 //����ȡ�����ݱ��浽GPS������
			Save_Data.isGetData = true;
			point1 = 0;
			memset(USART_RX_BUF, 0, USART_REC_LEN);      						 //�����ڽ����������				
		}	
	}
	
	if(point1 >= USART_REC_LEN)
	{
		point1 = USART_REC_LEN;
	}	 		 
   } 
}
/**
  * @brief  ��ʱ��6�жϣ����ڼ�ʱ��������gprs��Ϣÿ10�뷢��һ�Ρ�
  * @param  None
  * @retval None
  */
void  TIM6_IRQHandler (void)
{
	if ( TIM_GetITStatus( TIM6, TIM_IT_Update) != RESET ) 
	{	
		time++;
		if(time == 10000 )
		{
			time = 0;
			gprs_flag = 1;
		}
		TIM_ClearITPendingBit(TIM6 , TIM_FLAG_Update);  		 
	}		 	
}
/**
  * @brief  This function handles PPP interrupt request.
  * @param  None
  * @retval None
  */
/*void PPP_IRQHandler(void)
{
}*/

/******************* (C) COPYRIGHT 2011 STMicroelectronics *****END OF FILE****/
