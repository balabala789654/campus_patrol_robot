#include "stm32f4xx.h"                  // Device header
#include "pid.h"
#include "robot.h"

int main()
{
	robot_init();
	while(1){
		chassis_drive(chassis_cal(robot.controller(&rc_ctrl), &robot), robot.controller(&rc_ctrl), &robot);
	}
}




