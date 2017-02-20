#include "types.h"
#include "drivers/keyboard/keyboard.h"
#include "widget_edit.h"

char STR_FLOAT[]="0123456789.";
char STR_NUM[]="0123456789";
char STR_ALPHABET[]=" abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ";
char STR_NUMALPH[]="0123456789. abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ";
void edit_on_key(struct widget* p_e,struct key_event* p_keyEvent);
void edit_on_tick(struct widget* p_e,int32u ticks);//tick响应函数
void edit_on_event(struct widget* p_e,event_id_t event_id,void *p_event);
void edit_on_init(struct widget* p_e,struct widget* p_parent);
void edit_on_destroy(struct widget* p_e);
void edit_on_show(struct widget* p_e);
struct widget* edit_on_focus(struct widget* p_e);

/*edit的基本动作，初始化给base.p_handlers*/
const struct widget_handler edit_handler={edit_on_key,edit_on_tick,edit_on_event,
																	edit_on_init,edit_on_destroy,edit_on_show,edit_on_focus};


int edit_get_num(struct widget_edit* p_e)
{
	int i;
	float num=0;
	if(p_e->attribute & EDIT_NUMBER)
	{
		for(i=0;i<p_e->length;i++)
		{
			num*=10;
			num+=p_e->text[i];	
		}
		return num;
	}
	return 0;
}

void edit_set_num(struct widget_edit* p_e,int num)
{
	int i;
	if(!(p_e->attribute & EDIT_NUMBER))
		return;
	for(i=p_e->length-1;i>=0;i--)
	{	
		p_e->text[i]=num%10;	
		num/=10;
	}
}

