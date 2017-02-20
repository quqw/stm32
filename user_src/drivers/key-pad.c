#include "includes.h"
#include "platform/key-pad.h"

void keypad_init(struct keypad *keypad)
{
	gpio_bus_init(keypad->key_bus_in,GPIO_Mode_IN_FLOATING);
	gpio_bus_init(keypad->key_bus_out,GPIO_Mode_Out_PP);
	keypad_set_out(keypad,0);
}

void  keypad_set_out(struct keypad *keypad,int8u mask)
{
	gpio_bus_set_data(keypad->key_bus_out,mask);
}

int16u keypad_get(struct keypad *keypad){
  int16u in_value;
  int i;
  keypad_set_out(keypad,0x00);
	in_value=gpio_bus_read_input(keypad->key_bus_in);
	if( in_value==keypad->key_bus_in->mask)
	{
		return KEYVALUE_NULL;
	}
	for(i=0;i<keypad->key_bus_out->width;i++)
	{
		keypad_set_out(keypad,~(0x01<<i));
		in_value=gpio_bus_read_input(keypad->key_bus_in);
		if(in_value!=keypad->key_bus_in->mask)
		{   //·¢ÏÖ°´Å¥
			keypad_set_out(keypad,0xFF);
			return (((~in_value) & keypad->key_bus_in->mask)<<keypad->key_bus_out->width)+i;
		}
	}
	keypad_set_out(keypad,0xFF);
  return KEYVALUE_NULL;
}

