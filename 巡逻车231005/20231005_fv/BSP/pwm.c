#include "pwm.h"


/* 

TIM5_PWM_Init(4999, 84 - 1)
84M/84=1Mhz(1,000,000)的计数频率,重装载值5000，所以PWM频率为 1,000,000/5000=200hz.

*/

void TIM5_PWM_Init(u32 arr, u32 psc)//arr：自动重装值；psc：时钟预分频数
{
    GPIO_InitTypeDef GPIO_InitStructure;
    TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
    TIM_OCInitTypeDef  TIM_OCInitStructure;

    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM5, ENABLE);  	//TIM5时钟使能
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE); 	//使能PORTA时钟

    GPIO_PinAFConfig(GPIOA, GPIO_PinSource0, GPIO_AF_TIM5); //GPIOA0复用为定时器5
/**/GPIO_PinAFConfig(GPIOA, GPIO_PinSource1, GPIO_AF_TIM5); //GPIOA1复用为定时器5
	  GPIO_PinAFConfig(GPIOA, GPIO_PinSource2, GPIO_AF_TIM5); //GPIOA2复用为定时器5
/**/GPIO_PinAFConfig(GPIOA, GPIO_PinSource3, GPIO_AF_TIM5); //GPIOA3复用为定时器5
	
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0|GPIO_Pin_1|GPIO_Pin_2|GPIO_Pin_3;           //GPIOA0
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
/**/TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High; //输出极性：TIM输出比较极性高
    TIM_OCInitStructure.TIM_Pulse = 0;//比较初始值
    TIM_OC1Init(TIM5, &TIM_OCInitStructure);  //根据T指定的参数初始化外设TIM5 OC1
		TIM_OC1PreloadConfig(TIM5, TIM_OCPreload_Enable);  //使能TIM5在CCR1是的预装载寄存器

/**/TIM_OC2Init(TIM5, &TIM_OCInitStructure);  //根据T指定的参数初始化外设TIM5 OC1
/**/TIM_OC2PreloadConfig(TIM5, TIM_OCPreload_Enable);  //使能TIM5在CCR1是的预装载寄存器
    TIM_OC3Init(TIM5, &TIM_OCInitStructure);  //根据T指定的参数初始化外设TIM5 OC1
		TIM_OC3PreloadConfig(TIM5, TIM_OCPreload_Enable);  //使能TIM5在CCR1是的预装载寄存器    
		TIM_OC4Init(TIM5, &TIM_OCInitStructure);  //根据T指定的参数初始化外设TIM5 OC1
		TIM_OC4PreloadConfig(TIM5, TIM_OCPreload_Enable);  //使能TIM5在CCR1是的预装载寄存器
		
    TIM_ARRPreloadConfig(TIM5, ENABLE); //ARPE使能

    TIM_Cmd(TIM5, ENABLE);  //使能TIM5
}



/*================================================================  TIM1  =================================================================*/
/**
  * @brief  GPIO端口配置函数 
  * @param  void
  * @retval void
  */
static void TIM1_GPIO_Config(void)
{
	GPIO_InitTypeDef GPIOE_InitStructure;//申明GPIO初始化数据结构体变量
	
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOE, ENABLE);//使能GPIOE时钟
	
	GPIO_PinAFConfig(GPIOE,GPIO_PinSource9 ,GPIO_AF_TIM1);//端口GPIOE_9,GPIOE_11,GPIOE_13,GPIOE_14映射到TIM1
	GPIO_PinAFConfig(GPIOE,GPIO_PinSource11,GPIO_AF_TIM1);//端口GPIOE_9,GPIOE_11,GPIOE_13,GPIOE_14映射到TIM1
	GPIO_PinAFConfig(GPIOE,GPIO_PinSource13,GPIO_AF_TIM1);//端口GPIOE_9,GPIOE_11,GPIOE_13,GPIOE_14映射到TIM1
	GPIO_PinAFConfig(GPIOE,GPIO_PinSource14,GPIO_AF_TIM1);//端口GPIOE_9,GPIOE_11,GPIOE_13,GPIOE_14映射到TIM1

  GPIOE_InitStructure.GPIO_Pin = GPIO_Pin_9 | GPIO_Pin_11 | GPIO_Pin_13 | GPIO_Pin_14;//指定端口GPIOE_9,GPIOE_11,GPIOE_13,GPIOE_14
  GPIOE_InitStructure.GPIO_Mode = GPIO_Mode_AF;//复用模式
  GPIOE_InitStructure.GPIO_OType = GPIO_OType_PP;//推挽输出
  GPIOE_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;//100MHz
  GPIOE_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;//上拉
  GPIO_Init(GPIOE, &GPIOE_InitStructure);//初始化GPIOE
}


