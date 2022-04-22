#include "board.h"

/*********************************************
*@Copyright:  水木皆Ming
*@File:				board.c
*@Author:		  shuimujieming
*@Version:		1.0
*@Description:硬件资源初始化
*@Log:				
*@Date:				2022-01-22
*********************************************/
/**
 *@Function:		Board_Init
 *@Description:	硬件资源驱动初始化
 *@Param:       形参
 *@Return:	  	返回值
 */
void Board_Init(void)
{
	delay_init(168);		  //初始化延时函数

	LED_Init();		        //初始化LED端口
	
	DR16_Init();					//遥控器接收机初始化
	
	BMI088_Init();				//BMI088初始化

	TIM6_Init();					//定时器6初始化
	
	Beep_Init();					//蜂鸣器初始化
	
	Rotor_Init();					//电调PWM初始化
	
	User_USART_Init();		//用户串口初始化
	
	BMP_IIC_Init();				//BMP280初始化
	
	bmp280Init();					//气压计初始化

//	UPFLOW_Init();
}