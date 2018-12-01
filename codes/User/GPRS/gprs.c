#include "stm32f10x.h"
#include "bsp_usart.h"
#include "bsp_SysTick.h"
#include "string.h"
#include "GA6_module_errors.h"
#include "bsp_usart4.h"
#include <stdio.h>
#include "gprs.h"

char *AT_CMGS = "AT+CMGS=71";	
char *pdu_content = "0011000D91685112584368F90008AA3851736CE85BF98C6151FA73B07D27602560C551B5FF0C8BF7901A8FC7624B673A006100700070786E8BA476F24EBA4F4D7F6EFF01FF01FF01";
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
//	
	ret = UART4_Send_AT_Command("AT","OK",5,50);//测试通信是否成功

	if(ret == 0)
	{	
		UART4_SendString(esc_char);//万一进入>状态，那么久发送一个ESC字符
		return COMMUNITE_ERROR;
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

		ret = UART4_Send_AT_Command("AT+CIPSEND",">",3,50);           //开发发送数据
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
//		UART1_SendString(Uart4_Buf);//MY 首次连接，显示服务器发来的连接成功消息
		
		
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
	Uart4_Buf[First_Int] = Res;  	  		//将接收到的字符串存到缓存中
	First_Int++;                	  		//缓存指针向后移动
	if(First_Int > Buf4_Max)       	  	//如果缓存满,将缓存指针指向缓存的首地址
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
	int  ret;
		
	my_printf("GPRS传输help功能启动.......................\r\n");
	
	memset(error_result,'\0',20);
		

	ret = send_data_to_server("\"39.108.110.121\",10001","help");//发送数据到服务器					
	
	if(ret == 1)
	{
		sprintf(error_result,"成功发送到server\r\n");
		my_printf("%s",error_result);
		my_printf("收到回复：\r\n");  //received:后面的内容才是回复的真正内容
		my_printf("%s",Uart4_Buf);
		my_printf("\r\n");
	} 
	else //数据发送失败，此时可能连接时间过长导致的失败，这样就断开连接，然后就可以继续连接上了
	{
		sprintf(error_result,"错误代码 : %d\r\n",ret);
		my_printf("%s",error_result);
		UART4_Send_AT_Command("AT+CIPCLOSE","OK",3,150);//关闭链接
	}
}

void GPRS_Send_GPS(float lo, float la)
{
	int  ret;
	char sendData[30]; //需要发送的内容
		
	my_printf("GPRS传输坐标功能启动.......................\r\n");	
	
	memset(error_result,'\0',20);
	memset(sendData,'\0',30);

		sprintf(sendData,"help!lo:%f,la:%f\r\n",lo, la);//需要发送的内容
		ret = send_data_to_server("\"39.108.110.121\",10001",sendData);          //发送数据到服务器					

	
	if(ret == 1)
	{
		sprintf(error_result,"成功发送到server\r\n");
				my_printf("%s",error_result);
		my_printf("收到回复：\r\n");  //received:后面的内容才是回复的真正内容
		my_printf("%s",Uart4_Buf);
		my_printf("\r\n");
	}
	else //数据发送失败，此时可能连接时间过长导致的失败，这样就断开连接，然后就可以继续连接上了
	{
		sprintf(error_result,"错误代码 : %d\r\n",ret);
		my_printf("%s",error_result);
		UART4_Send_AT_Command("AT+CIPCLOSE","OK",3,150);//关闭链接
	}
}

//发送信息函数
int send_pdu_message(char *content)
{
	int ret;
	char end_char[2];
	
	end_char[0] = 0x1A;
	end_char[1] = '\0';
	
	//设置存储位置，不做返回值判断了
	ret = UART4_Send_AT_Command("AT+CPMS=\"SM\",\"ME\",\"SM\"","OK",3,100);
	if(ret == 0)
	{
		return AT_CPMS_ERROR;
	}
		
	ret = UART4_Send_AT_Command("AT+CMGF=0","OK",3,50);//配置为PDU模式
	if(ret == 0)
	{
		return AT_CMGF_ERROR;
	}
	
	ret = UART4_Send_AT_Command(AT_CMGS,">",3,50);//发送字符个数指令
	
	if(ret == 0)
	{
		return AT_CMGS_ERROR;
	}
	
	UART4_SendString(content);
	ret = UART4_Send_AT_Command_End(end_char,"OK",3,250);//发送结束符，等待返回ok,等待5S发一次，因为短信成功发送的状态时间比较长
	if(ret == 0)
	{
		return END_CHAR_ERROR;
	}
	
	return 1;
}

/*
 * 函数名：TIM6_TIM_NVIC_Config
 * 描述  ：配置定时器6的优先级，优先级分组使用NVIC_PriorityGroup_2
 * 输入  ：无
 * 输出  ：无	
 */
void TIM6_TIM_NVIC_Config(void)
{
    NVIC_InitTypeDef NVIC_InitStructure; 
    // 设置中断组为0
    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);		
		// 设置中断来源
    NVIC_InitStructure.NVIC_IRQChannel = TIM6_IRQn ;	
		// 设置主优先级为 0
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;	 
	  // 设置抢占优先级为3
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;	
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);
}

/*
 * 注意：TIM_TimeBaseInitTypeDef结构体里面有5个成员，TIM6和TIM7的寄存器里面只有
 * TIM_Prescaler和TIM_Period，所以使用TIM6和TIM7的时候只需初始化这两个成员即可，
 * 另外三个成员是通用定时器和高级定时器才有.
 *-----------------------------------------------------------------------------
 *typedef struct
 *{ TIM_Prescaler            都有
 *	TIM_CounterMode			     TIMx,x[6,7]没有，其他都有
 *  TIM_Period               都有
 *  TIM_ClockDivision        TIMx,x[6,7]没有，其他都有
 *  TIM_RepetitionCounter    TIMx,x[1,8,15,16,17]才有
 *}TIM_TimeBaseInitTypeDef; 
 *-----------------------------------------------------------------------------
 */

/*
 * 函数名：TIM6_TIM_Mode_Config
 * 描述  ：初始化定时器6
 * 输入  ：无
 * 输出  ：无	
 */
void TIM6_TIM_Mode_Config(void)
{
    TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
		
		// 开启定时器时钟,即内部时钟CK_INT=72M
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM6, ENABLE);
	
		// 自动重装载寄存器的值，累计TIM_Period+1个频率后产生一个更新或者中断，一次中断时间为1us秒
    TIM_TimeBaseStructure.TIM_Period = 999;	

	  // 时钟预分频数为
    TIM_TimeBaseStructure.TIM_Prescaler= 71;
	
	  // 初始化定时器
    TIM_TimeBaseInit(TIM6, &TIM_TimeBaseStructure);
		
		// 清除计数器中断标志位
    TIM_ClearFlag(TIM6, TIM_FLAG_Update);
	  
		// 开启计数器中断
    TIM_ITConfig(TIM6,TIM_IT_Update,ENABLE);
		
		// 使能计数器
    TIM_Cmd(TIM6, ENABLE);	
		
		TIM6_TIM_NVIC_Config();
}

