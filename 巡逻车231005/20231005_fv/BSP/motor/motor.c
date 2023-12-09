#include "motor.h"


void Motor_GPIO_Init()
{
	  GPIO_InitTypeDef GPIO_InitStructure;
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD , ENABLE);

    GPIO_InitStructure.GPIO_Pin =	MOTOR1_STOP_Pin|MOTOR1_DIRECTION_Pin;//һ�����������ɲ���뷽�����
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
    GPIO_Init(MOTOR1__GPIOx, &GPIO_InitStructure);
	
	  GPIO_InitStructure.GPIO_Pin =	MOTOR2_STOP_Pin|MOTOR2_DIRECTION_Pin;//һ�����������ɲ���뷽�����
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
    GPIO_Init(MOTOR2__GPIOx, &GPIO_InitStructure);
	
	  WHEEL_STOP;
}


void Motor_PWM_Init(u16 arr,u16 psc)
{
	  GPIO_InitTypeDef GPIO_InitStructure;
    TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
    TIM_OCInitTypeDef  TIM_OCInitStructure;

    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);  	//TIM5ʱ��ʹ��
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE); 	//ʹ��PORTAʱ��
	  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC, ENABLE); 	//ʹ��PORTAʱ��


    GPIO_PinAFConfig(GPIOB, GPIO_PinSource0, GPIO_AF_TIM3); //GPIOA0����Ϊ��ʱ��5
/**/GPIO_PinAFConfig(GPIOB, GPIO_PinSource1, GPIO_AF_TIM3); //GPIOA1����Ϊ��ʱ��5
    GPIO_PinAFConfig(GPIOC, GPIO_PinSource6, GPIO_AF_TIM3); //GPIOA0����Ϊ��ʱ��5
/**/GPIO_PinAFConfig(GPIOC, GPIO_PinSource7, GPIO_AF_TIM3); //GPIOA1����Ϊ��ʱ��5
	
	    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6|GPIO_Pin_7;           //GPIOA0
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;        //����
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;	//�ٶ�100MHz
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;      //���츴�����
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;        //����
    GPIO_Init(GPIOC, &GPIO_InitStructure);             //��ʼ��PA0
	
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0|GPIO_Pin_1;           //GPIOA0
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;        //����
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;	//�ٶ�100MHz
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;      //���츴�����
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;        //����
    GPIO_Init(GPIOB, &GPIO_InitStructure);             //��ʼ��PA0

/**/GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1;           //GPIOA1
/**/GPIO_Init(GPIOB, &GPIO_InitStructure);             //��ʼ��PA1
		
    TIM_TimeBaseStructure.TIM_Prescaler = psc; //��ʱ����Ƶ
    TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up; //���ϼ���ģʽ
    TIM_TimeBaseStructure.TIM_Period = arr; //�Զ���װ��ֵ
    TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;

    TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure); //��ʼ����ʱ��5
/*
	����Ϊ	canshuTIM_OCMode_PWM1��TIM_OCPolarity_High
	����ʱ��ֵС�ڱȽ����趨ֵʱ��TIMX����Ŵ�ʱ����ߵ�λ��
	����ʱ��ֵ���ڻ���ڱȽ����趨ֵʱ��TIMX����Ŵ�ʱ����͵�λ��*/
    //��ʼ��TIM5 Channel PWMģʽ
    TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1; //ѡ��ʱ��ģʽ��TIM�����ȵ���ģʽ1
    TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable; //�Ƚ�ʹ�����
/**/TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High; //������ԣ�TIM����Ƚϼ��Ը�
    TIM_OCInitStructure.TIM_Pulse = 0;//�Ƚϳ�ʼֵ
    TIM_OC1Init(TIM3, &TIM_OCInitStructure);  //����Tָ���Ĳ�����ʼ������TIM5 OC1
		TIM_OC1PreloadConfig(TIM3, TIM_OCPreload_Enable);  //ʹ��TIM5��CCR1�ǵ�Ԥװ�ؼĴ���

/**/TIM_OC2Init(TIM3, &TIM_OCInitStructure);  //����Tָ���Ĳ�����ʼ������TIM5 OC1
/**/TIM_OC2PreloadConfig(TIM3, TIM_OCPreload_Enable);  //ʹ��TIM5��CCR1�ǵ�Ԥװ�ؼĴ���
    TIM_OC3Init(TIM3, &TIM_OCInitStructure);  //����Tָ���Ĳ�����ʼ������TIM5 OC1
		TIM_OC3PreloadConfig(TIM3, TIM_OCPreload_Enable);  //ʹ��TIM5��CCR1�ǵ�Ԥװ�ؼĴ���    
		TIM_OC4Init(TIM3, &TIM_OCInitStructure);  //����Tָ���Ĳ�����ʼ������TIM5 OC1
		TIM_OC4PreloadConfig(TIM3, TIM_OCPreload_Enable);  //ʹ��TIM5��CCR1�ǵ�Ԥװ�ؼĴ���
	
    TIM_ARRPreloadConfig(TIM3, ENABLE); //ARPEʹ��

    TIM_Cmd(TIM3, ENABLE);  //ʹ��TIM5
}

