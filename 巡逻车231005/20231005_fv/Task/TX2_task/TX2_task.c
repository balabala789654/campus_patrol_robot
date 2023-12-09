#include "TX2_task.h"

extern QueueHandle_t TxCOM1;
extern QueueHandle_t RxCOM1;
static PC_Ctrl_Union_t PcData;
unsigned char  Forecast_or_not=0;

extern float re_yaw_absolute_angle;
extern float re_pitch_absolute_angle;

Inter_Data_t InterUpdate;
uint32_t TxTaskStack;
uint32_t sendTaskStack;

Send_Tx2_t TX_vision_Mes;

static void UART_PutChar(USART_TypeDef* USARTx, u8 ch);
static void Send_to_PC(USART_TypeDef* USARTx, Send_Tx2_t *SEND_PC);

void Tx2_task(void  *pvParameters)
{
	    DataRevice Buffer;
    while(1)
    {
			xQueueReceive(TxCOM1,&Buffer,portMAX_DELAY);

			memcpy(PcData.PcDataArray,Buffer.buffer,MINIPC_FRAME_LENGTH);
			InterUpdate.now_pc_pitch_ref=PcData.PcDate .angle_pitch ;
			InterUpdate.now_pc_yaw_ref =PcData.PcDate .angle_yaw ;
			if(InterUpdate.last_pc_yaw_ref !=InterUpdate.now_pc_yaw_ref ||InterUpdate.last_pc_pitch_ref !=InterUpdate.now_pc_pitch_ref)
				Forecast_or_not=1;
			InterUpdate.last_pc_yaw_ref=InterUpdate.now_pc_yaw_ref;
			InterUpdate.last_pc_pitch_ref=InterUpdate.now_pc_pitch_ref;
			TxTaskStack = uxTaskGetStackHighWaterMark(NULL);
			
    }



}
	
	
void Interactive_task(void  *pvParameters)
{
        while(1)
    {

//        TX_vision_Mes.yaw = re_yaw_absolute_angle; //获取yaw
//        TX_vision_Mes.pitch = re_pitch_absolute_angle; //获取pitch
        TX_vision_Mes.shoot_speed = 16; //获取速度
        TX_vision_Mes.color = 1;//颜色
				TX_vision_Mes.mode=1;
        Send_to_PC(USART1, &TX_vision_Mes);	
			sendTaskStack = uxTaskGetStackHighWaterMark(NULL);
        vTaskDelay(5);
				
    }
   
}

static void Send_to_PC(USART_TypeDef* USARTx, Send_Tx2_t *TXmessage)
{
    unsigned char crc = 0;
    unsigned char *TX_data;
    TX_data = (unsigned char*)TXmessage;
    crc = get_crc8_check_sum(TX_data, 11, 0xff); //CRC校验
    TXmessage->CRC8 = crc;
    //数据发送
    UART_PutChar(USARTx, 0x5a);//帧头
   
    for (int i = 0; i < 12; i++)
    {
        UART_PutChar(USARTx, *TX_data); //数据 + crc		
        TX_data++;

    }
//   detect_task();
//	UART_PutChar(USARTx,0xa5);//帧尾
}
static void UART_PutChar(USART_TypeDef* USARTx, u8 ch)
{
    USART_SendData(USARTx, (u8)ch);

    while(USART_GetFlagStatus(USARTx, USART_FLAG_TXE) == RESET);
}

