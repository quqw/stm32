#ifndef WIDGET_MENU_H001
#define WIDGET_MENU_H001

#include "gui/widget_form.h"
#include "gui/widget_button.h"
typedef struct _WIDGET_MENU
{
	WIDGET base;//基本属性
	struct _WIDGET *p_firstChild;//指向第一个子widget，子widget是指包含在此widget内的widget，需要同时显示在界面。如果不是一个容器的话这个应该是0
	WIDGET_HANDLER *p_handlers;
}WIDGET_MENU;

