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
#include "bsp_usart1.h"
#include <stdio.h>
#include "waveConfig.h"
#include "UltrasonicWave.h"
#include "debug.h"
#include "gps.h" 
#include "gprs.h"

extern GPSData SendGPS;
extern void TimingDelay_Decrement(void);
extern uint8_t direction_flag;
extern _SaveData Save_Data;
extern int8_t  MEASURE_FLAG;          //测距请求标志
////////调试开关//////////////
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
  * @brief  USART1_IRQHandler,用于接收求救信息
  * @param  None
  * @retval None
  */


void USART1_IRQHandler(void)
{
	u8 Res;
	float lo, la;			//存放GPS数据
	lo=22.2, la=33.3;
	if(USART_GetITStatus(USART1, USART_IT_RXNE) != RESET) 
	{
		Res =USART_ReceiveData(USART1);						//读取接收到的数据
		printf("收到获取数据请求\r\n");
		if(Res == '1')
		{
			UART1_SendString("救命1.......................\r\n");       //替换成相应的呼救函数
						//发送短信
			GPRS_Send_help();	//使用GPRS发送求救信号
			GPRS_Send_GPS(SendGPS.lo, SendGPS.la);	//使用GPRS发送当前位置坐标
		}
		else if(Res == '2')
		{
			UART1_SendString("救命2.......................\r\n");       //替换成相应的呼救函数
						GPRS_Send_help();	//使用GPRS发送求救信号
//			GPRS_Send_GPS(SendGPS.lo, SendGPS.la);	//使用GPRS发送当前位置坐标
		}
		else if(Res == '3')               //接收到获取数据信息的信号
		{
			MEASURE_FLAG=1;
		}

	}

}







