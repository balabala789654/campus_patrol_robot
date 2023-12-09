#include "chassis_task.h"


#define DELAY_TIME  100

chassis_move_t chassis_move;
chassis_speed absolute_chassis_speed;
int16_t Can_set_Current[4] ,Can_set_current3508[4];

int16_t chassis_3508_setspeed[4];  //目标转速
fp32 chassis_6020_setangle[4];     //目标角度

const int16_t	INIT_ECODE[4]={175, 2926, 229, 1905};
const fp32		RAD_TO_ECD	= 651.899f;		//4096/2Pi

static void chassis_init(chassis_move_t *chassis_init);
static void chassis_mode_set(chassis_move_t *chassis_mode_set);
static void chassis_Behaviour(chassis_move_t *chassis_behaviour);
static void chassis_DataBack(chassis_move_t *chassis_databack);
inline fp32 HighPass(fp32 num, fp32 min);
void AS5600_Read_12(u16 *back_angle , u8 reg_add ,uint8_t angle_state[][2]);
void AS5600_angle_calculate(chassis_move_t *angle_cale);
void MoveInfo(chassis_speed* move_t);
void ControlInfo(chassis_speed* move_t, chassis_move_t *DriveSst);
void DataOutput(chassis_speed* move_t, chassis_move_t *DriveSst);
void LightControl(chassis_speed* move_t);

void chassis_task(void *pvParameters)
{
	
	chassis_init(&chassis_move);
	chassis_DataBack(&chassis_move); //数据处理放在init之前
	vTaskDelay(DELAY_TIME);
	chassis_init(&chassis_move);
	
	GREEN_ACT_ON;
	while(1)
	{
		chassis_DataBack(&chassis_move);						//AS5600数据回传
		chassis_mode_set(&chassis_move);						//模式选择
		chassis_Behaviour(&chassis_move);						//遥控器数据处理
		MoveInfo(&absolute_chassis_speed);						//运动解算
		ControlInfo(&absolute_chassis_speed, &chassis_move);	//控制解算
		DataOutput(&absolute_chassis_speed, &chassis_move);		//解算输出
		LightControl(&absolute_chassis_speed);					//灯光控制
		
		if(chassis_move.remote_modename == CHASSIS_RELAX)		//额外的失能保护
		{
			WHEEL_STOP;
			RED_STOP_ON;
			TIM_SetCompare3(TIM3, 0);
			TIM_SetCompare4(TIM3, 0);
			
			CAN_CMD_CHASSIS_POWER(0,0,0,0); 
		}
		vTaskDelay(1);
	}
	
	
}

