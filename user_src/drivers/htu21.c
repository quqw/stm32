#include "htu21.h"
static void _htu21_crc8540(int8u *crc,int8u data,int8u bits){
	*crc^=data;
	do{
		if(*crc & 0x80){
			*crc<<=1;
			*crc ^=0x31; //8540
		}else{
            *crc<<=1;
		}
	}while(--bits);
}

static int8u _htu21_crc(int8u* data,int len)
{
	int8u crc=0;
	int i;
	for(i=0;i<len;i++)
	{
		_htu21_crc8540(&crc,data[i],8);
	}
	return crc;
}
htu21_result_t htu21_trigger_t(struct htu21 *htu)
{
	i2c_start(htu->i2c);
	if(!i2c_select_write(htu->i2c,htu->address))
	{
		i2c_stop(htu->i2c);
		return HTU21_BUS_ERR;
	}
	if(!i2c_send(htu->i2c,HTU21_CMD_TRIGGER_T))
	{
		i2c_stop(htu->i2c);
		return HTU21_BUS_ERR;
	}
	return HTU21_OK;
}
htu21_result_t htu21_trigger_rh(struct htu21 *htu)
{
	i2c_start(htu->i2c);
	if(!i2c_select_write(htu->i2c,htu->address))
	{
		i2c_stop(htu->i2c);
		return HTU21_BUS_ERR;
	}
	if(!i2c_send(htu->i2c,HTU21_CMD_TRIGGER_RH))
	{
		i2c_stop(htu->i2c);
		return HTU21_BUS_ERR;
	}
	return HTU21_OK;
}
htu21_result_t htu21_read(struct htu21 *htu,int16u* out)
{
	int8u data[2];
	int8u crc;
	i2c_start(htu->i2c);
	if(!i2c_select_read(htu->i2c,htu->address))
	{
		return HTU21_BUSY;
	}
	data[0]=i2c_receive(htu->i2c);
	i2c_ack(htu->i2c);
	data[1]=i2c_receive(htu->i2c);
	i2c_ack(htu->i2c);
	crc=i2c_receive(htu->i2c);
	i2c_nak(htu->i2c);
	i2c_stop(htu->i2c);
	if(_htu21_crc(data,2)!=crc)
		return HTU21_CRC_ERR;
	*out=data[1]+(((int16u)data[0])<<8);
	return HTU21_OK;
}

htu21_result_t htu21_read_t(struct htu21 *htu,int16s* out)
{
	int16u val;
	htu21_result_t ret;
	ret=htu21_read(htu,&val);
	if(ret!=HTU21_OK)
		return ret;
	if((val&0x02)!=0)
		return HTU21_TYPE_ERR;
	*out=17572l*(val&0xfffc)/65536-4685;
	return HTU21_OK;
}
htu21_result_t htu21_read_rh(struct htu21 *htu,int16u* out)
{
	int16u val;
	htu21_result_t ret;
	ret=htu21_read(htu,&val);
	if(ret!=HTU21_OK)
		return ret;
	if((val&0x02)!=0x02)
		return HTU21_TYPE_ERR;
	*out=12500*(val&0xfffc)/65536-600;
	return HTU21_OK;
}
htu21_result_t htu21_set_config(struct htu21 *htu,int8u config)
{
		i2c_start(htu->i2c);
	if(!i2c_select_write(htu->i2c,htu->address))
	{
		i2c_stop(htu->i2c);
		return HTU21_BUS_ERR;
	}
	if(!i2c_send(htu->i2c,HTU21_CMD_WRITE_REG))
	{
		i2c_stop(htu->i2c);
		return HTU21_BUS_ERR;
	}
	if(!i2c_send(htu->i2c,config))
	{
		i2c_stop(htu->i2c);
		return HTU21_BUS_ERR;
	}
	i2c_stop(htu->i2c);
	return HTU21_OK;
}

htu21_result_t htu21_read_config(struct htu21 *htu,int8u* config)
{
	i2c_start(htu->i2c);
	if(!i2c_select_write(htu->i2c,htu->address))
	{
		i2c_stop(htu->i2c);
		return HTU21_BUS_ERR;
	}
	if(!i2c_send(htu->i2c,HTU21_CMD_READ_REG))
	{
		i2c_stop(htu->i2c);
		return HTU21_BUS_ERR;
	}
		if(!i2c_select_read(htu->i2c,htu->address))
	{
		i2c_stop(htu->i2c);
		return HTU21_BUS_ERR;
	}
	*config=i2c_receive(htu->i2c);
	i2c_nak(htu->i2c);
	i2c_stop(htu->i2c);
	return HTU21_OK;
	
}

