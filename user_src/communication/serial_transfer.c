/*
通信传输层，完成底层数据的传送，不涉及上层协议
*/
//#include "includes.h"
#include "assist/assist.h"
#include "serial_transfer.h"
#include "assist/delay.h"
#include "platform_drivers/st_uart.h"

#define TIME_OUT_TICKS	1000
void serial_reset(struct transfer* trans)
{
		trans->state=TRANS_STATE_IDLE;
		trans->length=0;
}

BOOL serial_push_byte(struct transfer* trans,int8u* dat,int8u len)
{
	int8u crc;
	int8u data_in=*dat;
	if((trans->state==TRANS_STATE_IDLE||TRANS_STATE_END==trans->state) && data_in!=TRANS_SOF)
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
				if(trans->state==TRANS_STATE_IDLE||TRANS_STATE_END==trans->state)
				{//起始符
					trans->length=0;
					trans->state=TRANS_STATE_RECEVING;
				}
				else
				{
					//结束符
					trans->state=TRANS_STATE_END;
					crc=crc8_ccitt(CRC8_PRESET,trans->data,trans->length-1);
					if(crc==trans->data[trans->length-1])
					{
						trans->length-=1;
						return TRUE;
					}
					trans->reset(trans);
					return FALSE;
				}
				break;
		case TRANS_ESC:
			trans->state=TRANS_STATE_ESCAPE;
			break;
		default:
			if(trans->state==TRANS_STATE_ESCAPE)
			{
				trans->data[trans->length]=(data_in&0x0f)|0xE0;
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

void serial_send_byte(struct transfer* this,int8u by)
{
	if( by==TRANS_SOF || by==TRANS_ESC)
	{
			st_uart_send_byte_sync(this->port,TRANS_ESC);
			st_uart_send_byte_sync(this->port, by & 0x0F);
	}else{
			st_uart_send_byte_sync(this->port, by);
	}
}

BOOL serial_send(struct transfer* trans,int8u *data,int8u length)
{
	struct uart *uart=(struct uart*)(trans->port);
	int8u crc,i;
	uart_start_tx(uart);
	uart_send_byte_sync(uart,TRANS_SOF);
	for(i=0;i<length;i++)
		serial_send_byte(trans,data[i]);
	crc=crc8_ccitt(CRC8_PRESET,data,length);
	serial_send_byte(trans,crc);
	uart_send_byte_sync(uart,TRANS_EOF);
	uart_stop_tx(uart);
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
