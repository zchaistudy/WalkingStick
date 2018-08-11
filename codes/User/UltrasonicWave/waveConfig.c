
#include "waveConfig.h"
#include "UltrasonicWave.h"
// 定时器输入捕获用户自定义变量结构体定义
TIM_ICUserValueTypeDef TIM_ICUserValueStructure[ULTR_NUM];

// 中断优先级配置
static void GENERAL1_TIM_NVIC_Config(void)
{
    NVIC_InitTypeDef NVIC_InitStructure; 
		// 设置中断来源
    NVIC_InitStructure.NVIC_IRQChannel = GENERAL1_TIM_IRQ ;	
		// 设置主优先级为 0
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;	 
	  // 设置抢占优先级为3
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;	
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);
}

static void GENERAL2_TIM_NVIC_Config(void)
{
    NVIC_InitTypeDef NVIC_InitStructure; 	
		// 设置中断来源
    NVIC_InitStructure.NVIC_IRQChannel = GENERAL2_TIM_IRQ ;	
		// 设置主优先级为 0
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;	 
	  // 设置抢占优先级为3
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;	
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);
}

static void GENERAL1_TIM_GPIO_Config(void) 
{
  GPIO_InitTypeDef GPIO_InitStructure;

  // 输入捕获通道 GPIO 初始化
  RCC_APB2PeriphClockCmd(GENERAL1_TIM_CH1_GPIO_CLK, ENABLE);
  GPIO_InitStructure.GPIO_Pin =  GENERAL1_TIM_CH1_PIN;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
  GPIO_Init(GENERAL1_TIM_CH1_PORT, &GPIO_InitStructure);	
	
  RCC_APB2PeriphClockCmd(GENERAL1_TIM_CH2_GPIO_CLK, ENABLE);
  GPIO_InitStructure.GPIO_Pin =  GENERAL1_TIM_CH2_PIN;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
  GPIO_Init(GENERAL1_TIM_CH2_PORT, &GPIO_InitStructure);	

  RCC_APB2PeriphClockCmd(GENERAL1_TIM_CH3_GPIO_CLK, ENABLE);
  GPIO_InitStructure.GPIO_Pin =  GENERAL1_TIM_CH3_PIN;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
  GPIO_Init(GENERAL1_TIM_CH3_PORT, &GPIO_InitStructure);	
	
  RCC_APB2PeriphClockCmd(GENERAL1_TIM_CH4_GPIO_CLK, ENABLE);
  GPIO_InitStructure.GPIO_Pin =  GENERAL1_TIM_CH4_PIN;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
  GPIO_Init(GENERAL1_TIM_CH4_PORT, &GPIO_InitStructure);	
}


static void GENERAL2_TIM_GPIO_Config(void) 
{
  GPIO_InitTypeDef GPIO_InitStructure;

  // 输入捕获通道 GPIO 初始化
  RCC_APB2PeriphClockCmd(GENERAL2_TIM_CH3_GPIO_CLK, ENABLE);
  GPIO_InitStructure.GPIO_Pin =  GENERAL2_TIM_CH3_PIN;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
  GPIO_Init(GENERAL2_TIM_CH3_PORT, &GPIO_InitStructure);	
	
//  RCC_APB2PeriphClockCmd(GENERAL2_TIM_CH4_GPIO_CLK, ENABLE);
//  GPIO_InitStructure.GPIO_Pin =  GENERAL2_TIM_CH4_PIN;
//  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
//  GPIO_Init(GENERAL2_TIM_CH4_PORT, &GPIO_InitStructure);	
}

///*
// * 注意：TIM_TimeBaseInitTypeDef结构体里面有5个成员，TIM6和TIM7的寄存器里面只有
// * TIM_Prescaler和TIM_Period，所以使用TIM6和TIM7的时候只需初始化这两个成员即可，
// * 另外三个成员是通用定时器和高级定时器才有.
// *-----------------------------------------------------------------------------
// *typedef struct
// *{ TIM_Prescaler            都有
// *	TIM_CounterMode			     TIMx,x[6,7]没有，其他都有
// *  TIM_Period               都有
// *  TIM_ClockDivision        TIMx,x[6,7]没有，其他都有
// *  TIM_RepetitionCounter    TIMx,x[1,8,15,16,17]才有
// *}TIM_TimeBaseInitTypeDef; 
// *-----------------------------------------------------------------------------
// */

/* ----------------   PWM信号 周期和占空比的计算--------------- */
// ARR ：自动重装载寄存器的值
// CLK_cnt：计数器的时钟，等于 Fck_int / (psc+1) = 72M/(psc+1)
// PWM 信号的周期 T = ARR * (1/CLK_cnt) = ARR*(PSC+1) / 72M
// 占空比P=CCR/(ARR+1)

