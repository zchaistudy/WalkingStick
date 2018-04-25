#ifndef __ANGLE_DEFINE_H
#define	__ANGLE_DEFINE_H

#include "stm32f10x.h"
#include <stdio.h>
#include <math.h>   

#define   uchar unsigned char
#define   uint unsigned int	

 //磁场内部寄存器***********************************
/* Magnetometer registers */
#define CRA_REG_M		0x00	/* Configuration register A */
#define CRB_REG_M		0x01	/* Configuration register B */
#define MR_REG_M		0x02	/* Mode register */
/* resume state index */
#define RES_CRA_REG_M		0	/* Configuration register A */
#define RES_CRB_REG_M		1	/* Configuration register B */
#define RES_MR_REG_M		2	/* Mode register */
/* Output register start address*/
#define OUT_X_M			    0x03
/* Magnetic Sensor Operation Mode */
#define NORMAL_MODE     	0x00
#define POS_BIAS         	0x01
#define NEG_BIAS         	0x02
#define CC_MODE          	0x00
#define SC_MODE			    0x01
#define SLEEP_MODE		    0x03
 //加速度内部寄存器***********************************
#define AXISDATA_REG	  	    0x28
#define WHOAMI_LSM303DLH_ACC	0x32	/*	Expctd content for WAI	*/
/*	CONTROL REGISTERS	*/
#define WHO_AM_I		0x0F	/*	WhoAmI register		*/
#define CTRL_REG1		0x20	/*				*/
#define CTRL_REG2		0x21	/*				*/
#define CTRL_REG3		0x22	/*				*/
#define CTRL_REG4		0x23	/*				*/
#define	CTRL_REG5		0x24	/*				*/

#define	INT_CFG1		0x30	/*	interrupt 1 config	*/
#define	INT_SRC1		0x31	/*	interrupt 1 source	*/
#define	INT_THS1		0x32	/*	interrupt 1 threshold	*/
#define	INT_DUR1		0x33	/*	interrupt 1 duration	*/

#define	INT_CFG2		0x34	/*	interrupt 2 config	*/
#define	INT_SRC2		0x35	/*	interrupt 2 source	*/
#define	INT_THS2		0x36	/*	interrupt 2 threshold	*/
#define	INT_DUR2		0x37	/*	interrupt 2 duration	*/

#define	M_SlaveAddress    0x3C	
#define	A_SlaveAddress    0x30	

#define FALSE 0
#define TRUE  1
#define Statue int
#define SCL_H         GPIOB->BSRR = GPIO_Pin_6
#define SCL_L         GPIOB->BRR  = GPIO_Pin_6 
   
#define SDA_H         GPIOB->BSRR = GPIO_Pin_7
#define SDA_L         GPIOB->BRR  = GPIO_Pin_7

#define SCL_read      GPIOB->IDR  & GPIO_Pin_6
#define SDA_read      GPIOB->IDR  & GPIO_Pin_7

#endif
