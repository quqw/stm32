#include "includes.h"
#include "widget.h"

extern struct lcd mono_mem_lcd;
static struct widget *_pCurrentWidget=NULL;
static struct lcd* _pDraw=&mono_mem_lcd;//&s_memDraw;
static BOOL _screenDirty=FALSE;
struct widget* widget_on_focus(struct widget* p_w);
static int32u lcd_attr;
struct lcd* widget_get_lcd(void)
{
	return _pDraw;
}
void widget_set_lcd(struct lcd* p_draw)
{
	_pDraw=p_draw;
	if(_pDraw)
	{
		_pDraw->attribute=lcd_attr;
	}
}
void widget_set_rotate(int8u rot)
{
	lcd_attr&=~0x07;
	lcd_attr|=(rot&0x07);
	if(_pDraw)
	{
		_pDraw->attribute&=~0x07;
		_pDraw->attribute|=(rot&0x07);
	}
}
//struct widget* GetFocusedWidget(struct widget* p_parent)
//{
//	struct widget* p_w;
//	if(p_parent==NULL)
//		return NULL;
//	p_w=p_parent->p_firstChild;
//	while(p_w)
//	{
//		if(p_w->focused)
//			return p_w;
//		p_w=p_w->p_next;
//	}
//	return p_parent;
//}

/*
同级widget内获取下一个widget的焦点
*/
struct widget* widget_focus_next(struct widget* p_current)
{
	struct widget *p,*ret;
	if(p_current==NULL)
		return NULL;
	p=p_current->p_next;
	while(p)
	{
		ret=widget_on_focus(p);//这个函数里面会遍历所有子树,所以若p没有焦点，会返回其有焦点的第一个子树
		if(ret==NULL)
			p=p->p_next;//遍历所有邻节点
		else return ret;
	}
	return NULL;
}

/*
同级widget内获取上一个widget的焦点
直接调用WidgetOnFocus，返回值为上一个widget或者其子节点或者其邻节点
*/
struct widget* widget_focus_previous(struct widget* p_current)
{
	struct widget *p,*ret;
	if(p_current==NULL)
		return NULL;
	p=p_current->p_last;
	while(p)
	{
		ret=widget_on_focus(p);//这个函数里面会遍历所有子树,所以若p没有焦点，会返回其有焦点的第一个子树
		if(ret==NULL)
			p=p->p_last;//遍历所有邻节点
		else return ret;
	}
	return NULL;
}

/*
	获取父焦点
*/
struct widget* widget_focus_parent(struct widget* p_current)
{
	if(p_current==NULL)
		return NULL;
	widget_on_focus(p_current->p_parent);
	return p_current->p_parent;
}
/*直接调用WidgetOnFocus，返回p_w或者其第一个有焦点的子widget*/
struct widget* widget_focus(struct widget* p_w)
{
	return widget_on_focus(p_w);
}
/*
相当于前序遍历
*/
struct widget* widget_focus_child(struct widget* p_current)
{
	struct widget* p,*ret;
	if(p_current==NULL)
		return NULL;
	p=p_current->p_firstChild;
	while(p)
	{
		ret=widget_on_focus(p);//这个函数里面会遍历所有子树,所以若p没有焦点，会返回其有焦点的第一个子树
		if(ret==NULL)
			p=p->p_next;//遍历所有邻节点
		else return ret;
	}
	return NULL;
}

void widget_refresh(struct widget* p_w)
{
	if(p_w)
		p_w->attr.attribute|=WIDGET_DIRTY;
}

void widget_init(struct widget* p_w,struct widget* p_parent)
{
	struct widget *p_c;
	if(!p_w)
		return;
	p_w->loc_x=p_w->init_x;
	p_w->loc_y=p_w->init_y;
	p_w->back_color=p_w->init_back_color;
	p_w->text_color=p_w->init_text_color;
	memcpy(&(p_w->attr),&(p_w->init_attr),sizeof(struct widget_attribute));
	p_w->attr.attribute|=WIDGET_DIRTY;
	p_w->p_parent=p_parent;
  if(!(p_w->attr.attribute & WIDGET_ENABLED))
		return;
	//下面继续初始化所有子widget
	p_c=p_w->p_firstChild;
	while(p_c)
	{
		widget_init(p_c,p_w);
		p_c=p_c->p_next;
	}
	if(p_w->p_handlers && p_w->p_handlers->on_init)
		p_w->p_handlers->on_init(p_w,p_parent);
	#ifdef DEBUG_GUI
	lcd_fefresh(&mono_mem_lcd);
	#endif
}

