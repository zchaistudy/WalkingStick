#include "shake.h"
/**
  * @brief  配置TIM复用输出PWM时用到的I/O,这里使用的是TIM2
  * @param  无
  * @retval 无
  */
static void TIM2_GPIO_Config(void) 
{
  GPIO_InitTypeDef GPIO_InitStructure;

  /*  clock enable */
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA|RCC_APB2Periph_AFIO, ENABLE); 

  /* 配置呼吸灯用到的引脚 */
  GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_1 ;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;	// 复用推挽输出
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;

  GPIO_Init( GPIOA, &GPIO_InitStructure );
}


/**
  * @brief  配置嵌套向量中断控制器NVIC
  * @param  无
  * @retval 无
  */
static void NVIC_Config_PWM(void)
{
  NVIC_InitTypeDef NVIC_InitStructure;
  
  /* Configure one bit for preemption priority */
  NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
  
  /* 配置TIM3_IRQ中断为中断源 */
  NVIC_InitStructure.NVIC_IRQChannel = TIM2_IRQn;
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2;
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStructure);
}

/**
  * @brief  配置TIM输出的PWM信号的模式，如周期、极性
  * @param  无
  * @retval 无
  */

static void TIM2_Mode_Config(void)
{
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	TIM_OCInitTypeDef  TIM_OCInitStructure;																				
	
	
	/* 设置TIM3CLK 时钟 */
	RCC_APB1PeriphClockCmd ( RCC_APB1Periph_TIM2, ENABLE ); 

  /* 基本定时器配置 */		  
  TIM_TimeBaseStructure.TIM_Period = 9999;       							  //当定时器从0计数到 TIM_Period+1 ，为一个定时周期
  TIM_TimeBaseStructure.TIM_Prescaler = 0;	    							//设置预分频
  TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1 ;			//设置时钟分频系数：不分频(这里用不到)
  TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  	//向上计数模式
  TIM_TimeBaseInit(TIM2, &TIM_TimeBaseStructure);

  /* PWM模式配置 */
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;	    				//配置为PWM模式1
  TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;	//使能输出
  TIM_OCInitStructure.TIM_Pulse = 9999;				 						  		//设置初始PWM脉冲宽度为0	
  TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_Low;  	  //当定时器计数值小于CCR1_Val时为低电平

  TIM_OC2Init ( TIM2, &TIM_OCInitStructure );	 									//使能通道2
	

  TIM_OC2PreloadConfig ( TIM2, TIM_OCPreload_Enable );						//使能预装载	

  TIM_ARRPreloadConfig(TIM2, ENABLE);			 										//使能TIM重载寄存器ARR

  /* TIM3 enable counter */
  TIM_Cmd(TIM2, ENABLE);                   										//使能定时器	
	
	TIM_ITConfig(TIM2, TIM_IT_Update, ENABLE);										//使能update中断
		
	NVIC_Config_PWM();																					//配置中断优先级		
	
}

/**
  * @brief  TIM 呼吸灯初始化
  *         配置PWM模式和GPIO
  * @param  无
  * @retval 无
  */
void TIM2_Breathing_Init(void)
{
	TIM2_GPIO_Config();
	TIM2_Mode_Config();	
}


/**
  * @brief  TIM 呼吸灯初始化
  *         配置PWM模式和GPIO
  * @param  degree,用于描述震动的频率，分为1-4级，4级震动频率最大
  * @retval 无
  */
void AdjustVibrationFrequencyWalking(char degree)
{
	switch(degree)
	{
		case Zero:
			TIM2->CCR2 = 9999;
			break;	
		case First:
			TIM2->CCR2 = 7500;
			break;		
		case Second:
			TIM2->CCR2 = 5000;
			break;
		case Third:
			TIM2->CCR2 = 2500;
			break;
		case Forth:
			TIM2->CCR2 = 0;
			break;
		
//				case Forth:
//			TIM2->CCR2 = 9999;
//			break;	
//		case Third:
//			TIM2->CCR2 = 7500;
//			break;		
//		case Second:
//			TIM2->CCR2 = 5000;
//			break;
//		case First:
//			TIM2->CCR2 = 2500;
//			break;
//		case Zero:
//			TIM2->CCR2 = 0;
//			break;
	}
}
