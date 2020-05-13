#include "IIC.h"


//初始化IIC
void IIC_Init(iic_info_st *info)
{
	
	HAL_GPIO_WritePin(info->scl_port,info->scl_pin,GPIO_PIN_SET);
	HAL_GPIO_WritePin(info->sda_port,info->sda_pin,GPIO_PIN_SET);
}

/*******************************************************************************
* 函 数 名         : SDA_OUT
* 函数功能         : SDA 设置为输出
* 输    入         : 无
* 输    出         : 无
*******************************************************************************/
void SDA_OUT(iic_info_st *info)
{ 
	GPIO_InitTypeDef GPIO_InitStruct;
	GPIO_InitStruct.Pin = info->sda_pin;                   
	GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
	HAL_GPIO_Init(info->sda_port, &GPIO_InitStruct);
}


/*******************************************************************************
* 函 数 名         : SDA_IN
* 函数功能         : SDA 设置为输入
* 输    入         : 无
* 输    出         : 无
*******************************************************************************/
void SDA_IN(iic_info_st *info)  
{
	GPIO_InitTypeDef GPIO_InitStruct;
	GPIO_InitStruct.Pin = info->sda_pin;                    
	GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
	HAL_GPIO_Init(info->sda_port, &GPIO_InitStruct);
} 

void IIC_delay(void)
{
	uint16_t i;
	for(i=0;i<200;i++);
}

//产生IIC起始信号
void IIC_Start(iic_info_st *info)
{
	//SDA_OUT();
	HAL_GPIO_WritePin(info->sda_port,info->sda_pin,GPIO_PIN_SET);	  	  
	HAL_GPIO_WritePin(info->scl_port,info->scl_pin,GPIO_PIN_SET);
	IIC_delay();
 	HAL_GPIO_WritePin(info->sda_port,info->sda_pin,GPIO_PIN_RESET);//START:when CLK is high,DATA change form high to low 
	IIC_delay();
	HAL_GPIO_WritePin(info->scl_port,info->scl_pin,GPIO_PIN_RESET);//钳住I2C总线，准备发送或接收数据 
	IIC_delay();
}

//产生IIC停止信号
void IIC_Stop(iic_info_st *info)
{
	//SDA_OUT();
	HAL_GPIO_WritePin(info->scl_port,info->scl_pin,GPIO_PIN_RESET);
	HAL_GPIO_WritePin(info->sda_port,info->sda_pin,GPIO_PIN_RESET);//STOP:when CLK is high DATA change form low to high
 	IIC_delay();
	HAL_GPIO_WritePin(info->scl_port,info->scl_pin,GPIO_PIN_SET); 
	IIC_delay();
	HAL_GPIO_WritePin(info->sda_port,info->sda_pin,GPIO_PIN_SET);//发送I2C总线结束信号
	IIC_delay();							   	
}

//等待应答信号到来
//返回值：1，接收应答失败
//        0，接收应答成功
void IIC_Wait_Ack(iic_info_st *info)
{
	HAL_GPIO_WritePin(info->scl_port,info->scl_pin,GPIO_PIN_RESET);
	HAL_GPIO_WritePin(info->sda_port,info->sda_pin,GPIO_PIN_RESET);
	IIC_delay();
	HAL_GPIO_WritePin(info->scl_port,info->scl_pin,GPIO_PIN_SET);
	IIC_delay();
	HAL_GPIO_WritePin(info->scl_port,info->scl_pin,GPIO_PIN_RESET);
}

//产生ACK应答
void IIC_Ack(iic_info_st *info)
{
	HAL_GPIO_WritePin(info->scl_port,info->scl_pin,GPIO_PIN_RESET);
	IIC_delay();
	SDA_OUT(info);
	HAL_GPIO_WritePin(info->sda_port,info->sda_pin,GPIO_PIN_RESET);
	IIC_delay();
	HAL_GPIO_WritePin(info->scl_port,info->scl_pin,GPIO_PIN_SET);
	IIC_delay();
	HAL_GPIO_WritePin(info->scl_port,info->scl_pin,GPIO_PIN_RESET);
	IIC_delay();
}

//不产生ACK应答		    
void IIC_NAck(iic_info_st *info)
{
	HAL_GPIO_WritePin(info->scl_port,info->scl_pin,GPIO_PIN_RESET);
	IIC_delay();
	SDA_OUT(info);
	HAL_GPIO_WritePin(info->sda_port,info->sda_pin,GPIO_PIN_SET);
	IIC_delay();
	HAL_GPIO_WritePin(info->scl_port,info->scl_pin,GPIO_PIN_SET);
	IIC_delay();
	HAL_GPIO_WritePin(info->scl_port,info->scl_pin,GPIO_PIN_RESET);
	IIC_delay();
}

//IIC发送一个字节
//返回从机有无应答
//1，有应答
//0，无应答
void IIC_Send_Byte(iic_info_st *info,uint8_t txd)
{                        
	uint8_t i;

	SDA_OUT(info);
	HAL_GPIO_WritePin(info->scl_port,info->scl_pin,GPIO_PIN_RESET);
	for(i=0;i<8;i++)
	{           
		if(txd&0x80) HAL_GPIO_WritePin(info->sda_port,info->sda_pin,GPIO_PIN_SET);
		else HAL_GPIO_WritePin(info->sda_port,info->sda_pin,GPIO_PIN_RESET);
		txd<<=1; 
		IIC_delay();  
		HAL_GPIO_WritePin(info->scl_port,info->scl_pin,GPIO_PIN_SET);
		IIC_delay();
		HAL_GPIO_WritePin(info->scl_port,info->scl_pin,GPIO_PIN_RESET);	
		IIC_delay();
	}	 
	IIC_Wait_Ack(info);
}

//读1个字节，ack=1时，发送ACK，ack=0，发送nACK   
uint8_t IIC_Read_Byte(iic_info_st *info,uint8_t ack)
{
	uint8_t i,receive=0;
	
	SDA_IN(info);//SDA设置为输入
  	for(i=0;i<8;i++ )
	{
		HAL_GPIO_WritePin(info->scl_port,info->scl_pin,GPIO_PIN_RESET);
		IIC_delay();
		HAL_GPIO_WritePin(info->scl_port,info->scl_pin,GPIO_PIN_SET);
		IIC_delay();
		receive<<=1;
		if(HAL_GPIO_ReadPin(info->sda_port,info->sda_pin)) receive++;   
	}					 
	if(ack) IIC_Ack(info); //发送ACK 
	else IIC_NAck(info);	//发送nACK
	
	
	return receive;
}
