#ifndef __PID_H
#define __PID_H

#include "main.h"

typedef struct
{
	float P;
	float I;
	float D;
	float CurrentError;
	float LastError;
	float ErrorIgnored;
	float Pout;
	float Iout;
	float Dout;
	float PIDout;
	float PIDOutCompensate;
	float PMax;	
	float IMax;
	float DMax;
	float PIDOutMax;
	float PIDOutLast;
	float Target_Speed_Last;
	float Speed_Ratio;
	float Acceleration;
	char Motor_Type;
}PID_TypeDef;
extern PID_TypeDef PID_Chassis[4];
extern PID_TypeDef PID_Gimbal_Angle[2];
extern PID_TypeDef PID_Gimbal_Speed[2];
extern PID_TypeDef PID_Chassis_Omega;
extern PID_TypeDef PID_Shoot[2];
extern PID_TypeDef PID_Trigger;
extern PID_TypeDef PID_UAV_Angle[3];
extern PID_TypeDef PID_UAV_Speed[3];
extern PID_TypeDef PID_UAV_Throttle;

void Pid_Set(PID_TypeDef *PID,float Kp,float Ki,float Kd,float Pmax,float Imax,float Dmax,float PIDmax,float Speed_Ratio,float Acceletation,float Error_Ignored,u8 Motor_Type);
float Pid_Calc(PID_TypeDef *PID,float Current_Speed,float Target_Current);
void Pid_Reset(PID_TypeDef *PID);

#endif