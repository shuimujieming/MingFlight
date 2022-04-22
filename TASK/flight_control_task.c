#include "flight_control_task.h"


TaskHandle_t Flight_Control_Task_Handler;

float rol_aim = 0.20;
float pit_aim = -0.80;
float yaw_aim = 0.0;
double throttle_aim = 0;

extern float Yaw_Angle;//随动，积分角度

extern 	float pressure,temperature,altitude;

int stay_status = 0;
int stay_flag = 0;
//参数啊初始化
void UAV_Init()
{
	//角度恢复至当前位置
	rol_aim = 0.20;
	pit_aim = -0.80;
	yaw_aim = Yaw_Angle;
	throttle_aim = altitude;
	
	//Roll Pitch Yaw
	for(int i = 0;i<3;i++)
	{	
		Pid_Reset(&PID_UAV_Angle[i]);
		Pid_Reset(&PID_UAV_Speed[i]);
		
		Pid_Reset(&PID_UAV_Throttle);
	}
	
	Pid_Set(&PID_UAV_Throttle,
	10.0f, 0.f, 0,
	500,100,500,500,1,0,0,0);

	//Roll
	Pid_Set(&PID_UAV_Angle[0],
	0.6, 0.00, 0,
	1000,10,1000,1000,1,0,0,0);
	Pid_Set(&PID_UAV_Speed[0],
	1.8, 0.001, 0.8,
	10000,50,10000,400,1,0,0,0);
	
	
	//Pitch
	Pid_Set(&PID_UAV_Angle[1],
	0.6, 0.00, 0,
	1000,10,1000,1000,1,0,0,0);
	Pid_Set(&PID_UAV_Speed[1],
	1.8, 0.001, 0.8,
	10000,50,10000,400,1,0,0,0);
	
	//Yaw
	Pid_Set(&PID_UAV_Angle[2],
	2.0f, 0.f, 0.3f,
	1000,10,1000,1000,1,0,0,0);
	Pid_Set(&PID_UAV_Speed[2],
	3.0f, 0.00f, 0.0,
	10000,50,10000,400,1,0,0,0);
	
	TIM1->CCR1 = 1000;
	TIM1->CCR2 = 1000;
	TIM1->CCR3 = 1000;
	TIM1->CCR4 = 1000;		
	
	
		stay_status = -1;
		stay_flag = -1;
	
}

//四旋翼浆速度
int UAV_Speed[4] = {0};
//速度分量（油门，Roll，Pitch，Yaw）
int Throttle = 0,Roll_Out = 0,Pitch_Out = 0,Yaw_Out = 0;

float Pid_Calc_Anlge(PID_TypeDef *PID, float Current_Speed, float Target_Speed , float mpu_g)
{   	
	Current_Speed = Current_Speed / PID->Speed_Ratio;

	PID->Target_Speed_Last = Target_Speed;

	//死区忽略
	PID->CurrentError = Target_Speed - Current_Speed;
	if (fabs(PID->CurrentError) < PID->ErrorIgnored )  PID->CurrentError = 0;
	
	//Pout  
	PID->Pout = PID->P * PID->CurrentError;
	if(PID->Pout >  PID->PMax)  PID->Pout =  PID->PMax;
	if(PID->Pout < -PID->PMax)  PID->Pout = -PID->PMax;
	
	//Iout
	PID->Iout += PID->I * PID->CurrentError;
	if(PID->Iout >  PID->IMax) PID->Iout =  PID->IMax;
	if(PID->Iout < -PID->IMax) PID->Iout = -PID->IMax;
	
	//Dout 
	PID->Dout = PID->D * mpu_g;
	if(PID->Dout >  PID->DMax) PID->Dout =  PID->DMax;
	if(PID->Dout < -PID->DMax) PID->Dout = -PID->DMax;
	
	//PIDout
	PID->PIDout = PID->Pout + PID->Iout + PID->Dout + PID->PIDOutCompensate;

	
	if(PID->PIDout >  PID->PIDOutMax) PID->PIDout =  PID->PIDOutMax;
	if(PID->PIDout < -PID->PIDOutMax) PID->PIDout = -PID->PIDOutMax;

  PID->PIDOutLast = PID->PIDout;
	PID->LastError  = PID->CurrentError;
	
	return PID->PIDout;    
}



void UAV_Control()
{
	
	
//	Pid_Calc_Anlge(&PID_UAV_Angle[0],imu.rol,rol_aim,mpu_data.gx*0.01f);
//	Pid_Calc_Anlge(&PID_UAV_Angle[1],imu.pit,pit_aim,mpu_data.gy*0.01f);
//	Pid_Calc_Anlge(&PID_UAV_Angle[2],Yaw_Angle,yaw_aim,mpu_data.gz*0.01f);
//	
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
		Throttle = 300 + Pid_Calc(&PID_UAV_Throttle,altitude,throttle_aim);
	}
	
	rol_aim = (DBUS.RC.ch0) / 660.0f * 30.0 -2.5;
	pit_aim = (DBUS.RC.ch1) / 660.0f * 30.0 -1.0;
	yaw_aim = yaw_aim - DBUS.RC.ch2 * 0.0003;
	
	if(abs(DBUS.RC.ch3) > 200)
	{
		if(DBUS.RC.ch3 > 0)
		{
			throttle_aim = throttle_aim + (DBUS.RC.ch3 - 200)*0.00001;								
		}
		else
		{
			throttle_aim = throttle_aim + (DBUS.RC.ch3 + 200)*0.00001;					
		}
	}
	
