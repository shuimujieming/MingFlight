#ifndef __MODE_TASK_H
#define __MODE_TASK_H

#include "main.h"


enum Flight_Mode_Enum
{
	Flight_Mode_Locked,	//上锁
	Flight_Mode_Manual,	//手动（自稳）
	Flight_Mode_AltHold, //定高
	Flight_Mode_FixedPoint //定点
};


void mode_task_create();
extern enum Flight_Mode_Enum FlightMode;

#endif