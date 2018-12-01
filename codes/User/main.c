/**
  ******************************************************************************
  * @file    main.c
  * @author  chzh
  * @version V1.0
  * @date    2018-10-14
  * @brief   智能拐杖主函数
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

//GPRS相关外设初始化函数
static void Periph_GPRS_Init()
{
	UART4_Init_Config(115200);					//GPRS使用串口4初始化
	SysTick_Init_Config();							//滴答定时器初始化，用于GPRS延时计时
}

void delay_0(u32 time)
{
  while(time--);
}

int main(void)
{
	int angle;
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);       //中断优先级分组统一使用NVIC_PriorityGroup_2
	
	MEASURE_FLAG=0;                              //当拐杖获得从眼镜发来的拐杖超声波数据请求时，将此拐杖超声波标志位置1，拐杖开始获取拐杖上超声波数据，并发送给眼镜。
  usart3Config();                              //串口3初始化，用于调试
	USART1_Config(38400);                        //串口1初始化,用于与眼镜通讯
	I2C_GPIO_Config();									         //I2C通讯的初始化，用于方位模块
  InitLSM303D();											         //方位模块的初始化
	WaveConfig();
	USART2_Config();										         //串口2，用于gps
	EXTI_PA0_Config(); 									         //方位按键（当按下该按键后，进入外部中断，读取方位模块数据）及按键中断初始化
	Periph_GPRS_Init();	                         //GPRS相关外设初始化函数,同时初始化了滴答定时器用于计时
	TIM2_Breathing_Init();                       //定时器2初始化，使用PWM控制震动模块的震动频率
//	TIM6_TIM_Mode_Config();										 //定时器6初始化，用于计时（一开始计划为定时获取GPS信号，当是发现需要转载操作系统才能完成）
	my_printf("系统启动......");
	 while(1)                                    
		 
	{
//			parseGpsBuffer();		                   //读取gps信息并在控制台打印位置信息		
//当按下方位按钮进入外部中断后，将direction_flag置1，此时可以获取方位信息		
			if(direction_flag)										   
			{
					angle=getAngle();
					direction_flag=0;
					my_printf("当前方位为：");
					my_printf("%c",angle);
				  SendAngle(angle);										//由于语音模块位于眼镜上，故将方位信息发送给眼睛
			}
			if(HelpFlag)
			{
				GPRS_Send_GPS(SendGPS.lo, SendGPS.la);	//使用GPRS发送当前位置坐标
//					GPRS_Send_help();					//使用GPRS发送求救信号  会有一定时间的延时，所以导致数据不会很快被接收到
					HelpFlag=0;
			}
			if( IsFinishMeasure() )   //拐杖上模块数据采集完毕
			{
				SendGlasses(UltrasonicWave_Distance,ULTR_NUM);           //发送数据 	
				MEASURE_FLAG = 0;			
			}

	}
}

/*********************************************END OF FILE**********************/


