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
ͬ��widget�ڻ�ȡ��һ��widget�Ľ���
*/
struct widget* widget_focus_next(struct widget* p_current)
{
	struct widget *p,*ret;
	if(p_current==NULL)
		return NULL;
	p=p_current->p_next;
	while(p)
	{
		ret=widget_on_focus(p);//�����������������������,������pû�н��㣬�᷵�����н���ĵ�һ������
		if(ret==NULL)
			p=p->p_next;//���������ڽڵ�
		else return ret;
	}
	return NULL;
}

/*
ͬ��widget�ڻ�ȡ��һ��widget�Ľ���
ֱ�ӵ���WidgetOnFocus������ֵΪ��һ��widget�������ӽڵ�������ڽڵ�
*/
struct widget* widget_focus_previous(struct widget* p_current)
{
	struct widget *p,*ret;
	if(p_current==NULL)
		return NULL;
	p=p_current->p_last;
	while(p)
	{
		ret=widget_on_focus(p);//�����������������������,������pû�н��㣬�᷵�����н���ĵ�һ������
		if(ret==NULL)
			p=p->p_last;//���������ڽڵ�
		else return ret;
	}
	return NULL;
}

/*
	��ȡ������
*/
struct widget* widget_focus_parent(struct widget* p_current)
{
	if(p_current==NULL)
		return NULL;
	widget_on_focus(p_current->p_parent);
	return p_current->p_parent;
}
/*ֱ�ӵ���WidgetOnFocus������p_w�������һ���н������widget*/
struct widget* widget_focus(struct widget* p_w)
{
	return widget_on_focus(p_w);
}
/*
�൱��ǰ�����
*/
struct widget* widget_focus_child(struct widget* p_current)
{
	struct widget* p,*ret;
	if(p_current==NULL)
		return NULL;
	p=p_current->p_firstChild;
	while(p)
	{
		ret=widget_on_focus(p);//�����������������������,������pû�н��㣬�᷵�����н���ĵ�һ������
		if(ret==NULL)
			p=p->p_next;//���������ڽڵ�
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
	//���������ʼ��������widget
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
	//��ȡ����
	return widget_focus(p_w);
}
/*
����һ��widget��ͬʱҲ��ζ������������widget,������widget
*/
void widget_destroy(struct widget* p_w)
{
	if(!p_w)
		return;
	if(p_w->p_handlers->on_destroy)
		p_w->p_handlers->on_destroy(p_w);
	widget_create(p_w->p_parent,p_w->p_parent->p_parent);//�Ƿ���Ҫ����������?...
}
	
//��ȡwidget������λ��
//����ֵ��true��ʾ���ڲ���false��ʾ����
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
//	while(p_parent)//������widget��ʼλ�ñ����ڸ��ڲ�
//	{
//		if(p_w->attr.style & WIDGET_STYLE_BORDER)
//		if(x+p_w->width > p_parent->width-WIDGET_BORDER*2 || y+p_w->height > p_parent->height-WIDGET_BORDER*2)
//			ret=FALSE;//��widget������������ʾ
//		x+=p_parent->loc_x+WIDGET_BORDER;
//		y+=p_parent->loc_y+WIDGET_BORDER;
//		p_parent=p_parent->p_parent;
//	}
	*p_x=x;
	*p_y=y;
	return ret;
}

//����
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

	if(!widget_get_absolute_position(p_w,&x,&y))//�ڸ�widget�ڲ��Ĳ��ܱ���ʾ
		return FALSE;
	
	/*���ؼ���仯��С����Ҫ�ø��ؼ��ı���ɫ���Լ���䡣*/
	
	lcd_draw_block(_pDraw,x,y,p_w->width,p_w->height,p_w->back_color);//�Ȱ��Լ�ȫ������
	//DrawBlock(_pDraw,x+p_w->border,y+p_w->border,p_w->width-((p_w->border)<<1),p_w->height-((p_w->border)<<1),p_w->back_color);//�����ɫ
//	for(i=0;i<WIDGET_BORDER;i++)//���߿�,�ĳ����Ȼ��ڲ����ݣ��ٻ��߿�
//	{
//		lcd_draw_horizon(_pDraw,x,y+i,p_w->width,p_w->init_text_color);//�߿���ɫ���ǲ����
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
	if(!widget_draw(p_w))//�Ȼ��Լ��������
        return;
	#ifdef DEBUG_GUI
	lcd_fefresh(&mono_mem_lcd);
	#endif
