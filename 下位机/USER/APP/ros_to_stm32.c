#include "ros_to_stm32.h"

union FloatToBytes
{
	float value;
	char bytes[4];
};
union FloatToBytes linear;
union FloatToBytes angle;

static char flag=0;
static char i=0;
uint8_t usart6_buffers[10];

void ros_to_stm32_init(void)
{
    usart6_init(115200);
}
void USART6_IRQHandler(void)
{	
	if(USART_GetITStatus(USART6, USART_IT_RXNE) != RESET)  //接收中断(接收到的数据必须是0x0d 0x0a结尾)
	{
		if(USART_ReceiveData(USART6) == frame_head) 
			flag=1;
		else if(USART_ReceiveData(USART6) == frame_end) 
		{
			usart6_buffers[i++] = USART_ReceiveData(USART6);
			flag=0;
		}
		if(flag) 
			usart6_buffers[i++] = USART_ReceiveData(USART6);
//		usart6_buffers[i++] = USART_ReceiveData(USART6);
		if(i == 10) {
			i = 0;
			
			linear.bytes[0]=usart6_buffers[1];
			linear.bytes[1]=usart6_buffers[2];
			linear.bytes[2]=usart6_buffers[3];
			linear.bytes[3]=usart6_buffers[4];

			angle.bytes[0]=usart6_buffers[5];
			angle.bytes[1]=usart6_buffers[6];
			angle.bytes[2]=usart6_buffers[7];
			angle.bytes[3]=usart6_buffers[8];
			
			robot.twist_linear = linear.value;
			robot.twist_angle = angle.value;
		}
		USART_ClearFlag(USART6, USART_FLAG_RXNE);
	}
	else if(USART_GetITStatus(USART6, USART_IT_IDLE) != RESET){
		USART_ReceiveData(USART6);	 
		USART_ClearFlag(USART6, USART_FLAG_IDLE);
	}
}


