#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<stdarg.h>
#include "debug.h"
#include "bsp_usart.h"


static void printch(const char ch)   //输出字符
{  
    Usart3Send(ch);  
}  
 
 
static void printintraw(const int dec)     //输出整型数
{  
    if(dec == 0)  
    {  
        return;  
    }  
    printintraw(dec / 10);  
    Usart3Send((char)(dec % 10 + '0'));  
}  

static void printint(const int dec)     //输出整型数
{  
    if(dec == 0)  
    {  
		Usart3Send('0');  
        return;  
    }  
    printintraw(dec / 10);  
    Usart3Send((char)(dec % 10 + '0'));  
}  
 
 
static void printstr(const char *ptr)        //输出字符串
{  
    while(*ptr)  
    {  
        Usart3Send(*ptr);  
        ptr++;  
    }  
}  
 
 
static void printfloat(const float flt)     //输出浮点数，小数点第5位四舍五入
{  
    int tmpint = (int)flt;  
    int tmpflt = (int)(100000 * (flt - tmpint));  
    if(tmpflt % 10 >= 5)  
    {  
        tmpflt = tmpflt / 10 + 1;  
    }  
    else  
    {  
        tmpflt = tmpflt / 10;  
    }  
    printint(tmpint);  
    Usart3Send('.');  
    printint(tmpflt);  
 
}  

 void my_printf(const char *format,...)  
{  
    va_list ap;  
    va_start(ap,format);     //将ap指向第一个实际参数的地址
    while(*format)  
    {  
        if(*format != '%')  
        {  
            Usart3Send(*format);  
            format++;  
        }  
        else  
        {  
            format++;  
            switch(*format)  
            {  
                case 'c':  
                {  
                    char valch = va_arg(ap,int);  //记录当前实践参数所在地址
                    printch(valch);  
                    format++;  
                    break;  
                }  
                case 'd':  
                {  
                    int valint = va_arg(ap,int);  
                    printint(valint);  
                    format++;  
                    break;  
                }  
                case 's':  
                {  
                    char *valstr = va_arg(ap,char *);  
                    printstr(valstr);  
                    format++;  
                    break;  
                }  
                case 'f':  
                {  
                    float valflt = va_arg(ap,double);  
                    printfloat(valflt);  
                    format++;  
                    break;  
                }  
                default:  
                {  
                    printch(*format);  
                    format++;  
                }  
            }    
        }  
    }
    va_end(ap);         
}  
 
