#include "stm32f10x.h"
#include "bsp_usart1.h"
#include "bsp_SysTick.h"
#include "timer.h"
#include "string.h"
#include "GA6_module_errors.h"
#include "bsp_usart4.h"
#include <stdio.h>
#include "gprs.h"

#define Buf4_Max 	  100 //串口2缓存长度
#define STABLE_TIMES  10  //等待系统上电后的稳定

/*************	本地常量声明	**************/

/*************  本地变量声明	**************/
char Uart4_Buf[Buf4_Max];//串口2接收缓存
u8 First_Int = 0;
char error_result[20];

char server_ip_port_cmd[45];


/*************	本地函数声明	**************/
void CLR_Buf_4(void);     //清除串口2接收缓存
u8 Wait_CREG(u8 query_times);    //等待模块注册成功
u8 Find(char *a);

/*************  外部函数和变量声明*****************/

int check_status(void)
{
	int ret;
	char esc_char[2];
	
	esc_char[0] = 0x1B;//退出字符
	esc_char[1] = '\0';
	
	ret = UART4_Send_AT_Command("AT","OK",5,50);//测试通信是否成功
	if(ret == 0)
	{
		UART4_SendString(esc_char);//万一进入>状态，那么久发送一个ESC字符
		return COMMUNITE_ERROR;
	}
	
	ret = UART4_Send_AT_Command("AT+CPIN?","READY",2,50);//查询卡是否插上
	if(ret == 0)
	{
		return NO_SIM_CARD_ERROR;
	}
	
	ret = Wait_CREG(3);//查询卡是否注册到网络
	if(ret == 0)
	{
		return SIM_CARD_NO_REG_ERROR;
	}
    
    ret = UART4_Send_AT_Command("ATE0","OK",2,50);//关闭回显功能
	if(ret == 0)
	{
		return EAT0_ERROR;
	}
    
	return 1;
}

/***************************************************************
注：当然你可以返回其他值，来确定到底是哪一步发送指令出现失败了。
****************************************************************/
int send_data_to_server(char *server_IP_and_port,char *content)
{
	u8 ret;
	char end_char[2];
	
	
	end_char[0] = 0x1A;//结束字符
	end_char[1] = '\0';
	
	ret = UART4_Send_AT_Command("AT+CIPSTATUS","CONNECT OK",3,50*2);//查询连接状态
	if(ret == 1)//说明服务器处于连接状态
	{
		ret = UART4_Send_AT_Command("AT+CIPSEND",">",3,50);//开发发送数据
		if(ret == 0)
		{
			return AT_CIPSEND_ERROR;
		}
		
		UART4_SendString(content);
		
		ret = UART4_Send_AT_Command_End(end_char,"SEND OK",1,250);//发送结束符，等待返回ok,等待5S发一次，因为发送数据时间可能较长
		if(ret == 0)
		{
			return END_CHAR_ERROR;
		}
		
		return 1;
	}
	else
	{
		ret = UART4_Send_AT_Command("AT+CGATT=1","OK",3,50*7);//附着网络
		if(ret == 0)
		{
			return AT_CGATT_ERROR;
		}
		
		ret = UART4_Send_AT_Command("AT+CGACT=1,1","OK",3,50*2);//激活网络
		if(ret == 0)
		{
			return AT_CGATT1_ERROR;
		}

		memset(server_ip_port_cmd,'\0',45);
		strcpy(server_ip_port_cmd,"AT+CIPSTART=\"TCP\",");
		strcat(server_ip_port_cmd,server_IP_and_port);
		
		ret = UART4_Send_AT_Command(server_ip_port_cmd,"CONNECT OK",3,50*2);//连接服务器
		if(ret == 0)
		{
			return AT_CIPSTART_ERROR;
		}
		
		delay_ms(50);
		UART1_SendString(Uart4_Buf);//MY 首次连接，显示服务器发来的连接成功消息
		
		
		ret = UART4_Send_AT_Command("AT+CIPSEND",">",3,50);//开始发送数据
		if(ret == 0)
		{
			return AT_CIPSEND_ERROR;
		}
		
		UART4_SendString(content);
		ret = UART4_Send_AT_Command_End(end_char,"SEND OK",1,250);//发送结束符，等待返回ok,等待5S发一次，因为发送数据时间可能较长
			
		if(ret == 0)
		{
			return END_CHAR_ERROR;
		}
		
		return 1;

	}
}


