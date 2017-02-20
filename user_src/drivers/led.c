#include "includes.h"
#include "drivers/led.h"

void led_init(struct led *led,GPIOMode_TypeDef mode)
{
	gpio_init(led->gpio,mode);
}

void led_on(struct led *led)
{
	led->on_value?gpio_set(led->gpio):gpio_reset(led->gpio);
}

void led_off(struct led *led)
{
	led->on_value?gpio_reset(led->gpio):gpio_set(led->gpio);
}

void led_toggle(struct led *led)
{
	gpio_toggle(led->gpio);
}

