#include "sys.h"
#include "usart.h"	
////////////////////////////////////////////////////////////////////////////////// 	 
//如果使用ucos,则包括下面的头文件即可.
#if SYSTEM_SUPPORT_OS
		#include "FreeRTOS.h"	//FreeRTOS使用
		#include "queue.h"				  
#endif
#include "string.h"
#include "protocol.h"
#include "gpio.h"
#include "detect_task.h"
 QueueHandle_t TxCOM1;
 QueueHandle_t RxCOM1;

int c1=0,a1=0,c2=0,a2=0;
#if 1
#pragma import(__use_no_semihosting)             
//标准库需要的支持函数                 
struct __FILE 
{ 
	int handle; 
}; 

FILE __stdout;       
//定义_sys_exit()以避免使用半主机模式    
void _sys_exit(int x) 
{ 
	x = x; 
} 
//重定义fputc函数 
int fputc(int ch, FILE *f)
{ 	
	while((USART1->SR&0X40)==0);//循环发送,直到发送完毕   
	USART1->DR = (u8) ch;      
	return ch;
}
#endif
 

static uint8_t _USART1_DMA_RX_BUF[2][BSP_USART1_DMA_RX_BUF_LEN];
unsigned char fff[10];
//初始化IO 串口1 
//bound:波特率
void Device_Usart1_ENABLE_Init(u32 bound)
	{
		//GPIO端口设置
		GPIO_InitTypeDef GPIO_InitStructure;
		USART_InitTypeDef USART_InitStructure;
		USART_ClockInitTypeDef USART_ClockInitStruct;
		NVIC_InitTypeDef NVIC_InitStructure;
		DMA_InitTypeDef DMA_InitStructure;
		
		USART_StructInit(&USART_InitStructure);
    USART_ClockStructInit(&USART_ClockInitStruct);
		
		{
					RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA,ENABLE); //使能GPIOA时钟
					RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1,ENABLE);//使能USART1时钟
					RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_DMA2, ENABLE);
				 
					//串口1对应引脚复用映射
					GPIO_PinAFConfig(GPIOA,GPIO_PinSource9,GPIO_AF_USART1); //GPIOA9复用为USART1
					GPIO_PinAFConfig(GPIOA,GPIO_PinSource10,GPIO_AF_USART1); //GPIOA10复用为USART1
					
					//USART1端口配置
					GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9 | GPIO_Pin_10; //GPIOA9与GPIOA10
					GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;//复用功能
					GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;	//速度50MHz
					GPIO_InitStructure.GPIO_OType = GPIO_OType_PP; //推挽复用输出
					GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP; //上拉
					GPIO_Init(GPIOA,&GPIO_InitStructure); //初始化PA9，PA10

					 //USART1 初始化设置
					USART_InitStructure.USART_BaudRate = bound;//波特率设置
					USART_InitStructure.USART_WordLength = USART_WordLength_8b;//字长为8位数据格式
					USART_InitStructure.USART_StopBits = USART_StopBits_1;//一个停止位
					USART_InitStructure.USART_Parity = USART_Parity_No;//无奇偶校验位
					USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//无硬件数据流控制
					USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;	//收发模式
					USART_Init(USART1, &USART_InitStructure);//初始化串口
					
					
		}		
		{
					RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_DMA2, ENABLE);
					USART_DMACmd(USART1, USART_DMAReq_Rx, ENABLE);
					USART_DMACmd(USART1, USART_DMAReq_Tx, ENABLE);					
					//串口1接收DMA设置
					DMA_DeInit(DMA2_Stream2);
					DMA_StructInit(&DMA_InitStructure);
					DMA_InitStructure.DMA_Channel = DMA_Channel_4;
					DMA_InitStructure.DMA_PeripheralBaseAddr = (uint32_t) & (USART1->DR);
					DMA_InitStructure.DMA_Memory0BaseAddr = (uint32_t)& _USART1_DMA_RX_BUF[0][0];
					DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralToMemory;
					DMA_InitStructure.DMA_BufferSize = BSP_USART1_DMA_RX_BUF_LEN;
			    DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
					DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;
					DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;
					DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte;
		      DMA_InitStructure.DMA_Mode = DMA_Mode_Normal;
					DMA_InitStructure.DMA_Priority = DMA_Priority_Medium;
					DMA_InitStructure.DMA_FIFOMode = DMA_FIFOMode_Disable;
					DMA_InitStructure.DMA_FIFOThreshold = DMA_FIFOThreshold_1QuarterFull;
					DMA_InitStructure.DMA_MemoryBurst = DMA_MemoryBurst_Single;
					DMA_InitStructure.DMA_PeripheralBurst = DMA_PeripheralBurst_Single;
					DMA_Init(DMA2_Stream2, &DMA_InitStructure);
					//串口1发送DMA设置
					DMA_DeInit(DMA2_Stream7);
					DMA_StructInit(&DMA_InitStructure);
					DMA_InitStructure.DMA_Channel = DMA_Channel_4;				
					DMA_InitStructure.DMA_PeripheralBaseAddr = (uint32_t)(&USART1->DR);		
					DMA_InitStructure.DMA_Memory0BaseAddr = (uint32_t)fff;//(uint32_t)(Pc_Send_Data.PcDataArray);
					DMA_InitStructure.DMA_DIR = DMA_DIR_MemoryToPeripheral;
					DMA_InitStructure.DMA_BufferSize = 1;//TO_PC_LENGTH;
					DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
					DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;
					DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;
					DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte;
					DMA_InitStructure.DMA_Mode = DMA_Mode_Circular;
					DMA_InitStructure.DMA_Priority = DMA_Priority_High;
					DMA_InitStructure.DMA_FIFOMode = DMA_FIFOMode_Disable;
					DMA_InitStructure.DMA_FIFOThreshold = DMA_FIFOThreshold_1QuarterFull;
					DMA_InitStructure.DMA_MemoryBurst = DMA_MemoryBurst_Single;
					DMA_InitStructure.DMA_PeripheralBurst = DMA_PeripheralBurst_Single;
					DMA_Init(DMA2_Stream7, &DMA_InitStructure);
					
					
					//配置Memory1,Memory0是第一个使用的Memory
					DMA_DoubleBufferModeConfig(DMA2_Stream2, (uint32_t)&_USART1_DMA_RX_BUF[1][0], DMA_Memory_0);   //first used memory configuration
					DMA_DoubleBufferModeCmd(DMA2_Stream2, ENABLE);
					
					
					
					USART_ITConfig(USART1, USART_IT_IDLE, ENABLE);        //usart IDLE line interrupt  enabled
					USART_ClockInitStruct.USART_Clock = USART_Clock_Disable;  //时钟低电平活动
					USART_ClockInitStruct.USART_CPOL = USART_CPOL_Low;  //SLCK引脚上时钟输出的极性->低电平
					USART_ClockInitStruct.USART_CPHA = USART_CPHA_2Edge;  //时钟第二个边沿进行数据捕获
					USART_ClockInitStruct.USART_LastBit = USART_LastBit_Disable; //最后一位数据的时钟脉冲不从SCLK输出
					USART_ClockInit(USART1, &USART_ClockInitStruct);					
		}			

					//Usart1 NVIC 配置
					NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;//串口1中断通道
					NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=8;//抢占优先级3
					NVIC_InitStructure.NVIC_IRQChannelSubPriority =0;		//子优先级3
					NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			//IRQ通道使能
					NVIC_Init(&NVIC_InitStructure);	//根据指定的参数初始化VIC寄存器、

        /* 创建SEND消息队列 */
        TxCOM1 = xQueueCreate(50, sizeof(DataSend));

        if( TxCOM1 == 0 ) /* 没有创建成功，用户可以在这里加入创建失败的处理机制 */
        {
        }

        /* 创建REVICE消息队列 */
        RxCOM1 = xQueueCreate(50, sizeof(DataRevice));

        if( RxCOM1 == 0 ) /* 没有创建成功，用户可以在这里加入创建失败的处理机制 */
        {
        }		
	    USART_Cmd(USART1, ENABLE);
    DMA_Cmd(DMA2_Stream2, ENABLE);
}