void edit_set_initnum(struct widget_edit* p_e,int num)
{
	int i;
	if(!(p_e->attribute & EDIT_NUMBER))
		return;
	for(i=p_e->length-1;i>=0;i--)
	{	
		p_e->init_text[i]=num%10;	
		num/=10;
	}
}
//转换text widget的字符到真正的字符
void edit_get_text(struct widget_edit* p_e,char* text)
{
	int i;
	if((p_e->attribute & EDIT_NUMBER) && (p_e->attribute & EDIT_ALPHABET))
	{
		for(i=0;i<p_e->length;i++)
		{
			text[i]=STR_NUMALPH[p_e->text[i]];
		}
	}
	else if(p_e->attribute & EDIT_NUMBER)
	{
		for(i=0;i<p_e->length;i++)
		{
			text[i]=STR_NUM[p_e->text[i]];
		}
	}
	else if(p_e->attribute & EDIT_ALPHABET)
	{
		for(i=0;i<p_e->length;i++)
		{
			text[i]=STR_ALPHABET[p_e->text[i]];
		}
	}
}
/*先执行公共属性动作，然后再执行特殊动作*/
//上下：编辑状态数值变化，不在编辑状态定位到另一个widget
//左右：编辑状态编辑位置变化，不在编辑状态定位到另一个widget
//确认：进入或退出编辑状态																	
void edit_on_key(struct widget* p_w,struct key_event* p_keyEvent)
{	
	struct widget_edit* p_e= ((struct widget_edit*)p_w);
	if(p_keyEvent->key_action==KEY_PRESS)
	{
		switch(p_keyEvent->modify_val)
		{
#ifdef WIDGET_EDIT_PREV//定位之前的一个widget
			case WIDGET_EDIT_PREV:
				if(p_e->state & EDIT_STATE_EDITMODE)
				{
					p_e->focus_index--;
					if(p_e->focus_index<0)
						p_e->focus_index=p_e->length-1;
				}
				else
				{
					if(!p_w->p_last)
						return;
					p_w->back_color=p_w->init_back_color;
					p_w->text_color=p_w->init_text_color;
					p_w->attr.style&=~WIDGET_STYLE_BORDER;
					if(widget_focus_previous(p_w)==NULL)//如果有上一个可以获取焦点的widget，则获取焦点
						widget_focus(p_w);//若没有，则焦点还在自己，这种情况不需要重画自己
					else
						widget_show(p_w);//自己的样式改变，需要重画
				}
			break;
#endif
#ifdef	WIDGET_EDIT_NEXT		//定位之后的一个widget
			case WIDGET_EDIT_NEXT:
				if(p_e->state & EDIT_STATE_EDITMODE)
				{
					p_e->focus_index++;
					if(p_e->focus_index>=p_e->length)
						p_e->focus_index=0;
				}
				else
				{
					if(!p_w->p_next)
						return;
					p_w->back_color=p_w->init_back_color;
					p_w->text_color=p_w->init_text_color;
					p_w->attr.style&=~WIDGET_STYLE_BORDER;
					if(NULL==widget_focus_next(p_w))
						widget_focus(p_w);
					else
						widget_show(p_w);
				}
				break;
#endif	
				
#ifdef	WIDGET_EDIT_CONFIRM //确认编辑
			case WIDGET_EDIT_CONFIRM:
				if(p_e->state & EDIT_STATE_EDITMODE)
				{
					p_w->back_color=p_w->init_back_color;
					p_w->text_color=p_w->init_text_color;
					p_e->state &= ~EDIT_STATE_EDITMODE;
				}
				else
				{
					p_w->back_color=WIDGET_EDIT_SELECT_BACK_COLOR;
					p_w->text_color=WIDGET_EDIT_SELECT_TEXT_COLOR;
					p_e->state|=EDIT_STATE_EDITMODE;
				}
					p_e->focus_index=0;
			break;
#endif
#ifdef	WIDGET_EDIT_CANCEL
			case 	WIDGET_EDIT_CANCEL:
			{
				if(p_e->state & EDIT_STATE_EDITMODE)
				{
					memcpy(p_e->text,p_e->init_text,MAX_EDIT_LENGTH);
					p_w->back_color=p_w->init_back_color;
					p_w->text_color=p_w->init_text_color;
					p_e->state &= ~EDIT_STATE_EDITMODE;
				}
				p_e->focus_index=0;
			}
			break;
				
#endif
				
#ifdef	WIDGET_EDIT_VALUE_UP //字符上涨
			case WIDGET_EDIT_VALUE_UP:
				if(0==(p_e->state & EDIT_STATE_EDITMODE))
					return;
				p_e->text[p_e->focus_index]++;
				if((p_e->attribute & EDIT_NUMBER) && (p_e->attribute & EDIT_ALPHABET))
				{
					if(p_e->text[p_e->focus_index]>=strlen(STR_NUMALPH))
						p_e->text[p_e->focus_index]=0;
				}
				else if(p_e->attribute & EDIT_NUMBER)
				{
					if(p_e->text[p_e->focus_index]>=strlen(STR_NUM))
						p_e->text[p_e->focus_index]=0;
				}
				else if(p_e->attribute & EDIT_ALPHABET)
				{
					if(p_e->text[p_e->focus_index]>=strlen(STR_ALPHABET))
						p_e->text[p_e->focus_index]=0;
				}
			break;				
#endif

#ifdef	WIDGET_EDIT_VALUE_DOWN //下翻字符
				case WIDGET_EDIT_VALUE_DOWN:
				if(0==(p_e->state & EDIT_STATE_EDITMODE))
					return;
				p_e->text[p_e->focus_index]--;
				if((p_e->attribute & EDIT_NUMBER) && (p_e->attribute & EDIT_ALPHABET))
				{
					if(p_e->text[p_e->focus_index]<0)
						p_e->text[p_e->focus_index]=strlen(STR_NUMALPH)-1;
				}
				else if(p_e->attribute & EDIT_NUMBER)
				{
					if(p_e->text[p_e->focus_index]<0)
						p_e->text[p_e->focus_index]=strlen(STR_NUM)-1;
				}
				else if(p_e->attribute & EDIT_ALPHABET)
				{
					if(p_e->text[p_e->focus_index]<0)
						p_e->text[p_e->focus_index]=strlen(STR_ALPHABET)-1;
				}
			break;	
#endif								
			default:
				break;
		}
	}
	else if(p_keyEvent->key_action==KEY_RELEASE)
	{
		switch(p_keyEvent->key_value)
		{
			#ifdef	BUTTON_ENTER
			case BUTTON_ENTER:
				p_w->back_color=WIDGET_EDIT_SELECT_BACK_COLOR;
				//widget_refresh(p_w);
				break;
			#endif
			default:
				break;
		}
	}
	if(p_e->p_handlers && (p_e->p_handlers->on_key))
		p_e->p_handlers->on_key(p_w,p_keyEvent);
	widget_refresh(p_w);
}


