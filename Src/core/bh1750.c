#include "BH1750.h"
#include "IIC.h"
#include "gpio.h"

extern __IO uint16_t AHT_TimeCnt;
uint8_t BH1750_BUF[2];  
static iic_info_st bh_iic_info;


void BH1750_Init(void)
{
	bh_iic_info.sda_port = SDA_GPIO_Port;
	bh_iic_info.sda_pin = SDA_Pin;
	bh_iic_info.scl_port = SCL_GPIO_Port;
	bh_iic_info.scl_pin = SCL_Pin;
	HAL_GPIO_WritePin(ADDR_GPIO_Port,ADDR_Pin,GPIO_PIN_RESET);
	IIC_Init(&bh_iic_info); 
}

void BH1750_delayms(uint32_t ms)
{
	uint16_t i;
	for(i=0;i<ms*20;i++)
	{
		IIC_delay();
	}
}

void BH1750_Single_Write(uint8_t REG_Address)
{
    IIC_Start(&bh_iic_info);                  //起始信号
    IIC_Send_Byte(&bh_iic_info,0x46);          //发送设备地址+写信号
    IIC_Send_Byte(&bh_iic_info,REG_Address);   //内部寄存器地址，
    IIC_Stop(&bh_iic_info);                   //发送停止信号
}

void BH1750_Multiple_Read(void)
{
    IIC_Start(&bh_iic_info);
    IIC_Send_Byte(&bh_iic_info,0x46);
	IIC_Send_Byte(&bh_iic_info,0x10);
    IIC_Stop(&bh_iic_info);
    BH1750_delayms(200);
	IIC_Start(&bh_iic_info);
    IIC_Send_Byte(&bh_iic_info,0x47);
	BH1750_BUF[0]=IIC_Read_Byte(&bh_iic_info,1);
	BH1750_BUF[1]=IIC_Read_Byte(&bh_iic_info,0);	
    IIC_Stop(&bh_iic_info);
}

void BH1750_GetData(void)
{
	if(AHT_TimeCnt>1000)
	{
		AHT_TimeCnt=0;
		uint16_t temp=0;
		float lux=0;

		BH1750_Single_Write(0x01);   // power on
		BH1750_Single_Write(0x10);   // H- resolution mode
		BH1750_Multiple_Read();
		BH1750_delayms(200);
		temp=BH1750_BUF[0];
		temp=(temp<<8)+BH1750_BUF[1];
		lux=(float)temp/1.2;    //lux是float型	
		printf("%f lux.\r\n",lux);
	}
}


