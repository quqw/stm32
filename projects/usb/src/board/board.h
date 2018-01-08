#ifndef __PORTABLE_BOARD_H
#define __PORTABLE_BOARD_H
#include "types.h"
#include "platform/gpio.h"
#include "platform/io8080.h"
#include "platform/gpio_bus.h"
#include "platform/uart.h"
#include "platform/pwm.h"
#include "drivers/lcd/lm6029.h"
#include "drivers/led.h"
#include "drivers/modbus_master.h"
#include "drivers/at24c16.h"
#include "drivers/lcd/lcd.h"

#include "communication/comm_types.h"
#include "communication/serial_transfer.h"
#include "communication/usb_transfer.h"
#include "communication/comm_app.h"
#define LENGTH_PER_GAP	10//cm

//#define KEY_NONE		0
//#define KEY_UP			1
//#define KEY_DOWN		2
//#define KEY_LEFT		3
//#define KEY_RIGHT		4

#define BUTTON_NULL 0
//#define BUTTON_RIGHT		(1<<2)
//#define BUTTON_LEFT		(1<<3)
//#define BUTTON_UP				1
//#define BUTTON_DOWN		(1<<1)
//#define BUTTON_ENTER	(1<<4)


#define BUTTON_DOWN		(1<<4)
#define BUTTON_UP			(1<<5)
#define BUTTON_LEFT		(1<<3)
#define BUTTON_RIGHT		(1<<2)
#define BUTTON_ENTER		(1<<1)
#define BUTTON_ESC	(1)

extern struct gpio gpio_test;
extern struct gpio gpio_relay1,gpio_relay2,gpio_relay3;
extern struct gpio gpio_door,gpio_voltage;

#ifdef __DEBUG
#define TEST_OUT do{	gpio_set(&gpio_test);gpio_reset(&gpio_test);}while(0)
#else
#define TEST_OUT 
#endif
void board_init(void);

BOOL board_door_open(void);
BOOL board_item_ready(void);
BOOL board_voltage_signal(void);
void board_relay_on(struct gpio* gpio);
void board_relay_off(struct gpio* gpio);
void board_relay_toggle(struct gpio* gpio);
int32u board_key_get(void);
#endif

