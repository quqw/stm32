#ifndef KEY_BOARD_H001
#define KEY_BOARD_H001

#include "types.h"
#include "board/board.h"
typedef int32u button_value_t;
typedef int32u button_modify_t;

#define BUTTON_NULL 0
#define BUTTON_DOWN		(1<<4)
#define BUTTON_UP			(1<<5)
#define BUTTON_LEFT		(1<<3)
#define BUTTON_RIGHT		(1<<2)
#define BUTTON_ENTER		(1<<1)
#define BUTTON_ESC	(1)
	
typedef enum
{
    KEY_RELEASE=0,
    KEY_PRESS
}key_action_t;

typedef struct key_event
{
    button_value_t    key_value;
		button_modify_t		modify_val;
    key_action_t  		key_action;
}key_event_t;

void key_board_tick(int16u tick);
int32u key_board_get(int32u *pTimer);

#endif


