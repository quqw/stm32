#include "includes.h"
#include "mg2639.h"
#include <ctype.h>
#include <string.h>
#define SOCKET_ID	0

extern struct uart uart_1;

void mg2639_init(struct mg2639 *p_mg2639)
{
	p_mg2639->port->init(p_mg2639->port,115200,USART_WordLength_8b,USART_StopBits_1,USART_Parity_No);
	p_mg2639->port->start_rx(p_mg2639->port);
	p_mg2639->buf_len=0;
	p_mg2639->cmd_resp_state=CMD_RESP_IDLE;
	p_mg2639->module_state=MODULE_UNINIT;
	p_mg2639->wait_string_len=0;
}
void mg2639_tick(struct mg2639 *p_mg2639,int ticks)
{
	(void)ticks;
	mg2639_on_port_callback(p_mg2639);
}
//通信端口数据处理,不可阻塞
void mg2639_on_port_callback(struct mg2639 *p_mg2639)//接收到数据时的处理
{
	int8u ch;
	while(p_mg2639->port->read_byte(p_mg2639->port,&ch))
	{
		#ifdef __DEBUG_GSM
		uart_start_tx(&uart_1);
		uart_send_byte_sync(&uart_1,ch);//test
		uart_start_rx(&uart_1);
		#endif
//		if(p_mg2639->work_phase & PHASE_SEND_CMD)//处在指令发送阶段，不缓存
//		{
//			if(ch =='\r'|| ch =='\n')
//			{
//					p_mg2639->work_phase &=~PHASE_SEND_CMD;
//					p_mg2639->work_phase |=PHASE_WAIT_CMD_RESPONSE;
//			}
//		}
//		else//接受模块主动发出的消息 或者 接收命令的返回
//		{
//			if(!(p_mg2639->work_phase & PHASE_RECEIVE_DATA))//不处在数据收发阶段，忽略空格，需要解析指令
//			{
//				
//				if(ch=='\r'||ch=='\n')//指令结束或者指令
//				{
//					if(p_mg2639->buf_len==0)
//						continue;
//					p_mg2639->temp_buf[p_mg2639->buf_len]=0;
//					mg2639_parse_resp(p_mg2639);
//					p_mg2639->buf_len=0;
//					continue;
//				}
//				else if(p_mg2639->buf_len==0 && ch=='+')
//					p_mg2639->work_phase |=PHASE_RECIVING_RESPONSE;
//				else if(ch==':' && (p_mg2639->work_phase&PHASE_RECIVING_RESPONSE))
//				{
//					mg2639_parse_cmd_head(p_mg2639 );	
//					p_mg2639->buf_len=0;
//					p_mg2639->work_phase &=~PHASE_WAIT_PARAM;
//					p_mg2639->work_phase|=PHASE_WAIT_1ST_PARAM;
//					continue;
//				}
//				else if(ch==' ' || !isprint(ch))
//				{
//					continue;
//				}
//				p_mg2639->temp_buf[p_mg2639->buf_len]=toupper(ch);
//				p_mg2639->buf_len++;
//				if(p_mg2639->wait_string_len==p_mg2639->buf_len && (p_mg2639->work_phase & PHASE_WAIT_STRING) &&
//					memcmp(p_mg2639->wait_string,p_mg2639->temp_buf,p_mg2639->wait_string_len)==0)
//				{
//					p_mg2639->work_phase &=~PHASE_WAIT_STRING;
//					p_mg2639->wait_string_len=0;
//					p_mg2639->buf_len=0;
//				}
//				else if(p_mg2639->buf_len>=RESP_TEMP_BUF)
//				{
//					//???
//					p_mg2639->buf_len=0;
//				}
//			}
//			else
//				mg2639_on_tcp_data(p_mg2639,ch);
//		}
	}
}

