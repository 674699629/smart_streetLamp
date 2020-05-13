#include "atHandle.h"
#include "usart.h"

at_cmd_t at_table[] = {
	{"AT+CSQ",  deal_csq_func},
    {"ATE1",  deal_ate_func},
    {"AT+GSN", deal_gsn_func},
    {"AT+CPIN", deal_cpin_func},
    {"AT+CGREG",deal_cgreg_func},
    {"AT+CIPSTART",deal_sock_start_func},
    {"AT+CIPSEND",deal_cip_send_func},
    {"ALREADY",deal_sock_already_func},
    {"CONNECT",deal_sock_connect_func},
    {"SEND",deal_sock_send_func},
    {"CLOSED",deal_sock_close_func},
    {"+PDP：DEACT",deal_pdp_deact_func},
    {"AT+CIPSTATUS",deal_cip_status_func},
    {"AT+CIPSHUT",deal_cip_shut_func},
    
    
};

unsigned char atRecvNorFlag = 0;
unsigned char signal = 0;
unsigned char devId[4] = {0};
/*
GSM模块初始化流程：
1.打开回显
2.查询模块IMEI
3.查询SIM卡
4.查询信号
5.查询基站
6.建立TCP请求
*/

int atTableCmdNum()
{
    return (sizeof(at_table) / sizeof(at_cmd_t));
}

int atCmdSendStr(unsigned char * buf)
{
    HAL_UART_Transmit(&huart2,buf,strlen(buf),0xffff);
}

int atCmdSendbyte(unsigned char byte)
{
    HAL_UART_Transmit(&huart2,&byte,1,0xffff);
}


int atCmdSendHex(unsigned char * buf,unsigned int len)
{
    HAL_UART_Transmit(&huart2,buf,len,0xffff);
}


//计算校验和
uint8_t CheakSum(uint8_t *Head_Device,uint8_t CMD,uint8_t *Serial_Number,uint8_t *UartTxdatabuf,uint8_t TCP_End)
{
   unsigned char len = 0,i = 0;
   unsigned int sum = 0;

   for(i=0; i<5; i++)
      sum = sum + Head_Device[i];
   sum += CMD;
   for(i=0; i<4; i++)
      sum = sum + Serial_Number[i];
   for(i=0; i<1; i++)
      sum = sum + UartTxdatabuf[i];
   sum += TCP_End;  

   return (unsigned char)(sum & 0x00ff); //将所有数据字节和的低字节取反
}
int atCmdsendTcpData(unsigned char *data,unsigned char cmd)
{
    printf("sendbuf:%s.\r\n",data);
    atCmdSendStr(AT_CMD_CIPSEND);
    HAL_Delay(100);
    //19120022
    //aa 00 04 19 12 00 22 00 00 00 00 01 7b 22 64 65 76 69 63 65 5f 74 79 70 65 22 3a 22 30 30 30 34 22 2c 22 55 55 49 44 22 3a 22 31 39 31 32 30 30 32 32 22 7d 00 00 1a 0d 0a
    atCmdSendbyte(0xAA);
    atCmdSendbyte(0x00);
    atCmdSendbyte(0x04);
    // atCmdSendbyte(0x19);
    // atCmdSendbyte(0x12);
    // atCmdSendbyte(0x00);
    // atCmdSendbyte(0x22);
    atCmdSendHex(devId,4);
    atCmdSendbyte(cmd);
    atCmdSendbyte(0X00);
    atCmdSendbyte(0X00);
    atCmdSendbyte(0X00);
    atCmdSendbyte(0X01);
    atCmdSendStr(data);
    atCmdSendbyte(0X00);
    atCmdSendbyte(0X00);//效验和
    atCmdSendbyte(0X1A);
    atCmdSendbyte(0X0D);
    atCmdSendbyte(0X0A);
}

