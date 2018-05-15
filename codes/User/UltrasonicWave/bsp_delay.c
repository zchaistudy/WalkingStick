/***************************************************************************************
 *	FileName					:	   delay.c
 *	CopyRight					:
 *	ModuleName					:	
 *
 *	CPU							:
 *	RTOS						:
 *
 *	Create Data					:	
 *	Author/Corportation			:	
 *
 *	Abstract Description		:	 延时函数
 *
 *--------------------------------Revision History--------------------------------------
 *	No	version		Data			Revised By			Item			Description
 *	
 *
 ***************************************************************************************/


/**************************************************************
*	Include File Section
**************************************************************/
#include "bsp_delay.h"

/**************************************************************
*	Macro Define Section
**************************************************************/
 
/**************************************************************
*	Struct Define Section
**************************************************************/


/**************************************************************
*	Prototype Declare Section
**************************************************************/


/**************************************************************
*	Global Variable Declare Section
**************************************************************/


/**************************************************************
*	File Static Variable Define Section
**************************************************************/
static uint8_t  fac_us=0;							//us延时倍乘数			   
static uint16_t fac_ms=0;							//ms延时倍乘数,在ucos下,代表每个节拍的ms数
	

/**************************************************************
*	Function Define Section
**************************************************************/

/**
*  @name  : void delayInit()
*	@description : 初始化延时函数
 *	@param		：none
 *	@return		:none
 *  @notice   : none
 */
 void delayInit()
{
//	SysTick_CLKSourceConfig(SysTick_CLKSource_HCLK_Div8);  //选择外部时钟，  HCLK/8
	fac_us=SystemCoreClock/8000000;				//为系统时钟的1/8  
	fac_ms=(u16)fac_us*1000;					//代表每个ms需要的systick时钟数
}

/**
 *  @name    void delayUs( uint32_t t )
 *	@description   us延时
 *	@param		t 延时tus	
 *	@return		none
 *  @notice   t us<=（2^24）/fac_us
 */
void delayUs( uint32_t t )
{
	uint32_t temp;

	SysTick->LOAD=t*fac_us; 					//时间加载		
    SysTick->VAL=0x00;        					//清空计数器
	SysTick->CTRL|=SysTick_CTRL_ENABLE_Msk ;	//开始倒数
	do
	{
		temp=SysTick->CTRL;
	}while((temp&0x01)&&!(temp&(1<<16)));		//避免死循环，等待时间到达  
	
	SysTick->CTRL&=~SysTick_CTRL_ENABLE_Msk;	//关闭计数器
	SysTick->VAL =0X00;      					 //清空计数器	
}

/**
 *  @name   void delayMs(uint16_t t)
 *	@description    ms延时
 *	@param		t 延时tms
 *	@return		none
 *  @notice  ： 最大延时 t<=0xffffff*8*1000/SYSCLK
 */
void delayMs(uint16_t t)
{
	uint32_t temp;		   
	SysTick->LOAD=(u32)t*fac_ms;				//时间加载(SysTick->LOAD为24bit)
	SysTick->VAL =0x00;							//清空计数器
	SysTick->CTRL|=SysTick_CTRL_ENABLE_Msk ;	//开始倒数  

	do
	{
		temp=SysTick->CTRL;
	}while((temp&0x01)&&!(temp&(1<<16)));		//等待时间到达   

	SysTick->CTRL&=~SysTick_CTRL_ENABLE_Msk;	//关闭计数器
	SysTick->VAL =0X00;       					//清空计数器	 	
}
 