void GENERAL1_TIM_INT_FUN(void)
{
	// 当要被捕获的信号的周期大于定时器的最长定时时，定时器就会溢出，产生更新中断
	// 这个时候我们需要把这个最长的定时周期加到捕获信号的时间里面去
	if ( TIM_GetITStatus ( GENERAL1_TIM, TIM_IT_Update) != RESET )               
	{	
		TIM_ICUserValueStructure[0].Capture_CcrValue += GENERAL1_TIM_PERIOD;
		TIM_ICUserValueStructure[1].Capture_CcrValue += GENERAL1_TIM_PERIOD;	
		TIM_ICUserValueStructure[2].Capture_CcrValue += GENERAL1_TIM_PERIOD;	
		TIM_ICUserValueStructure[3].Capture_CcrValue += GENERAL1_TIM_PERIOD;			
		TIM_ClearITPendingBit ( GENERAL1_TIM, TIM_FLAG_Update ); 		
	}

//第一
	if ( TIM_GetITStatus (GENERAL1_TIM, GENERAL1_TIM_IT_CC1 ) != RESET)
	{
		// 第一次捕获
		if ( TIM_ICUserValueStructure[0].Capture_StartFlag == 0 )
		{
      // 存捕获比较寄存器的值的变量的值清0			
			TIM_ICUserValueStructure[0].Capture_CcrValue = 0;
            TIM_ICUserValueStructure[0].Capture_First  = GENERAL1_TIM_GetCapture1_FUN (GENERAL1_TIM);
			// 当第一次捕获到上升沿之后，就把捕获边沿配置为下降沿
			GENERAL1_TIM_OC1PolarityConfig_FUN(GENERAL1_TIM, TIM_ICPolarity_Falling);
      // 开始捕获标准置1			
			TIM_ICUserValueStructure[0].Capture_StartFlag = 1;			
		}
		// 下降沿捕获中断
		else // 第二次捕获
		{
			// 获取捕获比较寄存器的值，这个值就是捕获到的高电平的时间的值
			TIM_ICUserValueStructure[0].Capture_CcrValue += GENERAL1_TIM_GetCapture1_FUN (GENERAL1_TIM) - TIM_ICUserValueStructure[0].Capture_First;

			// 当第二次捕获到下降沿之后，就把捕获边沿配置为上升沿，好开启新的一轮捕获
			GENERAL1_TIM_OC1PolarityConfig_FUN(GENERAL1_TIM, TIM_ICPolarity_Rising);
      // 开始捕获标志清0		
			TIM_ICUserValueStructure[0].Capture_StartFlag = 0;
      // 捕获完成标志置1			
			TIM_ICUserValueStructure[0].Capture_FinishFlag = 1;		
		}

		TIM_ClearITPendingBit (GENERAL1_TIM,GENERAL1_TIM_IT_CC1);	    
	}	

//第二
	if ( TIM_GetITStatus (GENERAL1_TIM, GENERAL1_TIM_IT_CC2 ) != RESET)
	{
		// 第一次捕获
		if ( TIM_ICUserValueStructure[1].Capture_StartFlag == 0 )
		{
      // 存捕获比较寄存器的值的变量的值清0			
			TIM_ICUserValueStructure[1].Capture_CcrValue = 0;
            TIM_ICUserValueStructure[1].Capture_First  = GENERAL1_TIM_GetCapture2_FUN (GENERAL1_TIM);
			// 当第一次捕获到上升沿之后，就把捕获边沿配置为下降沿
			GENERAL1_TIM_OC2PolarityConfig_FUN(GENERAL1_TIM, TIM_ICPolarity_Falling);
      // 开始捕获标准置1			
			TIM_ICUserValueStructure[1].Capture_StartFlag = 1;			
		}
		// 下降沿捕获中断
		else // 第二次捕获
		{
			// 获取捕获比较寄存器的值，这个值就是捕获到的高电平的时间的值
			TIM_ICUserValueStructure[1].Capture_CcrValue += GENERAL1_TIM_GetCapture2_FUN (GENERAL1_TIM) - TIM_ICUserValueStructure[1].Capture_First;

			// 当第二次捕获到下降沿之后，就把捕获边沿配置为上升沿，好开启新的一轮捕获
			GENERAL1_TIM_OC2PolarityConfig_FUN(GENERAL1_TIM, TIM_ICPolarity_Rising);
      // 开始捕获标志清0		
			TIM_ICUserValueStructure[1].Capture_StartFlag = 0;
      // 捕获完成标志置1			
			TIM_ICUserValueStructure[1].Capture_FinishFlag = 1;		
		}

		TIM_ClearITPendingBit (GENERAL1_TIM,GENERAL1_TIM_IT_CC2);	    
	}

//第三
	if ( TIM_GetITStatus (GENERAL1_TIM, GENERAL1_TIM_IT_CC3 ) != RESET)
	{
		// 第一次捕获
		if ( TIM_ICUserValueStructure[2].Capture_StartFlag == 0 )
		{
      // 存捕获比较寄存器的值的变量的值清0			
			TIM_ICUserValueStructure[2].Capture_CcrValue = 0;
            TIM_ICUserValueStructure[2].Capture_First  = GENERAL1_TIM_GetCapture3_FUN (GENERAL1_TIM);
			// 当第一次捕获到上升沿之后，就把捕获边沿配置为下降沿
			GENERAL1_TIM_OC3PolarityConfig_FUN(GENERAL1_TIM, TIM_ICPolarity_Falling);
      // 开始捕获标准置1			
			TIM_ICUserValueStructure[2].Capture_StartFlag = 1;			
		}
		// 下降沿捕获中断
		else // 第二次捕获
		{
			// 获取捕获比较寄存器的值，这个值就是捕获到的高电平的时间的值
			TIM_ICUserValueStructure[2].Capture_CcrValue += GENERAL1_TIM_GetCapture3_FUN (GENERAL1_TIM) - TIM_ICUserValueStructure[2].Capture_First;

			// 当第二次捕获到下降沿之后，就把捕获边沿配置为上升沿，好开启新的一轮捕获
			GENERAL1_TIM_OC3PolarityConfig_FUN(GENERAL1_TIM, TIM_ICPolarity_Rising);
      // 开始捕获标志清0		
			TIM_ICUserValueStructure[2].Capture_StartFlag = 0;
      // 捕获完成标志置1			
			TIM_ICUserValueStructure[2].Capture_FinishFlag = 1;			
		}

		TIM_ClearITPendingBit (GENERAL1_TIM,GENERAL1_TIM_IT_CC3);	    
	}

//第四
	if ( TIM_GetITStatus (GENERAL1_TIM, GENERAL1_TIM_IT_CC4 ) != RESET)
	{
		// 第一次捕获
		if ( TIM_ICUserValueStructure[3].Capture_StartFlag == 0 )
		{
      // 存捕获比较寄存器的值的变量的值清0			
			TIM_ICUserValueStructure[3].Capture_CcrValue = 0;
            TIM_ICUserValueStructure[3].Capture_First  = GENERAL1_TIM_GetCapture4_FUN (GENERAL1_TIM);
			// 当第一次捕获到上升沿之后，就把捕获边沿配置为下降沿
			GENERAL1_TIM_OC4PolarityConfig_FUN(GENERAL1_TIM, TIM_ICPolarity_Falling);
      // 开始捕获标准置1			
			TIM_ICUserValueStructure[3].Capture_StartFlag = 1;			
		}
		// 下降沿捕获中断
		else // 第二次捕获
		{
			// 获取捕获比较寄存器的值，这个值就是捕获到的高电平的时间的值
			TIM_ICUserValueStructure[3].Capture_CcrValue += GENERAL1_TIM_GetCapture4_FUN (GENERAL1_TIM) - TIM_ICUserValueStructure[3].Capture_First;

			// 当第二次捕获到下降沿之后，就把捕获边沿配置为上升沿，好开启新的一轮捕获
			GENERAL1_TIM_OC4PolarityConfig_FUN(GENERAL1_TIM, TIM_ICPolarity_Rising);
      // 开始捕获标志清0		
			TIM_ICUserValueStructure[3].Capture_StartFlag = 0;
      // 捕获完成标志置1			
			TIM_ICUserValueStructure[3].Capture_FinishFlag = 1;		
		}

		TIM_ClearITPendingBit (GENERAL1_TIM,GENERAL1_TIM_IT_CC4);	    
	}	
}


