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

//IIC���в�������
extern void IIC_Init(iic_info_st *info);          				//��ʼ��IIC��IO��				 
extern void IIC_Start(iic_info_st *info);								//����IIC��ʼ�ź�
extern void IIC_Stop(iic_info_st *info);	  							//����IICֹͣ�ź�
extern void IIC_Send_Byte(iic_info_st *info,uint8_t txd);			//IIC����һ���ֽ�
extern uint8_t IIC_Read_Byte(iic_info_st *info,uint8_t ack);	//IIC��ȡһ���ֽ�


#endif


