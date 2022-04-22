#include "UPFLOW.h"
#define RCC_AHBPeriphClockCmd_GPIO_UPFLOW RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA,ENABLE)
#define RCC_APBPeriphClockCmd_USART_UPFLOW RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1, ENABLE)

#define GPIO_UPFLOW_1 GPIOA
#define GPIO_PinSource_UPFLOW_1 GPIO_PinSource9

#define GPIO_UPFLOW_2 GPIOB
#define GPIO_PinSource_UPFLOW_2 GPIO_PinSource7

#define GPIO_AF_USART_UPFLOW GPIO_AF_USART1

#define GPIO_Pin_UPFLOW_1 GPIO_Pin_9
#define GPIO_Pin_UPFLOW_2 GPIO_Pin_7

#define USART_UPFLOW USART1
#define USART_UPFLOW_IRQn USART1_IRQn
#define USART_UPFLOW_IRQHandler USART1_IRQHandler

#define RCC_AHBPeriphClockCmd_DMA_UPFLOW RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_DMA2,ENABLE)

#define DMA_Channel_UPFLOW_RX DMA_Channel_4
#define DMA_Stream_UPFLOW_RX DMA2_Stream2

#define DMA_Channel_UPFLOW_TX DMA_Channel_4
#define DMA_Stream_UPFLOW_TX DMA2_Stream7

#define DMA_Stream_UPFLOW_TX_IRQn DMA2_Stream7_IRQn
#define DMA_Stream_UPFLOW_TX_IRQHandler DMA2_Stream7_IRQHandler



#define DMA_IT_TCIF_UPFLOW DMA_IT_TCIF7


#define UPFLOW_RX_BUFF_SIZE 7 
#define UPFLOW_TX_BUFF_SIZE 10

uint8_t UPFLOW_rx_buff[2][UPFLOW_RX_BUFF_SIZE];

uint8_t UPFLOW_tx_buff[UPFLOW_TX_BUFF_SIZE];

UPFLOW_Info_TypeDef UPFLOW_Data;
uint8_t OpticalFlow_Is_Work=0;