void edit_on_tick(struct widget* p_e,int32u ticks)//tick响应函数
{
	if(((struct widget_edit*)p_e)->p_handlers && ((struct widget_edit*)p_e)->p_handlers->on_tick)
		((struct widget_edit*)p_e)->p_handlers->on_tick(p_e,ticks);
}

void edit_on_event(struct widget* p_e,event_id_t event_id,void *p_event)
{
	if(((struct widget_edit*)p_e)->p_handlers && ((struct widget_edit*)p_e)->p_handlers->on_event)	
		 ((struct widget_edit*)p_e)->p_handlers->on_event(p_e,event_id,p_event);
}

void edit_on_init(struct widget* p_w,struct widget* p_parent)
{
	struct widget_edit* p_e= ((struct widget_edit*)p_w);
	p_e->focus_index=0;
	memset(p_e->init_text,0,MAX_EDIT_LENGTH);
	p_e->state=EDIT_STATE_NULL;
	if(((struct widget_edit*)p_e)->p_handlers && ((struct widget_edit*)p_e)->p_handlers->on_init)
		((struct widget_edit*)p_e)->p_handlers->on_init(p_w,p_parent);
	memcpy(p_e->text,p_e->init_text,MAX_EDIT_LENGTH);
}
void edit_on_destroy(struct widget* p_e)
{
	if(((struct widget_edit*)p_e)->p_handlers && ((struct widget_edit*)p_e)->p_handlers->on_destroy)
		((struct widget_edit*)p_e)->p_handlers->on_destroy(p_e);
}

void edit_on_show(struct widget* p_w)
{
	char text[MAX_EDIT_LENGTH];
	wchar wtext[MAX_EDIT_LENGTH];
	struct widget_edit* p_e= ((struct widget_edit*)p_w);
	edit_get_text(p_e,text);
	text[p_e->length]=0;
	to_wchar(text,wtext);
	widget_draw_center_string(p_w,0,0,p_w->width,p_w->height,p_w->font,p_w->text_color,
			p_w->back_color, wtext);
	if(p_e->state & EDIT_STATE_EDITMODE)
		widget_draw_center_string_char(p_w,p_e->focus_index,0,0,p_w->width,p_w->height,p_w->font,
						LCD_COLOR_RESERVED(p_w->text_color),LCD_COLOR_RESERVED(p_w->back_color), wtext);
	if(p_e->p_handlers && p_e->p_handlers->on_show)
		p_e->p_handlers->on_show(p_w);

}

struct widget* edit_on_focus(struct widget* p_e)
{
//	p_e->back_color=LCD_COLOR_RESERVED(p_e->back_color);
//	p_e->text_color=LCD_COLOR_RESERVED(p_e->text_color);
	p_e->attr.style|=WIDGET_STYLE_BORDER;
	widget_refresh(p_e);
	if(((struct widget_edit*)p_e)->p_handlers && ((struct widget_edit*)p_e)->p_handlers->on_focus)	
		 return ((struct widget_edit*)p_e)->p_handlers->on_focus(p_e);	
	else return NULL;

}



