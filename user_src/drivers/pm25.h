#ifndef PM25_H
#define PM25_H

#include "platform/uart.h"
#define PM25_BUF_LENGTH	16
#define PM25_TIMEOUT		10000 //ms
typedef struct pm25
{
	struct uart *uart;
	int32s value;
	int16u length;
	int16u time_out;
	int8u buf[PM25_BUF_LENGTH];
	
}pm25_t;


#define DECLARE_PM25(name,uart) struct pm25 pm25_##name={&uart,0,0,PM25_TIMEOUT}
void pm25_init(struct pm25* pm25);
void pm25_tick(struct pm25* pm25,int ticks);
int32s pm25_read(struct pm25* pm25);
#endif