/*
*@Description：串口8初始化
*@param 1：	  参数1
*@param 2：	  参数2
*@return:：	  返回值
*/
void UPFLOW_USART_Init()
{
	GPIO_InitTypeDef   GPIO_InitStruct;
	USART_InitTypeDef  USART_InitStruct;
	DMA_InitTypeDef    DMA_InitStruct;
	NVIC_InitTypeDef   NVIC_InitStruct;
	
	
	RCC_AHBPeriphClockCmd_GPIO_UPFLOW;
	RCC_APBPeriphClockCmd_USART_UPFLOW;
	

	GPIO_PinAFConfig(GPIO_UPFLOW_1, GPIO_PinSource_UPFLOW_1, GPIO_AF_USART_UPFLOW);
	GPIO_PinAFConfig(GPIO_UPFLOW_2, GPIO_PinSource_UPFLOW_2, GPIO_AF_USART_UPFLOW);
	
	GPIO_InitStruct.GPIO_Pin   = GPIO_Pin_UPFLOW_1;
	GPIO_InitStruct.GPIO_Mode  = GPIO_Mode_AF;
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_100MHz;
	GPIO_InitStruct.GPIO_OType = GPIO_OType_PP;
	GPIO_Init(GPIO_UPFLOW_1, &GPIO_InitStruct);

	GPIO_InitStruct.GPIO_Pin   = GPIO_Pin_UPFLOW_2;
	GPIO_InitStruct.GPIO_Mode  = GPIO_Mode_AF;
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_100MHz;
	GPIO_InitStruct.GPIO_OType = GPIO_OType_PP;
	GPIO_Init(GPIO_UPFLOW_2, &GPIO_InitStruct);	
	
	USART_InitStruct.USART_BaudRate            = 19200;
	USART_InitStruct.USART_WordLength          = USART_WordLength_8b;
	USART_InitStruct.USART_StopBits            = USART_StopBits_1;
	USART_InitStruct.USART_Parity              = USART_Parity_No;
	USART_InitStruct.USART_Mode                = USART_Mode_Rx | USART_Mode_Tx;
	USART_InitStruct.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_Init(USART_UPFLOW, &USART_InitStruct);
	
	USART_ClearFlag(USART_UPFLOW, USART_FLAG_IDLE);
//	USART_ITConfig(USART_UPFLOW, USART_IT_IDLE, ENABLE);	
  USART_ITConfig(USART_UPFLOW,USART_IT_RXNE,ENABLE);//rx interrupt is enable
	
	USART_Cmd(USART_UPFLOW, ENABLE);  //使能串口8
	
	
		
  RingBuff_Init(&OpticalFlow_Ringbuf);
  OpticalFlow_Is_Work=LC306_Config_Init(); 
  OpticalFlow_Init(); 
}
void UPFLOW_USART_DMA_Init()
{
	DMA_InitTypeDef DMA_InitStructure;
	RCC_AHBPeriphClockCmd_DMA_UPFLOW;
	
	//DMA1_Stream6_Channel5
	//UART8_RX
	DMA_InitStructure.DMA_Channel = DMA_Channel_UPFLOW_RX;
	DMA_InitStructure.DMA_PeripheralBaseAddr = (uint32_t) & (USART_UPFLOW->DR);
	DMA_InitStructure.DMA_Memory0BaseAddr = (uint32_t)UPFLOW_rx_buff[0];
	DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralToMemory;
	DMA_InitStructure.DMA_BufferSize = UPFLOW_RX_BUFF_SIZE;
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
	DMA_Init(DMA_Stream_UPFLOW_RX, &DMA_InitStructure);
	
	DMA_DoubleBufferModeConfig(DMA_Stream_UPFLOW_RX, (uint32_t)UPFLOW_rx_buff[1], DMA_Memory_0);
	DMA_DoubleBufferModeCmd(DMA_Stream_UPFLOW_RX, ENABLE);
	
	USART_DMACmd(USART_UPFLOW, USART_DMAReq_Rx, ENABLE);	
	
	DMA_Cmd(DMA_Stream_UPFLOW_RX, ENABLE);
	

	//DMA1_Stream0_Channel5
	//UART8_TX
	DMA_InitStructure.DMA_Channel = DMA_Channel_UPFLOW_TX;
	DMA_InitStructure.DMA_PeripheralBaseAddr = (uint32_t) & (USART_UPFLOW->DR);
	DMA_InitStructure.DMA_Memory0BaseAddr = (uint32_t)UPFLOW_tx_buff;
	DMA_InitStructure.DMA_DIR = DMA_DIR_MemoryToPeripheral;
	DMA_InitStructure.DMA_BufferSize = UPFLOW_TX_BUFF_SIZE;
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
	DMA_Init(DMA_Stream_UPFLOW_TX, &DMA_InitStructure);
	
	USART_DMACmd(USART_UPFLOW, USART_DMAReq_Tx, ENABLE);

	DMA_ITConfig(DMA_Stream_UPFLOW_TX,DMA_IT_TC,ENABLE);
	
	
	DMA_Cmd(DMA_Stream_UPFLOW_TX, ENABLE);

}


void UPFLOW_USART_NVIC_Init()
{
	//串口8接收空闲中断
	NVIC_InitTypeDef NVIC_InitStructure;
	NVIC_InitStructure.NVIC_IRQChannel = USART_UPFLOW_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
	
	//发送中断
	NVIC_InitStructure.NVIC_IRQChannel                   =DMA_Stream_UPFLOW_TX_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority =1;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority        =0;
	NVIC_InitStructure.NVIC_IRQChannelCmd                =ENABLE ;
	NVIC_Init(&NVIC_InitStructure);
}

