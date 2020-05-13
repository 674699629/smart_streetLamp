#include "rs485Handle.h"
#include "usart.h"
#include "bsp_645.h"

//============================================================
//函数名称 :Float32
//函数功能 :将32位的float转换为10进制正数(0~99999)舍弃小数,IEEE 754标准
//输入变量 :NumFloat32:待转换32位浮点数	
//返 回 值 :计算结果 无符号整形 u32 ,返回-1为负数或者超限；
//调试记录 :
//============================================================
unsigned int Float32(const unsigned int NumFloat32)
{
	
	unsigned char  sbit,ebits;
	unsigned int mbits,result;
 
	sbit = NumFloat32 >> 31;
	ebits =  ( NumFloat32 >> 23 ) & 0xff ;
	mbits =  NumFloat32 & 0x7fffff ; 
 
	if(sbit)    //若需要负数部分，sbit为1时，result*-1,去掉判断；
	{
		return -1 ;
	}
	ebits -= 127;    //得到阶码<0，即结果小于1，返回0；
	if(ebits < 0)
	{
		return 0;
	}
	result = (1 << ebits) +(mbits >> (23 - ebits));     
    /*若有小数位数精度要求，mbits的0~(23 - ebits)位可以适当保留处理；可以进行自定义处理
    result = (1 << ebits) +(mbits >> (23 - ebits)); 改为 result = (1 << (ebits + 10))* 1000 / 1024 + (mbits >> (13 - ebits)) * 1000 / 1024 ; 
    结果扩大了1000倍，result/1000为整数部分，result%1000为小数*/
	if(result > 99999)
	{
		return -1 ;
	}
	printf("result = %d\n",result);
	
	return result;	
}

/**
**************************************************************************************************
* @Brief    Single byte data inversion        
* @Param    
*            @DesBuf: destination buffer
*            @SrcBuf: source buffer
* @RetVal    None
* @Note      (MSB)0101_0101 ---> 1010_1010(LSB)
**************************************************************************************************
*/
void InvertUint8(unsigned char *DesBuf, unsigned char *SrcBuf)
{
    int i;
    unsigned char temp = 0;
    
    for(i = 0; i < 8; i++)
    {
        if(SrcBuf[0] & (1 << i))
        {
            temp |= 1<<(7-i);
        }
    }
    DesBuf[0] = temp;
}

/**
**************************************************************************************************
* @Brief    double byte data inversion        
* @Param    
*            @DesBuf: destination buffer
*            @SrcBuf: source buffer
* @RetVal    None
* @Note      (MSB)0101_0101_1010_1010 ---> 0101_0101_1010_1010(LSB)
**************************************************************************************************
*/
void InvertUint16(unsigned short *DesBuf, unsigned short *SrcBuf)  
{  
    int i;  
    unsigned short temp = 0;    
    
    for(i = 0; i < 16; i++)  
    {  
        if(SrcBuf[0] & (1 << i))
        {          
            temp |= 1<<(15 - i);  
        }
    }  
    DesBuf[0] = temp;  
}
 

unsigned short CRC16_MODBUS(unsigned char *puchMsg, unsigned int usDataLen)  
{  
    unsigned short wCRCin = 0xFFFF;  
    unsigned short wCPoly = 0x8005;  
    unsigned char wChar = 0;  
    
    while (usDataLen--)     
    {  
        wChar = *(puchMsg++);  
        InvertUint8(&wChar, &wChar);  
        wCRCin ^= (wChar << 8); 
        
        for(int i = 0; i < 8; i++)  
        {  
            if(wCRCin & 0x8000) 
            {
                wCRCin = (wCRCin << 1) ^ wCPoly;  
            }
            else  
            {
                wCRCin = wCRCin << 1; 
            }            
        }  
    }  
    InvertUint16(&wCRCin, &wCRCin);  
    return (wCRCin) ;  
} 



#define Swap32(A) ((((uint32_t)(A) & 0xff000000) >> 24) | \
				   (((uint32_t)(A) & 0x00ff0000) >>  8) | \
				   (((uint32_t)(A) & 0x0000ff00) <<  8) | \
				   (((uint32_t)(A) & 0x000000ff) << 24))
 

#define RS485_ADDR 0X29
extern Data_Frame Power;
void rs485UartRxHandle()
{
    uint8_t userBuf[50] = {0};
    Rec_Data   Data_Test;
    // Data_Frame VPower;
    uint16_t datalen = jcFifoUsedLen(&rs485RxFifo);
    printf("datalen:%d\r\n",datalen);
    if( datalen < 16)
    {
        return;
    }
    datalen = jcFifoReadByte_del(&rs485RxFifo,userBuf,datalen);
    if( datalen > 0)
    {
        for(int i=0;i<datalen;i++)
        {
            printf("%02x",userBuf[i]);
        }
        printf("\r\n");
        //FE FE FE FE
        // if(userBuf[0]==0xFE && userBuf[1]==0xFE && userBuf[2]==0xFE && userBuf[3]==0xFE)
        // {
            // for(int i=4;i<datalen;i++)
            // {
            //     printf("%02x",userBuf[i]);
            // }
            // printf("\r\n");
            Data_Test = DataProcess(userBuf,&Power);
            printf("Voltage,A:%f,B:%f,C:%f\r\n",Data_Test.Voltage.A_Phase,Data_Test.Voltage.B_Phase,Data_Test.Voltage.C_Phase);
            // memset(&Power,0,sizeof(Data_Frame));
        // }
        
        
        // if(userBuf[0] == RS485_ADDR && userBuf[1] == 0x04)
        // {
        //     uint8_t valueLen = userBuf[2];
        //     uint8_t value[4] = {0};
        //     uint32_t valueInt;
        //     memcpy(value,&userBuf[3],valueLen);
        //     memcpy(&valueInt,&userBuf[3],valueLen);
        //     valueInt = Swap32(valueInt);
        //     printf("value:0x%08x,Float32:%d\r\n",valueInt,Float32(valueInt));
        // }
    }
}

uint8_t sendBuf[8] = {0};
void rs485UartGetData(uint16_t addr)
{
    sendBuf[0] = RS485_ADDR;
    sendBuf[1] = 0x04;
    sendBuf[2] = addr;
    sendBuf[3] = addr>> 8;
    sendBuf[4] = 0x0;
    sendBuf[5] = 0x02;
    uint16_t crc = CRC16_MODBUS(sendBuf,6);
    sendBuf[6] = crc;
    sendBuf[7] = crc >> 8;
    HAL_UART_Transmit(&huart1,sendBuf,8,0xffff);
}






