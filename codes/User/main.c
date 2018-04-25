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
//	
//	int angle;
//	int num[AVER_NUM];
//	delayInit();
//		EXTI_PA0_Config(); //方位的按键
//	UltrasonicWave_Configuration();
////	SysTick_Init();                   //嘀嗒定时器初始化
//	USART1_Config(38400);               //串口1初始化,用于调试
////	NVIC_Configuration();
////	USART3_Config(9600);              //串口3初始化，用于gprs
////	GPS_Config();                     //串口2初始化，用于gps
////	gprs_init("120.78.193.178","10000");
//	GENERAL_TIM_Init();
//////	GPS_GPRS();
//	  while(1)           
//	{
//		UltrasonicWave(num);
//	//	printf("S00000TES11114TE");
//		delayMs(60);
//	}
//  I2C_GPIO_Config();
//  InitLSM303D();
//  while(1)
//  {
//		if(direction_flag)
//		{
//				getAngle();
//				direction_flag=0;;
//		}
//		printf("无输出\n");
//		printf("direction_flag = %d\n",direction_flag);		
//		delayMs(1000);
//						
//  }


	int a,b;
	nmeaINFO GpsInfo;      //GPS结构体
	nmeaTIME beiJingTime;  //北京时间
	char sendData[30]; //需要发送的内容
	
	SysTick_Init();        //嘀嗒定时器初始化
	USART1_Config(115200); //串口1初始化,用于调试
	USART3_Config(9600); //串口3初始化，用于gprs
	GPS_Config();          //串口2初始化，用于gps
	gprs_init("47.106.74.67","10001");
	
	while(1)
	{
		GpsInfo = Get_GPS_Data();//GPS测试
		GMTconvert(&GpsInfo.utc,&beiJingTime,8,1);//对解码后的时间进行转换，转换成北京时间
		DEBUG_GPS("纬度：%lf,经度%lf\r\n\r\n",GpsInfo.lat,GpsInfo.lon);
		a=(int)(GpsInfo.lon/100);
		b=(int)(GpsInfo.lat/100);
		GpsInfo.lon=(GpsInfo.lon-100*a)/60+a;
		GpsInfo.lat=(GpsInfo.lat-100*b)/60+b;
		sprintf(sendData,"lo:%f,la:%f\r\n",GpsInfo.lon,GpsInfo.lat);//需要发送的内容
		DEBUG_GPS("时间：%d-%d-%d %d:%d:%d\r\n", beiJingTime.year+1900, beiJingTime.mon+1,beiJingTime.day,beiJingTime.hour,beiJingTime.min,beiJingTime.sec);
		DEBUG_GPS("纬度：%lf,经度%lf\r\n\r\n",GpsInfo.lat,GpsInfo.lon);

		/*测试TCP连接*/
		if(sim900a_gprs_send(sendData) == 1)//发送数据
		{
				printf("发送成功！\r\n");
				SIM900A_CLEAN_RX(); //清除串口缓冲区
				Delay_ms(2000);
				printf("地理位置为%s\r\n",SIM900A_RX());//输出结果		
		}
		else
		{
			printf("发送失败！\r\n\r\n");
			gprs_init("47.106.74.67","10001");
		}	
	}

}

/*********************************************END OF FILE**********************/


