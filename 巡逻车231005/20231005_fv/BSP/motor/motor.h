#ifndef __MOTOR_H
#define __MOTOR_H	
#include "sys.h"
/****************README*******************
* @brief	以车体正方向为基准，右侧俩为一组，命名moto1，左侧俩为一组，命名moto2；
*			右轮低电平倒转，左轮高电平倒转，因此一个方向行进时，左右电平一高一低；
*/



void Motor_GPIO_Init(void);
void Motor_PWM_Init(u16 arr,u16 psc);


#define MOTOR1_STOP_Pin         GPIO_Pin_10
#define MOTOR1_DIRECTION_Pin    GPIO_Pin_11
#define MOTOR1__GPIOx           GPIOD

#define MOTOR2_STOP_Pin         GPIO_Pin_7
#define MOTOR2_DIRECTION_Pin    GPIO_Pin_8
#define MOTOR2__GPIOx           GPIOE

#define WHEEL_STOP  GPIO_ResetBits(MOTOR1__GPIOx,MOTOR1_STOP_Pin);\
                    GPIO_ResetBits(MOTOR2__GPIOx,MOTOR2_STOP_Pin)
										
#define WHEEL_NORMAL GPIO_SetBits(MOTOR1__GPIOx,MOTOR1_STOP_Pin);\
                     GPIO_SetBits(MOTOR2__GPIOx,MOTOR2_STOP_Pin)

#define Back_Mode  GPIO_ResetBits(MOTOR1__GPIOx,MOTOR1_DIRECTION_Pin);\
                   GPIO_SetBits(MOTOR2__GPIOx,MOTOR2_DIRECTION_Pin)
										
#define Drive_Mode  GPIO_SetBits(MOTOR1__GPIOx,MOTOR1_DIRECTION_Pin);\
                   GPIO_ResetBits(MOTOR2__GPIOx,MOTOR2_DIRECTION_Pin)
				   
#define RIGHT_FORWARD			GPIO_SetBits(MOTOR1__GPIOx,MOTOR1_DIRECTION_Pin);
#define RIGHT_BACKWARD			GPIO_ResetBits(MOTOR1__GPIOx,MOTOR1_DIRECTION_Pin);
#define LEFT_FORWARD			GPIO_ResetBits(MOTOR2__GPIOx,MOTOR2_DIRECTION_Pin);
#define LEFT_BACKWARD			GPIO_SetBits(MOTOR2__GPIOx,MOTOR2_DIRECTION_Pin);
									 
//#define LEFT_Turn	  GPIO_SetBits(MOTOR1__GPIOx,MOTOR1_DIRECTION_Pin);\
//                     GPIO_SetBits(MOTOR2__GPIOx,MOTOR2_DIRECTION_Pin)
//										 
//#define RIGHT_TURN  GPIO_ResetBits(MOTOR1__GPIOx,MOTOR1_DIRECTION_Pin);\
//                     GPIO_ResetBits(MOTOR2__GPIOx,MOTOR2_DIRECTION_Pin)
                    
									 

#endif

