#ifndef TIMER_H
#define TIMER_H
#include "common_types.h"
#include "stm32f10x_tim.h"

typedef struct st_tim
{
	TIM_TypeDef *port;
	uint16_t channel;//??1-4
	volatile uint16_t* ccr_reg;
}st_tim_t;

#define DELCARE_ST_TIM(name,port,ch)	st_tim_t st_tim_##name={port,ch}


void st_tim_base_init(st_tim_t *this,TIM_TimeBaseInitTypeDef *init);
void st_tim_ic_init(st_tim_t *this,TIM_ICInitTypeDef *init);
void st_tim_oc_init(st_tim_t *this,TIM_OCInitTypeDef *init);

void st_tim_enable(st_tim_t* this);
void st_tim_disable(st_tim_t* this);
void st_tim_update_irq_enable(st_tim_t *this);
void st_tim_update_irq_disable(st_tim_t *this);
void st_tim_ic_irq_enable(st_tim_t *this);
void st_tim_ic_irq_disable(st_tim_t *this); 
#endif
