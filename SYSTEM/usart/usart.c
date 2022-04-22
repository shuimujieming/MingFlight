#include "usart.h"	
////////////////////////////////////////////////////////////////////////////////// 	 
//如果使用ucos,则包括下面的头文件即可.
#if SYSTEM_SUPPORT_OS
#include "FreeRTOS.h"					//ucos 使用	  
#endif
//////////////////////////////////////////////////////////////////////////////////	 
//本程序只供学习使用，未经作者许可，不得用于其它任何用途
//ALIENTEK STM32F4探索者开发板
//串口1初始化		   
//正点原子@ALIENTEK
//技术论坛:www.openedv.com
//修改日期:2014/6/10
//版本：V1.5
//版权所有，盗版必究。
//Copyright(C) 广州市星翼电子科技有限公司 2009-2019
//All rights reserved
//********************************************************************************
//V1.3修改说明 
//支持适应不同频率下的串口波特率设置.
//加入了对printf的支持
//增加了串口接收命令功能.
//修正了printf第一个字符丢失的bug
//V1.4修改说明
//1,修改串口初始化IO的bug
//2,修改了USART_RX_STA,使得串口最大接收字节数为2的14次方
//3,增加了USART_REC_LEN,用于定义串口最大允许接收的字节数(不大于2的14次方)
//4,修改了EN_USART6_RX的使能方式
//V1.5修改说明
//1,增加了对UCOSII的支持
////////////////////////////////////////////////////////////////////////////////// 	  
 

//////////////////////////////////////////////////////////////////
//加入以下代码,支持printf函数,而不需要选择use MicroLIB	  
#if 1
#pragma import(__use_no_semihosting)             
//标准库需要的支持函数                 
struct __FILE 
{ 
	int handle; 
}; 

FILE __stdout;       
//定义_sys_exit()以避免使用半主机模式    
void _sys_exit(int x) 
{ 
	x = x; 
} 
//重定义fputc函数 
int fputc(int ch, FILE *f)
{ 	
	while((USART_PRINT->SR&0X40)==0);//循环发送,直到发送完毕   
	USART_PRINT->DR = (u8) ch;      
	return ch;
}
#endif
 
#if EN_USART6_RX   //如果使能了接收
//串口1中断服务程序
//注意,读取USARTx->SR能避免莫名其妙的错误   	
u8 USART_RX_BUF[USART_REC_LEN];     //接收缓冲,最大USART_REC_LEN个字节.
//接收状态
//bit15，	接收完成标志
//bit14，	接收到0x0d
//bit13~0，	接收到的有效字节数目
u16 USART_RX_STA=0;       //接收状态标记	




#define RCC_AHBPeriphClockCmd_GPIO_User RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOG,ENABLE)
#define RCC_APBPeriphClockCmd_USART_User RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART6,ENABLE)
#define GPIO_User GPIOG
#define GPIO_PinSource_User_1 GPIO_PinSource9
#define GPIO_PinSource_User_2 GPIO_PinSource14
#define GPIO_AF_USART_User GPIO_AF_USART6

#define GPIO_Pin_User_1 GPIO_Pin_9
#define GPIO_Pin_User_2 GPIO_Pin_14

#define USART_User USART6
#define USART_User_IRQn USART6_IRQn
#define USART_User_IRQHandler USART6_IRQHandler

#define RCC_AHBPeriphClockCmd_DMA_Referee RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_DMA2,ENABLE)
#define DMA_Channel_Referee_RX DMA_Channel_5
#define DMA_Stream_Referee_RX DMA2_Stream1

#define User_TX_SIZE 44
#define User_RX_SIZE 38

uint8_t User_tx_buff[User_TX_SIZE];
uint8_t User_rx_buff[2][User_RX_SIZE];