struct widget* widget_create(struct widget* p_w,struct widget* p_parent)
{
	widget_init(p_w,p_parent);
	widget_show(p_w);
	_pCurrentWidget=p_w;
	//获取焦点
	return widget_focus(p_w);
}
/*
销毁一个widget，同时也意味着销毁所有子widget,建立父widget
*/
void widget_destroy(struct widget* p_w)
{
	if(!p_w)
		return;
	if(p_w->p_handlers->on_destroy)
		p_w->p_handlers->on_destroy(p_w);
	widget_create(p_w->p_parent,p_w->p_parent->p_parent);//是否需要立即建立父?...
}
	
//获取widget的物理位置
//返回值：true表示在内部，false表示超出
BOOL widget_get_absolute_position(struct widget* p_w,int16s* p_x,int16s* p_y )
{
//	struct widget *p_parent;
	BOOL ret=TRUE;
	int16s x=0,y=0;
	x=p_w->loc_x;
	y=p_w->loc_y;
	if(x<0 ||y<0)
		ret=FALSE;
//	p_parent=p_w->p_parent;
//	while(p_parent)//限制子widget起始位置必须在父内部
//	{
//		if(p_w->attr.style & WIDGET_STYLE_BORDER)
//		if(x+p_w->width > p_parent->width-WIDGET_BORDER*2 || y+p_w->height > p_parent->height-WIDGET_BORDER*2)
//			ret=FALSE;//子widget超出父，则不显示
//		x+=p_parent->loc_x+WIDGET_BORDER;
//		y+=p_parent->loc_y+WIDGET_BORDER;
//		p_parent=p_parent->p_parent;
//	}
	*p_x=x;
	*p_y=y;
	return ret;
}

//绘制
BOOL widget_draw(struct widget* p_w)
{
	int16s x=0,y=0;
	if(!p_w)
		return FALSE;
	if(p_w->attr.attribute & WIDGET_HIDED)
		return FALSE;
	if(!(p_w->attr.attribute & WIDGET_ENABLED))
		return FALSE;
//	if(!(p_w->attribute & struct widget_DIRTY))
//		return;

	if(!widget_get_absolute_position(p_w,&x,&y))//在父widget内部的才能被显示
		return FALSE;
	
	/*若控件会变化大小，先要用父控件的背景色将自己填充。*/
	
	lcd_draw_block(_pDraw,x,y,p_w->width,p_w->height,p_w->back_color);//先把自己全部填充掉
	//DrawBlock(_pDraw,x+p_w->border,y+p_w->border,p_w->width-((p_w->border)<<1),p_w->height-((p_w->border)<<1),p_w->back_color);//填充颜色
//	for(i=0;i<WIDGET_BORDER;i++)//画边框,改成先先画内部数据，再画边框
//	{
//		lcd_draw_horizon(_pDraw,x,y+i,p_w->width,p_w->init_text_color);//边框颜色总是不变的
//		lcd_draw_horizon(_pDraw,x,y+p_w->height-1-i,p_w->width,p_w->init_text_color);		
//		lcd_draw_vertical(_pDraw,x+i,y,p_w->height,p_w->init_text_color);
//		lcd_draw_vertical(_pDraw,x+p_w->width-1-i,y,p_w->height,p_w->init_text_color);
//	}	
	
    return TRUE;
}

