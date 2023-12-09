#include "main.h"
#include "start_task.h"


#define START_TASK_PRIO		1
#define START_STK_SIZE 		128
static TaskHandle_t StartTask_Handler;
void start_task(void *pvParameters);

#define IMU_TASK_PRIO 32
#define IMU_TASK_SIZE 512
static TaskHandle_t MPUTask_Handler;

#define CHASSIS_TASK_PRIO 18
#define CHASSIS_TASK_SIZE 256
static TaskHandle_t CHASSISTask_Handler;
void chassis_task(void *pvParameters);

#define JIGUANG_TASK_PRIO 19
#define JIGUANG_STK_SIZE 256
static TaskHandle_t JIGUANGTask_Handler;
void jiguang_task(void *pvParameters);

#define TX2_TASK_PRIO 20
#define TX2_TASK_SIZE 256
static TaskHandle_t TX2_TASK_Handler;

#define INTER_TASK_PRIO 15
#define INTER_TASK_SIZE 512
static TaskHandle_t INTER_TASK_Handler;

#define DETECT_TASK_PRIO 10
#define DETECT_TASK_SIZE 128
static TaskHandle_t DETECT_TASK_Handler;

void startTask(void)
{
    //创建开始任务
    xTaskCreate((TaskFunction_t )start_task,            //任务函数
                (const char*    )"start_task",          //任务名称
                (uint16_t       )START_STK_SIZE,        //任务堆栈大小
                (void*          )NULL,                  //传递给任务函数的参数
                (UBaseType_t    )START_TASK_PRIO,       //任务优先级
                (TaskHandle_t*  )&StartTask_Handler);   //任务句柄
    vTaskStartScheduler();          //开启任务调度
}



void start_task(void *pvParameters)
{
	    //陀螺仪任务
    xTaskCreate((TaskFunction_t )IMU_task,
                (const char*    )"IMU_task",
                (uint16_t       )IMU_TASK_SIZE,
                (void*          )NULL,
                (UBaseType_t    )IMU_TASK_PRIO,
                (TaskHandle_t*  )&MPUTask_Handler);
    //底盘任务
    xTaskCreate((TaskFunction_t )chassis_task,
                (const char*    )"chassis_task",
                (uint16_t       )CHASSIS_TASK_SIZE,
                (void*          )NULL,
                (UBaseType_t    )CHASSIS_TASK_PRIO,
                (TaskHandle_t*  )&CHASSISTask_Handler);
								
								
								
		xTaskCreate((TaskFunction_t )jiguang_task,             
                (const char*    )"jiguang_task",           
                (uint16_t       )JIGUANG_STK_SIZE,        
                (void*          )NULL,                  
                (UBaseType_t    )JIGUANG_TASK_PRIO,        
                (TaskHandle_t*  )&JIGUANGTask_Handler);  	

		//TX2任务
    xTaskCreate((TaskFunction_t )Tx2_task,
                (const char*    )"Tx2_task",
                (uint16_t       )TX2_TASK_SIZE,
                (void*          )NULL,
                (UBaseType_t    )TX2_TASK_PRIO,
                (TaskHandle_t*  )&TX2_TASK_Handler);
		//MINIPC发送任务
    xTaskCreate((TaskFunction_t )Interactive_task,
                (const char*    )"Interactive_task",
                (uint16_t       )INTER_TASK_SIZE,
                (void*          )NULL,
                (UBaseType_t    )INTER_TASK_PRIO,
                (TaskHandle_t*  )&INTER_TASK_Handler);
		//监测任务
    xTaskCreate((TaskFunction_t )detect_task,
                (const char*    )"detect_task",
                (uint16_t       )DETECT_TASK_SIZE,
                (void*          )NULL,
                (UBaseType_t    )DETECT_TASK_PRIO,
                (TaskHandle_t*  )&DETECT_TASK_Handler);
    vTaskDelete(StartTask_Handler); //删除开始任务
    taskEXIT_CRITICAL(); 
}

void BSP_Init(void)
{
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_4);//设置系统中断优先级分组4
	delay_init(168);					//初始化延时函数
	/*	串口初始化			*/
	Device_Usart1_ENABLE_Init(921600);
		/*	led初始化			*/
	LED_Init();
		//继电器
	RELAY_Init();
	fenwei_ON;
	PD4(ON);
	GPIO_ResetBits(GPIOE,GPIO_Pin_2);//
	GREEN_ACT_ON;
	delay_ms(500);
	GPIO_ResetBits(GPIOE,GPIO_Pin_3);
	GREEN_ACT_OFF;
	delay_ms(500);
	GPIO_SetBits(GPIOE,GPIO_Pin_4);//灯光
	GREEN_ACT_ON;
	delay_ms(500);
	GREEN_ACT_OFF;
	delay_ms(500);
	/*	超声波串口初始化			*/
	bizhang_Usart_init();
	/*	语音初始化	*/
		yuyin_Init();
	/*ros初始化*/
	ros_to_stm32_init(115200);
	/*	气缸初始化		*/
//	Air_Cylinder_Init();
	/*	电缸初始化		*/
//	Electric_actuator_Init();
	/*	定时器初始化	*/
	/*	84M/84=1Mhz(1,000,000)的计数频率,重装载值5000	频率为 1,000,000/250=4000hz*/
	Motor_PWM_Init(19999, 84 - 1);
	Motor_GPIO_Init();
//	TIM4_BAS_Init(10-1, 8400-1);
	/*	can初始化		波特率=48/((1+4+9)*3);		1M	*/
	CAN1_Mode_Init(CAN_SJW_1tq, CAN_BS2_4tq, CAN_BS1_9tq, 3, CAN_Mode_Normal);//1Mbps
	CAN2_Mode_Init(CAN_SJW_1tq, CAN_BS2_4tq, CAN_BS1_9tq, 3, CAN_Mode_Normal);
	/*	遥控器初始化	*/
	//5600
	A0_IIC_Init();
	A1_IIC_Init();
	A2_IIC_Init();
	A3_IIC_Init();
	remote_control_init();
	/*	陀螺仪初始化	*/
	  while(MPU6500_Init());
	  IMU_Calibration();
    accel_mat_init();
    ACCEL_Calibration();
}
