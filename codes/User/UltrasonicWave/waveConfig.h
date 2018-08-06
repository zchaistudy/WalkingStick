#ifndef __BSP_GENERALTIME_H
#define __BSP_GENERALTIME_H


#include "stm32f10x.h"


#define TIM_PscCLK   (72000000 / (GENERAL1_TIM_PSC+1))
/************通用定时器TIM参数定义，只限TIM2、3、4、5************/
// 当使用不同的定时器的时候，对应的GPIO是不一样的，这点要注意
// 我们这里默认使用TIM5

#define            GENERAL1_TIM                   TIM3
#define            GENERAL1_TIM_APBxClock_FUN     RCC_APB1PeriphClockCmd
#define            GENERAL1_TIM_CLK               RCC_APB1Periph_TIM3
#define            GENERAL1_TIM_PERIOD            0XFFFF
#define            GENERAL1_TIM_PSC              (72-1)

// TIM 输入捕获通道GPIO相关宏定义
#define            GENERAL1_TIM_CH1_GPIO_CLK      RCC_APB2Periph_GPIOA
#define            GENERAL1_TIM_CH1_PORT          GPIOA
#define            GENERAL1_TIM_CH1_PIN           GPIO_Pin_6
#define            GENERAL1_TIM_CHANNEL_1         TIM_Channel_1

#define            GENERAL1_TIM_CH2_GPIO_CLK      RCC_APB2Periph_GPIOA
#define            GENERAL1_TIM_CH2_PORT          GPIOA
#define            GENERAL1_TIM_CH2_PIN           GPIO_Pin_7
#define            GENERAL1_TIM_CHANNEL_2         TIM_Channel_2

#define            GENERAL1_TIM_CH3_GPIO_CLK      RCC_APB2Periph_GPIOB
#define            GENERAL1_TIM_CH3_PORT          GPIOB
#define            GENERAL1_TIM_CH3_PIN           GPIO_Pin_0
#define            GENERAL1_TIM_CHANNEL_3         TIM_Channel_3

#define            GENERAL1_TIM_CH4_GPIO_CLK      RCC_APB2Periph_GPIOB
#define            GENERAL1_TIM_CH4_PORT          GPIOB
#define            GENERAL1_TIM_CH4_PIN           GPIO_Pin_1
#define            GENERAL1_TIM_CHANNEL_4         TIM_Channel_4
// 中断相关宏定义
#define            GENERAL1_TIM_IT_CC1            TIM_IT_CC1
#define            GENERAL1_TIM_IT_CC2            TIM_IT_CC2
#define            GENERAL1_TIM_IT_CC3            TIM_IT_CC3
#define            GENERAL1_TIM_IT_CC4            TIM_IT_CC4
#define            GENERAL1_TIM_IRQ               TIM3_IRQn
#define            GENERAL1_TIM_INT_FUN           TIM3_IRQHandler

// 获取捕获寄存器值函数宏定义
#define            GENERAL1_TIM_GetCapture1_FUN                 TIM_GetCapture1
#define            GENERAL1_TIM_GetCapture2_FUN                 TIM_GetCapture2
#define            GENERAL1_TIM_GetCapture3_FUN                 TIM_GetCapture3
#define            GENERAL1_TIM_GetCapture4_FUN                 TIM_GetCapture4
// 捕获信号极性函数宏定义
#define            GENERAL1_TIM_OC1PolarityConfig_FUN           TIM_OC1PolarityConfig
#define            GENERAL1_TIM_OC2PolarityConfig_FUN           TIM_OC2PolarityConfig
#define            GENERAL1_TIM_OC3PolarityConfig_FUN           TIM_OC3PolarityConfig
#define            GENERAL1_TIM_OC4PolarityConfig_FUN           TIM_OC4PolarityConfig

/**********************************定时器4*****************************/

#define            GENERAL2_TIM                   TIM4
#define            GENERAL2_TIM_APBxClock_FUN     RCC_APB1PeriphClockCmd
#define            GENERAL2_TIM_CLK               RCC_APB1Periph_TIM4
#define            GENERAL2_TIM_PERIOD            0XFFFF
#define            GENERAL2_TIM_PSC              (72-1)

