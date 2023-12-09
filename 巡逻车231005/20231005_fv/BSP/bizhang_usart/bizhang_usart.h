#ifndef BIZHANG_USART_H
#define BIZHANG_USART_H



#include "stm32f4xx.h"

#define  RCC_AHB1Periph_GPIOTx     RCC_AHB1Periph_GPIOC     //gpio时钟 引脚
#define  RCC_AHB1Periph_GPIORx     RCC_AHB1Periph_GPIOD     //gpio时钟 引脚
#define  GPIO_Pin_Rx              GPIO_Pin_2
#define  GPIO_Pin_Tx              GPIO_Pin_12
#define  GPIO_PinSourceRx         GPIO_PinSource2
#define  GPIO_PinSourceTx         GPIO_PinSource12

#define  RCC_APB2Periph_USARTx    RCC_APB1Periph_UART5    //串口
#define  GPIO_AF_USARTx           GPIO_AF_UART5
#define  usartx										UART5
#define  USARTx_IRQn							UART5_IRQn

#define  usart_bound              56000                     //波特率
#define  usart_stopbits						USART_StopBits_1				 //停止位
#define	 usart_parity							USART_Parity_No					 //奇偶校验
#define	 usart_wordlength         USART_WordLength_8b			 //数据位

#define  RCC_AHB1Periph_DMAx      RCC_AHB1Periph_DMA1      //DMA时钟
#define  DMAx_Streamx_rx          DMA1_Stream0             //dma数据流
//#define  DMAx_Streamx_tx          DMA2_Stream7 
#define  DMA_Channel_x            DMA_Channel_4            //dma通道

extern void bizhang_Usart(uint8_t *rx1_buf, uint8_t *rx2_buf, uint16_t dma_buf_num);
extern void usart_restart(uint16_t dma_buf_num);
#endif
