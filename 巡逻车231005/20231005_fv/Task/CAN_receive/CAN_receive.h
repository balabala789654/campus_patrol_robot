#ifndef CANTASK_H
#define CANTASK_H
#include "stm32f4xx.h"
/* 电机 CAN ID */
typedef enum
{
    CAN_CHASSIS_ALL_ID = 0x200,
    CAN_3510_M1_ID = 0x201,
    CAN_3510_M2_ID = 0x202,
    CAN_3510_M3_ID = 0x203,
    CAN_3510_M4_ID = 0x204,

		CAN_2006_M1_ID = 0x205,
    CAN_2006_M2_ID = 0x206,
    CAN_2006_M3_ID = 0x207,
    CAN_2006_M4_ID = 0x208,
	
		CAN_3508_clamp_ID = 0x202,

} can_msg_id_e;

/* 电机数据结构体 */
typedef struct
{
    uint16_t ecd;
    int16_t speed_rpm;
    int16_t given_current;
    int32_t  all_ecd;
    int32_t  count;
    uint8_t temperate;
    int16_t last_ecd;
	  fp32  angle; 
} motor_measure_t;





//发送底盘3508,2006电机控制命令
extern void CAN_CMD_CHASSIS_Helm(int16_t motor1, int16_t motor2, int16_t motor3, int16_t motor4);
extern void CAN_CMD_CHASSIS_POWER(int16_t motor1, int16_t motor2, int16_t motor3, int16_t motor4);
//发送底盘夹爪电机控制命令
extern void CAN_CMD_CHASSIS_Clamp(int16_t motor2);
//返回底盘电机变量地址，通过指针方式获取原始数据,i的范围是0-3，对应0x201-0x204,
extern const motor_measure_t *get_Chassis_power_Motor_Measure_Point(uint8_t i);
extern const motor_measure_t *get_Chassis_helm_Motor_Measure_Point(uint8_t i);
//返回夹爪电机变量地址，通过指针方式获取原始数据
extern const motor_measure_t *get_clamp_Motor_Measure_Point(void);







































#endif