/*******************************************************************************
* 函数名  : USART2_IRQHandler
* 描述    : 串口1中断服务程序
* 输入    : 无
* 返回    : 无 
* 说明    : 
*******************************************************************************/
void UART4_IRQHandler(void)                	
{
	u8 Res=0;
	Res = USART_ReceiveData(UART4);
	Uart4_Buf[First_Int] = Res;  	  //将接收到的字符串存到缓存中
	First_Int++;                	  //缓存指针向后移动
	if(First_Int > Buf4_Max)       	  //如果缓存满,将缓存指针指向缓存的首地址
	{
		First_Int = 0;
	}
} 	

/*******************************************************************************
* 函数名  : TIM2_IRQHandler
* 描述    : 定时器2中断断服务函数
* 输入    : 无
* 输出    : 无
* 返回    : 无 
* 说明    : 无
*******************************************************************************/
void TIM2_IRQHandler(void)   //TIM2中断
{
	static u8 flag =1;

	if (TIM_GetITStatus(TIM2, TIM_IT_Update) != RESET)  //检查TIM2更新中断发生与否
	{
		
		TIM_ClearITPendingBit(TIM2, TIM_IT_Update);  //清除TIM2更新中断标志 
	
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
* 函数名 : CLR_Buf
* 描述   : 清除串口4缓存数据
* 输入   : 
* 输出   : 
* 返回   : 
* 注意   : 
*******************************************************************************/
void CLR_Buf_4(void)
{
	u16 k;
	for(k=0;k<Buf4_Max;k++)      //将缓存内容清零
	{
		Uart4_Buf[k] = 0x00;
	}
    First_Int = 0;              //接收字符串的起始存储位置
}

/*******************************************************************************
* 函数名 : Wait_CREG
* 描述   : 等待模块注册成功
* 输入   : 
* 输出   : 
* 返回   : 
* 注意   : 
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
* 函数名 : Find
* 描述   : 判断缓存中是否含有指定的字符串
* 输入   : 
* 输出   : 
* 返回   : unsigned char:1 找到指定字符，0 未找到指定字符 
* 注意   : 
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
* 函数名 : GPRS_Send_help
* 描述   : GPRS_Send_help函数
* 输入   : 
* 输出   : 
* 返回   : 
* 注意   : 串口2负责与GA6模块通信，串口1用于串口调试，
*******************************************************************************/
void GPRS_Send_help(void)
{
	u8  i;
	int  ret;
		
	UART1_SendString("GPRS传输help功能启动.......................\r\n");
	
	//后期决定是否删除
	for(i = 0;i < STABLE_TIMES;i++)
	{
		delay_ms(50);
	}
	
	memset(error_result,'\0',20);

	delay_ms(50);
		
	ret = check_status();
	if(ret == 1)
	{
		ret = send_data_to_server("\"47.106.74.67\",10001","help");//发送数据到服务器					
	}
	
	if(ret == 1)
	{
		sprintf(error_result,"成功发送到server\r\n");
		UART1_SendString(error_result);
		delay_ms(50);
		UART1_SendString("收到回复：\r\n");  //received:后面的内容才是回复的真正内容
		UART1_SendString(Uart4_Buf);
		UART1_SendString("\r\n");
		
	}
	else //数据发送失败，此时可能连接时间过长导致的失败，这样就断开连接，然后就可以继续连接上了
	{
		sprintf(error_result,"错误代码 : %d\r\n",ret);
		UART1_SendString(error_result);
		UART4_Send_AT_Command("AT+CIPCLOSE","OK",3,150);//关闭链接
		
	}
	

}

void GPRS_Send_GPS(float lo, float la)
{
	u8  i;
	int  ret;
	char sendData[30]; //需要发送的内容
		
	UART1_SendString("GPRS传输坐标功能启动.......................\r\n");
	
	for(i = 0;i < STABLE_TIMES;i++)
	{
		delay_ms(50);
	}
	memset(error_result,'\0',20);
	memset(sendData,'\0',30);

	delay_ms(50);
		
	ret = check_status();
	if(ret == 1)
	{
		sprintf(sendData,"lo:%f,la:%f\r\n",lo, la);//需要发送的内容
		ret = send_data_to_server("\"47.106.74.67\",10001",sendData);//发送数据到服务器					
	}
	
	if(ret == 1)
	{
		sprintf(error_result,"成功发送到server\r\n");
		UART1_SendString(error_result);
		delay_ms(50);
		UART1_SendString("收到回复：\r\n");  //received:后面的内容才是回复的真正内容
		UART1_SendString(Uart4_Buf);
		UART1_SendString("\r\n");
		
	}
	else //数据发送失败，此时可能连接时间过长导致的失败，这样就断开连接，然后就可以继续连接上了
	{
		sprintf(error_result,"错误代码 : %d\r\n",ret);
		UART1_SendString(error_result);
		UART4_Send_AT_Command("AT+CIPCLOSE","OK",3,150);//关闭链接
		
	}
}

