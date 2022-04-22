#ifndef __TASK_INIT_H
#define __TASK_INIT_H
/*********************************************
*@Copyright:  ˮľ��Ming
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

//�¼����־λ
#define GIMBAL_SIGNAL   ( 1 << 0 )
#define CHASSIS_SIGNAL ( 1 << 1 )
#define SHOOT_SIGNAL     ( 1 << 2 )
#define SUPERCAP_SIGNAL     ( 1 << 3 )

//��ʼ����
#define START_TASK_PRIO		1
#define START_STK_SIZE		256
//LED����
#define LED_TASK_PRIO		2
#define LED_STK_SIZE		128
//ģʽ״̬��������
#define MODE_TASK_PRIO		3
#define MODE_STK_SIZE		256
//IMU���ݻ�ȡ����
#define IMU_READ_TASK_PRIO		12
#define IMU_READ_STK_SIZE		256
//IMU����Ԥ����
#define IMU_PRETREAT_TASK_PRIO		13
#define IMU_PRETREAT_STK_SIZE		256
//���������ݻ�ȡ����
#define SENSOR_READ_TASK_PRIO		8
#define SENSOR_READ_STK_SIZE		256
//����������Ԥ��������
#define SENSOR_PRETREAT_TASK_PRIO		7
#define SENSOR_PRETREAT_STK_SIZE		256
//�����������
#define NAVIGATION_TASK_PRIO  11
#define NAVIGATION_STK_SIZE   512
//���п����������
#define FLIGHT_CONTROL_TASK_PRIO  10
#define FLIGHT_CONTROL_STK_SIZE   256
//��λ��ͨ������
#define COMMUNICATION_TASK_PRIO  6
#define COMMUNICATION_STK_SIZE   256

#endif