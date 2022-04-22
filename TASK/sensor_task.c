#include "sensor_task.h"

TaskHandle_t Sensor_Read_Task_Handler;
TaskHandle_t Sensor_PreTreat_Task_Handler;


float pressure,temperature,altitude;
int SR04_CC_Value = 0;

float Distance = 0;


/**
 *@Function:		imu_task
 *@Description:	IMU����
 *@Param:       �β�
 *@Return:	  	����ֵ
 */
void sensor_read_task(void *p_arg)
{
	const TickType_t TimeIncrement=pdMS_TO_TICKS(5);
	TickType_t	PreviousWakeTime;
	PreviousWakeTime=xTaskGetTickCount();	
	//ʱ�����ֵ����λ5ms
	static uint32_t count = 0;
	
	while(1)
	{		
		
		if(count % 2 == 0)	//100Hz
		{
			
		}
		//��ѹ�Ƹ߶����ݻ�ȡ
		if(count % 4 == 0)	//50Hz
		{
				//usת����s��һ��·������ʱ�����2
//				Distance = SR04_CC_Value / 1000000.0 / 2.0 * 340.0 ;
			bmp280GetData(&pressure,&temperature,&altitude);
		}
		if(count % 10 == 0)	//20Hz
		{
//			SR04_Get_Distance();
			
		}		
		//200Hz
				//����Ƶ�ʸ���
		Task_Freq_Update(SENSOR_READ_TASK_FREQ);
		
		//ʱ�����
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
 *@Description:	IMU����
 *@Param:       �β�
 *@Return:	  	����ֵ
 */
void sensor_pretreat_task(void *p_arg)
{
	const TickType_t TimeIncrement=pdMS_TO_TICKS(5);
	TickType_t	PreviousWakeTime;
	PreviousWakeTime=xTaskGetTickCount();	
	//ʱ�����ֵ����λ5ms
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
			
			//��ѹ�仯�߶��ٶ�
			velocity_z = velocity_z * 0.65 + ((altitude - alt_last) / deltaT) * 0.35;
			
			alt_last = altitude;
			
		}
		if(count % 20 == 0)	//10Hz
		{
			
		}		
		
		//200Hz
		//����Ƶ�ʸ���
		Task_Freq_Update(SENSOR_PRETREAT_TASK_FREQ);
		
		//ʱ�����
		count++;
		vTaskDelayUntil(&PreviousWakeTime,TimeIncrement);	
	}
}

/**
 *@Function:		imu_task_create
 *@Description:	IMU���񴴽�
 *@Param:       �β�
 *@Return:	  	����ֵ
 */
void sensor_task_create()
{
	 xTaskCreate((TaskFunction_t )sensor_read_task,          //������
							(const char*    )"sensor_read_task",          //��������
							(uint16_t       )SENSOR_READ_STK_SIZE,        //�����ջ��С
							(void*          )NULL,                //���ݸ��������Ĳ���
							(UBaseType_t    )SENSOR_READ_TASK_PRIO,       //�������ȼ�
							(TaskHandle_t*  )&Sensor_Read_Task_Handler);  //������  

	 xTaskCreate((TaskFunction_t )sensor_pretreat_task,          //������
							(const char*    )"sensor_pretreat_task",          //��������
							(uint16_t       )SENSOR_PRETREAT_STK_SIZE,        //�����ջ��С
							(void*          )NULL,                //���ݸ��������Ĳ���
							(UBaseType_t    )SENSOR_PRETREAT_TASK_PRIO,       //�������ȼ�
							(TaskHandle_t*  )&Sensor_PreTreat_Task_Handler);  //������  

}