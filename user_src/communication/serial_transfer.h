#ifndef COMM_TRANSFER_H
#define COMM_TRANSFER_H

#include "comm_types.h"

#define TRANS_SOF		0xE7
#define TRANS_ESC		0xEB


void serial_tick(struct transfer* trans,int16u ticks);
int serial_copy_out(struct transfer* trans,void* dest_buff);
BOOL serial_send(struct transfer* trans,int8u *data,int8u length);
BOOL serial_push_bytes(struct transfer* trans,int8u *data,int8u length);
BOOL serial_push_byte(struct transfer* trans,int8u *data_in,int8u len);//定义同上，这样两个函数可以互换
void serial_reset(struct transfer* trans);

#endif

