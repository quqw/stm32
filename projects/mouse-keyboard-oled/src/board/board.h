#ifndef __PORTABLE_BOARD_H
#define __PORTABLE_BOARD_H
#include "common_types.h"
#define LENGTH_PER_GAP	10//cm

#define KEY_NONE		0
#define KEY_UP			1
#define KEY_DOWN		2
#define KEY_LEFT		3
#define KEY_RIGHT		4
#define KEY_ENTER	5

void board_init(void);
int8u board_key_get(void);
void board_led1_toggle(void);
void board_led1_on(void);
void board_led1_off(void);
#endif

