#ifndef __PLATFORM_PWM_H
#define __PLATFORM_PWM_H
#include "platform/timer.h"

struct pwm
{
	struct gpio *pin;
	struct timer *timer;
	int8u oc;
	void (*oc_init)(TIM_TypeDef* TIMx, TIM_OCInitTypeDef* TIM_OCInitStruct);
};

//tim->1,2,3,4...
//oc->1,2,3,4
//gpio_x->0x01,0x13...
#define DECLARE_PWM(name,tim,oc,gpio_x) DECLARE_GPIO(pwm_##name,gpio_x);\
										DECLARE_TIMER(pwm_##name,tim);\
										struct pwm pwm_##name={&gpio_pwm_##name,\
															&timer_pwm_##name,\
															oc,\
															TIM_OC##oc##Init};

void pwm_init(struct pwm *pwm);
void pwm_stop(struct pwm *pwm);
void pwm_start(struct pwm *pwm);
void pwm_set_oc_mode(struct pwm *pwm,int16u pwm_mode,BOOL oc_prelod );
void pwm_set_frequence(struct pwm *pwm,int8u balance,int16u hz);
void pwm_set_balance(struct pwm *pwm,int8u balance);
void pwm_set_repeat(struct pwm *pwm,int16u repeat);
void pwm_set_oc(struct pwm *pwm,int16u oc_val);
int16u pwm_get_oc(struct pwm *pwm);
#endif

