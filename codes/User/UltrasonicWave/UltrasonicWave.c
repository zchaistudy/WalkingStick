/************************************************************
data 2018.6.30
************************************************************/
#include "UltrasonicWave.h"
#include "debug.h"


////////调试开关//////////////
#ifdef DEBUG_ON_OFF 
#undef  DEBUG_ON_OFF
#define DEBUG_ON_OFF 1
#endif
//////////////////////////////       

int UltrasonicWave_Distance[ULTR_NUM] = {1,2,3};      //计算出的距离 
uint8_t MEASURE_FINISH = 0;               //测距完成标志
#ifdef ONLY_WALKINGSTICK                 //拐杖单独测试
int8_t  MEASURE_FLAG = 1;          //测距请求标志
#else
int8_t  MEASURE_FLAG;          //测距请求标志
#endif
/*
 * 函数名：dealTIM_ICUserValueStructureData
 * 描述  ：将捕捉到的时间数据转化为距离
 * 输入  ：TIM_ICUserValueStructurex ：TIM_ICUserValueStructure1-6
 * 输出  ：无	
 */
static void dealTIM_ICUserValueStructureData(TIM_ICUserValueTypeDef TIM_ICUserValueStructurex, int i)
{

	uint32_t time;
	// 计算高电平时间的计数器的值
	time = TIM_ICUserValueStructurex.Capture_CcrValue+1;
	UltrasonicWave_Distance[i] = time * 340 / 2  * 100 / TIM_PscCLK ;
	// 打印高电平脉宽时间
//	ftime = ((double) TIM_ICUserValueStructurex.Capture_CcrValue+1)/TIM_PscCLK;
	p_debug( "%d :  distance   %d",i, UltrasonicWave_Distance[i]);
//	printf( "\r\n：%d us\r\n",time );	
}



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

/****************************************************************************
* 名    称：void UltrasonicWave(void *arg)
* 功    能：超声波测距线程
* 入口参数：无
* 出口参数：无
* 说    明：
* 调用方法：无 
****************************************************************************/
#if 0
void UltrasonicWave(int measureNum)
{
	int i= 0;	
	for( i = 0; i < ULTR_NUM; i++ )
	{
		if( TIM_ICUserValueStructure[i].Capture_FinishFlag == 1 )  
		{	
			dealTIM_ICUserValueStructureData(TIM_ICUserValueStructure[i] ,i);
			TIM_ICUserValueStructure[i].Capture_FinishFlag = 0;
            if( i == ULTR_NUM - 1)
			{
				MEASURE_FINISH  = 1;
			}
		}		
	}
	UltrasonicWave_StartMeasure(TRIG_PORT1,TRIG_PIN1);
	UltrasonicWave_StartMeasure(TRIG_PORT2,TRIG_PIN2);
	UltrasonicWave_StartMeasure(TRIG_PORT4,TRIG_PIN4);
	UltrasonicWave_StartMeasure(TRIG_PORT5,TRIG_PIN5);
//	switch(measureNum)          //开始测距，发送一个>10us的脉冲，
//	{
//		case 0: UltrasonicWave_StartMeasure(TRIG_PORT1,TRIG_PIN1); break;
//		case 1: UltrasonicWave_StartMeasure(TRIG_PORT2,TRIG_PIN2); break;
//		case 2: UltrasonicWave_StartMeasure(TRIG_PORT3,TRIG_PIN3); break;
//		case 3: UltrasonicWave_StartMeasure(TRIG_PORT4,TRIG_PIN4); break;
//		case 4: UltrasonicWave_StartMeasure(TRIG_PORT5,TRIG_PIN5); break;
//	}	
}


#else
void UltrasonicWave(int measureNum)
{
	UltrasonicWave_StartMeasure(TRIG_PORT1,TRIG_PIN1);
	UltrasonicWave_StartMeasure(TRIG_PORT2,TRIG_PIN2);
	UltrasonicWave_StartMeasure(TRIG_PORT3,TRIG_PIN3);
	UltrasonicWave_StartMeasure(TRIG_PORT4,TRIG_PIN4);
	UltrasonicWave_StartMeasure(TRIG_PORT5,TRIG_PIN5);
}
//判断测距是否完成
// 返回： 1： 测距完成 0 测距未完成
int IsFinishMeasure()
{
	int i= 0;	
	for( i = 0; i < ULTR_NUM; i++ )
	{
		if( TIM_ICUserValueStructure[i].Capture_FinishFlag == 1 )  
		{	
			dealTIM_ICUserValueStructureData(TIM_ICUserValueStructure[i] ,i);
			TIM_ICUserValueStructure[i].Capture_FinishFlag = 0;
            MEASURE_FINISH++;              //每完成测距则加1
		}		
	}
//	p_debug("mearsureflag: %d", MEASURE_FINISH);
	if(MEASURE_FINISH >= ULTR_NUM )
	{
		MEASURE_FINISH = 0;
		return 1;
	}
    return 0;  
}
#endif

/******************* (C) 1209 Lab *****END OF FILE************/
