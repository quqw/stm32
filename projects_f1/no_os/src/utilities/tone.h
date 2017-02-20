#ifndef __UTILITY_TONE_H
#define __UTILITY_TONE_H

#define FREQ_L1 262
#define FREQ_L2 294
#define FREQ_L3 330
#define FREQ_L4 349
#define FREQ_L5 392
#define FREQ_L6 440
#define FREQ_L7 494

#define FREQ_M1 523
#define FREQ_M2 578
#define FREQ_M3 659
#define FREQ_M4 698
#define FREQ_M5 784
#define FREQ_M6 880
#define FREQ_M7 988


#define FREQ_H1 1046
#define FREQ_H2 1175
#define FREQ_H3 1318
#define FREQ_H4 1397
#define FREQ_H5 1568
#define FREQ_H6 1760
#define FREQ_H7 1976


void tone_warn(struct beep *beep,int16u freq,int16u count,int16u _on_ms,int16u _off_ms);
void tone_poweron(struct beep *beep,int16u count);
void tone_ok(struct beep *beep,int16u count);
void tone_stop(struct beep *beep);

#endif //__UTILITY_TONE_H