void mg2639_parse_resp(struct mg2639 *p_mg2639)//接收到回车时的处理
{
		if(memcmp(p_mg2639->temp_buf,"OK",2)==0)
		{
			p_mg2639->work_phase =PHASE_IDLE;
			p_mg2639->cmd_resp_state=CMD_RESP_OK;
		}
		else if(memcmp(p_mg2639->temp_buf,"ERROR",5)==0)
		{
			p_mg2639->work_phase =PHASE_IDLE;
			p_mg2639->cmd_resp_state=CMD_RESP_ERROR;
		}
		else
		{
			if(p_mg2639->cmd_parse_phase!=p_mg2639->cmd_current_phase)
			{
				mg2639_parse_urc(p_mg2639);//主动型指令解析
				return;
			}
			if(p_mg2639->buf_len!=0)
			{
				memcpy(p_mg2639->resp_buf,p_mg2639->temp_buf,p_mg2639->buf_len);
				p_mg2639->resp_buf[p_mg2639->buf_len]=0;
			}
			p_mg2639->cmd_parse_phase=PARSE_NONE;
			p_mg2639->work_phase &=~PHASE_WAIT_CMD_RESPONSE;
		}


}
void mg2639_parse_cmd_head(struct mg2639 *p_mg2639)
{
	if(memcmp(p_mg2639->temp_buf,"+CREG",5)==0)
		p_mg2639->cmd_current_phase= PARSE_CREG;
	else if(memcmp(p_mg2639->temp_buf,"+CSQ",4)==0)
		p_mg2639->cmd_current_phase= PARSE_CSQ;
	else if(memcmp(p_mg2639->temp_buf,"+ZSMGS",6)==0)
		p_mg2639->cmd_current_phase= PARSE_CSQ;
	else if(memcmp(p_mg2639->temp_buf,"+ZIPRECV",6)==0)
	{
		p_mg2639->cmd_current_phase= PARSE_ZIPRECV;
		p_mg2639->work_phase |= PHASE_RECEIVE_DATA;
	}else if(memcmp(p_mg2639->temp_buf,"+CPIN",5)==0)
		p_mg2639->cmd_current_phase= PARSE_CPIN;
	else if(memcmp(p_mg2639->temp_buf,"+ZAPP",5)==0)
		p_mg2639->cmd_current_phase= PARSE_ZAPP;
	else if(memcmp(p_mg2639->temp_buf,"+CFUN",5)==0)
		p_mg2639->cmd_current_phase= PARSE_CFUN;
	else if(memcmp(p_mg2639->temp_buf,"+ZPPPOPEN",9)==0)
		p_mg2639->cmd_current_phase= PARSE_ZPPPOPEN;
	else if(memcmp(p_mg2639->temp_buf,"+ZIPCLOSE",9)==0)
		p_mg2639->cmd_current_phase= PARSE_ZIPCLOSE;
	else if(memcmp(p_mg2639->temp_buf,"+ZPPPCLOSE",10)==0)
		p_mg2639->cmd_current_phase= PARSE_ZPPPCLOSE;
	else if(memcmp(p_mg2639->temp_buf,"+ZIPSETUP",9)==0)
		p_mg2639->cmd_current_phase= PARSE_ZIPSETUP;
	else if(memcmp(p_mg2639->temp_buf,"+ZIPSTATUS",9)==0)
		p_mg2639->cmd_current_phase= PARSE_ZIPSTATUS;
	else if(memcmp(p_mg2639->temp_buf,"+ZDNSGETIP",9)==0)
		p_mg2639->cmd_current_phase= PARSE_ZDNSGETIP;
	else if (memcmp(p_mg2639->temp_buf,"+ZSMGS",6)==0)
		p_mg2639->cmd_current_phase= PARSE_ZSMGS;
	p_mg2639->buf_len=0;
}
void mg2639_parse_urc(struct mg2639 *p_mg2639)
{
	switch(p_mg2639->cmd_current_phase)
	{
		case PARSE_CREG:
			mg2639_parse_creg(p_mg2639);
			break;
		case  PARSE_CSQ:
			mg2639_parse_csq(p_mg2639);
			break;
		case  PARSE_CMGS:
			break;
		case  PARSE_CFUN:
			break;
		case PARSE_ZAPP:
			if(strcmp(p_mg2639->temp_buf,"START")==0)
				p_mg2639->module_state |=MODULE_INITED | MODULE_SIM_OK;
		case  PARSE_CPIN:
			if(strcmp(p_mg2639->temp_buf,"READY")==0)
			{
				p_mg2639->module_state |=MODULE_INITED | MODULE_SIM_OK;
			}
			break;
		case PARSE_ZPPPOPEN	:		//开gprs
			if(strcmp(p_mg2639->temp_buf,"CONNECTED")==0 ||strcmp(p_mg2639->resp_buf,"ESTABLISHED")==0)
				p_mg2639->module_state |=MODULE_GPRS_ATTACHED;
			else
				p_mg2639->module_state &=~(MODULE_GPRS_ATTACHED | MODULE_TCP_CONNECTED);
			break;
		case PARSE_ZPPPCLOSE:
			if(strcmp(p_mg2639->temp_buf,"OK")==0 ||strcmp(p_mg2639->resp_buf,"DISCONNECTED")==0)
				p_mg2639->module_state &=~(MODULE_GPRS_ATTACHED | MODULE_TCP_CONNECTED);
			break;
		case PARSE_ZIPSETUP	:		//开tcp
			if(strcmp(p_mg2639->temp_buf,"CONNECTED")==0 || strcmp(p_mg2639->resp_buf,"ESTABLISHED")==0)
				p_mg2639->module_state |=(MODULE_GPRS_ATTACHED | MODULE_TCP_CONNECTED);
			else if(strcmp(p_mg2639->temp_buf,"CONNECT FAIL")==0)
				p_mg2639->module_state &=(~MODULE_TCP_CONNECTED);
			break;
		case PARSE_ZIPCLOSE	:			//关闭tcp连接,+ZIPCLOSE: 0// 远端主动关闭tcp
//			if(strcmp(p_mg2639->temp_buf,"OK")==0 ||strcmp(p_mg2639->resp_buf,"DISCONNECTED")==0)
//				p_mg2639->module_state &=~(MODULE_TCP_CONNECTED);
			p_mg2639->module_state &=~(MODULE_TCP_CONNECTED);
			break;
		case PARSE_ZIPSTATUS	:			//查tcp状态
			if(strcmp(p_mg2639->temp_buf,"CONNECTED")==0 ||strcmp(p_mg2639->resp_buf,"ESTABLISHED")==0)
				p_mg2639->module_state |=MODULE_GPRS_ATTACHED;
			else
				p_mg2639->module_state &=~( MODULE_TCP_CONNECTED);
			break;
		case PARSE_ZIPRECV:			//接收到数据,不会到这里来
			break;
		case PARSE_ZIPSEND:					//发送数据
			break;
		case PARSE_ZSMGS:
			if(strstr(p_mg2639->temp_buf,"FULL"))
				p_mg2639->module_state|=MODULE_SMS_FULL;
			break;
		default:
			break;
	}
}

