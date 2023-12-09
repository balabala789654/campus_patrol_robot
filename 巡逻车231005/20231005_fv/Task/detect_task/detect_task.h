/**
  **********************************(C) NCIST ************************************
  * @file       detect_task.c/h
  * @brief      �豸�����ж�����ͨ��freeRTOS�δ�ʱ����Ϊϵͳʱ�䣬�豸��ȡ���ݺ�
  *             ����DetectHook��¼��Ӧ�豸��ʱ�䣬�ڸ������ͨ���жϼ�¼ʱ����ϵͳ
  *             ʱ��֮�����жϵ��ߡ�
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
#ifndef _DETECT_TASK_H_
#define _DETECT_TASK_H_
#include "main.h"

typedef unsigned char bool_t;
//�������Լ���Ӧ�豸˳��
enum errorList
{
    TX2DataTOE = 0,
    DBUSTOE,

    ChassisHelmMotor1TOE,
    ChassisHelmMotor2TOE,
    ChassisHelmMotor3TOE,
    ChassisHelmMotor4TOE,

    ChassisPowerMotor1TOE,
    ChassisPowerMotor2TOE,
    ChassisPowerMotor3TOE,
    ChassisPowerMotor4TOE,

    ClampMotorTOE,

    errorListLength,
};
//�������Լ���Ӧ�豸˳��
enum monitorList
{
    TX2DataWGD = 0,
    DBUSWDG,
    ChassisHelmMotor1WDG,
    ChassisHelmMotor2WDG,
    ChassisHelmMotor3WDG,
    ChassisHelmMotor4WDG,
    ChassisPowerMotor1WDG,
    ChassisPowerMotor2WDG,
    ChassisPowerMotor3WDG,
    ChassisPowerMotor4WDG,
    ClampMotorWDG,

    monitorLength,
};

typedef __packed struct
{
    uint32_t newTime;
    uint32_t lastTime;
    uint32_t Losttime;
    uint32_t worktime;
    uint16_t setOfflineTime : 12;
    uint16_t setOnlineTime : 12;
    uint8_t  enable : 1;
    uint8_t  Priority : 4;
    uint8_t  errorExist : 1;
    uint8_t  isLost : 1;
    uint8_t  dataIsError : 1;

    fp32 frequency;

    bool_t (*dataIsErrorFun)(void);//�ж����ݴ��� -- �˺����� char�ͷ���ֵ
    void (*solveLostFun)(void);//�����ʧ
    void (*solveDataErrorFun)(void);//������ݴ���
} monitor_t;
extern monitor_t monitorList[monitorLength + 1];
extern const monitor_t *getErrorListPoint(void);
extern bool_t toe_is_error(uint8_t err);
extern void DetectHook(uint8_t WDG);
extern void detect_task(void  *pvParameters);
//static void DetectInit(uint32_t time);
#endif



























