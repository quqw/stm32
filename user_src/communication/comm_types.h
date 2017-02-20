#ifndef COMM_TYPES_H
#define COMM_TYPES_H
#include "common_types.h"

#define COMM_MAX_LENGTH		256
#define COMM_PACKET_HEAD_LENGTH	4
#define COMM_PACKET_DATA_LENGTH	(COMM_MAX_LENGTH-COMM_PACKET_HEAD_LENGTH)



//zigbeeЭ��
#define FCF_DATA     (0x01)
#define FCF_MACCMD   (0x03)
#define FCF_ACKREQ   (0x20)

//zigbee��ַ����
#define FCF_INTRAPAN (0x40)
#define FCF_NODST    (0x00)
#define FCF_SHORTDST (0x08)
#define FCF_LONGDST  (0x0C)
#define FCF_NOSRC    (0x00)
#define FCF_SHORTSRC (0x80)
#define FCF_LONGSRC  (0xC0)


struct comm_packet_head
{
	int8u length;
	int8u zigbee_protocol;
	int8u address_flag;
	int8u toggle;//��4λtoggle����4λprotocol
};

struct comm_packet
{
	struct comm_packet_head head;
	int8u data[COMM_PACKET_DATA_LENGTH];
};


#define CMD_ERROR_FLAG	0x40
#define CMD_MUTE_FLAG	0x80
#define INS_MASK	0x7f
#define INS_RESPONSE_FLAG	0x80
#define CMD_PACKET_HEAD_LENGTH	4
struct cmd_packet
{
	int8u flag;
	int8u cla;
	int8u ins;
	int8u len;
	int8u data[COMM_PACKET_DATA_LENGTH-CMD_PACKET_HEAD_LENGTH];
};

//�׶�
#define COMM_PHASE_RECEIVING	1
#define COMM_PHASE_RECEIVE_END	2
#define COMM_PHASE_EXCUDING			3
#define COMM_PHASE_RESPONSE		4

//״̬
#define COMM_STSTE_MUTE	1 //��Ĭ

struct communicate
{
	struct transfer *trans;//�ײ��ͨ�ţ�usb�����ڣ�iic�ȣ����ܶ��ǲ�һ���Ĵ���
	BOOL (*protocol_parse)(struct communicate* comm);
	BOOL (*cmd_parse)(struct communicate* comm);
	int8u packet[COMM_MAX_LENGTH];//�ϰ汾��comm_packet packet;���������ݴ�Ŵ�,zigbee�����ڵȸ�ʽȫ��ͳһ
	void *cmd;//�ϰ汾��struct cmd_packet* cmd;//ָ������cmd����λ��
	int16u data_length;
	int8u state;//��ǰ״̬
	int8u phase;//��ǰ�����׶�
};

#define TRANS_MAX_LENGTH		128
#define TRANS_TIMEOUT			100
#define TRANS_STATE_IDLE				0
#define TRANS_STATE_RECEVING		1
#define TRANS_STATE_ESCAPE			2
#define TRANS_STATE_END					3
typedef struct transfer
{
	comm_port_t *port;//����ӿ�
	void (*reset)(struct transfer* trans);
	void (*tick)(struct transfer* trans,int16u ticks);
	BOOL (*push_data)(struct transfer* trans,int8u *data,int8u length);
	BOOL (*send)(struct transfer* trans,int8u *data,int8u length);//����㷢��Э��
	BOOL (*receive_end_callback)(struct transfer* trans);
	int (*copy_out_data)(struct transfer* trans,void* dest_buff);//ȥ���������Ϣ��������comm_packet����
	int8u data[TRANS_MAX_LENGTH];
	int8u length;
	int8u state;
	int16u time_ticks; //��ʱ
}transfer_t;

#endif

