#include "communication_task.h"

TaskHandle_t Communication_Task_Handler;
float sendbuf[6]={0};

/**
 *@Function:		imu_task
 *@Description:	IMU任务
 *@Param:       形参
 *@Return:	  	返回值
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
		
		//任务频率更新
		Task_Freq_Update(COMMUNICATION_TASK_FREQ);
		vTaskDelay(10);
	}
}

/**
 *@Function:		imu_task_create
 *@Description:	IMU任务创建
 *@Param:       形参
 *@Return:	  	返回值
 */
void communication_task_create()
{
	 xTaskCreate((TaskFunction_t )communication_task,          //任务函数
							(const char*    )"communication_task",          //任务名称
							(uint16_t       )COMMUNICATION_STK_SIZE,        //任务堆栈大小
							(void*          )NULL,                //传递给任务函数的参数
							(UBaseType_t    )COMMUNICATION_TASK_PRIO,       //任务优先级
							(TaskHandle_t*  )&Communication_Task_Handler);  //任务句柄  

}