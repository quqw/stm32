#ifndef WIDGET_FORM_H001
#define WIDGET_FORM_H001

#include "gui/widget.h"

typedef struct widget_form
{
	struct widget base;//»ù±¾ÊôĞÔ
	const struct widget_handler *p_handlers;
	int8u row_count;
	int8u column_count;
}widget_form_t;

extern const struct widget_handler form_handler;




#endif

