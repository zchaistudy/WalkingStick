/**
  ******************************************************************************
  * @file    bsp_angle.c
  * @author  chzh
  * @version V1.0
  * @date    2018-05-15
  * @brief   I2C通讯方位模块bsp
  ******************************************************************************
  */
  
#include "bsp_angle_define.h"

unsigned char TX_DATA[5]; 		  	//串口发送数组
unsigned char BUF[8];             //接收数据缓存区
short M_x,M_y,M_z,A_x,A_y,A_z;	  //磁场与加速度的原始值
char  test=0; 					 					//I2C使用
float angle;					  					//磁场计算的角度

void DATA_printf(uchar *s,short temp_data)
{
	if(temp_data<0){
	temp_data=-temp_data;
    *s='-';
	}
	else *s=' ';
	*++s =temp_data/1000+0x30;
    temp_data=temp_data%1000;     //取余运算
    *++s =temp_data/100+0x30;
    temp_data=temp_data%100;     //取余运算
    *++s =temp_data/10+0x30;
    temp_data=temp_data%10;      //取余运算
    *++s =temp_data+0x30; 	
}

/*******************************************************************************
* Function Name  : I2C_GPIO_Config
* Description    : Configration Simulation IIC GPIO
* Input          : None 
* Output         : None
* Return         : None
****************************************************************************** */
void I2C_GPIO_Config(void)
{
  GPIO_InitTypeDef  GPIO_InitStructure;  
  
  GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_6;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_OD;  
  GPIO_Init(GPIOB, &GPIO_InitStructure);

  GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_7;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_OD;
  GPIO_Init(GPIOB, &GPIO_InitStructure);
}

void I2C_delay(void)
{
		
   u8 i=30; //这里可以优化速度	，经测试最低到5还能写入
   while(i) 
   { 
     i--; 
   }  
}

void delay5ms(void)
{
		
   int i=5000;  
   while(i) 
   { 
     i--; 
   }  
}

Statue I2C_Start(void)
{
	SDA_H;
	SCL_H;
	I2C_delay();
	if(!SDA_read)return FALSE;	//SDA线为低电平则总线忙,退出
	SDA_L;
	I2C_delay();
	if(SDA_read) return FALSE;	//SDA线为高电平则总线出错,退出
	SDA_L;
	I2C_delay();
	return TRUE;
}
/*******************************************************************************
****************************************************************************** */
void I2C_Stop(void)
{
	SCL_L;
	I2C_delay();
	SDA_L;
	I2C_delay();
	SCL_H;
	I2C_delay();
	SDA_H;
	I2C_delay();
} 
/*******************************************************************************
****************************************************************************** */
void I2C_Ack(void)
{	
	SCL_L;
	I2C_delay();
	SDA_L;
	I2C_delay();
	SCL_H;
	I2C_delay();
	SCL_L;
	I2C_delay();
}   
/*******************************************************************************
****************************************************************************** */
void I2C_NoAck(void)
{	
	SCL_L;
	I2C_delay();
	SDA_H;
	I2C_delay();
	SCL_H;
	I2C_delay();
	SCL_L;
	I2C_delay();
} 
/*******************************************************************************
****************************************************************************** */
Statue I2C_WaitAck(void) 	 //返回为:=1有ACK,=0无ACK
{
	SCL_L;
	I2C_delay();
	SDA_H;			
	I2C_delay();
	SCL_H;
	I2C_delay();
	if(SDA_read)
	{
      SCL_L;
	  I2C_delay();
      return FALSE;
	}
	SCL_L;
	I2C_delay();
	return TRUE;
}
/*******************************************************************************
****************************************************************************** */
void I2C_SendByte(u8 SendByte) //数据从高位到低位//
{
    u8 i=8;
    while(i--)
    {
        SCL_L;
        I2C_delay();
      if(SendByte&0x80)
        SDA_H;  
      else 
        SDA_L;   
        SendByte<<=1;
        I2C_delay();
		SCL_H;
        I2C_delay();
    }
    SCL_L;
}  
/*******************************************************************************
****************************************************************************** */
unsigned char I2C_RadeByte(void)  //数据从高位到低位//
{ 
    u8 i=8;
    u8 ReceiveByte=0;

    SDA_H;				
    while(i--)
    {
      ReceiveByte<<=1;      
      SCL_L;
      I2C_delay();
	  SCL_H;
      I2C_delay();	
      if(SDA_read)
      {
        ReceiveByte|=0x01;
      }
    }
    SCL_L;
    return ReceiveByte;
} 
//ZRX          
//单字节写入*******************************************

