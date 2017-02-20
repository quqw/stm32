#include "pm25.h"

void pm25_init(struct pm25* pm25)
{
	uart_start_rx(pm25->uart);
}

static BOOL _pm25_cal_data(struct pm25* pm25)
{
	int i;
	int8u check=0;
	if(pm25->length<7)
		return FALSE;
	if((pm25->buf[pm25->length-1]!=0xff)||(pm25->buf[pm25->length-7]!=0xaa))
		return FALSE;
	for(i=pm25->length-6;i<pm25->length-2;i++)
	{
		check+=pm25->buf[i];
	}
	if(check!=pm25->buf[pm25->length-2])
		return FALSE;
	pm25->value=((int16u)pm25->buf[pm25->length-6])<<8;
	pm25->value+=pm25->buf[pm25->length-5];
	pm25->value*=1750;//(100*5*3.5)·Å´ó100±¶
	pm25->value/=1024;
	pm25->time_out=PM25_TIMEOUT;
	return TRUE;
}
static void _pm25_push_data(struct pm25* pm25,int8u* data,int8u length)
{
	int i;
	if(pm25->length>=PM25_BUF_LENGTH)
		pm25->length=0;
	for(i=0;i<length;i++)
	{
		pm25->buf[pm25->length++]=data[i];
		if(_pm25_cal_data(pm25))
		{
			pm25->length=0;
		}
	}
}

void pm25_tick(struct pm25* pm25,int ticks)
{
	int8u buf[8];
	int8u len;
	if(pm25->time_out<=ticks)
	{
		pm25->value=-1;
	}
	else
		pm25->time_out-=ticks;
	len=uart_read_bytes(pm25->uart,buf,7);
	if(len>0)
		_pm25_push_data(pm25,buf,len);
}

int32s pm25_read(struct pm25* pm25)
{
	return pm25->value;
}

