#include "robot.h"

_robot robot;
RC_ctrl_t* controller_callback(RC_ctrl_t* _rc);

void robot_init(void){
	usart1_init(115200);
	float pid_params[5] = {
		5.0f, 0.0f, 1.0f, 10000.0f, 500.0f
	};
	robot.controller = controller_callback;

	robot.straight_max = 1000;
	robot.spin_max = 1000;
	
	PID_init(&robot.motor[0].pid_params, 0, pid_params[0], pid_params[1], pid_params[2], pid_params[3], pid_params[4], 500);
	PID_init(&robot.motor[1].pid_params, 0, pid_params[0], pid_params[1], pid_params[2], pid_params[3], pid_params[4], 500);
	PID_init(&robot.motor[2].pid_params, 0, pid_params[0], pid_params[1], pid_params[2], pid_params[3], pid_params[4], 500);
	PID_init(&robot.motor[3].pid_params, 0, pid_params[0], pid_params[1], pid_params[2], pid_params[3], pid_params[4], 500);

}
float* chassis_cal(RC_ctrl_t* _rc, _robot* _bot){
	float x,y=0;
	float vx,vy;
	static float ret[2];
	
	y = _rc->rc.ch[3]*_bot->straight_max/660;
	x = _rc->rc.ch[2]*_bot->straight_max/660;
	vy = y*cos(PI/4)-x*cos(PI/4);
	vx = y*sin(PI/4)+x*sin(PI/4);
	
	ret[0] = vy;
	ret[1] = vx;
	return ret;
}

int chassis_drive(float _input[], RC_ctrl_t* _rc, _robot* _bot){
	float vx,vy;
	vy = _input[0];
	vx = _input[1];
	
	if(_rc->rc.s[0]==2 && _rc->rc.s[1]==2){
		_bot->motor[0].target_speed=0;
		_bot->motor[1].target_speed=0;
		_bot->motor[2].target_speed=0;
		_bot->motor[3].target_speed=0;
	}
	else {
		_bot->motor[0].target_speed=vy	+	_rc->rc.ch[0]*_bot->spin_max/660;
		_bot->motor[2].target_speed=-vy	+	_rc->rc.ch[0]*_bot->spin_max/660;
		_bot->motor[1].target_speed=-vx	+	_rc->rc.ch[0]*_bot->spin_max/660;
		_bot->motor[3].target_speed=vx	+	_rc->rc.ch[0]*_bot->spin_max/660;
	}
	
	PID_calc(&_bot->motor[0].pid_params, _bot->motor[0].feed_back.speed_rpm, _bot->motor[0].target_speed);
	PID_calc(&_bot->motor[1].pid_params, _bot->motor[1].feed_back.speed_rpm, _bot->motor[1].target_speed);
	PID_calc(&_bot->motor[2].pid_params, _bot->motor[2].feed_back.speed_rpm, _bot->motor[2].target_speed);
	PID_calc(&_bot->motor[3].pid_params, _bot->motor[3].feed_back.speed_rpm, _bot->motor[3].target_speed);
	
	motor_send_3508(_bot->motor[3].pid_params.out, _bot->motor[2].pid_params.out, _bot->motor[0].pid_params.out, _bot->motor[1].pid_params.out);
	return 0;
}




RC_ctrl_t* controller_callback(RC_ctrl_t* _rc){
	return _rc;
}





