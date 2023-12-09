#include "pwm.h"


/* 

TIM5_PWM_Init(4999, 84 - 1)
84M/84=1Mhz(1,000,000)�ļ���Ƶ��,��װ��ֵ5000������PWMƵ��Ϊ 1,000,000/5000=200hz.

*/

void TIM5_PWM_Init(u32 arr, u32 psc)//arr���Զ���װֵ��psc��ʱ��Ԥ��Ƶ��
{
    GPIO_InitTypeDef GPIO_InitStructure;
    TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
    TIM_OCInitTypeDef  TIM_OCInitStructure;

    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM5, ENABLE);  	//TIM5ʱ��ʹ��
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE); 	//ʹ��PORTAʱ��

    GPIO_PinAFConfig(GPIOA, GPIO_PinSource0, GPIO_AF_TIM5); //GPIOA0����Ϊ��ʱ��5
/**/GPIO_PinAFConfig(GPIOA, GPIO_PinSource1, GPIO_AF_TIM5); //GPIOA1����Ϊ��ʱ��5
	  GPIO_PinAFConfig(GPIOA, GPIO_PinSource2, GPIO_AF_TIM5); //GPIOA2����Ϊ��ʱ��5
/**/GPIO_PinAFConfig(GPIOA, GPIO_PinSource3, GPIO_AF_TIM5); //GPIOA3����Ϊ��ʱ��5
	
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0|GPIO_Pin_1|GPIO_Pin_2|GPIO_Pin_3;           //GPIOA0
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;        //����
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;	//�ٶ�100MHz
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;      //���츴�����
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;        //����
    GPIO_Init(GPIOA, &GPIO_InitStructure);             //��ʼ��PA0

/**/GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1;           //GPIOA1
/**/GPIO_Init(GPIOA, &GPIO_InitStructure);             //��ʼ��PA1
		
    TIM_TimeBaseStructure.TIM_Prescaler = psc; //��ʱ����Ƶ
    TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up; //���ϼ���ģʽ
    TIM_TimeBaseStructure.TIM_Period = arr; //�Զ���װ��ֵ
    TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;

    TIM_TimeBaseInit(TIM5, &TIM_TimeBaseStructure); //��ʼ����ʱ��5
/*
	����Ϊ	canshuTIM_OCMode_PWM1��TIM_OCPolarity_High
	����ʱ��ֵС�ڱȽ����趨ֵʱ��TIMX����Ŵ�ʱ����ߵ�λ��
	����ʱ��ֵ���ڻ���ڱȽ����趨ֵʱ��TIMX����Ŵ�ʱ����͵�λ��*/
    //��ʼ��TIM5 Channel PWMģʽ
    TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1; //ѡ��ʱ��ģʽ��TIM�����ȵ���ģʽ1
    TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable; //�Ƚ�ʹ�����
/**/TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High; //������ԣ�TIM����Ƚϼ��Ը�
    TIM_OCInitStructure.TIM_Pulse = 0;//�Ƚϳ�ʼֵ
    TIM_OC1Init(TIM5, &TIM_OCInitStructure);  //����Tָ���Ĳ�����ʼ������TIM5 OC1
		TIM_OC1PreloadConfig(TIM5, TIM_OCPreload_Enable);  //ʹ��TIM5��CCR1�ǵ�Ԥװ�ؼĴ���

/**/TIM_OC2Init(TIM5, &TIM_OCInitStructure);  //����Tָ���Ĳ�����ʼ������TIM5 OC1
/**/TIM_OC2PreloadConfig(TIM5, TIM_OCPreload_Enable);  //ʹ��TIM5��CCR1�ǵ�Ԥװ�ؼĴ���
    TIM_OC3Init(TIM5, &TIM_OCInitStructure);  //����Tָ���Ĳ�����ʼ������TIM5 OC1
		TIM_OC3PreloadConfig(TIM5, TIM_OCPreload_Enable);  //ʹ��TIM5��CCR1�ǵ�Ԥװ�ؼĴ���    
		TIM_OC4Init(TIM5, &TIM_OCInitStructure);  //����Tָ���Ĳ�����ʼ������TIM5 OC1
		TIM_OC4PreloadConfig(TIM5, TIM_OCPreload_Enable);  //ʹ��TIM5��CCR1�ǵ�Ԥװ�ؼĴ���
		
    TIM_ARRPreloadConfig(TIM5, ENABLE); //ARPEʹ��

    TIM_Cmd(TIM5, ENABLE);  //ʹ��TIM5
}



