#ifndef __PLATFORM_IO_H
#define __PLATFORM_IO_H
#include "platform/gpio.h"
#include "platform/gpio_bus.h"

struct io8080_bus
{
	void (*init)(struct io8080_bus *bus);
	void (*write)(struct io8080_bus *bus,int32u address,int16u dat);
	int16u (*read)(struct io8080_bus *bus,int32u address);
	struct gpio_bus *data_bus;
	struct gpio *rd;
	struct gpio *wr;
	struct gpio *cs;
	struct gpio *address0;
	int8u data_width;
	int8u data_offset;
	int8u addr_x;
};

void simulate_i8080_bus_init(struct io8080_bus *bus);
void simulate_i8080_bus_write(struct io8080_bus *bus,int32u address,int16u dat);//��ַ��ֻ��һ����ֻ��0��1����
int16u simulate_i8080_bus_read(struct io8080_bus *bus,int32u address);

//databus_start��������ʼλ��š�Ĭ��8λ���ݷ�ʽ
#define DECLARE_SIMULATE_IO_BUS(name,databus_start,rd_x,wr_x,cs_x,rs_x)	\
								DECLARE_GPIO(io_bus_##name##_cs,cs_x);\
								DECLARE_GPIO(io_bus_##name##_rs,rs_x);\
								DECLARE_GPIO(io_bus_##name##_wr,wr_x);\
								DECLARE_GPIO(io_bus_##name##_rd,rd_x);\
								DECLARE_GPIO_BUS(io_bus_##name##_data,databus_start,8);\
								struct io8080_bus io8080_bus_##name={simulate_i8080_bus_init,\
																		simulate_i8080_bus_write,\
																		simulate_i8080_bus_read,\
																		&gpio_bus_io_bus_##name##_data,\
                                                                        &gpio_io_bus_##name##_rd,\
																		&gpio_io_bus_##name##_wr,&gpio_io_bus_##name##_cs,\
																		&gpio_io_bus_##name##_rs};


																		
//fsmc��ʽ

void fsmc_i8080_bus_init(struct io8080_bus *bus);
void fsmc_i8080_bus_write(struct io8080_bus *bus,int32u address,int16u dat);//��ַֻ��0��1����
int16u fsmc_i8080_bus_read(struct io8080_bus *bus,int32u address);	
/*
data_width:����λ����
data_offset����������ʼλƫ��
addr_x:��ַ������															
*/																		
#define DECLARE_FSMC_I8080_BUS(name,data_width,data_offset,addr_x)\
																		struct io8080_bus io8080_bus_##name={fsmc_i8080_bus_init,\
																		fsmc_i8080_bus_write,\
																		fsmc_i8080_bus_read,0,0,0,0,0,\
																		data_width,data_offset,addr_x}


#endif

