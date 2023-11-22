#include "usart.h"
#if SYSTEM_SUPPORT_OS
#include "FreeRTOS.h"					//FreeRTOS使用	  
#endif

#include "const.h"
#include "robot.h"

void usart1_init(u32 bound, uint8_t* rx1_buf){
	//GPIO端口设置
	GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA,ENABLE); //使能GPIOA时钟
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1,ENABLE);//使能USART1时钟
 
	//串口1对应引脚复用映射
	GPIO_PinAFConfig(GPIOA,GPIO_PinSource9,GPIO_AF_USART1); //GPIOA9复用为USART1
	GPIO_PinAFConfig(GPIOA,GPIO_PinSource10,GPIO_AF_USART1); //GPIOA10复用为USART1
	
	//USART1端口配置
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9 | GPIO_Pin_10; //GPIOA9与GPIOA10
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;//复用功能
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;	//速度50MHz
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
	USART_Init(USART1, &USART_InitStructure); //初始化串口1
	
    USART_DMACmd(USART1, USART_DMAReq_Rx, ENABLE);
	
	USART_Cmd(USART1, ENABLE);  //使能串口1 
	
	//USART_ClearFlag(USART1, USART_FLAG_TC);
	
	USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);//开启相关中断
	USART_ITConfig(USART1, USART_IT_IDLE, ENABLE);//开启相关中断

	//Usart1 NVIC 配置
	NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;//串口1中断通道
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=3;//抢占优先级3
	NVIC_InitStructure.NVIC_IRQChannelSubPriority =3;		//子优先级3
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			//IRQ通道使能
	NVIC_Init(&NVIC_InitStructure);	//根据指定的参数初始化VIC寄存器、
	
//    {
//        DMA_InitTypeDef DMA_InitStructure;
//        DMA_DeInit(DMA2_Stream5);
//        DMA_InitStructure.DMA_Channel = DMA_Channel_4;
//        DMA_InitStructure.DMA_PeripheralBaseAddr = (uint32_t) & (USART1->DR);
//        DMA_InitStructure.DMA_Memory0BaseAddr = (uint32_t)rx1_buf;
//        DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralToMemory;
//        DMA_InitStructure.DMA_BufferSize = 10;//
//        DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
//        DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;
//        DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;
//        DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte;
//        DMA_InitStructure.DMA_Mode = DMA_Mode_Circular;
//        DMA_InitStructure.DMA_Priority = DMA_Priority_VeryHigh;
//        DMA_InitStructure.DMA_FIFOMode = DMA_FIFOMode_Disable;
//        DMA_InitStructure.DMA_FIFOThreshold = DMA_FIFOThreshold_1QuarterFull;
//        DMA_InitStructure.DMA_MemoryBurst = DMA_MemoryBurst_Single;
//        DMA_InitStructure.DMA_PeripheralBurst = DMA_PeripheralBurst_Single;
//        DMA_Init(DMA2_Stream5, &DMA_InitStructure);
//        DMA_DoubleBufferModeConfig(DMA2_Stream5, (uint32_t)rx2_buf, DMA_Memory_0);
//        DMA_DoubleBufferModeCmd(DMA2_Stream5, ENABLE);
//        DMA_Cmd(DMA2_Stream5, DISABLE); //Add a disable  DMA_Cmd(DMA1_Stream1, DISABLE)
//        DMA_Cmd(DMA2_Stream5, ENABLE);//   DMA_Cmd(DMA1_Stream1, ENABLE)
//    }
	
}

void usart2_Init(uint8_t* rx1_buf, uint8_t* rx2_buf, uint16_t dma_buf_num)
{
    /* -------------- Enable Module Clock Source ----------------------------*/
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);// | RCC_AHB1Periph_DMA1
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2, ENABLE);

    RCC_APB1PeriphResetCmd(RCC_APB1Periph_USART2, ENABLE);
    RCC_APB1PeriphResetCmd(RCC_APB1Periph_USART2, ENABLE);//RCC_APB1PeriphResetCmd(RCC_APB1Periph_USART3, DISABLE);
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_DMA1,ENABLE);

    GPIO_PinAFConfig(GPIOA, GPIO_PinSource3, GPIO_AF_USART2); //PB11  usart3 rx
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
		USART_ITConfig(USART2, USART_IT_RXNE, ENABLE);
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
        DMA_InitStructure.DMA_PeripheralBaseAddr = (uint32_t) & (USART2->DR);
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