void UPFLOW_Init()
{
	UPFLOW_USART_Init();
	UPFLOW_USART_NVIC_Init();
//	UPFLOW_USART_DMA_Init();
}
/*
*@title：UPFLOW发送中断
*@description：
*@param 1：	
*@param 2：	
*@return:：	
*/
void DMA_Stream_UPFLOW_TX_IRQHandler(void)
{
	if(DMA_GetITStatus(DMA_Stream_UPFLOW_TX, DMA_IT_TCIF_UPFLOW) == SET)
	{
		DMA_Cmd(DMA_Stream_UPFLOW_TX, DISABLE);
		DMA_SetCurrDataCounter(DMA_Stream_UPFLOW_TX, UPFLOW_TX_BUFF_SIZE); 
	}
	DMA_ClearITPendingBit(DMA_Stream_UPFLOW_TX, DMA_IT_TCIF_UPFLOW);
}
int Pitch_Angle_UPFLOW = 0;
int Yaw_Angle_UPFLOW = 0;
int Roll_Angle_UPFLOW = 0;
int Robot_Color = 0;

extern float yaw_angle;
//原始Pitch角
extern short Origin_Init_Pitch_Angle;
/**
 *@Function:		UPFLOW_Send_Data()
 *@Description:	UPFLOW数据发送
 *@Param:       形参
 *@Return:	  	返回值
 */
void UPFLOW_Send_Data()
{
//	//角度计算
//	Pitch_Angle_UPFLOW = (CAN_Gimbal[1].Current_MechAngle - Origin_Init_Pitch_Angle)/8192.0f*36000;
//	Yaw_Angle_UPFLOW = (int16_t)((int16_t)(yaw_angle * 100)%36000);
//	Roll_Angle_UPFLOW = (int16_t)((int16_t)(imu.rol * 100)%36000);

//	
//	if( game_robot_state.robot_id  <=7)	
//		Robot_Color = UPFLOW_tx_buff[8];//默认己方红色
//	else if(game_robot_state.robot_id >= 11)
//		Robot_Color = UPFLOW_tx_buff[8] + 0x80;//己方蓝色	
//	
//	
//	UPFLOW_tx_buff[0] = 0xff;
//	memcpy(&UPFLOW_tx_buff[1],&Pitch_Angle_UPFLOW,2);
//	memcpy(&UPFLOW_tx_buff[3],&Yaw_Angle_UPFLOW,2);
//	memcpy(&UPFLOW_tx_buff[5],&Roll_Angle_UPFLOW,2);
//	UPFLOW_tx_buff[7] = real_shoot_data.bullet_speed * 8.0f;
//	UPFLOW_tx_buff[8] = 0x00; //模式位
//	UPFLOW_tx_buff[8] = Robot_Color;			
//	UPFLOW_tx_buff[9] = 0xfe;
	//DMA发送启动
	DMA_Cmd(DMA_Stream_UPFLOW_TX,ENABLE);
}
/**
 *@Function:		UPFLOW_Data_Decode(uint8_t *buff)
 *@Description:	UPFLOW接收数据解析
 *@Param:       形参
 *@Return:	  	返回值
 */
