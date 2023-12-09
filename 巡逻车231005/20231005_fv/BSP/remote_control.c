/**
  ****************************(C) COPYRIGHT 2020 NCIST****************************
  * @file       remote_control.c/h
  * @brief      遥控器处理，遥控器是通过类似SBUS的协议传输，利用DMA传输方式节约CPU
  *             资源，利用串口空闲中断来拉起处理函数，同时提供一些掉线重启DMA，串口
  *             的方式保证热插拔的稳定性。
  * @note       该任务是通过串口中断启动，不是freeRTOS任务
  * @history
  *  Version    Date            Author          Modification
  *  V1.0.0     Dec-26-2018     RM              1. 完成
  *
  @verbatim
  ==============================================================================

  ==============================================================================
  @endverbatim
  ****************************(C) COPYRIGHT 2020 NCIST****************************
  */
#include "Remote_Control.h"
#include "CAN_receive.h"
#include "detect_task.h"

//遥控器出错数据上限
#define RC_CHANNAL_ERROR_VALUE 700

RC_ctrl_t rc_ctrl;// static

//取正函数
static int16_t RC_abs(int16_t value);
//遥控器处理函数
static void SBUS_TO_RC(volatile const uint8_t *sbus_buf, RC_ctrl_t *rc_ctrl);
static void RC_MODE_CONTROL(RC_ctrl_t *rc_ctrl);
/*******************************************
遥控器控制变量
********************************************/



//接收原始数据，为18个字节，给了36个字节长度，防止DMA传输越界
static uint8_t SBUS_rx_buf[2][SBUS_RX_BUF_NUM];

//初始化DMA，串口3
void remote_control_init(void)
{
    RC_Init(SBUS_rx_buf[0], SBUS_rx_buf[1], SBUS_RX_BUF_NUM);
}
/*******************************************
返回遥控器控制变量，通过指针传递方式传递信息
********************************************/
const RC_ctrl_t *get_remote_control_point(void)
{
    return &rc_ctrl;
}
remote_mode_e INPUTMOD = RUN_STOP;   //输入模式设定
/*******************************************
键鼠控制变量
********************************************/
//按键标志位
uint8_t Q_Flag = 0;
uint8_t E_Flag = 0;
uint8_t F_Flag = 0;
uint8_t G_Flag = 0;
uint8_t V_Flag = 0;
uint8_t R_Flag = 0;
uint8_t MOU_LEFT_F = 0;
uint8_t MOU_RIGH_F = 0;
uint8_t W_Flag = 0;
uint8_t A_Flag = 0;
uint8_t S_Flag = 0;
uint8_t D_Flag = 0;
uint8_t Shift_Flag = 2;
uint8_t Ctrl_Flag = 0;
//判断遥控器数据是否出错，
uint8_t RC_data_is_error(void)
{
    //使用了go to语句 方便出错统一处理遥控器变量数据归零
    if (RC_abs(rc_ctrl.rc.ch[0]) > RC_CHANNAL_ERROR_VALUE)
    {
        goto error;
    }

    if (RC_abs(rc_ctrl.rc.ch[1]) > RC_CHANNAL_ERROR_VALUE)
    {
        goto error;
    }

    if (RC_abs(rc_ctrl.rc.ch[2]) > RC_CHANNAL_ERROR_VALUE)
    {
        goto error;
    }

    if (RC_abs(rc_ctrl.rc.ch[3]) > RC_CHANNAL_ERROR_VALUE)
    {
        goto error;
    }

    if (rc_ctrl.rc.s[0] == 0)
    {
        goto error;
    }

    if (rc_ctrl.rc.s[1] == 0)
    {
        goto error;
    }

    return 0;

error:
    rc_ctrl.rc.ch[0] = 0;
    rc_ctrl.rc.ch[1] = 0;
    rc_ctrl.rc.ch[2] = 0;
    rc_ctrl.rc.ch[3] = 0;
    rc_ctrl.rc.ch[4] = 0;
    rc_ctrl.rc.s[0] = RC_SW_DOWN;
    rc_ctrl.rc.s[1] = RC_SW_DOWN;
    rc_ctrl.mouse.x = 0;
    rc_ctrl.mouse.y = 0;
    rc_ctrl.mouse.z = 0;
    rc_ctrl.mouse.press_l = 0;
    rc_ctrl.mouse.press_r = 0;
    rc_ctrl.key.v = 0;
    return 1;
}
static void RC_data_clean(RC_ctrl_t *rc_ctrl)//掉线数据清零
{
    rc_ctrl->rc.ch[0] = 0;
    rc_ctrl->rc.ch[1] = 0;
    rc_ctrl->rc.ch[2] = 0;
    rc_ctrl->rc.ch[3] = 0;
    rc_ctrl->rc.ch[4] = 0;
    rc_ctrl->rc.s[0] = RC_SW_DOWN;
    rc_ctrl->rc.s[1] = RC_SW_DOWN;
    rc_ctrl->mouse.x = 0;
    rc_ctrl->mouse.y = 0;
    rc_ctrl->mouse.z = 0;
    rc_ctrl->mouse.press_l = 0;
    rc_ctrl->mouse.press_r = 0;
    rc_ctrl->key.v = 0;
}
void slove_RC_lost(void)
{
    RC_data_clean(&rc_ctrl);//掉线数据清零
    RC_restart(SBUS_RX_BUF_NUM);
}
void slove_data_error(void)
{
    RC_restart(SBUS_RX_BUF_NUM);
}

