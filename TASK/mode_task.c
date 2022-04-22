#include "mode_task.h"

TaskHandle_t Mode_Task_Handler;

enum Flight_Mode_Enum FlightMode;


void Mode_Switch()
{
		//����ģʽ
		if(DBUS.RC.Switch_Left == RC_SW_DOWN || DBUS.RC.Switch_Right == RC_SW_DOWN)
		{
			FlightMode = Flight_Mode_Locked;
		}
		
		if(!(FlightMode == Flight_Mode_Locked && (DBUS.RC.ch3 + 660) > 50))
		{
			//�ֶ�ģʽ
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
 *@Description:	IMU����
 *@Param:       �β�
 *@Return:	  	����ֵ
 */
void mode_task(void *p_arg)
{
	const TickType_t TimeIncrement=pdMS_TO_TICKS(5);
	TickType_t	PreviousWakeTime;
	PreviousWakeTime=xTaskGetTickCount();	
	//ʱ�����ֵ����λ5ms
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
		//ģʽ�л�
		Mode_Switch();
		if(DR16_Signal < 0)
		{
			FlightMode = Flight_Mode_Locked;
		}
		//����Ƶ�ʸ���
		Task_Freq_Update(MODE_TASK_FREQ);
		
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
void mode_task_create()
{
	 xTaskCreate((TaskFunction_t )mode_task,          //������
							(const char*    )"mode_task",          //��������
							(uint16_t       )MODE_STK_SIZE,        //�����ջ��С
							(void*          )NULL,                //���ݸ��������Ĳ���
							(UBaseType_t    )MODE_TASK_PRIO,       //�������ȼ�
							(TaskHandle_t*  )&Mode_Task_Handler);  //������  

}