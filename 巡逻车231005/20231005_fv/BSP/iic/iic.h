#ifndef _IIC_H_
#define _IIC_H_
#include "sys.h" 
//IO方向设置
#define A0_SDA_IN()  {GPIOA->MODER&=~(3<<(0*2));GPIOA->MODER|=0<<0*2;}	//PB9输入模式
#define A0_SDA_OUT() {GPIOA->MODER&=~(3<<(0*2));GPIOA->MODER|=1<<0*2;} //PB9输出模式

#define A1_SDA_IN()  {GPIOA->MODER&=~(3<<(0*2));GPIOA->MODER|=0<<0*2;}	//PB9输入模式
#define A1_SDA_OUT() {GPIOA->MODER&=~(3<<(0*2));GPIOA->MODER|=1<<0*2;} //PB9输出模式

#define A2_SDA_IN()  {GPIOA->MODER&=~(3<<(0*2));GPIOA->MODER|=0<<0*2;}	//PB9输入模式
#define A2_SDA_OUT() {GPIOA->MODER&=~(3<<(0*2));GPIOA->MODER|=1<<0*2;} //PB9输出模式

#define A3_SDA_IN()  {GPIOA->MODER&=~(3<<(0*2));GPIOA->MODER|=0<<0*2;}	//PB9输入模式
#define A3_SDA_OUT() {GPIOA->MODER&=~(3<<(0*2));GPIOA->MODER|=1<<0*2;} //PB9输出模式
//IO操作函数	 
#define A0_IIC_SCL    PCout(0) //SCL
#define A0_IIC_SDA    PAout(0) //SDA	 
#define A0_READ_SDA   PAin(0)  //输入SDA

#define A1_IIC_SCL    PCout(1) //SCL
#define A1_IIC_SDA    PAout(0) //SDA	 
#define A1_READ_SDA   PAin(0)  //输入SDA 

#define A2_IIC_SCL    PCout(2) //SCL
#define A2_IIC_SDA    PAout(0) //SDA	 
#define A2_READ_SDA   PAin(0)  //输入SDA 

#define A3_IIC_SCL    PCout(3) //SCL
#define A3_IIC_SDA    PAout(0) //SDA	 
#define A3_READ_SDA   PAin(0)  //输入SDA 

//模块1所有操作函数
void A0_IIC_Init(void);                //初始化IIC的IO口				 
void A0_IIC_Start(void);				//发送IIC开始信号
void A0_IIC_Stop(void);	  			//发送IIC停止信号
void A0_IIC_Send_Byte(u8 txd);			//IIC发送一个字节
u8 A0_IIC_Read_Byte(unsigned char ack);//IIC读取一个字节
u8 A0_IIC_Wait_Ack(void); 				//IIC等待ACK信号
void A0_IIC_Ack(void);					//IIC发送ACK信号
void A0_IIC_NAck(void);				//IIC不发送ACK信号

void A0_IIC_Write_One_Byte(u8 daddr,u8 addr,u8 data);
u8 A0_IIC_Read_One_Byte(u8 daddr,u8 addr);	  

u8 A0_i2cRead_2(u16 addr, u8 reg,u8 *buf);


//模块2所有操作函数
void A1_IIC_Init(void);                //初始化IIC的IO口				 
void A1_IIC_Start(void);				//发送IIC开始信号
void A1_IIC_Stop(void);	  			//发送IIC停止信号
void A1_IIC_Send_Byte(u8 txd);			//IIC发送一个字节
u8 A1_IIC_Read_Byte(unsigned char ack);//IIC读取一个字节
u8 A1_IIC_Wait_Ack(void); 				//IIC等待ACK信号
void A1_IIC_Ack(void);					//IIC发送ACK信号
void A1_IIC_NAck(void);				//IIC不发送ACK信号
void A1_IIC_Write_One_Byte(u8 daddr,u8 addr,u8 data);
u8 A1_IIC_Read_One_Byte(u8 daddr,u8 addr);	  
u8 A1_i2cRead_2(u16 addr, u8 reg,u8 *buf);

//模块3所有操作函数
void A2_IIC_Init(void);                //初始化IIC的IO口				 
void A2_IIC_Start(void);				//发送IIC开始信号
void A2_IIC_Stop(void);	  			//发送IIC停止信号
void A2_IIC_Send_Byte(u8 txd);			//IIC发送一个字节
u8 A2_IIC_Read_Byte(unsigned char ack);//IIC读取一个字节
u8 A2_IIC_Wait_Ack(void); 				//IIC等待ACK信号
void A2_IIC_Ack(void);					//IIC发送ACK信号
void A2_IIC_NAck(void);				//IIC不发送ACK信号
void A2_IIC_Write_One_Byte(u8 daddr,u8 addr,u8 data);
u8 A2_IIC_Read_One_Byte(u8 daddr,u8 addr);	  
u8 A2_i2cRead_2(u16 addr, u8 reg,u8 *buf);

//模块4所有操作函数
void A3_IIC_Init(void);                //初始化IIC的IO口				 
void A3_IIC_Start(void);				//发送IIC开始信号
void A3_IIC_Stop(void);	  			//发送IIC停止信号
void A3_IIC_Send_Byte(u8 txd);			//IIC发送一个字节
u8 A3_IIC_Read_Byte(unsigned char ack);//IIC读取一个字节
u8 A3_IIC_Wait_Ack(void); 				//IIC等待ACK信号
void A3_IIC_Ack(void);					//IIC发送ACK信号
void A3_IIC_NAck(void);				//IIC不发送ACK信号
void A3_IIC_Write_One_Byte(u8 daddr,u8 addr,u8 data);
u8 A3_IIC_Read_One_Byte(u8 daddr,u8 addr);	  
u8 A3_i2cRead_2(u16 addr, u8 reg,u8 *buf);
#endif

