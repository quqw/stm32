/*
通信传输层，完成底层数据的传送，不涉及上层协议
*/
//#include "includes.h"
#include "assist/assist.h"
#include "usb_transfer.h"
#include "assist/delay.h"
#include "platform_drivers/st_uart.h"

BOOL usb_push_bytes(struct transfer* trans,int8u *data,int8u length)
{
	trans->state=TRANS_STATE_RECEVING;
	memcpy(trans->data,data,length);
	trans->length=length;
	trans->state=TRANS_STATE_END;
	return TRUE;
}
int usb_copy_out(struct transfer* trans,void* dest_buff)
{
	int len=0;
	if(trans->state==TRANS_STATE_END && dest_buff)
	{
		memcpy(dest_buff,trans->data,trans->length);
		len=trans->length;
	}
	return len;
}

extern BOOL hid_send_data(int8u* data,int8u length);
BOOL usb_send(struct transfer* trans,int8u *data,int8u length)
{
	return hid_send_data(data,length);
}