static void GENERAL1_TIM_Mode_Config(void)
{
		TIM_ICInitTypeDef TIM_ICInitStructure;
	 TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
  // 开启定时器时钟,即内部时钟CK_INT=72M
	GENERAL1_TIM_APBxClock_FUN(GENERAL1_TIM_CLK,ENABLE);

/*--------------------时基结构体初始化-------------------------*/	
	// 自动重装载寄存器的值，累计TIM_Period+1个频率后产生一个更新或者中断
	TIM_TimeBaseStructure.TIM_Period=GENERAL1_TIM_PERIOD;	
	// 驱动CNT计数器的时钟 = Fck_int/(psc+1)
	TIM_TimeBaseStructure.TIM_Prescaler= GENERAL1_TIM_PSC;	
	// 时钟分频因子 ，配置死区时间时需要用到
	TIM_TimeBaseStructure.TIM_ClockDivision=TIM_CKD_DIV1;		
	// 计数器计数模式，设置为向上计数
	TIM_TimeBaseStructure.TIM_CounterMode=TIM_CounterMode_Up;		
	// 重复计数器的值，没用到不用管
	TIM_TimeBaseStructure.TIM_RepetitionCounter=0;	
	// 初始化定时器
	TIM_TimeBaseInit(GENERAL1_TIM, &TIM_TimeBaseStructure);

	/*--------------------输入捕获结构体初始化-------------------*/	
	TIM_ICInitStructure.TIM_ICPolarity = GENERAL_TIM_STRAT_ICPolarity;
	// 输入通道和捕获通道的映射关系，有直连和非直连两种
	TIM_ICInitStructure.TIM_ICSelection = TIM_ICSelection_DirectTI;
	// 输入的需要被捕获的信号的分频系数
	TIM_ICInitStructure.TIM_ICPrescaler = TIM_ICPSC_DIV1;
	// 输入的需要被捕获的信号的滤波系数
	TIM_ICInitStructure.TIM_ICFilter = 0;
	
	TIM_ICInitStructure.TIM_Channel = GENERAL1_TIM_CHANNEL_1;    // 输入捕获信号的极性配置
	// 定时器输入捕获初始化
	TIM_ICInit(GENERAL1_TIM, &TIM_ICInitStructure);
	TIM_ICInitStructure.TIM_Channel = GENERAL1_TIM_CHANNEL_2;    // 输入捕获信号的极性配置
	// 定时器输入捕获初始化
	TIM_ICInit(GENERAL1_TIM, &TIM_ICInitStructure);
	TIM_ICInitStructure.TIM_Channel = GENERAL1_TIM_CHANNEL_3;    // 输入捕获信号的极性配置
	// 定时器输入捕获初始化
	TIM_ICInit(GENERAL1_TIM, &TIM_ICInitStructure);
	TIM_ICInitStructure.TIM_Channel = GENERAL1_TIM_CHANNEL_4;    // 输入捕获信号的极性配置
	// 定时器输入捕获初始化
	TIM_ICInit(GENERAL1_TIM, &TIM_ICInitStructure);	
	// 清除更新和捕获中断标志位
  TIM_ClearFlag(GENERAL1_TIM, TIM_FLAG_Update|GENERAL1_TIM_IT_CC1|GENERAL1_TIM_IT_CC2|GENERAL1_TIM_IT_CC3|GENERAL1_TIM_IT_CC4);	
  // 开启更新和捕获中断  
	TIM_ITConfig (GENERAL1_TIM, TIM_IT_Update | GENERAL1_TIM_IT_CC1|GENERAL1_TIM_IT_CC2|GENERAL1_TIM_IT_CC3|GENERAL1_TIM_IT_CC4, ENABLE );
	
	// 使能计数器
	TIM_Cmd(GENERAL1_TIM, ENABLE);
}