void usart6_init(u32 bound){
	//GPIO端口设置
	GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC,ENABLE); //使能GPIOA时钟
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART6,ENABLE);//使能USART1时钟
 
	//串口1对应引脚复用映射
	GPIO_PinAFConfig(GPIOC,GPIO_PinSource7,GPIO_AF_USART6); //GPIOA9复用为USART1
	GPIO_PinAFConfig(GPIOC,GPIO_PinSource6,GPIO_AF_USART6); //GPIOA10复用为USART1
	
	//USART1端口配置
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6 | GPIO_Pin_7; //GPIOA9与GPIOA10
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;//复用功能
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;	//速度50MHz
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP; //推挽复用输出
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP; //上拉
	GPIO_Init(GPIOC,&GPIO_InitStructure); //初始化PA9，PA10

   //USART1 初始化设置
	USART_InitStructure.USART_BaudRate = bound;//波特率设置
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;//字长为8位数据格式
	USART_InitStructure.USART_StopBits = USART_StopBits_1;//一个停止位
	USART_InitStructure.USART_Parity = USART_Parity_No;//无奇偶校验位
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//无硬件数据流控制
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;	//收发模式
	USART_Init(USART6, &USART_InitStructure); //初始化串口1

	USART_Cmd(USART6, ENABLE);  //使能串口1 
	USART_DMACmd(USART6, USART_DMAReq_Rx, ENABLE);

	//USART_ClearFlag(USART1, USART_FLAG_TC);
	
	USART_ITConfig(USART6, USART_IT_RXNE, ENABLE);//开启相关中断
	USART_ITConfig(USART6, USART_IT_IDLE, ENABLE);//开启相关中断

	//Usart1 NVIC 配置
	NVIC_InitStructure.NVIC_IRQChannel = USART6_IRQn;//串口1中断通道
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=3;//抢占优先级3
	NVIC_InitStructure.NVIC_IRQChannelSubPriority =3;		//子优先级3
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			//IRQ通道使能
	NVIC_Init(&NVIC_InitStructure);	//根据指定的参数初始化VIC寄存器、
	
//	
//	{
//        DMA_InitTypeDef DMA_InitStructure;
//        DMA_DeInit(DMA2_Stream1);
//        DMA_InitStructure.DMA_Channel = DMA_Channel_5;
//        DMA_InitStructure.DMA_PeripheralBaseAddr = (uint32_t) & (USART6->DR);
//        DMA_InitStructure.DMA_Memory0BaseAddr = (uint32_t)rx1_buf;
//        DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralToMemory;
//        DMA_InitStructure.DMA_BufferSize = 10;	//
//        DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
//        DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;
//        DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;
//        DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte;
//        DMA_InitStructure.DMA_Mode = DMA_Mode_Circular; // 循环模式
//        DMA_InitStructure.DMA_Priority = DMA_Priority_VeryHigh;
//        DMA_InitStructure.DMA_FIFOMode = DMA_FIFOMode_Disable;
//        DMA_InitStructure.DMA_FIFOThreshold = DMA_FIFOThreshold_1QuarterFull;
//        DMA_InitStructure.DMA_MemoryBurst = DMA_MemoryBurst_Single;
//        DMA_InitStructure.DMA_PeripheralBurst = DMA_PeripheralBurst_Single;
//        DMA_Init(DMA2_Stream1, &DMA_InitStructure);
//        DMA_DoubleBufferModeConfig(DMA2_Stream1, (uint32_t)rx2_buf, DMA_Memory_0);
//        DMA_DoubleBufferModeCmd(DMA2_Stream1, ENABLE);
//        DMA_Cmd(DMA2_Stream1, DISABLE); //Add a disable  DMA_Cmd(DMA1_Stream1, DISABLE)
//        DMA_Cmd(DMA2_Stream1, ENABLE);//   DMA_Cmd(DMA1_Stream1, ENABLE)
//    }
	
}

union DoubleToBytes
{
	float value;
	char bytes[4];
};
union DoubleToBytes conver;
union DoubleToBytes conver1;

uint8_t usart1_buffer[10];

static int i=0;
static char flag=0;
void USART1_IRQHandler(void)                	//串口1中断服务程序
{
	if(USART_GetITStatus(USART1, USART_IT_RXNE) != RESET)  //接收中断(接收到的数据必须是0x0d 0x0a结尾)
	{
		if(USART_ReceiveData(USART1) == frame_head) 
			flag=1;
		else if(USART_ReceiveData(USART1) == frame_end) 
		{
			usart1_buffer[i++] = USART_ReceiveData(USART1);
			flag=0;
		}
		if(flag) 
			usart1_buffer[i++] = USART_ReceiveData(USART1);
//		usart1_buffer[i++] = USART_ReceiveData(USART1);
		if(i == 10) {
			i = 0;
			
			conver.bytes[0]=usart1_buffer[1];
			conver.bytes[1]=usart1_buffer[2];
			conver.bytes[2]=usart1_buffer[3];
			conver.bytes[3]=usart1_buffer[4];

			conver1.bytes[0]=usart1_buffer[5];
			conver1.bytes[1]=usart1_buffer[6];
			conver1.bytes[2]=usart1_buffer[7];
			conver1.bytes[3]=usart1_buffer[8];
			
			robot.twist_linear = conver.value;
			robot.twist_angle = conver1.value;
		}
		USART_ClearFlag(USART1, USART_FLAG_RXNE);
	}
	else if(USART_GetITStatus(USART1, USART_IT_IDLE) != RESET){
		USART_ReceiveData(USART1);
		USART_ClearFlag(USART1, USART_FLAG_IDLE);
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