void widget_show(struct widget* p_w)
{
	struct widget *p_c;
	int i;
	int16s x,y;
	if(p_w==NULL)
		return;
	if(!(p_w->attr.attribute & WIDGET_ENABLED))
		return;	
	if(p_w->attr.attribute & WIDGET_HIDED)
		return;
	if(!widget_draw(p_w))//先画自己整个框架
        return;
	#ifdef DEBUG_GUI
	lcd_fefresh(&mono_mem_lcd);
	#endif
/*方式一：先显示所有子元素，然后显示父元素*/
	p_c=p_w->p_firstChild;
	while(p_c)
	{
		widget_show(p_c);
		p_c=p_c->p_next;
	}
		#ifdef DEBUG_GUI
	lcd_fefresh(&mono_mem_lcd);
	#endif
	if(p_w->p_handlers && p_w->p_handlers->on_show)
		p_w->p_handlers->on_show(p_w);
		#ifdef DEBUG_GUI
	lcd_fefresh(&mono_mem_lcd);
	#endif
///*方式二：先显示自己，再显示子元素*/
//	if(p_w->p_handlers && p_w->p_handlers->on_show)
//		p_w->p_handlers->on_show(p_w);
//	#ifdef DEBUG_GUI
//	lcd_fefresh(&mono_mem_lcd);
//	#endif
//	p_c=p_w->p_firstChild;
//	while(p_c)
//	{
//		widget_show(p_c);
//		p_c=p_c->p_next;
//	}
	if(!widget_get_absolute_position(p_w,&x,&y))//在父widget内部的才能被显示
		return;
	if(p_w->attr.style&WIDGET_STYLE_BORDER)
	{
		for(i=0;i<WIDGET_BORDER_WIDTH;i++)//由widget_draw内移来，先画内部，再画边框,这样如果内部可以画更大的东西
		{
			lcd_draw_horizon(_pDraw,x,y+i,p_w->width,p_w->init_text_color);//边框颜色总是不变的
			lcd_draw_horizon(_pDraw,x,y+p_w->height-1-i,p_w->width,p_w->init_text_color);		
			lcd_draw_vertical(_pDraw,x+i,y,p_w->height,p_w->init_text_color);
			lcd_draw_vertical(_pDraw,x+p_w->width-1-i,y,p_w->height,p_w->init_text_color);
		}	
	}
	else if(p_w->attr.style & WIDGET_STYLE_UNDERLINE)//有边框的地方就不显示下划线，因为两者是重合的
	{
		for(i=0;i<WIDGET_UNDERLINE_WIDTH;i++)//由widget_draw内移来，先画内部，再画边框,这样如果内部可以画更大的东西
		{
			lcd_draw_horizon(_pDraw,x,y+p_w->height-1-i,p_w->width,p_w->init_text_color);		
		}	
	}
	p_w->attr.attribute &=~ WIDGET_DIRTY;
	#ifdef DEBUG_GUI
	lcd_fefresh(&mono_mem_lcd);
	#endif
  ATOMIC_SET(_screenDirty,TRUE);
	
}

void widget_on_tick(struct widget* p_w,int16u ticks)
{
	if(!p_w)
		return;
	if(p_w->p_handlers && p_w->p_handlers->on_tick)
		p_w->p_handlers->on_tick(p_w,ticks);
	if(p_w->attr.attribute & WIDGET_DIRTY)
		widget_show(p_w);	
	if(_screenDirty)
	{
		_pDraw->refresh_screen();//对于单色屏，这里是真正的往屏幕刷数据
		ATOMIC_SET(_screenDirty,FALSE);
	}
}

void widget_on_key(struct widget* p_w, struct key_event *p_event)
{
	if(!p_w)
		return;
	if(p_w->p_handlers && p_w->p_handlers->on_key)
        p_w->p_handlers->on_key(p_w,p_event);
}
void widget_on_event(struct widget* p_w,event_id_t event_id,void *p_event)
{
	if(!p_w)
		return;
	if(p_w->p_handlers && p_w->p_handlers->on_event)
        p_w->p_handlers->on_event(p_w,event_id,p_event);
}
/*
规则：执行此函数的widget必须有WIDGET_HAS_FOCUS属性，否则检查child的focus属性，若没有则此函数无效 。
	执行onfoucus后自己变成active widget
返回值：最终获取焦点的widget
*/
struct widget* widget_on_focus(struct widget* p_w)
{
	struct widget *p,*p1=NULL;
	if(!p_w)
		return NULL;
	if(!(p_w->attr.attribute & WIDGET_ENABLED) || (p_w->attr.attribute & WIDGET_HIDED))//disable的widget不应该获得焦点
		return NULL;
	if(p_w->attr.attribute & WIDGET_HAS_FOCUS)
	{
		p_w->attr.attribute|=WIDGET_FOCUSED;
		_pCurrentWidget->attr.attribute&=~WIDGET_FOCUSED;
		_pCurrentWidget=p_w;
		p=p_w;
	}
	else
	{
		p=widget_focus_child(p_w);
	}
	//当前有焦点的widget若有on_focus函数，则返回on_foucus的值，否则返回自己
	if(p_w->p_handlers && p_w->p_handlers->on_focus)
		p1=p_w->p_handlers->on_focus(p_w);
	if(p1)
		return p1;
	else return p;
}

//事件是直接触发给当前widget的，别的widget不应该获得
void widget_event_handler(event_id_t event_id,void *p_event)
{

	if(!_pCurrentWidget)
		return;
	if(event_id==KEY_EVENT_ID)
		widget_on_key(_pCurrentWidget,(struct key_event*)p_event);
	else if(event_id==TICK_EVENT_ID)
		widget_on_tick(_pCurrentWidget,*(int16u*)p_event);
	else
		widget_on_event(_pCurrentWidget,event_id,p_event);
}




