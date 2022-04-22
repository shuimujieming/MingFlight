#include "led.h" 
/*********************************************
*@Copyright:  水木皆Ming
*@File:				led.c
*@Author:		  shuimujieming
*@Version:		1.0
*@Description:	
*@Log:				
*@Date:				2022-01-22
*********************************************/

/**
 *@Function:		LED_Init
 *@Description:	LED初始化
 *@Param:       形参
 *@Return:	  	返回值
 */
void LED_Init()
{    	 
  GPIO_InitTypeDef  GPIO_InitStructure;

  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOH, ENABLE);

  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10 | GPIO_Pin_11 | GPIO_Pin_12;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
  GPIO_Init(GPIOH, &GPIO_InitStructure); 
	
	//灭灯
	GPIO_ResetBits(GPIOH,GPIO_Pin_10 | GPIO_Pin_11 | GPIO_Pin_12);
	
	

}

/**
 *@Function:		LED_Run
 *@Description:	LED运行
 *@Param:       形参
 *@Return:	  	返回值
 */
void LED_Run(void)
{
	GPIO_ToggleBits(GPIOH,GPIO_Pin_11);
}