static void GENERAL2_TIM_Mode_Config(void)
{
   TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	TIM_ICInitTypeDef TIM_ICInitStructure;
  // 开启定时器时钟,即内部时钟CK_INT=72M
	GENERAL2_TIM_APBxClock_FUN(GENERAL2_TIM_CLK,ENABLE);

/*--------------------时基结构体初始化-------------------------*/	
	// 自动重装载寄存器的值，累计TIM_Period+1个频率后产生一个更新或者中断
	TIM_TimeBaseStructure.TIM_Period=GENERAL2_TIM_PERIOD;	
	// 驱动CNT计数器的时钟 = Fck_int/(psc+1)
	TIM_TimeBaseStructure.TIM_Prescaler= GENERAL2_TIM_PSC;	
	// 时钟分频因子 ，配置死区时间时需要用到
	TIM_TimeBaseStructure.TIM_ClockDivision=TIM_CKD_DIV1;		
	// 计数器计数模式，设置为向上计数
	TIM_TimeBaseStructure.TIM_CounterMode=TIM_CounterMode_Up;		
	// 重复计数器的值，没用到不用管
	TIM_TimeBaseStructure.TIM_RepetitionCounter=0;	
	// 初始化定时器
	TIM_TimeBaseInit(GENERAL2_TIM, &TIM_TimeBaseStructure);

	/*--------------------输入捕获结构体初始化-------------------*/	
	TIM_ICInitStructure.TIM_ICPolarity = GENERAL_TIM_STRAT_ICPolarity;
	// 输入通道和捕获通道的映射关系，有直连和非直连两种
	TIM_ICInitStructure.TIM_ICSelection = TIM_ICSelection_DirectTI;
	// 输入的需要被捕获的信号的分频系数
	TIM_ICInitStructure.TIM_ICPrescaler = TIM_ICPSC_DIV1;
	// 输入的需要被捕获的信号的滤波系数
	TIM_ICInitStructure.TIM_ICFilter = 0;
	
	TIM_ICInitStructure.TIM_Channel = GENERAL2_TIM_CHANNEL_3;    // 输入捕获信号的极性配置
	// 定时器输入捕获初始化
//	TIM_ICInit(GENERAL2_TIM, &TIM_ICInitStructure);
//	TIM_ICInitStructure.TIM_Channel = GENERAL2_TIM_CHANNEL_4;    // 输入捕获信号的极性配置
	// 定时器输入捕获初始化
	TIM_ICInit(GENERAL2_TIM, &TIM_ICInitStructure);	
	// 清除更新和捕获中断标志位
  TIM_ClearFlag(GENERAL2_TIM, TIM_FLAG_Update|GENERAL2_TIM_IT_CC3);	
  // 开启更新和捕获中断  
	TIM_ITConfig (GENERAL2_TIM, TIM_IT_Update|GENERAL2_TIM_IT_CC3, ENABLE );
	
	// 使能计数器
	TIM_Cmd(GENERAL2_TIM, ENABLE);
}

void GENERAL_TIM_Init(void)
{
	GENERAL1_TIM_GPIO_Config();
	GENERAL1_TIM_NVIC_Config();
	GENERAL1_TIM_Mode_Config();		
	
	GENERAL2_TIM_GPIO_Config();
	GENERAL2_TIM_NVIC_Config();
	GENERAL2_TIM_Mode_Config();		
}

/*********************************************END OF FILE**********************/


void TIM5_Init()
{
    TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	NVIC_InitTypeDef NVIC_InitStructure;

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM5, ENABLE); //时钟使能

	TIM_TimeBaseStructure.TIM_Period = 999; //设置在下一个更新事件装入活动的自动重装载寄存器周期的值	 计数到5000为500ms
	TIM_TimeBaseStructure.TIM_Prescaler =7199; //设置用来作为TIMx时钟频率除数的预分频值  10Khz的计数频率  
	TIM_TimeBaseStructure.TIM_ClockDivision = 0; //设置时钟分割:TDTS = Tck_tim
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  //TIM向上计数模式
	TIM_TimeBaseInit(TIM5, &TIM_TimeBaseStructure); //根据TIM_TimeBaseInitStruct中指定的参数初始化TIMx的时间基数单位
 
	TIM_ITConfig(  //使能或者失能指定的TIM中断
		TIM5, //TIM5
		TIM_IT_Update ,
		ENABLE  //使能
		);
	NVIC_InitStructure.NVIC_IRQChannel = TIM5_IRQn;  //TIM3中断
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2;  //先占优先级0级
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2;  //从优先级3级
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; //IRQ通道被使能
	NVIC_Init(&NVIC_InitStructure);  //根据NVIC_InitStruct中指定的参数初始化外设NVIC寄存器

	TIM_Cmd(TIM5, ENABLE);  //使能TIMx外设					 
}



/*
 * 函数名：UltrasonicWave_Configuration
 * 描述  ：超声波模块的初始化
 * 输入  ：无
 * 输出  ：无	
 */
static void UltrasonicWave_Configuration(void)
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

//测距功能初始化
void WaveConfig()
{
	UltrasonicWave_Configuration();			//初始化引脚
	GENERAL_TIM_Init();	
	TIM5_Init();
}