int atCmdRecvTcpData(unsigned char *data,unsigned int dataLen)
{
    //开锁BB 19 12 00 22 00 11 11 11 11 40 B3
    printf("recv cloud data,cmd:%02x.\r\n",data[10]);
    // for(int i=0;i<dataLen;i++)
    // {
    //     printf("%02x",data[i]);
    // }
    switch (data[10])
    {
        case 0x41:
        {
            printf("this is open lock1 cmd.\r\n");
            HAL_GPIO_WritePin(LOCK1_GPIO_Port,LOCK1_Pin,1);
            HAL_Delay(500);
            HAL_GPIO_WritePin(LOCK1_GPIO_Port,LOCK1_Pin,0);
        }
        break;
        case 0x42:
        {
            printf("this is open lock2 cmd.\r\n");
            HAL_GPIO_WritePin(LOCK2_GPIO_Port,LOCK2_Pin,1);
            HAL_Delay(500);
            HAL_GPIO_WritePin(LOCK2_GPIO_Port,LOCK2_Pin,0);
        }
        break;
        case 0x43:
        {
            printf("this is open relay1 cmd.\r\n");
            HAL_GPIO_WritePin(RELAY1_GPIO_Port,RELAY1_Pin,1);
        }
        break;
        case 0x44:
        {
            printf("this is close relay1 cmd.\r\n");
            HAL_GPIO_WritePin(RELAY1_GPIO_Port,RELAY1_Pin,0);
        }
        break;
        case 0x45:
        {
            printf("this is open relay2 cmd.\r\n");
            HAL_GPIO_WritePin(RELAY2_GPIO_Port,RELAY2_Pin,1); 
        }
        break;
        case 0x46:
        {
            printf("this is close relay2 cmd.\r\n");
            HAL_GPIO_WritePin(RELAY2_GPIO_Port,RELAY2_Pin,0); 
        }
        break;
        default:
        break;
    }
    
}

int deal_csq_func(int cmd, int result, int argc, char *argv[],unsigned char cmdBuf[][AT_RX_BUF_SIZE])
{
    // printf("%s cmd:%d,result:%d\r\n",__func__,cmd,result);
    // for(int i=0;i<argc;i++)
    // {
    //     printf("i:%d,argv:%s\r\n",i,argv[i]);
    // }
    int csqInt = atoi(argv[0]);
    printf("csq:%d.\r\n",csqInt);
    signal = csqInt;
    if( csqInt > 10 && csqInt < 31 )//可用信号范围暂定为10~31
    {
        atCmdSendStr(AT_CMD_CGREG);
    }
    else
    {
        HAL_Delay(1000);
        atCmdSendStr(AT_CMD_CSQ);
    }
}

int deal_ate_func(int cmd, int result, int argc, char *argv[],unsigned char cmdBuf[][AT_RX_BUF_SIZE])
{
    // printf("%s cmd:%d,result:%d\r\n",__func__,cmd,result);
    atRecvNorFlag = 1;
    atCmdSendStr(AT_CMD_GSN);
}

int deal_gsn_func(int cmd, int result, int argc, char *argv[],unsigned char cmdBuf[][AT_RX_BUF_SIZE])
{
    // printf("%s cmd:%d,result:%d\r\n",__func__,cmd,result);
    printf("imei:%s\r\n",cmdBuf[1]);
    int i;
    for ( i = 0; i < 15; i++)
    {
        if(cmdBuf[1][i] <= '0' && cmdBuf[1][i] >= '9')
        {
            break;
        }
    }
    if(i == 15)
    {
        printf("get imei success,get sim card.\r\n");
        uint8_t temNum = (cmdBuf[1][7]-0x30)*16+(cmdBuf[1][8]-0x30);
        devId[0] = temNum;
        temNum = (cmdBuf[1][9]-0x30)*16+(cmdBuf[1][10]-0x30);
        devId[1] = temNum;
        temNum = (cmdBuf[1][11]-0x30)*16+(cmdBuf[1][12]-0x30);
        devId[2] = temNum;
        temNum = (cmdBuf[1][13]-0x30)*16+(cmdBuf[1][14]-0x30);
        devId[3] = temNum;
        printf("devId:%08x.\r\n",devId);
        atCmdSendStr(AT_CMD_PIN);
    }
    else
    {
        printf("get imei fail,retry.\r\n");
        HAL_Delay(1000);
        atCmdSendStr(AT_CMD_GSN);
    }
}

int deal_cpin_func(int cmd, int result, int argc, char *argv[],unsigned char cmdBuf[][AT_RX_BUF_SIZE])
{
    // printf("%s cmd:%d,result:%d\r\n",__func__,cmd,result);
    printf("sim card status:%s\r\n",cmdBuf[1]);
    if(strstr(cmdBuf[1],"READY") != NULL)
    {
        printf("get sim card success,get csq.\r\n");
        atCmdSendStr(AT_CMD_CSQ);
    }
    else
    {
        printf("get sim card fail,retry.\r\n");
        HAL_Delay(1000);
        atCmdSendStr(AT_CMD_PIN);
    }
}