//////////////////////////////////////////////////////////////////////////////////
void widget_draw_pixel(struct widget* p_w,int16s x_pos, int16s y_pos, int16u color)
{
    int16s x,y;
	widget_get_absolute_position(p_w,&x,&y);//获取button的物理位置
//if(p_w->attr.attribute&WIDGET_STYLE_BORDER)	
//    x+=WIDGET_BORDER;//原先是先画边框再画内部，所以要计算字符串的起始点，已改
//    y+=WIDGET_BORDER;
	x+=x_pos;
	y+=y_pos;
	lcd_draw_pixel(_pDraw,x,y,color);
}

void widget_draw_center_string(struct widget* p_w,int16s x_pos, int16s y_pos,int16u width,int16u height,
	struct font* p_font, int16u TextColor , int16u BackColor,const wchar* p_string)
{
    int16s x,y,str_len,str_count;
	wchar str[32];
	widget_get_absolute_position(p_w,&x,&y);//获取button的物理位置
//if(p_w->attr.attribute&WIDGET_STYLE_BORDER)	
//    x+=WIDGET_BORDER;//原先是先画边框再画内部，所以要计算字符串的起始点，已改
//    y+=WIDGET_BORDER;
	x+=x_pos;
	y+=y_pos;
	str_len=font_get_length(p_font,p_string);
	if(str_len>p_w->width)
	{
		str_len=p_w->width;
		str_count=str_len/p_font->width;
		if(str_count<=2)
			return;
		memcpy(str,p_string,(str_count-2)*sizeof(wchar));
		str[(str_count-2)*sizeof(wchar)]='.';
		str[(str_count-1)*sizeof(wchar)]='.';
		str[(str_count)*sizeof(wchar)]=0;
		lcd_draw_center_string(widget_get_lcd(),x,y,width,height,p_font,TextColor,BackColor,str);
	}
	else 	
		lcd_draw_center_string(widget_get_lcd(),x,y,width,height,p_font,TextColor,BackColor,p_string);
	
}
//画居中字体中的第index个文字
void widget_draw_center_string_char(struct widget* p_w,int16s index,int16s x_pos, int16s y_pos,int16u width,int16u height,
	struct font* p_font, int16u TextColor , int16u BackColor,const wchar* p_string)
{
	int16s x,y,str_len;
	widget_get_absolute_position(p_w,&x,&y);//获取button的物理位置
	x+=x_pos;
	y+=y_pos;
	str_len=font_get_length(p_font,p_string);
	if(str_len>p_w->width)//显示不完整的widget不支持本指令
		return;
	else 	
		lcd_draw_center_string_char(widget_get_lcd(),index,x,y,width,height,p_font,TextColor,BackColor,p_string);
}

void widget_draw_string(struct widget* p_w,int16s x_pos, int16s y_pos,struct font* p_font, int16u TextColor , int16u BackColor, wchar* p_string)
{
    int16s x,y,str_len,str_count;
	wchar str[32];
	widget_get_absolute_position(p_w,&x,&y);//获取物理位置
//if(p_w->attr.attribute&WIDGET_STYLE_BORDER)	
//    x+=WIDGET_BORDER;//原先是先画边框再画内部，所以要计算字符串的起始点，已改
//    y+=WIDGET_BORDER;
	x+=x_pos;
	y+=y_pos;
	str_len=font_get_length(p_font,p_string);
	if(str_len>p_w->width)
	{
		str_len=p_w->width;
		str_count=str_len/p_font->width;
		if(str_count<=2)
			return;
		memcpy(str,p_string,(str_count-2)*sizeof(wchar));
		str[(str_count-2)*sizeof(wchar)]='.';
		str[(str_count-1)*sizeof(wchar)]='.';
		str[(str_count)*sizeof(wchar)]=0;
		lcd_draw_string(widget_get_lcd(),x,y,p_font,TextColor,BackColor,str);
	}
	else 	
		lcd_draw_string(widget_get_lcd(),x,y,p_font,TextColor,BackColor,p_string);
}


//line从0开始
void widget_draw_line_center_string(struct widget* p_w, int16s line,
	struct font* p_font, int16u TextColor , int16u BackColor,const wchar* p_string)
{
	widget_draw_center_string(p_w,0,line*(p_font->height+p_w->attr.line_space),p_w->width,p_font->height,p_font,TextColor,BackColor,p_string);
}

void widget_draw_line_string(struct widget* p_w,int16s x_pos, int16s line,struct font* p_font, int16u TextColor , int16u BackColor, wchar* p_string)
{
	widget_draw_string(p_w,x_pos,line*(p_font->height+p_w->attr.line_space),p_font,TextColor ,BackColor, p_string);
}

