#include "detect_task.h"

TaskHandle_t Detect_Task_Handler;

//
u32 Task_Freq[TASK_FREQ_NUM];
u32 Task_Freq_Tick[TASK_FREQ_NUM];

/**
 *@Function:		Task_Freq_Update
 *@Description:	任务频率更新
 *@Param:       形参
 *@Return:	  	返回值
 */
void Task_Freq_Update(enum Task_Freq_Enum task_enum)
{
	Task_Freq[task_enum] = xTaskGetTickCount() - Task_Freq_Tick[task_enum];
	Task_Freq_Tick[task_enum] = xTaskGetTickCount();
}

/**
 *@Function:		imu_task
 *@Description:	IMU任务
 *@Param:       形参
 *@Return:	  	返回值
 */
void detect_task(void *p_arg)
{
	const TickType_t TimeIncrement=pdMS_TO_TICKS(5);
	TickType_t	PreviousWakeTime;
	PreviousWakeTime=xTaskGetTickCount();	
	//时间计数值—单位5ms
	static uint32_t count = 0;
	
	while(1)
	{		
		
		if(count % 2 == 0)	//100Hz
		{
			
		}
		if(count % 4 == 0)	//50Hz
		{
			
		}
		if(count % 10 == 0)	//20Hz
		{
			
		}		
		//200Hz
		
		//时间计数
		count++;
		vTaskDelayUntil(&PreviousWakeTime,TimeIncrement);	
	}

}

/**
 *@Function:		imu_task_create
 *@Description:	IMU任务创建
 *@Param:       形参
 *@Return:	  	返回值
// */
//void detect_task_create()
//{
//	 xTaskCreate((TaskFunction_t )detect_task,          //任务函数
//							(const char*    )"detect_task",          //任务名称
//							(uint16_t       )DETECT_STK_SIZE,        //任务堆栈大小
//							(void*          )NULL,                //传递给任务函数的参数
//							(UBaseType_t    )MODE_TASK_PRIO,       //任务优先级
//							(TaskHandle_t*  )&DETECT_Task_Handler);  //任务句柄  

//}