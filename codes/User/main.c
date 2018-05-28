/**
  ******************************************************************************
  * @file    main.c
  * @author  chzh
  * @version V1.0
  * @date    2018-05-25
  * @brief   智能拐杖主函数
  ******************************************************************************
	*/
#include "UltrasonicWave.h"
#include "bsp_delay.h"
#include "stm32f10x.h"
#include "bsp_usart1.h"
#include <string.h>
#include "bsp_SysTick.h"
#include "bsp_angle.h"
#include "bsp_exti.h"
#include "gps.h"

#define _DEBUG_ 1										//用于调试
#if _DEBUG_
    #define DEBUG(x) printf(x)
#else
    #define DEBUG(x) 
#endif


uint8_t direction_flag;


int main(void)
{
	
	int angle=0; 
	int i=0;
	int num[AVER_NUM];									//保存超声波数据
	
	int sendByBlueTooth[MAX_SEND];		  //发送避障数据以及方位数据
	
	delayInit();												//初始化滴答定时器
	
	EXTI_PA0_Config(); 									//方位按键及按键中断初始化
	NVIC_Configuration();								//配置外部中断0优先级
	
	UltrasonicWave_Configuration();			//初始化引脚
	
	USART1_Config(38400);               //串口1初始化,用于调
	
	I2C_GPIO_Config();									//I2C通讯的初始化
	
  InitLSM303D();											//方位模块的初始化
	
	SysTick_Init();                  	  //嘀嗒定时器初始化
	
	GENERAL_TIM_Init();									//初始化定时器
	
	USART2_Config();										//串口2，用于gps
	
	 while(1)           
	{
//		i++;
//		if(i>100)
//		{
//			i=0;
//		}
		parseGpsBuffer();									
		printGpsBuffer();
//			UltrasonicWave(num);								//获取超声波数据
//			if(direction_flag)
//			{
//					angle=getAngle();
//					direction_flag=0;
//					printf("%d",angle);
//			}
//			angle=0;
//			num[AVER_NUM-1]=angle;
//			for(i=0;i<AVER_NUM;i++)				//num的最后一个数据用来发送方位信息
//			{
//				printf("%d",num[i]);
//			}
//			delayMs(600);            //问题1：这个会使gprs卡住，不知道为什么
////			GPS_GPRS(GpsInfo,beiJingTime,sendData);
//			i=0;
//		}

			UltrasonicWave(num);								//获取超声波数据
			delayMs(500);            //问题1：这个会使gprs卡住，不知道为什么
	}
}

/*********************************************END OF FILE**********************/


