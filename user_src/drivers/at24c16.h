#ifndef __DRV_AT24C16_H
#define __DRV_AT24C16_H
#include "platform/i2c.h"

#define AT24C16_CHIP_ID	0xA0
#define AT24C16_MAX_ADDRESS	2048
struct at24c16
{
	struct i2c_adapter *i2c;
	int8u address;
};

#define DECLARE_AT24C16(name,i2c,address) struct at24c16 at24c16_##name={&i2c,address}

BOOL at24c16_read(struct at24c16 *at24,int16u address,void* pBuffer,int16u len);
BOOL at24c16_write(struct at24c16 *at24,int16u address,void* pBuffer,int16u len);
BOOL at24c16_check(struct at24c16 *fm24,int16u address,void* pBuffer,int16u len);

#endif