//解析接收到的数据
void mg2639_on_tcp_data(struct mg2639 *p_mg2639,char ch)
{
	if(p_mg2639->work_phase & PHASE_WAIT_PARAM)
	{
		if(ch==',')
		{
			if(p_mg2639->work_phase & PHASE_WAIT_1ST_PARAM)
			{
				p_mg2639->work_phase &=~ PHASE_WAIT_1ST_PARAM;
				p_mg2639->work_phase |= PHASE_WAIT_2ND_PARAM;
			}
			else if(p_mg2639->work_phase & PHASE_WAIT_2ND_PARAM)
			{//
				p_mg2639->temp_buf[p_mg2639->buf_len]=0;
				p_mg2639->tcp_receive_length=atol(p_mg2639->temp_buf);
				if(p_mg2639->tcp_receive_length==0)
				{
					p_mg2639->work_phase &=~PHASE_RECEIVE_DATA;//退出数据接收状态
					p_mg2639->cmd_current_phase=PARSE_NONE;
				}
				else
				{
					p_mg2639->work_phase &=~ PHASE_WAIT_PARAM;
					p_mg2639->tcp_index=0;
				}
			}
			p_mg2639->buf_len=0;
		}
		else 
		{
			p_mg2639->temp_buf[p_mg2639->buf_len]=ch;
			p_mg2639->buf_len++;
		}
		return;
	}
	p_mg2639->tcp_recv_data[p_mg2639->tcp_index]=ch;
	p_mg2639->tcp_index++;
	if(p_mg2639->tcp_index>=p_mg2639->tcp_receive_length)
	{
		p_mg2639->work_phase &=~PHASE_RECEIVE_DATA;//退出数据接收状态
		p_mg2639->cmd_current_phase=PARSE_NONE;
		if(p_mg2639->on_tcp_complete)
			p_mg2639->on_tcp_complete((int8u*)(p_mg2639->tcp_recv_data),p_mg2639->tcp_receive_length);
	}
}

void mg2639_send_cmd(struct mg2639 *p_mg2639,string cmd)
{
#ifdef __DEBUG_GSM
		extern struct uart uart_1;
		uart_send_string_sync(&uart_1,cmd);//test
		uart_send_byte_sync(&uart_1,'\r');
#endif
	p_mg2639->port->send_string_sync(p_mg2639->port,cmd);
	p_mg2639->port->send_string_sync(p_mg2639->port,"\r");
}

