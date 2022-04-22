#include "sensor_task.h"

TaskHandle_t Sensor_Read_Task_Handler;
TaskHandle_t Sensor_PreTreat_Task_Handler;


float pressure,temperature,altitude;
int SR04_CC_Value = 0;

float Distance = 0;


/**
 *@Function:		imu_task
 *@Description:	IMU任务
 *@Param:       形参
 *@Return:	  	返回值
 */
void sensor_read_task(void *p_arg)
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
		//气压计高度数据获取
		if(count % 4 == 0)	//50Hz
		{
				//us转换成s，一般路程所以时间除以2
//				Distance = SR04_CC_Value / 1000000.0 / 2.0 * 340.0 ;
			bmp280GetData(&pressure,&temperature,&altitude);
		}
		if(count % 10 == 0)	//20Hz
		{
//			SR04_Get_Distance();
			
		}		
		//200Hz
				//任务频率更新
		Task_Freq_Update(SENSOR_READ_TASK_FREQ);
		
		//时间计数
		count++;
		vTaskDelayUntil(&PreviousWakeTime,TimeIncrement);	
	}

}


float velocity_z = 0.0;
float alt_last = 0.0;


uint64_t GetSysTimeUs(void)
{
    uint64_t ms;
    uint64_t value;
    ms = xTaskGetTickCount();
    value = ms * 1000 + (SysTick->LOAD - SysTick->VAL) * 1000 / SysTick->LOAD;
    return value;
}
/**
 *@Function:		imu_task
 *@Description:	IMU任务
 *@Param:       形参
 *@Return:	  	返回值
 */
void sensor_pretreat_task(void *p_arg)
{
	const TickType_t TimeIncrement=pdMS_TO_TICKS(5);
	TickType_t	PreviousWakeTime;
	PreviousWakeTime=xTaskGetTickCount();	
	//时间计数值—单位5ms
	static uint32_t count = 0;
	
	float lastTime = 0.0;
	
  float deltaT = 0.0;

	
	while(1)
	{

		
		
		if(count % 2 == 0)	//100Hz
		{
			
		}
		if(count % 8 == 0)	//25Hz
		{
			deltaT = (GetSysTimeUs() - lastTime) * 1e-6;
			lastTime = GetSysTimeUs();
			
			//气压变化高度速度
			velocity_z = velocity_z * 0.65 + ((altitude - alt_last) / deltaT) * 0.35;
			
			alt_last = altitude;
			
		}
		if(count % 20 == 0)	//10Hz
		{
			
		}		
		
		//200Hz
		//任务频率更新
		Task_Freq_Update(SENSOR_PRETREAT_TASK_FREQ);
		
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
void sensor_task_create()
{
	 xTaskCreate((TaskFunction_t )sensor_read_task,          //任务函数
							(const char*    )"sensor_read_task",          //任务名称
							(uint16_t       )SENSOR_READ_STK_SIZE,        //任务堆栈大小
							(void*          )NULL,                //传递给任务函数的参数
							(UBaseType_t    )SENSOR_READ_TASK_PRIO,       //任务优先级
							(TaskHandle_t*  )&Sensor_Read_Task_Handler);  //任务句柄  

	 xTaskCreate((TaskFunction_t )sensor_pretreat_task,          //任务函数
							(const char*    )"sensor_pretreat_task",          //任务名称
							(uint16_t       )SENSOR_PRETREAT_STK_SIZE,        //任务堆栈大小
							(void*          )NULL,                //传递给任务函数的参数
							(UBaseType_t    )SENSOR_PRETREAT_TASK_PRIO,       //任务优先级
							(TaskHandle_t*  )&Sensor_PreTreat_Task_Handler);  //任务句柄  

}