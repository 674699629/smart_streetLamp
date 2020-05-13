#include "atProcess.h"
#include <stdio.h>
#include <string.h>
#include "atHandle.h"


static unsigned char cmdbuf[AT_RX_BUF_CNT][AT_RX_BUF_SIZE];
extern unsigned int sysTimeCnt;
#define HAL_GetTick()     sysTimeCnt

/*
 * @brief 字符串拆分解析处理
 * @return 检测归类的参数个数
 **/
int string_split(char *strp, unsigned int strsize, char ch, char *argv[], unsigned int argcM )
{
	int ch_index = 0;
	int argc_index = 0;
	unsigned char splitflag = 0;
	
	if ((!strsize) || (!argcM)) return 0;

	argv[argc_index++] = &strp[ch_index];
	for (ch_index = 0; ch_index < strsize; ch_index++) 
    {
		if (strp[ch_index] == ch) 
        {
			strp[ch_index] = '\0';
            #if 1	
            /* 处理连续出现ch情况;
            打开这个宏,则空参数也会作为一个空的参数回调,如果1,,3会有3个参数;
            关闭这个宏,则空参数会自动省去,如1,,3会只有2个参数; */
			if (1 == splitflag) {
				argv[argc_index++] = &strp[ch_index];
			}
            #endif			
			splitflag = 1;
		} 
        else if (splitflag == 1) 
        {
			splitflag = 0;
			argv[argc_index++] = &strp[ch_index];
			if (argc_index >= argcM) break;
		} 
        else 
        {
			splitflag = 0;
		}
	}
	
	return argc_index;
}

int at_cmd_parse(unsigned char pdata[][AT_RX_BUF_SIZE], unsigned short size)
{
	int ret  = -1;
	char *ptr = NULL;
	int argc = 0;
	unsigned short offset = 0;
	int index = 0;
	char *argv[ARGC_LIMIT] = { (char *)0 };

    int cmdClass = -1;
    int result = -1;

    // if (strstr((const char *)pdata[0], "AT") == NULL &&
    //     strstr((const char *)pdata[0], "+") == NULL )
    // {
    //     printf("find header error.\r\n");
    //     goto at_end;
    // }

    for (index = 0; index < atTableCmdNum(); index++) 
    {
        ptr = strstr((const char *)pdata[0], at_table[index].cmd);
        if (ptr != NULL) 
        {
            printf("find cmd:%s.\r\n",at_table[index].cmd);
            ptr += strlen(at_table[index].cmd);
            break;
        }
    }
    if (index >= atTableCmdNum()) 
    {
        printf("find table error.\r\n");
        goto at_end;
    }
    
    if ((ptr[0] == '?') && (ptr[1] == '\r') && (ptr[2] == '\n')) 
    {
        /* 解析查询命令 */
        cmdClass = QUERY_CMD;
    } 
    else if ((ptr[0] == '=') && (ptr[1] == '?') && (ptr[2] == '\r') && (ptr[3] == '\n')) 
    { 
        /* 解析测试命令 */
        cmdClass = TEST_CMD;
    } 
    else if (ptr[0] == '=') 
    { 
        /* 解析设置命令 */
        cmdClass = SET_CMD;
    } 
    else if (ptr[0] == '>') //send >
    { 
        /* 解析设置命令 */
        cmdClass = SET_CMD;
    } 
    else if (pdata[0][0] == '+') 
    {
        /* 解析接收命令 */
        cmdClass = RECV_CMD;
    }
    else if(ptr[0] == '\r' && ptr[1] == '\n')//查询指令不一定带?
    {
        /* 解析查询命令 */
        cmdClass = QUERY_CMD;
    }
    else if(ptr[0] == ' ')
    {
        /* 解析接收命令 */
        cmdClass = RECV_CMD;
    }
    else
    {
        ret = -1;
        printf("find cmd error.\r\n");
        goto at_end;
    }
	printf("cmd class :%d.\r\n",cmdClass);
    for (int i = 1; i < size; i++)
    {
        // if( (cmdClass == QUERY_CMD && i == 2) ||
        //     (cmdClass == SET_CMD && i == 1)   ||
        //     (cmdClass == TEST_CMD && i == 2)  )
        {
            if(strstr((const char *)pdata[i], "OK") != NULL)
            {
                result = AT_RESULT_OK;
            }
            else if(strstr((const char *)pdata[i], "ERROR") != NULL)
            {
                result = AT_RESULT_ERROR;
            }
        }
        
    }
    if (NULL != at_table[index].deal_func) 
    {
        unsigned char *payload = NULL;
        unsigned int payloadLen = 0;
        switch (cmdClass)
        {
        case QUERY_CMD:
        case SET_CMD:
        case TEST_CMD:
            if(strlen(pdata[1]) < strlen(at_table[index].cmd) - 2 + 1)
            {
                break;
            }
            payload = pdata[1] + strlen(at_table[index].cmd) - 2 + 1;//减去命令表中的"AT"+去除数据中的":",如果格式中冒号后面是有空格的,这里需要再+1
            payloadLen = strlen(pdata[1]) - (strlen(at_table[index].cmd) - 2 + 1);
            break;
        case RECV_CMD:
            payload = pdata[0] + strlen(at_table[index].cmd) + 1;    //去除数据中的":",如果格式中冒号后面是有空格的,这里需要再+1
            payloadLen = strlen(pdata[0]) + (strlen(at_table[index].cmd) + 1);
            break;
        default:
            break;
        }
        if (payload != NULL)
        {
            argc = string_split((char*)payload, payloadLen, ',', argv, ARGC_LIMIT);
        }
        ret = at_table[index].deal_func(cmdClass,result,argc,argv,pdata);
    }

at_end:
	if (-1 == ret) respond_error();
	else respond_ok();

	return ret;
}


