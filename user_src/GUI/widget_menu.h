#ifndef WIDGET_MENU_H001
#define WIDGET_MENU_H001

#include "gui/widget_form.h"
#include "gui/widget_button.h"
typedef struct _WIDGET_MENU
{
	WIDGET base;//��������
	struct _WIDGET *p_firstChild;//ָ���һ����widget����widget��ָ�����ڴ�widget�ڵ�widget����Ҫͬʱ��ʾ�ڽ��档�������һ�������Ļ����Ӧ����0
	WIDGET_HANDLER *p_handlers;
}WIDGET_MENU;