Statue Single_WriteLSM303D(unsigned char SlaveAddress,unsigned char REG_Address,unsigned char REG_data)		     //void
{
  	if(!I2C_Start())return FALSE;
    I2C_SendByte(SlaveAddress);   //发送设备地址+写信号//I2C_SendByte(((REG_Address & 0x0700) >>7) | SlaveAddress & 0xFFFE);//设置高起始地址+器件地址 
    if(!I2C_WaitAck()){I2C_Stop(); return FALSE;}
    I2C_SendByte(REG_Address );   //设置低起始地址      
    I2C_WaitAck();	
    I2C_SendByte(REG_data);
    I2C_WaitAck();   
    I2C_Stop(); 
    return TRUE;

}
 //单字节读取*****************************************
unsigned char Single_Read(unsigned char SlaveAddress,unsigned char REG_Address)
{   
	  unsigned char REG_data;     	
	  if(!I2C_Start())return FALSE;
    I2C_SendByte(SlaveAddress); //I2C_SendByte(((REG_Address & 0x0700) >>7) | REG_Address & 0xFFFE);//设置高起始地址+器件地址 
    if(!I2C_WaitAck()){I2C_Stop();test=1; return FALSE;}
    I2C_SendByte((u8) REG_Address);   //设置低起始地址      
    I2C_WaitAck();
    I2C_Start();
    I2C_SendByte(SlaveAddress+1);
    I2C_WaitAck();

	REG_data= I2C_RadeByte();
    I2C_NoAck();
    I2C_Stop();
    //return TRUE;
	return REG_data;
}						      

//连续读取6个数据*****************************************
Statue Multiple_read(unsigned char SlaveAddress,unsigned char REG_Address)
{   char i;	
	if(!I2C_Start())return FALSE;
    I2C_SendByte(SlaveAddress); //I2C_SendByte(((REG_Address & 0x0700) >>7) | REG_Address & 0xFFFE);//设置高起始地址+器件地址 
    if(!I2C_WaitAck()){I2C_Stop();test=1; return FALSE;}
    I2C_SendByte((u8) REG_Address);   //设置起始地址      
    I2C_WaitAck();
    I2C_Start();
    I2C_SendByte(SlaveAddress+1);
    I2C_WaitAck();

	for (i=0; i<6; i++)                //连续读取6个地址数据，存储中BUF
    {
        BUF[i] = I2C_RadeByte();       //BUF[]存储
        if (i == 5)I2C_NoAck();        //最后一个数据需要回NOACK                    
          else I2C_Ack();              //回应ACK                    
    }
    I2C_Stop();
    return TRUE;     //修改
}						      

//初始化LSM303D，根据需要请参考数据手册，进行修改************************
void InitLSM303D(void)
{
	Single_WriteLSM303D(M_SlaveAddress,0x00,0x14);   //
  Single_WriteLSM303D(M_SlaveAddress,0x02,0x00);   //
	Single_WriteLSM303D(A_SlaveAddress,0x20,0x27);   //测量范围,正负2g，16位模式
}
void Delay(vu32 nCount)
{
  for(; nCount != 0; nCount--);
}

 void Delayms(vu32 m)
{
  u32 i;
  
  for(; m != 0; m--)	
       for (i=0; i<50000; i++);
}


void delay1_1ms(unsigned int time)
{
  unsigned int i;
  while(time--)
    {
      i=12000;
      while(i--);
    }
}
	
	
	

int getAngle()
{
	int num;
	 //磁场数据读取与发送
	Multiple_read(M_SlaveAddress,0x03);//OUT_X_M
	M_x=(BUF[0] << 8) | BUF[1]; //合成16位数据
	M_y=(BUF[2] << 8) | BUF[3]; //合成16位数据
	M_z=(BUF[4] << 8) | BUF[5]; //合成16位数据
	angle= atan2(M_y,M_x) * (180 / 3.14159265) + 180; // angle in degrees
		if(angle<30||angle>350)//南
//			printf(" S11316TE");
		num = 0;
		if(angle>=30&&angle<=62)//西南
//			printf("S11327TE");
		num = 1;
		if(angle>62&&angle<105)//西
//			printf("S11338TE");
			num =2;
		if(angle>=105&&angle<160)//西北
//			printf("S11349TE");
			num = 3;
		if(angle>160&&angle<200)//北
//			printf("S11349TE");
			num = 4;
		if(angle>200&&angle<300)//东北
//			printf("S11349TE");
			num = 5;
		if(angle>300&angle<330) //东
//			printf("S11349TE");
			num = 6;
		if(angle>330&&angle<350)  //东南
//			printf("S11349TE");
			num = 7;
		return num;
}