void UPFLOW_Data_Decode(uint8_t *buff)
{
	
	UPFLOW_Data.header = buff[0];
	UPFLOW_Data.tail = buff[6];
	
	if(UPFLOW_Data.header == 0xff && UPFLOW_Data.tail == 0xfe)
	{
		UPFLOW_Data.pit_offset = ((short)(buff[2] << 8 | buff[1]))/1000.0f;
		UPFLOW_Data.yaw_offset = ((short)(buff[4] << 8 | buff[3]))/1000.0f;
		
		UPFLOW_Data.buff_flag = buff[5];		
	}

}
/*
*@Description：串口8中断处理函数
*@param 1：	  参数1
*@param 2：	  参数2
*@return:：	  返回值
*/
void USART_UPFLOW_IRQHandler()
{
		if(USART_GetITStatus(USART_UPFLOW, USART_IT_RXNE) != RESET)	//判断是否为空闲中断
   {
			USART_ClearITPendingBit(USART_UPFLOW,USART_IT_RXNE);
			RingBuf_Write(USART_ReceiveData(USART_UPFLOW),&OpticalFlow_Ringbuf,28);//往环形队列里面写数据
		 
	 }

//			if(USART_GetITStatus(USART_UPFLOW, USART_IT_IDLE) != RESET)	//判断是否为空闲中断
//    {
//        uint16_t this_time_rx_len = 0;
//        USART_ReceiveData(USART_UPFLOW);

//        if(DMA_GetCurrentMemoryTarget(DMA_Stream_UPFLOW_RX) == DMA_Memory_0)	//获取当前目标内存是否为 DMA_Memory_0
//        {
//            //重新设置DMA
//            DMA_Cmd(DMA_Stream_UPFLOW_RX, DISABLE);
//            this_time_rx_len = DMA_GetCurrDataCounter(DMA_Stream_UPFLOW_RX);
//            DMA_SetCurrDataCounter(DMA_Stream_UPFLOW_RX, UPFLOW_RX_BUFF_SIZE);
//            DMA_Cmd(DMA_Stream_UPFLOW_RX, ENABLE);
//            if(this_time_rx_len == UPFLOW_RX_BUFF_SIZE)	//校验接收
//            {
//                //处理数据
//               UPFLOW_Data_Decode(UPFLOW_rx_buff[1]);
//            }
//        }
//        else
//        {
//            //重新设置DMA
//            DMA_Cmd(DMA_Stream_UPFLOW_RX, DISABLE);
//            this_time_rx_len = DMA_GetCurrDataCounter(DMA_Stream_UPFLOW_RX);
//            DMA_SetCurrDataCounter(DMA_Stream_UPFLOW_RX, UPFLOW_RX_BUFF_SIZE);
//            DMA_Cmd(DMA_Stream_UPFLOW_RX, ENABLE);
//            if(this_time_rx_len == UPFLOW_RX_BUFF_SIZE)	//校验接收
//            {
//                //处理数据
//               UPFLOW_Data_Decode(UPFLOW_rx_buff[0]);
//            }
//        }
//    }
}
/****************************************************/

RingBuff_t OpticalFlow_Ringbuf;

