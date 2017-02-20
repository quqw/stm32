#include "includes.h"
#include "platform/pwm.h"
#include "platform/gpio.h"

#define CLOCK_1M	1000000
void pwm_init(struct pwm *pwm)
{
	TIM_OCInitTypeDef TIM_OCStruct;
	TIM_OCStructInit(&TIM_OCStruct);
	timer_init(pwm->timer);

	
	TIM_OCStruct.TIM_OCMode = TIM_OCMode_PWM1;//����ΪPWM1ģʽ
	TIM_OCStruct.TIM_OutputState = TIM_OutputState_Enable; 
	TIM_OCStruct.TIM_OutputNState = TIM_OutputNState_Disable;
	TIM_OCStruct.TIM_Pulse = 0xff;
	TIM_OCStruct.TIM_OCPolarity = TIM_OCPolarity_High;
  TIM_OCStruct.TIM_OCNPolarity = TIM_OCNPolarity_Low;
  TIM_OCStruct.TIM_OCIdleState = TIM_OCIdleState_Set;
  TIM_OCStruct.TIM_OCNIdleState = TIM_OCIdleState_Reset;
	
	
	pwm->oc_init(pwm->timer->tim,&TIM_OCStruct);	//	
	timer_set_prescaler(pwm->timer,SystemCoreClock/CLOCK_1M-1);//Ĭ��1M
	
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
//	    pwm->timer->tim->CCMR1 &= 0xFF8F;//��������Ƚ�ģʽ
//			if(high)
//				pwm->timer->tim->CCMR1 |= 0x0050;//ǿ������
//			else
//				pwm->timer->tim->CCMR1 |= 0x0040;//ǿ������
//		break;
//	case 2:
//		pwm->timer->tim->CCMR1 &= 0x8FFF;//��������Ƚ�ģʽ
//	    pwm->timer->tim->CCMR1 |= 0x0050;//ǿ������
//		break;
//	case 3:
//	    pwm->timer->tim->CCMR2 &= 0xFF8F;//��������Ƚ�ģʽ
//	    pwm->timer->tim->CCMR2 |= 0x0050;//ǿ������
//		break;
//	case 4:
//		pwm->timer->tim->CCMR2 &= 0xFF8F;//��������Ƚ�ģʽ
//	    pwm->timer->tim->CCMR2 |= 0x0050;//ǿ������
//		break;
//	}
//	//pwm->timer->tim->CR1 |=0x0080;//���ƼĴ���1��ARPEλʹ��Ԥװ�ؼĴ�����TIM4_ARR��װ�뻺����
//	pwm->timer->tim->EGR = 0x0001;//����
	TIM_Cmd(pwm->timer->tim, DISABLE);
}

void pwm_start(struct pwm *pwm)
{
	pwm->timer->tim->EGR = 0x0001;//����
	timer_clear_interrupt_flag(pwm->timer);
	TIM_Cmd(pwm->timer->tim, ENABLE);
}


//#define TIM_OCMode_Timing                  ((uint16_t)0x0000)
//#define TIM_OCMode_Active                  ((uint16_t)0x0010)
//#define TIM_OCMode_Inactive                ((uint16_t)0x0020)
//#define TIM_OCMode_Toggle                  ((uint16_t)0x0030)
//#define TIM_OCMode_PWM1                    ((uint16_t)0x0060)
//#define TIM_OCMode_PWM2                    ((uint16_t)0x0070)
//oc_prelod:Ԥװ��CCR,TRUE��ʾ�´θ�����Ч��FALSE��ʾ������Ч
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
	//pwm->timer->tim->EGR = 0x0001;//����
}
//balance:��һ����ƽ��ռ������%50=��50��%25=��25
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
    *(int32u*)((int32u)(pwm->timer->tim)+0x34+((pwm->oc-1)*4)) = arr*balance/100; // ͨ��1 ����/�ȽϼĴ���,50%ռ�ձ�
    pwm->timer->tim->CNT = 0;//����������
    //pwm->timer->tim->EGR = 0x0001;//��ʼ��������������һ�������¼�����arr
}

void pwm_set_oc(struct pwm *pwm,int16u oc_val)
{
	*(int32u*)((int32u)(pwm->timer->tim)+0x34+((pwm->oc-1)*4)) =oc_val;
	pwm->timer->tim->CNT = 0;//����������
  //pwm->timer->tim->EGR = 0x0001;//��ʼ��������������һ�������¼�����arr
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

