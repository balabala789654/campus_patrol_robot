#include "iic.h"
#include "stm32f4xx.h"  
#include "delay.h"
#define false    0
#define	true     1

//初始化IIC
void A0_IIC_Init(void)
{			
  GPIO_InitTypeDef  GPIO_InitStructure;

  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);//使能GPIOB时钟
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC, ENABLE);//使能GPIOB时钟


  //GPIOB8,B9初始化设置
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 ;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;//普通输出模式
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;//推挽输出
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;//100MHz
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;//上拉
  GPIO_Init(GPIOA, &GPIO_InitStructure);//初始化
	
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 ;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;//普通输出模式
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;//推挽输出
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;//100MHz
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;//上拉
  GPIO_Init(GPIOC, &GPIO_InitStructure);//初始化
	A0_IIC_SCL=1;
	A0_IIC_SDA=1;
}
//产生IIC起始信号
void A0_IIC_Start(void)
{
	A0_SDA_OUT();     //sda线输出
	A0_IIC_SDA=1;	  	  
	A0_IIC_SCL=1;
	delay_us(4);
 	A0_IIC_SDA=0;//START:when CLK is high,DATA change form high to low 
	delay_us(4);
	A0_IIC_SCL=0;//钳住I2C总线，准备发送或接收数据 
}	  
//产生IIC停止信号
void A0_IIC_Stop(void)
{
	A0_SDA_OUT();//sda线输出
	A0_IIC_SCL=0;
	A0_IIC_SDA=0;//STOP:when CLK is high DATA change form low to high
 	delay_us(4);
	A0_IIC_SCL=1; 
	A0_IIC_SDA=1;//发送I2C总线结束信号
	delay_us(4);							   	
}
//等待应答信号到来
//返回值：1，接收应答失败
//        0，接收应答成功
u8 A0_IIC_Wait_Ack(void)
{
	u8 ucErrTime=0;
	A0_SDA_IN();      //SDA设置为输入  	
	A0_IIC_SDA=1;delay_us(2);	   
	A0_IIC_SCL=1;delay_us(2);	 
	while(A0_READ_SDA)
	{
		ucErrTime++;
		if(ucErrTime>250)
		{
			A0_IIC_Stop();
			return 1;
		}
	}
	A0_IIC_SCL=0;//时钟输出0 	   
	return 0;  
} 
//产生ACK应答
void A0_IIC_Ack(void)
{
	A0_IIC_SCL=0;
	A0_SDA_OUT();
	A0_IIC_SDA=0;
	delay_us(2);
	A0_IIC_SCL=1;
	delay_us(2);
	A0_IIC_SCL=0;
}
//不产生ACK应答		    
void A0_IIC_NAck(void)
{
	A0_IIC_SCL=0;
	A0_SDA_OUT();
	A0_IIC_SDA=1;
	delay_us(2);
	A0_IIC_SCL=1;
	delay_us(2);
	A0_IIC_SCL=0;
}					 				     
//IIC发送一个字节
//返回从机有无应答
//1，有应答
//0，无应答			  
void A0_IIC_Send_Byte(u8 txd)
{                        
    u8 t;   
		A0_SDA_OUT(); 	    
    A0_IIC_SCL=0;//拉低时钟开始数据传输
    for(t=0;t<8;t++)
    {              
        A0_IIC_SDA=(txd&0x80)>>7;
        txd<<=1; 	  
		delay_us(2);   //对TEA5767这三个延时都是必须的
		A0_IIC_SCL=1;
		delay_us(2); 
		A0_IIC_SCL=0;	
		delay_us(2);
    }	 
} 	    
//读1个字节，ack=1时，发送ACK，ack=0，发送nACK   
u8 A0_IIC_Read_Byte(unsigned char ack)
{
	unsigned char i,receive=0;
	A0_SDA_IN();//SDA设置为输入
    for(i=0;i<8;i++ )
	{
        A0_IIC_SCL=0; 
        delay_us(2);
				A0_IIC_SCL=1;
				delay_us(4); 
        receive<<=1;
				delay_us(4); 
        if(A0_READ_SDA)receive++;   
				delay_us(1); 
   }					 
    if (!ack)
        A0_IIC_NAck();//发送nACK
    else
        A0_IIC_Ack(); //发送ACK   
    return receive;
}
u8 A0_i2cRead_2(u16 addr,u8 reg,u8 *buf)
{
    A0_IIC_Start();
    A0_IIC_Send_Byte((addr << 1) | 0x00);
    A0_IIC_Wait_Ack();
    A0_IIC_Send_Byte(reg);
    A0_IIC_Wait_Ack();
    A0_IIC_Start(); 
    A0_IIC_Send_Byte((addr << 1 )| 0x01);
    A0_IIC_Wait_Ack();
	
    *buf++ = A0_IIC_Read_Byte(1);
		*buf=A0_IIC_Read_Byte(0);
	
    A0_IIC_Stop();
    return true;
}


