#include "led_task.h"
/*********************************************
*@Copyright:  水木皆Ming
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
 *@Description:	LED任务创建
 *@Param:       形参
 *@Return:	  	返回值
 */
void led_task_create()
{
		xTaskCreate((TaskFunction_t )led_task,          //任务函数
							(const char*    )"led_task",          //任务名称
							(uint16_t       )LED_STK_SIZE,        //任务堆栈大小
							(void*          )NULL,                //传递给任务函数的参数
							(UBaseType_t    )LED_TASK_PRIO,       //任务优先级
							(TaskHandle_t*  )&LED_Task_Handler);  //任务句柄  
}

/**
 *@Function:		led_task
 *@Description:	LED任务
 *@Param:       形参
 *@Return:	  	返回值
 */
void led_task(void *p_arg)
{
	int led_frequency = 0;
	
	vTaskDelay(2000);
	
	while(1)
	{		
		//led灯频率控制
		led_frequency++;
		
		//2Hz
		if(led_frequency % 25 == 0)
		{LED_Run();}
		
		//蜂鸣器响声
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
		
		//任务频率更新
		Task_Freq_Update(LED_TASK_FREQ);
		
		vTaskDelay(20);
	}
}


