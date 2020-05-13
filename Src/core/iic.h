#ifndef __IIC_H
#define __IIC_H
#include "stm32f0xx_hal.h"
#include "main.h"


typedef struct
{
    GPIO_TypeDef* sda_port;
    uint16_t sda_pin;
    GPIO_TypeDef* scl_port;
    uint16_t scl_pin;
}iic_info_st;

//IIC所有操作函数
extern void IIC_Init(iic_info_st *info);          				//初始化IIC的IO口				 
extern void IIC_Start(iic_info_st *info);								//发送IIC开始信号
extern void IIC_Stop(iic_info_st *info);	  							//发送IIC停止信号
extern void IIC_Send_Byte(iic_info_st *info,uint8_t txd);			//IIC发送一个字节
extern uint8_t IIC_Read_Byte(iic_info_st *info,uint8_t ack);	//IIC读取一个字节


#endif