void mg2639_send_cmd_ex(struct mg2639 *p_mg2639,string cmd,string param)
{
	p_mg2639->port->send_string_sync(p_mg2639->port,cmd);
	p_mg2639->port->send_string_sync(p_mg2639->port,param);
	p_mg2639->port->send_string_sync(p_mg2639->port,"\r");
}

int32u mg2639_wait_ok(struct mg2639 *p_mg2639,int ms)
{
	int i;
	if (ms<200)
		ms=200;
	for(i=0;i<ms;i+=100)
	{
		if(p_mg2639->work_phase & PHASE_WAIT_CMD_OK)
		{
			vTaskDelay(100);
		}
		else return p_mg2639->cmd_resp_state;
	}
	return CMD_RESP_TIMEOUT;
}

int32u mg2639_wait_response(struct mg2639 *p_mg2639,int ms)
{
	int i;
	if (ms<200)
		ms=200;
	for(i=0;i<ms;i+=100)
	{
		if(p_mg2639->work_phase & PHASE_WAIT_CMD_RESPONSE)
		{
			vTaskDelay(100);
		}
		else	
			return CMD_RESP_END;
	}
	return CMD_RESP_TIMEOUT;
}

int32u mg2639_wait_string(struct mg2639 *p_mg2639,string s,int ms)
{
	int i;
	p_mg2639->work_phase |=PHASE_WAIT_STRING;
	strcpy(p_mg2639->wait_string,s);
	p_mg2639->wait_string_len=strlen(s);
	if (ms<200)
		ms=200;
	for(i=0;i<ms;i+=100)
	{
		if(p_mg2639->work_phase & PHASE_WAIT_STRING)
		{
			vTaskDelay(100);
		}
		else	
			return CMD_RESP_END;
	}
	return CMD_RESP_TIMEOUT;
}

BOOL mg2639_wait_init(struct mg2639 *p_mg2639,int ms)
{
		int i;
	if (ms<200)
		ms=200;
	for(i=0;i<ms;i+=100)
	{
		if(!(p_mg2639->module_state & MODULE_INITED))
		{
			vTaskDelay(100);
		}
		else	
			return TRUE;
	}
	return FALSE;
}

int gsm_parse_creg(char* creg)
{
	char* p;
	int mode,state;
	p=strtok(creg,",");
	mode=atol(p);
	p=strtok(NULL,",");
	state=atol(p);
	return state;
}

void mg2639_parse_creg(struct mg2639 *p_mg2639)
{
	switch(gsm_parse_creg(p_mg2639->resp_buf))
	{
		case 1:
		case 5:
			p_mg2639->module_state |=MODULE_GSM_REGISTED;
			break;
		default:
			p_mg2639->module_state &=~MODULE_GSM_REGISTED;
	}
}

int gsm_parse_csq(char* csq)
{
	#define GSM_NO_NET	99
	char* p;
	int rssi,ber;
	p=strtok(csq,",");
	rssi=atol(p);
	p=strtok(NULL,",");
	ber=atol(p);
	if(ber==GSM_NO_NET)
		return -1;
	return rssi;
}

void mg2639_parse_csq(struct mg2639 *p_mg2639)
{
	p_mg2639->csq=gsm_parse_csq(p_mg2639->resp_buf);
}

void mg2639_parse_tcp_status(struct mg2639 *p_mg2639)
{
	if(strcmp(p_mg2639->resp_buf,"ESTABLISHED")==0||strcmp(p_mg2639->resp_buf,"CONNECTED")==0)
		p_mg2639->module_state |=(MODULE_GPRS_ATTACHED | MODULE_TCP_CONNECTED);
	else if(strcmp(p_mg2639->resp_buf,"DISCONNECTED")==0)
		p_mg2639->module_state &=~MODULE_TCP_CONNECTED;
}

BOOL mg2639_at(struct mg2639 *p_mg2639)
{
	p_mg2639->work_phase |=PHASE_SEND_CMD;
	mg2639_send_cmd(p_mg2639,"AT");
	p_mg2639->work_phase &=~PHASE_SEND_CMD;
	p_mg2639->cmd_parse_phase=PARSE_NONE;
	p_mg2639->work_phase |=PHASE_WAIT_CMD_OK;	
	if( mg2639_wait_ok(p_mg2639,500)==CMD_RESP_OK)
		return TRUE;
	return FALSE;
}

