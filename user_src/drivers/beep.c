#include "includes.h"
#include "drivers/beep.h"

void beep_begin(struct beep *beep,int16u repeat)
{
	  beep->repeat=repeat;
    beep->cur_step=0;
    beep->cur_tick=beep->row_tick[0];
    pwm_set_frequence(beep->pwm,50,beep->row_freq[0]);
}
void beep_stop(struct beep *beep)
{
	  beep->repeat=0;
    pwm_stop(beep->pwm);
}
bool beep_is_stop(struct beep *beep)
{
	return (bool)(beep->repeat==0);
}
void beep_tick(struct beep *beep,int16u tick)
{
	    if(beep->repeat==0){
        return;
    }
    if(beep->cur_tick < tick){   //Tickʱ����,�ƽ�cur_step
        if((beep->cur_step+1)==beep->row_step){//һ��ѭ�����,��ʼһ����ѭ��
            beep->cur_step=0;
            beep->repeat--;
            if(beep->repeat==0){//����ѭ��ȫ������
                beep->cur_tick=0;
                pwm_stop(beep->pwm);//ȫ������
            }else{
                pwm_set_frequence(beep->pwm,50,beep->row_freq[0]);
                beep->cur_tick=beep->row_tick[0];
            }
        }else{
            beep->cur_step++;
            pwm_set_frequence(beep->pwm,50,beep->row_freq[beep->cur_step]);
            beep->cur_tick=beep->row_tick[beep->cur_step];
        }
    }else{
        beep->cur_tick-=tick;
    }
}
void beep_init(struct beep *beep)
{
	pwm_init(beep->pwm);
	beep_stop(beep);
}

