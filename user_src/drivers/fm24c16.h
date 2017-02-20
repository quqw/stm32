#ifndef __DRV_AT24C16_H
#define __DRV_AT24C16_H
#include "platform/i2c.h"

#define FM24_MAX_ADDRESS	2048
struct fm24c16
{
	struct i2c_adapter *i2c;
	int8u address;
	int8u write_delay;
};

#define DECLARE_FM24C16(name,i2c,address) struct fm24c16 fm24_##name={&i2c,address,0}

BOOL fm24c16_read(struct fm24c16 *fm24,int16u address,void* pBuffer,int16u len);
BOOL fm24c16_write(struct fm24c16 *fm24,int16u address,void* pBuffer,int16u len);
BOOL fm24c16_check(struct fm24c16 *fm24,int16u address,void* pBuffer,int16u len);

#endif

