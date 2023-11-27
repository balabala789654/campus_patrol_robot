#ifndef __USART_H
#define __USART_H
#include "stdio.h"	
#include "stm32f4xx_conf.h"
#include "sys.h" 

void usart1_init(u32 bound, uint8_t* rx1_buf);
void usart2_Init(uint8_t* rx1_buf, uint8_t* rx2_buf, uint16_t dma_buf_num);
void RC_unable(void);
void RC_restart(uint16_t dma_buf_num);
void usart6_init(u32 bound);

extern uint8_t usart1_buffer[10];
extern uint8_t usart1_buffer2[10];

#define MAX_interaction_byte 20
#endif


