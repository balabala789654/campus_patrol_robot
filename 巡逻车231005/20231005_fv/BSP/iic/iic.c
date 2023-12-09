#include "iic.h"
#include "stm32f4xx.h"  
#include "delay.h"
#define false    0
#define	true     1

//��ʼ��IIC
void A0_IIC_Init(void)
{			
  GPIO_InitTypeDef  GPIO_InitStructure;

  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);//ʹ��GPIOBʱ��
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC, ENABLE);//ʹ��GPIOBʱ��


  //GPIOB8,B9��ʼ������
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 ;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;//��ͨ���ģʽ
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;//�������
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;//100MHz
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;//����
  GPIO_Init(GPIOA, &GPIO_InitStructure);//��ʼ��
	
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 ;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;//��ͨ���ģʽ
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;//�������
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;//100MHz
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;//����
  GPIO_Init(GPIOC, &GPIO_InitStructure);//��ʼ��
	A0_IIC_SCL=1;
	A0_IIC_SDA=1;
}
//����IIC��ʼ�ź�
void A0_IIC_Start(void)
{
	A0_SDA_OUT();     //sda�����
	A0_IIC_SDA=1;	  	  
	A0_IIC_SCL=1;
	delay_us(4);
 	A0_IIC_SDA=0;//START:when CLK is high,DATA change form high to low 
	delay_us(4);
	A0_IIC_SCL=0;//ǯסI2C���ߣ�׼�����ͻ�������� 
}	  
//����IICֹͣ�ź�
void A0_IIC_Stop(void)
{
	A0_SDA_OUT();//sda�����
	A0_IIC_SCL=0;
	A0_IIC_SDA=0;//STOP:when CLK is high DATA change form low to high
 	delay_us(4);
	A0_IIC_SCL=1; 
	A0_IIC_SDA=1;//����I2C���߽����ź�
	delay_us(4);							   	
}
//�ȴ�Ӧ���źŵ���
//����ֵ��1������Ӧ��ʧ��
//        0������Ӧ��ɹ�
u8 A0_IIC_Wait_Ack(void)
{
	u8 ucErrTime=0;
	A0_SDA_IN();      //SDA����Ϊ����  	
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
	A0_IIC_SCL=0;//ʱ�����0 	   
	return 0;  
} 
//����ACKӦ��
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
//������ACKӦ��		    
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
//IIC����һ���ֽ�
//���شӻ�����Ӧ��
//1����Ӧ��
//0����Ӧ��			  
void A0_IIC_Send_Byte(u8 txd)
{                        
    u8 t;   
		A0_SDA_OUT(); 	    
    A0_IIC_SCL=0;//����ʱ�ӿ�ʼ���ݴ���
    for(t=0;t<8;t++)
    {              
        A0_IIC_SDA=(txd&0x80)>>7;
        txd<<=1; 	  
		delay_us(2);   //��TEA5767��������ʱ���Ǳ����
		A0_IIC_SCL=1;
		delay_us(2); 
		A0_IIC_SCL=0;	
		delay_us(2);
    }	 
} 	    
//��1���ֽڣ�ack=1ʱ������ACK��ack=0������nACK   
u8 A0_IIC_Read_Byte(unsigned char ack)
{
	unsigned char i,receive=0;
	A0_SDA_IN();//SDA����Ϊ����
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
        A0_IIC_NAck();//����nACK
    else
        A0_IIC_Ack(); //����ACK   
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


////////////////////ģ��2
//��ʼ��IIC
void A1_IIC_Init(void)
{			
  GPIO_InitTypeDef  GPIO_InitStructure;

  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);//ʹ��GPIOBʱ��
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC, ENABLE);//ʹ��GPIOBʱ��


  //GPIOB8,B9��ʼ������
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 ;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;//��ͨ���ģʽ
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;//�������
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;//100MHz
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;//����
  GPIO_Init(GPIOA, &GPIO_InitStructure);//��ʼ��
	
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1 ;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;//��ͨ���ģʽ
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;//�������
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;//100MHz
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;//����
  GPIO_Init(GPIOC, &GPIO_InitStructure);//��ʼ��
	A1_IIC_SCL=1;
	A1_IIC_SDA=1;
}
//����IIC��ʼ�ź�
void A1_IIC_Start(void)
{
	A1_SDA_OUT();     //sda�����
	A1_IIC_SDA=1;	  	  
	A1_IIC_SCL=1;
	delay_us(4);
 	A1_IIC_SDA=0;//START:when CLK is high,DATA change form high to low 
	delay_us(4);
	A1_IIC_SCL=0;//ǯסI2C���ߣ�׼�����ͻ�������� 
}	  
//����IICֹͣ�ź�
void A1_IIC_Stop(void)
{
	A1_SDA_OUT();//sda�����
	A1_IIC_SCL=0;
	A1_IIC_SDA=0;//STOP:when CLK is high DATA change form low to high
 	delay_us(4);
	A1_IIC_SCL=1; 
	A1_IIC_SDA=1;//����I2C���߽����ź�
	delay_us(4);							   	
}
//�ȴ�Ӧ���źŵ���
//����ֵ��1������Ӧ��ʧ��
//        0������Ӧ��ɹ�
u8 A1_IIC_Wait_Ack(void)
{
	u8 ucErrTime=0;
	A1_SDA_IN();      //SDA����Ϊ����  	
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
	A1_IIC_SCL=0;//ʱ�����0 	   
	return 0;  
} 
//����ACKӦ��
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
//������ACKӦ��		    
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
//IIC����һ���ֽ�
//���شӻ�����Ӧ��
//1����Ӧ��
//0����Ӧ��			  
void A1_IIC_Send_Byte(u8 txd)
{                        
    u8 t;   
		A1_SDA_OUT(); 	    
    A1_IIC_SCL=0;//����ʱ�ӿ�ʼ���ݴ���
    for(t=0;t<8;t++)
    {              
        A1_IIC_SDA=(txd&0x80)>>7;
        txd<<=1; 	  
		delay_us(2);   //��TEA5767��������ʱ���Ǳ����
		A1_IIC_SCL=1;
		delay_us(2); 
		A1_IIC_SCL=0;	
		delay_us(2);
    }	 
} 	    
//��1���ֽڣ�ack=1ʱ������ACK��ack=0������nACK   
u8 A1_IIC_Read_Byte(unsigned char ack)
{
	unsigned char i,receive=0;
	A1_SDA_IN();//SDA����Ϊ����
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
        A1_IIC_NAck();//����nACK
    else
        A1_IIC_Ack(); //����ACK   
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
////////////ģ��2
void A2_IIC_Init(void)
{			
  GPIO_InitTypeDef  GPIO_InitStructure;

  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);//ʹ��GPIOBʱ��
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC, ENABLE);//ʹ��GPIOBʱ��


  //GPIOB8,B9��ʼ������
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 ;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;//��ͨ���ģʽ
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;//�������
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;//100MHz
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;//����
  GPIO_Init(GPIOA, &GPIO_InitStructure);//��ʼ��
	
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2 ;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;//��ͨ���ģʽ
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;//�������
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;//100MHz
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;//����
  GPIO_Init(GPIOC, &GPIO_InitStructure);//��ʼ��
	A2_IIC_SCL=1;
	A2_IIC_SDA=1;
}
//����IIC��ʼ�ź�
void A2_IIC_Start(void)
{
	A2_SDA_OUT();     //sda�����
	A2_IIC_SDA=1;	  	  
	A2_IIC_SCL=1;
	delay_us(4);
 	A2_IIC_SDA=0;//START:when CLK is high,DATA change form high to low 
	delay_us(4);
	A2_IIC_SCL=0;//ǯסI2C���ߣ�׼�����ͻ�������� 
}	  
//����IICֹͣ�ź�
void A2_IIC_Stop(void)
{
	A2_SDA_OUT();//sda�����
	A2_IIC_SCL=0;
	A2_IIC_SDA=0;//STOP:when CLK is high DATA change form low to high
 	delay_us(4);
	A2_IIC_SCL=1; 
	A2_IIC_SDA=1;//����I2C���߽����ź�
	delay_us(4);							   	
}
//�ȴ�Ӧ���źŵ���
//����ֵ��1������Ӧ��ʧ��
//        0������Ӧ��ɹ�
u8 A2_IIC_Wait_Ack(void)
{
	u8 ucErrTime=0;
	A2_SDA_IN();      //SDA����Ϊ����  	
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
	A2_IIC_SCL=0;//ʱ�����0 	   
	return 0;  
} 
//����ACKӦ��
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
//������ACKӦ��		    
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
//IIC����һ���ֽ�
//���شӻ�����Ӧ��
//1����Ӧ��
//0����Ӧ��			  
void A2_IIC_Send_Byte(u8 txd)
{                        
    u8 t;   
		A2_SDA_OUT(); 	    
    A2_IIC_SCL=0;//����ʱ�ӿ�ʼ���ݴ���
    for(t=0;t<8;t++)
    {              
        A2_IIC_SDA=(txd&0x80)>>7;
        txd<<=1; 	  
		delay_us(2);   //��TEA5767��������ʱ���Ǳ����
		A2_IIC_SCL=1;
		delay_us(2); 
		A2_IIC_SCL=0;	
		delay_us(2);
    }	 
} 	    
//��1���ֽڣ�ack=1ʱ������ACK��ack=0������nACK   
u8 A2_IIC_Read_Byte(unsigned char ack)
{
	unsigned char i,receive=0;
	A2_SDA_IN();//SDA����Ϊ����
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
        A2_IIC_NAck();//����nACK
    else
        A2_IIC_Ack(); //����ACK   
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




////////////ģ��3
void A3_IIC_Init(void)
{			
  GPIO_InitTypeDef  GPIO_InitStructure;

  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);//ʹ��GPIOBʱ��
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC, ENABLE);//ʹ��GPIOBʱ��


  //GPIOB8,B9��ʼ������
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 ;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;//��ͨ���ģʽ
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;//�������
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;//100MHz
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;//����
  GPIO_Init(GPIOA, &GPIO_InitStructure);//��ʼ��
	
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3 ;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;//��ͨ���ģʽ
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;//�������
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;//100MHz
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;//����
  GPIO_Init(GPIOC, &GPIO_InitStructure);//��ʼ��
	A3_IIC_SCL=1;
	A3_IIC_SDA=1;
}
//����IIC��ʼ�ź�
void A3_IIC_Start(void)
{
	A3_SDA_OUT();     //sda�����
	A3_IIC_SDA=1;	  	  
	A3_IIC_SCL=1;
	delay_us(4);
 	A3_IIC_SDA=0;//START:when CLK is high,DATA change form high to low 
	delay_us(4);
	A3_IIC_SCL=0;//ǯסI2C���ߣ�׼�����ͻ�������� 
}	  
//����IICֹͣ�ź�
void A3_IIC_Stop(void)
{
	A3_SDA_OUT();//sda�����
	A3_IIC_SCL=0;
	A3_IIC_SDA=0;//STOP:when CLK is high DATA change form low to high
 	delay_us(4);
	A3_IIC_SCL=1; 
	A3_IIC_SDA=1;//����I2C���߽����ź�
	delay_us(4);							   	
}
//�ȴ�Ӧ���źŵ���
//����ֵ��1������Ӧ��ʧ��
//        0������Ӧ��ɹ�
u8 A3_IIC_Wait_Ack(void)
{
	u8 ucErrTime=0;
	A3_SDA_IN();      //SDA����Ϊ����  	
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
	A3_IIC_SCL=0;//ʱ�����0 	   
	return 0;  
} 
//����ACKӦ��
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
//������ACKӦ��		    
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
//IIC����һ���ֽ�
//���شӻ�����Ӧ��
//1����Ӧ��
//0����Ӧ��			  
void A3_IIC_Send_Byte(u8 txd)
{                        
    u8 t;   
		A3_SDA_OUT(); 	    
    A3_IIC_SCL=0;//����ʱ�ӿ�ʼ���ݴ���
    for(t=0;t<8;t++)
    {              
        A3_IIC_SDA=(txd&0x80)>>7;
        txd<<=1; 	  
		delay_us(2);   //��TEA5767��������ʱ���Ǳ����
		A3_IIC_SCL=1;
		delay_us(2); 
		A3_IIC_SCL=0;	
		delay_us(2);
    }	 
} 	    
//��1���ֽڣ�ack=1ʱ������ACK��ack=0������nACK   
u8 A3_IIC_Read_Byte(unsigned char ack)
{
	unsigned char i,receive=0;
	A3_SDA_IN();//SDA����Ϊ����
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
        A3_IIC_NAck();//����nACK
    else
        A3_IIC_Ack(); //����ACK   
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







