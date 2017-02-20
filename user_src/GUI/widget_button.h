#ifndef WIDGET_BUTTON_H001
#define WIDGET_BUTTON_H001
#include "gui/widget.h"


typedef struct widget_button
{
	struct widget base;
	const struct widget_handler *p_handlers;//不同的button的特有动作
	const wchar* text;
}widget_button_t;

extern const struct widget_handler button_handler;








#endif


