
#include "cmd.h"
#include "usb/usb_func.h"
#include "apps/storage.h"
#include "apps/motor-control.h"
extern struct time_params s_time_params;
extern struct work_params s_work_params;
extern struct work_state s_work_state;

#define CLA_COMMON		0
#define INS_DEVICE_ID	0
BOOL run_motor_cmd(struct cmd_packet *cmd);
BOOL run_cmd(struct cmd_packet *cmd)
{
	switch(cmd->cla)
	{
		case CLA_COMMON:
			*(int32u*)cmd->data=0x12345678;
			cmd->len=4;
			return TRUE;
		case CLA_MOTOR_CTL:
			return run_motor_cmd(cmd);
		default:
			cmd->flag|=CMD_ERROR_FLAG;
			*(int32u*)cmd->data=ERR_NOT_SUPPORT;
			cmd->len=4;
			return TRUE;
	}
}


BOOL set_motor_work_params(struct cmd_packet *cmd)
{

	return TRUE;
}



BOOL (*func_motor[])(struct cmd_packet *cmd)={
													set_motor_work_params,

												};
BOOL run_motor_cmd(struct cmd_packet *cmd)//
{
	if((cmd->ins & INS_MASK)>sizeof(func_motor))
	{
		cmd->flag|=CMD_ERROR_FLAG;
		cmd->len=4;
		*(int32u*)(cmd->data)=ERR_NOT_SUPPORT;
		return TRUE;
	}
	cmd->flag=0;
	return (func_motor[cmd->ins & INS_MASK])(cmd);

}

