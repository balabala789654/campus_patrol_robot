
#include "jiguang_task.h"


u8 USART5_TX_BUF[9]={0x62,0x33,0x09,0x00,0x01,0x00,0x00,0x00};
int a=0;
void jiguang_task(void *pvParameters)
{
	u8 t;
	while(1)
	{
		a++;
				USART5_TX_BUF[8]=USART5_TX_BUF[0]^USART5_TX_BUF[1]^USART5_TX_BUF[2]^USART5_TX_BUF[3]^USART5_TX_BUF[4]^USART5_TX_BUF[5]^USART5_TX_BUF[6]^USART5_TX_BUF[7];
					for(t=0;t<9;t++)
			{
				USART_SendData(UART5, USART5_TX_BUF[t]);         //向串口1发送数据
				while(USART_GetFlagStatus(UART5,USART_FLAG_TXE)==RESET);//等待发送结束
			}
			USART5_TX_BUF[4]++;
			if(USART5_TX_BUF[4]==11)
			{
			USART5_TX_BUF[4]=01;
			}
			if(GPIO_ReadInputDataBit(GPIOH, GPIO_Pin_14) == 1)
			{
			GPIO_ResetBits(GPIOE,GPIO_Pin_4);
			}
			if(GPIO_ReadInputDataBit(GPIOH, GPIO_Pin_14) == 0)
			{
			GPIO_SetBits(GPIOE,GPIO_Pin_4);
			}
			if(GPIO_ReadInputDataBit(GPIOH, GPIO_Pin_15) == 0)
			{
			GPIO_ResetBits(GPIOD,GPIO_Pin_0);
			}
			if(GPIO_ReadInputDataBit(GPIOH, GPIO_Pin_15) == 1)
			{
			GPIO_SetBits(GPIOD,GPIO_Pin_0);
			}
//		delay_ms(50);
		vTaskDelay(50);
	}

}