static void chassis_init(chassis_move_t *chassis_init)
{
	if(chassis_init == NULL)
	{
		return;
	}
	
	const static fp32 M3508_0_speed_pid[4] = {M3508_0_SPEED_PID_KP, M3508_0_SPEED_PID_KI, M3508_0_SPEED_PID_KD};
	const static fp32 M3508_0_angle_pid[4] = {M3508_0_ANGLE_PID_KP, M3508_0_ANGLE_PID_KI, M3508_0_ANGLE_PID_KD};
	
		const static fp32 M3508_1_speed_pid[4] = {M3508_1_SPEED_PID_KP, M3508_1_SPEED_PID_KI, M3508_1_SPEED_PID_KD};
		const static fp32 M3508_1_angle_pid[4] = {M3508_1_ANGLE_PID_KP, M3508_1_ANGLE_PID_KI, M3508_1_ANGLE_PID_KD};

		const static fp32 M3508_2_speed_pid[4] = {M3508_2_SPEED_PID_KP, M3508_2_SPEED_PID_KI, M3508_2_SPEED_PID_KD};
		const static fp32 M3508_2_angle_pid[4] = {M3508_2_ANGLE_PID_KP, M3508_2_ANGLE_PID_KI, M3508_2_ANGLE_PID_KD};
		
		const static fp32 M3508_3_speed_pid[4] = {M3508_3_SPEED_PID_KP, M3508_3_SPEED_PID_KI, M3508_3_SPEED_PID_KD};
		const static fp32 M3508_3_angle_pid[4] = {M3508_3_ANGLE_PID_KP, M3508_3_ANGLE_PID_KI, M3508_3_ANGLE_PID_KD};
	
//舵向电机pid初始化
	
		PID_init(&chassis_init->chassis_3508_motor.M3508_angle_pid[0],PID_POSITION,M3508_0_angle_pid,M3508_ANGLE_PID_MAX_OUT,M3508_ANGLE_PID_MAX_IOUT);		
		PID_init(&chassis_init->chassis_3508_motor.M3508_speed_pid[0],PID_POSITION,M3508_0_speed_pid,M3508_SPEED_PID_MAX_OUT,M3508_SPEED_PID_MAX_IOUT);
	
		PID_init(&chassis_init->chassis_3508_motor.M3508_angle_pid[1],PID_POSITION,M3508_1_angle_pid,M3508_ANGLE_PID_MAX_OUT,M3508_ANGLE_PID_MAX_IOUT);		
		PID_init(&chassis_init->chassis_3508_motor.M3508_speed_pid[1],PID_POSITION,M3508_1_speed_pid,M3508_SPEED_PID_MAX_OUT,M3508_SPEED_PID_MAX_IOUT);
	
		PID_init(&chassis_init->chassis_3508_motor.M3508_angle_pid[2],PID_POSITION,M3508_2_angle_pid,M3508_ANGLE_PID_MAX_OUT,M3508_ANGLE_PID_MAX_IOUT);		
		PID_init(&chassis_init->chassis_3508_motor.M3508_speed_pid[2],PID_POSITION,M3508_2_speed_pid,M3508_SPEED_PID_MAX_OUT,M3508_SPEED_PID_MAX_IOUT);

		PID_init(&chassis_init->chassis_3508_motor.M3508_angle_pid[3],PID_POSITION,M3508_3_angle_pid,M3508_ANGLE_PID_MAX_OUT,M3508_ANGLE_PID_MAX_IOUT);		
		PID_init(&chassis_init->chassis_3508_motor.M3508_speed_pid[3],PID_POSITION,M3508_3_speed_pid,M3508_SPEED_PID_MAX_OUT,M3508_SPEED_PID_MAX_IOUT);

		for(int i=0;i<4;i++)
		{
			chassis_init->chassis_3508_motor.chassis_3508_measure[i] = get_Chassis_power_Motor_Measure_Point(i);
		}
		chassis_init->distance_measure_t = get_sensor_Distance_point();
	
	chassis_init->chassis_rc_ctrl = get_remote_control_point();
	chassis_init->chassis_3508_motor.CHASSIS_INIT_ANGLE[0] = 270;//131;//200
	chassis_init->chassis_3508_motor.CHASSIS_INIT_ANGLE[1] = -230;//-199;//-270
	chassis_init->chassis_3508_motor.CHASSIS_INIT_ANGLE[2] = 1915;//298;//262
	chassis_init->chassis_3508_motor.CHASSIS_INIT_ANGLE[3] = 259;//1937;//1904
	
	chassis_init->chassis_3508_motor.CHASSIS_STOP_ANGLE[0] = chassis_init->chassis_3508_motor.chassis_3508_measure[0]->all_ecd;
	chassis_init->chassis_3508_motor.CHASSIS_STOP_ANGLE[1] = chassis_init->chassis_3508_motor.chassis_3508_measure[1]->all_ecd;
	chassis_init->chassis_3508_motor.CHASSIS_STOP_ANGLE[2] = chassis_init->chassis_3508_motor.chassis_3508_measure[2]->all_ecd;
	chassis_init->chassis_3508_motor.CHASSIS_STOP_ANGLE[3] = chassis_init->chassis_3508_motor.chassis_3508_measure[3]->all_ecd;

}

