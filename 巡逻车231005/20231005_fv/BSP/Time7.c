#include "Time7.h"

u32 time4_count = 0;

void TIM4_BAS_Init(u16 arr,u16 psc)
{   
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4,ENABLE);  ///使能TIM7时钟
	
	TIM_TimeBaseInitStructure.TIM_Period = arr; 	//自动重装载值
	TIM_TimeBaseInitStructure.TIM_Prescaler=psc;  //定时器分频
	TIM_TimeBaseInitStructure.TIM_CounterMode=TIM_CounterMode_Up; //向上计数模式
	TIM_TimeBaseInitStructure.TIM_ClockDivision=TIM_CKD_DIV2; 
	
	TIM_TimeBaseInit(TIM4,&TIM_TimeBaseInitStructure);//初始化TIM7
	
	TIM_ITConfig(TIM4,TIM_IT_Update,ENABLE); //允许定时器7更新中断
	//TIM_Cmd(TIM7,ENABLE); //使能定时器7
	
	NVIC_InitStructure.NVIC_IRQChannel=TIM4_IRQn;//定时器7中断
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=0x01; //抢占优先级1
	NVIC_InitStructure.NVIC_IRQChannelSubPriority=0x00; //子优先级0
	NVIC_InitStructure.NVIC_IRQChannelCmd=ENABLE;
	NVIC_Init(&NVIC_InitStructure);	
}


void TIM4_IRQHandler(void)
{
	if(TIM_GetITStatus(TIM4,TIM_IT_Update)==SET) //溢出中断
	{		
		TIM_ClearITPendingBit(TIM4,TIM_IT_Update);  //清除中断标志位	
		time4_count ++;
	}
}


