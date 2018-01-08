#ifndef CMD_H
#define CMD_H
#include "common_types.h"
#include "communication/comm_types.h"
#define MAX_CMD_DATA_LEN    (MAX_PACKET_SIZE-8)

#define OK      0
#define ERR_PC_TYPE		0x00000
#define ERR_CMD_TYPE	0x10000
#define ERR_HW_TYPE		0x20000

#define ERR_PARAMS    (ERR_CMD_TYPE+1)
#define ERR_LENGTH    (ERR_CMD_TYPE+2)
#define ERR_NOT_SUPPORT (ERR_CMD_TYPE+3)
#define ERR_CMD_REPEAT (ERR_CMD_TYPE+4)

#define ERR_HW_BUSY (ERR_HW_TYPE+1)

#define CLA_COMMON		0
#define INS_DEVICE_ID	0


#define CLA_MOUSE_KEYBOARD_CTL 1
#define INS_MOUSE       0
#define INS_KEYBOARD    1

BOOL run_cmd(struct cmd_packet *cmd);
BOOL run_fly_cmd(struct cmd_packet *cmd);

#endif
