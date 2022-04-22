#include "mode_task.h"

TaskHandle_t Mode_Task_Handler;

enum Flight_Mode_Enum FlightMode;


void Mode_Switch()
{
		//上锁模式
		if(DBUS.RC.Switch_Left == RC_SW_DOWN || DBUS.RC.Switch_Right == RC_SW_DOWN)
		{
			FlightMode = Flight_Mode_Locked;
		}
		
		if(!(FlightMode == Flight_Mode_Locked && (DBUS.RC.ch3 + 660) > 50))
		{
			//手动模式
			if(DBUS.RC.Switch_Left == RC_SW_MID && DBUS.RC.Switch_Right == RC_SW_MID)
			{
				FlightMode = Flight_Mode_Manual;			
			}
			else if(DBUS.RC.Switch_Left == RC_SW_MID && DBUS.RC.Switch_Right == RC_SW_UP)
			{
				FlightMode = Flight_Mode_AltHold;						
			}
			else if(DBUS.RC.Switch_Left == RC_SW_UP && DBUS.RC.Switch_Right == RC_SW_MID)
			{
				FlightMode = Flight_Mode_FixedPoint;								
			}
		}		
}
extern int DR16_Signal;

/**
 *@Function:		imu_task
 *@Description:	IMU任务
 *@Param:       形参
 *@Return:	  	返回值
 */
void mode_task(void *p_arg)
{
	const TickType_t TimeIncrement=pdMS_TO_TICKS(5);
	TickType_t	PreviousWakeTime;
	PreviousWakeTime=xTaskGetTickCount();	
	//时间计数值—单位5ms
	static uint32_t count = 0;
	
	while(1)
	{		
		DR16_Signal --;
		
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
		//模式切换
		Mode_Switch();
		if(DR16_Signal < 0)
		{
			FlightMode = Flight_Mode_Locked;
		}
		//任务频率更新
		Task_Freq_Update(MODE_TASK_FREQ);
		
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
 */
void mode_task_create()
{
	 xTaskCreate((TaskFunction_t )mode_task,          //任务函数
							(const char*    )"mode_task",          //任务名称
							(uint16_t       )MODE_STK_SIZE,        //任务堆栈大小
							(void*          )NULL,                //传递给任务函数的参数
							(UBaseType_t    )MODE_TASK_PRIO,       //任务优先级
							(TaskHandle_t*  )&Mode_Task_Handler);  //任务句柄  

}