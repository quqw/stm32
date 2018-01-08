
#include "cmd.h"
#include "usb/usb_func.h"
BOOL run_mkc_cmd(struct cmd_packet *cmd);

#define CLA_COMMON		0
#define INS_DEVICE_ID	0

BOOL run_cmd(struct cmd_packet *cmd)
{
	switch(cmd->cla)
	{
		case CLA_COMMON:
			*(int32u*)cmd->data=0x12345678;
			cmd->len=4;
			return TRUE;
		case CLA_MOUSE_KEYBOARD_CTL:
			return run_mkc_cmd(cmd);
		default:
			cmd->flag|=CMD_ERROR_FLAG;
			*(int32u*)cmd->data=ERR_NOT_SUPPORT;
			cmd->len=4;
			return TRUE;
	}
}


BOOL run_mkc_cmd(struct cmd_packet *cmd)//
{
	switch(cmd->ins)
	{
		case INS_MOUSE:
			if(cmd->len!=4)
			{
				cmd->len=4;
				cmd->flag|=CMD_ERROR_FLAG;
				*(int32u*)(cmd->data)=ERR_LENGTH;
				return TRUE;
			}
			else if(!hid_send_mouse(cmd->data[0],cmd->data[1],cmd->data[2]))
			{
				cmd->len=4;
				cmd->flag|=CMD_ERROR_FLAG;
				*(int32u*)(cmd->data)=ERR_HW_BUSY;
				return TRUE;
			}
			break;
		case INS_KEYBOARD:
			if(cmd->len!=8)
			{
				cmd->len=4;
				cmd->flag|=CMD_ERROR_FLAG;
				*(int32u*)(cmd->data)=ERR_LENGTH;
				return TRUE;
			}
			else if(!hid_send_keyboard(cmd->data))
			{
				cmd->len=4;
				cmd->flag|=CMD_ERROR_FLAG;
				*(int32u*)(cmd->data)=ERR_HW_BUSY;
				return TRUE;
			}
			break;
	}
	cmd->len=0;
	return TRUE;
}

