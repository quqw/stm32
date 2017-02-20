#include "dumo_dust.h"

BOOL dumo_read_raw(struct dumo *d,int32s* out)
{
	int16u value[2];
	if(MB_EX_NONE!=modbus_master_read_registers_rtu(d->port,d->address,MB_REG_DUSTRUNNINGAVERAGE_HIGH,2,value))
		return FALSE;
	*out=((int32u)value[0]<<16)+value[1];
	return TRUE;
}

BOOL dumo_read_calbration_data(struct dumo *d,int32s* out)
{
	int16u value[2];
	if(MB_EX_NONE!=modbus_master_read_registers_rtu(d->port,d->address,MB_REG_DUSTCALIBRATED_HIGH,2,value))
		return FALSE;
	*out=((int32u)value[0]<<16)+value[1];
	return TRUE;
}

