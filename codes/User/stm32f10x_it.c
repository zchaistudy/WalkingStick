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
#include "bsp_GeneralTim.h"
#include "UltrasonicWave.h"
#include "debug.h"
#include "gps.h" 

extern void TimingDelay_Decrement(void);
extern uint8_t direction_flag;
extern _SaveData Save_Data;

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
void USART1_IRQHandler(void)
{


}








/**
  * @brief  This function handles PPP interrupt request.
  * @param  None
  * @retval None
  */

void GENERAL_TIM_INT_FUN(void)
{
	// 当要被捕获的信号的周期大于定时器的最长定时时，定时器就会溢出，产生更新中断
	if ( TIM_GetITStatus ( GENERAL_TIM, TIM_IT_Update) != RESET )               
	{	
//		TIM_ICUserValueStructure.Capture_FinishFlag = 1;	
		TIM_ICUserValueStructure2.Capture_CcrValue += GENERAL_TIM_PERIOD+1;
		TIM_ClearITPendingBit ( GENERAL_TIM, TIM_FLAG_Update ); 		
	}

//第一个超声波模块上升沿捕获中断
	if ( TIM_GetITStatus (GENERAL_TIM, TIM_IT_CC3 ) != RESET)
	{
		// 第一次捕获
		if ( TIM_ICUserValueStructure.Capture_StartFlag == 0 )
		{
			// 计数器清0
			TIM_SetCounter ( GENERAL_TIM, 0 );
			// 自动重装载寄存器更新标志清0
			TIM_ICUserValueStructure.Capture_Period = 0;
      // 存捕获比较寄存器的值的变量的值清0			
			TIM_ICUserValueStructure.Capture_CcrValue = 0;

			// 当第一次捕获到上升沿之后，就把捕获边沿配置为下降沿
			TIM_OC3PolarityConfig(GENERAL_TIM, TIM_ICPolarity_Falling);
      // 开始捕获标准置1			
			TIM_ICUserValueStructure.Capture_StartFlag = 1;			
		}
		// 下降沿捕获中断
		else // 第二次捕获
		{
			// 获取捕获比较寄存器的值，这个值就是捕获到的高电平的时间的值
			TIM_ICUserValueStructure.Capture_CcrValue = TIM_GetCapture3 (GENERAL_TIM);
			// 当第二次捕获到下降沿之后，就把捕获边沿配置为上升沿，好开启新的一轮捕获
			TIM_OC3PolarityConfig(GENERAL_TIM, TIM_ICPolarity_Rising);
      // 开始捕获标志清0		
			TIM_ICUserValueStructure.Capture_StartFlag = 0;
      // 捕获完成标志置1			
			TIM_ICUserValueStructure.Capture_FinishFlag = 1;	
            p_debug("tim2, cc0\r\n");
            			
		}
		TIM_ClearITPendingBit (GENERAL_TIM,TIM_IT_CC3);	    
	}	

//第二个超声波模块上升沿捕获中断
	else if ( TIM_GetITStatus (GENERAL_TIM, TIM_IT_CC4 ) != RESET)
	{
		// 第一次捕获
		if ( TIM_ICUserValueStructure.Capture_StartFlag == 0 )
		{
			// 计数器清0
			TIM_SetCounter ( GENERAL_TIM, 0 );
			// 自动重装载寄存器更新标志清0
			TIM_ICUserValueStructure.Capture_Period = 0;
      // 存捕获比较寄存器的值的变量的值清0			
			TIM_ICUserValueStructure.Capture_CcrValue = 0;

			// 当第一次捕获到上升沿之后，就把捕获边沿配置为下降沿
			TIM_OC4PolarityConfig(GENERAL_TIM, TIM_ICPolarity_Falling);
      // 开始捕获标准置1			
			TIM_ICUserValueStructure.Capture_StartFlag = 1;			
		}
		// 下降沿捕获中断
		else // 第二次捕获
		{
			// 获取捕获比较寄存器的值，这个值就是捕获到的高电平的时间的值
			TIM_ICUserValueStructure.Capture_CcrValue = 
			TIM_GetCapture4 (GENERAL_TIM);

			// 当第二次捕获到下降沿之后，就把捕获边沿配置为上升沿，好开启新的一轮捕获
			TIM_OC4PolarityConfig(GENERAL_TIM, TIM_ICPolarity_Rising);
      // 开始捕获标志清0		
			TIM_ICUserValueStructure.Capture_StartFlag = 0;
      // 捕获完成标志置1			
			TIM_ICUserValueStructure.Capture_FinishFlag = 1;	
             p_debug("tim2, cc1\r\n");			
		}
		TIM_ClearITPendingBit (GENERAL_TIM,TIM_IT_CC4);	    
	}	
}

