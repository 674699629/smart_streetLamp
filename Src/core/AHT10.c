#include "AHT10.h"
#include "IIC.h"
#include "gpio.h"

uint8_t AHT_State=0,AHT_DATA_FLAG=0;
float Humidity=0,Temperature=0;
__IO uint16_t AHT_TimeCnt=0;
extern uint8_t led_flag;
static iic_info_st aht_iic_info;

void AHT_Init(void)
{
	aht_iic_info.sda_port = SDA_T_GPIO_Port;
	aht_iic_info.sda_pin = SDA_T_Pin;
	aht_iic_info.scl_port = SCL_T_GPIO_Port;
	aht_iic_info.scl_pin = SCL_T_Pin;
	IIC_Init(&aht_iic_info);                                 //initial I2C circuit
	
	IIC_Start(&aht_iic_info);
	IIC_Send_Byte(&aht_iic_info,0x70);
	IIC_Send_Byte(&aht_iic_info,0xE1);
	IIC_Send_Byte(&aht_iic_info,0x08);
	IIC_Send_Byte(&aht_iic_info,0x00);
	IIC_Stop(&aht_iic_info);
}

void AHT_Reset(void)
{
//	IIC_Start();
//	IIC_Send_Byte(0x80);
//	IIC_Send_Byte(0xBA);
//	IIC_Stop();
}


void AHT_GetData(void)
{
	uint8_t i;
	uint32_t result,temp[6];

	if(AHT_TimeCnt>1000)
	{
		AHT_TimeCnt=0;
		AHT_DATA_FLAG=1;
		
		IIC_Start(&aht_iic_info);
		IIC_Send_Byte(&aht_iic_info,0x70);
		IIC_Send_Byte(&aht_iic_info,0xAC);
		IIC_Send_Byte(&aht_iic_info,0x33);
		IIC_Send_Byte(&aht_iic_info,0x00);
		IIC_Stop(&aht_iic_info);
	}
	
	
	
	if(AHT_DATA_FLAG && AHT_TimeCnt>100)
	{
		AHT_DATA_FLAG=0;
		
		IIC_Start(&aht_iic_info);
		IIC_Send_Byte(&aht_iic_info,0x71);
		for(i=0;i<5;i++)	
			temp[i] = IIC_Read_Byte(&aht_iic_info,1);
		temp[i] = IIC_Read_Byte(&aht_iic_info,0);
		IIC_Stop(&aht_iic_info);
		
		AHT_State = temp[0];
		
		result = ((temp[1] << 16) | (temp[2] << 8) | temp[3]) >> 4;
		Humidity = result * 100 / 1048576.0;
		
		result = ((temp[3] & 0x0F) << 16) | (temp[4] << 8) | temp[5];
		Temperature = ((200 * result) / 1048576.0) - 50;
		printf("Humidity:%0.1f,Temperatur:%0.1f.\r\n",Humidity,Temperature);
	}
	
	
}


