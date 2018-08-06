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

/**************************************************************
*	Function Define Section
**************************************************************/



/**
 *  @name    void delayUs( uint32_t t )
 *	@description   us延时
 *	@param		t 延时tus	
 *	@return		none
 *  @notice   t us<=（2^24）/fac_us
 */
void delayUs( uint32_t t )
{

	int i;
    for( i = 0; i < t * 10; i++ )
 	{
		__NOP();
	}	
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
	int i;
    for( i = 0; i < t * 6000; i++ )
 	{
		__NOP();
	}		
}
 
