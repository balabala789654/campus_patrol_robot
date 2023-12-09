#ifndef __GPIO_H
#define __GPIO_H
#include "sys.h"

#define LED1_Pin    GPIO_Pin_8
#define LED1_GPIOx  GPIOD
#define LED1_CLK    RCC_AHB1Periph_GPIOD

#define LED2_Pin    GPIO_Pin_9
#define LED2_GPIOx  GPIOD
#define LED2_CLK    RCC_AHB1Periph_GPIOD

#define LED3_Pin    GPIO_Pin_10
#define LED3_GPIOx  GPIOD
#define LED3_CLK    RCC_AHB1Periph_GPIOD

#define LED4_Pin    GPIO_Pin_11
#define LED4_GPIOx  GPIOD
#define LED4_CLK    RCC_AHB1Periph_GPIOD


#define LED5_Pin    GPIO_Pin_6
#define LED5_GPIOx  GPIOD
#define LED5_CLK    RCC_AHB1Periph_GPIOD

#define LED6_Pin    GPIO_Pin_7
#define LED6_GPIOx  GPIOD
#define LED6_CLK    RCC_AHB1Periph_GPIOD

#define LED7_Pin    GPIO_Pin_4
#define LED7_GPIOx  GPIOD
#define LED7_CLK    RCC_AHB1Periph_GPIOD

#define LED8_Pin    GPIO_Pin_5
#define LED8_GPIOx  GPIOD
#define LED8_CLK    RCC_AHB1Periph_GPIOD


#define LED9_Pin    GPIO_Pin_2
#define LED9_GPIOx  GPIOD
#define LED9_CLK    RCC_AHB1Periph_GPIOD

#define LED10_Pin    GPIO_Pin_3
#define LED10_GPIOx  GPIOD
#define LED10_CLK    RCC_AHB1Periph_GPIOD

#define LED11_Pin    GPIO_Pin_0
#define LED11_GPIOx  GPIOD
#define LED11_CLK    RCC_AHB1Periph_GPIOD

#define LED12_Pin    GPIO_Pin_1
#define LED12_GPIOx  GPIOD
#define LED12_CLK    RCC_AHB1Periph_GPIOD


#define OFF  0
#define ON 1

#define LED1(a)  if(a)\
				 GPIO_SetBits(LED1_GPIOx,LED1_Pin);\
		 else    GPIO_ResetBits(LED1_GPIOx,LED1_Pin);\


#define LED2(a)  if(a)\
				 GPIO_SetBits(LED2_GPIOx,LED2_Pin);\
		 else    GPIO_ResetBits(LED2_GPIOx,LED2_Pin);\


#define LED3(a)  if(a)\
				 GPIO_SetBits(LED3_GPIOx,LED3_Pin);\
		 else    GPIO_ResetBits(LED3_GPIOx,LED3_Pin);\


#define LED4(a)  if(a)\
				 GPIO_SetBits(LED4_GPIOx,LED4_Pin);\
		 else    GPIO_ResetBits(LED4_GPIOx,LED4_Pin);\


#define LED5(a)  if(a)\
				 GPIO_SetBits(LED5_GPIOx,LED5_Pin);\
		 else    GPIO_ResetBits(LED5_GPIOx,LED5_Pin);\


#define LED6(a)  if(a)\
				 GPIO_SetBits(LED6_GPIOx,LED6_Pin);\
		 else    GPIO_ResetBits(LED6_GPIOx,LED6_Pin);\


#define LED7(a)  if(a)\
				 GPIO_SetBits(LED7_GPIOx,LED7_Pin);\
		 else    GPIO_ResetBits(LED7_GPIOx,LED7_Pin);\


#define LED8(a)  if(a)\
				 GPIO_SetBits(LED8_GPIOx,LED8_Pin);\
		 else    GPIO_ResetBits(LED8_GPIOx,LED8_Pin);\

 
#define LED9(a)  if(a)\
				 GPIO_SetBits(LED9_GPIOx,LED9_Pin);\
		 else    GPIO_ResetBits(LED9_GPIOx,LED9_Pin);\

#define LED10(a)  if(a)\
				 GPIO_SetBits(LED10_GPIOx,LED10_Pin);\
		 else    GPIO_ResetBits(LED10_GPIOx,LED10_Pin);\


