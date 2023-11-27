#ifndef __CANTASK_H
#define __CANTASK_H

void motor_send_3508(int i1,int i2,int i3,int i4);
void motor_send_6020(int i1,int i2,int i3,int i4);
void CAN1_RX0_IRQnHandler(void);
void CAN2_RX1_IRQnHandler(void);


#endif