/**
  * @brief  TIM1时基初始化函数  
  * @param  arr:自动重装载值
  * 				psc:预分频系数
  * @retval void
  */
static void TIM1_TimeBase_Config(u16 arr,u16 psc)
{
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure;//申明TIM1时基初始化数据结构体变量
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM1,ENABLE);//使能TIM1时钟
		
	TIM_TimeBaseInitStructure.TIM_Period = arr; 	//自动重装载值
	TIM_TimeBaseInitStructure.TIM_Prescaler=psc;  //定时器分频
	TIM_TimeBaseInitStructure.TIM_CounterMode=TIM_CounterMode_Up;//向上计数模式
	TIM_TimeBaseInitStructure.TIM_ClockDivision=TIM_CKD_DIV1; //定时器频率与数字滤波器之间的分频比为 1:1
	TIM_TimeBaseInitStructure.TIM_RepetitionCounter = 0;//重复计数器的值
	
	TIM_TimeBaseInit(TIM1,&TIM_TimeBaseInitStructure);//初始化TIM1
}



/**
  * @brief  TIM1输出比较配置函数 
  * @param  void
  * @retval void
  */
static void TIM1_OutputCompare_Config(void)
{
	TIM_OCInitTypeDef TIM_OCInitStructure;
	
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;//PWM模式1
	TIM_OCInitStructure.TIM_OCIdleState = TIM_OCIdleState_Reset;//空闲输出电平为低
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;//比较输出极性为高
	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;//比较输出使能
	
	TIM_OC1PreloadConfig(TIM1,TIM_OCPreload_Enable);//TIM1_Channel_1预装载使能
	TIM_OC2PreloadConfig(TIM1,TIM_OCPreload_Enable);//TIM1_Channel_2预装载使能
	TIM_OC3PreloadConfig(TIM1,TIM_OCPreload_Enable);//TIM1_Channel_3预装载使能
	TIM_OC4PreloadConfig(TIM1,TIM_OCPreload_Enable);//TIM1_Channel_4预装载使能
	
	TIM_OC1Init(TIM1,&TIM_OCInitStructure);//初始化TIM1_Channel_1
	TIM_OC2Init(TIM1,&TIM_OCInitStructure);//初始化TIM1_Channel_2
	TIM_OC3Init(TIM1,&TIM_OCInitStructure);//初始化TIM1_Channel_3
	TIM_OC4Init(TIM1,&TIM_OCInitStructure);//初始化TIM1_Channel_4
}



/**
  * @brief  TIM1初始化为PWM输出模式
  * @param  Arr:自动重装载值
  * 				Psc:预分频系数
  * @retval void
  */
void TIM1_PWM_Init(u16 Arr,u16 Psc)
{
	TIM1_GPIO_Config();//GPIO端口配置
	
	TIM1_TimeBase_Config(Arr,Psc);//TIM1时基初始化函数
	
	TIM1_OutputCompare_Config();//TIM1输出比较配置
	
	TIM_CtrlPWMOutputs(TIM1,ENABLE);//使能TIM1外围主输出
	
	TIM_ARRPreloadConfig(TIM1,ENABLE);//使能ARR预装载寄存器
	
	TIM_Cmd(TIM1,ENABLE);//使能TIM1
}



/*================================================================  TIM8  =================================================================*/

/**
  * @brief  GPIO端口配置函数 
  * @param  void
  * @retval void
  */
