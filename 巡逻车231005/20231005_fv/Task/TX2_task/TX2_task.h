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


//��λ������ת��������
typedef union
{
    PC_Ctrl_t PcDate;
    unsigned char PcDataArray[sizeof(PC_Ctrl_t)];
} PC_Ctrl_Union_t;


typedef struct
{
    float yaw;//������yaw�ĽǶ�
    float pitch;//������pitch�ĽǶ�
    u8 mode;//ģʽ 0��������� 1���з�С���� 2��С�糵 3����糵
    u8 color;//��ɫ
    u8 shoot_speed;//����
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