BOOL mg2639_echo(struct mg2639 *p_mg2639,BOOL enable)
{
	p_mg2639->work_phase |=PHASE_SEND_CMD;
	if(enable)
		mg2639_send_cmd(p_mg2639,"ATE1");
	else 
		mg2639_send_cmd(p_mg2639,"ATE0");
	p_mg2639->work_phase &=~PHASE_SEND_CMD;
	p_mg2639->cmd_parse_phase=PARSE_NONE;
	p_mg2639->work_phase |=PHASE_WAIT_CMD_OK;	
	if( mg2639_wait_ok(p_mg2639,500)==CMD_RESP_OK)
		return TRUE;
	return FALSE;
}


BOOL mg2639_check_creg(struct mg2639 *p_mg2639)
{
	p_mg2639->work_phase |=PHASE_SEND_CMD;
	mg2639_send_cmd(p_mg2639,"AT+CREG?");
	p_mg2639->work_phase &=~PHASE_SEND_CMD;
	p_mg2639->cmd_parse_phase=PARSE_CREG;
	p_mg2639->work_phase |= PHASE_WAIT_CMD_RESPONSE;
	p_mg2639->work_phase |= PHASE_WAIT_CMD_OK;	
	if( mg2639_wait_response(p_mg2639,500)!=CMD_RESP_END || mg2639_wait_ok(p_mg2639,100)!=CMD_RESP_OK)
		return FALSE;
	mg2639_parse_creg(p_mg2639);
	return TRUE;
}


BOOL mg2639_check_csq(struct mg2639 *p_mg2639)
{
	p_mg2639->work_phase |=PHASE_SEND_CMD;
	mg2639_send_cmd(p_mg2639,"AT+CSQ");
	p_mg2639->work_phase &=~PHASE_SEND_CMD;
	p_mg2639->cmd_parse_phase=PARSE_CSQ;
	p_mg2639->work_phase |= PHASE_WAIT_CMD_RESPONSE;
	p_mg2639->work_phase |= PHASE_WAIT_CMD_OK;	
	if( mg2639_wait_response(p_mg2639,500)!=CMD_RESP_END || mg2639_wait_ok(p_mg2639,500)!=CMD_RESP_OK)
		return FALSE;
	p_mg2639->csq=gsm_parse_csq(p_mg2639->resp_buf);
	return TRUE;
}


BOOL mg2639_set_apn(struct mg2639 *p_mg2639,string apn)
{
	char cmd[16];
	sprintf(cmd,"AT+ZPNUM=\"%s\",\"\",\"\"",apn);
	p_mg2639->work_phase |=PHASE_SEND_CMD;
	mg2639_send_cmd(p_mg2639,cmd);
	p_mg2639->work_phase &=~PHASE_SEND_CMD;
	p_mg2639->cmd_parse_phase=PARSE_NONE;
	p_mg2639->work_phase |= PHASE_WAIT_CMD_RESPONSE;
	p_mg2639->work_phase |= PHASE_WAIT_CMD_OK;	
	if( mg2639_wait_ok(p_mg2639,500)!=CMD_RESP_OK)
		return FALSE;
	p_mg2639->module_state |=MODULE_GPRS_INITED;
	return TRUE;
}

BOOL mg2639_set_dns(struct mg2639 *p_mg2639,string dns1,string dns2)
{
	char cmd[64];
	sprintf(cmd,"AT+ZDNSSERV=\"%s\",\"%s\"",dns1,dns2);
	p_mg2639->work_phase |=PHASE_SEND_CMD;
	mg2639_send_cmd(p_mg2639,cmd);
	p_mg2639->work_phase &=~PHASE_SEND_CMD;
	p_mg2639->cmd_parse_phase=PARSE_NONE;
	p_mg2639->work_phase |= PHASE_WAIT_CMD_RESPONSE;
	p_mg2639->work_phase |= PHASE_WAIT_CMD_OK;	
	if( mg2639_wait_ok(p_mg2639,500)!=CMD_RESP_OK)
		return FALSE;
	p_mg2639->module_state |=MODULE_GPRS_DNS_SET;
	return TRUE;
}

