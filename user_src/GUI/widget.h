#ifndef WIDGWT_H001
#define WIDGWT_H001
#include "drivers/lcd/lcd.h"
#include "drivers/keyboard/keyboard.h"
#include "config.h"
#define WIDGET_REFERSH_EVENT	1

#define WIDGET_STYLE_NULL			0
#define WIDGET_STYLE_BORDER		0x01
#define WIDGET_STYLE_UNDERLINE		0x02
struct widget;

typedef struct widget_attribute
{
	int8u attribute ;//��ʼ������
	int8u style;//�»��ߣ��߿���,��
	int8u word_space;//�ּ��
	int8u line_space;//�м��
}widget_attribute_t;
//widget�Ĺ�������
typedef struct widget_handler
{
	void (*on_key)(struct widget *p_this,struct key_event* p_keyEvent);//������Ӧ����
	void (*on_tick)(struct widget *p_this,int32u ticks);//tick��Ӧ����
	void (*on_event)(struct widget *p_this,event_id_t event_id,void *p_event);
	void (*on_init)(struct widget *p_this,struct widget* p_parent);
	void (*on_destroy)(struct widget *p_this);
	void (*on_show)(struct widget *p_this);
	struct widget* (*on_focus)(struct widget *p_this);
}widget_handler_t;

#define WIDGET_HAS_FOCUS 	((int8u)1)
#define	WIDGET_FOCUSED		((int8u)2)
#define	WIDGET_ENABLED		((int8u)4)
#define	WIDGET_HIDED		((int8u)8)
#define WIDGET_DIRTY		((int8u)(1<<6))

typedef struct widget
{
	struct widget *p_parent;//ָ�򸸣�
	struct widget *p_firstChild;//ָ���һ����widget����widget��ָ�����ڴ�widget�ڵ�widget����Ҫͬʱ��ʾ�ڽ��档�������һ�������Ļ����Ӧ����0
	struct widget *p_last;//ͬһ��ε�widget�����һ��˫������
	struct widget *p_next;
	widget_handler_t const *p_handlers;
	const void *p_map;//����ͼ
	struct font* font;//��������
	const int16u init_x;//��ʼ��x��λ��,�����parent��λ��
	const int16u init_y;//��ʼ��y��λ��,�����parent��λ��
	const int16u width;
	const int16u height;
	const int16u init_back_color;
	const int16u init_text_color;
	const struct widget_attribute init_attr;
	int16s loc_x;//��ǰλ�ã������parent
	int16s loc_y;
	int16u back_color;
	int16u text_color;
	struct widget_attribute attr;
}widget_t;



struct lcd* widget_get_lcd(void);
void widget_set_lcd(struct lcd* p_draw);
void widget_set_rotate(int8u rot);
struct widget* widget_focus_next(struct widget* p_current);
struct widget* widget_focus_previous(struct widget* p_current);
struct widget* widget_focus_parent(struct widget* p_current);
struct widget* widget_focus_child(struct widget* p_current);
struct widget* widget_focus(struct widget* p_w);
void widget_refresh(struct widget* p_w);
void widget_init(struct widget* p_w,struct widget* p_parent);
struct widget* widget_create(struct widget* p_w,struct widget* p_parent);
void widget_destroy(struct widget* p_w);
BOOL widget_get_absolute_position(struct widget* p_w,int16s* p_x,int16s* p_y );
BOOL widget_draw(struct widget* p_w);
void widget_show(struct widget* p_w);
void widget_on_tick(struct widget* p_w,int16u ticks);
void widget_on_key(struct widget* p_w, struct key_event *p_event);
struct widget* widget_on_focus(struct widget* p_w);
void widget_event_handler(event_id_t event_id,void *p_event);

void widget_draw_pixel(struct widget* p_w,int16s x_pos, int16s y_pos, int16u color);
void widget_draw_center_string(struct widget* p_w,int16s x_pos, int16s y_pos,
	int16u width,int16u height,struct font* p_font, int16u TextColor , int16u BackColor,const wchar* p_string);
void widget_draw_center_string_char(struct widget* p_w,int16s index,int16s x_pos, int16s y_pos,int16u width,int16u height,
	struct font* p_font, int16u TextColor , int16u BackColor,const wchar* p_string);
//����д��������
void widget_draw_line_center_string(struct widget* p_w, int16s line,
	struct font* p_font, int16u TextColor , int16u BackColor,const wchar* p_string);
void widget_draw_line_string(struct widget* p_w,int16s x_pos, int16s line,
	struct font* p_font, int16u TextColor , int16u BackColor, wchar* p_string);
#endif


