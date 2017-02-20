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
void simulate_i8080_bus_write(struct io8080_bus *bus,int32u address,int16u dat);//地址线只有一根，只有0和1两种
int16u simulate_i8080_bus_read(struct io8080_bus *bus,int32u address);

//databus_start数据线起始位序号。默认8位数据方式
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


																		
//fsmc方式

void fsmc_i8080_bus_init(struct io8080_bus *bus);
void fsmc_i8080_bus_write(struct io8080_bus *bus,int32u address,int16u dat);//地址只有0和1两种
int16u fsmc_i8080_bus_read(struct io8080_bus *bus,int32u address);	
/*
data_width:数据位长度
data_offset：数据线起始位偏移
addr_x:地址线数字															
*/																		
#define DECLARE_FSMC_I8080_BUS(name,data_width,data_offset,addr_x)\
																		struct io8080_bus io8080_bus_##name={fsmc_i8080_bus_init,\
																		fsmc_i8080_bus_write,\
																		fsmc_i8080_bus_read,0,0,0,0,0,\
																		data_width,data_offset,addr_x}


#endif

