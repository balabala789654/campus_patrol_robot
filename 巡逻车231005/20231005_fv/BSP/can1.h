#ifndef __CAN1_H
#define __CAN1_H	 
#include "sys.h"	
#define fp32 float
	
//===============CAN1 GPIO的初始化=============
#define ADVANCE_CAN1_GPIOx_CLK  RCC_AHB1Periph_GPIOB   
#define ADVANCE_CAN1_CLK        RCC_APB1Periph_CAN1
#define ADVANCE_CAN1_RX  				GPIO_Pin_8 //can的接收
#define ADVANCE_CAN1_TX  				GPIO_Pin_9 
#define ADVANCE_CAN1_GPIOx  		GPIOB
#define ADVANCE_CAN1_RX_PIN			GPIO_PinSource8   //复用引脚
#define ADVANCE_CAN1_TX_PIN			GPIO_PinSource9	

void CAN1_Mode_Init(u8 tsjw,u8 tbs2,u8 tbs1,u16 brp,u8 mode);//CAN初始化
#endif