void GENERAL2_TIM_INT_FUN(void)
{
	// 当要被捕获的信号的周期大于定时器的最长定时时，定时器就会溢出，产生更新中断
	// 这个时候我们需要把这个最长的定时周期加到捕获信号的时间里面去
	if ( TIM_GetITStatus ( GENERAL2_TIM, TIM_IT_Update) != RESET )               
	{	
		TIM_ICUserValueStructure[4].Capture_CcrValue += GENERAL2_TIM_PERIOD;		
		TIM_ClearITPendingBit ( GENERAL2_TIM, TIM_FLAG_Update ); 		
	}

//第一
	if ( TIM_GetITStatus (GENERAL2_TIM, GENERAL1_TIM_IT_CC3 ) != RESET)
	{
		// 第一次捕获
		if ( TIM_ICUserValueStructure[4].Capture_StartFlag == 0 )
		{
      // 存捕获比较寄存器的值的变量的值清0			
			TIM_ICUserValueStructure[4].Capture_CcrValue = 0;
            TIM_ICUserValueStructure[4].Capture_First  = GENERAL2_TIM_GetCapture3_FUN (GENERAL2_TIM);
			// 当第一次捕获到上升沿之后，就把捕获边沿配置为下降沿
			GENERAL2_TIM_OC3PolarityConfig_FUN(GENERAL2_TIM, TIM_ICPolarity_Falling);
      // 开始捕获标准置1			
			TIM_ICUserValueStructure[4].Capture_StartFlag = 1;			
		}
		// 下降沿捕获中断
		else // 第二次捕获
		{
			// 获取捕获比较寄存器的值，这个值就是捕获到的高电平的时间的值
			TIM_ICUserValueStructure[4].Capture_CcrValue += GENERAL2_TIM_GetCapture3_FUN (GENERAL2_TIM) - TIM_ICUserValueStructure[4].Capture_First;

			// 当第二次捕获到下降沿之后，就把捕获边沿配置为上升沿，好开启新的一轮捕获
			GENERAL2_TIM_OC3PolarityConfig_FUN(GENERAL2_TIM, TIM_ICPolarity_Rising);
      // 开始捕获标志清0		
			TIM_ICUserValueStructure[4].Capture_StartFlag = 0;
      // 捕获完成标志置1			
			TIM_ICUserValueStructure[4].Capture_FinishFlag = 1;		
//			printf ( "5\r\n：%d us\r\n", TIM_ICUserValueStructure[4].Capture_CcrValue);
		}

		TIM_ClearITPendingBit (GENERAL2_TIM,GENERAL2_TIM_IT_CC3);	    
	}	
}
void TIM5_IRQHandler(void)
{
	extern int8_t  MEASURE_FLAG;   // 1 采集数据
	
	static int portNum = 0;      //选择测距通道
	
	if ( TIM_GetITStatus( TIM5, TIM_IT_Update) != RESET ) 
	{			
		if( MEASURE_FLAG)
		{	
			UltrasonicWave(portNum);    //采集一个模块数据
			portNum++;
			if( portNum > ULTR_NUM)   //拐杖上模块数据采集完毕
			{
			    portNum = 0; 	
#ifndef ONLY_WALKINGSTICK               //眼镜+拐杖
				
				SendGlasses(UltrasonicWave_Distance,ULTR_NUM);           //发送数据 	
				MEASURE_FLAG = 0;
#endif							
			}
		}	
		TIM_ClearITPendingBit(TIM5 , TIM_FLAG_Update);  		 
	}		
	
}


/**
  * @brief  外部中断0，用于检测按键
  * @param  None
  * @retval None
  */

void EXTI0_IRQHandler(void)
{
	if(EXTI_GetITStatus(EXTI_Line0) != RESET) //确保是否产生了EXTI Line中断
	{		
		direction_flag=1;												//将按键标志位进行标记
		EXTI_ClearITPendingBit(EXTI_Line0);     //清除中断标志位
	}  
}


/**
  * @brief  串口2中断，用于gps
  * @param  None
  * @retval None
  */

void USART2_IRQHandler(void)                	//串口2中断服务程序
{
	u8 Res;
	if(USART_GetITStatus(USART2, USART_IT_RXNE) != RESET) 
	{
		Res =USART_ReceiveData(USART2);						//读取接收到的数据
	
	if(Res == '$')
	{
		point1 = 0;	
	}
		

	  USART_RX_BUF[point1++] = Res;

	if(USART_RX_BUF[0] == '$' && USART_RX_BUF[4] == 'M' && USART_RX_BUF[5] == 'C')			//确定是否收到"GPRMC/GNRMC"这一帧数据
	{
		if(Res == '\n')									   
		{
			memset(Save_Data.GPS_Buffer, 0, GPS_Buffer_Length);      //将GPS数据初始化
			memcpy(Save_Data.GPS_Buffer, USART_RX_BUF, point1); 		 //将读取的数据保存到GPS数据中
			Save_Data.isGetData = true;
			point1 = 0;
			memset(USART_RX_BUF, 0, USART_REC_LEN);      						 //将串口接收数据清空				
		}	
	}
	
	if(point1 >= USART_REC_LEN)
	{
		point1 = USART_REC_LEN;
	}	 		 
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
