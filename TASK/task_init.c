#include "task_init.h"
/*********************************************
*@Copyright:  水木皆Ming
*@File:				task_init.c
*@Author:		  shuimujieming
*@Version:		1.0
*@Description:	
*@Log:				
*@Date:				2022-01-22
*********************************************/

void start_task(void *pvParameters);				
TaskHandle_t StartTask_Handler;							//开始任务句柄
EventGroupHandle_t EventGroupHandler;				//事件组句柄

/**
 *@Function:		start_task_create
 *@Description:	启动任务创建
 *@Param:       形参
 *@Return:	  	返回值
 */
void start_task_create()
{
		xTaskCreate((TaskFunction_t )start_task,            //任务函数
							(const char*    )"start_task",          //任务名称
							(uint16_t       )START_STK_SIZE,        //任务堆栈大小
							(void*          )NULL,                  //传递给任务函数的参数
							(UBaseType_t    )START_TASK_PRIO,       //任务优先级
							(TaskHandle_t*  )&StartTask_Handler);   //任务句柄              
		
	  vTaskStartScheduler();          //开启任务调度			
}
/**
 *@Function:		start_task
 *@Description:	启动任务
 *@Param:       形参
 *@Return:	  	返回值
 */
void start_task(void *pvParameters)
{
	
	EventGroupHandler = xEventGroupCreate();	//创建事件组

	while(1)
	{
		taskENTER_CRITICAL();	//进入临界区
		
		led_task_create();		//创建LED灯任务
				
		imu_task_create();		//创建IMU任务
		
		mode_task_create();		//创建模式任务
		
		sensor_task_create();	//创建传感器任务
		
		navigation_task_create(); //创建导航任务
		
		flight_control_task_create(); //创建飞行控制任务
		
		communication_task_create();  //创建上位机通信任务
		
		vTaskDelete(StartTask_Handler);	//删除开始任务
		
		taskEXIT_CRITICAL();	//退出临界区
	}
	
}
/**
 *@Function:		Task_Init
 *@Description:	任务初始化
 *@Param:       形参
 *@Return:	  	返回值
 */
void Task_Init()
{
	start_task_create();	//开始任务
}