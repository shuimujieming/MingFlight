#include "main.h"

/*********************************************
*@Copyright:  ˮľ��Ming
*@File:				main.c
*@Author:		  shuimujieming
*@Version:		1.0
*@Description:�������ļ�	
*@Log:				2022.1.22��������ʹ��
*@Date:				2022-01-22
*********************************************/

/**
 *@Function:		main
 *@Description:	������
 *@Param:       �β�
 *@Return:	  	����ֵ
 */
int main(void)
{ 
	
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_4);	//�����ж����ȼ�����Ϊ4
	
	Board_Init();																		//Ӳ����Դ������ʼ��
	
	Task_Init();																		//����ϵͳ�����ʼ��
	
}