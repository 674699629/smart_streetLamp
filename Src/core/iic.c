#include "IIC.h"


//��ʼ��IIC
void IIC_Init(iic_info_st *info)
{
	
	HAL_GPIO_WritePin(info->scl_port,info->scl_pin,GPIO_PIN_SET);
	HAL_GPIO_WritePin(info->sda_port,info->sda_pin,GPIO_PIN_SET);
}

/*******************************************************************************
* �� �� ��         : SDA_OUT
* ��������         : SDA ����Ϊ���
* ��    ��         : ��
* ��    ��         : ��
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
* �� �� ��         : SDA_IN
* ��������         : SDA ����Ϊ����
* ��    ��         : ��
* ��    ��         : ��
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

//����IIC��ʼ�ź�
void IIC_Start(iic_info_st *info)
{
	//SDA_OUT();
	HAL_GPIO_WritePin(info->sda_port,info->sda_pin,GPIO_PIN_SET);	  	  
	HAL_GPIO_WritePin(info->scl_port,info->scl_pin,GPIO_PIN_SET);
	IIC_delay();
 	HAL_GPIO_WritePin(info->sda_port,info->sda_pin,GPIO_PIN_RESET);//START:when CLK is high,DATA change form high to low 
	IIC_delay();
	HAL_GPIO_WritePin(info->scl_port,info->scl_pin,GPIO_PIN_RESET);//ǯסI2C���ߣ�׼�����ͻ�������� 
	IIC_delay();
}

//����IICֹͣ�ź�
void IIC_Stop(iic_info_st *info)
{
	//SDA_OUT();
	HAL_GPIO_WritePin(info->scl_port,info->scl_pin,GPIO_PIN_RESET);
	HAL_GPIO_WritePin(info->sda_port,info->sda_pin,GPIO_PIN_RESET);//STOP:when CLK is high DATA change form low to high
 	IIC_delay();
	HAL_GPIO_WritePin(info->scl_port,info->scl_pin,GPIO_PIN_SET); 
	IIC_delay();
	HAL_GPIO_WritePin(info->sda_port,info->sda_pin,GPIO_PIN_SET);//����I2C���߽����ź�
	IIC_delay();							   	
}

//�ȴ�Ӧ���źŵ���
//����ֵ��1������Ӧ��ʧ��
//        0������Ӧ��ɹ�
void IIC_Wait_Ack(iic_info_st *info)
{
	HAL_GPIO_WritePin(info->scl_port,info->scl_pin,GPIO_PIN_RESET);
	HAL_GPIO_WritePin(info->sda_port,info->sda_pin,GPIO_PIN_RESET);
	IIC_delay();
	HAL_GPIO_WritePin(info->scl_port,info->scl_pin,GPIO_PIN_SET);
	IIC_delay();
	HAL_GPIO_WritePin(info->scl_port,info->scl_pin,GPIO_PIN_RESET);
}

//����ACKӦ��
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

//������ACKӦ��		    
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

//IIC����һ���ֽ�
//���شӻ�����Ӧ��
//1����Ӧ��
//0����Ӧ��
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

//��1���ֽڣ�ack=1ʱ������ACK��ack=0������nACK   
uint8_t IIC_Read_Byte(iic_info_st *info,uint8_t ack)
{
	uint8_t i,receive=0;
	
	SDA_IN(info);//SDA����Ϊ����
  	for(i=0;i<8;i++ )
	{
		HAL_GPIO_WritePin(info->scl_port,info->scl_pin,GPIO_PIN_RESET);
		IIC_delay();
		HAL_GPIO_WritePin(info->scl_port,info->scl_pin,GPIO_PIN_SET);
		IIC_delay();
		receive<<=1;
		if(HAL_GPIO_ReadPin(info->sda_port,info->sda_pin)) receive++;   
	}					 
	if(ack) IIC_Ack(info); //����ACK 
	else IIC_NAck(info);	//����nACK
	
	
	return receive;
}
