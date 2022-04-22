#include "uav_task.h"


TaskHandle_t UAV_Task_Handler;
void uav_task(void *p_arg);

//��������ƺ���
void UAV_Control();
//�����������ʼ������
void UAV_Init();

/*
*@title��UAV���񴴽�
*@description��
*@param 1��	
*@param 2��	
*@return:��	
*/
void uav_task_create()
{
		xTaskCreate((TaskFunction_t )uav_task,          //������
							(const char*    )"uav_task",          //��������
							(uint16_t       )UAV_STK_SIZE,        //�����ջ��С
							(void*          )NULL,                //���ݸ��������Ĳ���
							(UBaseType_t    )UAV_TASK_PRIO,       //�������ȼ�
							(TaskHandle_t*  )&UAV_Task_Handler);  //������  
}
float rol_aim = 0.20;
float pit_aim = -0.80;
float yaw_aim = 0.0;
double throttle_aim = 0;

extern float Yaw_Angle;//�涯�����ֽǶ�

int lock_flag = 0;

/*float rol_aim = 1.12;
float pit_aim = 0.45;
float yaw_aim = 0.0;
*@title��UAV����
*@description��
*@param 1��	
*@param 2��	
*@return:��	
*/
void uav_task(void *p_arg)
{
	const TickType_t TimeIncrement=pdMS_TO_TICKS(2);
	TickType_t	PreviousWakeTime;
	PreviousWakeTime=xTaskGetTickCount();	
	
	//����Ĭ��˫��
	DBUS.RC.Switch_Left = RC_SW_DOWN;
	DBUS.RC.Switch_Right = RC_SW_DOWN;
	
	//���˻���ز�����ʼ��
	UAV_Init();
	
	while(1)
	{
		if(DBUS.RC.Switch_Left == RC_SW_DOWN || DBUS.RC.Switch_Right == RC_SW_DOWN || lock_flag == 1)
		{
			lock_flag = 1;
			
			//���ֹͣ
			TIM1->CCR1 = 1000;
			TIM1->CCR2 = 1000;
			TIM1->CCR3 = 1000;
			TIM1->CCR4 = 1000;		
			
			if(DBUS.RC.Switch_Right == RC_SW_UP)
			{
				//�����������Ҫ�������õ����λ
				if(abs(DBUS.RC.ch3) > 300)
				{
					//��������״̬
					lock_flag = 1;
				}
				else
				{
					//����
					lock_flag = 0;
					//������ʼ��
					UAV_Init();			
				}				
			}
			else
			{
				//�����������Ҫ�������õ����λ
				if(((DBUS.RC.ch3 + 660.0) / 1320.0f * 900.0) > 100)
				{
					//��������״̬
					lock_flag = 1;
				}
				else
				{
					//����
					lock_flag = 0;
					//������ʼ��
					UAV_Init();			
				}				
			}

		}
		else
		{
			UAV_Control();
		}
	
		vTaskDelayUntil(&PreviousWakeTime,TimeIncrement);	
	}

}
extern 	float bmp280_press;

//��������ʼ��
void UAV_Init()
{
	
	//�ǶȻָ�����ǰλ��
	rol_aim = 0.20;
	pit_aim = -0.80;
	yaw_aim = Yaw_Angle;
	throttle_aim = bmp280_press;
	
	//Roll Pitch Yaw
	for(int i = 0;i<3;i++)
	{	
		Pid_Reset(&PID_UAV_Angle[i]);
		Pid_Reset(&PID_UAV_Speed[i]);
		
		Pid_Reset(&PID_UAV_Throttle);
	}
	
	Pid_Set(&PID_UAV_Throttle,
	30.0f, 0.f, 0,
	500,100,500,500,1,0,0,0);
	
	
	
//	//Roll
//	Pid_Set(&PID_UAV_Angle[0],
//	1.2, 0.003, 3,
//	1000,50,1000,1000,1,0,0,0);
//	Pid_Set(&PID_UAV_Speed[0],
//	3.0, 0.001, 0,
//	1000,50,1000,1000,1,0,0,0);
//	
//	
//	//Pitch
//	Pid_Set(&PID_UAV_Angle[1],
//	1.2, 0.003, 3,
//	1000,50,1000,1000,1,0,0,0);
//	Pid_Set(&PID_UAV_Speed[1],
//	3.0, 0.001, 0,
//	1000,50,1000,1000,1,0,0,0);

	//Roll
	Pid_Set(&PID_UAV_Angle[0],
	1.0, 0.000, 0,
	1000,10,1000,1000,1,0,0,0);
	Pid_Set(&PID_UAV_Speed[0],
	3.0, 0.00, 0.5,
	1000,50,1000,1000,1,0,0,0);
	
	
	//Pitch
	Pid_Set(&PID_UAV_Angle[1],
	1.0, 0.000, 0,
	1000,10,1000,1000,1,0,0,0);
	Pid_Set(&PID_UAV_Speed[1],
	3.0, 0.00, 0.5,
	1000,50,1000,1000,1,0,0,0);
	
	//Yaw
	Pid_Set(&PID_UAV_Angle[2],
	2.5f, 0.f, 0.1f,
	1000,10,1000,1000,1,0,0,0);
	Pid_Set(&PID_UAV_Speed[2],
	1.0f, 0.003f, 0,
	1000,10,1000,1000,1,0,0,0);
}
//���˻�����
//X��
/*

  ǰ��
 0     1
  \   /
    *
	/   \
 3     2
	��

*/