#define OPTICALFLOW_UART USART1
#define SENSOR_IIC_ADDR 0xdc
const static u8 tab_focus[4] = {0x96,0x26,0xbc,0x50};		
const static u8 Sensor_cfg[]={
  //地址, 数据
  0x12, 0x80, 
  0x11, 0x30, 
  0x1b, 0x06, 
  0x6b, 0x43, 
  0x12, 0x20, 
  0x3a, 0x00, 
  0x15, 0x02, 
  0x62, 0x81, 
  0x08, 0xa0, 
  0x06, 0x68, 
  0x2b, 0x20, 
  0x92, 0x25, 
  0x27, 0x97, 
  0x17, 0x01, 
  0x18, 0x79, 
  0x19, 0x00, 
  0x1a, 0xa0, 
  0x03, 0x00, 
  0x13, 0x00, 
  0x01, 0x13, 
  0x02, 0x20, 
  0x87, 0x16, 
  0x8c, 0x01, 
  0x8d, 0xcc, 
  0x13, 0x07, 
  0x33, 0x10, 
  0x34, 0x1d, 
  0x35, 0x46, 
  0x36, 0x40, 
  0x37, 0xa4, 
  0x38, 0x7c, 
  0x65, 0x46, 
  0x66, 0x46, 
  0x6e, 0x20, 
  0x9b, 0xa4, 
  0x9c, 0x7c, 
  0xbc, 0x0c, 
  0xbd, 0xa4, 
  0xbe, 0x7c, 
  0x20, 0x09, 
  0x09, 0x03, 
  0x72, 0x2f, 
  0x73, 0x2f, 
  0x74, 0xa7, 
  0x75, 0x12, 
  0x79, 0x8d, 
  0x7a, 0x00, 
  0x7e, 0xfa, 
  0x70, 0x0f, 
  0x7c, 0x84, 
  0x7d, 0xba, 
  0x5b, 0xc2, 
  0x76, 0x90, 
  0x7b, 0x55, 
  0x71, 0x46, 
  0x77, 0xdd, 
  0x13, 0x0f, 
  0x8a, 0x10, 
  0x8b, 0x20, 
  0x8e, 0x21, 
  0x8f, 0x40, 
  0x94, 0x41, 
  0x95, 0x7e, 
  0x96, 0x7f, 
  0x97, 0xf3, 
  0x13, 0x07, 
  0x24, 0x58, 
  0x97, 0x48, 
  0x25, 0x08, 
  0x94, 0xb5, 
  0x95, 0xc0, 
  0x80, 0xf4, 
  0x81, 0xe0, 
  0x82, 0x1b, 
  0x83, 0x37, 
  0x84, 0x39, 
  0x85, 0x58, 
  0x86, 0xff, 
  0x89, 0x15, 
  0x8a, 0xb8, 
  0x8b, 0x99, 
  0x39, 0x98, 
  0x3f, 0x98, 
  0x90, 0xa0, 
  0x91, 0xe0, 
  0x40, 0x20, 
  0x41, 0x28, 
  0x42, 0x26, 
  0x43, 0x25, 
  0x44, 0x1f, 
  0x45, 0x1a, 
  0x46, 0x16, 
  0x47, 0x12, 
  0x48, 0x0f, 
  0x49, 0x0d, 
  0x4b, 0x0b, 
  0x4c, 0x0a, 
  0x4e, 0x08, 
  0x4f, 0x06, 
  0x50, 0x06, 
  0x5a, 0x56, 
  0x51, 0x1b, 
  0x52, 0x04, 
  0x53, 0x4a, 
  0x54, 0x26, 
  0x57, 0x75, 
  0x58, 0x2b, 
  0x5a, 0xd6, 
  0x51, 0x28, 
  0x52, 0x1e, 
  0x53, 0x9e, 
  0x54, 0x70, 
  0x57, 0x50, 
  0x58, 0x07, 
  0x5c, 0x28, 
  0xb0, 0xe0, 
  0xb1, 0xc0, 
  0xb2, 0xb0, 
  0xb3, 0x4f, 
  0xb4, 0x63, 
  0xb4, 0xe3, 
  0xb1, 0xf0, 
  0xb2, 0xa0, 
  0x55, 0x00, 
  0x56, 0x40, 
  0x96, 0x50, 
  0x9a, 0x30, 
  0x6a, 0x81, 
  0x23, 0x33, 
  0xa0, 0xd0, 
  0xa1, 0x31, 
  0xa6, 0x04, 
  0xa2, 0x0f, 
  0xa3, 0x2b, 
  0xa4, 0x0f, 
  0xa5, 0x2b, 
  0xa7, 0x9a, 
  0xa8, 0x1c, 
  0xa9, 0x11, 
  0xaa, 0x16, 
  0xab, 0x16, 
  0xac, 0x3c, 
  0xad, 0xf0, 
  0xae, 0x57, 
  0xc6, 0xaa, 
  0xd2, 0x78, 
  0xd0, 0xb4, 
  0xd1, 0x00, 
  0xc8, 0x10, 
  0xc9, 0x12, 
  0xd3, 0x09, 
  0xd4, 0x2a, 
  0xee, 0x4c, 
  0x7e, 0xfa, 
  0x74, 0xa7, 
  0x78, 0x4e, 
  0x60, 0xe7, 
  0x61, 0xc8, 
  0x6d, 0x70, 
  0x1e, 0x39, 
  0x98, 0x1a
};
static void SensorConfig_UartSend(u8 dat)
{
	while((OPTICALFLOW_UART->SR&0X40)==0);//循环发送,直到发送完毕   
	OPTICALFLOW_UART->DR = (u8) dat;      
}

