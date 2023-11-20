#ifndef __CAN_H
#define __CAN_H

#include "stm32f4xx.h"                  // Device header
#include "sys.h"
u8 CAN1_Mode_Init(u8 tsjw,u8 tbs2,u8 tbs1,u16 brp,u8 mode);
u8 CAN2_Mode_Init(u8 tsjw,u8 tbs2,u8 tbs1,u16 brp,u8 mode);

typedef struct
{
    uint16_t ecd;
    int16_t speed_rpm;
    int16_t given_current;
    int32_t  all_ecd;
    int32_t  count;

    uint8_t temperate;
    int16_t last_ecd;
	float pid_set_speed;
} motor_measure_t;


#define get_motor_measure(ptr, rx_message)                                              \
{                                                                                       \
    if((ptr)->ecd - (ptr)->last_ecd > 4096) (ptr)->count-- ;                            \
		else if((ptr)->ecd - (ptr)->last_ecd < -4096 ) (ptr)->count ++ ;											\
    (ptr)->last_ecd = (ptr)->ecd;                                                       \
    (ptr)->ecd = (uint16_t)((rx_message).Data[0] << 8 | (rx_message).Data[1]);          \
    (ptr)->speed_rpm = (uint16_t)((rx_message).Data[2] << 8 |(rx_message).Data[3]);     \
    (ptr)->given_current = (uint16_t)((rx_message).Data[4] << 8 | (rx_message).Data[5]); \
    (ptr)->temperate = (rx_message).Data[6];                                             \
    (ptr)->all_ecd=(ptr)->count*8191+(ptr)->ecd;                                     \
}


#endif

