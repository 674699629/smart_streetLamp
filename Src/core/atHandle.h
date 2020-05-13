#ifndef __AT_HANDLE_H_
#define __AT_HANDLE_H_

#include "atprocess.h"

int deal_csq_func(int cmd, int result, int argc, char *argv[],unsigned char cmdBuf[][AT_RX_BUF_SIZE]);
int deal_ate_func(int cmd, int result, int argc, char *argv[],unsigned char cmdBuf[][AT_RX_BUF_SIZE]);
int deal_gsn_func(int cmd, int result, int argc, char *argv[],unsigned char cmdBuf[][AT_RX_BUF_SIZE]);
int deal_cpin_func(int cmd, int result, int argc, char *argv[],unsigned char cmdBuf[][AT_RX_BUF_SIZE]);
int deal_cgreg_func(int cmd, int result, int argc, char *argv[],unsigned char cmdBuf[][AT_RX_BUF_SIZE]);
int deal_sock_start_func(int cmd, int result, int argc, char *argv[],unsigned char cmdBuf[][AT_RX_BUF_SIZE]);
int deal_sock_connect_func(int cmd, int result, int argc, char *argv[],unsigned char cmdBuf[][AT_RX_BUF_SIZE]);
int deal_sock_send_func(int cmd, int result, int argc, char *argv[],unsigned char cmdBuf[][AT_RX_BUF_SIZE]);
int deal_sock_close_func(int cmd, int result, int argc, char *argv[],unsigned char cmdBuf[][AT_RX_BUF_SIZE]);
int deal_cip_send_func(int cmd, int result, int argc, char *argv[],unsigned char cmdBuf[][AT_RX_BUF_SIZE]);
int deal_sock_already_func(int cmd, int result, int argc, char *argv[],unsigned char cmdBuf[][AT_RX_BUF_SIZE]);
int deal_pdp_deact_func(int cmd, int result, int argc, char *argv[],unsigned char cmdBuf[][AT_RX_BUF_SIZE]);
int deal_cip_status_func(int cmd, int result, int argc, char *argv[],unsigned char cmdBuf[][AT_RX_BUF_SIZE]);
int deal_cip_shut_func(int cmd, int result, int argc, char *argv[],unsigned char cmdBuf[][AT_RX_BUF_SIZE]);


extern at_cmd_t at_table[];
extern unsigned char atRecvNorFlag;
extern unsigned char signal;

// #define AT_TABLE_SIZE	(sizeof(at_table) / sizeof(at_cmd_t))

#define AT_CMD_ATE          "ATE1\r\n"          //开启命令回显
#define AT_CMD_CSQ          "AT+CSQ\r\n"        //查询信号质量 
#define AT_CMD_PIN          "AT+CPIN?\r\n"      //查询SIM卡的状态	 
#define AT_CMD_GSN          "AT+GSN\r\n"        //查询IMEI
#define AT_CMD_CGREG        "AT+CGREG?\r\n"     //查询基站连接状态

#define AT_CMD_CIPSTART 	"AT+CIPSTART=\"TCP\",\"120.24.149.113\",\"7071\"\r\n" //客户服务器
#define AT_CMD_CIPSEND 		"AT+CIPSEND\r\n"  			//启动发送
#define AT_CMD_CIPSTATUS 	"AT+CIPSTATUS\r\n" 			//查询当前连接状态,在连接建立后，如果收到,+PDP：DEACT,则必须发送：AT+CIPSHUT，关闭场景后，才能实现重连
#define AT_CMD_CIPCLOSE 	"AT+CIPCLOSE=1\r\n" 		//快速关闭当前 TCP/UDP连接
#define AT_CMD_CIPSHUT 		"AT+CIPSHUT\r\n" 		    //关闭移动场景，关闭场景后连接状态为：IP INITIAL

int atCmdSendStr(unsigned char * buf);
int atTableCmdNum();

#endif

