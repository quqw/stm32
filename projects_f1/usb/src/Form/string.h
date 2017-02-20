#ifndef STRING_H
#define STRING_H

#include "types.h"

#define STR_OK	0
#define STR_CANCEL	3
#define STR_FREQ	6
#define STR_TIME	9
#define STR_DOOR_OPEN	12
#define STR_FREQ_LOW	15
#define STR_FREQ_NORMAL	18
#define STR_FREQ_HIGH	21
#define STR_STATUS	24
#define	STR_TIME_GONE	27
#define	STR_SPEED_STEP1		30
#define	STR_SPEED_STEP2	  34
#define	STR_SPEED_STEP3		38
#define STR_TIME_TOTAL	42//总时
#define STR_IDLE		45//
#define STR_ERR	48//	/*异常*/
#define STR_COMPLETE	51 /*完成*/
#define STR_STOP	54/*停止*/
#define STR_PHASE	57/*阶段*/
#define STR_SYSTEM_START 60///*系统启动中*/
#define STR_SYS_NAME 66//电机控制系统
extern const wchar str_table[];
extern const wchar hz_table[];
#define GET_WSTR(x)	(&str_table[x])
#endif