void TIM3_IRQHandler(void)
{
	// 当要被捕获的信号的周期大于定时器的最长定时时，定时器就会溢出，产生更新中断
	if ( TIM_GetITStatus ( TIM3, TIM_IT_Update) != RESET )               
	{	
//		TIM_ICUserValueStructure.Capture_FinishFlag = 1;	
		TIM_ICUserValueStructure2.Capture_CcrValue += GENERAL_TIM_PERIOD+1;
		TIM_ClearITPendingBit ( TIM3, TIM_FLAG_Update ); 		
	}
//printf("tim3\r\n");
//第三个超声波模块上升沿捕获中断
	if ( TIM_GetITStatus (TIM3, TIM_IT_CC1 ) != RESET)
	{  	
//printf("cc1\r\n");		
		// 第一次捕获
		if ( TIM_ICUserValueStructure2.Capture_StartFlag == 0 )
		{
			// 计数器清0
			TIM_SetCounter ( TIM3, 0 );
			// 自动重装载寄存器更新标志清0
			TIM_ICUserValueStructure2.Capture_Period = 0;
      // 存捕获比较寄存器的值的变量的值清0			
			TIM_ICUserValueStructure2.Capture_CcrValue = 0;

			// 当第一次捕获到上升沿之后，就把捕获边沿配置为下降沿
			TIM_OC1PolarityConfig(TIM3, TIM_ICPolarity_Falling);
      // 开始捕获标准置1			
			TIM_ICUserValueStructure2.Capture_StartFlag = 1;			
		}
		// 下降沿捕获中断
		else // 第二次捕获
		{
			// 获取捕获比较寄存器的值，这个值就是捕获到的高电平的时间的值
			TIM_ICUserValueStructure2.Capture_CcrValue = 
			TIM_GetCapture1 (TIM3);

			// 当第二次捕获到下降沿之后，就把捕获边沿配置为上升沿，好开启新的一轮捕获
			TIM_OC1PolarityConfig(TIM3, TIM_ICPolarity_Rising);
      // 开始捕获标志清0		
			TIM_ICUserValueStructure2.Capture_StartFlag = 0;
      // 捕获完成标志置1			
			TIM_ICUserValueStructure2.Capture_FinishFlag = 1;	
			p_debug("tim3, cc1\r\n");
		}
		TIM_ClearITPendingBit (TIM3,TIM_IT_CC1);	    
	}	
//第四个超声波模块上升沿捕获中断
	else if ( TIM_GetITStatus (TIM3, TIM_IT_CC2 ) != RESET)
	{	
		// 第一次捕获
		if ( TIM_ICUserValueStructure2.Capture_StartFlag == 0 )
		{
			// 计数器清0
			TIM_SetCounter ( TIM3, 0 );
			// 自动重装载寄存器更新标志清0
			TIM_ICUserValueStructure2.Capture_Period = 0;
      // 存捕获比较寄存器的值的变量的值清0			
			TIM_ICUserValueStructure2.Capture_CcrValue = 0;

			// 当第一次捕获到上升沿之后，就把捕获边沿配置为下降沿
			TIM_OC2PolarityConfig(TIM3, TIM_ICPolarity_Falling);
      // 开始捕获标准置1			
			TIM_ICUserValueStructure2.Capture_StartFlag = 1;			
		}
		// 下降沿捕获中断
		else // 第二次捕获
		{
			// 获取捕获比较寄存器的值，这个值就是捕获到的高电平的时间的值
			TIM_ICUserValueStructure2.Capture_CcrValue = TIM_GetCapture2 (TIM3);
			// 当第二次捕获到下降沿之后，就把捕获边沿配置为上升沿，好开启新的一轮捕获
			TIM_OC2PolarityConfig(TIM3, TIM_ICPolarity_Rising);
      // 开始捕获标志清0		
			TIM_ICUserValueStructure2.Capture_StartFlag = 0;
      // 捕获完成标志置1			
			TIM_ICUserValueStructure2.Capture_FinishFlag = 1;	
			p_debug("tim3, cc2\r\n");			
		}
		TIM_ClearITPendingBit (TIM3,TIM_IT_CC2);	    
	}	

//第五个超声波模块上升沿捕获中断
	else if ( TIM_GetITStatus (TIM3, TIM_IT_CC3 ) != RESET)
	{	
		// 第一次捕获
		if ( TIM_ICUserValueStructure2.Capture_StartFlag == 0 )
		{
			// 计数器清0
			TIM_SetCounter ( TIM3, 0 );
			// 自动重装载寄存器更新标志清0
			TIM_ICUserValueStructure2.Capture_Period = 0;
      // 存捕获比较寄存器的值的变量的值清0			
			TIM_ICUserValueStructure2.Capture_CcrValue = 0;

			// 当第一次捕获到上升沿之后，就把捕获边沿配置为下降沿
			TIM_OC3PolarityConfig(TIM3, TIM_ICPolarity_Falling);
      // 开始捕获标准置1			
			TIM_ICUserValueStructure2.Capture_StartFlag = 1;			
		}
		// 下降沿捕获中断
		else // 第二次捕获
		{
			// 获取捕获比较寄存器的值，这个值就是捕获到的高电平的时间的值
			TIM_ICUserValueStructure2.Capture_CcrValue = 
			TIM_GetCapture3(TIM3);

			// 当第二次捕获到下降沿之后，就把捕获边沿配置为上升沿，好开启新的一轮捕获
			TIM_OC3PolarityConfig(TIM3, TIM_ICPolarity_Rising);
      // 开始捕获标志清0		
			TIM_ICUserValueStructure2.Capture_StartFlag = 0;
      // 捕获完成标志置1			
			TIM_ICUserValueStructure2.Capture_FinishFlag = 1;
			p_debug("tim3, cc3\r\n");
		}
		TIM_ClearITPendingBit (TIM3,TIM_IT_CC3);	    
	}	
//第六个超声波模块上升沿捕获中断
	else if ( TIM_GetITStatus (TIM3, TIM_IT_CC4 ) != RESET)
	{	
		// 第一次捕获
		if ( TIM_ICUserValueStructure2.Capture_StartFlag == 0 )
		{
			// 计数器清0
			TIM_SetCounter ( TIM3, 0 );
			// 自动重装载寄存器更新标志清0
			TIM_ICUserValueStructure2.Capture_Period = 0;
      // 存捕获比较寄存器的值的变量的值清0			
			TIM_ICUserValueStructure2.Capture_CcrValue = 0;

			// 当第一次捕获到上升沿之后，就把捕获边沿配置为下降沿
			TIM_OC4PolarityConfig(TIM3, TIM_ICPolarity_Falling);
      // 开始捕获标准置1			
			TIM_ICUserValueStructure2.Capture_StartFlag = 1;			
		}
		// 下降沿捕获中断
		else // 第二次捕获
		{
			// 获取捕获比较寄存器的值，这个值就是捕获到的高电平的时间的值
			TIM_ICUserValueStructure2.Capture_CcrValue = 
			TIM_GetCapture4(TIM3);

			// 当第二次捕获到下降沿之后，就把捕获边沿配置为上升沿，好开启新的一轮捕获
			TIM_OC4PolarityConfig(TIM3, TIM_ICPolarity_Rising);
      // 开始捕获标志清0		
			TIM_ICUserValueStructure2.Capture_StartFlag = 0;
      // 捕获完成标志置1			
			TIM_ICUserValueStructure2.Capture_FinishFlag = 1;	
           p_debug("tim3, cc4\r\n");			
		}
		TIM_ClearITPendingBit (TIM3,TIM_IT_CC4);	    
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
