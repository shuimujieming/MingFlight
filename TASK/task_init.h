#ifndef __TASK_INIT_H
#define __TASK_INIT_H
/*********************************************
*@Copyright:  水木皆Ming
*@File:				task_init.h
*@Author:		  shuimujieming
*@Version:		1.0
*@Description:	
*@Log:				
*@Date:				2022-01-22
*********************************************/
#include "main.h"
extern EventGroupHandle_t EventGroupHandler;
void Task_Init();

//事件组标志位
#define GIMBAL_SIGNAL   ( 1 << 0 )
#define CHASSIS_SIGNAL ( 1 << 1 )
#define SHOOT_SIGNAL     ( 1 << 2 )
#define SUPERCAP_SIGNAL     ( 1 << 3 )

//开始任务
#define START_TASK_PRIO		1
#define START_STK_SIZE		256
//LED任务
#define LED_TASK_PRIO		2
#define LED_STK_SIZE		128
//模式状态更新任务
#define MODE_TASK_PRIO		3
#define MODE_STK_SIZE		256
//IMU数据获取任务
#define IMU_READ_TASK_PRIO		12
#define IMU_READ_STK_SIZE		256
//IMU数据预处理
#define IMU_PRETREAT_TASK_PRIO		13
#define IMU_PRETREAT_STK_SIZE		256
//传感器数据获取任务
#define SENSOR_READ_TASK_PRIO		8
#define SENSOR_READ_STK_SIZE		256
//传感器数据预处理任务
#define SENSOR_PRETREAT_TASK_PRIO		7
#define SENSOR_PRETREAT_STK_SIZE		256
//导航相关任务
#define NAVIGATION_TASK_PRIO  11
#define NAVIGATION_STK_SIZE   512
//飞行控制相关任务
#define FLIGHT_CONTROL_TASK_PRIO  10
#define FLIGHT_CONTROL_STK_SIZE   256
//上位机通信任务
#define COMMUNICATION_TASK_PRIO  6
#define COMMUNICATION_STK_SIZE   256

#endif