int at_cmd_recv(char data,char timeOut)
{
	int ret = -1;
	static unsigned short index = 0;
    static unsigned short cnt = 0;
	static unsigned int tick = 0;
	static unsigned char flag = 0;
    static unsigned char cloud = 0;

	// if (((HAL_GetTick() - tick) > AT_RX_TIMEOUT) || (index >= AT_RX_BUF_SIZE)) 
    // {
    //     printf("recv timeout.\r\n");
    //     if(cloud)
    //     {
    //         printf("process cloud data.\r\n");
    //         atCmdRecvTcpData(cmdbuf[cnt],index);
    //     }
    //     cloud = 0;
	// 	index = 0;
	// 	flag = 0;
    //     cnt = 0;
    //     memset(cmdbuf,0,sizeof(cmdbuf));
	// }
	// tick = HAL_GetTick();
    if(timeOut)
    {
        if(cloud)
        {
            printf("process cloud data.\r\n");
            atCmdRecvTcpData(cmdbuf[cnt],index);
        }
        // printf("timeout no process.\r\n");
        cloud = 0;
		index = 0;
		flag = 0;
        cnt = 0;
        return 0;
    }
    if( (cmdbuf[cnt][index-1] == data && data == '\r')
        || (cmdbuf[cnt][index-1] == data && data == '\n')
        || (cmdbuf[cnt][index-1] == '\n' && data == '\r') 
        || (index == 0 && (data == '\r' || data == '\n'))
        )
    {
        // printf("Remove redundant newline characters.\r\n");
        return 0;
    }
    // printf("recv:%c[0x%02x].\r\n",data,data);
	cmdbuf[cnt][index++] = data;
    if(cmdbuf[cnt][0] == 0XBB)
    {
        cloud = 1;
        return -2;
    }
	if ((data == '\n' || data == ' ' ) && flag) 
    {
        printf("cnt:%d,index:%d.\r\n",cnt,index);
        if(cmdbuf[cnt][0] == 'A' && cmdbuf[cnt][1] == 'T')
        {
            printf("find AT.\r\n");
            cnt ++;
        }
        else if(cmdbuf[cnt][0] == '+')
        {
            printf("find +.\r\n");
            cnt ++;
            if(cnt == 1) //如果直接接收到"+"开头的命令,则是接收到模组主动推送的命令,不需要等待OK或者ERROR直接处理
            {
                printf("this passive receiving data.\r\n");
                ret = at_cmd_parse(cmdbuf, cnt);
                cnt = 0;
                memset(cmdbuf,0,sizeof(cmdbuf));
            }
        }
        else if(cmdbuf[cnt][0] == '>')
        {
            printf("find >.\r\n");
            cnt ++;
            ret = at_cmd_parse(cmdbuf, cnt);
            cnt = 0;
            memset(cmdbuf,0,sizeof(cmdbuf));
        }
        else if(cmdbuf[cnt][0] == 'O' && cmdbuf[cnt][1] == 'K')
        {
            printf("find OK.\r\n");
            cnt ++;
            ret = at_cmd_parse(cmdbuf, cnt);
            cnt = 0;
            memset(cmdbuf,0,sizeof(cmdbuf));
        }
        else if(cmdbuf[cnt][0] == 'E' && cmdbuf[cnt][1] == 'R' && cmdbuf[cnt][2] == 'R' && cmdbuf[cnt][3] == 'O' && cmdbuf[cnt][4] == 'R')
        {
            printf("find ERROR.\r\n");
            cnt ++;
            ret = at_cmd_parse(cmdbuf, cnt);
            cnt = 0;
            memset(cmdbuf,0,sizeof(cmdbuf));
        }
        else if( (cmdbuf[cnt][0] == 'C' && cmdbuf[cnt][1] == 'O' && cmdbuf[cnt][2] == 'N' && cmdbuf[cnt][3] == 'N' && cmdbuf[cnt][4] == 'E' && cmdbuf[cnt][5] == 'C' && cmdbuf[cnt][6] == 'T')
            || ( cmdbuf[cnt][0] == 'S' && cmdbuf[cnt][1] == 'E' && cmdbuf[cnt][2] == 'N' && cmdbuf[cnt][3] == 'D' ) 
            || ( cmdbuf[cnt][0] == 'C' && cmdbuf[cnt][1] == 'L' && cmdbuf[cnt][2] == 'O' && cmdbuf[cnt][3] == 'S' && cmdbuf[cnt][4] == 'E' ) 
            || (cmdbuf[cnt][0] == 'A' && cmdbuf[cnt][1] == 'L' && cmdbuf[cnt][2] == 'R' && cmdbuf[cnt][3] == 'E' && cmdbuf[cnt][4] == 'A' && cmdbuf[cnt][5] == 'D' && cmdbuf[cnt][6] == 'Y')
            )
        {
            printf("find socket event.\r\n");
            cnt ++;
            if(cnt == 1) //如果直接接收到socket的命令,则是接收到模组主动推送的命令,不需要等待OK或者ERROR直接处理
            {
                printf("this socket event data.\r\n");
                ret = at_cmd_parse(cmdbuf, cnt);
                cnt = 0;
                memset(cmdbuf,0,sizeof(cmdbuf));
            }
        }
        // else if((cmdbuf[cnt][0]>='a' && cmdbuf[cnt][0]<='z') 
        //     || (cmdbuf[cnt][0]>='A' && cmdbuf[cnt][0]<='Z')
        //     || (cmdbuf[cnt][0]>='0' && cmdbuf[cnt][0]<='9')
        else if((cmdbuf[cnt][0]>='0' && cmdbuf[cnt][0]<='9') 
            )//这里考虑的是类似获取imei时,imei前面没有+
        {
            printf("find a-z/A-Z/0-9.\r\n");
            cnt ++;
        }
        else
        {
            printf("error data:%s[0x%02x].\r\n",cmdbuf[cnt],cmdbuf[cnt]);
            cnt = 0;
            memset(cmdbuf,0,sizeof(cmdbuf));
        }
        flag = 0;
		index = 0;
	} 
    else if (data == '\r') 
    {
		flag = 1;
	} 
    else if(data == '>')
    {
        flag = 1;
    }
    else 
    {
		flag = 0;
	}

	return ret;
}