////////////////////模块2
//初始化IIC
void A1_IIC_Init(void)
{			
  GPIO_InitTypeDef  GPIO_InitStructure;

  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);//使能GPIOB时钟
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC, ENABLE);//使能GPIOB时钟


  //GPIOB8,B9初始化设置
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 ;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;//普通输出模式
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;//推挽输出
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;//100MHz
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;//上拉
  GPIO_Init(GPIOA, &GPIO_InitStructure);//初始化
	
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1 ;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;//普通输出模式
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;//推挽输出
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;//100MHz
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;//上拉
  GPIO_Init(GPIOC, &GPIO_InitStructure);//初始化
	A1_IIC_SCL=1;
	A1_IIC_SDA=1;
}
//产生IIC起始信号
void A1_IIC_Start(void)
{
	A1_SDA_OUT();     //sda线输出
	A1_IIC_SDA=1;	  	  
	A1_IIC_SCL=1;
	delay_us(4);
 	A1_IIC_SDA=0;//START:when CLK is high,DATA change form high to low 
	delay_us(4);
	A1_IIC_SCL=0;//钳住I2C总线，准备发送或接收数据 
}	  
//产生IIC停止信号
void A1_IIC_Stop(void)
{
	A1_SDA_OUT();//sda线输出
	A1_IIC_SCL=0;
	A1_IIC_SDA=0;//STOP:when CLK is high DATA change form low to high
 	delay_us(4);
	A1_IIC_SCL=1; 
	A1_IIC_SDA=1;//发送I2C总线结束信号
	delay_us(4);							   	
}
//等待应答信号到来
//返回值：1，接收应答失败
//        0，接收应答成功
u8 A1_IIC_Wait_Ack(void)
{
	u8 ucErrTime=0;
	A1_SDA_IN();      //SDA设置为输入  	
	A1_IIC_SDA=1;delay_us(2);	   
	A1_IIC_SCL=1;delay_us(2);	 
	while(A1_READ_SDA)
	{
		ucErrTime++;
		if(ucErrTime>250)
		{
			A1_IIC_Stop();
			return 1;
		}
	}
	A1_IIC_SCL=0;//时钟输出0 	   
	return 0;  
} 
//产生ACK应答
void A1_IIC_Ack(void)
{
	A1_IIC_SCL=0;
	A1_SDA_OUT();
	A1_IIC_SDA=0;
	delay_us(2);
	A1_IIC_SCL=1;
	delay_us(2);
	A1_IIC_SCL=0;
}
//不产生ACK应答		    
void A1_IIC_NAck(void)
{
	A1_IIC_SCL=0;
	A1_SDA_OUT();
	A1_IIC_SDA=1;
	delay_us(2);
	A1_IIC_SCL=1;
	delay_us(2);
	A1_IIC_SCL=0;
}					 				     
//IIC发送一个字节
//返回从机有无应答
//1，有应答
//0，无应答			  
void A1_IIC_Send_Byte(u8 txd)
{                        
    u8 t;   
		A1_SDA_OUT(); 	    
    A1_IIC_SCL=0;//拉低时钟开始数据传输
    for(t=0;t<8;t++)
    {              
        A1_IIC_SDA=(txd&0x80)>>7;
        txd<<=1; 	  
		delay_us(2);   //对TEA5767这三个延时都是必须的
		A1_IIC_SCL=1;
		delay_us(2); 
		A1_IIC_SCL=0;	
		delay_us(2);
    }	 
} 	    
//读1个字节，ack=1时，发送ACK，ack=0，发送nACK   
u8 A1_IIC_Read_Byte(unsigned char ack)
{
	unsigned char i,receive=0;
	A1_SDA_IN();//SDA设置为输入
    for(i=0;i<8;i++ )
	{
        A1_IIC_SCL=0; 
        delay_us(2);
				A1_IIC_SCL=1;
				delay_us(4); 
        receive<<=1;
				delay_us(4); 
        if(A1_READ_SDA)receive++;   
				delay_us(1); 
   }					 
    if (!ack)
        A1_IIC_NAck();//发送nACK
    else
        A1_IIC_Ack(); //发送ACK   
    return receive;
}
u8 A1_i2cRead_2(u16 addr,u8 reg,u8 *buf)
{
    A1_IIC_Start();
    A1_IIC_Send_Byte((addr << 1) | 0x00);
    A1_IIC_Wait_Ack();
    A1_IIC_Send_Byte(reg);
    A1_IIC_Wait_Ack();
    A1_IIC_Start(); 
    A1_IIC_Send_Byte((addr << 1 )| 0x01);
    A1_IIC_Wait_Ack();
	
    *buf++ = A1_IIC_Read_Byte(1);
		*buf=A1_IIC_Read_Byte(0);
	
    A1_IIC_Stop();
    return true;
}
////////////模块2
void A2_IIC_Init(void)
{			
  GPIO_InitTypeDef  GPIO_InitStructure;

  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);//使能GPIOB时钟
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC, ENABLE);//使能GPIOB时钟


  //GPIOB8,B9初始化设置
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 ;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;//普通输出模式
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;//推挽输出
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;//100MHz
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;//上拉
  GPIO_Init(GPIOA, &GPIO_InitStructure);//初始化
	
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2 ;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;//普通输出模式
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;//推挽输出
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;//100MHz
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;//上拉
  GPIO_Init(GPIOC, &GPIO_InitStructure);//初始化
	A2_IIC_SCL=1;
	A2_IIC_SDA=1;
}
//产生IIC起始信号
void A2_IIC_Start(void)
{
	A2_SDA_OUT();     //sda线输出
	A2_IIC_SDA=1;	  	  
	A2_IIC_SCL=1;
	delay_us(4);
 	A2_IIC_SDA=0;//START:when CLK is high,DATA change form high to low 
	delay_us(4);
	A2_IIC_SCL=0;//钳住I2C总线，准备发送或接收数据 
}	  
//产生IIC停止信号
void A2_IIC_Stop(void)
{
	A2_SDA_OUT();//sda线输出
	A2_IIC_SCL=0;
	A2_IIC_SDA=0;//STOP:when CLK is high DATA change form low to high
 	delay_us(4);
	A2_IIC_SCL=1; 
	A2_IIC_SDA=1;//发送I2C总线结束信号
	delay_us(4);							   	
}
//等待应答信号到来
//返回值：1，接收应答失败
//        0，接收应答成功
u8 A2_IIC_Wait_Ack(void)
{
	u8 ucErrTime=0;
	A2_SDA_IN();      //SDA设置为输入  	
	A2_IIC_SDA=1;delay_us(2);	   
	A2_IIC_SCL=1;delay_us(2);	 
	while(A2_READ_SDA)
	{
		ucErrTime++;
		if(ucErrTime>250)
		{
			A2_IIC_Stop();
			return 1;
		}
	}
	A2_IIC_SCL=0;//时钟输出0 	   
	return 0;  
} 
//产生ACK应答
void A2_IIC_Ack(void)
{
	A2_IIC_SCL=0;
	A2_SDA_OUT();
	A2_IIC_SDA=0;
	delay_us(2);
	A2_IIC_SCL=1;
	delay_us(2);
	A2_IIC_SCL=0;
}
//不产生ACK应答		    
void A2_IIC_NAck(void)
{
	A2_IIC_SCL=0;
	A2_SDA_OUT();
	A2_IIC_SDA=1;
	delay_us(2);
	A2_IIC_SCL=1;
	delay_us(2);
	A2_IIC_SCL=0;
}					 				     
//IIC发送一个字节
//返回从机有无应答
//1，有应答
//0，无应答			  
void A2_IIC_Send_Byte(u8 txd)
{                        
    u8 t;   
		A2_SDA_OUT(); 	    
    A2_IIC_SCL=0;//拉低时钟开始数据传输
    for(t=0;t<8;t++)
    {              
        A2_IIC_SDA=(txd&0x80)>>7;
        txd<<=1; 	  
		delay_us(2);   //对TEA5767这三个延时都是必须的
		A2_IIC_SCL=1;
		delay_us(2); 
		A2_IIC_SCL=0;	
		delay_us(2);
    }	 
} 	    
//读1个字节，ack=1时，发送ACK，ack=0，发送nACK   
u8 A2_IIC_Read_Byte(unsigned char ack)
{
	unsigned char i,receive=0;
	A2_SDA_IN();//SDA设置为输入
    for(i=0;i<8;i++ )
	{
        A2_IIC_SCL=0; 
        delay_us(2);
				A2_IIC_SCL=1;
				delay_us(4); 
        receive<<=1;
				delay_us(4); 
        if(A2_READ_SDA)receive++;   
				delay_us(1); 
   }					 
    if (!ack)
        A2_IIC_NAck();//发送nACK
    else
        A2_IIC_Ack(); //发送ACK   
    return receive;
}
u8 A2_i2cRead_2(u16 addr,u8 reg,u8 *buf)
{
    A2_IIC_Start();
    A2_IIC_Send_Byte((addr << 1) | 0x00);
    A2_IIC_Wait_Ack();
    A2_IIC_Send_Byte(reg);
    A2_IIC_Wait_Ack();
    A2_IIC_Start(); 
    A2_IIC_Send_Byte((addr << 1 )| 0x01);
    A2_IIC_Wait_Ack();
	
    *buf++ = A2_IIC_Read_Byte(1);
		*buf=A2_IIC_Read_Byte(0);
	
    A2_IIC_Stop();
    return true;
}




