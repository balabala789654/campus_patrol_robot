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
//��׼����Ҫ��֧�ֺ���
struct __FILE
{
    int handle;
};

FILE __stdout;
//����_sys_exit()�Ա���ʹ�ð�����ģʽ
void _sys_exit(int x)
{
    x = x;
}
//�ض���fputc����
int fputc(int ch, FILE *f)
{
    while((USART1->SR & 0X40) == 0); //ѭ������,ֱ���������

    USART1->DR = (u8) ch;
    return ch;
}
#endif

void Device_Usart1_ENABLE_Init(u32 bound)
{




}