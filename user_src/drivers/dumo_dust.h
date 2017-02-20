#ifndef DUMO_DUST_H
#define DUMO_DUST_H
#include "modbus_master.h"
//raw data
#define MB_REG_DUSTRUNNINGAVERAGE_HIGH	0x0006
#define MB_REG_DUSTRUNNINGAVERAGE_LOW		0x0007
//after calbration
#define MB_REG_DUSTCALIBRATED_HIGH 			0x0008
#define MB_REG_DUSTCALIBRATED_LOW 			0x0009

typedef struct dumo
{
	struct modbus_master *port;
	int8u	address;
	int8u reserved[3];
}dumo_t;

#define DECLARE_DUMO_DUST(x,modbus,address)	 struct dumo dumo_##x={&modbus,address}

BOOL dumo_read_calbration_data(struct dumo *d,int32s* out);
BOOL dumo_read_raw(struct dumo *d,int32s* out);
#endif

