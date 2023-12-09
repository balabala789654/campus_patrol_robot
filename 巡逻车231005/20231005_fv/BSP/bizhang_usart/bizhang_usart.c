#include "bizhang_usart.h"

void bizhang_Usart(uint8_t *rx1_buf, uint8_t *rx2_buf, uint16_t dma_buf_num)
{
	  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOTx, ENABLE);// | RCC_AHB1Periph_DMA1
		RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIORx, ENABLE);
    RCC_APB1PeriphClockCmd(RCC_APB2Periph_USARTx, ENABLE);
		RCC_APB1PeriphResetCmd(RCC_APB2Periph_USARTx, ENABLE);//���踴λ
		
	  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_DMAx,ENABLE);
		GPIO_PinAFConfig(GPIOD, GPIO_PinSourceRx, GPIO_AF_USARTx); //io����
		GPIO_PinAFConfig(GPIOC, GPIO_PinSourceTx, GPIO_AF_USARTx);
	    /* -------------- Configure DMA -----------------------------------------*/
    {
        DMA_InitTypeDef DMA_InitStructure;
        DMA_DeInit(DMAx_Streamx_rx);//��λ
				DMA_StructInit(&DMA_InitStructure);
				
        DMA_InitStructure.DMA_Channel = DMA_Channel_x;//ͨ��4
        DMA_InitStructure.DMA_PeripheralBaseAddr = (uint32_t) & (usartx->DR);//����3�Ľ��ռĴ����ĵ�ַ
        DMA_InitStructure.DMA_Memory0BaseAddr = (uint32_t)rx1_buf;//�洢���ݵ������ַ
        DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralToMemory;//���赽�Ĵ���ģʽ
        DMA_InitStructure.DMA_BufferSize = dma_buf_num;//���ݴ�����
        DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;//���������ģʽ
        DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;//����DMA���ڴ����ģʽ��DMA ���ʶ���ڴ����ʱ����Ҫʹ��
        DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;//�洢�����ݳ���8λ
        DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte;//�������ݵĿ��
        DMA_InitStructure.DMA_Mode = DMA_Mode_Normal;//ѭ��ģʽ
        DMA_InitStructure.DMA_Priority = DMA_Priority_VeryHigh;//�������ȼ�
        DMA_InitStructure.DMA_FIFOMode = DMA_FIFOMode_Disable;
        DMA_InitStructure.DMA_FIFOThreshold = DMA_FIFOThreshold_1QuarterFull;
        DMA_InitStructure.DMA_MemoryBurst = DMA_MemoryBurst_Single;
        DMA_InitStructure.DMA_PeripheralBurst = DMA_PeripheralBurst_Single;
        DMA_Init(DMAx_Streamx_rx, &DMA_InitStructure);
        DMA_DoubleBufferModeConfig(DMAx_Streamx_rx, (uint32_t)rx2_buf, DMA_Memory_0);//˫����
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
        GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;//���ù���
        GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;//���츴�����
        GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;////�ٶ�50MHz
        GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;//����
        GPIO_Init(GPIOC, &GPIO_InitStructure);//��ʼ��
			
        GPIO_InitStructure.GPIO_Pin = GPIO_Pin_Rx;//GPIOC11
        GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;//���ù���
        GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;//���츴�����
        GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;////�ٶ�50MHz
        GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;//����
        GPIO_Init(GPIOD, &GPIO_InitStructure);//��ʼ��			

        USART_DeInit(usartx);//������RCC_APBxPeriphResetCmd�����ԼĴ������и�λ
				USART_StructInit(&USART_InitStructure);
        USART_InitStructure.USART_BaudRate = usart_bound;//����������
        USART_InitStructure.USART_WordLength = usart_wordlength;//�ֳ�Ϊ8λ���ݸ�ʽ
        USART_InitStructure.USART_StopBits = usart_stopbits;//һ��ֹͣλ
        USART_InitStructure.USART_Parity = usart_parity;//У��λ
        USART_InitStructure.USART_Mode = USART_Mode_Rx|USART_Mode_Tx;//����ģʽ
        USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//��Ӳ������������
        USART_Init(usartx, &USART_InitStructure);//��ʼ������3

        USART_DMACmd(usartx, USART_DMAReq_Rx, ENABLE);// �������ڽ���DMA
//				USART_DMACmd(usartx, USART_DMAReq_Tx, ENABLE);
 
        USART_ClearFlag(usartx, USART_FLAG_IDLE);// ����жϱ�־λ
        USART_ITConfig(usartx, USART_IT_IDLE, ENABLE);//���ô����ж����Ͳ�ʹ��
				USART_ClockInitStruct.USART_Clock = USART_Clock_Disable;  //ʱ�ӵ͵�ƽ�
        USART_ClockInitStruct.USART_CPOL = USART_CPOL_Low;  //SLCK������ʱ������ļ���->�͵�ƽ
        USART_ClockInitStruct.USART_CPHA = USART_CPHA_2Edge;  //ʱ�ӵڶ������ؽ������ݲ���
        USART_ClockInitStruct.USART_LastBit = USART_LastBit_Disable; //���һλ���ݵ�ʱ�����岻��SCLK���
        USART_ClockInit(usartx, &USART_ClockInitStruct);
        USART_Cmd(usartx, ENABLE);//ʹ�ܴ���3
    }
		    DMA_Cmd(DMAx_Streamx_rx, DISABLE); //
        DMA_Cmd(DMAx_Streamx_rx, ENABLE);//
				
    /* -------------- ��ʼ�� NVIC ---------------------------------------*/
    {
        NVIC_InitTypeDef NVIC_InitStructure;
        NVIC_InitStructure.NVIC_IRQChannel = USARTx_IRQn;//����3�ж�ͨ��
        NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 7;//��ռ���ȼ�
        NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;//�����ȼ�
        NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;//ͨ��ʹ��
        NVIC_Init(&NVIC_InitStructure);//��ʼ��NVIC
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
