#include "includes.h"
#include "platform/pwm.h"
#include "platform/gpio.h"

#define CLOCK_1M	1000000
void pwm_init(struct pwm *pwm)
{
	TIM_OCInitTypeDef TIM_OCStruct;
	TIM_OCStructInit(&TIM_OCStruct);
	timer_init(pwm->timer);

	
	TIM_OCStruct.TIM_OCMode = TIM_OCMode_PWM1;//配置为PWM1模式
	TIM_OCStruct.TIM_OutputState = TIM_OutputState_Enable; 
	TIM_OCStruct.TIM_OutputNState = TIM_OutputNState_Disable;
	TIM_OCStruct.TIM_Pulse = 0xff;
	TIM_OCStruct.TIM_OCPolarity = TIM_OCPolarity_High;
  TIM_OCStruct.TIM_OCNPolarity = TIM_OCNPolarity_Low;
  TIM_OCStruct.TIM_OCIdleState = TIM_OCIdleState_Set;
  TIM_OCStruct.TIM_OCNIdleState = TIM_OCIdleState_Reset;
	
	
	pwm->oc_init(pwm->timer->tim,&TIM_OCStruct);	//	
	timer_set_prescaler(pwm->timer,SystemCoreClock/CLOCK_1M-1);//默认1M
	
	if(pwm->timer->tim==TIM1 || pwm->timer->tim==TIM8)
		TIM_CtrlPWMOutputs(pwm->timer->tim, ENABLE);
	gpio_init(pwm->pin,GPIO_Mode_AF_PP);
	gpio_reset(pwm->pin);
	
}


void pwm_stop(struct pwm *pwm)
{
//	switch(pwm->oc)
//	{
//	case 1:
//	    pwm->timer->tim->CCMR1 &= 0xFF8F;//冻结输出比较模式
//			if(high)
//				pwm->timer->tim->CCMR1 |= 0x0050;//强制拉高
//			else
//				pwm->timer->tim->CCMR1 |= 0x0040;//强制拉高
//		break;
//	case 2:
//		pwm->timer->tim->CCMR1 &= 0x8FFF;//冻结输出比较模式
//	    pwm->timer->tim->CCMR1 |= 0x0050;//强制拉高
//		break;
//	case 3:
//	    pwm->timer->tim->CCMR2 &= 0xFF8F;//冻结输出比较模式
//	    pwm->timer->tim->CCMR2 |= 0x0050;//强制拉高
//		break;
//	case 4:
//		pwm->timer->tim->CCMR2 &= 0xFF8F;//冻结输出比较模式
//	    pwm->timer->tim->CCMR2 |= 0x0050;//强制拉高
//		break;
//	}
//	//pwm->timer->tim->CR1 |=0x0080;//控制寄存器1中ARPE位使能预装载寄存器，TIM4_ARR被装入缓冲区
//	pwm->timer->tim->EGR = 0x0001;//更新
	TIM_Cmd(pwm->timer->tim, DISABLE);
}

void pwm_start(struct pwm *pwm)
{
	pwm->timer->tim->EGR = 0x0001;//更新
	timer_clear_interrupt_flag(pwm->timer);
	TIM_Cmd(pwm->timer->tim, ENABLE);
}


//#define TIM_OCMode_Timing                  ((uint16_t)0x0000)
//#define TIM_OCMode_Active                  ((uint16_t)0x0010)
//#define TIM_OCMode_Inactive                ((uint16_t)0x0020)
//#define TIM_OCMode_Toggle                  ((uint16_t)0x0030)
//#define TIM_OCMode_PWM1                    ((uint16_t)0x0060)
//#define TIM_OCMode_PWM2                    ((uint16_t)0x0070)
//oc_prelod:预装载CCR,TRUE表示下次更新生效。FALSE表示立即生效
void pwm_set_oc_mode(struct pwm *pwm,int16u pwm_mode,BOOL oc_prelod )
{
	switch(pwm->oc)
	{
	case 1:
    	pwm->timer->tim->CCMR1 &= 0xFF87;
    	pwm->timer->tim->CCMR1 |=  pwm_mode;
		if(oc_prelod)
			pwm->timer->tim->CCMR1 |=0x08;
		else
			pwm->timer->tim->CCMR1 &=~0x08;
		break;
	case 2:
	    pwm->timer->tim->CCMR1 &= 0x87FF;
    	pwm->timer->tim->CCMR1 |=  pwm_mode<<8;
		if(oc_prelod)
			pwm->timer->tim->CCMR1 |=0x800;
		else
			pwm->timer->tim->CCMR1 &=~0x800;
		break;
	case 3:
    	pwm->timer->tim->CCMR2 &= 0xFF87;
    	pwm->timer->tim->CCMR2 |=  pwm_mode;
		if(oc_prelod)
			pwm->timer->tim->CCMR2 |=0x08;
		else
			pwm->timer->tim->CCMR2 &=~0x08;
		break;
	case 4:
	    pwm->timer->tim->CCMR2 &= 0x87FF;
    	pwm->timer->tim->CCMR2 |=  pwm_mode<<8;
		if(oc_prelod)
			pwm->timer->tim->CCMR2 |=0x800;
		else
			pwm->timer->tim->CCMR2 &=~0x800;
		break;
	}	
	//pwm->timer->tim->EGR = 0x0001;//更新
}
//balance:第一个电平所占比例。%50=》50，%25=》25
void pwm_set_frequence(struct pwm *pwm,int8u balance,int16u hz)
{  
	 int32u arr;
    if(hz==0){
        pwm_stop(pwm);
        return;
    }
		arr=SystemCoreClock/(timer_get_prescaler(pwm->timer)+1)/hz;
		if(arr==0)
			pwm->timer->tim->ARR = 0;
		else
			pwm->timer->tim->ARR = arr-1;
    *(int32u*)((int32u)(pwm->timer->tim)+0x34+((pwm->oc-1)*4)) = arr*balance/100; // 通道1 捕获/比较寄存器,50%占空比
    pwm->timer->tim->CNT = 0;//计数器清零
    //pwm->timer->tim->EGR = 0x0001;//初始化计数器，产生一个更新事件加载arr
}

void pwm_set_oc(struct pwm *pwm,int16u oc_val)
{
	*(int32u*)((int32u)(pwm->timer->tim)+0x34+((pwm->oc-1)*4)) =oc_val;
	pwm->timer->tim->CNT = 0;//计数器清零
  //pwm->timer->tim->EGR = 0x0001;//初始化计数器，产生一个更新事件加载arr
}

int16u pwm_get_oc(struct pwm *pwm)
{
	return *(int32u*)((int32u)(pwm->timer->tim)+0x34+((pwm->oc-1)*4));
}

void pwm_set_balance(struct pwm *pwm,int8u balance)
{
		int32u arr=timer_get_auto_reload(pwm->timer);
		pwm_set_oc(pwm,arr*balance/100);
}
void pwm_set_repeat(struct pwm *pwm,int16u repeat)
{
	if(pwm->timer->tim==TIM1 || pwm->timer->tim==TIM8)
	{
		pwm->timer->tim->RCR=repeat&0xff;
	}
}

