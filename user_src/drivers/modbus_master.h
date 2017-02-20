#ifndef MODBUS_MASTER_H
#define MODBUS_MASTER_H
#include "types.h"
#include "platform/uart.h"
#include "modbus/mbproto.h"
#define MODBUS_BUFFER_LEN	128
#define MODBUS_FUNCTION_MASK	0x7f
#define MODBUS_FUNCTION_ERROR	0x80


//发送，停止谍3.5个字节时间，开启接收
struct modbus_master
{
	struct uart *uart;
	MB_EXCEPION (*read_coil)(struct modbus_master* mb,int8u slave_addr,int16u start_addr,int16u count,int8u* read_out);
	MB_EXCEPION (*read_registers)(struct modbus_master* mb,int8u slave_addr,int16u reg_addr,int16u count,int16u* read_out);
	MB_EXCEPION (*write_register)(struct modbus_master* mb,int8u slave_addr,int16u reg_addr,int16u value);
	int8u send_buffer[MODBUS_BUFFER_LEN];
	int8u receive_buffer[MODBUS_BUFFER_LEN];
	int16u send_length;
	int16u receive_length;
	int32u expirt_us;
};

//x:串口号
//tx_x:发送脚的号码，rx_x：接收脚的编号
#define DECLARE_MODBUS_MASTER(x) \
																					struct modbus_master	modbus_master_##x={&uart_##x,\
																																			modbus_master_read_coil_rtu,\
																																			modbus_master_read_registers_rtu,\
																																			modbus_master_write_register_rtu}


void modbus_master_init(struct modbus_master* mb,int32u bandrate,int16u word_length,int16u stop_bit,int16u parity);
MB_EXCEPION modbus_master_read_coil_rtu(struct modbus_master* mb,int8u slave_addr,int16u start_addr,int16u count,int8u* read_out);
MB_EXCEPION modbus_master_read_registers_rtu(struct modbus_master* mb,int8u slave_addr,int16u reg_addr,int16u count,int16u* read_out);
MB_EXCEPION modbus_master_write_register_rtu(struct modbus_master* mb,int8u slave_addr,int16u reg_addr,int16u value);
			
MB_EXCEPION modbus_standalone_master_read_registers_rtu(struct modbus_master* mb,int8u slave_addr,int16u reg_addr,int16u count,int16u* read_out);																																			
#endif