uint16_t LC306_Ready_Cnt=0;
/**************************************************/		
uint8_t LC306_Config_Init()
{
  u16 i;
  u16 len ;
  u8 recv[3];
  int recv_cnt;		
  delay_ms(100);		
  len = sizeof(Sensor_cfg);	
  SensorConfig_UartSend(0xAA);//0xAA指令
  SensorConfig_UartSend(0xAB);//0xAB指令		 
  SensorConfig_UartSend(tab_focus[0]);		
  SensorConfig_UartSend(tab_focus[1]);
  SensorConfig_UartSend(tab_focus[2]);
  SensorConfig_UartSend(tab_focus[3]);
  SensorConfig_UartSend(tab_focus[0]^tab_focus[1]^tab_focus[2]^tab_focus[3]);			 		 
  recv_cnt = 0;
  LC306_Ready_Cnt=65535;
  while(recv_cnt<3)  //如接收不到模块返回的三个数，可延时10ms从0xAA指令开始重新配置
  {
    while(USART_GetFlagStatus(OPTICALFLOW_UART, USART_FLAG_RXNE)==RESET)
    {
      LC306_Ready_Cnt--;
      if(LC306_Ready_Cnt==0) return 0;
    };
    recv[recv_cnt++] = USART_ReceiveData(OPTICALFLOW_UART);							
  }			
  if(((recv[0]^recv[1]) == recv[2]) & (recv[1] == 0x00)) 
    printf("AB Command configuration successconfig succefful\r\n");		
  
  for(i=0;i<len;i+=2)//0xBB? 噶?
  {
    SensorConfig_UartSend(0xBB);		 
    SensorConfig_UartSend(SENSOR_IIC_ADDR);		
    SensorConfig_UartSend(Sensor_cfg[i]);
    SensorConfig_UartSend(Sensor_cfg[i+1]);
    SensorConfig_UartSend(SENSOR_IIC_ADDR^Sensor_cfg[i]^Sensor_cfg[i+1]);	 
    recv_cnt = 0;
    while(recv_cnt<3)  //如接收不到模块返回的三个数，可延时1ms重新发送0xBB指令
    {
      while(USART_GetFlagStatus(OPTICALFLOW_UART, USART_FLAG_RXNE)==RESET){};
      recv[recv_cnt++] = USART_ReceiveData(OPTICALFLOW_UART);							
    }			
    if(((recv[0]^recv[1]) == recv[2]) & (recv[1] == 0x00))
    printf("BB Command configuration successconfig succefful\r\n");								 
  }	 		 
  SensorConfig_UartSend(0xDD);//0xDD		
  printf("Configuration success\r\n");
  return 1;
	

}		



//https://blog.csdn.net/sszhouplus/article/details/43113505
//https://blog.csdn.net/shengzhadon/article/details/46784509
//https://blog.csdn.net/shengzhadon/article/details/46791903
//https://blog.csdn.net/shengzhadon/article/details/46803401
/************************************************************************/
#define M_PI_F 3.141592653589793f
void Set_Cutoff_Frequency(float sample_frequent, float cutoff_frequent,Butter_Parameter *LPF)
{
  float fr = sample_frequent / cutoff_frequent;
  float ohm = tanf(M_PI_F / fr);
  float c = 1.0f + 2.0f * cosf(M_PI_F / 4.0f) * ohm + ohm * ohm;
  if (cutoff_frequent <= 0.0f) {
    // no filtering
    return;
  }
  LPF->b[0] = ohm * ohm / c;
  LPF->b[1] = 2.0f * LPF->b[0];
  LPF->b[2] = LPF->b[0];
  LPF->a[0]=1.0f;
  LPF->a[1] = 2.0f * (ohm * ohm - 1.0f) / c;
  LPF->a[2] = (1.0f - 2.0f * cosf(M_PI_F / 4.0f) * ohm + ohm * ohm) / c;
}

