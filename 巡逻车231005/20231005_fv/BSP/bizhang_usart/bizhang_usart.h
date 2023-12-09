#ifndef BIZHANG_USART_H
#define BIZHANG_USART_H



#include "stm32f4xx.h"

#define  RCC_AHB1Periph_GPIOTx     RCC_AHB1Periph_GPIOC     //gpioʱ�� ����
#define  RCC_AHB1Periph_GPIORx     RCC_AHB1Periph_GPIOD     //gpioʱ�� ����
#define  GPIO_Pin_Rx              GPIO_Pin_2
#define  GPIO_Pin_Tx              GPIO_Pin_12
#define  GPIO_PinSourceRx         GPIO_PinSource2
#define  GPIO_PinSourceTx         GPIO_PinSource12

#define  RCC_APB2Periph_USARTx    RCC_APB1Periph_UART5    //����
#define  GPIO_AF_USARTx           GPIO_AF_UART5
#define  usartx										UART5
#define  USARTx_IRQn							UART5_IRQn

#define  usart_bound              56000                     //������
#define  usart_stopbits						USART_StopBits_1				 //ֹͣλ
#define	 usart_parity							USART_Parity_No					 //��żУ��
#define	 usart_wordlength         USART_WordLength_8b			 //����λ

#define  RCC_AHB1Periph_DMAx      RCC_AHB1Periph_DMA1      //DMAʱ��
#define  DMAx_Streamx_rx          DMA1_Stream0             //dma������
//#define  DMAx_Streamx_tx          DMA2_Stream7 
#define  DMA_Channel_x            DMA_Channel_4            //dmaͨ��

extern void bizhang_Usart(uint8_t *rx1_buf, uint8_t *rx2_buf, uint16_t dma_buf_num);
extern void usart_restart(uint16_t dma_buf_num);
#endif
