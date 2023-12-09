#include "bizhang_usart.h"

void bizhang_Usart(uint8_t *rx1_buf, uint8_t *rx2_buf, uint16_t dma_buf_num)
{
	  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOTx, ENABLE);// | RCC_AHB1Periph_DMA1
		RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIORx, ENABLE);
    RCC_APB1PeriphClockCmd(RCC_APB2Periph_USARTx, ENABLE);
		RCC_APB1PeriphResetCmd(RCC_APB2Periph_USARTx, ENABLE);//外设复位
		
	  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_DMAx,ENABLE);
		GPIO_PinAFConfig(GPIOD, GPIO_PinSourceRx, GPIO_AF_USARTx); //io复用
		GPIO_PinAFConfig(GPIOC, GPIO_PinSourceTx, GPIO_AF_USARTx);
	    /* -------------- Configure DMA -----------------------------------------*/
    {
        DMA_InitTypeDef DMA_InitStructure;
        DMA_DeInit(DMAx_Streamx_rx);//复位
				DMA_StructInit(&DMA_InitStructure);
				
        DMA_InitStructure.DMA_Channel = DMA_Channel_x;//通道4
        DMA_InitStructure.DMA_PeripheralBaseAddr = (uint32_t) & (usartx->DR);//串口3的接收寄存器的地址
        DMA_InitStructure.DMA_Memory0BaseAddr = (uint32_t)rx1_buf;//存储数据的数组地址
        DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralToMemory;//外设到寄存器模式
        DMA_InitStructure.DMA_BufferSize = dma_buf_num;//数据传输量
        DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;//外设非增量模式
        DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;//设置DMA的内存递增模式，DMA 访问多个内存参数时，需要使用
        DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;//存储器数据长度8位
        DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte;//传输数据的宽度
        DMA_InitStructure.DMA_Mode = DMA_Mode_Normal;//循环模式
        DMA_InitStructure.DMA_Priority = DMA_Priority_VeryHigh;//设置优先级
        DMA_InitStructure.DMA_FIFOMode = DMA_FIFOMode_Disable;
        DMA_InitStructure.DMA_FIFOThreshold = DMA_FIFOThreshold_1QuarterFull;
        DMA_InitStructure.DMA_MemoryBurst = DMA_MemoryBurst_Single;
        DMA_InitStructure.DMA_PeripheralBurst = DMA_PeripheralBurst_Single;
        DMA_Init(DMAx_Streamx_rx, &DMA_InitStructure);
        DMA_DoubleBufferModeConfig(DMAx_Streamx_rx, (uint32_t)rx2_buf, DMA_Memory_0);//双缓冲
        DMA_DoubleBufferModeCmd(DMAx_Streamx_rx, ENABLE);
				
//				DMA_DeInit(DMAx_Streamx_tx);
//				DMA_StructInit(&DMA_InitStructure);
//				DMA_InitStructure.DMA_Channel = DMA_Channel_x;
//        DMA_InitStructure.DMA_PeripheralBaseAddr = (uint32_t)(&USART6->DR);
//        DMA_InitStructure.DMA_Memory0BaseAddr = (uint32_t)fff;//(uint32_t)(Pc_Send_Data.PcDataArray);
//        DMA_InitStructure.DMA_DIR = DMA_DIR_MemoryToPeripheral;
//        DMA_InitStructure.DMA_BufferSize = 1;//TO_PC_LENGTH;
//        DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
//        DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;
//        DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;
//        DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte;
//        DMA_InitStructure.DMA_Mode = DMA_Mode_Circular;
//        DMA_InitStructure.DMA_Priority = DMA_Priority_High;
//        DMA_InitStructure.DMA_FIFOMode = DMA_FIFOMode_Disable;
//        DMA_InitStructure.DMA_FIFOThreshold = DMA_FIFOThreshold_1QuarterFull;
//        DMA_InitStructure.DMA_MemoryBurst = DMA_MemoryBurst_Single;
//        DMA_InitStructure.DMA_PeripheralBurst = DMA_PeripheralBurst_Single;
//        DMA_Init(DMAx_Streamx_tx, &DMA_InitStructure);

    }
    /* -------------- Configure GPIO ---------------------------------------*/
    {
        GPIO_InitTypeDef GPIO_InitStructure;
        USART_InitTypeDef USART_InitStructure;
			  USART_ClockInitTypeDef USART_ClockInitStruct;
        GPIO_InitStructure.GPIO_Pin = GPIO_Pin_Tx;//GPIOC11
        GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;//复用功能
        GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;//推挽复用输出
        GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;////速度50MHz
        GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;//上拉
        GPIO_Init(GPIOC, &GPIO_InitStructure);//初始化
			
        GPIO_InitStructure.GPIO_Pin = GPIO_Pin_Rx;//GPIOC11
        GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;//复用功能
        GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;//推挽复用输出
        GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;////速度50MHz
        GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;//上拉
        GPIO_Init(GPIOD, &GPIO_InitStructure);//初始化			

        USART_DeInit(usartx);//调用了RCC_APBxPeriphResetCmd函数对寄存器进行复位
				USART_StructInit(&USART_InitStructure);
        USART_InitStructure.USART_BaudRate = usart_bound;//波特率设置
        USART_InitStructure.USART_WordLength = usart_wordlength;//字长为8位数据格式
        USART_InitStructure.USART_StopBits = usart_stopbits;//一个停止位
        USART_InitStructure.USART_Parity = usart_parity;//校验位
        USART_InitStructure.USART_Mode = USART_Mode_Rx|USART_Mode_Tx;//接收模式
        USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//无硬件数据流控制
        USART_Init(usartx, &USART_InitStructure);//初始化串口3

        USART_DMACmd(usartx, USART_DMAReq_Rx, ENABLE);// 开启串口接收DMA
//				USART_DMACmd(usartx, USART_DMAReq_Tx, ENABLE);
 
        USART_ClearFlag(usartx, USART_FLAG_IDLE);// 清除中断标志位
        USART_ITConfig(usartx, USART_IT_IDLE, ENABLE);//设置串口中断类型并使能
				USART_ClockInitStruct.USART_Clock = USART_Clock_Disable;  //时钟低电平活动
        USART_ClockInitStruct.USART_CPOL = USART_CPOL_Low;  //SLCK引脚上时钟输出的极性->低电平
        USART_ClockInitStruct.USART_CPHA = USART_CPHA_2Edge;  //时钟第二个边沿进行数据捕获
        USART_ClockInitStruct.USART_LastBit = USART_LastBit_Disable; //最后一位数据的时钟脉冲不从SCLK输出
        USART_ClockInit(usartx, &USART_ClockInitStruct);
        USART_Cmd(usartx, ENABLE);//使能串口3
    }
		    DMA_Cmd(DMAx_Streamx_rx, DISABLE); //
        DMA_Cmd(DMAx_Streamx_rx, ENABLE);//
				
    /* -------------- 初始化 NVIC ---------------------------------------*/
    {
        NVIC_InitTypeDef NVIC_InitStructure;
        NVIC_InitStructure.NVIC_IRQChannel = USARTx_IRQn;//串口3中断通道
        NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 7;//抢占优先级
        NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;//子优先级
        NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;//通道使能
        NVIC_Init(&NVIC_InitStructure);//初始化NVIC
    }



}

//void usart_restart(uint16_t dma_buf_num)
//{
//    USART_Cmd(usartx, DISABLE);
//    DMA_Cmd(DMAx_Streamx, DISABLE);
//    DMA_SetCurrDataCounter(DMAx_Streamx, dma_buf_num);

//    USART_ClearFlag(usartx, USART_FLAG_IDLE);

//    DMA_ClearFlag(DMAx_Streamx, DMA_FLAG_TCIF4);
//    DMA_ClearITPendingBit(DMAx_Streamx, DMA_IT_TCIF4);
//    DMA_Cmd(DMAx_Streamx, ENABLE);
//    USART_Cmd(usartx, ENABLE);
//}
