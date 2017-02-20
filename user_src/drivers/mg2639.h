#ifndef MG2639_H
#define MG2639_H
#include "platform/uart.h"

//ģ�鵱ǰ�����׶�
#define PHASE_IDLE								0
#define PHASE_RECEIVE_DATA				1//���ڽ������ݹ����У������Ժ��Կո�ͻس�
#define PHASE_SEND_DATA						(1<<1)
#define PHASE_WAIT_CMD_RESPONSE		(1<<2)//�ȴ���ѯָ��ķ��ؽ��
#define PHASE_WAIT_CMD_OK					(1<<3)//�ȴ�ָ�����
#define PHASE_SEND_CMD						(1<<4)
#define PHASE_RECIVING_RESPONSE		(1<<5)//���ڽ���ָ������У����Ժ��Կո�ͻس�
#define PHASE_WAIT_STRING					(1<<6)//�ȴ�ĳ���ַ�������

#define PHASE_WAIT_1ST_PARAM			(1<<8)//�ȴ����յ�1������
#define PHASE_WAIT_2ND_PARAM			(1<<9)//�ȴ����յ�2������
#define PHASE_WAIT_3RD_PARAM			(1<<10)//�ȴ����յ�3������
#define PHASE_WAIT_4TH_PARAM			(1<<11)//�ȴ����յ�4������
#define PHASE_WAIT_PARAM					(0xf00)
//ģ������״̬
#define MODULE_UNINIT			0
#define MODULE_INITED							(1)
#define MODULE_SIM_OK							(1<<1)
#define MODULE_GSM_REGISTED				(1<<3)//gsm������ע��
#define MODULE_GPRS_INITED				(1<<4)//GPRS������ʼ��
#define MODULE_GPRS_DNS_SET				(1<<5)//GPRS������ʼ��
#define MODULE_GPRS_ATTACHED			(1<<6)//gprs������
#define MODULE_TCP_CONNECTED			(1<<7)//gprs�Ѿ����ӵ�������
#define MODULE_DATA_SEND_PANDING	(1<<8)//��������Ҫ����
#define MODULE_SMS_FULL						(1<<9)//sms��

//ģ���Ӧ��״̬��Ҳ��ģ���Ӧ��ֵ
#define CMD_RESP_IDLE				0
#define CMD_RESP_WAIT				1
#define CMD_RESP_END				2  //ָ���Ӧ����
#define CMD_RESP_OK					3
#define CMD_RESP_ERROR			4
#define CMD_RESP_UNKNOW			5
#define CMD_RESP_TIMEOUT		6

//��ǰ���ڽ�����ָ��
#define PARSE_NONE					0
#define PARSE_UNKNOW				1
#define PARSE_CREG					2
#define PARSE_CSQ						3
#define PARSE_CMGS					4
#define PARSE_CFUN					5
#define PARSE_CPIN					6
#define PARSE_ZPPPOPEN			7	//��gprs
#define PARSE_ZIPSETUP			8	//��tcp
#define PARSE_ZIPCLOSE			9	//�ر�tcp����
#define PARSE_ZIPSTATUS			10	//��tcp״̬
#define PARSE_ZIPRECV				11	//���յ�����
#define PARSE_ZIPSEND				12	//��������
#define PARSE_ZPPPCLOSE			13	//��gprs
#define PARSE_ZDNSSERV			14
#define PARSE_ZDNSGETIP			15 //��ȡip
#define PARSE_ZAPP					16
#define PARSE_ZSMGS					17//������ָʾ

#define RESP_TEMP_BUF	16
#define WAIT_BUF	16
#define TCP_DATA_BUF	1024
typedef struct mg2639
{
	struct uart *port;
	void (*on_tcp_complete)(int8u* data,int length);
	void (*on_sms)(string phone,string sms_cmd_str);
	volatile int32u work_phase;//�����׶�
	volatile int32u module_state;//ģ��״̬
	volatile int8u buf_len;
	volatile int8s csq;
	volatile int8u cmd_resp_state;//ģ���Ӧֵ
	volatile int8u cmd_parse_phase;//��ǰ��Ҫ������ָ��
	volatile int8u cmd_current_phase;//��ǰ���ڽ�����ָ��
	volatile int8u wait_string_len;
	volatile int16u tcp_receive_length;
	volatile int16u tcp_index;
	volatile int16u tcp_send_length;
	char temp_buf[RESP_TEMP_BUF];//����ָ�����ʱ�洢��
	char resp_buf[RESP_TEMP_BUF];//ָ��ز����洢��
	char wait_string[WAIT_BUF];
	char tcp_recv_data[TCP_DATA_BUF];
	//char tcp_send_data[TCP_DATA_BUF];
}mg2639_t;

#define DELCARE_MG2639(name,uart) struct mg2639 mg2639_##name={&uart,0,0,0,0,0,0,0,0,0,0,0}

int gsm_parse_creg(char* creg);

void mg2639_init(struct mg2639 *p_mg2639);
void mg2639_on_port_callback(struct mg2639 *p_mg2639);//���յ�����ʱ�Ĵ���
void mg2639_parse_resp(struct mg2639 *p_mg2639);//���յ�����ʱ�Ĵ���
void mg2639_parse_cmd_head(struct mg2639 *p_mg2639);
void mg2639_tick(struct mg2639 *p_mg2639,int ticks);
void mg2639_parse_urc(struct mg2639 *p_mg2639);
void mg2639_parse_creg(struct mg2639 *p_mg2639);
void mg2639_parse_csq(struct mg2639 *p_mg2639);
void mg2639_on_tcp_data(struct mg2639 *p_mg2639,char ch);
BOOL mg2639_wait_init(struct mg2639 *p_mg2639,int ms);
BOOL mg2639_at(struct mg2639 *p_mg2639);
BOOL mg2639_echo(struct mg2639 *p_mg2639,BOOL enable);
BOOL mg2639_check_creg(struct mg2639 *p_mg2639);
BOOL mg2639_check_csq(struct mg2639 *p_mg2639);
BOOL mg2639_tcp_connect(struct mg2639 *p_mg2639,string ip,int16u port);
BOOL mg2639_check_tcp(struct mg2639 *p_mg2639);
BOOL mg2639_gprs_connect(struct mg2639 *p_mg2639);
BOOL mg2639_set_apn(struct mg2639 *p_mg2639,string apn);
BOOL mg2639_set_dns(struct mg2639 *p_mg2639,string dns1,string dns2);
//BOOL mg2639_tcp_send(struct mg2639 *p_mg2639);
BOOL mg2639_tcp_send(struct mg2639 *p_mg2639,int8u* data,int16u length);
BOOL mg2639_get_ip(struct mg2639 *p_mg2639,string domain,char* ip);
BOOL mg2639_sms_set_pdu(struct mg2639 *p_mg2639,BOOL pdu_on);
BOOL mg2639_send_txt(struct mg2639 *p_mg2639,const char* phone,char* sms);
BOOL mg2639_sms_config_mode(struct mg2639 *p_mg2639);
#endif

