/**
  **********************************(C) NCIST ************************************
  * @file       detect_task.c/h
  * @brief      �豸�����ж�����ͨ��freeRTOS�δ�ʱ����Ϊϵͳʱ�䣬�豸��ȡ���ݺ�
  *             ����DetectHook��¼��Ӧ�豸��ʱ�䣬�ڸ������ͨ���жϼ�¼ʱ����ϵͳ
  *             ʱ��֮�����жϵ��ߡ�
  *				���֡�ʣ��жϸ�ϵͳ֡���Ƿ���������Ӧ�������ݴ�����
  *				��ȡ��Ӧ��ʩ�������ʩ���ָ���ʩ��
  *				���� 12��LED��ѯ�鿴ϵͳ״̬
  * @history
  *  Version    Date            Author          Modification
  *  V1.0.0     Dec-26-2018     RM              1. ���
  *
  @verbatim
  ==============================================================================

  ==============================================================================
  @endverbatim
  **********************************(C) NCIST ************************************
  */
#include "main.h"
#define DETECT_TASK_INIT_TIME 57

//��ʼ���������б�
static void DetectInit(uint32_t time);
//static void DetectDisplay(void);
void DetectHook(uint8_t WDG);

monitor_t monitorList[monitorLength + 1];//static

#if INCLUDE_uxTaskGetStackHighWaterMark
    uint32_t DetectTaskStack;
#endif

static void DetectInit(uint32_t time)
{
    //��������ʱ�䣬�����ȶ�����ʱ�䣬���ȼ� offlineTime onlinetime priority (��Ҫ�ظ�)
    uint16_t setItem[monitorLength][3] =
    {
        {40, 40, 13}, //PCDATA 0
        {60, 40, 12}, //SBUS 1
        {10, 10, 11},   //Helm motor1 2
        {10, 10, 10},   //Helm motor2 3
        {10, 10, 9}, //Helm motor13 4
        {10, 10, 8}, //Helm motor14 5
        {10, 10, 7}, //Power motor1 6
        {10, 10, 6},  //Power motor2 7
        {10, 10, 5},  //Power motor3 8
        {10, 10, 4}, //Power motor4 9
        {10, 10, 3}, //Clamp  motor 10
    };

    for (uint8_t i = 0; i < monitorLength; i++)
    {
        monitorList[i].setOfflineTime = setItem[i][0];
        monitorList[i].setOnlineTime = setItem[i][1];
        monitorList[i].Priority = setItem[i][2];
        monitorList[i].dataIsErrorFun = NULL;
        monitorList[i].solveLostFun = NULL;
        monitorList[i].solveDataErrorFun = NULL;

        monitorList[i].enable = 1;
        monitorList[i].errorExist = 1;
        monitorList[i].isLost = 1;
        monitorList[i].dataIsError = 1;
        monitorList[i].frequency = 0.0f;
        monitorList[i].newTime = time;
        monitorList[i].lastTime = time;
        monitorList[i].Losttime = time;
        monitorList[i].worktime = time;
    }

    monitorList[DBUSWDG].dataIsErrorFun = RC_data_is_error;
    monitorList[DBUSWDG].solveLostFun = NULL;
    monitorList[DBUSWDG].solveDataErrorFun = slove_data_error;

}
//����������
void detect_task(void  *pvParameters)
{
    static uint32_t systemTime;
    systemTime = xTaskGetTickCount();
    //��ʼ��
    DetectInit(systemTime);
    //����һ��ʱ��
    vTaskDelay(DETECT_TASK_INIT_TIME);

    while(1)
    {
        static uint8_t error_num_display = 0;
        systemTime = xTaskGetTickCount();

        error_num_display = monitorLength;
        monitorList[monitorLength].isLost = 0;
        monitorList[monitorLength].errorExist = 0;

        for (int i = 0; i < monitorLength; i++)
        {
            //δʹ�ܣ�����
            if (monitorList[i].enable == 0)
            {
                continue;
            }

            //�жϵ���
            if (systemTime - monitorList[i].newTime > monitorList[i].setOfflineTime)//�Ѿ�����
            {
                if (monitorList[i].errorExist == 0)
                {
                    //��¼�����Լ�����ʱ��
                    monitorList[i].isLost = 1;//�Ѷ�ʧ
                    monitorList[i].errorExist = 1;//�������
                    monitorList[i].Losttime = systemTime;
                }

                //�жϴ������ȼ��� �������ȼ���ߵĴ�����
                if (monitorList[i].Priority > monitorList[error_num_display].Priority)
                {
                    error_num_display = i;
                }

                //����ṩ������������н������
                if (monitorList[i].solveLostFun != NULL)
                {
                    monitorList[i].solveLostFun();
                }
            }
            else if (systemTime - monitorList[i].worktime < monitorList[i].setOnlineTime)
            {
                //�ո����ߣ����ܴ������ݲ��ȶ���ֻ��¼����ʧ��
                monitorList[i].isLost = 0;//δ��ʧ
                monitorList[i].errorExist = 1;//�������
            }
            else
            {
                monitorList[i].isLost = 0;//δ��ʧ

                //�ж��Ƿ�������ݴ���
                if (monitorList[i].dataIsError)
                {
                    monitorList[i].errorExist = 1;//�������
                }
                else
                {
                    monitorList[i].errorExist = 0;//���󲻴���
                }

                //����Ƶ��
                if (monitorList[i].newTime > monitorList[i].lastTime)
                {
                    monitorList[i].frequency = configTICK_RATE_HZ / (fp32)(monitorList[i].newTime - monitorList[i].lastTime);
                }
            }
        }

//        DetectDisplay();
        vTaskDelay(10);
        #if INCLUDE_uxTaskGetStackHighWaterMark
        DetectTaskStack = uxTaskGetStackHighWaterMark(NULL);
        #endif
    }
}

//�豸�������ݹ��Ӻ���
void DetectHook(uint8_t WDG)
{
    monitorList[WDG].lastTime = monitorList[WDG].newTime;
    monitorList[WDG].newTime = xTaskGetTickCount();

//���¶�ʧ���
    if (monitorList[WDG].isLost)
    {
        monitorList[WDG].isLost = 0;
        monitorList[WDG].worktime = monitorList[WDG].newTime;
    }

    //�ж������Ƿ����
    if (monitorList[WDG].dataIsErrorFun != NULL)
    {
        if (monitorList[WDG].dataIsErrorFun())	//NULL == 0
        {
            monitorList[WDG].errorExist = 1;
            monitorList[WDG].dataIsError = 1;

            if (monitorList[WDG].solveDataErrorFun != NULL)
            {
                monitorList[WDG].solveDataErrorFun();
            }
        }
        else
        {
            monitorList[WDG].dataIsError = 0;
        }
    }
    else
    {
        monitorList[WDG].dataIsError = 0;
    }
}

//static void DetectDisplay(void)
//{
////	static uint8_t last_num = errorListLength + 1;
//    static uint8_t i = 0 ;
//    static int time = 0;
//    static uint8_t cnt = 0;
//		static uint8_t cnts = 0;
//    //��ˮ��ʾÿ�����ݵĴ���������
//    for (i = 0; i < ChassisPowerMotor4WDG; i++)
//    {
//       
////	ÿ�μ������һ��ʱ�� ������ʾ12�����ݴ������

//    }
//}


//���ض�Ӧ���豸�Ƿ���ڴ���
bool_t toe_is_error(uint8_t err)
{
    return (monitorList[err].errorExist == 1);
}
//�����б�ָ�����ݴ���
const monitor_t *getErrorListPoint(void)
{
    return monitorList;
}












