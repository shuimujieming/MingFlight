#include "detect_task.h"

TaskHandle_t Detect_Task_Handler;

//
u32 Task_Freq[TASK_FREQ_NUM];
u32 Task_Freq_Tick[TASK_FREQ_NUM];

/**
 *@Function:		Task_Freq_Update
 *@Description:	����Ƶ�ʸ���
 *@Param:       �β�
 *@Return:	  	����ֵ
 */
void Task_Freq_Update(enum Task_Freq_Enum task_enum)
{
	Task_Freq[task_enum] = xTaskGetTickCount() - Task_Freq_Tick[task_enum];
	Task_Freq_Tick[task_enum] = xTaskGetTickCount();
}

/**
 *@Function:		imu_task
 *@Description:	IMU����
 *@Param:       �β�
 *@Return:	  	����ֵ
 */
void detect_task(void *p_arg)
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
		if(count % 4 == 0)	//50Hz
		{
			
		}
		if(count % 10 == 0)	//20Hz
		{
			
		}		
		//200Hz
		
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
// */
//void detect_task_create()
//{
//	 xTaskCreate((TaskFunction_t )detect_task,          //������
//							(const char*    )"detect_task",          //��������
//							(uint16_t       )DETECT_STK_SIZE,        //�����ջ��С
//							(void*          )NULL,                //���ݸ��������Ĳ���
//							(UBaseType_t    )MODE_TASK_PRIO,       //�������ȼ�
//							(TaskHandle_t*  )&DETECT_Task_Handler);  //������  

//}