static void chassis_mode_set(chassis_move_t *chassis_mode_set)
{
	if(chassis_mode_set == NULL)
	{
		return;
	}
	
	if(switch_is_down(chassis_mode_set->chassis_rc_ctrl->rc.s[S1]) && switch_is_down(chassis_mode_set->chassis_rc_ctrl->rc.s[S2]))
	{
		chassis_mode_set->remote_modename = CHASSIS_RELAX;
	}
	
	else if(switch_is_down(chassis_mode_set->chassis_rc_ctrl->rc.s[S1]) && switch_is_mid(chassis_mode_set->chassis_rc_ctrl->rc.s[S2]))
	{
		chassis_mode_set->remote_modename = CHASSIS_INIT;
	}
	else if(switch_is_mid(chassis_mode_set->chassis_rc_ctrl->rc.s[S1]) && switch_is_mid(chassis_mode_set->chassis_rc_ctrl->rc.s[S2]))
	{
		chassis_mode_set->remote_modename = CHASSIS_FOLLOW;
	}
	 
	else if(switch_is_up(chassis_mode_set->chassis_rc_ctrl->rc.s[S1] && switch_is_up(chassis_mode_set->chassis_rc_ctrl->rc.s[S2])))  
	{
		// 此模式为 导航模式
		chassis_mode_set->remote_modename = CHASSIS_BIZHANG;
	}
	else
	{
		chassis_mode_set->remote_modename = CHASSIS_RELAX;
	}
	
}


fp32 HighPass(fp32 num, fp32 min)
{
	if(num > 0 && fabs(num) < fabs(min))
	{
		return fabs(min);
	}
	else if(num < 0 && fabs(num) < fabs(min))
	{
		return -fabs(min);
	}
	else
		return num;
}

static void chassis_Behaviour(chassis_move_t *chassis_behaviour)
{
	if(chassis_behaviour == NULL)
	{
		return;
	}
	
	if(chassis_behaviour->remote_modename == CHASSIS_RELAX)
	{

		absolute_chassis_speed.vx = 0;
		absolute_chassis_speed.vy = 0;
		absolute_chassis_speed.vw = 0;
		
		chassis_behaviour->chassis_3508_motor.given_current[0] = 0.0f;
		chassis_behaviour->chassis_3508_motor.given_current[1] = 0.0f;
		chassis_behaviour->chassis_3508_motor.given_current[2] = 0.0f;
		chassis_behaviour->chassis_3508_motor.given_current[3] = 0.0f;


	}
	else if(chassis_behaviour->remote_modename == CHASSIS_INIT)
	{
		absolute_chassis_speed.vx = 0;
		absolute_chassis_speed.vy = 0;
		absolute_chassis_speed.vw = 0;
		
		chassis_behaviour->chassis_3508_motor.given_current[0] = 0.0f;
		chassis_behaviour->chassis_3508_motor.given_current[1] = 0.0f;
		chassis_behaviour->chassis_3508_motor.given_current[2] = 0.0f;
		chassis_behaviour->chassis_3508_motor.given_current[3] = 0.0f; 
	}
	 if(chassis_behaviour->remote_modename == CHASSIS_FOLLOW) //跟随
	{
		
		
		absolute_chassis_speed.vw = (fp32)chassis_behaviour->chassis_rc_ctrl->rc.ch[2]*0.5f;//*MAX_SPEED/660/500;
		absolute_chassis_speed.vx = (fp32)chassis_behaviour->chassis_rc_ctrl->rc.ch[1];	//这里可以决定旋转方向
		absolute_chassis_speed.vy = (fp32)chassis_behaviour->chassis_rc_ctrl->rc.ch[0];//*MAX_SPEED/660;
		
		absolute_chassis_speed.vx = HighPass(absolute_chassis_speed.vx, 30);
		absolute_chassis_speed.vy = HighPass(absolute_chassis_speed.vy, 30);
		absolute_chassis_speed.vw = HighPass(absolute_chassis_speed.vw, 30);
	}
	
	else if(chassis_behaviour->remote_modename == CHASSIS_BIZHANG)  //导航
	{
		chassis_behaviour->ROS.twist_linear = ros_to_stm32_output()[0];
		chassis_behaviour->ROS.twist_angle = ros_to_stm32_output()[1];
		
		absolute_chassis_speed.vx = (fp32)chassis_behaviour->ROS.twist_linear*660/1000;
		absolute_chassis_speed.vy = 0;
		absolute_chassis_speed.vw = (fp32)chassis_behaviour->ROS.twist_angle*660/1000;	
	}
	else
	{
		
	}
}