/*================================================================  TIM1  =================================================================*/
/**
  * @brief  GPIO�˿����ú��� 
  * @param  void
  * @retval void
  */
static void TIM1_GPIO_Config(void)
{
	GPIO_InitTypeDef GPIOE_InitStructure;//����GPIO��ʼ�����ݽṹ�����
	
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOE, ENABLE);//ʹ��GPIOEʱ��
	
	GPIO_PinAFConfig(GPIOE,GPIO_PinSource9 ,GPIO_AF_TIM1);//�˿�GPIOE_9,GPIOE_11,GPIOE_13,GPIOE_14ӳ�䵽TIM1
	GPIO_PinAFConfig(GPIOE,GPIO_PinSource11,GPIO_AF_TIM1);//�˿�GPIOE_9,GPIOE_11,GPIOE_13,GPIOE_14ӳ�䵽TIM1
	GPIO_PinAFConfig(GPIOE,GPIO_PinSource13,GPIO_AF_TIM1);//�˿�GPIOE_9,GPIOE_11,GPIOE_13,GPIOE_14ӳ�䵽TIM1
	GPIO_PinAFConfig(GPIOE,GPIO_PinSource14,GPIO_AF_TIM1);//�˿�GPIOE_9,GPIOE_11,GPIOE_13,GPIOE_14ӳ�䵽TIM1

  GPIOE_InitStructure.GPIO_Pin = GPIO_Pin_9 | GPIO_Pin_11 | GPIO_Pin_13 | GPIO_Pin_14;//ָ���˿�GPIOE_9,GPIOE_11,GPIOE_13,GPIOE_14
  GPIOE_InitStructure.GPIO_Mode = GPIO_Mode_AF;//����ģʽ
  GPIOE_InitStructure.GPIO_OType = GPIO_OType_PP;//�������
  GPIOE_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;//100MHz
  GPIOE_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;//����
  GPIO_Init(GPIOE, &GPIOE_InitStructure);//��ʼ��GPIOE
}


/**
  * @brief  TIM1ʱ����ʼ������  
  * @param  arr:�Զ���װ��ֵ
  * 				psc:Ԥ��Ƶϵ��
  * @retval void
  */
static void TIM1_TimeBase_Config(u16 arr,u16 psc)
{
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure;//����TIM1ʱ����ʼ�����ݽṹ�����
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM1,ENABLE);//ʹ��TIM1ʱ��
		
	TIM_TimeBaseInitStructure.TIM_Period = arr; 	//�Զ���װ��ֵ
	TIM_TimeBaseInitStructure.TIM_Prescaler=psc;  //��ʱ����Ƶ
	TIM_TimeBaseInitStructure.TIM_CounterMode=TIM_CounterMode_Up;//���ϼ���ģʽ
	TIM_TimeBaseInitStructure.TIM_ClockDivision=TIM_CKD_DIV1; //��ʱ��Ƶ���������˲���֮��ķ�Ƶ��Ϊ 1:1
	TIM_TimeBaseInitStructure.TIM_RepetitionCounter = 0;//�ظ���������ֵ
	
	TIM_TimeBaseInit(TIM1,&TIM_TimeBaseInitStructure);//��ʼ��TIM1
}



/**
  * @brief  TIM1����Ƚ����ú��� 
  * @param  void
  * @retval void
  */
