#include "includes.h"
#include "drivers/at24c16.h"

BOOL at24c16_read(struct at24c16 *fm24,int16u address,void* pBuffer,int16u len)
{
	int i;
	if(address+len>AT24C16_MAX_ADDRESS)
		return FALSE;
	if(!i2c_start(fm24->i2c))
		return FALSE;
	if(!i2c_select_write(fm24->i2c,((address>>7)&0x0E) |(fm24->address &0xf0)))
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
	if(!i2c_select_read(fm24->i2c,((address>>7)&0x0E) | (fm24->address &0xf0)))
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

BOOL at24c16_write(struct at24c16 *fm24,int16u address,void* pBuffer,int16u len)
{
	int i;
	if(address+len>AT24C16_MAX_ADDRESS)
		return FALSE;
	if(address%16)
	{
		if(!i2c_start(fm24->i2c))
			return FALSE;	
		if(!i2c_select_write(fm24->i2c,((address>>7)&0x0E) | (fm24->address &0xf0)))//写高地址
		{
			i2c_stop(fm24->i2c);
			return FALSE;
		}
		if(!i2c_send(fm24->i2c,address&0xff))//写低地址
		{
			i2c_stop(fm24->i2c);
			return FALSE;
		}
	}
	for(i=0;i<len;i++)
	{
		if((address+i)%16==0)//先首页发送地址
		{
			if(!i2c_start(fm24->i2c))
				return FALSE;
			if(!i2c_select_write(fm24->i2c,(((address+i)>>7)&0x0E) | (fm24->address &0xf0)))//写高地址
			{
				i2c_stop(fm24->i2c);
				return FALSE;
			}
			if(!i2c_send(fm24->i2c,(address+i)&0xff))//写低地址
			{
				i2c_stop(fm24->i2c);
				return FALSE;
			}
		}
		if(!i2c_send(fm24->i2c,*((int8u*)pBuffer+i)))
		{
			i2c_stop(fm24->i2c);
			return FALSE;
		}
		if((address+i)%16==15)//最后一个字节，等待5ms
		{
			i2c_stop(fm24->i2c);
			delay_ms(5);//等待5ms写入时间

		}
	}
	i2c_stop(fm24->i2c);
	delay_ms(5);//等待5ms写入时间
	return TRUE;
}

BOOL at24c16_check(struct at24c16 *fm24,int16u address,void* pBuffer,int16u len)
{
  int8u buf[32];
  while(len>0)
  {
    if(len<32)
    {
      if(!at24c16_read(fm24,address,buf,len))
        return FALSE;
      if(memcmp(pBuffer,buf,len)==0)
        return TRUE;
      else return FALSE;
    }
    else
    {
      if(!at24c16_read(fm24,address,buf,32))
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


