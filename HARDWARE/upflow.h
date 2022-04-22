#ifndef __UPFLOW_H
#define __UPFLOW_H

#include "main.h"

#define  RINGBUFF_LEN   50     //�����������ֽ��� 50
typedef struct
{
  uint16_t Head;           
  uint16_t Tail;
  uint16_t Lenght;
  uint8_t  Ring_Buff[RINGBUFF_LEN];
}RingBuff_t;

#define USER_RINGBUFF  1  //ʹ�û��λ�������ʽ��������
#if  USER_RINGBUFF
/**���ʹ�û��λ�����ʽ���մ�������***/
void RingBuff_Init(RingBuff_t *ringBuff);
u8 Write_RingBuff(uint8_t data,RingBuff_t *ringBuff);
u8 Read_RingBuff(uint8_t *rData,RingBuff_t *ringBuff);
#endif


void RingBuf_Write(unsigned char data,RingBuff_t *ringBuff,uint16_t Length);
u8 RingBuf_Read(unsigned char* pData,RingBuff_t *ringBuff);


typedef struct{
unsigned short frame_count_since_last_readout;
signed short pixel_flow_x_integral;
signed short pixel_flow_y_integral;
signed short gyro_x_rate_integral;
signed short gyro_y_rate_integral;
signed short gyro_z_rate_integral;
unsigned int integration_timespan;
unsigned int sonar_timestamp;
unsigned short ground_distance;
signed short gyro_temperature;
unsigned char qual;
}flow_integral_frame;

typedef struct{
float x;
float y;
unsigned short dt;
unsigned char qual;
unsigned char update;
}flow_float;


typedef struct
{
 float Input_Butter[3];
 float Output_Butter[3];
}Butter_BufferData;

typedef struct
{
  float a[3];
  float b[3];
}Butter_Parameter;


typedef struct
{
  float x;
  float y;
}Vector2f;
extern flow_integral_frame opt_origin_data;
extern Vector2f opt_filter_data; //�˲��������λ��
extern Vector2f opt_gyro_data;   //�������ٶ�
extern RingBuff_t OpticalFlow_Ringbuf;

void UPFLOW_Init();

typedef  struct
{
	uint8_t header;
	float pit_offset;
	float yaw_offset;
	uint8_t buff_flag;
	uint8_t tail;
}UPFLOW_Info_TypeDef;

void UPFLOW_Send_Data();
extern UPFLOW_Info_TypeDef UPFLOW_Data;
uint8_t LC306_Config_Init(void);
uint8_t Optflow_Prase(void);
void OpticalFlow_Init(void);
#endif