static void TIM1_OutputCompare_Config(void)
{
	TIM_OCInitTypeDef TIM_OCInitStructure;
	
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;//PWMģʽ1
	TIM_OCInitStructure.TIM_OCIdleState = TIM_OCIdleState_Reset;//���������ƽΪ��
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;//�Ƚ��������Ϊ��
	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;//�Ƚ����ʹ��
	
	TIM_OC1PreloadConfig(TIM1,TIM_OCPreload_Enable);//TIM1_Channel_1Ԥװ��ʹ��
	TIM_OC2PreloadConfig(TIM1,TIM_OCPreload_Enable);//TIM1_Channel_2Ԥװ��ʹ��
	TIM_OC3PreloadConfig(TIM1,TIM_OCPreload_Enable);//TIM1_Channel_3Ԥװ��ʹ��
	TIM_OC4PreloadConfig(TIM1,TIM_OCPreload_Enable);//TIM1_Channel_4Ԥװ��ʹ��
	
	TIM_OC1Init(TIM1,&TIM_OCInitStructure);//��ʼ��TIM1_Channel_1
	TIM_OC2Init(TIM1,&TIM_OCInitStructure);//��ʼ��TIM1_Channel_2
	TIM_OC3Init(TIM1,&TIM_OCInitStructure);//��ʼ��TIM1_Channel_3
	TIM_OC4Init(TIM1,&TIM_OCInitStructure);//��ʼ��TIM1_Channel_4
}



/**
  * @brief  TIM1��ʼ��ΪPWM���ģʽ
  * @param  Arr:�Զ���װ��ֵ
  * 				Psc:Ԥ��Ƶϵ��
  * @retval void
  */
void TIM1_PWM_Init(u16 Arr,u16 Psc)
{
	TIM1_GPIO_Config();//GPIO�˿�����
	
	TIM1_TimeBase_Config(Arr,Psc);//TIM1ʱ����ʼ������
	
	TIM1_OutputCompare_Config();//TIM1����Ƚ�����
	
	TIM_CtrlPWMOutputs(TIM1,ENABLE);//ʹ��TIM1��Χ�����
	
	TIM_ARRPreloadConfig(TIM1,ENABLE);//ʹ��ARRԤװ�ؼĴ���
	
	TIM_Cmd(TIM1,ENABLE);//ʹ��TIM1
}



/*================================================================  TIM8  =================================================================*/

/**
  * @brief  GPIO�˿����ú��� 
  * @param  void
  * @retval void
  */
static void TIM8_GPIO_Config(void)
{
	GPIO_InitTypeDef GPIOC_InitStructure;//����GPIO��ʼ�����ݽṹ�����
	GPIO_InitTypeDef GPIOI_InitStructure;//����GPIO��ʼ�����ݽṹ�����
	
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC, ENABLE);//ʹ��GPIOCʱ��
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOI, ENABLE);//ʹ��GPIOIʱ��
	
	GPIO_PinAFConfig(GPIOC,GPIO_PinSource6,GPIO_AF_TIM8);//�˿�GPIOC_6ӳ�䵽TIM8
	GPIO_PinAFConfig(GPIOI,GPIO_PinSource6,GPIO_AF_TIM8);//�˿�GPIOI_6,GPIOI_7ӳ�䵽TIM8
	GPIO_PinAFConfig(GPIOI,GPIO_PinSource7,GPIO_AF_TIM8);//�˿�GPIOI_6,GPIOI_7ӳ�䵽TIM8

  GPIOC_InitStructure.GPIO_Pin = GPIO_Pin_6;//ָ���˿�GPIOC6
  GPIOC_InitStructure.GPIO_Mode = GPIO_Mode_AF;//����ģʽ
  GPIOC_InitStructure.GPIO_OType = GPIO_OType_PP;//�������
  GPIOC_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;//100MHz
  GPIOC_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;//����
	
	GPIOI_InitStructure.GPIO_Pin = GPIO_Pin_6 | GPIO_Pin_7;//ָ���˿�GPIOI_6,GPIOI_7
  GPIOI_InitStructure.GPIO_Mode = GPIO_Mode_AF;//����ģʽ
  GPIOI_InitStructure.GPIO_OType = GPIO_OType_PP;//�������
  GPIOI_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;//100MHz
  GPIOI_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;//����
	
	GPIO_Init(GPIOC, &GPIOC_InitStructure);//��ʼ��GPIOC
	GPIO_Init(GPIOI, &GPIOI_InitStructure);//��ʼ��GPIOI
}



