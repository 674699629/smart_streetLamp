#ifndef _BSP_645_H
#define _BSP_645_H

#include "main.h"
#include <stdio.h>
#define CMD_LEN    76
extern uint8_t flag;

typedef struct 
{
  uint8_t  Sta_Cha;       //起始符
	uint8_t  addr[6];       //地址
	uint8_t  Re_Sta_Cha;    //第二起始符
	uint8_t  cmd;           //控制码
	uint8_t  len;           //数据长度
	uint8_t  data[4];       //数据标识
	uint8_t  CR;            //校验
	uint8_t  End;           //结束符
	uint8_t  Rec_Buff[100]; //接收的数据
}Data_Frame;

typedef struct 
{
	float  Total;  //总的能量
	float  Top;    //尖峰时刻能量
	float  Peak;   //峰时的能量
	float  Flat;   //平时的能量
	float  Bottom; //谷时的能量
}Rec_Power_Frame;

typedef struct
{
	float    A_Phase;
	float    B_Phase;
	float    C_Phase;
}Rec_V_Frame;

typedef struct
{
	float    A_Phase;
	float    B_Phase;
	float    C_Phase;
}Rec_I_Frame;

typedef struct
{
	float Total;
	float A_Phase;
	float B_Phase;
	float C_Phase;
}Rec_InsPow_Frame;


typedef struct
{
	float  OverAll;
	float  A_Phase;
	float  B_Phase;
	float  C_Phase;
}Rec_PowerFactor_Frame;

typedef struct
{
	Rec_Power_Frame   Energy;
	Rec_V_Frame       Voltage;
	Rec_I_Frame       Current;
	Rec_InsPow_Frame  Power;
	Rec_PowerFactor_Frame PowerFactor;
}Rec_Data;

/*定义出开始符，重新起始符，以及地址管理*/
#define   START_CHAR        0x68
#define   RE_START_CHAR     0x68 
#define   ADDR_DEFINE       1

/*中断相关*/
#define RS485_INT_IRQ                 					USART2_IRQn
#define RS485_IRQHandler                        USART2_IRQHandler

#define UART_BUFF_SIZE      100


	/// 不精确的延时
static void RS485_delay(__IO uint32_t nCount)
{
	for(; nCount != 0; nCount--);
} 


void RS485_Config(void);
void RS485_SendByte(  uint8_t ch );
void RS485_SendStr_length( uint8_t *str,uint32_t strlen );
void RS485_SendString(  uint8_t *str, uint8_t num);
void bsp_RS485_IRQHandler(void);
char *get_rebuff(uint16_t *len);
void clean_rebuff(void);
void SendCmd(uint8_t *Cmd ,uint8_t  num);
void Read_Ack(Data_Frame *p,uint8_t len) ;
Data_Frame Cmd_Bucket(int a);
uint8_t Add_Fun(uint8_t *a,uint8_t *b);

Rec_Data  DataProcess(uint8_t *str,Data_Frame*  Temp);																 																							
//void  OutPut_Ack(Data_Frame *p,uint8_t len);																	 
void  Voltage_Process(Rec_Data *temp,uint8_t *str ,Data_Frame* Temp );
Rec_Data  Current_Process(uint8_t *str ,Data_Frame* Temp );
Rec_Data  Energy_Process(uint8_t *str ,Data_Frame* Temp );
Rec_Data  Power_Process(uint8_t *str ,Data_Frame* Temp );
Rec_Data  Power_Factor_Process(uint8_t *str ,Data_Frame* Temp );
int   atoi(const char *nptr);													 
float To_Real_part(uint8_t *str,uint16_t Res,uint8_t Len);


//void DataPut_Ack(Rec_Data *p,uint8_t len) ;




//float Transform(Data_Frame* Temp);



																			 
																			 
#endif  /*_BSP_645_H*/


