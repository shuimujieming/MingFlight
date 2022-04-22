//#include "neo6m.h"

//#define RCC_AHBPeriphClockCmd_GPIO_GPS RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA,ENABLE)
//#define RCC_APBPeriphClockCmd_USART_GPS RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1, ENABLE)

//#define GPIO_GPS_1 GPIOA
//#define GPIO_PinSource_GPS_1 GPIO_PinSource9

//#define GPIO_GPS_2 GPIOB
//#define GPIO_PinSource_GPS_2 GPIO_PinSource7

//#define GPIO_AF_USART_GPS GPIO_AF_USART1

//#define GPIO_Pin_GPS_1 GPIO_Pin_9
//#define GPIO_Pin_GPS_2 GPIO_Pin_7

//#define USART_GPS USART1
//#define USART_GPS_IRQn USART1_IRQn
//#define USART_GPS_IRQHandler USART1_IRQHandler

//#define RCC_AHBPeriphClockCmd_DMA_GPS RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_DMA2,ENABLE)

//#define DMA_Channel_GPS_RX DMA_Channel_4
//#define DMA_Stream_GPS_RX DMA2_Stream2

//#define DMA_Channel_GPS_TX DMA_Channel_4
//#define DMA_Stream_GPS_TX DMA2_Stream7

//#define DMA_Stream_GPS_TX_IRQn DMA2_Stream7_IRQn
//#define DMA_Stream_GPS_TX_IRQHandler DMA2_Stream7_IRQHandler



//#define DMA_IT_TCIF_GPS DMA_IT_TCIF7


//#define GPS_RX_BUFF_SIZE 7 
//#define GPS_TX_BUFF_SIZE 10

//uint8_t GPS_rx_buff[2][GPS_RX_BUFF_SIZE];

//uint8_t GPS_tx_buff[GPS_TX_BUFF_SIZE];

///*
//*@Description：串口8初始化
//*@param 1：	  参数1
//*@param 2：	  参数2
//*@return:：	  返回值
//*/
//void GPS_USART_Init()
//{
//	GPIO_InitTypeDef   GPIO_InitStruct;
//	USART_InitTypeDef  USART_InitStruct;
//	DMA_InitTypeDef    DMA_InitStruct;
//	NVIC_InitTypeDef   NVIC_InitStruct;
//	
//	
//	RCC_AHBPeriphClockCmd_GPIO_GPS;//此处可以用或运算，因为是控制位的
//	RCC_APBPeriphClockCmd_USART_GPS;
//	

//	GPIO_PinAFConfig(GPIO_GPS_1, GPIO_PinSource_GPS_1, GPIO_AF_USART_GPS);
//	GPIO_PinAFConfig(GPIO_GPS_2, GPIO_PinSource_GPS_2, GPIO_AF_USART_GPS);
//	
//	GPIO_InitStruct.GPIO_Pin   = GPIO_Pin_GPS_1;
//	GPIO_InitStruct.GPIO_Mode  = GPIO_Mode_AF;
//	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_100MHz;
//	GPIO_InitStruct.GPIO_OType = GPIO_OType_PP;
//	GPIO_Init(GPIO_GPS_1, &GPIO_InitStruct);