/*************************************************
函数名:	float LPButterworth(float curr_input,Butter_BufferData *Buffer,Butter_Parameter *Parameter)
说明:	加速度计低通滤波器
入口:	float curr_input 当前输入加速度计,滤波器参数，滤波器缓存
出口:	无
备注:	2阶Butterworth低通滤波器
*************************************************/
float LPButterworth(float curr_input,Butter_BufferData *Buffer,Butter_Parameter *Parameter)
{
  /* 加速度计Butterworth滤波 */
  /* 获取最新x(n) */
  static int LPF_Cnt=0;
  Buffer->Input_Butter[2]=curr_input;
  if(LPF_Cnt>=100)
  {
    /* Butterworth滤波 */
    Buffer->Output_Butter[2]=
      Parameter->b[0] * Buffer->Input_Butter[2]
        +Parameter->b[1] * Buffer->Input_Butter[1]
          +Parameter->b[2] * Buffer->Input_Butter[0]
            -Parameter->a[1] * Buffer->Output_Butter[1]
              -Parameter->a[2] * Buffer->Output_Butter[0];
  }
  else
  {
    Buffer->Output_Butter[2]=Buffer->Input_Butter[2];
    LPF_Cnt++;
  }
  /* x(n) 序列保存 */
  Buffer->Input_Butter[0]=Buffer->Input_Butter[1];
  Buffer->Input_Butter[1]=Buffer->Input_Butter[2];
  /* y(n) 序列保存 */
  Buffer->Output_Butter[0]=Buffer->Output_Butter[1];
  Buffer->Output_Butter[1]=Buffer->Output_Butter[2];
  return Buffer->Output_Butter[2];
}


float opticalflow_high=1000;//默认1m=100cm=1000mm
flow_float opt_data;
flow_integral_frame opt_origin_data;
Vector2f opt_filter_data; //滤波后的像素位移
Vector2f opt_gyro_data;   //光流角速度
Vector2f opt_gyro_filter_data;//光流经过旋转互补滤波后的角速度
Vector2f gyro_filter_data;//低通同步相位的角速度
Butter_Parameter OpticalFlow_Parameter,OpticalFlow_Gyro_Parameter;
Butter_BufferData Buffer_OpticalFlow[2],Buffer_OpticalFlow_Gyro[2];
void  OpticalFlow_CF(float flow_height,Vector2f accel,Vector2f flow);
void OpticalFlow_Init()
{
  Set_Cutoff_Frequency(50, 20,&OpticalFlow_Parameter);//20
  Set_Cutoff_Frequency(50, 10,&OpticalFlow_Gyro_Parameter);//同步相位//10
}

extern RingBuff_t OpticalFlow_Ringbuf;
uint8_t Optflow_Prase()//50hz
{
  for(uint16_t i=0;i<=14;i++)
  {
    if(OpticalFlow_Ringbuf.Ring_Buff[i]==0xfe//帧头
       &&OpticalFlow_Ringbuf.Ring_Buff[i+1]==0x0a//帧长
         &&OpticalFlow_Ringbuf.Ring_Buff[i+13]==0x55)//帧尾
    {
      opt_origin_data.pixel_flow_x_integral=(int16_t)(OpticalFlow_Ringbuf.Ring_Buff[i+3]<<8)|OpticalFlow_Ringbuf.Ring_Buff[i+2];
      opt_origin_data.pixel_flow_y_integral=(int16_t)(OpticalFlow_Ringbuf.Ring_Buff[i+5]<<8)|OpticalFlow_Ringbuf.Ring_Buff[i+4];
      opt_origin_data.integration_timespan= (int16_t)(OpticalFlow_Ringbuf.Ring_Buff[i+7]<<8)|OpticalFlow_Ringbuf.Ring_Buff[i+6];
      opt_origin_data.qual=OpticalFlow_Ringbuf.Ring_Buff[i+12];      
      opt_filter_data.x=LPButterworth(opt_origin_data.pixel_flow_x_integral,&Buffer_OpticalFlow[0],&OpticalFlow_Parameter);
      opt_filter_data.y=LPButterworth(opt_origin_data.pixel_flow_y_integral,&Buffer_OpticalFlow[1],&OpticalFlow_Parameter);   
      opt_data.x=(opt_origin_data.pixel_flow_x_integral*opticalflow_high)/10000.0f;//单位:乘以高度单位mm后为实际位移mm
      opt_data.y=(opt_origin_data.pixel_flow_y_integral*opticalflow_high)/10000.0f;//单位:乘以高度单位mm后为实际位移mm     
      opt_data.dt=(int16_t)(opt_origin_data.integration_timespan*0.001f);//单位ms
      opt_data.qual=opt_origin_data.qual;    
      opt_gyro_data.x=(opt_filter_data.x)/200.0f;//光流角速度rad/s
      opt_gyro_data.y=(opt_filter_data.y)/200.0f;//光流角速度rad/s         
      //gyro_filter_data.x=LPButterworth(Roll_Gyro,&Buffer_OpticalFlow_Gyro[0],&OpticalFlow_Gyro_Parameter)/57.3f;//陀螺仪相位同步角速度
      //gyro_filter_data.y=LPButterworth(Pitch_Gyro,&Buffer_OpticalFlow_Gyro[1],&OpticalFlow_Gyro_Parameter)/57.3f;//陀螺仪相位同步角速度
      //opt_gyro_filter_data.x=OpticalFlow_Rotate_Complementary_Filter(opt_gyro_data.x,gyro_filter_data.x,'x');//光流角速度与陀螺仪角速度融合 
      //opt_gyro_filter_data.y=OpticalFlow_Rotate_Complementary_Filter(opt_gyro_data.y,gyro_filter_data.y,'y'); //光流角速度与陀螺仪角速度融合 
      return 1;
    }
  }
  return 0;
}
/********************************************************/
/**
* @brief  RingBuff_Init
* @param  void
* @return void
* @author 杰杰
* @date   2018
* @version v1.0
* @note   初始化环形缓冲区
*/
void RingBuff_Init(RingBuff_t *ringBuff)
{
  //初始化相关信息
  ringBuff->Head = 0;
  ringBuff->Tail = 0;
  ringBuff->Lenght = 0;
}

