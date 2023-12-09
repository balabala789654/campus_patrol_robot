#include "time5.h"

void TIM5_PWM_Init(u32 arr, u32 psc)//arr：自动重装值；psc：时钟预分频数
{
    GPIO_InitTypeDef GPIO_InitStructure;
    TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
    TIM_OCInitTypeDef  TIM_OCInitStructure;

    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM5, ENABLE);  	//TIM5时钟使能
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE); 	//使能PORTA时钟

    GPIO_PinAFConfig(GPIOA, GPIO_PinSource0, GPIO_AF_TIM5); //GPIOA0复用为定时器5
/**/GPIO_PinAFConfig(GPIOA, GPIO_PinSource1, GPIO_AF_TIM5); //GPIOA0复用为定时器5
	
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;           //GPIOA0
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;        //复用
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;	//速度100MHz
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;      //推挽复用输出
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;        //上拉
    GPIO_Init(GPIOA, &GPIO_InitStructure);             //初始化PA0

/**/GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1;           //GPIOA1
/**/GPIO_Init(GPIOA, &GPIO_InitStructure);             //初始化PA1
		
    TIM_TimeBaseStructure.TIM_Prescaler = psc; //定时器分频
    TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up; //向上计数模式
    TIM_TimeBaseStructure.TIM_Period = arr; //自动重装载值
    TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;

    TIM_TimeBaseInit(TIM5, &TIM_TimeBaseStructure); //初始化定时器5
/*
	参数为	canshuTIM_OCMode_PWM1、TIM_OCPolarity_High
	当计时器值小于比较器设定值时则TIMX输出脚此时输出高电位。
	当计时器值大于或等于比较器设定值时则TIMX输出脚此时输出低电位。*/
    //初始化TIM5 Channel PWM模式
    TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1; //选择定时器模式：TIM脉冲宽度调制模式1
    TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable; //比较使能输出
/**/TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High; //输出极性：TIM输出比较极性低
    TIM_OCInitStructure.TIM_Pulse = 0;//比较初始值
    TIM_OC1Init(TIM5, &TIM_OCInitStructure);  //根据T指定的参数初始化外设TIM5 OC1
		TIM_OC1PreloadConfig(TIM5, TIM_OCPreload_Enable);  //使能TIM5在CCR1是的预装载寄存器

/**/TIM_OC2Init(TIM5, &TIM_OCInitStructure);  //根据T指定的参数初始化外设TIM5 OC1
/**/TIM_OC2PreloadConfig(TIM5, TIM_OCPreload_Enable);  //使能TIM5在CCR1是的预装载寄存器
		
    TIM_ARRPreloadConfig(TIM5, ENABLE); //ARPE使能

    TIM_Cmd(TIM5, ENABLE);  //使能TIM5
}