int deal_cgreg_func(int cmd, int result, int argc, char *argv[],unsigned char cmdBuf[][AT_RX_BUF_SIZE])
{
    // printf("%s cmd:%d,result:%d\r\n",__func__,cmd,result);
    // for(int i=0;i<argc;i++)
    // {
    //     printf("i:%d,argv:%s\r\n",i,argv[i]);
    // }
    printf("cgreg:%s,%s.\r\n",argv[0],argv[1]);
		int cgregInt = atoi(argv[1]);
    if( cgregInt == 1 )//可用信号范围暂定为10~31
    {
        printf("The network is working. Start connecting to the cloud.\r\n");
        atCmdSendStr(AT_CMD_CIPSTART);
    }
    else
    {
        HAL_Delay(1000);
        atCmdSendStr(AT_CMD_CGREG);
    }
}


int deal_sock_start_func(int cmd, int result, int argc, char *argv[],unsigned char cmdBuf[][AT_RX_BUF_SIZE])
{
    printf("%s cmd:%d,result:%d\r\n",__func__,cmd,result);
}

int deal_sock_connect_func(int cmd, int result, int argc, char *argv[],unsigned char cmdBuf[][AT_RX_BUF_SIZE])
{
    printf("%s cmd:%d,result:%d\r\n",__func__,cmd,result);
    // atCmdsendTcpData("{\"device_type\":\"0004\", \"UUID\":\"19120022\"}",0);
    uint8_t sendBuf[100]  = {0};
    sprintf(sendBuf,"{\"device_type\":\"0004\", \"UUID\":\"%02x%02x%02x%02x\"}",devId[0],devId[1],devId[2],devId[3]);
    printf("sendBuf:%s.\r\n",sendBuf);
    atCmdsendTcpData(sendBuf,0);
}

int deal_sock_send_func(int cmd, int result, int argc, char *argv[],unsigned char cmdBuf[][AT_RX_BUF_SIZE])
{
    printf("%s cmd:%d,result:%d\r\n",__func__,cmd,result);
}

int deal_sock_close_func(int cmd, int result, int argc, char *argv[],unsigned char cmdBuf[][AT_RX_BUF_SIZE])
{
    printf("%s cmd:%d,result:%d\r\n",__func__,cmd,result);
    atCmdSendStr(AT_CMD_CIPSTART);
}

int deal_sock_already_func(int cmd, int result, int argc, char *argv[],unsigned char cmdBuf[][AT_RX_BUF_SIZE])
{
    printf("%s cmd:%d,result:%d\r\n",__func__,cmd,result);
    // atCmdsendTcpData("{\"device_type\":\"0004\", \"UUID\":\"19120022\"}",0);
    uint8_t sendBuf[100]  = {0};
    sprintf(sendBuf,"{\"device_type\":\"0004\", \"UUID\":\"%02x%02x%02x%02x\"}",devId[0],devId[1],devId[2],devId[3]);
    printf("sendBuf:%s.\r\n",sendBuf);
    atCmdsendTcpData(sendBuf,0);
}

int deal_cip_send_func(int cmd, int result, int argc, char *argv[],unsigned char cmdBuf[][AT_RX_BUF_SIZE])
{
    printf("%s cmd:%d,result:%d\r\n",__func__,cmd,result);
    if(result == AT_RESULT_ERROR)
    {
        printf("send fail,retry.\r\n");
        atCmdSendStr(AT_CMD_CIPSHUT);
    }
}

int deal_pdp_deact_func(int cmd, int result, int argc, char *argv[],unsigned char cmdBuf[][AT_RX_BUF_SIZE])
{
    printf("%s cmd:%d,result:%d\r\n",__func__,cmd,result);
    atCmdSendStr(AT_CMD_CIPSHUT);
}

int deal_cip_status_func(int cmd, int result, int argc, char *argv[],unsigned char cmdBuf[][AT_RX_BUF_SIZE])
{
    printf("%s cmd:%d,result:%d\r\n",__func__,cmd,result);
}

int deal_cip_shut_func(int cmd, int result, int argc, char *argv[],unsigned char cmdBuf[][AT_RX_BUF_SIZE])
{
    printf("%s cmd:%d,result:%d\r\n",__func__,cmd,result);
    atCmdSendStr(AT_CMD_CIPSTART);
}
