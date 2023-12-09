#ifndef _TX2_TASK_H_
#define	_TX2_TASK_H_

#include "main.h"

typedef __packed struct
{
    unsigned char head;
    float angle_pitch;
    float angle_yaw;
	  float distance;
    unsigned char CRC8;
} PC_Ctrl_t;


//上位机数据转换共用体
typedef union
{
    PC_Ctrl_t PcDate;
    unsigned char PcDataArray[sizeof(PC_Ctrl_t)];
} PC_Ctrl_Union_t;


typedef struct
{
    float yaw;//陀螺仪yaw的角度
    float pitch;//陀螺仪pitch的角度
    u8 mode;//模式 0：辅助射击 1：敌方小陀螺 2：小风车 3：大风车
    u8 color;//颜色
    u8 shoot_speed;//射速
    unsigned char CRC8;
} Send_Tx2_t;



typedef struct
{

	float now_pc_yaw_ref;
	float now_pc_pitch_ref;
	float last_pc_yaw_ref;
	float last_pc_pitch_ref;
} Inter_Data_t;
extern Inter_Data_t InterUpdate;

extern void Tx2_task(void  *pvParameters);
extern void Interactive_task(void  *pvParameters);





































#endif