//	GPIO_InitStruct.GPIO_Pin   = GPIO_Pin_GPS_2;
//	GPIO_InitStruct.GPIO_Mode  = GPIO_Mode_AF;
//	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_100MHz;
//	GPIO_InitStruct.GPIO_OType = GPIO_OType_PP;
//	GPIO_Init(GPIO_GPS_2, &GPIO_InitStruct);	
//	
//	USART_InitStruct.USART_BaudRate            = 115200;
//	USART_InitStruct.USART_WordLength          = USART_WordLength_8b;
//	USART_InitStruct.USART_StopBits            = USART_StopBits_1;
//	USART_InitStruct.USART_Parity              = USART_Parity_No;
//	USART_InitStruct.USART_Mode                = USART_Mode_Rx | USART_Mode_Tx;
//	USART_InitStruct.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
//	USART_Init(USART_GPS, &USART_InitStruct);
//	
//	USART_ClearFlag(USART_GPS, USART_FLAG_IDLE);
//	USART_ITConfig(USART_GPS, USART_IT_IDLE, ENABLE);	
//	
//	USART_Cmd(USART_GPS, ENABLE);  //使能串口8
//}
//void GPS_USART_DMA_Init()
//{
//	DMA_InitTypeDef DMA_InitStructure;
//	RCC_AHBPeriphClockCmd_DMA_GPS;
//	
//	//DMA1_Stream6_Channel5
//	//UART8_RX
//	DMA_InitStructure.DMA_Channel = DMA_Channel_GPS_RX;
//	DMA_InitStructure.DMA_PeripheralBaseAddr = (uint32_t) & (USART_GPS->DR);
//	DMA_InitStructure.DMA_Memory0BaseAddr = (uint32_t)GPS_rx_buff[0];
//	DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralToMemory;
//	DMA_InitStructure.DMA_BufferSize = GPS_RX_BUFF_SIZE;
//	DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
//	DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;
//	DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;
//	DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte;
//	DMA_InitStructure.DMA_Mode = DMA_Mode_Circular;
//	DMA_InitStructure.DMA_Priority = DMA_Priority_VeryHigh;
//	DMA_InitStructure.DMA_FIFOMode = DMA_FIFOMode_Disable;
//	DMA_InitStructure.DMA_FIFOThreshold = DMA_FIFOThreshold_1QuarterFull;
//	DMA_InitStructure.DMA_MemoryBurst = DMA_MemoryBurst_Single;
//	DMA_InitStructure.DMA_PeripheralBurst = DMA_PeripheralBurst_Single;
//	DMA_Init(DMA_Stream_GPS_RX, &DMA_InitStructure);
//	
//	DMA_DoubleBufferModeConfig(DMA_Stream_GPS_RX, (uint32_t)GPS_rx_buff[1], DMA_Memory_0);
//	DMA_DoubleBufferModeCmd(DMA_Stream_GPS_RX, ENABLE);
//	
//	USART_DMACmd(USART_GPS, USART_DMAReq_Rx, ENABLE);	
//	
//	DMA_Cmd(DMA_Stream_GPS_RX, ENABLE);
//	

//	//DMA1_Stream0_Channel5
//	//UART8_TX
//	DMA_InitStructure.DMA_Channel = DMA_Channel_GPS_TX;
//	DMA_InitStructure.DMA_PeripheralBaseAddr = (uint32_t) & (USART_GPS->DR);
//	DMA_InitStructure.DMA_Memory0BaseAddr = (uint32_t)GPS_tx_buff;
//	DMA_InitStructure.DMA_DIR = DMA_DIR_MemoryToPeripheral;
//	DMA_InitStructure.DMA_BufferSize = GPS_TX_BUFF_SIZE;
//	DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
//	DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;
//	DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;
//	DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte;
//	DMA_InitStructure.DMA_Mode = DMA_Mode_Normal;
//	DMA_InitStructure.DMA_Priority = DMA_Priority_VeryHigh;
//	DMA_InitStructure.DMA_FIFOMode = DMA_FIFOMode_Disable;
//	DMA_InitStructure.DMA_FIFOThreshold = DMA_FIFOThreshold_1QuarterFull;
//	DMA_InitStructure.DMA_MemoryBurst = DMA_MemoryBurst_Single;
//	DMA_InitStructure.DMA_PeripheralBurst = DMA_PeripheralBurst_Single;
//	DMA_Init(DMA_Stream_GPS_TX, &DMA_InitStructure);
//	
//	USART_DMACmd(USART_GPS, USART_DMAReq_Tx, ENABLE);

