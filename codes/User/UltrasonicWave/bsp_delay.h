/***************************************************************************************
 *	File Name				:		delay.c
 *	CopyRight				:
 *	ModuleName				:	
 *
 *	CPU						:
 *	RTOS					:
 *
 *	Create Data				:	
 *	Author/Corportation		:	
 *
 *	Abstract Description	:	延时函数头文件 
 *
 *--------------------------------Revision History--------------------------------------
 *	No	version		Data			Revised By			Item			Description
 *	
 *
 ***************************************************************************************/


/**************************************************************
*	Multi-Include-Prevent Section
**************************************************************/
#ifndef __DELAY_H
#define __DELAY_H

 
/**************************************************************
*	Include File Section
**************************************************************/
#include "stm32f10x.h"

/**************************************************************
*	Prototype Declare Section
**************************************************************/
/**
*  @name  : void delayInit()
*	@description : 初始化延时函数
 *	@param		：none
 *	@return		:none
 *  @notice   : none
 */
 void delayInit(void);
 
 /**
 *  @name    void delayUs( uint32_t t )
 *	@description   us延时
 *	@param		t 延时tus	
 *	@return		none
 *  @notice   t us<=（2^24）/fac_us
 */
void delayUs( uint32_t t );

/**
 *  @name   void delayMs(uint16_t t)
 *	@description    ms延时
 *	@param		t 延时tms
 *	@return		none
 *  @notice  ： 最大延时 t<=0xffffff*8*1000/SYSCLK
 */
void delayMs(uint16_t t);

/**************************************************************
*	End-Multi-Include-Prevent Section
**************************************************************/
#endif
