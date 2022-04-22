#include "imu_task.h"
/*********************************************
*@Copyright:  ˮľ��Ming
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
 *@Description:	IMU����
 *@Param:       �β�
 *@Return:	  	����ֵ
 */
void imu_read_task(void *p_arg)
{
	const TickType_t TimeIncrement=pdMS_TO_TICKS(1);
	TickType_t	PreviousWakeTime;
	PreviousWakeTime=xTaskGetTickCount();	
	
	
	while(1)
	{	
		//���������ݸ���
		BMI088_Get_Data();

		//��������֪ͨ
		xTaskNotifyGive(IMU_PreTreat_Task_Handler);
		
		//����Ƶ�ʸ���
		Task_Freq_Update(IMU_READ_TASK_FREQ);
		
		vTaskDelayUntil(&PreviousWakeTime,TimeIncrement);	
		
	}

}

extern TaskHandle_t Navigation_Task_Handler;
extern TaskHandle_t Flight_Control_Task_Handler;


/**
 *@Function:		imu_task
 *@Description:	IMU����
 *@Param:       �β�
 *@Return:	  	����ֵ
 */
void imu_pretreat_task(void *p_arg)
{

	uint32_t NotifyValue = 0;
	
	while(1)
	{	
		//��ȡIMU���ݻ�ȡ����֪ͨ
		NotifyValue = ulTaskNotifyTake(pdTRUE,portMAX_DELAY);
		//���յ�����֪ͨ
		if(NotifyValue != 0)
		{
			//��̬����
			IMU_Ahrs_Update();
			//ŷ����ת��	
			IMU_Attitude_Update();
			
			
			//�����������������֪ͨ
			xTaskNotifyGive(Navigation_Task_Handler);
			xTaskNotifyGive(Flight_Control_Task_Handler);
			
			//����Ƶ�ʸ���
			Task_Freq_Update(IMU_PRETREAT_TASK_FREQ);
		}
	}

}
/**
 *@Function:		imu_task_create
 *@Description:	IMU���񴴽�
 *@Param:       �β�
 *@Return:	  	����ֵ
 */
void imu_task_create()
{
	 xTaskCreate((TaskFunction_t )imu_read_task,          //������
							(const char*    )"imu_read_task",          //��������
							(uint16_t       )IMU_READ_STK_SIZE,        //�����ջ��С
							(void*          )NULL,                //���ݸ��������Ĳ���
							(UBaseType_t    )IMU_READ_TASK_PRIO,       //�������ȼ�
							(TaskHandle_t*  )&IMU_Read_Task_Handler);  //������  

	 xTaskCreate((TaskFunction_t )imu_pretreat_task,          //������
							(const char*    )"imu_pretreat_task",          //��������
							(uint16_t       )IMU_PRETREAT_STK_SIZE,        //�����ջ��С
							(void*          )NULL,                //���ݸ��������Ĳ���
							(UBaseType_t    )IMU_PRETREAT_TASK_PRIO,       //�������ȼ�
							(TaskHandle_t*  )&IMU_PreTreat_Task_Handler);  //������  

}