#ifndef BIZHANG_H
#define BIZHANG_H
#include "stm32f4xx.h"
#include "bizhang_usart.h"
#include <string.h>


#define RX_BUF_NUM 20u

#define FRAME_LENGTH 9u

typedef __packed struct
{
//	u8 fh;//֡ͷ
	u16 sensor;//����
//	u8 sum;//У���
}sensor_Distance_t;

extern sensor_Distance_t sensor_Distance;

extern void bizhang_Usart_init(void);
extern const sensor_Distance_t *get_sensor_Distance_point(void);

#endif

