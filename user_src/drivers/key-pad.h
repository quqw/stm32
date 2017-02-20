#ifndef __DRV_KEY_PAD_H
#define __DRV_KEY_PAD_H
#include "platform/gpio_bus.h"

#define KEYVALUE_NULL 0

struct keypad
{
	struct gpio_bus *key_bus_in;
	struct gpio_bus *key_bus_out;
};

#define DELCARE_KEY_PAD(name,in,out)	struct keypad keypad_##name={&in,&out}

void keypad_init(struct keypad *keypad);
void  keypad_set_out(struct keypad *keypad,int8u mask);
int16u keypad_get(struct keypad *keypad);
#endif

