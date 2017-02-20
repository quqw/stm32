#ifndef COMM_TYPES_H
#define COMM_TYPES_H


#define COMM_MAX_LENGTH		128
#define COMM_PACKET_HEAD_LENGTH	4
#define COMM_PACKET_DATA_LENGTH	(COMM_MAX_LENGTH-COMM_PACKET_HEAD_LENGTH)

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
	int8u protocol;
	int8u address_flag;
	int8u toggle;
};

struct comm_packet
{
	struct comm_packet_head head;
	int8u data[COMM_PACKET_DATA_LENGTH];
};

#define CMD_PACKET_HEAD_LENGTH	4
#define CMD_ERROR_FLAG	0x40
#define CMD_MUTE_MUTE_FLAG	0x80

#define INS_MASK	0x7f
#define INS_RESPONSE_FLAG	0x80

struct cmd_packet
{
	int8u flag;
	int8u cla;
	int8u ins;
	int8u len;
	int8u data[COMM_PACKET_DATA_LENGTH-CMD_PACKET_HEAD_LENGTH];
};

struct communicate
{
	struct transfer *trans;//底层的通信，usb，串口，iic等，可能都是不一样的代码
	BOOL (*protocol_parse)(struct communicate* comm);
	BOOL (*cmd_parse)(struct communicate* comm);
	struct comm_packet packet;//真正的数据存放处
	struct cmd_packet *cmd;
	int16u data_length;
	int8u state;
	int8u reserved;
};


#endif

