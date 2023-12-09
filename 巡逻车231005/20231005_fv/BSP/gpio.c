#include "gpio.h"

/*************************LED初始化***************************/
void LED_Init(void)
{    	 
	GPIO_InitTypeDef  GPIO_InitStructure;

	RCC_AHB1PeriphClockCmd(LED1_CLK , ENABLE);	//使能GPIOD时钟
  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOG , ENABLE);	//使能GPIOD时钟
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA , ENABLE);	//使能GPIOD时钟


	GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_7| GPIO_Pin_8 ;	//G7 红刹车  G8右黄
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;			//普通输出模式
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;			//推挽输出
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;		//100MHz
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;			//上拉
	GPIO_Init(GPIOG, &GPIO_InitStructure);					//初始化GPIO
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8;	//左黄
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;			//普通输出模式
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;			//推挽输出
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;		//100MHz
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;			//上拉
	GPIO_Init(GPIOA, &GPIO_InitStructure);					//初始化GPIO
	
		//GPIOC10,C11,C12初始化设置
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3|GPIO_Pin_0|GPIO_Pin_4;	//启动灯 D0氛围灯 D4警示灯
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;			//普通输出模式//D3启动灯
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;			//推挽输出
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;		//100MHz
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;			//上拉
	GPIO_Init(GPIOD, &GPIO_InitStructure);					//初始化GPIO
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_15;	//启动灯按下检测 按下——0 
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;			//普通输出模式
	GPIO_InitStructure.GPIO_OType = GPIO_OType_OD;			//推挽输出
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;		//100MHz
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;			//上拉
	GPIO_Init(GPIOA, &GPIO_InitStructure);					//初始化GPIO
	
	LED_ALL_OFF
}

/*************************气缸初始化***************************/
void Air_Cylinder_Init(void)  
{

    GPIO_InitTypeDef GPIO_InitStructure;

    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOE , ENABLE);


    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_13|GPIO_Pin_14|GPIO_Pin_15;//GPIO_Pin_8|GPIO_Pin_9|GPIO_Pin_10|GPIO_Pin_11|GPIO_Pin_12|
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
    GPIO_Init(GPIOE, &GPIO_InitStructure);
	
	   Air_Cylinder_ALLOFF
}

/*************************电缸初始化***************************/
void Electric_actuator_Init(void)  
{

    GPIO_InitTypeDef GPIO_InitStructure;

    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD , ENABLE);


    GPIO_InitStructure.GPIO_Pin =	GPIO_Pin_12|GPIO_Pin_13;//|GPIO_Pin_14|GPIO_Pin_15
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
    GPIO_Init(GPIOD, &GPIO_InitStructure);
	
		Pitch_Stop
}
/*************************语音初始化***************************/
void yuyin_Init(void)
{    	 
	GPIO_InitTypeDef  GPIO_InitStructure;

	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOH , ENABLE);	//使能GPIOD时钟
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOI , ENABLE);
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_13|GPIO_Pin_14|GPIO_Pin_15;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;			//普通输入模式
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;			//推挽输出
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;		//100MHz
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;			//上拉
	GPIO_Init(GPIOH, &GPIO_InitStructure);					//初始化GPIO
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0|GPIO_Pin_1|GPIO_Pin_2|GPIO_Pin_3;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;			//普通输入模式
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;			//推挽输出
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;		//100MHz
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;			//上拉
	GPIO_Init(GPIOI, &GPIO_InitStructure);					//初始化GPIO

}

//********************继电器*************
void RELAY_Init(void)
{
	    GPIO_InitTypeDef GPIO_InitStructure;

    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOE , ENABLE);


    GPIO_InitStructure.GPIO_Pin =	GPIO_Pin_2|GPIO_Pin_3|GPIO_Pin_4|GPIO_Pin_5|GPIO_Pin_6;//
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
    GPIO_Init(GPIOE, &GPIO_InitStructure);
	
		GPIO_ResetBits(GPIOE,GPIO_Pin_2);
		GPIO_ResetBits(GPIOE,GPIO_Pin_3);
		GPIO_ResetBits(GPIOE,GPIO_Pin_4);
		GPIO_ResetBits(GPIOE,GPIO_Pin_5);
		GPIO_ResetBits(GPIOE,GPIO_Pin_6);
}

