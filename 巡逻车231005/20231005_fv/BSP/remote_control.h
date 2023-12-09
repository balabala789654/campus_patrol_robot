/**
  ****************************(C) COPYRIGHT 2016 DJI****************************
  * @file       remote_control.c/h
  * @brief      遥控器处理，遥控器是通过类似SBUS的协议传输，利用DMA传输方式节约CPU
  *             资源，利用串口空闲中断来拉起处理函数，同时提供一些掉线重启DMA，串口
  *             的方式保证热插拔的稳定性。
  * @note
  * @history
  *  Version    Date            Author          Modification
  *  V1.0.0     Dec-26-2018     RM              1. 完成
  *
  @verbatim
  ==============================================================================

  ==============================================================================
  @endverbatim
  ****************************(C) COPYRIGHT 2016 DJI****************************
  */
#ifndef REMOTE_CONTROL_H
#define REMOTE_CONTROL_H
#include "stm32f4xx.h"
#include "rc.h"
#include <string.h>
#include "delay.h"
#define rc_deadline_limit(input, output, dealine)        \
    {                                                    \
        if ((input) > (dealine) || (input) < -(dealine)) \
        {                                                \
            (output) = (input);                          \
        }                                                \
        else                                             \
        {                                                \
            (output) = 0;                                \
        }                                                \
    }

#define SBUS_RX_BUF_NUM 36u

#define RC_FRAME_LENGTH 18u

#define RC_CH_VALUE_MIN ((uint16_t)364)
#define RC_CH_VALUE_OFFSET ((uint16_t)1024)
#define RC_CH_VALUE_MAX ((uint16_t)1684)

/* ----------------------- RC Switch Definition----------------------------- */
#define RC_SW_UP ((uint16_t)1)
#define RC_SW_MID ((uint16_t)3)
#define RC_SW_DOWN ((uint16_t)2)
#define switch_is_down(s) (s == RC_SW_DOWN)
#define switch_is_mid(s) (s == RC_SW_MID)
#define switch_is_up(s) (s == RC_SW_UP)
/* ----------------------- PC Key Definition-------------------------------- */


//S1 S2 拨杆
#define 	ModeChannel_R 		0
#define 	ModeChannel_L 		1


//按键标志位
extern uint8_t Q_Flag;
extern uint8_t E_Flag;
extern uint8_t F_Flag;
extern uint8_t G_Flag;
extern uint8_t V_Flag;
extern uint8_t R_Flag;
extern uint8_t B_Flag;
extern uint8_t MOU_LEFT_F;//射击
extern uint8_t MOU_RIGH_F;//自瞄
extern uint8_t Shift_Flag;
extern uint8_t Ctrl_Flag;
extern uint8_t W_Flag;
extern uint8_t A_Flag;
extern uint8_t S_Flag;
extern uint8_t D_Flag;
/* ----------------------- Data Struct ------------------------------------- */
typedef __packed struct
{
    __packed struct
    {
        int16_t ch[5];
        char s[2];
    } rc;
    __packed struct
    {
        int16_t x;
        int16_t y;
        int16_t z;
        uint8_t press_l;
        uint8_t press_r;
    } mouse;
    __packed struct
    {
        uint16_t v;
    } key;

} RC_ctrl_t;
//输入模式枚举：遥控器/键盘鼠标/停止运行
typedef enum
{
    RUN_STOP = 1,
    KEYMOUSE_INPUT = 2,
    REMOTE_INPUT = 3,

} remote_mode_e;

extern RC_ctrl_t rc_ctrl;

