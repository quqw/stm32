#include "includes.h"
#include "drivers/fm24cl64.h"


BOOL fm24cl64_read(struct fm24cl64 *fm24,int16u address,void* pBuffer,int16u len)
{
	int i;
	if(address+len>FM24_MAX_ADDRESS)
		return FALSE;
	if(!i2c_start(fm24->i2c))
		return FALSE;
	if(!i2c_select_write(fm24->i2c,fm24->address))
	{
		i2c_stop(fm24->i2c);
		return FALSE;
	}
	if(!i2c_send(fm24->i2c,(address&0xff00)>>8))
	{
		i2c_stop(fm24->i2c);
		return FALSE;
	}
	if(!i2c_send(fm24->i2c,address&0xff))
	{
		i2c_stop(fm24->i2c);
		return FALSE;
	}
		
	if(!i2c_start(fm24->i2c))
		return FALSE;	
	if(!i2c_select_read(fm24->i2c,fm24->address))
	{
		i2c_stop(fm24->i2c);
		return FALSE;
	}
	for(i=0;i<len-1;i++)
	{
		*((int8u*)pBuffer+i)=i2c_receive(fm24->i2c);
		i2c_ack(fm24->i2c);
	}
	*((int8u*)pBuffer+i)=i2c_receive(fm24->i2c);
	i2c_nak(fm24->i2c);
	i2c_stop(fm24->i2c);
	return TRUE;
}

BOOL fm24cl64_write(struct fm24cl64 *fm24,int16u address,void* pBuffer,int16u len)
{
	int i;
	if(address+len>FM24_MAX_ADDRESS)
		return FALSE;
	if(!i2c_start(fm24->i2c))
		return FALSE;
	if(!i2c_select_write(fm24->i2c,fm24->address))
	{
		i2c_stop(fm24->i2c);
		return FALSE;
	}
	if(!i2c_send(fm24->i2c,(address&0xff00)>>8))
	{
		i2c_stop(fm24->i2c);
		return FALSE;
	}
	if(!i2c_send(fm24->i2c,address&0xff))
	{
		i2c_stop(fm24->i2c);
		return FALSE;
	}
	for(i=0;i<len;i++)
	{
		if(!i2c_send(fm24->i2c,*((int8u*)pBuffer+i)))
		{
			i2c_stop(fm24->i2c);
			return FALSE;
		}
	}
	i2c_stop(fm24->i2c);
	return TRUE;
}



