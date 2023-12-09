#include "CAN_receive.h"
#include "detect_task.h"
/*底盘数据读取*/
#define get_motor_measure(ptr, rx_message)                                              \
{                                                                                       \
    if((ptr)->ecd - (ptr)->last_ecd > 4096) (ptr)->count-- ;                            \
	else if((ptr)->ecd - (ptr)->last_ecd < -4096 ) (ptr)->count ++ ;                    \
    (ptr)->last_ecd = (ptr)->ecd;                                                       \
    (ptr)->ecd = (uint16_t)((rx_message).Data[0] << 8 | (rx_message).Data[1]);          \
    (ptr)->speed_rpm = (uint16_t)((rx_message).Data[2] << 8 |(rx_message).Data[3]);     \
    (ptr)->given_current = (uint16_t)((rx_message).Data[4] << 8 | (rx_message).Data[5]); \
    (ptr)->temperate = (rx_message).Data[6];                                             \
    (ptr)->all_ecd=(ptr)->count*8191+(ptr)->ecd;                                     \
}



//声明电机变量
motor_measure_t  motor_chassis_power[4],motor_chassis_helm[4],motor_clamp;
//can1中断
void CAN1_RX0_IRQHandler(void)
{
		static CanRxMsg CanRxMessage1;
		CAN_Receive(CAN1,CAN_FIFO0,&CanRxMessage1); 
		CAN_ClearFlag(CAN1,CAN_IT_FMP0);
		switch(CanRxMessage1.StdId)
		{
			case CAN_3510_M1_ID :
			case CAN_3510_M2_ID :
			case CAN_3510_M3_ID :
			case CAN_3510_M4_ID :
			{
				static uint8_t i = 0;
				i = CanRxMessage1.StdId - CAN_3510_M1_ID;
				get_motor_measure(&motor_chassis_power[i],CanRxMessage1);
				//记录时间
				DetectHook(ChassisPowerMotor1TOE + i);
				break;
			}
			default:
			{
				break;
			}
		}
}

//can2中断
void CAN2_RX1_IRQHandler(void)
{
		static CanRxMsg CanRxMessage2;
		CAN_Receive(CAN2,CAN_FIFO1,&CanRxMessage2); 
		CAN_ClearFlag(CAN2,CAN_IT_FMP0);
		switch(CanRxMessage2.StdId)
		{
					case CAN_3508_clamp_ID :
				{
				get_motor_measure(&motor_clamp,CanRxMessage2);
				DetectHook(ClampMotorTOE);	
				break;
			
				}
					case CAN_2006_M1_ID :
					case CAN_2006_M2_ID :
					case CAN_2006_M3_ID :
					case CAN_2006_M4_ID :
				{
				static uint8_t i = 0;
				i = CanRxMessage2.StdId - CAN_2006_M1_ID;
				get_motor_measure(&motor_chassis_helm[i],CanRxMessage2);
				DetectHook(ChassisHelmMotor1TOE + i);
				break;
				}
				default:
        {
            break;
        }
		}

}


//返回底盘3508电机变量地址，通过指针方式获取原始数据
const motor_measure_t *get_Chassis_power_Motor_Measure_Point(uint8_t i)
{
    return &motor_chassis_power[(i & 0x03)];
}
//返回底盘2006电机变量地址，通过指针方式获取原始数据
const motor_measure_t *get_Chassis_helm_Motor_Measure_Point(uint8_t i)
{
    return &motor_chassis_helm[(i & 0x03)];
}
//返回夹爪电机变量地址，通过指针方式获取原始数据
const motor_measure_t *get_clamp_Motor_Measure_Point(void)
{
    return &motor_clamp;
}

/***********************************************************************************************/
void CAN_CMD_CHASSIS_POWER(int16_t motor1, int16_t motor2, int16_t motor3, int16_t motor4)//发送数据
{
    CanTxMsg TxMessage;//定义结构体
    TxMessage.StdId = 0x200;
    TxMessage.IDE = CAN_ID_STD;
    TxMessage.RTR = CAN_RTR_DATA;
    TxMessage.DLC = 0x08;
    TxMessage.Data[0] = motor1 >> 8;
    TxMessage.Data[1] = motor1;
    TxMessage.Data[2] = motor2 >> 8;
    TxMessage.Data[3] = motor2;
    TxMessage.Data[4] = motor3 >> 8;
    TxMessage.Data[5] = motor3;
    TxMessage.Data[6] = motor4 >> 8;
    TxMessage.Data[7] = motor4;

    CAN_Transmit(CAN1, &TxMessage);
}


void CAN_CMD_CHASSIS_Helm(int16_t motor1, int16_t motor2, int16_t motor3, int16_t motor4)//发送数据
{
    CanTxMsg TxMessage;//定义结构体
    TxMessage.StdId = 0x1FF;
    TxMessage.IDE = CAN_ID_STD;
    TxMessage.RTR = CAN_RTR_DATA;
    TxMessage.DLC = 0x08;
    TxMessage.Data[0] = motor1 >> 8;
    TxMessage.Data[1] = motor1;
    TxMessage.Data[2] = motor2 >> 8;
    TxMessage.Data[3] = motor2;
    TxMessage.Data[4] = motor3 >> 8;
    TxMessage.Data[5] = motor3;
    TxMessage.Data[6] = motor4 >> 8;
    TxMessage.Data[7] = motor4;

    CAN_Transmit(CAN2, &TxMessage);
}

void CAN_CMD_CHASSIS_Clamp(int16_t motor2)//发送数据
{
    CanTxMsg TxMessage;//定义结构体
    TxMessage.StdId = 0X200;
    TxMessage.IDE = CAN_ID_STD;
    TxMessage.RTR = CAN_RTR_DATA;
    TxMessage.DLC = 0x08;
    TxMessage.Data[0] = 0;
    TxMessage.Data[1] = 0;
    TxMessage.Data[2] = motor2 >> 8;
    TxMessage.Data[3] = motor2;
    TxMessage.Data[4] = 0;
    TxMessage.Data[5] = 0;
    TxMessage.Data[6] = 0;
    TxMessage.Data[7] = 0;

    CAN_Transmit(CAN2, &TxMessage);
}





