void User_USART_Init()
{
	//GPIO端口设置
	GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;

	RCC_AHBPeriphClockCmd_GPIO_User; //使能GPIOB时钟
	RCC_APBPeriphClockCmd_USART_User;//使能USART1时钟

	GPIO_PinAFConfig(GPIO_User,GPIO_PinSource_User_1,GPIO_AF_USART_User); 
	GPIO_PinAFConfig(GPIO_User,GPIO_PinSource_User_2,GPIO_AF_USART_User); 


	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_User_1|GPIO_Pin_User_2; 
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;//复用功能
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;	//速度50MHz
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP; //推挽复用输出
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP; //上拉
	GPIO_Init(GPIO_User,&GPIO_InitStructure);

	//USART1初始化设置
	USART_InitStructure.USART_BaudRate = 115200;//波特率设置
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;//字长为8位数据格式
	USART_InitStructure.USART_StopBits = USART_StopBits_1;//一个停止位
	USART_InitStructure.USART_Parity = USART_Parity_No;//
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//无硬件数据流控制
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;	//收发模式
	USART_Init(USART_User, &USART_InitStructure); //初始化串口3

	USART_ClearFlag(USART_User, USART_FLAG_IDLE);
	USART_ITConfig(USART_User, USART_IT_IDLE, ENABLE);
	
	USART_Cmd(USART_User, ENABLE);  //
	
	
	NVIC_InitTypeDef NVIC_InitStructure;
	NVIC_InitStructure.NVIC_IRQChannel = DMA2_Stream6_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
	
	NVIC_InitStructure.NVIC_IRQChannel = USART6_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
	
	DMA_InitTypeDef DMA_InitStructure;
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_DMA2,ENABLE);
	//USART6_TX
	DMA_InitStructure.DMA_Channel = DMA_Channel_5;
	DMA_InitStructure.DMA_PeripheralBaseAddr = (uint32_t) & (USART6->DR);
	DMA_InitStructure.DMA_Memory0BaseAddr = (uint32_t)User_tx_buff;
	DMA_InitStructure.DMA_DIR = DMA_DIR_MemoryToPeripheral;
	DMA_InitStructure.DMA_BufferSize = User_TX_SIZE;
	DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
	DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;
	DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;
	DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte;
	DMA_InitStructure.DMA_Mode = DMA_Mode_Normal;
	DMA_InitStructure.DMA_Priority = DMA_Priority_VeryHigh;
	DMA_InitStructure.DMA_FIFOMode = DMA_FIFOMode_Disable;
	DMA_InitStructure.DMA_FIFOThreshold = DMA_FIFOThreshold_1QuarterFull;
	DMA_InitStructure.DMA_MemoryBurst = DMA_MemoryBurst_Single;
	DMA_InitStructure.DMA_PeripheralBurst = DMA_PeripheralBurst_Single;
	DMA_Init(DMA2_Stream6, &DMA_InitStructure);
	
	USART_DMACmd(USART_User, USART_DMAReq_Tx, ENABLE);
	DMA_ITConfig(DMA2_Stream6,DMA_IT_TC,ENABLE);
	
	
	DMA_Cmd(DMA2_Stream6, ENABLE);
	
	
	DMA_InitStructure.DMA_Channel = DMA_Channel_5;
	DMA_InitStructure.DMA_PeripheralBaseAddr = (uint32_t) & (USART6->DR);
	DMA_InitStructure.DMA_Memory0BaseAddr = (uint32_t)User_rx_buff[0];
	DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralToMemory;
	DMA_InitStructure.DMA_BufferSize = User_RX_SIZE;
	DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
	DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;
	DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;
	DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte;
	DMA_InitStructure.DMA_Mode = DMA_Mode_Circular;
	DMA_InitStructure.DMA_Priority = DMA_Priority_VeryHigh;
	DMA_InitStructure.DMA_FIFOMode = DMA_FIFOMode_Disable;
	DMA_InitStructure.DMA_FIFOThreshold = DMA_FIFOThreshold_1QuarterFull;
	DMA_InitStructure.DMA_MemoryBurst = DMA_MemoryBurst_Single;
	DMA_InitStructure.DMA_PeripheralBurst = DMA_PeripheralBurst_Single;
	DMA_Init(DMA2_Stream1, &DMA_InitStructure);
	
	DMA_DoubleBufferModeConfig(DMA2_Stream1, (uint32_t)User_rx_buff[1], DMA_Memory_0);
	DMA_DoubleBufferModeCmd(DMA2_Stream1, ENABLE);
	
	USART_DMACmd(USART_User, USART_DMAReq_Rx, ENABLE);	
	
	DMA_Cmd(DMA2_Stream1, ENABLE);
}
typedef struct 
{
	int16_t P;
	int16_t I;
	int16_t D;
}Send_Pid;
//发送串口数据
void User_Send_Data()
{
	int16_t pitch,yaw,rol;
	pitch = (imu.pit)*100;
	yaw = (imu.yaw)*100;
	rol = (imu.rol)*100;

	Send_Pid pid_pitch_angle,pid_yaw_angle,pid_rol_angle;
	Send_Pid pid_pitch_speed,pid_yaw_speed,pid_rol_speed;
	
	pid_rol_angle.P = PID_UAV_Angle[0].P * 10;
	pid_rol_angle.I = PID_UAV_Angle[0].I * 10;
	pid_rol_angle.D = PID_UAV_Angle[0].D * 10;
	
	pid_pitch_angle.P = PID_UAV_Angle[1].P * 10;
	pid_pitch_angle.I = PID_UAV_Angle[1].I * 10;
	pid_pitch_angle.D = PID_UAV_Angle[1].D * 10;
	
	pid_yaw_angle.P = PID_UAV_Angle[2].P * 10;
	pid_yaw_angle.I = PID_UAV_Angle[2].I * 10;
	pid_yaw_angle.D = PID_UAV_Angle[2].D * 10;

	pid_rol_speed.P = PID_UAV_Speed[0].P * 10;
	pid_rol_speed.I = PID_UAV_Speed[0].I * 10;
	pid_rol_speed.D = PID_UAV_Speed[0].D * 10;

	pid_pitch_speed.P = PID_UAV_Speed[1].P * 10;
	pid_pitch_speed.I = PID_UAV_Speed[1].I * 10;
	pid_pitch_speed.D = PID_UAV_Speed[1].D * 10;
	
	pid_yaw_speed.P = PID_UAV_Speed[2].P * 10;
	pid_yaw_speed.I = PID_UAV_Speed[2].I * 10;
	pid_yaw_speed.D = PID_UAV_Speed[2].D * 10;
	
	
	memcpy(&User_tx_buff[1],&pitch,2);
	memcpy(&User_tx_buff[3],&yaw,2);
	memcpy(&User_tx_buff[5],&rol,2);
	
	memcpy(&User_tx_buff[7],&pid_pitch_angle.P,6);
	memcpy(&User_tx_buff[13],&pid_yaw_angle.P,6);
	memcpy(&User_tx_buff[19],&pid_rol_angle.P,6);


	memcpy(&User_tx_buff[25],&pid_pitch_speed.P,6);
	memcpy(&User_tx_buff[31],&pid_yaw_speed.P,6);
	memcpy(&User_tx_buff[37],&pid_rol_speed.P,6);
	
	
	User_tx_buff[0] = 0xff;	
	User_tx_buff[User_TX_SIZE - 1] = 0xfe;
	//DMA发送启动
	DMA_Cmd(DMA2_Stream6,ENABLE);
	
}
	Send_Pid pid_pitch_angle,pid_yaw_angle,pid_rol_angle;
	Send_Pid pid_pitch_speed,pid_yaw_speed,pid_rol_speed;
