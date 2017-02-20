#include "types.h"
#include "drivers/keyboard/keyboard.h"
#include "widget_form.h"
/*
fromΪһ��������������԰���һ��������widget��Ҳ����û�С�
��widget��Ϊform�ڵ�Ԫ�أ���form��ʾ��ʱ��Ҳһ����ʾ
��õ�һ�����Ӿ���һ����Ϣ��Ϊһ��form��
*/
void form_on_key(struct widget* p_b,key_event_t* p_keyEvent);
void form_on_tick(struct widget* p_b,int32u ticks);//tick��Ӧ����
void form_on_event(struct widget* p_f,event_id_t event_id,void *p_event);
void form_on_init(struct widget* p_f,struct widget *p_parent);
void form_on_destroy(struct widget* p_f);
void form_on_show(struct widget* p_f);
struct widget* form_on_focus(struct widget* p_f);
const struct widget_handler form_handler={form_on_key,form_on_tick,form_on_event,
																					form_on_init,form_on_destroy,form_on_show,form_on_focus};

void form_on_key(struct widget* p_f,key_event_t* p_keyEvent)	
{
	if(((struct widget_form*)p_f)->p_handlers && ((struct widget_form*)p_f)->p_handlers->on_key)	
		 ((struct widget_form*)p_f)->p_handlers->on_key(p_f,p_keyEvent);
}
void form_on_tick(struct widget* p_f,int32u ticks)//tick��Ӧ����
{
	if(((struct widget_form*)p_f)->p_handlers && ((struct widget_form*)p_f)->p_handlers->on_tick)	
		 ((struct widget_form*)p_f)->p_handlers->on_tick(p_f,ticks);
}
void form_on_event(struct widget* p_f,event_id_t event_id,void *p_event)
{
	if(((struct widget_form*)p_f)->p_handlers && ((struct widget_form*)p_f)->p_handlers->on_event)	
		 ((struct widget_form*)p_f)->p_handlers->on_event(p_f,event_id,p_event);
}
void form_on_init(struct widget* p_f,struct widget *p_parent)
{
	
	p_f->attr.attribute&=~WIDGET_HAS_FOCUS;//ǿ���޽���
	/*
	����ӣ�����form����������������ʼ����widgetλ��
	�������룺��ƫ��Ϊ��0��0�����Զ���ʼ����widgetλ�ã�����ʹ��Ԥ��ֵ
	
	*/
	
	if(((struct widget_form*)p_f)->p_handlers && ((struct widget_form*)p_f)->p_handlers->on_init)	
		 ((struct widget_form*)p_f)->p_handlers->on_init(p_f,p_parent);
}

void form_on_destroy(struct widget* p_f)
{
}

void form_on_show(struct widget* p_f)//form����ֻ��һ���߿򣬵���ʾ��ʱ��Ҫ���ڲ�Ԫ��һ����ʾ
{

	if(((struct widget_form*)p_f)->p_handlers && ((struct widget_form*)p_f)->p_handlers->on_show)	
		 ((struct widget_form*)p_f)->p_handlers->on_show(p_f);
}


struct widget* form_on_focus(struct widget* p_f)
{
	if(((struct widget_form*)p_f)->p_handlers && ((struct widget_form*)p_f)->p_handlers->on_focus)	
		 return ((struct widget_form*)p_f)->p_handlers->on_focus(p_f);	
	else return NULL;
}






