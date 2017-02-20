#include "includes.h"
#include "st_timer.h"


#define THIS_STM_TIMER	((st_tim_t*)this)

void st_tim_base_init(st_tim_t *this,TIM_TimeBaseInitTypeDef *init)
{
	TIM_TimeBaseInit(this->port, init);
}
void st_tim_ic_init(st_tim_t *this,TIM_ICInitTypeDef *init)
{
	TIM_ICInit(this->port, init);
	switch(this->channel)
	{
		case 1:
			this->ccr_reg=&(this->port->CCR1);
			break;
		case 2:
			this->ccr_reg=&(this->port->CCR2);
			break;
		case 3:
			this->ccr_reg=&(this->port->CCR3);
			break;
		case 4:
			this->ccr_reg=&(this->port->CCR4);
			break;
	}
}
void st_tim_oc_init(st_tim_t *this,TIM_OCInitTypeDef *init)
{
	
}
void st_tim_ic_irq_enable(st_tim_t *this)
{
	this->port->DIER |= (1<<this->channel);
}
void st_tim_ic_irq_disable(st_tim_t *this)
{
	this->port->DIER &= (uint16_t)~(1<<this->channel);
}

void st_tim_update_irq_enable(st_tim_t *this)
{
	this->port->DIER |= TIM_IT_Update;
}
void st_tim_update_irq_disable(st_tim_t *this)
{
	THIS_STM_TIMER->port->DIER &= ~TIM_IT_Update;
}

void st_tim_enable(st_tim_t *this)
{
	THIS_STM_TIMER->port->CR1 |= TIM_CR1_CEN;
	
}
void st_tim_disable(st_tim_t *this)
{
	THIS_STM_TIMER->port->CR1 &= ~TIM_CR1_CEN;
}

//取样频率设置
//#define TIM_CKD_DIV1                       ((uint16_t)0x0000)
//#define TIM_CKD_DIV2                       ((uint16_t)0x0100)
//#define TIM_CKD_DIV4                       ((uint16_t)0x0200)
void st_tim_set_clock_division(st_tim_t *this,int32u division)
{
	this->port->CR1 &=0xff;
	this->port->CR1|=division;
}

//#define TIM_PSCReloadMode_Update           ((uint16_t)0x0000)
//#define TIM_PSCReloadMode_Immediate        ((uint16_t)0x0001)
void timer_set_prescaler(st_tim_t *this,int32u prescaler)
{
	this->port->PSC = prescaler;
}

int32u timer_get_prescaler(st_tim_t *this)
{
	return this->port->PSC;
}

void timer_set_auto_reload(st_tim_t *this,int32u arr)
{
	this->port->ARR = arr;
}

int32u timer_get_auto_reload(st_tim_t *this)
{
	return this->port->ARR ;
}
//state false：设置arr后直接生效 true：不直接生效，下一次更新事件到了才生效
void timer_auto_reload_preload(st_tim_t *this,BOOL state)
{
	if(state)
		this->port->CR1 |= TIM_CR1_ARPE;
	else
		this->port->CR1 &= ~TIM_CR1_ARPE;
}
void timer_set_counter(st_tim_t *this,int32u count)
{
	this->port->CNT = count;
}

int32u timer_get_counter(st_tim_t *this)
{
	return this->port->CNT;
}


//  *     @arg TIM_CounterMode_Up: TIM Up Counting Mode
//  *     @arg TIM_CounterMode_Down: TIM Down Counting Mode
//  *     @arg TIM_CounterMode_CenterAligned1: TIM Center Aligned Mode1
//  *     @arg TIM_CounterMode_CenterAligned2: TIM Center Aligned Mode2
//  *     @arg TIM_CounterMode_CenterAligned3: TIM Center Aligned Mode3
void timer_set_counter_mode(st_tim_t *this,int16u count_mode)
{
	TIM_CounterModeConfig(this->port,count_mode);
}

void timer_stop(st_tim_t *this)
{
	this->port->CR1 &= ~TIM_CR1_CEN;
}
void timer_start(st_tim_t *this)
{
	this->port->EGR = TIM_FLAG_Update;	//手动触发一次更新事件,使所有寄存器生效
	this->port->CR1 |= TIM_CR1_CEN;
}
void timer_update(st_tim_t *this)
{
	this->port->EGR = TIM_FLAG_Update;	//手动触发一次更新事件,使所有寄存器生效
}
void timer_init_interrupt(st_tim_t *this,int8u priority,int8u sub_priority)
{
	NVIC_InitTypeDef   nvic_init;
	if(this->port==TIM1)
    	nvic_init.NVIC_IRQChannel                   = TIM1_UP_IRQn;
	else if(this->port==TIM2)
		nvic_init.NVIC_IRQChannel                   =TIM2_IRQn;
	else if(this->port==TIM3)
		nvic_init.NVIC_IRQChannel                   =TIM3_IRQn;	
	else if(this->port==TIM4)
		nvic_init.NVIC_IRQChannel                   =TIM4_IRQn;			
    nvic_init.NVIC_IRQChannelPreemptionPriority = priority;
    nvic_init.NVIC_IRQChannelSubPriority        = sub_priority;
    nvic_init.NVIC_IRQChannelCmd                = ENABLE;
    NVIC_Init(&nvic_init);
	//timer_init(timer);
}

//#define TIM_IT_Update                      ((uint16_t)0x0001)
//#define TIM_IT_CC1                         ((uint16_t)0x0002)
//#define TIM_IT_CC2                         ((uint16_t)0x0004)
//#define TIM_IT_CC3                         ((uint16_t)0x0008)
//#define TIM_IT_CC4                         ((uint16_t)0x0010)
//#define TIM_IT_COM                         ((uint16_t)0x0020)
//#define TIM_IT_Trigger                     ((uint16_t)0x0040)
//#define TIM_IT_Break                       ((uint16_t)0x0080)
void timer_enable_interrupt(st_tim_t *this,int16u it)
{
	this->port->SR=0;					//状态清0
	this->port->DIER |= it; 	//计数器溢出中断使能
}
		
void timer_disable_interrupt(st_tim_t *this,int16u it)
{
	this->port->DIER &=~it;
	this->port->SR =0;					//状态清0
}

void timer_clear_interrupt_flag(st_tim_t *this)
{
	this->port->SR=0;					//状态清0
}