#define LED11(a)  if(a)\
				 GPIO_SetBits(LED11_GPIOx,LED11_Pin);\
		 else    GPIO_ResetBits(LED11_GPIOx,LED11_Pin);\

#define LED12(a)  if(a)\
				 GPIO_SetBits(LED12_GPIOx,LED12_Pin);\
		 else    GPIO_ResetBits(LED12_GPIOx,LED12_Pin);\


#define LED_ALL_ON  \
					LED1(ON);\
					LED2(ON);\
					LED3(ON);\
					LED4(ON);\
					LED5(ON);\
					LED6(ON);\
					LED7(ON);\
					LED8(ON);\
					LED9(ON);\
					LED10(ON);\
					LED11(ON);\
					LED12(ON)

#define LED_ALL_OFF \
					LED1(OFF);\
					LED2(OFF);\
					LED3(OFF);\
					LED4(OFF);\
					LED5(OFF);\
					LED6(OFF);\
					LED7(OFF);\
					LED8(OFF);\
					LED9(OFF);\
					LED10(OFF);\
					LED11(OFF);\
					LED12(OFF)

#define PE8(a)  if(a)\
				 GPIO_SetBits(GPIOE,GPIO_Pin_8);\
		 else    GPIO_ResetBits(GPIOE,GPIO_Pin_8);\

#define PE9(a)  if(a)\
				 GPIO_SetBits(GPIOE,GPIO_Pin_9);\
		 else    GPIO_ResetBits(GPIOE,GPIO_Pin_9);\

#define PE10(a)  if(a)\
				 GPIO_SetBits(GPIOE,GPIO_Pin_10);\
		 else    GPIO_ResetBits(GPIOE,GPIO_Pin_10);\

#define PE11(a)  if(a)\
				 GPIO_SetBits(GPIOE,GPIO_Pin_11);\
		 else    GPIO_ResetBits(GPIOE,GPIO_Pin_11);\

#define PE12(a)  if(a)\
				 GPIO_SetBits(GPIOE,GPIO_Pin_12);\
		 else    GPIO_ResetBits(GPIOE,GPIO_Pin_12);\

#define PE13(a)  if(a)\
				 GPIO_SetBits(GPIOE,GPIO_Pin_13);\
		 else    GPIO_ResetBits(GPIOE,GPIO_Pin_13);\

#define PE14(a)  if(a)\
				 GPIO_SetBits(GPIOE,GPIO_Pin_14);\
		 else    GPIO_ResetBits(GPIOE,GPIO_Pin_14);\

#define PE15(a)  if(a)\
				 GPIO_SetBits(GPIOE,GPIO_Pin_15);\
		 else    GPIO_ResetBits(GPIOE,GPIO_Pin_15);\

#define PD12(a)  if(a)\
				 GPIO_SetBits(GPIOE,GPIO_Pin_15);\
		 else    GPIO_ResetBits(GPIOE,GPIO_Pin_15);\

#define PD13(a)  if(a)\
				 GPIO_SetBits(GPIOE,GPIO_Pin_15);\
		 else    GPIO_ResetBits(GPIOE,GPIO_Pin_15);\
//尾灯
#define PA8(a)  if(a)\
				 GPIO_SetBits(GPIOA,GPIO_Pin_8);\
		 else    GPIO_ResetBits(GPIOA,GPIO_Pin_8);\

#define PG8(a)  if(a)\
				 GPIO_SetBits(GPIOG,GPIO_Pin_8);\
		 else    GPIO_ResetBits(GPIOG,GPIO_Pin_8);\

#define PG7(a)  if(a)\
				 GPIO_SetBits(GPIOG,GPIO_Pin_7);\
		 else    GPIO_ResetBits(GPIOG,GPIO_Pin_7);\
//启动灯
#define PD3(a)  if(a)\
				 GPIO_SetBits(GPIOD,GPIO_Pin_3);\
		 else    GPIO_ResetBits(GPIOD,GPIO_Pin_3);\

//警示灯
#define PD4(a)  if(a)\
				  GPIO_ResetBits(GPIOD,GPIO_Pin_4);\
		 else   GPIO_SetBits(GPIOD,GPIO_Pin_4);\

