#ifndef __DRV_LED_H
#define __DRV_LED_H
#include "platform/gpio.h"

struct led
{
	struct gpio *gpio;
	int8u on_value;
};
#define DELCARE_LED(name,x,on) DECLARE_GPIO(led_##name,x);struct led led_##name={&gpio_led_##name,on}
void led_init(struct led *led,GPIOMode_TypeDef mode);
void led_on(struct led *led);
void led_off(struct led *led);
void led_toggle(struct led *led);
#endif //__DRV_LED_H