//串口中断
void USART2_IRQHandler(void)
{
    if (USART_GetITStatus(USART2, USART_IT_RXNE) != RESET)//检查指定的USART中断是否发生  接受中断
    {
        USART_ReceiveData(USART2);//
	
    }
    else if (USART_GetITStatus(USART2, USART_IT_IDLE) != RESET)//空闲中断
    {
//			LED2(ON);
        static uint16_t this_time_rx_len = 0;
        USART_ReceiveData(USART2);

        if(DMA_GetCurrentMemoryTarget(DMA1_Stream5) == 0)
        {
            //重新设置DMA
            DMA_Cmd(DMA1_Stream5, DISABLE);
            this_time_rx_len = SBUS_RX_BUF_NUM - DMA_GetCurrDataCounter(DMA1_Stream5);
            DMA_SetCurrDataCounter(DMA1_Stream5, SBUS_RX_BUF_NUM);
            DMA1_Stream5->CR |= DMA_SxCR_CT;
            //清DMA中断标志
            DMA_ClearFlag(DMA1_Stream5, DMA_FLAG_TCIF4 | DMA_FLAG_HTIF4);
            DMA_Cmd(DMA1_Stream5, ENABLE);

            if(this_time_rx_len == RC_FRAME_LENGTH)
            {
                //处理遥控器数据
                SBUS_TO_RC(SBUS_rx_buf[0], &rc_ctrl);
                RC_MODE_CONTROL( &rc_ctrl);
//                //记录数据接收时间
//                DetectHook(DBUSTOE);
            }
        }
        else
        {
//					LED2(OFF);
            //重新设置DMA
            DMA_Cmd(DMA1_Stream5, DISABLE);
            this_time_rx_len = SBUS_RX_BUF_NUM - DMA_GetCurrDataCounter(DMA1_Stream5);
            DMA_SetCurrDataCounter(DMA1_Stream5, SBUS_RX_BUF_NUM);
            DMA1_Stream5->CR &= ~(DMA_SxCR_CT);
            //清DMA中断标志
            DMA_ClearFlag(DMA1_Stream5, DMA_FLAG_TCIF4 | DMA_FLAG_HTIF4);
            DMA_Cmd(DMA1_Stream5, ENABLE);

            if(this_time_rx_len == RC_FRAME_LENGTH)
            {
                //处理遥控器数据
                SBUS_TO_RC(SBUS_rx_buf[1], &rc_ctrl);
                RC_MODE_CONTROL( &rc_ctrl);
                //记录数据接收时间
                DetectHook(DBUSTOE);
            }
        }
				USART_ClearITPendingBit(USART2, USART_IT_IDLE);
    }
}


