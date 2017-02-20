#include "platform/delay.h"
#include "platform/i2c.h"

#define SET_SDA()	do{gpio_set(i2c->sda);delay_us(10);}while(0)
#define RESET_SDA()	do{gpio_reset(i2c->sda);delay_us(10);}while(0)
#define SET_SCL()	do{gpio_set(i2c->scl);delay_us(10);}while(0)
#define RESET_SCL()	do{gpio_reset(i2c->scl);delay_us(10);}while(0)
#define READ_SDA() gpio_read_input(i2c->sda)

void i2c_init(struct i2c_adapter *i2c)
{
	gpio_init(i2c->scl,GPIO_Mode_Out_OD);
	gpio_init(i2c->sda,GPIO_Mode_Out_OD);
}
BOOL i2c_start(struct i2c_adapter *i2c)
{
		//int8u ret;
	SET_SDA();
	SET_SCL();
	READ_SDA();
	RESET_SDA();
	RESET_SCL();
	return TRUE;
}
void i2c_stop(struct i2c_adapter *i2c)
{
	//int8u ret;
	RESET_SDA();
	SET_SCL();
	SET_SDA();
}
void i2c_ack(struct i2c_adapter *i2c)
{
	RESET_SDA();
	SET_SCL();
	RESET_SCL();
	SET_SDA();
}

void i2c_nak(struct i2c_adapter *i2c)
{
	SET_SDA();
	SET_SCL();
	RESET_SCL();
}


BOOL i2c_send(struct i2c_adapter *i2c,int8u b)
{
	BOOL ret;
	int i;
	for(i=0;i<8;i++)
	{
		
		if(b&0x80)
			SET_SDA();
		else
			RESET_SDA();	
		SET_SCL();
		b<<=1;
		RESET_SCL();
	}
	SET_SDA();
	SET_SCL();
	
	ret=!READ_SDA();//低电平识别ACK
	RESET_SCL();
	return ret;
} 

int8u i2c_receive(struct i2c_adapter *i2c)
{
	int i;
	int8u b=0;
	SET_SDA();
	for(i=0;i<8;i++)
	{
		SET_SCL();
		b<<=1;
		b|=READ_SDA();
		RESET_SCL();
	}
	return b;
}

BOOL i2c_select_read(struct i2c_adapter *i2c,int8u address)
{
	return i2c_send(i2c,address|0x01); 
}

BOOL i2c_select_write(struct i2c_adapter *i2c,int8u address)
{
	return i2c_send(i2c,address&0xfe); 
}

