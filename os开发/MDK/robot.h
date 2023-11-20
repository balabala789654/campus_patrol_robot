#ifndef _ROBOT_H
#define _ROBOT_H

#include "pid.h"
#include "can.h"
#include "Remote_Control.h"
#include "math.h"
#include "canrecive.h"
#include "usart.h"

#define PI 3.14
typedef struct{
	PidType pid_params;
	motor_measure_t feed_back;
	float target_speed;
	void (*init)(void);
}_motor;
	
typedef struct{
	_motor motor[4];
	RC_ctrl_t* (*controller)(RC_ctrl_t* _rc);
	int straight_max;
	int spin_max;
	
}_robot;

extern _robot robot;

void robot_init(void);
int chassis_drive(float _input[], RC_ctrl_t* _rc, _robot* _bot);
float* chassis_cal(RC_ctrl_t* _rc, _robot* _bot);
#endif







