#include "includes.h"
#include "drivers/beep.h"
#include "utilities/tone.h"

//Ó¦ÓÃ²ãAPI
void tone_warn(struct beep *beep,int16u freq,int16u count,int16u _on_ms,int16u _off_ms)
{
	__disable_irq();
    beep->row_freq[0]=freq;
    beep->row_tick[0]=_on_ms;
    beep->row_freq[1]=0;
    beep->row_tick[1]=_off_ms;
    beep->row_step=2;
    beep_begin(beep,count);
	__enable_irq();
}

void tone_poweron(struct beep *beep,int16u count){   
	__disable_irq();
    beep->row_freq[0]=FREQ_L4;
    beep->row_tick[0]=80;
    beep->row_freq[1]=FREQ_L5;
    beep->row_tick[1]=150;
    beep->row_freq[2]=FREQ_M2;
    beep->row_tick[2]=80;
    beep->row_freq[3]=FREQ_M5;
    beep->row_tick[3]=100;
    beep->row_step=4;
    beep_begin(beep,count);
	__enable_irq();
}

void tone_ok(struct beep *beep,int16u count){
    __disable_irq();
    beep->row_freq[0]=FREQ_M4;
    beep->row_tick[0]=150;
    beep->row_freq[1]=FREQ_M3;
    beep->row_tick[1]=150;
    beep->row_freq[2]=FREQ_L4;
    beep->row_tick[2]=80;
    beep->row_freq[3]=FREQ_L3;
    beep->row_tick[3]=100;
    beep->row_step=4;
    beep_begin(beep,count);
	__enable_irq();

}

void tone_stop(struct beep *beep){
	__disable_irq();
    beep_stop(beep);
	__enable_irq();
}








