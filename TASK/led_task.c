#include "led_task.h"
/*********************************************
*@Copyright:  ˮľ��Ming
*@File:				led_task.c
*@Author:		  shuimujieming
*@Version:		1.0
*@Description:	
*@Log:				
*@Date:				2022-01-22
*********************************************/
TaskHandle_t LED_Task_Handler;
void led_task(void *p_arg);




/**
 *@Function:		led_task_create
 *@Description:	LED���񴴽�
 *@Param:       �β�
 *@Return:	  	����ֵ
 */
void led_task_create()
{
		xTaskCreate((TaskFunction_t )led_task,          //������
							(const char*    )"led_task",          //��������
							(uint16_t       )LED_STK_SIZE,        //�����ջ��С
							(void*          )NULL,                //���ݸ��������Ĳ���
							(UBaseType_t    )LED_TASK_PRIO,       //�������ȼ�
							(TaskHandle_t*  )&LED_Task_Handler);  //������  
}

/**
 *@Function:		led_task
 *@Description:	LED����
 *@Param:       �β�
 *@Return:	  	����ֵ
 */
void led_task(void *p_arg)
{
	int led_frequency = 0;
	
	vTaskDelay(2000);
	
	while(1)
	{		
		//led��Ƶ�ʿ���
		led_frequency++;
		
		//2Hz
		if(led_frequency % 25 == 0)
		{LED_Run();}
		
		//����������
		if(led_frequency == 25)
		{
			Beep_Ctrl(800,200);			
		}
		else if(led_frequency == 50)
		{
			Beep_Ctrl(500,100);						
		}
		else
		{
			Beep_Ctrl(500,0);									
		}	
		
		//����Ƶ�ʸ���
		Task_Freq_Update(LED_TASK_FREQ);
		
		vTaskDelay(20);
	}
}