// TIM 输入捕获通道GPIO相关宏定义
#define            GENERAL2_TIM_CH3_GPIO_CLK      RCC_APB2Periph_GPIOB
#define            GENERAL2_TIM_CH3_PORT          GPIOB
#define            GENERAL2_TIM_CH3_PIN           GPIO_Pin_8
#define            GENERAL2_TIM_CHANNEL_3         TIM_Channel_3

#define            GENERAL2_TIM_CH4_GPIO_CLK      RCC_APB2Periph_GPIOB
#define            GENERAL2_TIM_CH4_PORT          GPIOB
#define            GENERAL2_TIM_CH4_PIN           GPIO_Pin_1
#define            GENERAL2_TIM_CHANNEL_4         TIM_Channel_4
// 中断相关宏定义
#define            GENERAL2_TIM_IT_CC3            TIM_IT_CC3
#define            GENERAL2_TIM_IT_CC4            TIM_IT_CC4
#define            GENERAL2_TIM_IRQ               TIM4_IRQn
#define            GENERAL2_TIM_INT_FUN           TIM4_IRQHandler

// 获取捕获寄存器值函数宏定义
#define            GENERAL2_TIM_GetCapture1_FUN                 TIM_GetCapture1
#define            GENERAL2_TIM_GetCapture2_FUN                 TIM_GetCapture2
#define            GENERAL2_TIM_GetCapture3_FUN                 TIM_GetCapture3
#define            GENERAL2_TIM_GetCapture4_FUN                 TIM_GetCapture4
// 捕获信号极性函数宏定义
#define            GENERAL2_TIM_OC1PolarityConfig_FUN           TIM_OC1PolarityConfig
#define            GENERAL2_TIM_OC2PolarityConfig_FUN           TIM_OC2PolarityConfig
#define            GENERAL2_TIM_OC3PolarityConfig_FUN           TIM_OC3PolarityConfig
#define            GENERAL2_TIM_OC4PolarityConfig_FUN           TIM_OC4PolarityConfig

// 测量的起始边沿
#define            GENERAL_TIM_STRAT_ICPolarity                TIM_ICPolarity_Rising
// 测量的结束边沿
#define            GENERAL_TIM_END_ICPolarity                  TIM_ICPolarity_Falling


// 定时器输入捕获用户自定义变量结构体声明
typedef struct
{   
	uint8_t   Capture_FinishFlag;   // 捕获结束标志位
	uint8_t   Capture_StartFlag;    // 捕获开始标志位
	uint16_t  Capture_First;     //第一次 捕获寄存器的值
	uint16_t  Capture_CcrValue;       //两次寄存器捕获差值  
}TIM_ICUserValueTypeDef;

extern TIM_ICUserValueTypeDef TIM_ICUserValueStructure[];


/**************************函数声明********************************/
void WaveConfig(void);




#define	TRIG_PORT1     			  GPIOC		//TRIG       
#define	TRIG_PIN1      			  GPIO_Pin_0   //TRIG       

#define	TRIG_PORT2     			  GPIOC		//TRIG       
#define	TRIG_PIN2      			  GPIO_Pin_1   //TRIG     

#define	TRIG_PORT3     			  GPIOC		//TRIG       
#define	TRIG_PIN3      			  GPIO_Pin_2   //TRIG       

#define	TRIG_PORT4     			  GPIOC		//TRIG       
#define	TRIG_PIN4      			  GPIO_Pin_3   //TRIG     

#define	TRIG_PORT5     			  GPIOC		//TRIG       
#define	TRIG_PIN5     			  GPIO_Pin_4   //TRIG 

#define	TRIG_PORT6     			  GPIOC		//TRIG       
#define	TRIG_PIN6      			  GPIO_Pin_5   //TRIG 


#define RCC_APB2Periph_GPIOx  RCC_APB2Periph_GPIOC

#endif	/* __BSP_GENERALTIME_H */


