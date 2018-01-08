#include "includes.h"
#include "comm_types.h"
#include "comm_transfer.h"
#include "platform/delay.h"
#include "platform/uart.h"

#define TIME_OUT_TICKS	1000
void serial_reset(struct transfer* trans)
{
		trans->state=TRANS_STATE_IDLE;
		trans->length=0;
}

BOOL serial_push_byte(struct transfer* trans,int8u data_in)
{
	int8u crc;
	if(trans->state==TRANS_STATE_IDLE && data_in!=TRANS_SOF)
		return FALSE;
	if(trans->length>=TRANS_MAX_LENGTH)
	{
		trans->reset(trans);
		return FALSE;
	}
	trans->time_ticks=TIME_OUT_TICKS;
	switch(data_in)
	{
		case TRANS_SOF:
				trans->length=0;
				trans->state=TRANS_STATE_RECEVING;
				break;
		case TRANS_EOF:
			trans->state=TRANS_STATE_END;
			if(trans->receive_end_callback)
				trans->receive_end_callback(trans);
			crc=crc8_ccitt(CRC8_PRESET,trans->data,trans->length-1);
			if(crc==trans->data[trans->length-1])
			{
				trans->length-=1;
				return TRUE;
			}
			trans->reset(trans);
			return FALSE;
		case TRANS_ESC:
			trans->state=TRANS_STATE_ESCAPE;
			break;
		default:
			if(trans->state==TRANS_STATE_ESCAPE)
			{
				trans->data[trans->length]=data_in|0xA0;
				trans->length++;
				trans->state=TRANS_STATE_RECEVING;
			}
			else if(trans->state==TRANS_STATE_RECEVING)
			{
				trans->data[trans->length]=data_in;
				trans->length++;
			}
			else
				trans->reset(trans);
			break;
	}
	return FALSE;
}
BOOL serial_push_bytes(struct transfer* trans,int8u *data,int8u length)
{
	int i;
	for(i=0;i<length;i++)
	{
		if(serial_push_byte(trans,data[i]))
			return TRUE;
	}
	return FALSE;
}

void serial_send_byte(struct transfer* trans,int8u by)
{
	struct uart *uart=(struct uart*)(trans->port);
	if( by==TRANS_SOF || by==TRANS_EOF || by==TRANS_ESC)
	{
			uart_send_byte_sync(uart,TRANS_ESC);
			uart_send_byte_sync(uart, by & 0x0F);
	}else{
			uart_send_byte_sync(uart, by);
	}
}

BOOL serial_send(struct transfer* trans,int8u *data,int8u length)
{
	struct uart *uart=(struct uart*)(trans->port);
	int8u crc,i;
	uart_send_byte_sync(uart,TRANS_SOF);
	for(i=0;i<length;i++)
		serial_send_byte(trans,data[i]);
	crc=crc8_ccitt(CRC8_PRESET,data,length);
	serial_send_byte(trans,crc);
	uart_send_byte_sync(uart,TRANS_EOF);
	return TRUE;
}

int serial_copy_out(struct transfer* trans,void* dest_buff)
{
	int len=0;
	//关中断
	if(trans->state==TRANS_STATE_END && dest_buff)
	{
		memcpy(dest_buff,trans->data,trans->length);
		len=trans->length;
	}
	//开中断
	return len;
}

void serial_tick(struct transfer* trans,int16u ticks)
{
	if(trans->time_ticks==0)
		return;
	if(trans->time_ticks>ticks)
		trans->time_ticks-=ticks;
	else
	{
		trans->time_ticks=0;
		serial_reset(trans);
	}
}