void USART1_IRQHandler(void)                	//串口1中断服务程序
{
		static uint32_t this_time_rx_len = 0;
	  DataRevice Buffer;
    BaseType_t  TaskWoken;
	  if(USART_GetITStatus(USART1, USART_IT_IDLE) != RESET)
    {
      
        (void)USART1->SR;
        (void)USART1->DR;
			   if(DMA_GetCurrentMemoryTarget(DMA2_Stream2) == 0)
        {

            DMA_Cmd(DMA2_Stream2, DISABLE);
            this_time_rx_len = BSP_USART1_DMA_RX_BUF_LEN - DMA_GetCurrDataCounter(DMA2_Stream2);
            DMA2_Stream2->NDTR = (uint16_t)BSP_USART1_DMA_RX_BUF_LEN;     //relocate the dma memory pointer to the beginning position
            DMA2_Stream2->CR |= (uint32_t)(DMA_SxCR_CT);                  //enable the current selected memory is Memory 1
            DMA_Cmd(DMA2_Stream2, ENABLE);
			      if(this_time_rx_len == MINIPC_FRAME_LENGTH)
            {
                memcpy(Buffer.buffer, _USART1_DMA_RX_BUF[0],BSP_USART1_DMA_RX_BUF_LEN ); //22uMINIPC_FRAME_LENGTH

                if(Buffer.buffer[0] == 0xA5)
                {
//									a1++;
                    if(verify_crc8_check_sum(&Buffer.buffer[1], 13) != NULL)
                    {
											c1++;
											xQueueSendFromISR(TxCOM1, &Buffer, &TaskWoken);
											DetectHook(TX2DataWGD);
										}
										
								}
								else
                {
                    return;
                }	
						}	
				}	
        else
        {
            DMA_Cmd(DMA2_Stream2, DISABLE);
            this_time_rx_len = BSP_USART1_DMA_RX_BUF_LEN - DMA_GetCurrDataCounter(DMA2_Stream2);
            DMA2_Stream2->NDTR = (uint16_t)BSP_USART1_DMA_RX_BUF_LEN;      //relocate the dma memory pointer to the beginning position
            DMA2_Stream2->CR &= ~(uint32_t)(DMA_SxCR_CT);                  //enable the current selected memory is Memory 0
            DMA_Cmd(DMA2_Stream2, ENABLE);

            if(this_time_rx_len == MINIPC_FRAME_LENGTH)
            {
                memcpy(Buffer.buffer, _USART1_DMA_RX_BUF[1],BSP_USART1_DMA_RX_BUF_LEN );//MINIPC_FRAME_LENGTH

                if(Buffer.buffer[0] == 0xA5)
                {
									a2++;
                    if(verify_crc8_check_sum(&Buffer.buffer[1], 13) != NULL)
                    {
//											c2++;
                       xQueueSendFromISR(TxCOM1, &Buffer, &TaskWoken); 
											DetectHook(TX2DataWGD);
                    }

                   
                }
                else
                {
                    return;
                }
            }
				}
				USART_ClearITPendingBit(USART1, USART_IT_IDLE);
		}
   else if(USART_GetITStatus(USART1, USART_IT_ORE_RX) != RESET)    //读入USART_SR 
    {
			    USART_ClearFlag(USART1,USART_FLAG_ORE); //读SR 
          USART_ReceiveData(USART1);       //读入USART_DR 
    }				
} 

 