{
	//低于一定油门时不起飞，防止倾角过大
	if(Throttle > 150)
	{
		UAV_Speed[0] = Throttle + Roll_Out - Pitch_Out + Yaw_Out;
		UAV_Speed[1] = Throttle - Roll_Out - Pitch_Out - Yaw_Out;
		UAV_Speed[2] = Throttle - Roll_Out + Pitch_Out + Yaw_Out;
		UAV_Speed[3] = Throttle + Roll_Out + Pitch_Out - Yaw_Out;
		
		

	}
	else
	{

		stay_status ++;
	
		if(stay_flag == -1)
		{
	
			if(stay_status < 600)
			{
		UAV_Speed[0] = 150;
		UAV_Speed[1] = 150;
		UAV_Speed[2] = 150;
		UAV_Speed[3] = 150;						
			}
			else
			{
				stay_flag = 0;
			
			}
		}
		
		if(stay_flag == 0)
		{
	
			if(stay_status < 800)
			{
		UAV_Speed[0] = 300;
		UAV_Speed[1] = 300;
		UAV_Speed[2] = 300;
		UAV_Speed[3] = 300;						
			}
			else
			{
				stay_flag = 1;
			
			}
		}
		
		if(stay_flag == 1)
		{
	
			if(stay_status < 1000)
			{
		UAV_Speed[0] = 150;
		UAV_Speed[1] = 150;
		UAV_Speed[2] = 150;
		UAV_Speed[3] = 150;						
			}
			else
			{
				stay_flag = 2;
			
			}
		}
				
		if(stay_flag == 2)
		{
	
			if(stay_status  < 1200)
			{
		UAV_Speed[0] = 300;
		UAV_Speed[1] = 300;
		UAV_Speed[2] = 300;
		UAV_Speed[3] = 300;						
			}
			else
			{
				stay_flag = 3;
			
			}
		}	
		else if(stay_flag == 3)
		{
			UAV_Speed[0] = 150;
		UAV_Speed[1] = 150;
		UAV_Speed[2] = 150;
		UAV_Speed[3] = 150;		
		}
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
	if(Speed_Max > 1500)
	{
		for(int i = 0;i<4;i++)
		{
			UAV_Speed[i] *= 1500/Speed_Max;
		}
	}
	
	for(int i =0;i<4;i++)
	{
		if(UAV_Speed[i] < 0)
		{
			UAV_Speed[i] = 0;
		}
	}

	//电调从1000开始有动力
	TIM1->CCR1 = 1000 + (UAV_Speed[0]);
	TIM1->CCR2 = 1000 + (UAV_Speed[1]);
	TIM1->CCR3 = 1000 + (UAV_Speed[2]);
	TIM1->CCR4 = 1000 + (UAV_Speed[3]);				
	
}
extern enum Flight_Mode_Enum FlightMode;

/**
 *@Function:		imu_task
 *@Description:	IMU任务
 *@Param:       形参
 *@Return:	  	返回值
 */
void flight_control_task(void *p_arg)
{
	uint32_t NotifyValue = 0;
	//时间计数值—单位1ms
	static uint32_t count = 0;
	
	UAV_Init();
	
		
	
	vTaskDelay(1000);
	
	while(1)
	{
		//获取IMU数据预处理任务通知
		NotifyValue = ulTaskNotifyTake(pdTRUE,portMAX_DELAY);
		//接收到任务通知
		if(NotifyValue != 0)
		{
			if(count % 2 == 0)	//500Hz
			{
				
			}
			if(count % 5 == 0)	//200Hz
			{
				
			}
			if(count % 10 == 0)	//100Hz
			{
				
			}		
			
			if(FlightMode == Flight_Mode_Locked)
			{
				UAV_Init();
			}
			else if(FlightMode != Flight_Mode_Locked)
			{
				UAV_Control();	
			}
			
			count ++;
		}
		//任务频率更新
		Task_Freq_Update(FLIGHT_CONTROL_TASK_FREQ);
	}

}

/**
 *@Function:		imu_task_create
 *@Description:	IMU任务创建
 *@Param:       形参
 *@Return:	  	返回值
 */
void flight_control_task_create()
{
	 xTaskCreate((TaskFunction_t )flight_control_task,          //任务函数
							(const char*    )"flight_control_task",          //任务名称
							(uint16_t       )FLIGHT_CONTROL_STK_SIZE,        //任务堆栈大小
							(void*          )NULL,                //传递给任务函数的参数
							(UBaseType_t    )FLIGHT_CONTROL_TASK_PRIO,       //任务优先级
							(TaskHandle_t*  )&Flight_Control_Task_Handler);  //任务句柄  

}