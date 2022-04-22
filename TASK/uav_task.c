#include "uav_task.h"


TaskHandle_t UAV_Task_Handler;
void uav_task(void *p_arg);

//四旋翼控制函数
void UAV_Control();
//四旋翼参数初始化函数
void UAV_Init();

/*
*@title：UAV任务创建
*@description：
*@param 1：	
*@param 2：	
*@return:：	
*/
void uav_task_create()
{
		xTaskCreate((TaskFunction_t )uav_task,          //任务函数
							(const char*    )"uav_task",          //任务名称
							(uint16_t       )UAV_STK_SIZE,        //任务堆栈大小
							(void*          )NULL,                //传递给任务函数的参数
							(UBaseType_t    )UAV_TASK_PRIO,       //任务优先级
							(TaskHandle_t*  )&UAV_Task_Handler);  //任务句柄  
}
float rol_aim = 0.20;
float pit_aim = -0.80;
float yaw_aim = 0.0;
double throttle_aim = 0;

extern float Yaw_Angle;//随动，积分角度

int lock_flag = 0;

/*float rol_aim = 1.12;
float pit_aim = 0.45;
float yaw_aim = 0.0;
*@title：UAV任务
*@description：
*@param 1：	
*@param 2：	
*@return:：	
*/
void uav_task(void *p_arg)
{
	const TickType_t TimeIncrement=pdMS_TO_TICKS(2);
	TickType_t	PreviousWakeTime;
	PreviousWakeTime=xTaskGetTickCount();	
	
	//开机默认双下
	DBUS.RC.Switch_Left = RC_SW_DOWN;
	DBUS.RC.Switch_Right = RC_SW_DOWN;
	
	//无人机相关参数初始化
	UAV_Init();
	
	while(1)
	{
		if(DBUS.RC.Switch_Left == RC_SW_DOWN || DBUS.RC.Switch_Right == RC_SW_DOWN || lock_flag == 1)
		{
			lock_flag = 1;
			
			//电机停止
			TIM1->CCR1 = 1000;
			TIM1->CCR2 = 1000;
			TIM1->CCR3 = 1000;
			TIM1->CCR4 = 1000;		
			
			if(DBUS.RC.Switch_Right == RC_SW_UP)
			{
				//上锁后解锁需要将油门置到最低位
				if(abs(DBUS.RC.ch3) > 300)
				{
					//保持上锁状态
					lock_flag = 1;
				}
				else
				{
					//解锁
					lock_flag = 0;
					//参数初始化
					UAV_Init();			
				}				
			}
			else
			{
				//上锁后解锁需要将油门置到最低位
				if(((DBUS.RC.ch3 + 660.0) / 1320.0f * 900.0) > 100)
				{
					//保持上锁状态
					lock_flag = 1;
				}
				else
				{
					//解锁
					lock_flag = 0;
					//参数初始化
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

//参数啊初始化
void UAV_Init()
{
	
	//角度恢复至当前位置
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
//无人机控制
//X型
/*

  前方
 0     1
  \   /
    *
	/   \
 3     2
	后方

*/

//四旋翼浆速度
int UAV_Speed[4] = {0};
//速度分量（油门，Roll，Pitch，Yaw）
int Throttle = 0,Roll_Out = 0,Pitch_Out = 0,Yaw_Out = 0;

void UAV_Control()
{
	
	Pid_Calc(&PID_UAV_Angle[0],imu.rol,rol_aim);
	Pid_Calc(&PID_UAV_Angle[1],imu.pit,pit_aim);
	Pid_Calc(&PID_UAV_Angle[2],Yaw_Angle,yaw_aim);
	
	Roll_Out  =	Pid_Calc(&PID_UAV_Speed[0],mpu_data.gx * 0.01f,PID_UAV_Angle[0].PIDout);
	Pitch_Out = Pid_Calc(&PID_UAV_Speed[1],mpu_data.gy * 0.01f,PID_UAV_Angle[1].PIDout);
	Yaw_Out   = Pid_Calc(&PID_UAV_Speed[2],mpu_data.gz * 0.01f,PID_UAV_Angle[2].PIDout);
	
	//油门最大值限定
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
	//低于一定油门时不起飞，防止倾角过大
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

	
	//速度限幅调整
	//最大值寻找
	float Speed_Max = 0;

	for(int i = 0;i<4;i++)
	{
		if(abs(UAV_Speed[i]) > Speed_Max)
		{
			Speed_Max = abs(UAV_Speed[i]);
		}
	}
	//等比例下调速度（速度上限为1500，即PWM值为2500）
	if(Speed_Max > 1200)
	{
		for(int i = 0;i<4;i++)
		{
			UAV_Speed[i] *= 1200/Speed_Max;
		}
	}

	//电调从1000开始有动力
	TIM1->CCR1 = 1000 + (UAV_Speed[0]);
	TIM1->CCR2 = 1000 + (UAV_Speed[1]);
	TIM1->CCR3 = 1000 + (UAV_Speed[2]);
	TIM1->CCR4 = 1000 + (UAV_Speed[3]);				
	
}