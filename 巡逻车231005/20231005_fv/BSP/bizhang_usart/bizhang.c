#include "bizhang.h"

#include "detect_task.h"
sensor_Distance_t sensor_Distance;

//接收原始数据，为10个字节，给了20个字节长度，防止DMA传输越界
static uint8_t rx_buf[2][RX_BUF_NUM];

//初始化DMA，串口3
void bizhang_Usart_init(void)
{
    bizhang_Usart(rx_buf[0], rx_buf[1], RX_BUF_NUM);
}

/*******************************************
返回数据变量，通过指针传递方式传递信息
********************************************/
const sensor_Distance_t *get_sensor_Distance_point(void)
{
    return &sensor_Distance;
}

static void Data_settlement(volatile const uint8_t *data_buf, sensor_Distance_t *sensor_Distance)
{
	  if (data_buf == NULL || sensor_Distance == NULL)
    {
        return;
    }
//	  sensor_Distance->fh = data_buf[0];
		sensor_Distance->sensor = (data_buf[5] << 8) + data_buf[6];
//		sensor_Distance->sensor[1] = (data_buf[3] << 8) + data_buf[4];
//		sensor_Distance->sensor[2] = (data_buf[5] << 8) + data_buf[6];
//		sensor_Distance->sensor[3] = (data_buf[7] << 8) + data_buf[8];
//		sensor_Distance->sum = data_buf[9];

}
u8 rx_sum=0;
u16 sensor_Distance1[10];
u8 ii=0;
//串口中断
void UART5_IRQHandler(void)
{
	u8 buffer[20];
		static uint16_t this_time_rx_len = 0;
    if (USART_GetITStatus(UART5, USART_IT_IDLE) != RESET)//空闲中断
    {
        
//        USART_ReceiveData(USART6);
        (void)UART5->SR;
        (void)UART5->DR;
			//					new_time=xTaskGetTickCount();
//					tim_err=new_time-last_tim;
//					last_tim=new_time;
        if(DMA_GetCurrentMemoryTarget(DMA1_Stream0) == 0)
        {
            //重新设置DMA
            DMA_Cmd(DMA1_Stream0, DISABLE);
            this_time_rx_len = RX_BUF_NUM - DMA_GetCurrDataCounter(DMA1_Stream0);
            DMA1_Stream0->NDTR = (uint16_t)RX_BUF_NUM;
            DMA1_Stream0->CR |= (uint32_t)DMA_SxCR_CT;
            //清DMA中断标志
//            DMA_ClearFlag(DMA2_Stream1, DMA_FLAG_TCIF5 | DMA_FLAG_HTIF5);
            DMA_Cmd(DMA1_Stream0, ENABLE);

            if(this_time_rx_len == FRAME_LENGTH)
            {
							DetectHook(DBUSWDG);
							memcpy(buffer, rx_buf[0],RX_BUF_NUM );
							if(buffer[0]==0x62)
							{

									rx_sum=buffer[0]^buffer[1]^buffer[2]^buffer[3]^buffer[4]^buffer[5]^buffer[6]^buffer[7];
								if(buffer[8]==rx_sum)//                //处理数据

                {
									ii=buffer[4]-1;
									sensor_Distance1[ii]=(buffer[5] << 8) + buffer[6];
									DetectHook(TX2DataWGD);
								}
							}

            }
        }
        else
        {
            //重新设置DMA
            DMA_Cmd(DMA1_Stream0, DISABLE);
            this_time_rx_len = RX_BUF_NUM - DMA_GetCurrDataCounter(DMA1_Stream0);
            DMA1_Stream0->NDTR = (uint16_t)RX_BUF_NUM;
            DMA1_Stream0->CR &= ~(uint32_t)(DMA_SxCR_CT);
            //清DMA中断标志
//            DMA_ClearFlag(DMA2_Stream1, DMA_FLAG_TCIF5 | DMA_FLAG_HTIF5);
            DMA_Cmd(DMA1_Stream0, ENABLE);

            if(this_time_rx_len == FRAME_LENGTH)
            {
							
							 memcpy(buffer, rx_buf[1],RX_BUF_NUM );
							if(buffer[0]==0x62)
							{
									rx_sum=buffer[0]^buffer[1]^buffer[2]^buffer[3]^buffer[4]^buffer[5]^buffer[6]^buffer[7];
								if(buffer[8]==rx_sum)//                //处理数据

                {
									ii=buffer[4]-1;
									sensor_Distance1[ii]=(buffer[5] << 8) + buffer[6];
									DetectHook(TX2DataWGD);
								}
							}
						 else
							{
									return;
							}

            }
        }
				USART_ClearITPendingBit(UART5, USART_IT_IDLE);
    }
		 else if (USART_GetITStatus(UART5, USART_IT_ORE_RX) != RESET)//检查指定的USART中断是否发生  接受中断
    {
			  USART_ClearFlag(UART5,USART_FLAG_ORE);
        USART_ReceiveData(UART5);//
			
    }
}

