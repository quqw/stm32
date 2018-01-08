#ifndef STORAGE_H
#define STORAGE_H
#include "types.h"
#include "motor-control.h"

#define SYSTEM_PARAMS_START_ADDRESS	0
#define WORK_PARAMS_START_ADDRESS	32//32B
#define TIME_PARAMS_START_ADDRESS	96//128B
#define INIT_WORK_STATE_ADDRESS		480

#define CURRENT_WORK_START_ADDRESS		1024

typedef struct system_params
{
	int8u init_code[4];
	int8u reserved[2];
	int16u crc16;
}system_params_t;

BOOL store_system_params(struct system_params* params);
BOOL restore_system_params(struct system_params* params);
BOOL store_work_params(struct work_params* params);
BOOL restore_work_params(struct work_params* params);
BOOL store_time_params(struct time_params* params);
BOOL restore_time_params(struct time_params* params);
BOOL restore_work_state(struct work_state* work);
BOOL store_work_state(struct work_state* work);



#endif

