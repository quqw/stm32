#ifndef COMM_TRANSFER_H
#define COMM_TRANSFER_H


#define TRANS_SOF		0xA1
#define TRANS_EOF		0xA2
#define TRANS_ESC		0xA3

#define TRANS_MAX_LENGTH		128
#define TRANS_TIMEOUT			100
#define TRANS_STATE_IDLE				0
#define TRANS_STATE_RECEVING		1
#define TRANS_STATE_ESCAPE			2
#define TRANS_STATE_END					3

struct transfer
{
	void *port;//物理接口
	void (*reset)(struct transfer* trans);
	void (*tick)(struct transfer* trans,int16u ticks);
	BOOL (*push_data)(struct transfer* trans,int8u *data,int8u length);
	BOOL (*send)(struct transfer* trans,int8u *data,int8u length);
	BOOL (*receive_end_callback)(struct transfer* trans);
	int (*copy_out_data)(struct transfer* trans,void* dest_buff);//拷贝出数据
	int8u data[TRANS_MAX_LENGTH];
	int8u length;
	int8u state;
	int16u time_ticks; //超时
};

void serial_tick(struct transfer* trans,int16u ticks);
int serial_copy_out(struct transfer* trans,void* dest_buff);
BOOL serial_send(struct transfer* trans,int8u *data,int8u length);
BOOL serial_push_bytes(struct transfer* trans,int8u *data,int8u length);
BOOL serial_push_byte(struct transfer* trans,int8u data_in);
void serial_reset(struct transfer* trans);

#endif

