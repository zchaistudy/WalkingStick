/**
  ******************************************************************************
  * @file    main.c
  * @author  fire
  * @version V1.0
  * @date    2014-08-xx
  * @brief   gps模块演示测试程序
  ******************************************************************************
  * @attention
  *
  * 实验平台:野火 ISO-STM32 开发板
  * 论坛    :http://www.chuxue123.com
  * 淘宝    :http://firestm32.taobao.com
  *
  ******************************************************************************
	*/
#include "UltrasonicWave.h"
#include "bsp_delay.h"
#include "stm32f10x.h"
#include "bsp_usart1.h"
#include "gps_config.h"
#include <string.h>
#include "bsp_SysTick.h"
#include "bsp_usart3.h"
#include "sim900a.h"
#include "bsp_angle.h"
#include "bsp_exti.h"
uint8_t direction_flag;

/*
 * 测试GPS模块功能
 * 
 */


int main(void)
{
	
	int angle=0; 
	int i=0;
	int num[AVER_NUM];									//保存超声波数据
	nmeaINFO GpsInfo;     						  //GPS结构体
	nmeaTIME beiJingTime;  							//北京时间
	char sendData[30]; 									//gprs需要发送的内容
	int sendByBlueTooth[MAX_SEND];		  //发送避障数据以及方位数据
//	delayInit();
//	EXTI_PA0_Config(); 									//方位按键及按键中断初始化
	NVIC_Configuration();								//配置外部中断0优先级
	UltrasonicWave_Configuration();			//初始化引脚
	USART1_Config(38400);               //串口1初始化,用于调试
//	I2C_GPIO_Config();									//I2C通讯的初始化

//  InitLSM303D();											//方位模块的初始化
//	SysTick_Init();                  	  //嘀嗒定时器初始化
	GENERAL_TIM_Init();									//初始化定时器
//	USART3_Config(9600);             	 	//串口3初始化，用于gprs
//	GPS_Config();                    	 	//串口2初始化，用于gps
//	gprs_init("47.106.74.67","10001");
	printf("holle word");
	 while(1)           
	{
//		i++;
//		if(i>100)
//		{
////			GPS_GPRS(GpsInfo,beiJingTime,sendData);
//			i=0;
//		}

			UltrasonicWave(num);								//获取超声波数据
			delayMs(500);            //问题1：这个会使gprs卡住，不知道为什么
	}
}

/*********************************************END OF FILE**********************/


