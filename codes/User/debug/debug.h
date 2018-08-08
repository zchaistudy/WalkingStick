#ifndef DEBUG_H_
#define DEBUG_H_
#include "stm32f10x.h"
#include "bsp_usart.h"

//#define DEBUG  0
#define  DEBUG_ON_OFF  0 

void CloseDebug(void);   //关闭调试
void OnDebug(void);    //打开调试
/**
////////调试开关//////////////
#ifdef DEBUG_ON_OFF 
#undef  DEBUG_ON_OFF
#endif
#define DEBUG_ON_OFF 1       //1打开调试。0关闭
//////////////////////////////
*/

#define  DEBUG_USART3_USART1_ENABLE 0      //通过串口3打印串口1蓝牙发送的数据


#define p_debug(restrict,args...)  do{if(DEBUG_ON_OFF){my_printf("\r\ndebug: "restrict"\r\n",##args);}}while(0)
#define p_err(restrict,args...)  do{if(DEBUG_ON_OFF){my_printf("file:%s line:%d: \r\n",__FILE__,__LINE__);my_printf(restrict,##args);}}while(0)

void my_printf(const char *format,...); 

#endif