/**
* @brief  Write_RingBuff
* @param  u8 data
* @return FLASE:环形缓冲区已满，写入失败;TRUE:写入成功
* @author 杰杰
* @date   2018
* @version v1.0
* @note   往环形缓冲区写入u8类型的数据
*/
uint8_t Write_RingBuff(uint8_t data,RingBuff_t *ringBuff)
{
  if(ringBuff->Lenght >= RINGBUFF_LEN) //判断缓冲区是否已满
  {
    return 0;
  }
  ringBuff->Ring_Buff[ringBuff->Tail]=data;
  //ringBuff.Tail++;
  ringBuff->Tail = (ringBuff->Tail+1)%RINGBUFF_LEN;//防止越界非法访问
  ringBuff->Lenght++;
  return 1;
}

/**
* @brief  Read_RingBuff
* @param  u8 *rData，用于保存读取的数据
* @return FLASE:环形缓冲区没有数据，读取失败;TRUE:读取成功
* @author 杰杰
* @date   2018
* @version v1.0
* @note   从环形缓冲区读取一个u8类型的数据
*/
uint8_t Read_RingBuff(uint8_t *rData,RingBuff_t *ringBuff)
{
  if(ringBuff->Lenght == 0)//判断非空
  {
    return 0;
  }
  *rData = ringBuff->Ring_Buff[ringBuff->Head];//先进先出FIFO，从缓冲区头出
  //ringBuff.Head++;
  ringBuff->Head = (ringBuff->Head+1)%RINGBUFF_LEN;//防止越界非法访问
  ringBuff->Lenght--;
  return 1;
}



void RingBuf_Write(unsigned char data,RingBuff_t *ringBuff,uint16_t Length)
{
  ringBuff->Ring_Buff[ringBuff->Tail]=data;//从尾部追加
  if(++ringBuff->Tail>=Length)//尾节点偏移
    ringBuff->Tail=0;//大于数组最大长度 归零 形成环形队列  
  if(ringBuff->Tail==ringBuff->Head)//如果尾部节点追到头部节点，则修改头节点偏移位置丢弃早期数据
  {
    if((++ringBuff->Head)>=Length)  
      ringBuff->Head=0; 
  }
}

uint8_t RingBuf_Read(unsigned char* pData,RingBuff_t *ringBuff)
{
  if(ringBuff->Head==ringBuff->Tail)  return 1;//如果头尾接触表示缓冲区为空
  else 
  {  
    *pData=ringBuff->Ring_Buff[ringBuff->Head];//如果缓冲区非空则取头节点值并偏移头节点
    if((++ringBuff->Head)>=RINGBUFF_LEN)   ringBuff->Head=0;
    return 0;//返回0，表示读取数据成功
  }
}