static void TIM8_GPIO_Config(void)
{
	GPIO_InitTypeDef GPIOC_InitStructure;//申明GPIO初始化数据结构体变量
	GPIO_InitTypeDef GPIOI_InitStructure;//申明GPIO初始化数据结构体变量
	
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC, ENABLE);//使能GPIOC时钟
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOI, ENABLE);//使能GPIOI时钟
	
	GPIO_PinAFConfig(GPIOC,GPIO_PinSource6,GPIO_AF_TIM8);//端口GPIOC_6映射到TIM8
	GPIO_PinAFConfig(GPIOI,GPIO_PinSource6,GPIO_AF_TIM8);//端口GPIOI_6,GPIOI_7映射到TIM8
	GPIO_PinAFConfig(GPIOI,GPIO_PinSource7,GPIO_AF_TIM8);//端口GPIOI_6,GPIOI_7映射到TIM8

  GPIOC_InitStructure.GPIO_Pin = GPIO_Pin_6;//指定端口GPIOC6
  GPIOC_InitStructure.GPIO_Mode = GPIO_Mode_AF;//复用模式
  GPIOC_InitStructure.GPIO_OType = GPIO_OType_PP;//推挽输出
  GPIOC_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;//100MHz
  GPIOC_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;//上拉
	
	GPIOI_InitStructure.GPIO_Pin = GPIO_Pin_6 | GPIO_Pin_7;//指定端口GPIOI_6,GPIOI_7
  GPIOI_InitStructure.GPIO_Mode = GPIO_Mode_AF;//复用模式
  GPIOI_InitStructure.GPIO_OType = GPIO_OType_PP;//推挽输出
  GPIOI_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;//100MHz
  GPIOI_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;//上拉
	
	GPIO_Init(GPIOC, &GPIOC_InitStructure);//初始化GPIOC
	GPIO_Init(GPIOI, &GPIOI_InitStructure);//初始化GPIOI
}



/**
  * @brief  TIM8时基初始化函数  
  * @param  arr:自动重装载值
  * 				psc:预分频系数
  * @retval void
  */
static void TIM8_TimeBase_Config(u16 arr,u16 psc)
{
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure;//申明TIM8时基初始化数据结构体变量
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM8,ENABLE);//使能TIM8时钟
		
	TIM_TimeBaseInitStructure.TIM_Period = arr; 	//自动重装载值
	TIM_TimeBaseInitStructure.TIM_Prescaler=psc;  //定时器分频
	TIM_TimeBaseInitStructure.TIM_CounterMode=TIM_CounterMode_Up;//向上计数模式
	TIM_TimeBaseInitStructure.TIM_ClockDivision=TIM_CKD_DIV1; //定时器频率与数字滤波器之间的分频比为 1:1
	TIM_TimeBaseInitStructure.TIM_RepetitionCounter = 0;//重复计数器的值
	
	TIM_TimeBaseInit(TIM8,&TIM_TimeBaseInitStructure);//初始化TIM8
}



/**
  * @brief  TIM8输出比较配置函数 
  * @param  void
  * @retval void
  */
static void TIM8_OutputCompare_Config(void)
{
	TIM_OCInitTypeDef TIM_OCInitStructure;
	
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;//PWM模式1
	TIM_OCInitStructure.TIM_OCIdleState = TIM_OCIdleState_Reset;//空闲输出电平为低
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;//比较输出极性为高
	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;//比较输出使能
	
	TIM_OC1PreloadConfig(TIM8,TIM_OCPreload_Enable);//TIM8_Channel_1预装载使能
	TIM_OC2PreloadConfig(TIM8,TIM_OCPreload_Enable);//TIM8_Channel_2预装载使能
	TIM_OC3PreloadConfig(TIM8,TIM_OCPreload_Enable);//TIM8_Channel_3预装载使能
	
	TIM_OC1Init(TIM8,&TIM_OCInitStructure);//初始化TIM8_Channel_1
	TIM_OC2Init(TIM8,&TIM_OCInitStructure);//初始化TIM8_Channel_2
	TIM_OC3Init(TIM8,&TIM_OCInitStructure);//初始化TIM8_Channel_3
}



/**
  * @brief  TIM8初始化为PWM输出模式
  * @param  Arr:自动重装载值
  * 				Psc:预分频系数
  * @retval void
  */
void TIM8_PWM_Init(u16 Arr,u16 Psc)
{
	TIM8_GPIO_Config();//GPIO端口配置
	
	TIM8_TimeBase_Config(Arr,Psc);//TIM8时基初始化函数
	
	TIM8_OutputCompare_Config();//TIM8输出比较配置
	
	TIM_CtrlPWMOutputs(TIM8,ENABLE);//使能TIM8外围主输出
	
	TIM_ARRPreloadConfig(TIM8,ENABLE);//使能ARR预装载寄存器
	
	TIM_Cmd(TIM8,ENABLE);//使能TIM8
}

