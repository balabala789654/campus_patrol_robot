#ifndef CHASSISTASK_H
#define CHASSISTASK_H

#include "main.h"


#define S1  1
#define S2  0
#define MAX_SPEED 6.0f//=/1500*9000			3.33f//=/1500*5000			¾É×¢ÊÍ£º2.083f //¿ÕÔØ×ªËÙ 125 rad/min 
#define WHEEL_PERIMETER  400 //ÖÜ³¤
#define CHASSIS_DECELE_RATIO (1.0f/19.0f)
#define Pi  3.14159f
#define down_ratio 17/1 


#define angle_init0  4348
#define angle_init1  5351
#define angle_init2  8168
#define angle_init3  91


/*¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª3508¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª*/
#define M3508_SPEED_PID_KP 0.0f//15000.0f
#define M3508_SPEED_PID_KI 0.0f//10.0f
#define M3508_SPEED_PID_KD 0.0f
#define M3508_SPEED_PID_MAX_OUT MAX_CAN_CURRENT
#define M3508_SPEED_PID_MAX_IOUT 2000.0f
#define MAX_CAN_CURRENT 16000.0f //×î´óµçÁ÷

#define M3508_ANGLE_PID_KP 0.0f//15000.0f//speed_kp40  angle_53
#define M3508_ANGLE_PID_KI 0.0f//10.0f
#define M3508_ANGLE_PID_KD 0.0f
#define M3508_ANGLE_PID_MAX_OUT 1000.0f
#define M3508_ANGLE_PID_MAX_IOUT 50.0f


#define M3508_0_ANGLE_PID_KP 5.0f
#define M3508_0_ANGLE_PID_KI 0.0f
#define M3508_0_ANGLE_PID_KD 0.0f
#define M3508_0_SPEED_PID_KP 10.0f
#define M3508_0_SPEED_PID_KI 0.0f
#define M3508_0_SPEED_PID_KD 0.0f

#define M3508_1_ANGLE_PID_KP 5.0f
#define M3508_1_ANGLE_PID_KI 0.0f
#define M3508_1_ANGLE_PID_KD 0.0f
#define M3508_1_SPEED_PID_KP 10.0f
#define M3508_1_SPEED_PID_KI 0.0f
#define M3508_1_SPEED_PID_KD 3.0f

#define M3508_2_ANGLE_PID_KP 5.0f
#define M3508_2_ANGLE_PID_KI 0.0f
#define M3508_2_ANGLE_PID_KD 0.0f
#define M3508_2_SPEED_PID_KP 10.0f
#define M3508_2_SPEED_PID_KI 0.0f
#define M3508_2_SPEED_PID_KD 0.0f

#define M3508_3_ANGLE_PID_KP 5.0f
#define M3508_3_ANGLE_PID_KI 0.0f
#define M3508_3_ANGLE_PID_KD 0.0f
#define M3508_3_SPEED_PID_KP 10.0f
#define M3508_3_SPEED_PID_KI 0.0f
#define M3508_3_SPEED_PID_KD 0.0f


///*¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª6020¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª*/
//#define M6020_SPEED_PID_KP 15.0f//15000.0f//speed_kp40  angle_53
//#define M6020_SPEED_PID_KI 0.0f//10.0f
//#define M6020_SPEED_PID_KD 0.0f

//#define M6020_ANGLE_PID_KP 15.0f//15000.0f//speed_kp40  angle_53
//#define M6020_ANGLE_PID_KI 0.0f//10.0f
//#define M6020_ANGLE_PID_KD 0.0f

//#define M6020_gospeed_kp  10
//#define M6020_speed_ki  0
//#define M6020_speed_kd  0

//#define M6020_goangle_kp  10
//#define M6020_angle_ki  0
//#define M6020_angle_kd  0

//#define M6020_SPEED_PID_MAX_OUT MAX_CAN_CURRENT
//#define M6020_SPEED_PID_MAX_IOUT 2000.0f
//#define MAX_CAN_CURRENT 16000.0f //×î´óµçÁ÷

#define CHASSIS_6020_intoffect0 0
#define CHASSIS_6020_intoffect1 4000
#define CHASSIS_6020_intoffect2 0
#define CHASSIS_6020_intoffect3 0

#define TURNR45  1024
#define TURNL45  -1024
#define TURNR90  2048
#define TURNL90  -2048

#define MAX_rc 660.f
#define MAX_speed 8000.f






typedef enum
{
	CHASSIS_RELAX   =0,
	CHASSIS_INIT    =1,  
	CHASSIS_FOLLOW      =2,
	CHASSIS_BIZHANG  =3,
	CHASSIS_TURN    =4,
	
} remote_modename_t;

typedef struct
{
	u16 absolute_angle[4];
	int16_t last_absolute_angle[4];
	uint8_t angle_state[4][2];//Êý¾Ý´æ·Å
	
	int count[4];
	int16_t real_absolute_angle[4];
}As5600_State;

typedef struct
{
	
	
	
	/**********3508*******/
	const motor_measure_t *chassis_3508_measure[4];

		
//		pid_type_def M3508_speed1_pid[4];
	
	fp32 speed_set[4];
	fp32 Drive_Moto_Current[4];

	
	
	/********6020*********/
	const motor_measure_t *chassis_6020_measure[4];
//	pid_type_def M6020_speed_pid[4];
//	pid_type_def M6020_angle_pid[4];
  pid_type_def M3508_angle_pid[4];
	pid_type_def M3508_speed_pid[4];
//	pid_type_def M3508_angle_speedpid[4];
	
	pid_type_def AUTO_Bizhang_Xpid;
	pid_type_def AUTO_Bizhang_Zpid;
	
	
	fp32 relative_angle[4];
	fp32 relative_angle_set[4];
	fp32 allecd_angle_set[4];
	fp32 current_angle_set[4];
	fp32 current_set[4];
	fp32 given_current[4];
	
	fp32 CHASSIS_INIT_ANGLE[4];
	fp32 CHASSIS_STOP_ANGLE[4];//Í£Ö¹½Ç¶È£¬·ÀÖ¹Áï³µ
}chissis_Motor_t;

typedef struct{
	float twist_linear;
	float twist_angle;
}_ROS2;

typedef struct
{
	const sensor_Distance_t *distance_measure_t;
  /*******drive_moto************/
  chissis_Motor_t chassis_drive_motor;
	
	fp32  Drive_Moto_SpeedRef[4];
	fp32  Drive_Moto_SpeedSet[4];
	/******3508duo¶æÏòµç»ú************/
  chissis_Motor_t chassis_3508_motor;
	
	/*********¶æÏò½Ç¶È´«¸Ð***************/
	As5600_State as5600_state;
//	const motor_measure_t *motor_6020_measure[4];
	/******remote*********/
	const RC_ctrl_t *chassis_rc_ctrl;
	remote_modename_t remote_modename;
	
	_ROS2 ROS;
} chassis_move_t;


typedef struct
{
	fp32 vx;
	fp32 vy;
	fp32 vw;
	
	int8_t dist[4];
	int32_t SpeedTarget[4];
	int32_t AngleTarget[4];
	int32_t LastAngleTarget[4];
	
}chassis_speed;





#endif
