#include "stm32f4xx.h"                  // Device header
#include "pid.h"
#include "robot.h"
#include "delay.h"
int count;
int main()
{	
	delay_init(168);
	robot_init();
	while(1){
		chassis_control(robot.controller(&rc_ctrl));
		delay_ms(1);
	}
}




