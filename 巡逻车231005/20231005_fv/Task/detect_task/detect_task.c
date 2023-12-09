/**
  **********************************(C) NCIST ************************************
  * @file       detect_task.c/h
  * @brief      设备离线判断任务，通过freeRTOS滴答时间作为系统时间，设备获取数据后
  *             调用DetectHook记录对应设备的时间，在该任务会通过判断记录时间与系统
  *             时间之差来判断掉线。
  *				检测帧率，判断各系统帧率是否正常，对应处理数据错误功能
  *				采取对应措施。防疯措施，恢复措施。
  *				采用 12个LED轮询查看系统状态
  * @history
  *  Version    Date            Author          Modification
  *  V1.0.0     Dec-26-2018     RM              1. 完成
  *
  @verbatim
  ==============================================================================

  ==============================================================================
  @endverbatim
  **********************************(C) NCIST ************************************
  */
#include "main.h"
#define DETECT_TASK_INIT_TIME 57

//初始化监视器列表
static void DetectInit(uint32_t time);
//static void DetectDisplay(void);
void DetectHook(uint8_t WDG);

monitor_t monitorList[monitorLength + 1];//static

#if INCLUDE_uxTaskGetStackHighWaterMark
    uint32_t DetectTaskStack;
#endif

static void DetectInit(uint32_t time)
{
    //设置离线时间，上线稳定工作时间，优先级 offlineTime onlinetime priority (不要重复)
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
//监视器任务
void detect_task(void  *pvParameters)
{
    static uint32_t systemTime;
    systemTime = xTaskGetTickCount();
    //初始化
    DetectInit(systemTime);
    //空闲一段时间
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
            //未使能，跳过
            if (monitorList[i].enable == 0)
            {
                continue;
            }

            //判断掉线
            if (systemTime - monitorList[i].newTime > monitorList[i].setOfflineTime)//已经离线
            {
                if (monitorList[i].errorExist == 0)
                {
                    //记录错误以及掉线时间
                    monitorList[i].isLost = 1;//已丢失
                    monitorList[i].errorExist = 1;//错误存在
                    monitorList[i].Losttime = systemTime;
                }

                //判断错误优先级， 保存优先级最高的错误码
                if (monitorList[i].Priority > monitorList[error_num_display].Priority)
                {
                    error_num_display = i;
                }

                //如果提供解决函数，运行解决函数
                if (monitorList[i].solveLostFun != NULL)
                {
                    monitorList[i].solveLostFun();
                }
            }
            else if (systemTime - monitorList[i].worktime < monitorList[i].setOnlineTime)
            {
                //刚刚上线，可能存在数据不稳定，只记录不丢失，
                monitorList[i].isLost = 0;//未丢失
                monitorList[i].errorExist = 1;//错误存在
            }
            else
            {
                monitorList[i].isLost = 0;//未丢失

                //判断是否存在数据错误
                if (monitorList[i].dataIsError)
                {
                    monitorList[i].errorExist = 1;//错误存在
                }
                else
                {
                    monitorList[i].errorExist = 0;//错误不存在
                }

                //计算频率
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

//设备接收数据钩子函数
void DetectHook(uint8_t WDG)
{
    monitorList[WDG].lastTime = monitorList[WDG].newTime;
    monitorList[WDG].newTime = xTaskGetTickCount();

//更新丢失情况
    if (monitorList[WDG].isLost)
    {
        monitorList[WDG].isLost = 0;
        monitorList[WDG].worktime = monitorList[WDG].newTime;
    }

    //判断数据是否错误
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
//    //流水显示每组数据的错误码的情况
//    for (i = 0; i < ChassisPowerMotor4WDG; i++)
//    {
//       
////	每次检索间隔一段时间 分组显示12组数据传输情况

//    }
//}


//返回对应的设备是否存在错误
bool_t toe_is_error(uint8_t err)
{
    return (monitorList[err].errorExist == 1);
}
//错误列表指针数据传递
const monitor_t *getErrorListPoint(void)
{
    return monitorList;
}