static void chassis_DataBack(chassis_move_t *chassis_databack)
{
	//读取5600的原始数据
	AS5600_Read_12(chassis_databack->as5600_state.absolute_angle,0x0E,chassis_databack->as5600_state.angle_state);
	AS5600_angle_calculate(chassis_databack);

}



//运动解算，如：姿态、方向、速度
void MoveInfo(chassis_speed* move_t)
{
	u8 i=0;
	const fp32 cosAngle = 0.5813f;
	const fp32 sinAngle = 0.8137f;
	
	fp32 real_x[4]={0}, real_y[4]={0};
	real_x[0] = move_t->vx - move_t->vw*cosAngle;		real_y[0] = move_t->vy + move_t->vw*sinAngle;
	real_x[1] = move_t->vx - move_t->vw*cosAngle;		real_y[1] = move_t->vy - move_t->vw*sinAngle;
	real_x[2] = move_t->vx + move_t->vw*cosAngle;		real_y[2] = move_t->vy - move_t->vw*sinAngle;
	real_x[3] = move_t->vx + move_t->vw*cosAngle;		real_y[3] = move_t->vy + move_t->vw*sinAngle;
	
	for(i=0;i<4;i++)
	{
		//正反转，令轮组保持在±90°范围内
		if(real_x[i] < 0)
		{
			move_t->dist[i] = -1;
			real_x[i] = -real_x[i];
			real_y[i] = -real_y[i];
		}
		else if(real_x[i] > 0 || real_y[i] != 0)
			move_t->dist[i] = 1;
		else
			move_t->dist[i] = 0;
		
		//速度
		move_t->SpeedTarget[i] = sqrt(real_x[i] * real_x[i] + real_y[i] * real_y[i]);
		
		//舵向
		if(real_x[i] == 0 && real_y[i] == 0)
			move_t->AngleTarget[i] = move_t->LastAngleTarget[i];
		else
		{
			move_t->AngleTarget[i] = (int32_t)(atan2(real_y[i], real_x[i]) * RAD_TO_ECD);
			move_t->LastAngleTarget[i] = (int32_t)(atan2(real_y[i], real_x[i]) * RAD_TO_ECD);
		}
	}
}

//进行控制解算，如：PID，量纲转化
void ControlInfo(chassis_speed* move_t, chassis_move_t *DriveSst)
{
	for(uint8_t i=0;i<4;i++)
	{
		chassis_move.chassis_3508_motor.speed_set[i] = -PID_calc(	&chassis_move.chassis_3508_motor.M3508_angle_pid[i], 
															chassis_move.as5600_state.real_absolute_angle[i], 
															move_t->AngleTarget[i]);	
		
		chassis_move.chassis_3508_motor.given_current[i] = PID_calc(	&chassis_move.chassis_3508_motor.M3508_speed_pid[i], 
															chassis_move.chassis_3508_motor.chassis_3508_measure[i]->speed_rpm, 
															chassis_move.chassis_3508_motor.speed_set[i]);
		
		//速度设定值转化为PWM占空比输出
		DriveSst->chassis_drive_motor.Drive_Moto_Current[i] = move_t->SpeedTarget[i] *MAX_SPEED;
	}
}


