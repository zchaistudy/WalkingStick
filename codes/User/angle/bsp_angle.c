/**
  ******************************************************************************
  * @file    bsp_angle.c
  * @author  chzh
  * @version V1.0
  * @date    2018-05-15
  * @brief   I2CͨѶ��λģ��bsp
  ******************************************************************************
  */
  
#include "bsp_angle_define.h"
#include "debug.h"

unsigned char TX_DATA[5]; 		  	//���ڷ�������
unsigned char BUF[8];             //�������ݻ�����
short M_x,M_y,M_z,A_x,A_y,A_z;	  //�ų�����ٶȵ�ԭʼֵ
char  test=0; 					 					//I2Cʹ��
float angle;					  					//�ų�����ĽǶ�

void DATA_printf(uchar *s,short temp_data)
{
	if(temp_data<0){
	temp_data=-temp_data;
    *s='-';
	}
	else *s=' ';
	*++s =temp_data/1000+0x30;
    temp_data=temp_data%1000;     //ȡ������
    *++s =temp_data/100+0x30;
    temp_data=temp_data%100;     //ȡ������
    *++s =temp_data/10+0x30;
    temp_data=temp_data%10;      //ȡ������
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
  
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB , ENABLE);
	
  GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_6;				//SCL
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_OD;  
  GPIO_Init(GPIOB, &GPIO_InitStructure);

  GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_7;				//SDA
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_OD;
  GPIO_Init(GPIOB, &GPIO_InitStructure);
}

void I2C_delay(void)
{
		
   u8 i=30; //��������Ż��ٶ�	����������͵�5����д��
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
	if(!SDA_read)return FALSE;	//SDA��Ϊ�͵�ƽ������æ,�˳�
	SDA_L;
	I2C_delay();
	if(SDA_read) return FALSE;	//SDA��Ϊ�ߵ�ƽ�����߳���,�˳�
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
Statue I2C_WaitAck(void) 	 //����Ϊ:=1��ACK,=0��ACK
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
void I2C_SendByte(u8 SendByte) //���ݴӸ�λ����λ//
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
unsigned char I2C_RadeByte(void)  //���ݴӸ�λ����λ//
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
//���ֽ�д��*******************************************

Statue Single_WriteLSM303D(unsigned char SlaveAddress,unsigned char REG_Address,unsigned char REG_data)		     //void
{
  	if(!I2C_Start())return FALSE;
    I2C_SendByte(SlaveAddress);   //�����豸��ַ+д�ź�//I2C_SendByte(((REG_Address & 0x0700) >>7) | SlaveAddress & 0xFFFE);//���ø���ʼ��ַ+������ַ 
    if(!I2C_WaitAck()){I2C_Stop(); return FALSE;}
    I2C_SendByte(REG_Address );   //���õ���ʼ��ַ      
    I2C_WaitAck();	
    I2C_SendByte(REG_data);
    I2C_WaitAck();   
    I2C_Stop(); 
    return TRUE;

}
 //���ֽڶ�ȡ*****************************************
unsigned char Single_Read(unsigned char SlaveAddress,unsigned char REG_Address)
{   
	  unsigned char REG_data;     	
	  if(!I2C_Start())return FALSE;
    I2C_SendByte(SlaveAddress); //I2C_SendByte(((REG_Address & 0x0700) >>7) | REG_Address & 0xFFFE);//���ø���ʼ��ַ+������ַ 
    if(!I2C_WaitAck()){I2C_Stop();test=1; return FALSE;}
    I2C_SendByte((u8) REG_Address);   //���õ���ʼ��ַ      
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

//������ȡ6������*****************************************
Statue Multiple_read(unsigned char SlaveAddress,unsigned char REG_Address)
{   char i;	
	if(!I2C_Start())return FALSE;
    I2C_SendByte(SlaveAddress); //I2C_SendByte(((REG_Address & 0x0700) >>7) | REG_Address & 0xFFFE);//���ø���ʼ��ַ+������ַ 
    if(!I2C_WaitAck()){I2C_Stop();test=1; return FALSE;}
    I2C_SendByte((u8) REG_Address);   //������ʼ��ַ      
    I2C_WaitAck();
    I2C_Start();
    I2C_SendByte(SlaveAddress+1);
    I2C_WaitAck();

	for (i=0; i<6; i++)                //������ȡ6����ַ���ݣ��洢��BUF
    {
        BUF[i] = I2C_RadeByte();       //BUF[]�洢
        if (i == 5)I2C_NoAck();        //���һ��������Ҫ��NOACK                    
          else I2C_Ack();              //��ӦACK                    
    }
    I2C_Stop();
    return TRUE;     //�޸�
}						      

//��ʼ��LSM303D��������Ҫ��ο������ֲᣬ�����޸�************************
void InitLSM303D(void)
{
	Single_WriteLSM303D(M_SlaveAddress,0x00,0x14);   //
  Single_WriteLSM303D(M_SlaveAddress,0x02,0x00);   //
	Single_WriteLSM303D(A_SlaveAddress,0x20,0x27);   //������Χ,����2g��16λģʽ
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
	
	
	

char getAngle()
{
	char num;
	 //�ų����ݶ�ȡ�뷢��
	Multiple_read(M_SlaveAddress,0x03);//OUT_X_M
	M_x=(BUF[0] << 8) | BUF[1]; //�ϳ�16λ����
	M_y=(BUF[2] << 8) | BUF[3]; //�ϳ�16λ����
	M_z=(BUF[4] << 8) | BUF[5]; //�ϳ�16λ����
	angle= atan2(M_y,M_x) * (180 / 3.14159265) + 180; // angle in degrees
	my_printf("angle = %f\r\n",angle);
		if(angle>=19&&angle<40)//��
			num = '0';
		if(angle>=40&&angle<50)//��
			num ='2';
		if(angle>=50&angle<70)//��
			num = '4';
		if(angle>1&&angle<19) //��
			num = '6';
//		if(angle<30||angle>350)//��
//			num = '0';
//		if(angle>=30&&angle<=62)//����
//			num = '1';
//		if(angle>62&&angle<95)//��
//			num ='2';
//		if(angle>=95&&angle<140)//����
//			num = '3';
//		if(angle>140&&angle<200)//��
//			num = '4';
//		if(angle>200&&angle<300)//����
//			num = '5';
//		if(angle>300&angle<330) //��
//			num = '6';
//		if(angle>330&&angle<350)  //����
//			num = '7';
		return num;
}