BOOL mg2639_get_ip(struct mg2639 *p_mg2639,string domain,char* ip)
{
		char cmd[64];
	sprintf(cmd,"AT+ZDNSGETIP=\"%s\"",domain);
	p_mg2639->work_phase |=PHASE_SEND_CMD;
	mg2639_send_cmd(p_mg2639,cmd);
	p_mg2639->work_phase &=~PHASE_SEND_CMD;
	p_mg2639->cmd_parse_phase=PARSE_ZDNSGETIP;
	p_mg2639->work_phase |= PHASE_WAIT_CMD_RESPONSE;
	p_mg2639->work_phase |= PHASE_WAIT_CMD_OK;	
	if( mg2639_wait_response(p_mg2639,20000)!=CMD_RESP_END || mg2639_wait_ok(p_mg2639,1000)!=CMD_RESP_OK)
		return FALSE;
	if(ip)
		strcpy(ip,p_mg2639->resp_buf);
	return TRUE;
}

BOOL mg2639_gprs_connect(struct mg2639 *p_mg2639)
{
	p_mg2639->work_phase |=PHASE_SEND_CMD;
	mg2639_send_cmd(p_mg2639,"AT+ZPPPOPEN");
	p_mg2639->work_phase &=~PHASE_SEND_CMD;
	p_mg2639->cmd_parse_phase=PARSE_ZPPPOPEN;
	p_mg2639->work_phase |= PHASE_WAIT_CMD_RESPONSE;
	p_mg2639->work_phase |= PHASE_WAIT_CMD_OK;	
	if( mg2639_wait_response(p_mg2639,5000)!=CMD_RESP_END || mg2639_wait_ok(p_mg2639,1000)!=CMD_RESP_OK)
		return FALSE;
	if(strcmp(p_mg2639->resp_buf,"CONNECTED")==0 ||strcmp(p_mg2639->resp_buf,"ESTABLISHED")==0)
		p_mg2639->module_state |=MODULE_GPRS_ATTACHED;
	else
		p_mg2639->module_state &=~(MODULE_GPRS_ATTACHED | MODULE_TCP_CONNECTED);
	return TRUE;
}

BOOL mg2639_tcp_connect(struct mg2639 *p_mg2639,string ip,int16u port)
{
	char cmd[64];
	sprintf(cmd,"AT+ZIPSETUP=%d,%s,%d",SOCKET_ID,ip,port);
	p_mg2639->work_phase |=PHASE_SEND_CMD;
	mg2639_send_cmd(p_mg2639,cmd);
	p_mg2639->work_phase &=~PHASE_SEND_CMD;
	p_mg2639->cmd_parse_phase=PARSE_ZIPSETUP;
	p_mg2639->work_phase |= PHASE_WAIT_CMD_RESPONSE;
	p_mg2639->work_phase |= PHASE_WAIT_CMD_OK;	
	if( mg2639_wait_response(p_mg2639,30000)!=CMD_RESP_END || mg2639_wait_ok(p_mg2639,1000)!=CMD_RESP_OK)
		return FALSE;
	if(strcmp(p_mg2639->resp_buf,"CONNECTED")==0 ||strcmp(p_mg2639->resp_buf,"ESTABLISHED")==0)
		p_mg2639->module_state |=(MODULE_GPRS_ATTACHED | MODULE_TCP_CONNECTED);
	return TRUE;
}


BOOL mg2639_check_tcp(struct mg2639 *p_mg2639)
{
	char cmd[16];
	p_mg2639->work_phase |=PHASE_SEND_CMD;
	sprintf(cmd,"AT+ZIPSTATUS=%d",SOCKET_ID);
	mg2639_send_cmd(p_mg2639,cmd);
	p_mg2639->work_phase &=~PHASE_SEND_CMD;
	p_mg2639->cmd_parse_phase=PARSE_ZIPSTATUS;
	p_mg2639->work_phase |= PHASE_WAIT_CMD_RESPONSE;
	p_mg2639->work_phase |= PHASE_WAIT_CMD_OK;	
	if( mg2639_wait_response(p_mg2639,500)!=CMD_RESP_END || mg2639_wait_ok(p_mg2639,500)!=CMD_RESP_OK)
		return FALSE;
	if(strcmp(p_mg2639->resp_buf,"CONNECTED")==0 ||strcmp(p_mg2639->resp_buf,"ESTABLISHED")==0)
		p_mg2639->module_state |=(MODULE_GPRS_ATTACHED | MODULE_TCP_CONNECTED);
	else if(strcmp(p_mg2639->resp_buf,"DISCONNECTED")==0)
		p_mg2639->module_state |=~(MODULE_TCP_CONNECTED);
	return TRUE;
}
//static BOOL _mg2639_tcp_send(struct mg2639 *p_mg2639)
//{
//	char cmd[64];
//	sprintf(cmd,"AT+ZIPSEND=%d,%d",SOCKET_ID,p_mg2639->tcp_send_length);
//	p_mg2639->work_phase |=PHASE_SEND_CMD;
//	mg2639_send_cmd(p_mg2639,cmd);
//	p_mg2639->work_phase &=~PHASE_SEND_CMD;
//	p_mg2639->cmd_parse_phase=PARSE_ZIPSEND;
//	p_mg2639->work_phase |= PHASE_WAIT_CMD_RESPONSE;
//	p_mg2639->work_phase |= PHASE_WAIT_CMD_OK;	
//	if( mg2639_wait_string(p_mg2639,">",2000)!=CMD_RESP_END )
//		return FALSE;
//	p_mg2639->port->send_string_sync(p_mg2639->port,p_mg2639->tcp_send_data);
//	if(mg2639_wait_ok(p_mg2639,10000)!=CMD_RESP_OK)
//		return FALSE;
//	return TRUE;
//}

