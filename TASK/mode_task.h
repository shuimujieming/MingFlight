#ifndef __MODE_TASK_H
#define __MODE_TASK_H

#include "main.h"


enum Flight_Mode_Enum
{
	Flight_Mode_Locked,	//����
	Flight_Mode_Manual,	//�ֶ������ȣ�
	Flight_Mode_AltHold, //����
	Flight_Mode_FixedPoint //����
};


void mode_task_create();
extern enum Flight_Mode_Enum FlightMode;

#endif