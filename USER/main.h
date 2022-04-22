#ifndef __MAIN_H
#define __MAIN_H

/*********************************************
*@Copyright:  水木皆Ming
*@File:				main.h
*@Author:		  shuimujieming
*@Version:		1.0
*@Description:	
*@Log:				
*@Date:				2022-01-22
*********************************************/

#include "sys.h"
#include "stdio.h"	
#include <string.h>
#include <math.h>
#include <stdlib.h>



typedef signed char int8_t;
typedef signed short int int16_t;
typedef signed int int32_t;
typedef signed long long int64_t;

/* exact-width unsigned integer types */
typedef unsigned char uint8_t;
typedef unsigned short int uint16_t;
typedef unsigned int uint32_t;
typedef unsigned long long uint64_t;
typedef unsigned char bool_t;
typedef float fp32;
typedef double fp64;

typedef uint32_t  u32;
typedef uint16_t u16;
typedef uint8_t  u8;


#include "FreeRTOS.h"
#include "task.h"
#include "event_groups.h"

#include "task_init.h"
#include "led_task.h"
#include "imu_task.h"
#include "mode_task.h"
#include "sensor_task.h"
#include "navigation_task.h"
#include "flight_control_task.h"
#include "communication_task.h"
#include "detect_task.h"


//用于打印的串口配置
#define USART_PRINT USART6

//基础配置
#include "board.h"
#include "delay.h"
#include "usart.h"
#include "led.h"
#include "dr16.h"
#include "tim6.h"
#include "pid.h"
#include "spi.h"
#include "imu.h"
#include "beep.h"
#include "bmi088.h"
#include "bmi088driver.h"
#include "rotor.h"
#include "bmp280.h"
#include "neo6m.h"
#include "sr04.h"
#include "upflow.h"


#include "kalman.h"

#endif