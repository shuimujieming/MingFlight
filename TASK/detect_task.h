#ifndef __DETECT_TASK_H
#define __DETECT_TASK_H

#include "main.h"

enum Task_Freq_Enum
{
	LED_TASK_FREQ,				//0
	MODE_TASK_FREQ,				//1
	IMU_READ_TASK_FREQ,		//2
	IMU_PRETREAT_TASK_FREQ,	//3
	SENSOR_READ_TASK_FREQ,	//4
	SENSOR_PRETREAT_TASK_FREQ,	//5
	NAVIGATION_TASK_FREQ,				//6
	FLIGHT_CONTROL_TASK_FREQ,		//7
	COMMUNICATION_TASK_FREQ,		//8
	TASK_FREQ_NUM
} ;

void Task_Freq_Update(enum Task_Freq_Enum task_enum);

#endif