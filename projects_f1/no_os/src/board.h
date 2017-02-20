#ifndef __PORTABLE_BOARD_H
#define __PORTABLE_BOARD_H
#include "types.h"

#define LED1 0x01
#define LED2 0x02
void board_init(void);
void board_led_on(int led,int time);
void board_led_off(int led);
void board_led_toggle(int led);
void led2_tick(int ticks);
void led1_tick(int ticks);
void board_rf_power_on(void);
void board_rf_power_off(void);
void board_485_tx(void);
void board_485_rx(void);
void uart_send_byte_sync(USART_TypeDef *uart,int8u b);
#endif

