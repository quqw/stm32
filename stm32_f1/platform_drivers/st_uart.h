#ifndef __PLATFORM_UART_H
#define __PLATFORM_UART_H
#include "common_types.h"
#include "stm32f10x_usart.h"


typedef struct st_uart
{
	comm_port_t base;
	void (*init)(struct st_uart *this,USART_InitTypeDef *init);
	void (*set_nvic)(struct st_uart* this,uint8_t pre_priority,uint8_t sub_priority);
	
}st_uart_t;

#define DELCARE_ST_UART(name,usart)	st_uart_t st_uart_##name={{usart},st_uart_init}
void st_uart_init(st_uart_t *this,USART_InitTypeDef *init);
void st_uart_set_nvic(st_uart_t* this,uint8_t pre_priority,uint8_t sub_priority);
void st_uart_set_baudrate(comm_port_t* this,int32u baudrate,int16u word_length,int16u stop_bit,int16u parity,uint16_t hardware_ctl);
int st_uart_send_byte(comm_port_t *this,uint8_t b);
int st_uart_send_byte_sync(comm_port_t *this,int8u b);
int st_uart_send_bytes(comm_port_t *this,const void* buf,int32u length);
int st_uart_send_bytes_sync(comm_port_t *this,const void* buf,int32u length);
int st_uart_send_string(comm_port_t *this,string s);
int st_uart_send_string_sync(comm_port_t *this,string s);
int st_uart_read_bytes(comm_port_t *this,void* out,uint32_t length);
int st_uart_read_byte(comm_port_t *this,void* out);
void st_uart_start_rx(comm_port_t *this);
void st_uart_stop_rx(comm_port_t *this);
void st_uart_start_tx(comm_port_t *this);
void st_uart_stop_tx(comm_port_t *this);
void st_uart_clear(comm_port_t *this);
void st_uart_preapre_tx(comm_port_t *this);
void st_uart_preapre_rx(comm_port_t *this);

#endif //__UART_H
