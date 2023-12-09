#ifndef __ENCODER_H
#define __ENCODER_H
#include "sys.h"

void TIM14_PWM_Init(u32 arr,u32 psc);
void TIM5_CH1_Cap_Init(u32 arr,u16 psc);
void TIM1_CH4_Cap_Init(u32 arr,u16 psc);
void get_distance(void);

#endif























