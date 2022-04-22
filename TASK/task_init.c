#include "task_init.h"
/*********************************************
*@Copyright:  ˮľ��Ming
*@File:				task_init.c
*@Author:		  shuimujieming
*@Version:		1.0
*@Description:	
*@Log:				
*@Date:				2022-01-22
*********************************************/

void start_task(void *pvParameters);				
TaskHandle_t StartTask_Handler;							//��ʼ������
EventGroupHandle_t EventGroupHandler;				//�¼�����

/**
 *@Function:		start_task_create
 *@Description:	�������񴴽�
 *@Param:       �β�
 *@Return:	  	����ֵ
 */
void start_task_create()
{
		xTaskCreate((TaskFunction_t )start_task,            //������
							(const char*    )"start_task",          //��������
							(uint16_t       )START_STK_SIZE,        //�����ջ��С
							(void*          )NULL,                  //���ݸ��������Ĳ���
							(UBaseType_t    )START_TASK_PRIO,       //�������ȼ�
							(TaskHandle_t*  )&StartTask_Handler);   //������              
		
	  vTaskStartScheduler();          //�����������			
}
/**
 *@Function:		start_task
 *@Description:	��������
 *@Param:       �β�
 *@Return:	  	����ֵ
 */
void start_task(void *pvParameters)
{
	
	EventGroupHandler = xEventGroupCreate();	//�����¼���

	while(1)
	{
		taskENTER_CRITICAL();	//�����ٽ���
		
		led_task_create();		//����LED������
				
		imu_task_create();		//����IMU����
		
		mode_task_create();		//����ģʽ����
		
		sensor_task_create();	//��������������
		
		navigation_task_create(); //������������
		
		flight_control_task_create(); //�������п�������
		
		communication_task_create();  //������λ��ͨ������
		
		vTaskDelete(StartTask_Handler);	//ɾ����ʼ����
		
		taskEXIT_CRITICAL();	//�˳��ٽ���
	}
	
}
/**
 *@Function:		Task_Init
 *@Description:	�����ʼ��
 *@Param:       �β�
 *@Return:	  	����ֵ
 */
void Task_Init()
{
	start_task_create();	//��ʼ����
}