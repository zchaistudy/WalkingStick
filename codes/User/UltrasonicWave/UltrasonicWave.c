/******************** (C) 1209 Lab **************************
 * 文件名  : UltrasonicWave.c
 * 描述    ：超声波测距模块，UltrasonicWave_Configuration（）函数
             初始化超声模块，UltrasonicWave_StartMeasure（）函数
			 启动测距，并将测得的数据通过串口1打印出来         
 * 实验平台：Mini STM32开发板  STM32F103RBT6
 * 硬件连接：------------------
 *          | PC8  - TRIG      |
 *          | PC7  - ECHO      |
 *           ------------------
 * 库版本  ：ST3.5.0
 *
 * 作者    ：Lee 
*********************************************************************************/

#include "UltrasonicWave.h"
#include "debug.h"

////////调试开关//////////////
#ifdef DEBUG_ON_OFF 
#undef  DEBUG_ON_OFF
#define DEBUG_ON_OFF 1
#endif
//////////////////////////////

static void UltrasonicWave_StartMeasure(GPIO_TypeDef *  port, int32_t pin);              

int UltrasonicWave_Distance[ULTR_NUM];      //计算出的距离    
static int8_t index = 0;
static int16_t MAX_DISTACE = 150;        //最大距离
int8_t  IT_TAG = 0;          //读取标志，为1时表示以读取到数据

/*
 * 函数名：UltrasonicWave_Configuration
 * 描述  ：超声波模块的初始化
 * 输入  ：无
 * 输出  ：无	
 */
void UltrasonicWave_Configuration(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;	
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);
    
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;		     //设为推挽输出模式
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	
	GPIO_InitStructure.GPIO_Pin = TRIG_PIN1;					
	GPIO_Init(TRIG_PORT1, &GPIO_InitStructure);	
	GPIO_InitStructure.GPIO_Pin = TRIG_PIN2;					
	GPIO_Init(TRIG_PORT2, &GPIO_InitStructure);	
	GPIO_InitStructure.GPIO_Pin = TRIG_PIN3;					
	GPIO_Init(TRIG_PORT3, &GPIO_InitStructure);	
	GPIO_InitStructure.GPIO_Pin = TRIG_PIN4;					
	GPIO_Init(TRIG_PORT4, &GPIO_InitStructure);	
	GPIO_InitStructure.GPIO_Pin = TRIG_PIN5;					
	GPIO_Init(TRIG_PORT5, &GPIO_InitStructure);	
	GPIO_InitStructure.GPIO_Pin = TRIG_PIN6;					
	GPIO_Init(TRIG_PORT6, &GPIO_InitStructure);			
}



/*
 * 函数名：dealTIM_ICUserValueStructureData
 * 描述  ：将捕捉到的时间数据转化为距离
 * 输入  ：TIM_ICUserValueStructurex ：TIM_ICUserValueStructure1-6
			i：对应超声波序号
 * 输出  ：无	
 */
void dealTIM_ICUserValueStructureData(TIM_ICUserValueTypeDef TIM_ICUserValueStructurex, int i)
{

	uint32_t time;
	double ftime;
	// 计算高电平时间的计数器的值
	time = TIM_ICUserValueStructurex.Capture_CcrValue+1;
	UltrasonicWave_Distance[i] = time * 340 / 2  * 100 / TIM_PscCLK ;
	// 打印高电平脉宽时间
	ftime = ((double)time)/TIM_PscCLK;
//	UltrasonicWave_Distance[i] = ftime * 340 / 2  * 100;
//	printf( "\r\n time %d us\r\n", time );	
//	printf( "\r\n ftime %lf\r\n",ftime );
	printf( "\r\n%d : distance %d\r\n",i, UltrasonicWave_Distance[i]);
//	printf( "\r\n：%d.%d s\r\n",time/TIM_PscCLK,time%TIM_PscCLK );	
}

//void dealTIM_ICUserValueStructureData(TIM_ICUserValueTypeDef TIM_ICUserValueStructurex, int i)
//{
//		uint32_t time;
//	

//	
//	
//	if(TIM_ICUserValueStructure.Capture_FinishFlag == 1)
//		{
//			// 计算高电平时间的计数器的值
//			time = TIM_ICUserValueStructure.Capture_Period * (GENERAL_TIM_PERIOD+1) + 
//			       (TIM_ICUserValueStructure.Capture_CcrValue+1);
//			
//			// 打印高电平脉宽时间
//			printf ( "\r\n：%d.%4d s\r\n",time/TIM_PscCLK,time%TIM_PscCLK );
//			
//			TIM_ICUserValueStructure.Capture_FinishFlag = 0;			
//		}		
//}

/*
 * 函数名：UltrasonicWave_StartMeasure
 * 描述  ：开始测距，打开中断，并发送一个>10us的脉冲
 * 输入  ：port = TRIG_PORTX ,pin = TRIG_PINX
 * 输出  ：无	
 */
