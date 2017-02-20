#include "includes.h"
#include "drivers/fm24c16.h"

BOOL fm24c16_read(struct fm24c16 *fm24,int16u address,void* pBuffer,int16u len)
{
	int i;
	if(address+len>FM24_MAX_ADDRESS)
		return FALSE;
	if(!i2c_start(fm24->i2c))
		return FALSE;
	if(!i2c_select_write(fm24->i2c,((address>>7)&0x0E) | fm24->address))
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
	if(!i2c_select_read(fm24->i2c,((address>>7)&0x0E) | fm24->address))
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

BOOL fm24c16_write(struct fm24c16 *fm24,int16u address,void* pBuffer,int16u len)
{
	int i;
	if(address+len>FM24_MAX_ADDRESS)
		return FALSE;
	if(!i2c_start(fm24->i2c))
		return FALSE;
	if(!i2c_select_write(fm24->i2c,((address>>7)&0x0E) | fm24->address))
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

BOOL fm24c16_check(struct fm24c16 *fm24,int16u address,void* pBuffer,int16u len)
{
  int8u buf[32];
  while(len>0)
  {
    if(len<32)
    {
      if(!fm24c16_read(fm24,address,buf,len))
        return FALSE;
      if(memcmp(pBuffer,buf,len)==0)
        return TRUE;
      else return FALSE;
    }
    else
    {
      if(!fm24c16_read(fm24,address,buf,32))
        return FALSE;
      if(memcmp(pBuffer,buf,32)==0)
      {
        len-=32;
        address+=32;
        continue;
      }
      else
        return FALSE;
    }
  }
  return TRUE;
}


