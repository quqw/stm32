#ifndef __PLATFORM_GPIO_H
#define __PLATFORM_GPIO_H
#include "common_types.h"
#include "stm32f10x_gpio.h"
typedef struct st_gpio
{
	gpio_t base;
	uint16_t pin;
	volatile uint32_t *idr,*odr;
	void (*init)(struct st_gpio* this,GPIO_InitTypeDef *init_type);
}st_gpio_t;

void st_gpio_init(st_gpio_t* this,GPIO_InitTypeDef *init_type);
void st_gpio_set(gpio_t* this);
void st_gpio_reset(gpio_t* this);
void st_gpio_toggle(gpio_t* this);
uint16_t st_gpio_read_output(gpio_t* this);
uint16_t st_gpio_read_input(gpio_t* this);
void st_gpio_enable_isr(gpio_t* this);
void st_gpio_disable_isr(gpio_t* this);
void st_gpio_set_isr_trigger(gpio_t* this,exti_rigger_t trigger);
#define DECLARE_ST_GPIO(name,gpio,pin) \
	st_gpio_t st_gpio_##name={{gpio,0,},pin,0,0,st_gpio_init}


#endif //__PLATFORM_GPIO_H