//氛围灯
#define PD0(a)  if(a)\
				 GPIO_SetBits(GPIOD,GPIO_Pin_0);\
		 else    GPIO_ResetBits(GPIOD,GPIO_Pin_0);\
/***************************************************/
#define LED1_TOGGLE()				GPIO_ToggleBits(LED1_GPIOx,LED1_PIN)
#define LED2_TOGGLE()				GPIO_ToggleBits(LED2_GPIOx,LED2_PIN)
#define LED3_TOGGLE()				GPIO_ToggleBits(LED3_GPIOx,LED3_PIN)
#define LED4_TOGGLE()				GPIO_ToggleBits(LED4_GPIOx,LED4_PIN)
#define LED5_TOGGLE()				GPIO_ToggleBits(LED5_GPIOx,LED5_PIN)
#define LED6_TOGGLE()				GPIO_ToggleBits(LED6_GPIOx,LED6_PIN)
#define LED7_TOGGLE()				GPIO_ToggleBits(LED7_GPIOx,LED7_PIN)
#define LED8_TOGGLE()				GPIO_ToggleBits(LED8_GPIOx,LED8_PIN)
#define LED9_TOGGLE()				GPIO_ToggleBits(LED9_GPIOx,LED9_PIN)
#define LED10_TOGGLE()			GPIO_ToggleBits(LED10_GPIOx,LED10_PIN)
#define LED11_TOGGLE()			GPIO_ToggleBits(LED11_GPIOx,LED11_Pin)
#define LED12_TOGGLE()			GPIO_ToggleBits(LED12_GPIOx,LED12_PIN)

/*********************夹子**********************/
#define 		Clamp_Up 						PE15(OFF)
#define 		Clamp_Down 					PE15(ON)

/*******************发球推杆********************/
#define 		Pushrod_Up 						PE13(OFF)
#define 		Pushrod_Down 					PE13(ON)

/*******************夹子抬升********************/
#define 		Lifting_Up 						PE14(OFF)
#define 		Lifting_Down 					PE14(ON)

/*******************Pitch向上********************/
#define Pitch_Up	\
					GPIO_SetBits(GPIOD,GPIO_Pin_13);\
					GPIO_ResetBits(GPIOD,GPIO_Pin_12);
/*******************Pitch向下********************/
#define Pitch_Down	\
					GPIO_SetBits(GPIOD,GPIO_Pin_12);\
					GPIO_ResetBits(GPIOD,GPIO_Pin_13);
					
/*******************Pitch停止********************/
#define Pitch_Stop	\
			GPIO_ResetBits(GPIOD,GPIO_Pin_12);\
		GPIO_ResetBits(GPIOD,GPIO_Pin_13);
					
#define Air_Cylinder_ALLON	\
					PE13(OFF);\
					PE14(OFF);\
						PE15(OFF)

#define Air_Cylinder_ALLOFF	\
					PE13(ON);\
					PE14(ON);\
           PE15(ON)
					 
#define Beep PEout(2)		

#define LIGHT					0
#define DARK					1

#define YELLOW_LEFT_ON   		PA8(LIGHT);\
								PG7(DARK);\
								PG8(DARK);
								
#define YELLOW_LEFT_OFF 		PA8(DARK);

#define YELLOW_RIGHT_ON 		PG8(LIGHT);\
								PA8(DARK);\
								PG7(DARK);
								
#define YELLOW_RIGHT_OFF 		PG8(DARK);

#define RED_STOP_ON  			PG7(LIGHT);\
								PG8(DARK);\
								PA8(DARK);
								
#define RED_STOP_OFF  			PG7(DARK);

#define ALL_OFF 				PA8(DARK);\
								PG7(DARK);\
								PG8(DARK);

#define LIGHT_ALL				PA8(LIGHT);\
								PG7(LIGHT);\
								PG8(LIGHT);
								
								

						
#define GREEN_ACT_ON  PD3(ON);
#define GREEN_ACT_OFF PD3(OFF);

#define fenwei_ON PD0(ON);
#define fenwei_OFF PD0(OFF)
void LED_Init(void);//初始化	
void Air_Cylinder_Init(void);
void Electric_actuator_Init(void);
void yuyin_Init(void);
void RELAY_Init(void);//继电器初始化
#endif