void User_Data_Decode(uint8_t *buff)
{

	
	if(buff[0] == 0xff)
	{
		if(buff[User_RX_SIZE - 1] == 0xfe)
		{
			memcpy(&pid_pitch_angle.P,&buff[1],6);
			memcpy(&pid_yaw_angle.P,&buff[7],6);
			memcpy(&pid_rol_angle.P,&buff[13],6);
			
			memcpy(&pid_pitch_speed.P,&buff[19],6);
			memcpy(&pid_yaw_speed.P,&buff[25],6);
			memcpy(&pid_rol_speed.P,&buff[31],6);

			PID_UAV_Angle[0].P = pid_rol_angle.P / 10.0;
			PID_UAV_Angle[0].I = pid_rol_angle.I / 10.0;
			PID_UAV_Angle[0].D = pid_rol_angle.D / 10.0;
	
			PID_UAV_Angle[1].P = pid_pitch_angle.P / 10.0;
			PID_UAV_Angle[1].I = pid_pitch_angle.I / 10.0;
			PID_UAV_Angle[1].D = pid_pitch_angle.D / 10.0;

			PID_UAV_Angle[2].P = pid_yaw_angle.P / 10.0;
			PID_UAV_Angle[2].I = pid_yaw_angle.I / 10.0;
			PID_UAV_Angle[2].D = pid_yaw_angle.D / 10.0;

			PID_UAV_Speed[0].P = pid_rol_speed.P / 10.0;
			PID_UAV_Speed[0].I = pid_rol_speed.I / 10.0;
			PID_UAV_Speed[0].D = pid_rol_speed.D / 10.0;		
			
		  PID_UAV_Speed[1].P = pid_pitch_speed.P / 10.0;
			PID_UAV_Speed[1].I = pid_pitch_speed.I / 10.0;
			PID_UAV_Speed[1].D = pid_pitch_speed.D / 10.0;		
			
			PID_UAV_Speed[2].P = pid_yaw_speed.P / 10.0;
			PID_UAV_Speed[2].I = pid_yaw_speed.I / 10.0;
			PID_UAV_Speed[2].D = pid_yaw_speed.D / 10.0;			
			
			for(int i = 0;i<3;i++)
			{
				PID_UAV_Angle[i].Iout = 0;
				PID_UAV_Speed[i].Iout = 0;
			}
			
		}
	}
	
}
void DMA2_Stream6_IRQHandler(void)
{
	if(DMA_GetITStatus(DMA2_Stream6, DMA_IT_TCIF6) == SET)
	{
		DMA_Cmd(DMA2_Stream6, DISABLE);
		DMA_SetCurrDataCounter(DMA2_Stream6, User_TX_SIZE); 
	}
	DMA_ClearITPendingBit(DMA2_Stream6, DMA_IT_TCIF6);
}
void USART6_IRQHandler()
{
			if(USART_GetITStatus(USART_User, USART_IT_IDLE) != RESET)	//判断是否为空闲中断
    {
        uint16_t this_time_rx_len = 0;
        USART_ReceiveData(USART_User);

        if(DMA_GetCurrentMemoryTarget(DMA2_Stream1) == DMA_Memory_0)	//获取当前目标内存是否为 DMA_Memory_0
        {
            //重新设置DMA
            DMA_Cmd(DMA2_Stream1, DISABLE);
            this_time_rx_len = DMA_GetCurrDataCounter(DMA2_Stream1);
            DMA_SetCurrDataCounter(DMA2_Stream1, User_RX_SIZE);
            DMA_Cmd(DMA2_Stream1, ENABLE);
            if(this_time_rx_len == User_RX_SIZE)	//校验接收
            {
                //处理数据
               User_Data_Decode(User_rx_buff[1]);
            }
        }
        else
        {
            //重新设置DMA
            DMA_Cmd(DMA2_Stream1, DISABLE);
            this_time_rx_len = DMA_GetCurrDataCounter(DMA2_Stream1);
            DMA_SetCurrDataCounter(DMA2_Stream1, User_RX_SIZE);
            DMA_Cmd(DMA2_Stream1, ENABLE);
            if(this_time_rx_len == User_RX_SIZE)	//校验接收
            {
                //处理数据
               User_Data_Decode(User_rx_buff[0]);
            }
        }
    }
}
#endif	

 