BOOL mg2639_tcp_send(struct mg2639 *p_mg2639,int8u* data,int16u length)
{
	char cmd[64];
	sprintf(cmd,"AT+ZIPSEND=%d,%d",SOCKET_ID,length);
	p_mg2639->work_phase |=PHASE_SEND_CMD;
	mg2639_send_cmd(p_mg2639,cmd);
	p_mg2639->work_phase &=~PHASE_SEND_CMD;
	p_mg2639->cmd_parse_phase=PARSE_ZIPSEND;
	p_mg2639->work_phase |= PHASE_WAIT_CMD_RESPONSE;
	p_mg2639->work_phase |= PHASE_WAIT_CMD_OK;	
	if( mg2639_wait_string(p_mg2639,">",2000)!=CMD_RESP_END )
		return FALSE;
	p_mg2639->port->send_bytes_sync(p_mg2639->port,data,length);
	if(!(p_mg2639->module_state & MODULE_TCP_CONNECTED))
		return FALSE;
	if(mg2639_wait_ok(p_mg2639,5000)!=CMD_RESP_OK)
		return FALSE;
	return TRUE;
}

//BOOL mg2639_tcp_send_prepare(struct mg2639 *p_mg2639,int8u* data,int16u length)
//{
//}


BOOL mg2639_sms_set_pdu(struct mg2639 *p_mg2639,BOOL pdu_on)
{
	char cmd[64];
	sprintf(cmd,"AT+CMGF=%d",pdu_on?0:1);
	p_mg2639->work_phase |=PHASE_SEND_CMD;
	mg2639_send_cmd(p_mg2639,cmd);
	p_mg2639->work_phase &=~PHASE_SEND_CMD;
	p_mg2639->cmd_parse_phase=PARSE_NONE;
	p_mg2639->work_phase |= PHASE_WAIT_CMD_RESPONSE;
	p_mg2639->work_phase |= PHASE_WAIT_CMD_OK;	
	if(mg2639_wait_ok(p_mg2639,5000)!=CMD_RESP_OK)
		return FALSE;
	return TRUE;
}



BOOL mg2639_send_pdu(struct mg2639 *p_mg2639,char* sms)
{
//	char cmd[16];
//	p_mg2639->work_phase |=PHASE_SEND_CMD;
//	sprintf(cmd,"AT+ZIPSTATUS=%d",SOCKET_ID);
//	mg2639_send_cmd(p_mg2639,cmd);
//	p_mg2639->work_phase &=~PHASE_SEND_CMD;
//	p_mg2639->cmd_parse_phase=PARSE_ZIPSTATUS;
//	p_mg2639->work_phase |= PHASE_WAIT_CMD_RESPONSE;
//	p_mg2639->work_phase |= PHASE_WAIT_CMD_OK;	
//	if( mg2639_wait_response(p_mg2639,500)!=CMD_RESP_END || mg2639_wait_ok(p_mg2639,500)!=CMD_RESP_OK)
//		return FALSE;
//	if(strcmp(p_mg2639->resp_buf,"CONNECTED")==0 ||strcmp(p_mg2639->resp_buf,"ESTABLISHED")==0)
//		p_mg2639->module_state |=(MODULE_GPRS_ATTACHED | MODULE_TCP_CONNECTED);
//	else if(strcmp(p_mg2639->resp_buf,"DISCONNECTED")==0)
//		p_mg2639->module_state |=~(MODULE_TCP_CONNECTED);
	return TRUE;
}


