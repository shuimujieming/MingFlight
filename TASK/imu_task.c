#include "imu_task.h"
/*********************************************
*@Copyright:  水木皆Ming
*@File:				imu_task.c
*@Author:		  shuimujieming
*@Version:		1.0
*@Description:	
*@Log:				
*@Date:				2022-01-22
*********************************************/

TaskHandle_t IMU_Read_Task_Handler;
TaskHandle_t IMU_PreTreat_Task_Handler;

int freq = 0;

/**
 *@Function:		imu_task
 *@Description:	IMU任务
 *@Param:       形参
 *@Return:	  	返回值
 */
void imu_read_task(void *p_arg)
{
	const TickType_t TimeIncrement=pdMS_TO_TICKS(1);
	TickType_t	PreviousWakeTime;
	PreviousWakeTime=xTaskGetTickCount();	
	
	
	while(1)
	{	
		//陀螺仪数据更新
		BMI088_Get_Data();

		//发送任务通知
		xTaskNotifyGive(IMU_PreTreat_Task_Handler);
		
		//任务频率更新
		Task_Freq_Update(IMU_READ_TASK_FREQ);
		
		vTaskDelayUntil(&PreviousWakeTime,TimeIncrement);	
		
	}

}

extern TaskHandle_t Navigation_Task_Handler;
extern TaskHandle_t Flight_Control_Task_Handler;


/**
 *@Function:		imu_task
 *@Description:	IMU任务
 *@Param:       形参
 *@Return:	  	返回值
 */
void imu_pretreat_task(void *p_arg)
{

	uint32_t NotifyValue = 0;
	
	while(1)
	{	
		//获取IMU数据获取任务通知
		NotifyValue = ulTaskNotifyTake(pdTRUE,portMAX_DELAY);
		//接收到任务通知
		if(NotifyValue != 0)
		{
			//姿态解算
			IMU_Ahrs_Update();
			//欧拉角转化	
			IMU_Attitude_Update();
			
			
			//发送至导航相关任务通知
			xTaskNotifyGive(Navigation_Task_Handler);
			xTaskNotifyGive(Flight_Control_Task_Handler);
			
			//任务频率更新
			Task_Freq_Update(IMU_PRETREAT_TASK_FREQ);
		}
	}

}
/**
 *@Function:		imu_task_create
 *@Description:	IMU任务创建
 *@Param:       形参
 *@Return:	  	返回值
 */
void imu_task_create()
{
	 xTaskCreate((TaskFunction_t )imu_read_task,          //任务函数
							(const char*    )"imu_read_task",          //任务名称
							(uint16_t       )IMU_READ_STK_SIZE,        //任务堆栈大小
							(void*          )NULL,                //传递给任务函数的参数
							(UBaseType_t    )IMU_READ_TASK_PRIO,       //任务优先级
							(TaskHandle_t*  )&IMU_Read_Task_Handler);  //任务句柄  

	 xTaskCreate((TaskFunction_t )imu_pretreat_task,          //任务函数
							(const char*    )"imu_pretreat_task",          //任务名称
							(uint16_t       )IMU_PRETREAT_STK_SIZE,        //任务堆栈大小
							(void*          )NULL,                //传递给任务函数的参数
							(UBaseType_t    )IMU_PRETREAT_TASK_PRIO,       //任务优先级
							(TaskHandle_t*  )&IMU_PreTreat_Task_Handler);  //任务句柄  

}