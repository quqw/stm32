#include "storage.h"
#include "drivers/at24c16.h"
#include "motor-control.h"
#include "assist/assist.h"
extern struct at24c16 at24c16_1;
//index:参数序号

BOOL store_system_params(struct system_params* params)
{
	params->crc16=crc16_ccitt(params,sizeof(struct system_params)-2);
	if(!at24c16_write(&at24c16_1,SYSTEM_PARAMS_START_ADDRESS,params,sizeof(struct system_params)))
		return FALSE;
	return at24c16_check(&at24c16_1,SYSTEM_PARAMS_START_ADDRESS,params,sizeof(struct system_params));
}
BOOL restore_system_params(struct system_params* params)
{
	if(!at24c16_read(&at24c16_1,SYSTEM_PARAMS_START_ADDRESS,params,sizeof(struct system_params)))
		return FALSE;
	if(params->crc16!=crc16_ccitt(params,sizeof(struct system_params)-2))
		return FALSE;
	return TRUE;
}	

BOOL store_work_params(struct work_params* params)
{
	params->flag=0;
	params->crc16=crc16_ccitt(params,sizeof(struct work_params)-2);
	if(!at24c16_write(&at24c16_1,WORK_PARAMS_START_ADDRESS,params,sizeof(struct work_params)))
		return FALSE;
	return at24c16_check(&at24c16_1,WORK_PARAMS_START_ADDRESS,params,sizeof(struct work_params));
}

BOOL restore_work_params(struct work_params* params)
{
	if(!at24c16_read(&at24c16_1,WORK_PARAMS_START_ADDRESS,params,sizeof(struct work_params)))
		return FALSE;
	if(params->crc16!=crc16_ccitt(params,sizeof(struct work_params)-2))
		return FALSE;
	return TRUE;
}

BOOL store_time_params(struct time_params* params)
{
	params->flag=0;
	params->crc16=crc16_ccitt(params,sizeof(struct time_params)-2);
	if(!at24c16_write(&at24c16_1,TIME_PARAMS_START_ADDRESS,params,sizeof(struct time_params)))
		return FALSE;
	return at24c16_check(&at24c16_1,TIME_PARAMS_START_ADDRESS,params,sizeof(struct time_params));
}
BOOL restore_time_params(struct time_params* params)
{
	if(!at24c16_read(&at24c16_1,TIME_PARAMS_START_ADDRESS,params,sizeof(struct time_params)))
		return FALSE;
	if(params->crc16!=crc16_ccitt(params,sizeof(struct time_params)-2))
		return FALSE;
	return TRUE;
}


//这个状态只有当设备异常复位重启后才需要恢复
BOOL restore_work_state(struct work_state* work)
{
	return TRUE;
}

BOOL store_work_state(struct work_state* work)
{
	return TRUE;
}

