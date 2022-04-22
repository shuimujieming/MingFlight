#include "navigation_task.h"

TaskHandle_t Navigation_Task_Handler;


/**
 *@Function:		imu_task
 *@Description:	IMU任务
 *@Param:       形参
 *@Return:	  	返回值
 */
void navigation_task(void *p_arg)
{
	uint32_t NotifyValue = 0;

	while(1)
	{
		//获取IMU数据预处理任务通知
		NotifyValue = ulTaskNotifyTake(pdTRUE,portMAX_DELAY);
		//接收到任务通知
		if(NotifyValue != 0)
		{

		}
		
				//任务频率更新
		Task_Freq_Update(NAVIGATION_TASK_FREQ);
	}

}

/**
 *@Function:		imu_task_create
 *@Description:	IMU任务创建
 *@Param:       形参
 *@Return:	  	返回值
 */
void navigation_task_create()
{
	 xTaskCreate((TaskFunction_t )navigation_task,          //任务函数
							(const char*    )"navigation_task",          //任务名称
							(uint16_t       )NAVIGATION_STK_SIZE,        //任务堆栈大小
							(void*          )NULL,                //传递给任务函数的参数
							(UBaseType_t    )NAVIGATION_TASK_PRIO,       //任务优先级
							(TaskHandle_t*  )&Navigation_Task_Handler);  //任务句柄  

}