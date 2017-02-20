#include "types.h"
#include "drivers/keyboard/keyboard.h"
#include "widget_form.h"
/*
from为一个容器，里面可以包含一个或多个子widget，也可以没有。
子widget作为form内的元素，在form显示的时候也一并显示
最常用的一个例子就是一屏信息作为一个form。
*/
void form_on_key(struct widget* p_b,key_event_t* p_keyEvent);
void form_on_tick(struct widget* p_b,int32u ticks);//tick响应函数
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
void form_on_tick(struct widget* p_f,int32u ticks)//tick响应函数
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
	
	p_f->attr.attribute&=~WIDGET_HAS_FOCUS;//强制无焦点
	/*
	需添加：根据form的行列数量继续初始化子widget位置
	初步设想：若偏移为（0，0）则自动初始化子widget位置，否则使用预设值
	
	*/
	
	if(((struct widget_form*)p_f)->p_handlers && ((struct widget_form*)p_f)->p_handlers->on_init)	
		 ((struct widget_form*)p_f)->p_handlers->on_init(p_f,p_parent);
}

void form_on_destroy(struct widget* p_f)
{
}

void form_on_show(struct widget* p_f)//form本身只有一个边框，但显示的时候要把内部元素一起显示
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






