#include "includes.h"
#include "modbus_master.h"
#include "assist/assist.h"
#include "platform/delay.h"

#define RECEIVE_TIME_OUT_US 1000000

void modbus_master_init(struct modbus_master* mb,int32u bandrate,int16u word_length,int16u stop_bit,int16u parity)
{
	uart_init(mb->uart,bandrate,word_length,stop_bit,parity);
		/* If baudrate > 19200 then we should use the fixed timer values
	 * t35 = 1750us. Otherwise t35 must be 3.5 times the character time.
	 */
	if( bandrate > 19200 )
	{
			mb->expirt_us = 1750;
	}
	else
	{
		/*
		每个字节的时间（s）：11/bandrate
		us~ 1000000/bandrate*11
		3.5bytes 3500000*11/bandrate=
		*/
			mb->expirt_us= 38500000/bandrate;
	}
}

static void modbus_master_wait_expirt(struct modbus_master* mb)
{
	delay_us(mb->expirt_us);
}

static BOOL modbus_master_receive(struct modbus_master* mb)
{
	int8u out;
	BOOL start=FALSE;
	int16u crc;
	int32u expirt=0;
	mb->receive_length=0;
	while(expirt<RECEIVE_TIME_OUT_US)
	{
		if(uart_read_byte(mb->uart,&out)==1)
		{
			start=TRUE;
			if(mb->receive_length>MODBUS_BUFFER_LEN)
				return FALSE;
			mb->receive_buffer[mb->receive_length++]=out;
			expirt=0;
		}
		else if(!start)
		{
			delay_ms(1);//vTaskDelay(1);
			expirt+=1000;
		}
		else
		{
			delay_us(1);
			expirt++;
			if(expirt>mb->expirt_us)
			{
				if(mb->receive_length<5)
					return FALSE;
				crc=mb_crc16( mb->receive_buffer, mb->receive_length-2 );
				if((crc&0xff)==mb->receive_buffer[mb->receive_length-2] && 
					(crc>>8)==mb->receive_buffer[mb->receive_length-1])//crc校验低字节在前，高字节在后
				{
					mb->receive_length-=2;
					return TRUE;
				}
				return FALSE;
			}
		}
	}
	return FALSE;
	
}
//func 01 读线圈
MB_EXCEPION modbus_master_read_coil_rtu(struct modbus_master* mb,int8u slave_addr,int16u start_addr,int16u count,int8u* read_out)
{
	int16u crc;
	mb->send_buffer[0]=slave_addr;
	mb->send_buffer[1]=MB_FUNC_READ_COILS;
	mb->send_buffer[2]=start_addr>>8;//地址高字节
	mb->send_buffer[3]=start_addr&0xff;//地址低字节
	mb->send_buffer[4]=count>>8;//数量高字节
	mb->send_buffer[5]=count&0xff;//数量低字节
	crc=mb_crc16( mb->send_buffer, 6 );
	mb->send_buffer[6]=crc&0xff;//crc校验低字节在前
	mb->send_buffer[7]=crc>>8;//
	mb->send_length=8;
	OSLOCK_RUN(
	uart_start_tx(mb->uart);
	uart_send_bytes_sync(mb->uart,mb->send_buffer,mb->send_length);
	uart_clear(mb->uart);
	uart_start_rx(mb->uart);//先开启接收，防止时间不标准的协议回应
	)
	modbus_master_wait_expirt(mb);//发送结束后等待3.5个超时时间
	if(!modbus_master_receive(mb))
	{
		uart_stop_rx(mb->uart);
		return MB_EX_ACKNOWLEDGE;
	}
	uart_stop_rx(mb->uart);
	if(mb->receive_buffer[0]!=slave_addr)
		return MB_EX_SLAVE_DEVICE_FAILURE;
	if((mb->receive_buffer[1] & MODBUS_FUNCTION_MASK)!=MB_FUNC_READ_COILS)
		return MB_EX_SLAVE_DEVICE_FAILURE;
	if(mb->receive_buffer[1] & MODBUS_FUNCTION_ERROR)
		return (MB_EXCEPION)(mb->receive_buffer[2]);
	if(mb->receive_buffer[2]>count/8+1)
		return MB_EX_SLAVE_DEVICE_FAILURE;
	memcpy(read_out,mb->receive_buffer+3,mb->receive_buffer[2]);
	return MB_EX_NONE;
}


//func 03 读多个寄存器
MB_EXCEPION modbus_master_read_registers_rtu(struct modbus_master* mb,int8u slave_addr,int16u reg_addr,int16u count,int16u* read_out)
{
	int16u crc,i;
	mb->send_buffer[0]=slave_addr;
	mb->send_buffer[1]=MB_FUNC_READ_HOLDING_REGISTER;
	mb->send_buffer[2]=reg_addr>>8;//地址高字节
	mb->send_buffer[3]=reg_addr&0xff;//地址低字节
	mb->send_buffer[4]=count>>8;//数量高字节
	mb->send_buffer[5]=count&0xff;//数量低字节
	crc=mb_crc16( mb->send_buffer, 6 );
	mb->send_buffer[6]=crc&0xff;//数量低字节,crc校验低字节在前
	mb->send_buffer[7]=crc>>8;//数量高字节
	mb->send_length=8;
	OSLOCK_RUN(
	uart_start_tx(mb->uart);
	uart_send_bytes_sync(mb->uart,mb->send_buffer,mb->send_length);
	uart_clear(mb->uart);
	uart_start_rx(mb->uart);//先开启接收，防止不标准的协议回应
	)
	modbus_master_wait_expirt(mb);//发送结束后等待3.5个超时时间
	if(!modbus_master_receive(mb))
	{
		uart_stop_rx(mb->uart);
		return MB_EX_ACKNOWLEDGE;
	}
	uart_stop_rx(mb->uart);
	if(mb->receive_buffer[0]!=slave_addr)
		return MB_EX_SLAVE_DEVICE_FAILURE;
	if((mb->receive_buffer[1] & MODBUS_FUNCTION_MASK)!=MB_FUNC_READ_HOLDING_REGISTER)
		return MB_EX_SLAVE_DEVICE_FAILURE;
	if(mb->receive_buffer[1] & MODBUS_FUNCTION_ERROR)
		return (MB_EXCEPION)(mb->receive_buffer[2]);
	if(mb->receive_buffer[2]!=count*2)//返回的字节数量
		return MB_EX_SLAVE_DEVICE_FAILURE;
	for(i=0;i<count;i++)//数据输出并转换为小端模式
	{
		read_out[i]=mb->receive_buffer[3+2*i+1] |(mb->receive_buffer[3+2*i]<<8);
	}
	return MB_EX_NONE;
}