void DataOutput(chassis_speed* move_t, chassis_move_t *DriveSst)
{
	if(move_t->vx == 0 && move_t->vy == 0)
	{
		WHEEL_STOP;
		RED_STOP_ON;
		TIM_SetCompare3(TIM3, 0);
		TIM_SetCompare4(TIM3, 0);
		CAN_CMD_CHASSIS_POWER(0,0,0,0); 
	}
	
	if(chassis_move.remote_modename == CHASSIS_FOLLOW)  //跟随
	{
		WHEEL_NORMAL;
		if(move_t->dist[0] == 1)
		{
			RIGHT_FORWARD;
		}
		else if(move_t->dist[0] == -1)
		{
			RIGHT_BACKWARD;
		}
		else
		{
			WHEEL_STOP;
		}
		
		if(move_t->dist[3] == 1)
		{
			LEFT_FORWARD;
		}
		else if(move_t->dist[3] == -1)
		{
			LEFT_BACKWARD;
		}
		else
		{
			WHEEL_STOP;
		}
		
		TIM_SetCompare3(TIM3,chassis_move.chassis_drive_motor.Drive_Moto_Current[0]);
		TIM_SetCompare4(TIM3,chassis_move.chassis_drive_motor.Drive_Moto_Current[1]);
		
		CAN_CMD_CHASSIS_POWER(	chassis_move.chassis_3508_motor.given_current[0],
								chassis_move.chassis_3508_motor.given_current[1],
								chassis_move.chassis_3508_motor.given_current[2],
								chassis_move.chassis_3508_motor.given_current[3]); 
	}
	else if(chassis_move.remote_modename == CHASSIS_INIT)
	{
		WHEEL_STOP;
		TIM_SetCompare3(TIM3, 0);
		TIM_SetCompare4(TIM3, 0);
		
		CAN_CMD_CHASSIS_POWER(	chassis_move.chassis_3508_motor.given_current[0],
								chassis_move.chassis_3508_motor.given_current[1],
								chassis_move.chassis_3508_motor.given_current[2],
								chassis_move.chassis_3508_motor.given_current[3]); 
	}
	else
	{
		WHEEL_STOP;
		RED_STOP_ON;
		TIM_SetCompare3(TIM3, 0);
		TIM_SetCompare4(TIM3, 0);
		
		CAN_CMD_CHASSIS_POWER(0,0,0,0); 
	}
}


void LightControl(chassis_speed* move_t)
{
	if(move_t->vx > 100)
	{
		ALL_OFF;
	}
	else
	{
		RED_STOP_ON;
	}
	
	if(move_t->vy > 100 || move_t->vw > 50 ) 
	{
		YELLOW_RIGHT_ON;
	}
	else
	{
		YELLOW_RIGHT_OFF;
	}
	
	if(move_t->vy < -100 || move_t->vw < -50)
	{
		YELLOW_LEFT_ON;
	}
	else
	{
		YELLOW_LEFT_OFF;
	}
	
	if(move_t->vx < -100)
	{
		LIGHT_ALL;
	}
	
}

	
void AS5600_Read_12(u16 *back_angle , u8 reg_add ,uint8_t angle_state[][2])
{
	uint16_t temp = 0;
	
	A0_i2cRead_2(0x36,reg_add,angle_state[0]);
	A1_i2cRead_2(0x36,reg_add,angle_state[1]);
	A2_i2cRead_2(0x36,reg_add,angle_state[2]);
	A3_i2cRead_2(0x36,reg_add,angle_state[3]);
	for(int i=0;i<4;i++)
	{
		back_angle[i]  =  angle_state[i][0]<<8|angle_state[i][1];
	}
	//这俩编码器和电机串号了
	temp = back_angle[2];
	back_angle[2] = back_angle[3];
	back_angle[3] = temp;
}


void AS5600_angle_calculate(chassis_move_t *angle_cale)
{
	for(int i=0;i<4;i++)
	{
		if(angle_cale->as5600_state.absolute_angle[i] - INIT_ECODE[i] > 2048)
			angle_cale->as5600_state.real_absolute_angle[i] = angle_cale->as5600_state.absolute_angle[i] -4096 - INIT_ECODE[i];
		else if(angle_cale->as5600_state.absolute_angle[i] - INIT_ECODE[i] < -2048)
			angle_cale->as5600_state.real_absolute_angle[i] = angle_cale->as5600_state.absolute_angle[i] +4096 - INIT_ECODE[i];
		else
			angle_cale->as5600_state.real_absolute_angle[i] = angle_cale->as5600_state.absolute_angle[i] - INIT_ECODE[i];
	}
}