void UltrasonicWave_StartMeasure(GPIO_TypeDef *  port, int32_t pin)
{
  GPIO_SetBits(port,pin); 		  //送>10US的高电平RIG_PORT,TRIG_PIN这两个在define中有?
  delayUs(10);		                      //延时10US以上
  GPIO_ResetBits(port,pin);

}



///*
// * 函数名：isObase
// * 描述  ：判断同组超声波是否存在障碍物
// * 输入  ：同组i, j超声波序号
// * 输出  ：0 无障碍物， 1 i前方有障碍物， 2 j前方有障碍物， 3 i和j前方均有障碍物
// */
//int isObase(int i, int j)
//{
//	int num = 0;
//	if( UltrasonicWave_Distance[i] < MAX_DISTACE)
//	{
//		num += 1;
//	}	
//	if( UltrasonicWave_Distance[j] < MAX_DISTACE)
//	{
//		num += 2;
//	}
//	return num;
//}
///*
// * 函数名：hasObase
// * 描述  ：判断是否存在障碍物
// * 输入  ： num【i】记录对应超声波模块障碍物情况，0 无障碍物， 1 i前方有障碍物， 2 j前方有障碍物， 3 i和j前方均有障碍物
// * 输出  ：无
// */
//void hasObase(int* num)
//{
//	int i;
//	for( i = 0; i < AVER_NUM; i+=2 )
//	{
//		num[i/2] = isObase(i, i+1);
//	}

//}

//获取各个超声波测距距离，单位cm
static void getDistance(int* num)
{
	int i =0; 
	for( i = 0; i < ULTR_NUM; i++ )
	{
		num[i] = UltrasonicWave_Distance[i];
	     printf( "\r\n%d :     %dcm\r\n",i, num[i]);
	}
}




/*
 * 函数名：addDistance
 * 描述  ：增加检测距离，最大不超过250厘米
 * 输入  ：num 检测范围增加num厘米
 * 输出  ：修改后的检测范围
 */
int addDistance(int distance)
{
	MAX_DISTACE += distance;
	if( MAX_DISTACE > 250 )
		MAX_DISTACE = 250;
	return MAX_DISTACE;
}


/*
 * 函数名：addDistance
 * 描述  ：减小检测距离，最大不超过250厘米
 * 输入  ：distance 检测范围减小distanc厘米
 * 输出  ：修改后的检测范围
 */
int minusDistance(int distance)
{
	MAX_DISTACE -= distance;
	if( MAX_DISTACE <30 )
		MAX_DISTACE = 30;
	return MAX_DISTACE;
}

/*
 * 函数名：getDistance
 * 描述  ：获得当前检测范围
 * 输入  ：无
 * 输出  ：当前的检测范围
 */
int getRange()
{
	return MAX_DISTACE;
}


/****************************************************************************
* 名    称：void UltrasonicWave(void *arg)
* 功    能：超声波测距线程
* 入口参数：无
* 出口参数：无
* 说    明：
* 调用方法：无 
****************************************************************************/
void UltrasonicWave(int* num)
{
	static int8_t tag = 0;
	int i= 0;	
	if( TIM_ICUserValueStructure.Capture_FinishFlag == 1 )  
	{
		TIM_ICUserValueStructure.Capture_FinishFlag = 0;
	    dealTIM_ICUserValueStructureData(TIM_ICUserValueStructure, tag);
	}
	else if( TIM_ICUserValueStructure2.Capture_FinishFlag == 1 )  
	{
		TIM_ICUserValueStructure2.Capture_FinishFlag = 0;
	    dealTIM_ICUserValueStructureData(TIM_ICUserValueStructure2, tag);
	}	
//	getDistance(num);                     //获取障碍物距离
	switch(tag)          //开始测距，发送一个>10us的脉冲，
	{
		case 0: UltrasonicWave_StartMeasure(TRIG_PORT1,TRIG_PIN1); break;
		case 1: UltrasonicWave_StartMeasure(TRIG_PORT2,TRIG_PIN2); break;
		case 2: UltrasonicWave_StartMeasure(TRIG_PORT3,TRIG_PIN3); break;
		case 3: UltrasonicWave_StartMeasure(TRIG_PORT4,TRIG_PIN4); break;
		case 4: UltrasonicWave_StartMeasure(TRIG_PORT5,TRIG_PIN5); break;
		case 5: UltrasonicWave_StartMeasure(TRIG_PORT6,TRIG_PIN6); break;
	}
	tag = (tag +1) % ULTR_NUM;
	

////////调试开关//////////////
#ifdef DEBUG_ON_OFF 
#undef  DEBUG_ON_OFF
#endif
#define DEBUG_ON_OFF 0
//////////////////////////////
	
	p_debug("\r\n tag : %d \r\n", tag);
	
}


/******************* (C) 1209 Lab *****END OF FILE************/
