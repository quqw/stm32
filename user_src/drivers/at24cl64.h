#ifndef __DRV_AT24CL64_H
#define __DRV_AT24CL64_H
#include "platform/i2c.h"

#define AT24C64_CHIP_ID	0xA0
#define AT24C64_MAX_ADDRESS	8096
typedef struct at24c64
{
	struct i2c_adapter *i2c;
	int8u address;
}at24c64_t;

#define DECLARE_AT24C64(name,i2c,address) struct at24c64 at24c64_##name={&i2c,address}

BOOL at24c64_read(struct at24c64 *at24,int16u address,void* pBuffer,int16u len);
BOOL at24c64_write(struct at24c64 *at24,int16u address,void* pBuffer,int16u len);
BOOL at24c64_check(struct at24c64 *fm24,int16u address,void* pBuffer,int16u len);

#endif

