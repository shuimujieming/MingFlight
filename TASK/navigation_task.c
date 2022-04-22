#include "navigation_task.h"

TaskHandle_t Navigation_Task_Handler;


/**
 *@Function:		imu_task
 *@Description:	IMU����
 *@Param:       �β�
 *@Return:	  	����ֵ
 */
void navigation_task(void *p_arg)
{
	uint32_t NotifyValue = 0;

	while(1)
	{
		//��ȡIMU����Ԥ��������֪ͨ
		NotifyValue = ulTaskNotifyTake(pdTRUE,portMAX_DELAY);
		//���յ�����֪ͨ
		if(NotifyValue != 0)
		{

		}
		
				//����Ƶ�ʸ���
		Task_Freq_Update(NAVIGATION_TASK_FREQ);
	}

}

/**
 *@Function:		imu_task_create
 *@Description:	IMU���񴴽�
 *@Param:       �β�
 *@Return:	  	����ֵ
 */
void navigation_task_create()
{
	 xTaskCreate((TaskFunction_t )navigation_task,          //������
							(const char*    )"navigation_task",          //��������
							(uint16_t       )NAVIGATION_STK_SIZE,        //�����ջ��С
							(void*          )NULL,                //���ݸ��������Ĳ���
							(UBaseType_t    )NAVIGATION_TASK_PRIO,       //�������ȼ�
							(TaskHandle_t*  )&Navigation_Task_Handler);  //������  

}