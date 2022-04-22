#include "board.h"

/*********************************************
*@Copyright:  ˮľ��Ming
*@File:				board.c
*@Author:		  shuimujieming
*@Version:		1.0
*@Description:Ӳ����Դ��ʼ��
*@Log:				
*@Date:				2022-01-22
*********************************************/
/**
 *@Function:		Board_Init
 *@Description:	Ӳ����Դ������ʼ��
 *@Param:       �β�
 *@Return:	  	����ֵ
 */
void Board_Init(void)
{
	delay_init(168);		  //��ʼ����ʱ����

	LED_Init();		        //��ʼ��LED�˿�
	
	DR16_Init();					//ң�������ջ���ʼ��
	
	BMI088_Init();				//BMI088��ʼ��

	TIM6_Init();					//��ʱ��6��ʼ��
	
	Beep_Init();					//��������ʼ��
	
	Rotor_Init();					//���PWM��ʼ��
	
	User_USART_Init();		//�û����ڳ�ʼ��
	
	BMP_IIC_Init();				//BMP280��ʼ��
	
	bmp280Init();					//��ѹ�Ƴ�ʼ��

//	UPFLOW_Init();
}