////////////模块3
void A3_IIC_Init(void)
{			
  GPIO_InitTypeDef  GPIO_InitStructure;

  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);//使能GPIOB时钟
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC, ENABLE);//使能GPIOB时钟


  //GPIOB8,B9初始化设置
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 ;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;//普通输出模式
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;//推挽输出
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;//100MHz
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;//上拉
  GPIO_Init(GPIOA, &GPIO_InitStructure);//初始化
	
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3 ;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;//普通输出模式
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;//推挽输出
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;//100MHz
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;//上拉
  GPIO_Init(GPIOC, &GPIO_InitStructure);//初始化
	A3_IIC_SCL=1;
	A3_IIC_SDA=1;
}
//产生IIC起始信号
void A3_IIC_Start(void)
{
	A3_SDA_OUT();     //sda线输出
	A3_IIC_SDA=1;	  	  
	A3_IIC_SCL=1;
	delay_us(4);
 	A3_IIC_SDA=0;//START:when CLK is high,DATA change form high to low 
	delay_us(4);
	A3_IIC_SCL=0;//钳住I2C总线，准备发送或接收数据 
}	  
//产生IIC停止信号
void A3_IIC_Stop(void)
{
	A3_SDA_OUT();//sda线输出
	A3_IIC_SCL=0;
	A3_IIC_SDA=0;//STOP:when CLK is high DATA change form low to high
 	delay_us(4);
	A3_IIC_SCL=1; 
	A3_IIC_SDA=1;//发送I2C总线结束信号
	delay_us(4);							   	
}
//等待应答信号到来
//返回值：1，接收应答失败
//        0，接收应答成功
u8 A3_IIC_Wait_Ack(void)
{
	u8 ucErrTime=0;
	A3_SDA_IN();      //SDA设置为输入  	
	A3_IIC_SDA=1;delay_us(2);	   
	A3_IIC_SCL=1;delay_us(2);	 
	while(A3_READ_SDA)
	{
		ucErrTime++;
		if(ucErrTime>250)
		{
			A3_IIC_Stop();
			return 1;
		}
	}
	A3_IIC_SCL=0;//时钟输出0 	   
	return 0;  
} 
//产生ACK应答
void A3_IIC_Ack(void)
{
	A3_IIC_SCL=0;
	A3_SDA_OUT();
	A3_IIC_SDA=0;
	delay_us(2);
	A3_IIC_SCL=1;
	delay_us(2);
	A3_IIC_SCL=0;
}
//不产生ACK应答		    
void A3_IIC_NAck(void)
{
	A3_IIC_SCL=0;
	A3_SDA_OUT();
	A3_IIC_SDA=1;
	delay_us(2);
	A3_IIC_SCL=1;
	delay_us(2);
	A3_IIC_SCL=0;
}					 				     
//IIC发送一个字节
//返回从机有无应答
//1，有应答
//0，无应答			  
void A3_IIC_Send_Byte(u8 txd)
{                        
    u8 t;   
		A3_SDA_OUT(); 	    
    A3_IIC_SCL=0;//拉低时钟开始数据传输
    for(t=0;t<8;t++)
    {              
        A3_IIC_SDA=(txd&0x80)>>7;
        txd<<=1; 	  
		delay_us(2);   //对TEA5767这三个延时都是必须的
		A3_IIC_SCL=1;
		delay_us(2); 
		A3_IIC_SCL=0;	
		delay_us(2);
    }	 
} 	    
//读1个字节，ack=1时，发送ACK，ack=0，发送nACK   
u8 A3_IIC_Read_Byte(unsigned char ack)
{
	unsigned char i,receive=0;
	A3_SDA_IN();//SDA设置为输入
    for(i=0;i<8;i++ )
	{
        A3_IIC_SCL=0; 
        delay_us(2);
				A3_IIC_SCL=1;
				delay_us(4); 
        receive<<=1;
				delay_us(4); 
        if(A3_READ_SDA)receive++;   
				delay_us(1); 
   }					 
    if (!ack)
        A3_IIC_NAck();//发送nACK
    else
        A3_IIC_Ack(); //发送ACK   
    return receive;
}
u8 A3_i2cRead_2(u16 addr,u8 reg,u8 *buf)
{
    A3_IIC_Start();
    A3_IIC_Send_Byte((addr << 1) | 0x00);
    A3_IIC_Wait_Ack();
    A3_IIC_Send_Byte(reg);
    A3_IIC_Wait_Ack();
    A3_IIC_Start(); 
    A3_IIC_Send_Byte((addr << 1 )| 0x01);
    A3_IIC_Wait_Ack();
	
    *buf++ = A3_IIC_Read_Byte(1);
		*buf=A3_IIC_Read_Byte(0);
	
    A3_IIC_Stop();
    return true;
}







