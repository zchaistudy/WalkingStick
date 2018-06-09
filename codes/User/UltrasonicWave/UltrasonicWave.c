#include "UltrasonicWave.h"
#include "debug.h"

////////调试开关//////////////
#ifdef DEBUG_ON_OFF 
#undef  DEBUG_ON_OFF
#define DEBUG_ON_OFF 1
#endif
//////////////////////////////       

int UltrasonicWave_Distance[ULTR_NUM];      //计算出的距离    
int8_t  MEASURE_FLAG = 0;          //测距请求标志
/*
 * 函数名：dealTIM_ICUserValueStructureData
 * 描述  ：将捕捉到的时间数据转化为距离
 * 输入  ：TIM_ICUserValueStructurex ：TIM_ICUserValueStructure1-6
 * 输出  ：无	
 */
static void dealTIM_ICUserValueStructureData(TIM_ICUserValueTypeDef TIM_ICUserValueStructurex)
{

	uint32_t time;
	int i;
	// 计算高电平时间的计数器的值
	i = TIM_ICUserValueStructurex.Capture_CCx - 1;
	time = TIM_ICUserValueStructurex.Capture_CcrValue+1;
	UltrasonicWave_Distance[i] = time * 340 / 2  * 100 / TIM_PscCLK ;
	// 打印高电平脉宽时间
	printf( "\r\n%d : distance %d\r\n",i, UltrasonicWave_Distance[i]);
//	printf( "\r\n：%d.%d s\r\n",time/TIM_PscCLK,time%TIM_PscCLK );	
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
void UltrasonicWave(int measureNum)
{
	int i= 0;	
	for( i = 0; i < ULTR_NUM; i++ )
	{
		if( TIM_ICUserValueStructure[i].Capture_FinishFlag == 1 )  
		{	
			dealTIM_ICUserValueStructureData(TIM_ICUserValueStructure[i]);
			TIM_ICUserValueStructure[i].Capture_FinishFlag = 0;
		}		
	}
	
	switch(measureNum)          //开始测距，发送一个>10us的脉冲，
	{
		case 0: UltrasonicWave_StartMeasure(TRIG_PORT1,TRIG_PIN1); break;
		case 1: UltrasonicWave_StartMeasure(TRIG_PORT2,TRIG_PIN2); break;
		case 2: UltrasonicWave_StartMeasure(TRIG_PORT3,TRIG_PIN3); break;
		case 3: UltrasonicWave_StartMeasure(TRIG_PORT4,TRIG_PIN4); break;
		case 4: UltrasonicWave_StartMeasure(TRIG_PORT5,TRIG_PIN5); break;
	}	
}


/******************* (C) 1209 Lab *****END OF FILE************/
