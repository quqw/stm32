#ifndef __DRV_FM24CL64_H
#define __DRV_FM24CL64_H
#include "platform/i2c.h"

#define FM24_MAX_ADDRESS	8096
struct fm24cl64
{
	struct i2c_adapter *i2c;
	int8u address;
};

#define DECLARE_FM24CL64(name,i2c,address) struct fm24cl64 fm24_##name={&i2c,address}

BOOL fm24cl64_read(struct fm24cl64 *fm24,int16u address,void* pBuffer,int16u len);
BOOL fm24cl64_write(struct fm24cl64 *fm24,int16u address,void* pBuffer,int16u len);


#endif

