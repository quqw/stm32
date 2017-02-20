#include "includes.h"
#include "st_gpio.h"

#define PTR_ST_GPIO	((st_gpio_t*)this)
#define PTR_ST_GPIO_PORT	((GPIO_TypeDef *)(this->port_addr))

void st_gpio_init(st_gpio_t* this,GPIO_InitTypeDef *init_type)
{
	st_gpio_t* stm_gpio=(st_gpio_t*)this;
	gpio_t* gpio=(gpio_t*)this;
	init_type->GPIO_Pin=stm_gpio->pin;
	switch((uint32_t)(gpio->port_addr))
	{
		case (uint32_t)GPIOA:
			RCC_APB2PeriphResetCmd(RCC_APB2Periph_GPIOA, ENABLE);
			break;
		case (uint32_t)GPIOB:
			RCC_APB2PeriphResetCmd(RCC_APB2Periph_GPIOB, ENABLE);
			break;
		case (uint32_t)GPIOC:
			RCC_APB2PeriphResetCmd(RCC_APB2Periph_GPIOC, ENABLE);
			break;
		case (uint32_t)GPIOD:
			RCC_APB2PeriphResetCmd(RCC_APB2Periph_GPIOD, ENABLE);
			break;
		case (uint32_t)GPIOE:
			RCC_APB2PeriphResetCmd(RCC_APB2Periph_GPIOE, ENABLE);
			break;
		case (uint32_t)GPIOF:
			RCC_APB2PeriphResetCmd(RCC_APB2Periph_GPIOF, ENABLE);
			break;
		case (uint32_t)GPIOG:
			RCC_APB2PeriphResetCmd(RCC_APB2Periph_GPIOG, ENABLE);
			break;
	}
	switch(stm_gpio->pin)
	{
		case GPIO_Pin_0:
			gpio->pin_num=0;
		break;
		case GPIO_Pin_1:
			gpio->pin_num=1;
		break;
		case GPIO_Pin_2:
			gpio->pin_num=2;
		break;
		case GPIO_Pin_3:
			gpio->pin_num=3;
		break;
		case GPIO_Pin_4:
			gpio->pin_num=4;
		break;
		case GPIO_Pin_5:
			gpio->pin_num=5;
		break;
		case GPIO_Pin_6:
			gpio->pin_num=6;
		break;
		case GPIO_Pin_7:
			gpio->pin_num=7;
		break;
		case GPIO_Pin_8:
			gpio->pin_num=8;
		break;
		case GPIO_Pin_9:
			gpio->pin_num=9;
		break;
		case GPIO_Pin_10:
			gpio->pin_num=10;
		break;
		case GPIO_Pin_11:
			gpio->pin_num=11;
		break;
		case GPIO_Pin_12:
			gpio->pin_num=12;
		break;
		case GPIO_Pin_13:
			gpio->pin_num=13;
		break;
		case GPIO_Pin_14:
			gpio->pin_num=14;
		break;
		case GPIO_Pin_15:
			gpio->pin_num=15;
		break;
		default:
			gpio->pin_num=0;
	}
	GPIO_Init((GPIO_TypeDef*)(gpio->port_addr),init_type);
	gpio->set=st_gpio_set;
	gpio->reset=st_gpio_reset;
	gpio->toggle=st_gpio_toggle;
	gpio->read_input=st_gpio_read_input;
	gpio->read_output=st_gpio_read_output;
	gpio->enable_isr=st_gpio_enable_isr;
	gpio->disable_isr=st_gpio_disable_isr;
	gpio->set_isr_trigger=st_gpio_set_isr_trigger;
	stm_gpio->init=st_gpio_init;
	stm_gpio->idr=GPIO_IDR_ADDR(((gpio_t*)this)->port_addr,((gpio_t*)this)->pin_num);
	stm_gpio->odr=GPIO_ODR_ADDR(((gpio_t*)this)->port_addr,((gpio_t*)this)->pin_num);
}


void st_gpio_set(gpio_t* this)
{
  PTR_ST_GPIO_PORT->ODR |= PTR_ST_GPIO->pin;
}

void st_gpio_toggle(gpio_t* this)
{
  PTR_ST_GPIO_PORT->ODR ^= PTR_ST_GPIO->pin;
}

void st_gpio_reset(gpio_t* this)
{
  PTR_ST_GPIO_PORT->ODR &= ~(PTR_ST_GPIO->pin);
}

uint16_t st_gpio_read_output(gpio_t* this)
{
  return PTR_ST_GPIO_PORT->ODR & PTR_ST_GPIO->pin;
}

uint16_t st_gpio_read_input(gpio_t* this)
{
  return (PTR_ST_GPIO_PORT->IDR & PTR_ST_GPIO->pin);  
}
void st_gpio_enable_isr(gpio_t* this)
{
	EXTI_IMR_BIT(((gpio_t*)this)->pin_num)=1;
}

void st_gpio_disable_isr(gpio_t* this)
{
	EXTI_IMR_BIT(((gpio_t*)this)->pin_num)=0;
}

void st_gpio_set_isr_trigger(gpio_t* this,exti_rigger_t trigger)
{
	if(trigger==TRIGGER_BOTH)
	{
		EXTI_RTSR_BIT(((gpio_t*)this)->pin_num)=1;
		EXTI_FTSR_BIT(((gpio_t*)this)->pin_num)=1;
	}
	else if(trigger==TRIGGER_RISING)
	{
		EXTI_RTSR_BIT(((gpio_t*)this)->pin_num)=1;
		EXTI_FTSR_BIT(((gpio_t*)this)->pin_num)=0;
	}
	else if(trigger==TRIGGER_FALLING)
	{
		EXTI_RTSR_BIT(((gpio_t*)this)->pin_num)=0;
		EXTI_FTSR_BIT(((gpio_t*)this)->pin_num)=1;
	}
	else
	{
		EXTI_RTSR_BIT(((gpio_t*)this)->pin_num)=0;
		EXTI_FTSR_BIT(((gpio_t*)this)->pin_num)=0;
	}
}