/*��ʽһ������ʾ������Ԫ�أ�Ȼ����ʾ��Ԫ��*/
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
///*��ʽ��������ʾ�Լ�������ʾ��Ԫ��*/
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
	if(!widget_get_absolute_position(p_w,&x,&y))//�ڸ�widget�ڲ��Ĳ��ܱ���ʾ
		return;
	if(p_w->attr.style&WIDGET_STYLE_BORDER)
	{
		for(i=0;i<WIDGET_BORDER_WIDTH;i++)//��widget_draw���������Ȼ��ڲ����ٻ��߿�,��������ڲ����Ի�����Ķ���
		{
			lcd_draw_horizon(_pDraw,x,y+i,p_w->width,p_w->init_text_color);//�߿���ɫ���ǲ����
			lcd_draw_horizon(_pDraw,x,y+p_w->height-1-i,p_w->width,p_w->init_text_color);		
			lcd_draw_vertical(_pDraw,x+i,y,p_w->height,p_w->init_text_color);
			lcd_draw_vertical(_pDraw,x+p_w->width-1-i,y,p_w->height,p_w->init_text_color);
		}	
	}
	else if(p_w->attr.style & WIDGET_STYLE_UNDERLINE)//�б߿�ĵط��Ͳ���ʾ�»��ߣ���Ϊ�������غϵ�
	{
		for(i=0;i<WIDGET_UNDERLINE_WIDTH;i++)//��widget_draw���������Ȼ��ڲ����ٻ��߿�,��������ڲ����Ի�����Ķ���
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
		_pDraw->refresh_screen();//���ڵ�ɫ��������������������Ļˢ����
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
����ִ�д˺�����widget������WIDGET_HAS_FOCUS���ԣ�������child��focus���ԣ���û����˺�����Ч ��
	ִ��onfoucus���Լ����active widget
����ֵ�����ջ�ȡ�����widget
*/
struct widget* widget_on_focus(struct widget* p_w)
{
	struct widget *p,*p1=NULL;
	if(!p_w)
		return NULL;
	if(!(p_w->attr.attribute & WIDGET_ENABLED) || (p_w->attr.attribute & WIDGET_HIDED))//disable��widget��Ӧ�û�ý���
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
	//��ǰ�н����widget����on_focus�������򷵻�on_foucus��ֵ�����򷵻��Լ�
	if(p_w->p_handlers && p_w->p_handlers->on_focus)
		p1=p_w->p_handlers->on_focus(p_w);
	if(p1)
		return p1;
	else return p;
}

//�¼���ֱ�Ӵ�������ǰwidget�ģ����widget��Ӧ�û��
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
	widget_get_absolute_position(p_w,&x,&y);//��ȡbutton������λ��
//if(p_w->attr.attribute&WIDGET_STYLE_BORDER)	
//    x+=WIDGET_BORDER;//ԭ�����Ȼ��߿��ٻ��ڲ�������Ҫ�����ַ�������ʼ�㣬�Ѹ�
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
	widget_get_absolute_position(p_w,&x,&y);//��ȡbutton������λ��
//if(p_w->attr.attribute&WIDGET_STYLE_BORDER)	
//    x+=WIDGET_BORDER;//ԭ�����Ȼ��߿��ٻ��ڲ�������Ҫ�����ַ�������ʼ�㣬�Ѹ�
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
//�����������еĵ�index������
void widget_draw_center_string_char(struct widget* p_w,int16s index,int16s x_pos, int16s y_pos,int16u width,int16u height,
	struct font* p_font, int16u TextColor , int16u BackColor,const wchar* p_string)
{
	int16s x,y,str_len;
	widget_get_absolute_position(p_w,&x,&y);//��ȡbutton������λ��
	x+=x_pos;
	y+=y_pos;
	str_len=font_get_length(p_font,p_string);
	if(str_len>p_w->width)//��ʾ��������widget��֧�ֱ�ָ��
		return;
	else 	
		lcd_draw_center_string_char(widget_get_lcd(),index,x,y,width,height,p_font,TextColor,BackColor,p_string);
}

void widget_draw_string(struct widget* p_w,int16s x_pos, int16s y_pos,struct font* p_font, int16u TextColor , int16u BackColor, wchar* p_string)
{
    int16s x,y,str_len,str_count;
	wchar str[32];
	widget_get_absolute_position(p_w,&x,&y);//��ȡ����λ��
//if(p_w->attr.attribute&WIDGET_STYLE_BORDER)	
//    x+=WIDGET_BORDER;//ԭ�����Ȼ��߿��ٻ��ڲ�������Ҫ�����ַ�������ʼ�㣬�Ѹ�
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


//line��0��ʼ
void widget_draw_line_center_string(struct widget* p_w, int16s line,
	struct font* p_font, int16u TextColor , int16u BackColor,const wchar* p_string)
{
	widget_draw_center_string(p_w,0,line*(p_font->height+p_w->attr.line_space),p_w->width,p_font->height,p_font,TextColor,BackColor,p_string);
}

void widget_draw_line_string(struct widget* p_w,int16s x_pos, int16s line,struct font* p_font, int16u TextColor , int16u BackColor, wchar* p_string)
{
	widget_draw_string(p_w,x_pos,line*(p_font->height+p_w->attr.line_space),p_font,TextColor ,BackColor, p_string);
}