//func 06 写单个寄存器
MB_EXCEPION modbus_master_write_register_rtu(struct modbus_master* mb,int8u slave_addr,int16u reg_addr,int16u value)
{
	int16u crc;
	mb->send_buffer[0]=slave_addr;
	mb->send_buffer[1]=MB_FUNC_WRITE_REGISTER;
	mb->send_buffer[2]=reg_addr>>8;//地址高字节
	mb->send_buffer[3]=reg_addr&0xff;//地址低字节
	mb->send_buffer[4]=value>>8;//数量高字节
	mb->send_buffer[5]=value&0xff;//数量低字节
	crc=mb_crc16( mb->send_buffer, 6 );
	mb->send_buffer[6]=crc&0xff;//数量低字节,crc校验低字节在前
	mb->send_buffer[7]=crc>>8;//数量高字节
	mb->send_length=8;
	OSLOCK_RUN(
	uart_start_tx(mb->uart);
	uart_send_bytes_sync(mb->uart,mb->send_buffer,mb->send_length);
	uart_clear(mb->uart);
	uart_start_rx(mb->uart);//先开启接收，防止不标准的协议回应
	)
	modbus_master_wait_expirt(mb);//发送结束后等待3.5个超时时间
	if(!modbus_master_receive(mb))
	{
		uart_stop_rx(mb->uart);
		return MB_EX_ACKNOWLEDGE;
	}
	uart_stop_rx(mb->uart);
	if(mb->receive_buffer[0]!=slave_addr)
		return MB_EX_SLAVE_DEVICE_FAILURE;
	if((mb->receive_buffer[1] & MODBUS_FUNCTION_MASK)!=MB_FUNC_WRITE_REGISTER)
		return MB_EX_SLAVE_DEVICE_FAILURE;
	if(mb->receive_buffer[1] & MODBUS_FUNCTION_ERROR)
		return (MB_EXCEPION)(mb->receive_buffer[2]);
	if(mb->receive_buffer[2]!=(reg_addr>>8) || mb->receive_buffer[3]!=(reg_addr&0xff))
		return MB_EX_SLAVE_DEVICE_FAILURE;
	if(mb->receive_buffer[4]!=(value>>8) || mb->receive_buffer[5]!=(value&0xff))
		return MB_EX_SLAVE_DEVICE_FAILURE;
	return MB_EX_NONE;
		
}


//////////////
//mdumo stand alone
/////////////
//func 03 读多个寄存器
MB_EXCEPION modbus_standalone_master_read_registers_rtu(struct modbus_master* mb,int8u slave_addr,int16u reg_addr,int16u count,int16u* read_out)
{
	int16u crc,i;
	mb->send_buffer[0]=MB_FUNC_READ_HOLDING_REGISTER;
	mb->send_buffer[1]=reg_addr>>8;//地址高字节
	mb->send_buffer[2]=reg_addr&0xff;//地址低字节
	mb->send_buffer[3]=count>>8;//数量高字节
	mb->send_buffer[4]=count&0xff;//数量低字节
	crc=mb_crc16( mb->send_buffer, 5 );
	mb->send_buffer[5]=crc&0xff;//数量低字节,crc校验低字节在前
	mb->send_buffer[6]=crc>>8;//数量高字节
	mb->send_length=5;
	OSLOCK_RUN(
	uart_start_tx(mb->uart);
	uart_send_bytes_sync(mb->uart,mb->send_buffer,mb->send_length);
	uart_clear(mb->uart);
	uart_start_rx(mb->uart);//先开启接收，防止不标准的协议回应
	)
	modbus_master_wait_expirt(mb);//发送结束后等待3.5个超时时间
	if(!modbus_master_receive(mb))
	{
		uart_stop_rx(mb->uart);
		return MB_EX_ACKNOWLEDGE;
	}
	uart_stop_rx(mb->uart);
	if(mb->receive_buffer[0]!=slave_addr)
		return MB_EX_SLAVE_DEVICE_FAILURE;
	if((mb->receive_buffer[1] & MODBUS_FUNCTION_MASK)!=MB_FUNC_READ_HOLDING_REGISTER)
		return MB_EX_SLAVE_DEVICE_FAILURE;
	if(mb->receive_buffer[1] & MODBUS_FUNCTION_ERROR)
		return (MB_EXCEPION)(mb->receive_buffer[2]);
	if(mb->receive_buffer[2]!=count*2)//返回的字节数量
		return MB_EX_SLAVE_DEVICE_FAILURE;
	for(i=0;i<count;i++)//数据输出并转换为小端模式
	{
		read_out[i]=mb->receive_buffer[3+2*i+1] |(mb->receive_buffer[3+2*i]<<8);
	}
	return MB_EX_NONE;
}


