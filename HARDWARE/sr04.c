#include "sr04.h"


//��������ʼ��
void SR04_Init()
{
	GPIO_InitTypeDef  GPIO_InitStructure;
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	TIM_ICInitTypeDef  TIM_ICInitStructure;
   EXTI_InitTypeDef EXTI_InitStructure;

  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC, ENABLE);
  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOI, ENABLE);
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM8, ENABLE);

	//PC6--TRIG
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
  GPIO_Init(GPIOC, &GPIO_InitStructure); 
	
	//PI6--ECHO
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_DOWN; 
  GPIO_Init(GPIOI, &GPIO_InitStructure); 	
	
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_SYSCFG, ENABLE);//ʹ��SYSCFGʱ��
	
	SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOI, EXTI_PinSource6);//PD1 ���ӵ��ж���1
	
	EXTI_ClearITPendingBit(EXTI_Line6);
	EXTI_InitStructure.EXTI_Line = EXTI_Line6;
	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt; 
	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising_Falling;//////�����غ��½��ش�����ʽ
	EXTI_InitStructure.EXTI_LineCmd = ENABLE;
	EXTI_Init(&EXTI_InitStructure);  	

	
	//ʱ�ӳ�ʼ��
	TIM_TimeBaseStructure.TIM_Prescaler=168-1;  //��ʱ����Ƶ
	TIM_TimeBaseStructure.TIM_CounterMode=TIM_CounterMode_Up; //���ϼ���ģʽ
	TIM_TimeBaseStructure.TIM_Period=10000-1;   //�Զ���װ��ֵ
	TIM_TimeBaseStructure.TIM_ClockDivision=TIM_CKD_DIV1; 
	TIM_TimeBaseInit(TIM8,&TIM_TimeBaseStructure);

	
	//������ʱ���ж�
	TIM_ITConfig(TIM8,TIM_IT_Update,ENABLE);
	TIM_Cmd(TIM8,ENABLE);


	NVIC_InitTypeDef NVIC_InitStructure;
	NVIC_InitStructure.NVIC_IRQChannel = TIM8_UP_TIM13_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
	
	NVIC_InitStructure.NVIC_IRQChannel = EXTI9_5_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
}

int CC_Rising_Flag = 0;
extern int SR04_CC_Value;
int cap_success = 1;
uint32_t ms10 = 0;

//���벶���жϴ���
void TIM8_UP_TIM13_IRQHandler(void)
{
	if(TIM_GetITStatus(TIM8,TIM_IT_Update)!=RESET)
	{
	
		ms10++;
		TIM_ClearITPendingBit(TIM8,TIM_IT_Update);
		
	}

}
u32 Last_HC_Time=0;
u32 HC_Time=0;
u32 HC_Time_Delta=1000;//����������ʱ��
void EXTI9_5_IRQHandler(void)
{
  if (EXTI_GetITStatus(EXTI_Line6) != RESET)
	{
        if (GPIO_ReadInputDataBit(GPIOI,GPIO_Pin_6)!= 0) 
				{  
            Last_HC_Time=10000*ms10+TIM8->CNT; //�ߵ�ƽ��ʼʱ��
        } 
				else 
				{
            HC_Time=10000*ms10+TIM8->CNT;      //�ߵ�ƽ����ʱ��
            HC_Time_Delta=HC_Time-Last_HC_Time; //���������������յ�ʱ��
					
						cap_success = 1;
         }
  }
  EXTI_ClearITPendingBit(EXTI_Line6);
}


void SR04_Get_Distance()
{
	static uint32_t last_time = 0;
	
	if(cap_success == 1)
	{
		//��������������
		PCout(6) = 1;
		delay_us(20);
		PCout(6) = 0;
		last_time = xTaskGetTickCount();		
		cap_success = 0;
	}
}