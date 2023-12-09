#ifndef _IIC_H_
#define _IIC_H_
#include "sys.h" 
//IO��������
#define A0_SDA_IN()  {GPIOA->MODER&=~(3<<(0*2));GPIOA->MODER|=0<<0*2;}	//PB9����ģʽ
#define A0_SDA_OUT() {GPIOA->MODER&=~(3<<(0*2));GPIOA->MODER|=1<<0*2;} //PB9���ģʽ

#define A1_SDA_IN()  {GPIOA->MODER&=~(3<<(0*2));GPIOA->MODER|=0<<0*2;}	//PB9����ģʽ
#define A1_SDA_OUT() {GPIOA->MODER&=~(3<<(0*2));GPIOA->MODER|=1<<0*2;} //PB9���ģʽ

#define A2_SDA_IN()  {GPIOA->MODER&=~(3<<(0*2));GPIOA->MODER|=0<<0*2;}	//PB9����ģʽ
#define A2_SDA_OUT() {GPIOA->MODER&=~(3<<(0*2));GPIOA->MODER|=1<<0*2;} //PB9���ģʽ

#define A3_SDA_IN()  {GPIOA->MODER&=~(3<<(0*2));GPIOA->MODER|=0<<0*2;}	//PB9����ģʽ
#define A3_SDA_OUT() {GPIOA->MODER&=~(3<<(0*2));GPIOA->MODER|=1<<0*2;} //PB9���ģʽ
//IO��������	 
#define A0_IIC_SCL    PCout(0) //SCL
#define A0_IIC_SDA    PAout(0) //SDA	 
#define A0_READ_SDA   PAin(0)  //����SDA

#define A1_IIC_SCL    PCout(1) //SCL
#define A1_IIC_SDA    PAout(0) //SDA	 
#define A1_READ_SDA   PAin(0)  //����SDA 

#define A2_IIC_SCL    PCout(2) //SCL
#define A2_IIC_SDA    PAout(0) //SDA	 
#define A2_READ_SDA   PAin(0)  //����SDA 

#define A3_IIC_SCL    PCout(3) //SCL
#define A3_IIC_SDA    PAout(0) //SDA	 
#define A3_READ_SDA   PAin(0)  //����SDA 

//ģ��1���в�������
void A0_IIC_Init(void);                //��ʼ��IIC��IO��				 
void A0_IIC_Start(void);				//����IIC��ʼ�ź�
void A0_IIC_Stop(void);	  			//����IICֹͣ�ź�
void A0_IIC_Send_Byte(u8 txd);			//IIC����һ���ֽ�
u8 A0_IIC_Read_Byte(unsigned char ack);//IIC��ȡһ���ֽ�
u8 A0_IIC_Wait_Ack(void); 				//IIC�ȴ�ACK�ź�
void A0_IIC_Ack(void);					//IIC����ACK�ź�
void A0_IIC_NAck(void);				//IIC������ACK�ź�

void A0_IIC_Write_One_Byte(u8 daddr,u8 addr,u8 data);
u8 A0_IIC_Read_One_Byte(u8 daddr,u8 addr);	  

u8 A0_i2cRead_2(u16 addr, u8 reg,u8 *buf);


//ģ��2���в�������
void A1_IIC_Init(void);                //��ʼ��IIC��IO��				 
void A1_IIC_Start(void);				//����IIC��ʼ�ź�
void A1_IIC_Stop(void);	  			//����IICֹͣ�ź�
void A1_IIC_Send_Byte(u8 txd);			//IIC����һ���ֽ�
u8 A1_IIC_Read_Byte(unsigned char ack);//IIC��ȡһ���ֽ�
u8 A1_IIC_Wait_Ack(void); 				//IIC�ȴ�ACK�ź�
void A1_IIC_Ack(void);					//IIC����ACK�ź�
void A1_IIC_NAck(void);				//IIC������ACK�ź�
void A1_IIC_Write_One_Byte(u8 daddr,u8 addr,u8 data);
u8 A1_IIC_Read_One_Byte(u8 daddr,u8 addr);	  
u8 A1_i2cRead_2(u16 addr, u8 reg,u8 *buf);

//ģ��3���в�������
void A2_IIC_Init(void);                //��ʼ��IIC��IO��				 
void A2_IIC_Start(void);				//����IIC��ʼ�ź�
void A2_IIC_Stop(void);	  			//����IICֹͣ�ź�
void A2_IIC_Send_Byte(u8 txd);			//IIC����һ���ֽ�
u8 A2_IIC_Read_Byte(unsigned char ack);//IIC��ȡһ���ֽ�
u8 A2_IIC_Wait_Ack(void); 				//IIC�ȴ�ACK�ź�
void A2_IIC_Ack(void);					//IIC����ACK�ź�
void A2_IIC_NAck(void);				//IIC������ACK�ź�
void A2_IIC_Write_One_Byte(u8 daddr,u8 addr,u8 data);
u8 A2_IIC_Read_One_Byte(u8 daddr,u8 addr);	  
u8 A2_i2cRead_2(u16 addr, u8 reg,u8 *buf);

//ģ��4���в�������
void A3_IIC_Init(void);                //��ʼ��IIC��IO��				 
void A3_IIC_Start(void);				//����IIC��ʼ�ź�
void A3_IIC_Stop(void);	  			//����IICֹͣ�ź�
void A3_IIC_Send_Byte(u8 txd);			//IIC����һ���ֽ�
u8 A3_IIC_Read_Byte(unsigned char ack);//IIC��ȡһ���ֽ�
u8 A3_IIC_Wait_Ack(void); 				//IIC�ȴ�ACK�ź�
void A3_IIC_Ack(void);					//IIC����ACK�ź�
void A3_IIC_NAck(void);				//IIC������ACK�ź�
void A3_IIC_Write_One_Byte(u8 daddr,u8 addr,u8 data);
u8 A3_IIC_Read_One_Byte(u8 daddr,u8 addr);	  
u8 A3_i2cRead_2(u16 addr, u8 reg,u8 *buf);
#endif

