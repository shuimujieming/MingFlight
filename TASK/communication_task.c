#include "communication_task.h"

TaskHandle_t Communication_Task_Handler;
float sendbuf[6]={0};

/**
 *@Function:		imu_task
 *@Description:	IMU����
 *@Param:       �β�
 *@Return:	  	����ֵ
 */
void communication_task(void *p_arg)
{
	while(1)
	{
//		Optflow_Prase();
		User_Send_Data();
//		sendbuf[0]=opt_origin_data.pixel_flow_x_integral;
//		sendbuf[1]=opt_origin_data.pixel_flow_y_integral;
//		sendbuf[2]=opt_filter_data.x;
//		sendbuf[3]=opt_filter_data.y;
//		sendbuf[4]=opt_gyro_data.x;
		
		//����Ƶ�ʸ���
		Task_Freq_Update(COMMUNICATION_TASK_FREQ);
		vTaskDelay(10);
	}
}

/**
 *@Function:		imu_task_create
 *@Description:	IMU���񴴽�
 *@Param:       �β�
 *@Return:	  	����ֵ
 */
void communication_task_create()
{
	 xTaskCreate((TaskFunction_t )communication_task,          //������
							(const char*    )"communication_task",          //��������
							(uint16_t       )COMMUNICATION_STK_SIZE,        //�����ջ��С
							(void*          )NULL,                //���ݸ��������Ĳ���
							(UBaseType_t    )COMMUNICATION_TASK_PRIO,       //�������ȼ�
							(TaskHandle_t*  )&Communication_Task_Handler);  //������  

}