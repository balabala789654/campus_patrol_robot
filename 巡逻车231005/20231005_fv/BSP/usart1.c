#include "sys.h"
#if SYSTEM_SUPPORT_OS
    #include "FreeRTOS.h"
    #include "queue.h"
#endif

#include "usart.h"
#include "string.h"


extern QueueHandle_t TxCOM1;
extern QueueHandle_t RxCOM1;

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
    while((USART1->SR & 0X40) == 0); //循环发送,直到发送完毕

    USART1->DR = (u8) ch;
    return ch;
}
#endif

void Device_Usart1_ENABLE_Init(u32 bound)
{




}