//	DMA_ITConfig(DMA_Stream_GPS_TX,DMA_IT_TC,ENABLE);
//	
//	DMA_Cmd(DMA_Stream_GPS_TX, ENABLE);

//}


//void GPS_USART_NVIC_Init()
//{
//	//串口8接收空闲中断
//	NVIC_InitTypeDef NVIC_InitStructure;
//	NVIC_InitStructure.NVIC_IRQChannel = USART_GPS_IRQn;
//	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
//	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
//	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
//	NVIC_Init(&NVIC_InitStructure);
//	
//	//发送中断
//	NVIC_InitStructure.NVIC_IRQChannel                   =DMA_Stream_GPS_TX_IRQn;
//	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority =1;
//	NVIC_InitStructure.NVIC_IRQChannelSubPriority        =0;
//	NVIC_InitStructure.NVIC_IRQChannelCmd                =ENABLE ;
//	NVIC_Init(&NVIC_InitStructure);
//}

//void GPS_Init()
//{
//	GPS_USART_Init();
//	GPS_USART_NVIC_Init();
//	GPS_USART_DMA_Init();
//}
///*
//*@title：GPS发送中断
//*@description：
//*@param 1：	
//*@param 2：	
//*@return:：	
//*/
//void DMA_Stream_GPS_TX_IRQHandler(void)
//{
//	if(DMA_GetITStatus(DMA_Stream_GPS_TX, DMA_IT_TCIF_GPS) == SET)
//	{
//		DMA_Cmd(DMA_Stream_GPS_TX, DISABLE);
//		DMA_SetCurrDataCounter(DMA_Stream_GPS_TX, GPS_TX_BUFF_SIZE); 
//	}
//	DMA_ClearITPendingBit(DMA_Stream_GPS_TX, DMA_IT_TCIF_GPS);
//}


//void GPS_Send_Data()
//{

//	DMA_Cmd(DMA_Stream_GPS_TX,ENABLE);
//}

//void GPS_Data_Decode(uint8_t *buff)
//{
//	

//}
///*
//*@Description：串口8中断处理函数
//*@param 1：	  参数1
//*@param 2：	  参数2
//*@return:：	  返回值
//*/
//void USART_GPS_IRQHandler()
//{
//			if(USART_GetITStatus(USART_GPS, USART_IT_IDLE) != RESET)	//判断是否为空闲中断
//    {
//        uint16_t this_time_rx_len = 0;
//        USART_ReceiveData(USART_GPS);

//        if(DMA_GetCurrentMemoryTarget(DMA_Stream_GPS_RX) == DMA_Memory_0)	//获取当前目标内存是否为 DMA_Memory_0
//        {
//            //重新设置DMA
//            DMA_Cmd(DMA_Stream_GPS_RX, DISABLE);
//            this_time_rx_len = DMA_GetCurrDataCounter(DMA_Stream_GPS_RX);
//            DMA_SetCurrDataCounter(DMA_Stream_GPS_RX, GPS_RX_BUFF_SIZE);
//            DMA_Cmd(DMA_Stream_GPS_RX, ENABLE);
//            if(this_time_rx_len == GPS_RX_BUFF_SIZE)	//校验接收
//            {
//                //处理数据
//               GPS_Data_Decode(GPS_rx_buff[1]);
//            }
//        }
//        else
//        {
//            //重新设置DMA
//            DMA_Cmd(DMA_Stream_GPS_RX, DISABLE);
//            this_time_rx_len = DMA_GetCurrDataCounter(DMA_Stream_GPS_RX);
//            DMA_SetCurrDataCounter(DMA_Stream_GPS_RX, GPS_RX_BUFF_SIZE);
//            DMA_Cmd(DMA_Stream_GPS_RX, ENABLE);
//            if(this_time_rx_len == GPS_RX_BUFF_SIZE)	//校验接收
//            {
//                //处理数据
//               GPS_Data_Decode(GPS_rx_buff[0]);
//            }
//        }
//    }
//}