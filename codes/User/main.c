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
#include "bsp_usart.h"
#include <string.h>
#include "bsp_SysTick.h"
#include "bsp_angle.h"
#include "bsp_exti.h"
#include "gps.h"
#include "timer.h"
#include "bsp_usart4.h"
#include "gprs.h"
#include "bsp_usart.h"
#include "debug.h"


extern char *pdu_content ;
extern int8_t  MEASURE_FLAG;
uint8_t direction_flag;
extern int UltrasonicWave_Distance[];     

//GPRS相关外设初始化函数
static void Periph_GPRS_Init()
{
	UART4_Init_Config(115200);//GPRS使用串口4初始化
	SysTick_Init_Config();//滴答定时器初始化
	Timer2_Init_Config();	//定时器2初始化
}

void delay_0(u32 time)
{
  while(time--);
}

int main(void)
{
	int ret;
	int angle=0; 
	int i=0;
	int num[AVER_NUM];									         //保存超声波数据
	
	int sendByBlueTooth[MAX_SEND];		           //发送避障数据以及方位数据
	int XX[5]={1,2,3,4,5};
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);       //中断优先级分组
	
	MEASURE_FLAG=0;
//	delayInit();												       //初始化滴答定时器
    usart3Config();
	USART1_Config(38400);                        //串口1初始化,用于蓝牙
	I2C_GPIO_Config();									         //I2C通讯的初始化
    InitLSM303D();											         //方位模块的初始化
//	SysTick_Init();                  	         //嘀嗒定时器初始化
	WaveConfig();

	USART2_Config();										         //串口2，用于gps
	EXTI_PA0_Config(); 									         //方位按键及按键中断初始化
	Periph_GPRS_Init();	//GPRS相关外设初始化函数   

	
	DEBUG("系统启动......");
	my_printf("start debug\r\n");
	printf("test\r\n");
	 while(1)           
	{

//			if(MEASURE_FLAG)
//			{
//					SendGlasses(UltrasonicWave_Distance,ULTR_NUM);
//					MEASURE_FLAG=0;
//			}


//		GPRS_Send_GPS(22.2, 33.3);	//使用GPRS发送位置坐标
//		 GPRS_Send_help();					//使用GPRS发送求救信号
//		i++;
//		if(i>100)
//		{
//			i=0;
//		}
//		parseGpsBuffer();									
//		printGpsBuffer();

//			UltrasonicWave(num);								//获取超声波数据
			if(direction_flag)
			{
					angle=getAngle();
					direction_flag=0;
					DEBUG("当前方位为：");
					DEBUG("%d",angle);
			}
			
			
			
//			angle=0;
//			printf("%d",angle);
//			num[AVER_NUM-1]=angle;
//			for(i=0;i<AVER_NUM;i++)				//num的最后一个数据用来发送方位信息
//			{
//				printf("%d",num[i]);
//			}
//			delayMs(600);            //问题1：这个会使gprs卡住，不知道为什么
////			GPS_GPRS(GpsInfo,beiJingTime,sendData);
//			i=0;
//		}

//			UltrasonicWave(num);								//获取超声波数据
//			delayMs(500);            //问题1：这个会使gprs卡住，不知道为什么

	}
}

/*********************************************END OF FILE**********************/