//�������ٶ�
int UAV_Speed[4] = {0};
//�ٶȷ��������ţ�Roll��Pitch��Yaw��
int Throttle = 0,Roll_Out = 0,Pitch_Out = 0,Yaw_Out = 0;

void UAV_Control()
{
	
	Pid_Calc(&PID_UAV_Angle[0],imu.rol,rol_aim);
	Pid_Calc(&PID_UAV_Angle[1],imu.pit,pit_aim);
	Pid_Calc(&PID_UAV_Angle[2],Yaw_Angle,yaw_aim);
	
	Roll_Out  =	Pid_Calc(&PID_UAV_Speed[0],mpu_data.gx * 0.01f,PID_UAV_Angle[0].PIDout);
	Pitch_Out = Pid_Calc(&PID_UAV_Speed[1],mpu_data.gy * 0.01f,PID_UAV_Angle[1].PIDout);
	Yaw_Out   = Pid_Calc(&PID_UAV_Speed[2],mpu_data.gz * 0.01f,PID_UAV_Angle[2].PIDout);
	
	//�������ֵ�޶�
	Throttle  = (DBUS.RC.ch3 + 660.0) / 1320.0f * 900.0;
	
	if(DBUS.RC.Switch_Right == RC_SW_UP)
	{
		Throttle = Pid_Calc(&PID_UAV_Throttle,bmp280_press,throttle_aim);
	}
	
//	rol_aim = (DBUS.RC.ch0) / 660.0f * 12.0 + 1.24;
//	pit_aim = (DBUS.RC.ch1) / 660.0f * 12.0 + 0.28;
	rol_aim = (DBUS.RC.ch0) / 660.0f * 30.0 - 0.20;
	pit_aim = (DBUS.RC.ch1) / 660.0f * 30.0 - 1.00;
	yaw_aim = yaw_aim - DBUS.RC.ch2 * 0.0003;
	throttle_aim = throttle_aim + DBUS.RC.ch3*0.003;
	
{
	//����һ������ʱ����ɣ���ֹ��ǹ���
	if(Throttle > 100)
	{
		UAV_Speed[0] = Throttle + Roll_Out - Pitch_Out + Yaw_Out;
		UAV_Speed[1] = Throttle - Roll_Out - Pitch_Out - Yaw_Out;
		UAV_Speed[2] = Throttle - Roll_Out + Pitch_Out + Yaw_Out;
		UAV_Speed[3] = Throttle + Roll_Out + Pitch_Out - Yaw_Out;
	}
	else
	{
		UAV_Speed[0] = 150;
		UAV_Speed[1] = 150;
		UAV_Speed[2] = 150;
		UAV_Speed[3] = 150;
	}

}

	
	//�ٶ��޷�����
	//���ֵѰ��
	float Speed_Max = 0;

	for(int i = 0;i<4;i++)
	{
		if(abs(UAV_Speed[i]) > Speed_Max)
		{
			Speed_Max = abs(UAV_Speed[i]);
		}
	}
	//�ȱ����µ��ٶȣ��ٶ�����Ϊ1500����PWMֵΪ2500��
	if(Speed_Max > 1200)
	{
		for(int i = 0;i<4;i++)
		{
			UAV_Speed[i] *= 1200/Speed_Max;
		}
	}

	//�����1000��ʼ�ж���
	TIM1->CCR1 = 1000 + (UAV_Speed[0]);
	TIM1->CCR2 = 1000 + (UAV_Speed[1]);
	TIM1->CCR3 = 1000 + (UAV_Speed[2]);
	TIM1->CCR4 = 1000 + (UAV_Speed[3]);				
	
}