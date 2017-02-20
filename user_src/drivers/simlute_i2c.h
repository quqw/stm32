#ifndef __PLATFORM_I2C_H
#define __PLATFORM_I2C_H
#include "platform/gpio.h"
struct i2c_adapter
{
	struct gpio *scl;
	struct gpio *sda;
};

#define DECLARE_I2C_ADAPTER(name,scl_x,sda_x)	DECLARE_GPIO(i2c_##name##_scl,scl_x);\
																					DECLARE_GPIO(i2c_##name##_sda,sda_x); \
																					struct i2c_adapter i2c_##name={&gpio_i2c_##name##_scl,&gpio_i2c_##name##_sda}

void i2c_init(struct i2c_adapter *i2c);
BOOL i2c_start(struct i2c_adapter *i2c);
void i2c_stop(struct i2c_adapter *i2c);
void i2c_ack(struct i2c_adapter *i2c);
void i2c_nak(struct i2c_adapter *i2c);
BOOL i2c_send(struct i2c_adapter *i2c,int8u b);
int8u i2c_receive(struct i2c_adapter *i2c);
BOOL i2c_select_read(struct i2c_adapter *i2c,int8u address);
BOOL i2c_select_write(struct i2c_adapter *i2c,int8u address);


#endif