/**
  * @brief  TIM8ʱ����ʼ������  
  * @param  arr:�Զ���װ��ֵ
  * 				psc:Ԥ��Ƶϵ��
  * @retval void
  */
static void TIM8_TimeBase_Config(u16 arr,u16 psc)
{
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure;//����TIM8ʱ����ʼ�����ݽṹ�����
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM8,ENABLE);//ʹ��TIM8ʱ��
		
	TIM_TimeBaseInitStructure.TIM_Period = arr; 	//�Զ���װ��ֵ
	TIM_TimeBaseInitStructure.TIM_Prescaler=psc;  //��ʱ����Ƶ
	TIM_TimeBaseInitStructure.TIM_CounterMode=TIM_CounterMode_Up;//���ϼ���ģʽ
	TIM_TimeBaseInitStructure.TIM_ClockDivision=TIM_CKD_DIV1; //��ʱ��Ƶ���������˲���֮��ķ�Ƶ��Ϊ 1:1
	TIM_TimeBaseInitStructure.TIM_RepetitionCounter = 0;//�ظ���������ֵ
	
	TIM_TimeBaseInit(TIM8,&TIM_TimeBaseInitStructure);//��ʼ��TIM8
}



/**
  * @brief  TIM8����Ƚ����ú��� 
  * @param  void
  * @retval void
  */
static void TIM8_OutputCompare_Config(void)
{
	TIM_OCInitTypeDef TIM_OCInitStructure;
	
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;//PWMģʽ1
	TIM_OCInitStructure.TIM_OCIdleState = TIM_OCIdleState_Reset;//���������ƽΪ��
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;//�Ƚ��������Ϊ��
	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;//�Ƚ����ʹ��
	
	TIM_OC1PreloadConfig(TIM8,TIM_OCPreload_Enable);//TIM8_Channel_1Ԥװ��ʹ��
	TIM_OC2PreloadConfig(TIM8,TIM_OCPreload_Enable);//TIM8_Channel_2Ԥװ��ʹ��
	TIM_OC3PreloadConfig(TIM8,TIM_OCPreload_Enable);//TIM8_Channel_3Ԥװ��ʹ��
	
	TIM_OC1Init(TIM8,&TIM_OCInitStructure);//��ʼ��TIM8_Channel_1
	TIM_OC2Init(TIM8,&TIM_OCInitStructure);//��ʼ��TIM8_Channel_2
	TIM_OC3Init(TIM8,&TIM_OCInitStructure);//��ʼ��TIM8_Channel_3
}



/**
  * @brief  TIM8��ʼ��ΪPWM���ģʽ
  * @param  Arr:�Զ���װ��ֵ
  * 				Psc:Ԥ��Ƶϵ��
  * @retval void
  */
void TIM8_PWM_Init(u16 Arr,u16 Psc)
{
	TIM8_GPIO_Config();//GPIO�˿�����
	
	TIM8_TimeBase_Config(Arr,Psc);//TIM8ʱ����ʼ������
	
	TIM8_OutputCompare_Config();//TIM8����Ƚ�����
	
	TIM_CtrlPWMOutputs(TIM8,ENABLE);//ʹ��TIM8��Χ�����
	
	TIM_ARRPreloadConfig(TIM8,ENABLE);//ʹ��ARRԤװ�ؼĴ���
	
	TIM_Cmd(TIM8,ENABLE);//ʹ��TIM8
}

