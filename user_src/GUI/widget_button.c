#include "types.h"
#include "drivers/keyboard/keyboard.h"
#include "widget_button.h"

#define BUTTON_SELECT_COLOR	LCD_COLOR_BLACK
void button_on_key(struct widget* p_b,struct key_event* p_keyEvent);
void button_on_tick(struct widget* p_b,int32u ticks);//tick响应函数
void button_on_event(struct widget* p_b,event_id_t event_id,void *p_event);
void button_on_init(struct widget* p_b,struct widget* p_parent);
void button_on_destroy(struct widget* p_b);
void button_on_show(struct widget* p_b);
struct widget* button_on_focus(struct widget* p_b);

/*button的基本动作，初始化给base.p_handlers*/
const struct widget_handler button_handler={button_on_key,button_on_tick,button_on_event,
																	button_on_init,button_on_destroy,button_on_show,button_on_focus};


/*先执行公共属性动作，然后再执行特殊动作*/
void button_on_key(struct widget* p_b,struct key_event* p_keyEvent)
{
	if(p_keyEvent->key_action==KEY_PRESS)
	{
		switch(p_keyEvent->modify_val)
		{
#ifdef WIDGET_PREV
			case WIDGET_BUTTON_PREV:
				if(!p_b->p_last)
					return;
				p_b->text_color=p_b->init_text_color;
				p_b->back_color=p_b->init_back_color;
				if(widget_focus_previous(p_b)==NULL)//如果有上一个可以获取焦点的widget，则获取焦点
					widget_focus(p_b);//若没有，则焦点还在自己，这种情况不需要重画自己
				else
					widget_show(p_b);//自己的样式改变，需要重画
			break;
#endif

#ifdef WIDGET_BUTTON_NEXT 				
			case WIDGET_BUTTON_NEXT:
				if(!p_b->p_next)
					return;
				p_b->text_color=p_b->init_text_color;
				p_b->back_color=p_b->init_back_color;
				if(NULL==widget_focus_next(p_b))
					widget_focus(p_b);
				else
					widget_show(p_b);
				break;
#endif
#ifdef		WIDGET_BUTTON_ENTER		
			case WIDGET_BUTTON_ENTER:
				//p_b->back_color=LCD_COLOR_BLACK;
				p_b->loc_x++;
				p_b->loc_y++;
				widget_refresh(p_b);
			break;
#endif			
		}
	}
	else if(p_keyEvent->key_action==KEY_RELEASE)
	{
		switch(p_keyEvent->modify_val)
		{
#ifdef	WIDGET_BUTTON_ENTER
			case WIDGET_BUTTON_ENTER:
//				p_b->back_color=LCD_COLOR_RESERVED(p_b->back_color);
//				p_b->text_color=LCD_COLOR_RESERVED(p_b->text_color);
//				p_b->back_color=BUTTON_SELECT_COLOR;
				p_b->loc_x--;
				p_b->loc_y--;
				widget_refresh(p_b);
				break;
#endif
		}
	}
	if(((struct widget_button*)p_b)->p_handlers && ((struct widget_button*)p_b)->p_handlers->on_key)
		((struct widget_button*)p_b)->p_handlers->on_key(p_b,p_keyEvent);
}
void button_on_tick(struct widget* p_b,int32u ticks)//tick响应函数
{

	if(((struct widget_button*)p_b)->p_handlers && ((struct widget_button*)p_b)->p_handlers->on_tick)
		((struct widget_button*)p_b)->p_handlers->on_tick(p_b,ticks);
}

void button_on_event(struct widget* p_b,event_id_t event_id,void *p_event)
{
	if(((struct widget_button*)p_b)->p_handlers && ((struct widget_button*)p_b)->p_handlers->on_event)	
		 ((struct widget_button*)p_b)->p_handlers->on_event(p_b,event_id,p_event);
}

void button_on_init(struct widget* p_b,struct widget* p_parent)
{
	if(((struct widget_button*)p_b)->p_handlers && ((struct widget_button*)p_b)->p_handlers->on_init)
		((struct widget_button*)p_b)->p_handlers->on_init(p_b,p_parent);

}
void button_on_destroy(struct widget* p_b)
{
	if(((struct widget_button*)p_b)->p_handlers && ((struct widget_button*)p_b)->p_handlers->on_destroy)
		((struct widget_button*)p_b)->p_handlers->on_destroy(p_b);
}

void button_on_show(struct widget* p_b)
{
	widget_draw_center_string(p_b,0,0,p_b->width,p_b->height,p_b->font,p_b->text_color,p_b->back_color, ((struct widget_button*)p_b)->text);
	if(((struct widget_button*)p_b)->p_handlers && ((struct widget_button*)p_b)->p_handlers->on_show)
		((struct widget_button*)p_b)->p_handlers->on_show(p_b);
}

struct widget* button_on_focus(struct widget* p_b)
{
	p_b->back_color=LCD_COLOR_RESERVED(p_b->back_color);
	p_b->text_color=LCD_COLOR_RESERVED(p_b->text_color);
	widget_refresh(p_b);
	if(((struct widget_button*)p_b)->p_handlers && ((struct widget_button*)p_b)->p_handlers->on_focus)	
		 return ((struct widget_button*)p_b)->p_handlers->on_focus(p_b);	
	else return NULL;

}



