#include "rc.h"

void RC_Init(uint8_t *rx1_buf, uint8_t *rx2_buf, uint16_t dma_buf_num)//数组0 1   缓存长度
{
    /* -------------- Enable Module Clock Source ----------------------------*/
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);// | RCC_AHB1Periph_DMA1
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2, ENABLE);

    RCC_APB1PeriphResetCmd(RCC_APB1Periph_USART2, ENABLE);
    RCC_APB1PeriphResetCmd(RCC_APB1Periph_USART2, ENABLE);//RCC_APB1PeriphResetCmd(RCC_APB1Periph_USART3, DISABLE);
	  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_DMA1,ENABLE);

    GPIO_PinAFConfig(GPIOA, GPIO_PinSource3, GPIO_AF_USART2); //PA3  usart2 rx
    /* -------------- Configure GPIO ---------------------------------------*/
    {
        GPIO_InitTypeDef GPIO_InitStructure;
        USART_InitTypeDef USART_InitStructure;
        GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3;
        GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
        GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
        GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;//GPIO_Speed_100MHz
        GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
        GPIO_Init(GPIOA, &GPIO_InitStructure);

        USART_DeInit(USART2);

        USART_InitStructure.USART_BaudRate = 100000;
        USART_InitStructure.USART_WordLength = USART_WordLength_8b;
        USART_InitStructure.USART_StopBits = USART_StopBits_1;
        USART_InitStructure.USART_Parity = USART_Parity_Even;
        USART_InitStructure.USART_Mode = USART_Mode_Rx;
        USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
        USART_Init(USART2, &USART_InitStructure);

        USART_DMACmd(USART2, USART_DMAReq_Rx, ENABLE);

        USART_ClearFlag(USART2, USART_FLAG_IDLE);
        USART_ITConfig(USART2, USART_IT_IDLE, ENABLE);

        USART_Cmd(USART2, ENABLE);
    }

    /* -------------- Configure NVIC ---------------------------------------*/
    {
        NVIC_InitTypeDef NVIC_InitStructure;
        NVIC_InitStructure.NVIC_IRQChannel = USART2_IRQn;
        NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = RC_NVIC;
        NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
        NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
        NVIC_Init(&NVIC_InitStructure);
    }


    /* -------------- Configure DMA -----------------------------------------*/
    {
        DMA_InitTypeDef DMA_InitStructure;
        DMA_DeInit(DMA1_Stream5);

        DMA_InitStructure.DMA_Channel = DMA_Channel_4;
        DMA_InitStructure.DMA_PeripheralBaseAddr = (uint32_t) & (USART2->DR);//更新数据流
        DMA_InitStructure.DMA_Memory0BaseAddr = (uint32_t)rx1_buf;
        DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralToMemory;
        DMA_InitStructure.DMA_BufferSize = dma_buf_num;//
        DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
        DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;
        DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;
        DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte;
        DMA_InitStructure.DMA_Mode = DMA_Mode_Circular;
        DMA_InitStructure.DMA_Priority = DMA_Priority_VeryHigh;
        DMA_InitStructure.DMA_FIFOMode = DMA_FIFOMode_Disable;
        DMA_InitStructure.DMA_FIFOThreshold = DMA_FIFOThreshold_1QuarterFull;
        DMA_InitStructure.DMA_MemoryBurst = DMA_MemoryBurst_Single;
        DMA_InitStructure.DMA_PeripheralBurst = DMA_PeripheralBurst_Single;
        DMA_Init(DMA1_Stream5, &DMA_InitStructure);
        DMA_DoubleBufferModeConfig(DMA1_Stream5, (uint32_t)rx2_buf, DMA_Memory_0);
        DMA_DoubleBufferModeCmd(DMA1_Stream5, ENABLE);
        DMA_Cmd(DMA1_Stream5, DISABLE); //Add a disable  DMA_Cmd(DMA1_Stream1, DISABLE)
        DMA_Cmd(DMA1_Stream5, ENABLE);//   DMA_Cmd(DMA1_Stream1, ENABLE)
    }
}
void RC_unable(void)
{
    USART_Cmd(USART2, DISABLE);
}
void RC_restart(uint16_t dma_buf_num)
{
    USART_Cmd(USART2, DISABLE);
    DMA_Cmd(DMA1_Stream5, DISABLE);
    DMA_SetCurrDataCounter(DMA1_Stream5, dma_buf_num);

    USART_ClearFlag(USART2, USART_FLAG_IDLE);

    DMA_ClearFlag(DMA1_Stream5, DMA_FLAG_TCIF4);
    DMA_ClearITPendingBit(DMA1_Stream5, DMA_IT_TCIF4);
    DMA_Cmd(DMA1_Stream5, ENABLE);
    USART_Cmd(USART2, ENABLE);
}