BOOL mg2639_send_txt(struct mg2639 *p_mg2639,const char* phone,char* sms)
{
//	char cmd[16];
//	p_mg2639->work_phase |=PHASE_SEND_CMD;
//	sprintf(cmd,"AT+ZIPSTATUS=%d",SOCKET_ID);
//	mg2639_send_cmd(p_mg2639,cmd);
//	p_mg2639->work_phase &=~PHASE_SEND_CMD;
//	p_mg2639->cmd_parse_phase=PARSE_ZIPSTATUS;
//	p_mg2639->work_phase |= PHASE_WAIT_CMD_RESPONSE;
//	p_mg2639->work_phase |= PHASE_WAIT_CMD_OK;	
//	if( mg2639_wait_response(p_mg2639,500)!=CMD_RESP_END || mg2639_wait_ok(p_mg2639,500)!=CMD_RESP_OK)
//		return FALSE;
//	if(strcmp(p_mg2639->resp_buf,"CONNECTED")==0 ||strcmp(p_mg2639->resp_buf,"ESTABLISHED")==0)
//		p_mg2639->module_state |=(MODULE_GPRS_ATTACHED | MODULE_TCP_CONNECTED);
//	else if(strcmp(p_mg2639->resp_buf,"DISCONNECTED")==0)
//		p_mg2639->module_state |=~(MODULE_TCP_CONNECTED);
	return TRUE;
}



BOOL mg2639_sms_del(struct mg2639 *p_mg2639,int index)
{
		char cmd[64];
	sprintf(cmd,"AT+CMGD=%d",index);
	p_mg2639->work_phase |=PHASE_SEND_CMD;
	mg2639_send_cmd(p_mg2639,cmd);
	p_mg2639->work_phase &=~PHASE_SEND_CMD;
	p_mg2639->cmd_parse_phase=PARSE_NONE;
	p_mg2639->work_phase |= PHASE_WAIT_CMD_RESPONSE;
	p_mg2639->work_phase |= PHASE_WAIT_CMD_OK;	
	if(mg2639_wait_ok(p_mg2639,5000)!=CMD_RESP_OK)
		return FALSE;
	return TRUE;
}

BOOL mg2639_sms_delall(struct mg2639 *p_mg2639)
{
		char cmd[64];
	sprintf(cmd,"AT+CMGD=1,4");
	p_mg2639->work_phase |=PHASE_SEND_CMD;
	mg2639_send_cmd(p_mg2639,cmd);
	p_mg2639->work_phase &=~PHASE_SEND_CMD;
	p_mg2639->cmd_parse_phase=PARSE_NONE;
	p_mg2639->work_phase |= PHASE_WAIT_CMD_RESPONSE;
	p_mg2639->work_phase |= PHASE_WAIT_CMD_OK;	
	if(mg2639_wait_ok(p_mg2639,5000)!=CMD_RESP_OK)
		return FALSE;
	return TRUE;
}

BOOL mg2639_sms_config_storage(struct mg2639 *p_mg2639)
{
			char cmd[64];
	sprintf(cmd,"AT+CPMS=\"SM\",\"SM\",\"SM\"");
	p_mg2639->work_phase |=PHASE_SEND_CMD;
	mg2639_send_cmd(p_mg2639,cmd);
	p_mg2639->work_phase &=~PHASE_SEND_CMD;
	p_mg2639->cmd_parse_phase=PARSE_NONE;
	p_mg2639->work_phase |= PHASE_WAIT_CMD_RESPONSE;
	p_mg2639->work_phase |= PHASE_WAIT_CMD_OK;	
	if(mg2639_wait_ok(p_mg2639,5000)!=CMD_RESP_OK)
		return FALSE;
	return TRUE;
}

BOOL mg2639_sms_config_mode(struct mg2639 *p_mg2639)
{
			char cmd[64];
	sprintf(cmd,"AT+CNMI=3,2,0,0,0");//终端直接显示短信而不存储
	p_mg2639->work_phase |=PHASE_SEND_CMD;
	mg2639_send_cmd(p_mg2639,cmd);
	p_mg2639->work_phase &=~PHASE_SEND_CMD;
	p_mg2639->cmd_parse_phase=PARSE_NONE;
	p_mg2639->work_phase |= PHASE_WAIT_CMD_RESPONSE;
	p_mg2639->work_phase |= PHASE_WAIT_CMD_OK;	
	if(mg2639_wait_ok(p_mg2639,5000)!=CMD_RESP_OK)
		return FALSE;
	return TRUE;
}