//取正函数
static int16_t RC_abs(int16_t value)
{
    if (value > 0)
    {
        return value;
    }
    else
    {
        return -value;
    }
}
static void SBUS_TO_RC(volatile const uint8_t *sbus_buf, RC_ctrl_t *rc_ctrl)
{
    if (sbus_buf == NULL || rc_ctrl == NULL)
    {
        return;
    }

    rc_ctrl->rc.ch[0] = (sbus_buf[0] | (sbus_buf[1] << 8)) & 0x07ff;        //!< Channel 0
    rc_ctrl->rc.ch[1] = ((sbus_buf[1] >> 3) | (sbus_buf[2] << 5)) & 0x07ff; //!< Channel 1
    rc_ctrl->rc.ch[2] = ((sbus_buf[2] >> 6) | (sbus_buf[3] << 2) | (sbus_buf[4] << 10)) & 0x07ff;        //!< Channel 2
    rc_ctrl->rc.ch[3] = ((sbus_buf[4] >> 1) | (sbus_buf[5] << 7)) & 0x07ff; //!< Channel 3
    rc_ctrl->rc.s[0] = ((sbus_buf[5] >> 4) & 0x0003);                  //!< Switch left
    rc_ctrl->rc.s[1] = ((sbus_buf[5] >> 4) & 0x000C) >> 2;                       //!< Switch right
    rc_ctrl->mouse.x = sbus_buf[6] | (sbus_buf[7] << 8);                    //!< Mouse X axis
    rc_ctrl->mouse.y = sbus_buf[8] | (sbus_buf[9] << 8);                    //!< Mouse Y axis
    rc_ctrl->mouse.z = sbus_buf[10] | (sbus_buf[11] << 8);                  //!< Mouse Z axis
    rc_ctrl->mouse.press_l = sbus_buf[12];                                  //!< Mouse Left Is Press ?
    rc_ctrl->mouse.press_r = sbus_buf[13];                                  //!< Mouse Right Is Press ?
    rc_ctrl->key.v = sbus_buf[14] | (sbus_buf[15] << 8);                    //!< KeyBoard value
    rc_ctrl->rc.ch[4] = sbus_buf[16] | (sbus_buf[17] << 8);                 //NULL

    rc_ctrl->rc.ch[0] -= RC_CH_VALUE_OFFSET;
    rc_ctrl->rc.ch[1] -= RC_CH_VALUE_OFFSET;
    rc_ctrl->rc.ch[2] -= RC_CH_VALUE_OFFSET;
    rc_ctrl->rc.ch[3] -= RC_CH_VALUE_OFFSET;
    rc_ctrl->rc.ch[4] -= RC_CH_VALUE_OFFSET;
}

static void RC_MODE_CONTROL(RC_ctrl_t *rc_ctrl)
{
    if(rc_ctrl == NULL)
    {
        return;
    }

    if (rc_ctrl->rc.ch[0] == -660 && rc_ctrl->rc.ch[1] == -660 && rc_ctrl->rc.ch[2] == 660 && rc_ctrl->rc.ch[3] == -660) //拨杆内八软件强制复位
    {
        CAN_CMD_CHASSIS_POWER(0, 0, 0, 0);
				CAN_CMD_CHASSIS_Clamp(0);
				CAN_CMD_CHASSIS_Helm(0,0,0,0);

        __set_FAULTMASK(1);//关闭所有中断
        NVIC_SystemReset();//软件复位
    }

    else if (switch_is_mid(rc_ctrl->rc.s[ModeChannel_R]))//右中
    {
        INPUTMOD = REMOTE_INPUT;//遥控器控制
    }
    else if (switch_is_up(rc_ctrl->rc.s[ModeChannel_R]))//右上
    {
        INPUTMOD = KEYMOUSE_INPUT;//键盘控制
    }
    else if (switch_is_down(rc_ctrl->rc.s[ModeChannel_R]))//右下
    {
        INPUTMOD = RUN_STOP;//停止
    }

}
