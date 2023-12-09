#ifndef BIZHANG_H
#define BIZHANG_H
#include "stm32f4xx.h"
#include "bizhang_usart.h"
#include <string.h>


#define RX_BUF_NUM 20u

#define FRAME_LENGTH 9u

typedef __packed struct
{
//	u8 fh;//帧头
	u16 sensor;//数据
//	u8 sum;//校验和
}sensor_Distance_t;

extern sensor_Distance_t sensor_Distance;

extern void bizhang_Usart_init(void);
extern const sensor_Distance_t *get_sensor_Distance_point(void);

#endif

