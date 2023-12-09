#include "main.h"
int16_t PWM_N=0;
int16_t work=0;
float PWM_pulse=0;
extern u32 time4_count;
extern chassis_move_t chassis_move;
static void Clamp(chassis_move_t *clamp_move_Down,float Location);
void clamp_shook_task(void *pvParameters)
{
	
	while(1)
	{
				if(chassis_move.chassis_rc_ctrl->rc.ch[4]>540&&chassis_move.chassis_rc_ctrl->rc.s[0]==RC_SW_UP)
				{
					Pushrod_Up
//					LED4(ON)
					vTaskDelay(500);
//					LED4(OFF)
					Pushrod_Down
				}
				else if((chassis_move.chassis_rc_ctrl->rc.ch[4]>540||chassis_move.chassis_rc_ctrl->rc.ch[4]<-540)&&chassis_move.chassis_rc_ctrl->rc.s[0]==RC_SW_MID)
				{
						if(chassis_move.chassis_rc_ctrl->rc.ch[4]>540)PWM_N++;
						else if(rc_ctrl.rc.ch[4]<-540)PWM_N--;
						else;
						if(PWM_N>=2)PWM_N=2;
						if(PWM_N<0)PWM_N=0;
				}
				else ;
				if(chassis_move.chassis_rc_ctrl->rc.s[1]==RC_SW_UP&&(work==0||work==2))
				{
//					LED1(OFF)
				clamp_ref=-530;
//				Clamp(&chassis_move,-360);
				vTaskDelay(100);
				Clamp_Up
				vTaskDelay(300);
				clamp_ref=-2566;
//				Clamp(&chassis_move,-2566);
				vTaskDelay(500);
					Lifting_Up
				vTaskDelay(300);
				clamp_ref=-3350;
//				Clamp(&chassis_move,-3350);
				vTaskDelay(500);
					work=1;
				}
				else if(rc_ctrl.rc.s[1]==RC_SW_MID&&(work==1||work==0))
				{
//					LED1(ON)
					Clamp_Down
					vTaskDelay(500);
					clamp_ref=-260;
//					Clamp(&chassis_move,-360);
					vTaskDelay(500);
					Lifting_Down
					work=2;
				}	
				else;		
				PWM_pulse=PWM_N*475;
				if(PWM_pulse>1100)PWM_pulse=1000;
				TIM_SetCompare1(TIM5,1000+PWM_pulse);
				TIM_SetCompare2(TIM5,1000+PWM_pulse);
//				LED8(ON)
				vTaskDelay(100);
//				LED8(OFF)
//				vTaskDelay(100);
	
	
	
	}


}

static void Clamp(chassis_move_t *clamp_move_Down,float Location)
{
	TIM_Cmd(TIM4,ENABLE); 
while(time4_count < 200)
	{
	if((clamp_move_Down->clamp_refs < Location))
	{
			clamp_move_Down->clamp_refs = RAMP_float(Location, clamp_move_Down->clamp_refs, 5);
	}
	
	else if((clamp_move_Down->clamp_refs > Location))
	{

	clamp_move_Down->clamp_refs = RAMP_float(Location, clamp_move_Down->clamp_refs, 5);
	}
	else
		break;
	}
		if(time4_count >= 200)
	{
	clamp_move_Down->clamp_refs = clamp_move_Down->motor_clamp_chassis.chassis_motor_measure->all_ecd/8192.0f*360;
	}

	TIM_Cmd(TIM4,DISABLE);
	time4_count = 0;

}