#define KEY_PRESSED_OFFSET_W ((uint16_t)1 << 0)
#define KEY_PRESSED_OFFSET_S ((uint16_t)1 << 1)
#define KEY_PRESSED_OFFSET_A ((uint16_t)1 << 2)
#define KEY_PRESSED_OFFSET_D ((uint16_t)1 << 3)
#define KEY_PRESSED_OFFSET_SHIFT ((uint16_t)1 << 4)//加速
#define KEY_PRESSED_OFFSET_CTRL ((uint16_t)1 << 5)//慢速
#define KEY_PRESSED_OFFSET_Q ((uint16_t)1 << 6)
#define KEY_PRESSED_OFFSET_E ((uint16_t)1 << 7)
#define KEY_PRESSED_OFFSET_R ((uint16_t)1 << 8)//换弹
#define KEY_PRESSED_OFFSET_F ((uint16_t)1 << 9)//小陀螺
#define KEY_PRESSED_OFFSET_G ((uint16_t)1 << 10)//射符
#define KEY_PRESSED_OFFSET_Z ((uint16_t)1 << 11)
#define KEY_PRESSED_OFFSET_X ((uint16_t)1 << 12)
#define KEY_PRESSED_OFFSET_C ((uint16_t)1 << 13)
#define KEY_PRESSED_OFFSET_V ((uint16_t)1 << 14)//
#define KEY_PRESSED_OFFSET_B ((uint16_t)1 << 15)
/* 检测鼠标按键状态
   按下为1，没按下为0*/
#define    IF_MOUSE_PRESSED_LEFT    (rc_ctrl.mouse.press_l == 1)
#define    IF_MOUSE_PRESSED_RIGH    (rc_ctrl.mouse.press_r == 1)


/* 检测键盘按键状态
   若对应按键被按下，则逻辑表达式的值为1，否则为0 */
#define    IF_KEY_PRESSED         (  rc_ctrl.key.v  )
#define    IF_KEY_PRESSED_W       ( (rc_ctrl.key.v & KEY_PRESSED_OFFSET_W)    != 0 )
#define    IF_KEY_PRESSED_S       ( (rc_ctrl.key.v & KEY_PRESSED_OFFSET_S)    != 0 )
#define    IF_KEY_PRESSED_A       ( (rc_ctrl.key.v & KEY_PRESSED_OFFSET_A)    != 0 )
#define    IF_KEY_PRESSED_D       ( (rc_ctrl.key.v & KEY_PRESSED_OFFSET_D)    != 0 )
#define    IF_KEY_PRESSED_Q       ( (rc_ctrl.key.v & KEY_PRESSED_OFFSET_Q)    != 0 )
#define    IF_KEY_PRESSED_E       ( (rc_ctrl.key.v & KEY_PRESSED_OFFSET_E)    != 0 )
#define    IF_KEY_PRESSED_G       ( (rc_ctrl.key.v & KEY_PRESSED_OFFSET_G)    != 0 )
#define    IF_KEY_PRESSED_X       ( (rc_ctrl.key.v & KEY_PRESSED_OFFSET_X)    != 0 )
#define    IF_KEY_PRESSED_Z       ( (rc_ctrl.key.v & KEY_PRESSED_OFFSET_Z)    != 0 )
#define    IF_KEY_PRESSED_C       ( (rc_ctrl.key.v & KEY_PRESSED_OFFSET_C)    != 0 )
#define    IF_KEY_PRESSED_B       ( (rc_ctrl.key.v & KEY_PRESSED_OFFSET_B)    != 0 )
#define    IF_KEY_PRESSED_V       ( (rc_ctrl.key.v & KEY_PRESSED_OFFSET_V)    != 0 )
#define    IF_KEY_PRESSED_F       ( (rc_ctrl.key.v & KEY_PRESSED_OFFSET_F)    != 0 )
#define    IF_KEY_PRESSED_R       ( (rc_ctrl.key.v & KEY_PRESSED_OFFSET_R)    != 0 )
#define    IF_KEY_PRESSED_CTRL    ( (rc_ctrl.key.v & KEY_PRESSED_OFFSET_CTRL) != 0 )
#define    IF_KEY_PRESSED_SHIFT   ( (rc_ctrl.key.v & KEY_PRESSED_OFFSET_SHIFT) != 0 )

/* ----------------------- Internal Data ----------------------------------- */

extern void remote_control_init(void);
extern const RC_ctrl_t *get_remote_control_point(void);
extern uint8_t RC_data_is_error(void);
extern void slove_RC_lost(void);
extern void slove_data_error(void);
static void RC_data_clean(RC_ctrl_t *rc_ctrl);
#endif
