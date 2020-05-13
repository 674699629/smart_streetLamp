#ifndef __AT_PROCESS_H_
#define __AT_PROCESS_H_

/* 
发送的AT命令分3种格式
1.写命令    
AT+CTZR=0
OK

2.读命令
AT+CTZR?
+CTZR:0
OK

3.测试命令 
AT+CTZR=?
+CTZR:(0,1,2,3)
OK
*/

#ifndef NULL
#define         NULL                ((void *) 0)
#endif

#define AT_RESULT_ERROR    0
#define AT_RESULT_OK       1

#define SET_CMD		    0x01 	/* 设置命令 */
#define QUERY_CMD		0x02	/* 查询命令 */
#define TEST_CMD		0x03	/* 测试命令 */
#define RECV_CMD	    0x04	/* 接收命令 */

#define respond_error()	printf("ERROR\r\n")
#define respond_ok()	printf("OK\r\n");

#define ARGC_LIMIT      10
#define AT_RX_TIMEOUT	200	/* ms */
#define AT_RX_BUF_SIZE	512	/* bytes */
#define AT_RX_BUF_CNT	3	/* bytes */

typedef struct 
{
	char *cmd;	/* AT指令 */
	int (*deal_func)(int opt,int result,int argc, char *argv[],unsigned char cmdBuf[][AT_RX_BUF_SIZE]);	
}at_cmd_t;

#define AT_TABLE_SIZE	(sizeof(at_table) / sizeof(at_cmd_t))


int at_cmd_recv(char data,char timeOut);

#endif

