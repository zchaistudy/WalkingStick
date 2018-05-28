#ifndef DEBUG_H_
#define DEBUG_H_
#include "stm32f10x.h"

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
#define p_debug(restrict,args...)  do{if(DEBUG_ON_OFF){printf(restrict,##args);}}while(0)


#endif
