#ifndef __DRV_BEEP_H
#define __DRV_BEEP_H
#include "platform/pwm.h"

#define BEEPROW_COUNT 10
struct beep{
	struct pwm *pwm;
	int16u row_freq[BEEPROW_COUNT];
	int16u row_tick[BEEPROW_COUNT];
	int16u row_step;
	int16u cur_tick;     //毫秒数,递减
	int16u cur_step;     //频率阶段,***递增处理***
	int16u repeat;     //重复次数,递减
};

#define DECLARE_BEEP(name,pwm) struct beep beep_##name={&pwm}


void beep_begin(struct beep *beep,int16u repeat);
void beep_stop(struct beep *beep);
BOOL beep_is_stop(struct beep *beep);
void beep_tick(struct beep *beep,int16u nTick);
void beep_init(struct beep *beep);

#endif
