#include "includes.h"
#include "drivers/at24cl64.h"

#define PAGE_SIZE 32

BOOL at24c64_read(struct at24c64 *at24,int16u address,void* pBuffer,int16u len)
{
	int i;
	if(address+len>AT24C64_MAX_ADDRESS)
		return FALSE;
	if(!i2c_start(at24->i2c))
		return FALSE;
	if(!i2c_select_write(at24->i2c,at24->address))
	{
		i2c_stop(at24->i2c);
		return FALSE;
	}
	if(!i2c_send(at24->i2c,address>>8))
	{
		i2c_stop(at24->i2c);
		return FALSE;
	}
	if(!i2c_send(at24->i2c,address&0xff))
	{
		i2c_stop(at24->i2c);
		return FALSE;
	}
		
	if(!i2c_start(at24->i2c))
		return FALSE;	
	if(!i2c_select_read(at24->i2c,at24->address))
	{
		i2c_stop(at24->i2c);
		return FALSE;
	}

		for(i=0;i<len-1;i++)
		{
			*((int8u*)pBuffer+i)=i2c_receive(at24->i2c);
			i2c_ack(at24->i2c);
		}
	
	*((int8u*)pBuffer+i)=i2c_receive(at24->i2c);
	i2c_nak(at24->i2c);
	i2c_stop(at24->i2c);
	return TRUE;
}

BOOL at24c64_write(struct at24c64 *at24,int16u address,void* pBuffer,int16u len)
{
	int i;
	if(address+len>AT24C64_MAX_ADDRESS)
		return FALSE;
	if(!i2c_start(at24->i2c))
		return FALSE;
	if(!i2c_select_write(at24->i2c,at24->address))
	{
		i2c_stop(at24->i2c);
		return FALSE;
	}
	if(!i2c_send(at24->i2c,address>>8))
	{
		i2c_stop(at24->i2c);
		return FALSE;
	}
	if(!i2c_send(at24->i2c,address&0xff))
	{
		i2c_stop(at24->i2c);
		return FALSE;
	}
		
	for(i=0;i<len;i++)
	{      
		if((i!=0)&&((address+i)%PAGE_SIZE==0))
		{ //出现新的页起始地址
			i2c_stop(at24->i2c);					//结束本页，等待5MS后发送下一页地址重新开始写
			delay_ms(5);
			if(!i2c_start(at24->i2c))
				return FALSE;	     
			if(!i2c_select_write(at24->i2c,at24->address))
			{
				i2c_stop(at24->i2c);
				return FALSE;
			}			
			if(!i2c_send(at24->i2c,(address+i)>>8))
			{
				i2c_stop(at24->i2c);
				return FALSE;
			}
			if(!i2c_send(at24->i2c,(address+i)&0xff))
			{
				i2c_stop(at24->i2c);
				return FALSE;
			}     
		}
		if( !i2c_send(at24->i2c,(((int8u*)pBuffer)[i])))
		{//写一个字节		
			i2c_stop(at24->i2c);			
			return FALSE;
		}
	}
	i2c_stop(at24->i2c);
	delay_ms(5);
	return TRUE;
}


BOOL at24c64_check(struct at24c64 *fm24,int16u address,void* pBuffer,int16u len)
{
	return TRUE;
}

