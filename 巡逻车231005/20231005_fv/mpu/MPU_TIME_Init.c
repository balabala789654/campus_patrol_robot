//#include "BSP_TIME_Init.h"
#include "stm32f4xx.h"
#include "MPU_TIME_Init.h"
#include "cmsis_os.h"
uint32_t Systic = 0;
//TIM1->CH1=PA8

//uint32_t Get_Time_Micros(void)
//{
//	static float last;
//	static float now;
//  last = now;
//	now = Get_Sys_Ticks_float_ms();
// 
//  
//	return (uint32_t)((now - last)*1000000);
//}


double Get_Sys_Ticks(void)
{
	double sec;
	double value;
	sec = osKernelSysTick();///1000.0;
	value = sec + ((SysTick->LOAD - SysTick->VAL)*1000/SysTick->LOAD)*1e-3;
//	return Systic;
	return value;
}

//得到微秒级的CPU时间
/*
   return float ms time
*/
float Get_Sys_Ticks_float_ms(void)
{
	return (Systic + (TIM7->CNT)/1000.0);
}


