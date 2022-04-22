#include "main.h"

/*********************************************
*@Copyright:  水木皆Ming
*@File:				main.c
*@Author:		  shuimujieming
*@Version:		1.0
*@Description:主函数文件	
*@Log:				2022.1.22简化主函数使用
*@Date:				2022-01-22
*********************************************/

/**
 *@Function:		main
 *@Description:	主函数
 *@Param:       形参
 *@Return:	  	返回值
 */
int main(void)
{ 
	
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_4);	//设置中断优先级分组为4
	
	Board_Init();																		//硬件资源驱动初始化
	
	Task_Init();																		//操作系统任务初始化
	
}