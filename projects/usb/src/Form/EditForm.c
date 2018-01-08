#include "includes.h"
#include "gui/widget_edit.h"
#include "gui/widget_button.h"
#include "gui/widget_form.h"
#include "editform.h"
#include "string.h"
#include "apps/motor-control.h"
#include "apps/storage.h"
#include "form/mainform.h"

#define MODE_MOVE   1
#define MODE_NAVI   2


#define FORM_WIDTH	128
#define FORM_HEIGHT	64
#define BUTTON_WIDTH	52
#define BUTTON_HEIGHT	12
#define BUTTON_BORDER	0

#define EDIT_WIDTH_2	15
#define EDIT_WIDTH	30
#define EDIT_HEIGHT	12
#define EDIT_BORDER	1

#define MENUFORM_X POS_CENTER(0,SCREEN_WIDTH-1,MENUFORM_WIDTH)
#define MENUFORM_Y POS_CENTER(0,SCREEN_HEIGHT-1-12,MENUFORM_HEIGHT)



static struct widget_form _form_edit;//������
static struct widget_edit _edit_freq0;//��Ƶ����
static struct widget_edit _edit_time0;
static struct widget_edit _edit_freq1;//��Ƶ����
static struct widget_edit _edit_time1;
static struct widget_edit _edit_freq2;//��Ƶ����
static struct widget_edit _edit_time2;
static struct widget_edit _edit_freq_door;//����Ƶ����
static struct widget_edit _edit_time_door;
static struct widget_button _btn_cancel;
static struct widget_button _btn_set;


/////��һ��edit///
static void edit_freq0_on_init(struct widget* p_b,struct widget* p_parent);
void edit_freq0_on_key(struct widget* p_w,struct key_event* p_keyEvent);
static const struct widget_handler _edit_freq0_handler={edit_freq0_on_key,NULL,NULL,edit_freq0_on_init,NULL,NULL,NULL};
static struct widget_edit _edit_freq0={
																			{
																				(struct widget*)&_form_edit,//��widget
																				NULL,//��һ����widget
																				NULL,//last widgetͬһ��ε�widget�����һ��˫������
																				(struct widget*)&_edit_time0,//next widget
																				&edit_handler,//button widget�����¼�
																				NULL,//����
																				&font1212,
																				32,//��ʼ��x��λ��
																				0,//��ʼ��y��λ��
																				EDIT_WIDTH_2,//�ؼ����
																				EDIT_HEIGHT,//�ؼ��߶�
																				LCD_COLOR_WHITE,//����ɫ
																				LCD_COLOR_BLACK,//������ɫ
																				{
																					WIDGET_HAS_FOCUS|WIDGET_ENABLED,//��ʼ������
																					WIDGET_STYLE_UNDERLINE,//�߿���
																					1,1,//�ּ�࣬�м��
																				},
																			},
																			&_edit_freq0_handler,
																			EDIT_NUMBER,	
																			2																			
																			};
static void edit_freq0_on_init(struct widget* p_w,struct widget* p_parent)
{
		struct widget_edit *p_e=(struct widget_edit *)p_w;
	edit_set_initnum(p_e,s_time_params.item[0].work_freq/100);
}
void edit_freq0_on_key(struct widget* p_w,struct key_event* p_keyEvent)
{	
	struct widget_edit* p_e= ((struct widget_edit*)p_w);
	if(p_keyEvent->key_action==KEY_PRESS)
	{
		switch(p_keyEvent->modify_val)
		{
			case WIDGET_EDIT_VALUE_UP:
				if(0==(p_e->state & EDIT_STATE_EDITMODE))
					return;
				if(edit_get_num(p_e)>MAX_WORK_FREQ_MAX_VAL/100)
					edit_set_num(p_e,MAX_WORK_FREQ_MAX_VAL/100);
			break;
			case WIDGET_EDIT_VALUE_DOWN:
				if(0==(p_e->state & EDIT_STATE_EDITMODE))
					return;
				if(edit_get_num(p_e)>MAX_WORK_FREQ_MAX_VAL/100)
					edit_set_num(p_e,0);
		}
	}
}
/////��2��edit,��Ƶʱ��///
void edit_time0_on_key(struct widget* p_w,struct key_event* p_keyEvent);
static void edit_time0_on_init(struct widget* p_b,struct widget* p_parent);
static const struct widget_handler _edit_time_low_handler={edit_time0_on_key,NULL,NULL,edit_time0_on_init,NULL,NULL,NULL};
static struct widget_edit _edit_time0={
																			{
																				(struct widget*)&_form_edit,//��widget
																				NULL,//��һ����widget
																				(struct widget*)&_edit_freq0,//last widgetͬһ��ε�widget�����һ��˫������
																				(struct widget*)&_edit_freq1,//next widget
																				&edit_handler,//button widget�����¼�
																				NULL,//����
																				&font1212,
																				94,//��ʼ��x��λ��
																				0,//��ʼ��y��λ��
																				EDIT_WIDTH,//�ؼ����
																				EDIT_HEIGHT,//�ؼ��߶�
																				LCD_COLOR_WHITE,//����ɫ
																				LCD_COLOR_BLACK,//������ɫ
																				{
																					WIDGET_HAS_FOCUS|WIDGET_ENABLED,//��ʼ������
																					WIDGET_STYLE_UNDERLINE,//�߿���
																					1,1,//�ּ�࣬�м��
																				},
																			},
																			&_edit_time_low_handler,
																			EDIT_NUMBER,	
																			4
																			};
static void edit_time0_on_init(struct widget* p_w,struct widget* p_parent)
{
	struct widget_edit *p_e=(struct widget_edit *)p_w;
	edit_set_initnum(p_e,s_time_params.item[0].period/10);
	//��ȡ��ʼֵ
}
void edit_time0_on_key(struct widget* p_w,struct key_event* p_keyEvent)
{	
	struct widget_edit* p_e= ((struct widget_edit*)p_w);
	if(p_keyEvent->key_action==KEY_PRESS)
	{
		switch(p_keyEvent->modify_val)
		{
			case WIDGET_EDIT_VALUE_UP:
				if(0==(p_e->state & EDIT_STATE_EDITMODE))
					return;
				if(edit_get_num(p_e)>MAX_TIME/10)
					edit_set_num(p_e,MAX_TIME/10);
			break;
			case WIDGET_EDIT_VALUE_DOWN:
				if(0==(p_e->state & EDIT_STATE_EDITMODE))
					return;
				if(edit_get_num(p_e)>MAX_TIME/10)
					edit_set_num(p_e,0);
		}
	}
}
/////��Ƶ�༭����3��edit///
void edit_freq1_on_key(struct widget* p_w,struct key_event* p_keyEvent);
static void edit_freq1_on_init(struct widget* p_w,struct widget* p_parent);
static const struct widget_handler _edit_freq1_handler={edit_freq1_on_key,NULL,NULL,edit_freq1_on_init,NULL,NULL,NULL};
static struct widget_edit _edit_freq1={
																			{
																				(struct widget*)&_form_edit,//��widget
																				NULL,//��һ����widget
																				(struct widget*)&_edit_time0,//last widgetͬһ��ε�widget�����һ��˫������
																				(struct widget*)&_edit_time1,//next widget
																				&edit_handler,//button widget�����¼�
																				NULL,//����
																				&font1212,
																				32,//��ʼ��x��λ��
																				13,//��ʼ��y��λ��
																				EDIT_WIDTH_2,//�ؼ����
																				EDIT_HEIGHT,//�ؼ��߶�
																				LCD_COLOR_WHITE,//����ɫ
																				LCD_COLOR_BLACK,//������ɫ
																				{
																					WIDGET_HAS_FOCUS|WIDGET_ENABLED,//��ʼ������
																					WIDGET_STYLE_UNDERLINE,//�߿���
																					1,1,//�ּ�࣬�м��
																				},
																			},
																			&_edit_freq1_handler,
																			EDIT_NUMBER,	
																			2																			
																			};
static void edit_freq1_on_init(struct widget* p_w,struct widget* p_parent)
{
	struct widget_edit *p_e=(struct widget_edit *)p_w;
	edit_set_initnum(p_e,s_time_params.item[1].work_freq/100);
}
void edit_freq1_on_key(struct widget* p_w,struct key_event* p_keyEvent)
{	
	struct widget_edit* p_e= ((struct widget_edit*)p_w);
	if(p_keyEvent->key_action==KEY_PRESS)
	{
		switch(p_keyEvent->modify_val)
		{
			case WIDGET_EDIT_VALUE_UP:
				if(0==(p_e->state & EDIT_STATE_EDITMODE))
					return;
				if(edit_get_num(p_e)>MAX_WORK_FREQ_MAX_VAL/100)
					edit_set_num(p_e,MAX_WORK_FREQ_MAX_VAL/100);
			break;
			case WIDGET_EDIT_VALUE_DOWN:
				if(0==(p_e->state & EDIT_STATE_EDITMODE))
					return;
				if(edit_get_num(p_e)>MAX_WORK_FREQ_MAX_VAL/100)
					edit_set_num(p_e,0);
		}
	}
}
/////��4��edit,��Ƶʱ��///
void edit_time1_on_key(struct widget* p_w,struct key_event* p_keyEvent);
static void edit_time1_on_init(struct widget* p_b,struct widget* p_parent);
static const struct widget_handler _edit_time1_handler={edit_time1_on_key,NULL,NULL,edit_time1_on_init,NULL,NULL,NULL};
static struct widget_edit _edit_time1={
																			{
																				(struct widget*)&_form_edit,//��widget
																				NULL,//��һ����widget
																				(struct widget*)&_edit_freq1,//last widgetͬһ��ε�widget�����һ��˫������
																				(struct widget*)&_edit_freq2,//next widget
																				&edit_handler,//button widget�����¼�
																				NULL,//����
																				&font1212,
																				94,//��ʼ��x��λ��
																				13,//��ʼ��y��λ��
																				EDIT_WIDTH,//�ؼ����
																				EDIT_HEIGHT,//�ؼ��߶�
																				LCD_COLOR_WHITE,//����ɫ
																				LCD_COLOR_BLACK,//������ɫ
																				{
																					WIDGET_HAS_FOCUS|WIDGET_ENABLED,//��ʼ������
																					WIDGET_STYLE_UNDERLINE,//�߿���
																					1,1,//�ּ�࣬�м��
																				},
																			},
																			&_edit_time1_handler,
																			EDIT_NUMBER,	
																			4
																			};
static void edit_time1_on_init(struct widget* p_w,struct widget* p_parent)
{
		struct widget_edit *p_e=(struct widget_edit *)p_w;
	edit_set_initnum(p_e,s_time_params.item[1].period/10);
}
void edit_time1_on_key(struct widget* p_w,struct key_event* p_keyEvent)
{	
	struct widget_edit* p_e= ((struct widget_edit*)p_w);
	if(p_keyEvent->key_action==KEY_PRESS)
	{
		switch(p_keyEvent->modify_val)
		{
			case WIDGET_EDIT_VALUE_UP:
				if(0==(p_e->state & EDIT_STATE_EDITMODE))
					return;
				if(edit_get_num(p_e)>MAX_TIME/10)
					edit_set_num(p_e,MAX_TIME/10);
			break;
			case WIDGET_EDIT_VALUE_DOWN:
				if(0==(p_e->state & EDIT_STATE_EDITMODE))
					return;
				if(edit_get_num(p_e)>MAX_TIME/10)
					edit_set_num(p_e,0);
		}
	}
}
/////��Ƶ�༭����5��edit///
void edit_freq2_on_key(struct widget* p_w,struct key_event* p_keyEvent);
static void edit_freq2_on_init(struct widget* p_b,struct widget* p_parent);
static const struct widget_handler _edit_freq2_handler={edit_freq2_on_key,NULL,NULL,edit_freq2_on_init,NULL,NULL,NULL};
static struct widget_edit _edit_freq2={
																			{
																				(struct widget*)&_form_edit,//��widget
																				NULL,//��һ����widget
																				(struct widget*)&_edit_time1,//last widgetͬһ��ε�widget�����һ��˫������
																				(struct widget*)&_edit_time2,//next widget
																				&edit_handler,//button widget�����¼�
																				NULL,//����
																				&font1212,
																				32,//��ʼ��x��λ��
																				26,//��ʼ��y��λ��
																				EDIT_WIDTH_2,//�ؼ����
																				EDIT_HEIGHT,//�ؼ��߶�
																				LCD_COLOR_WHITE,//����ɫ
																				LCD_COLOR_BLACK,//������ɫ
																				{
																					WIDGET_HAS_FOCUS|WIDGET_ENABLED,//��ʼ������
																					WIDGET_STYLE_UNDERLINE,//�߿���
																					1,1,//�ּ�࣬�м��
																				},
																			},
																			&_edit_freq2_handler,
																			EDIT_NUMBER,	
																			2																			
																			};
static void edit_freq2_on_init(struct widget* p_w,struct widget* p_parent)
{
		struct widget_edit *p_e=(struct widget_edit *)p_w;
	edit_set_initnum(p_e,s_time_params.item[2].work_freq/100);
}
void edit_freq2_on_key(struct widget* p_w,struct key_event* p_keyEvent)
{	
	struct widget_edit* p_e= ((struct widget_edit*)p_w);
	if(p_keyEvent->key_action==KEY_PRESS)
	{
		switch(p_keyEvent->modify_val)
		{
			case WIDGET_EDIT_VALUE_UP:
				if(0==(p_e->state & EDIT_STATE_EDITMODE))
					return;
				if(edit_get_num(p_e)>MAX_WORK_FREQ_MAX_VAL/100)
					edit_set_num(p_e,MAX_WORK_FREQ_MAX_VAL/100);
			break;
			case WIDGET_EDIT_VALUE_DOWN:
				if(0==(p_e->state & EDIT_STATE_EDITMODE))
					return;
				if(edit_get_num(p_e)>MAX_WORK_FREQ_MAX_VAL/100)
					edit_set_num(p_e,0);
		}
	}
}
/////��6��edit,��Ƶʱ��///
void edit_time2_on_key(struct widget* p_w,struct key_event* p_keyEvent);
static void edit_time2_on_init(struct widget* p_b,struct widget* p_parent);
static const struct widget_handler _edit_time2_handler={edit_time2_on_key,NULL,NULL,edit_time2_on_init,NULL,NULL,NULL};
static struct widget_edit _edit_time2={
																			{
																				(struct widget*)&_form_edit,//��widget
																				NULL,//��һ����widget
																				(struct widget*)&_edit_freq2,//last widgetͬһ��ε�widget�����һ��˫������
																				(struct widget*)&_edit_freq_door,//next widget
																				&edit_handler,//button widget�����¼�
																				NULL,//����
																				&font1212,
																				94,//��ʼ��x��λ��
																				26,//��ʼ��y��λ��
																				EDIT_WIDTH,//�ؼ����
																				EDIT_HEIGHT,//�ؼ��߶�
																				LCD_COLOR_WHITE,//����ɫ
																				LCD_COLOR_BLACK,//������ɫ
																				{
																					WIDGET_HAS_FOCUS|WIDGET_ENABLED,//��ʼ������
																					WIDGET_STYLE_UNDERLINE,//�߿���
																					1,1,//�ּ�࣬�м��
																				},
																			},
																			&_edit_time2_handler,
																			EDIT_NUMBER,	
																			4
																			};
static void edit_time2_on_init(struct widget* p_w,struct widget* p_parent)
{
		struct widget_edit *p_e=(struct widget_edit *)p_w;
	edit_set_initnum(p_e,s_time_params.item[2].period/10);
}
void edit_time2_on_key(struct widget* p_w,struct key_event* p_keyEvent)
{	
	struct widget_edit* p_e= ((struct widget_edit*)p_w);
	if(p_keyEvent->key_action==KEY_PRESS)
	{
		switch(p_keyEvent->modify_val)
		{
			case WIDGET_EDIT_VALUE_UP:
				if(0==(p_e->state & EDIT_STATE_EDITMODE))
					return;
				if(edit_get_num(p_e)>MAX_TIME/10)
					edit_set_num(p_e,MAX_TIME/10);
			break;
			case WIDGET_EDIT_VALUE_DOWN:
				if(0==(p_e->state & EDIT_STATE_EDITMODE))
					return;
				if(edit_get_num(p_e)>MAX_TIME/10)
					edit_set_num(p_e,0);
		}
	}
}
/////��7��edit������Ƶ��///
void edit_freq_door_on_key(struct widget* p_w,struct key_event* p_keyEvent);
static void edit_freq_door_on_init(struct widget* p_b,struct widget* p_parent);
static const struct widget_handler _edit_freq_door_handler={edit_freq_door_on_key,
																														NULL,NULL,edit_freq_door_on_init,NULL,NULL,NULL};
static struct widget_edit _edit_freq_door;//��Ƶ����
static struct widget_edit _edit_freq_door={
																			{
																				(struct widget*)&_form_edit,//��widget
																				NULL,//��һ����widget
																				(struct widget*)&_edit_time2,//last widgetͬһ��ε�widget�����һ��˫������
																				(struct widget*)&_edit_time_door,//next widget
																				&edit_handler,//button widget�����¼�
																				NULL,//����
																				&font1212,
																				32,//��ʼ��x��λ��
																				39,//��ʼ��y��λ��
																				EDIT_WIDTH_2,//�ؼ����
																				EDIT_HEIGHT,//�ؼ��߶�
																				LCD_COLOR_WHITE,//����ɫ
																				LCD_COLOR_BLACK,//������ɫ
																				{
																					WIDGET_HAS_FOCUS|WIDGET_ENABLED,//��ʼ������
																					WIDGET_STYLE_UNDERLINE,//�߿���
																					1,1,//�ּ�࣬�м��
																				},
																			},
																			&_edit_freq_door_handler,
																			EDIT_NUMBER,	
																			2																			
																			};
static void edit_freq_door_on_init(struct widget* p_w,struct widget* p_parent)
{
		struct widget_edit *p_e=(struct widget_edit *)p_w;
	edit_set_initnum(p_e,s_time_params.item[DOOR_FREQ_INDEX].work_freq/100);
}
void edit_freq_door_on_key(struct widget* p_w,struct key_event* p_keyEvent)
{	
	struct widget_edit* p_e= ((struct widget_edit*)p_w);
	if(p_keyEvent->key_action==KEY_PRESS)
	{
		switch(p_keyEvent->modify_val)
		{
			case WIDGET_EDIT_VALUE_UP:
				if(0==(p_e->state & EDIT_STATE_EDITMODE))
					return;
				if(edit_get_num(p_e)>MAX_WORK_FREQ_MAX_VAL/100)
					edit_set_num(p_e,MAX_WORK_FREQ_MAX_VAL/100);
			break;
			case WIDGET_EDIT_VALUE_DOWN:
				if(0==(p_e->state & EDIT_STATE_EDITMODE))
					return;
				if(edit_get_num(p_e)>MAX_WORK_FREQ_MAX_VAL/100)
					edit_set_num(p_e,0);
		}
	}
}
/////��8��edit,����ʱ��///
void edit_time_door_on_key(struct widget* p_w,struct key_event* p_keyEvent);
static void edit_time_door_on_init(struct widget* p_b,struct widget* p_parent);
static const struct widget_handler _edit_time_door_handler={edit_time_door_on_key,
NULL,NULL,edit_time_door_on_init,NULL,NULL,NULL};
static struct widget_edit _edit_time_door;
static struct widget_edit _edit_time_door={
																			{
																				(struct widget*)&_form_edit,//��widget
																				NULL,//��һ����widget
																				(struct widget*)&_edit_freq_door,//last widgetͬһ��ε�widget�����һ��˫������
																				(struct widget*)&_btn_set,//next widget
																				&edit_handler,//button widget�����¼�
																				NULL,//����
																				&font1212,
																				94,//��ʼ��x��λ��
																				39,//��ʼ��y��λ��
																				EDIT_WIDTH,//�ؼ����
																				EDIT_HEIGHT,//�ؼ��߶�
																				LCD_COLOR_WHITE,//����ɫ
																				LCD_COLOR_BLACK,//������ɫ
																				{
																					WIDGET_HAS_FOCUS|WIDGET_ENABLED,//��ʼ������
																					WIDGET_STYLE_UNDERLINE,//�߿���
																					1,1,//�ּ�࣬�м��
																				},
																			},
																			&_edit_time_door_handler,
																			EDIT_NUMBER,	
																			4
																			};
static void edit_time_door_on_init(struct widget* p_w,struct widget* p_parent)
{
		struct widget_edit *p_e=(struct widget_edit *)p_w;
	edit_set_initnum(p_e,s_time_params.item[DOOR_FREQ_INDEX].period/10);
}
void edit_time_door_on_key(struct widget* p_w,struct key_event* p_keyEvent)
{	
	struct widget_edit* p_e= ((struct widget_edit*)p_w);
	if(p_keyEvent->key_action==KEY_PRESS)
	{
		switch(p_keyEvent->modify_val)
		{
			case WIDGET_EDIT_VALUE_UP:
				if(0==(p_e->state & EDIT_STATE_EDITMODE))
					return;
				if(edit_get_num(p_e)>MAX_TIME/10)
					edit_set_num(p_e,MAX_TIME/10);
			break;
			case WIDGET_EDIT_VALUE_DOWN:
				if(0==(p_e->state & EDIT_STATE_EDITMODE))
					return;
				if(edit_get_num(p_e)>MAX_TIME/10)
					edit_set_num(p_e,0);
		}
	}
}
///////
//set
///////
static void btn_set_on_key(struct widget* p_b,key_event_t* p_keyEvent);
struct widget* btn_set_on_focus(struct widget* p_b);
void btn_set_on_init(struct widget* p_b,struct widget* p_parent);
static const struct widget_handler _set_button_handler={btn_set_on_key,NULL,NULL,NULL,NULL,NULL,NULL};
static struct widget_button _btn_set={
																			{(struct widget*)&_form_edit,//��widget
																				NULL,//��һ����widget
																				(struct widget*)&_edit_time_door,//last widgetͬһ��ε�widget�����һ��˫������
																				(struct widget*)&_btn_cancel,//next widget
																				&button_handler,//button widget�����¼�
																				NULL,//����
																				&font1212,
																				0,//��ʼ��x��λ��
																				FORM_HEIGHT-BUTTON_HEIGHT,//��ʼ��y��λ��
																				BUTTON_WIDTH,//�ؼ����
																				BUTTON_HEIGHT,//�ؼ��߶�
																				LCD_COLOR_WHITE,//����ɫ
																				LCD_COLOR_BLACK,//������ɫ
																				{
																				WIDGET_HAS_FOCUS|WIDGET_ENABLED,//��ʼ������
																				WIDGET_STYLE_BORDER,//�߿���
																				1,1,//�ּ�࣬�м��
																				},
																			},
																			&_set_button_handler,
																			GET_WSTR(STR_OK),																			
																			};

static void btn_set_on_key(struct widget* p_b,struct key_event* p_key_event)
{
	int freq[4],time[4];
	if(p_key_event->key_action==KEY_RELEASE)
	{
		switch(p_key_event->modify_val)
		{
			case BUTTON_ENTER:
			{//�洢ʱ����0.1�룬Ƶ����0.01Hz����Ҫת��
				freq[0]=edit_get_num(&_edit_freq0)*100;
				freq[1]=edit_get_num(&_edit_freq1)*100;
				freq[2]=edit_get_num(&_edit_freq2)*100;
				freq[3]=edit_get_num(&_edit_freq_door)*100;
				time[0]=edit_get_num(&_edit_time0)*10;
				time[1]=edit_get_num(&_edit_time1)*10;
				time[2]=edit_get_num(&_edit_time2)*10;
				time[3]=edit_get_num(&_edit_time_door)*10;
				
				s_time_params.item[0].work_freq=freq[0];
				s_time_params.item[1].work_freq=freq[1];
				s_time_params.item[2].work_freq=freq[2];
				s_time_params.item[DOOR_FREQ_INDEX].work_freq=freq[3];
				
				s_time_params.item[0].period=time[0];
				s_time_params.item[1].period=time[1];
				s_time_params.item[2].period=time[2];
				s_time_params.item[DOOR_FREQ_INDEX].period=time[3];
				if(!store_time_params(&s_time_params))//�洢Ƶ��
					assert_param(FALSE);	
				widget_destroy((struct widget*)&_form_edit);
				create_main_form();
			}
			break;
		}
	}
}
//struct widget* btn_set_on_focus(struct widget* p_b)
//{
////	if((p_b->attribute & WIDGET_HAS_FOCUS) && (p_b->attribute & WIDGET_ENABLED) && (!(p_b->attribute & WIDGET_HIDED)))
////		return NULL;
////	else
////		return widget_focus((struct widget*)&_btn_set);
//}

//void btn_set_on_init(struct widget* p_b,struct widget* p_parent)
//{

//}


////////
//cancel
/////
static void btn_cancel_on_key(struct widget* p_b,struct key_event* p_key_event);
struct widget* btn_cancel_on_focus(struct widget* p_b);
//static void btn_set_cancel_init(struct widget* p_b,struct widget* p_parent);
static const struct widget_handler _cancel_button_handler={btn_cancel_on_key,NULL,NULL,NULL,NULL,NULL,NULL};
static struct widget_button _btn_cancel={
																			{
																				(struct widget*)&_form_edit,//��widget
																				NULL,//��һ����widget
																				(struct widget*)&_btn_set,//last widgetͬһ��ε�widget�����һ��˫������
																				NULL,//next widget
																				&button_handler,//button widget�����¼�
																				NULL,//����
																				&font1212,
																				(FORM_WIDTH-BUTTON_WIDTH),//��ʼ��x��λ��
																				FORM_HEIGHT-BUTTON_HEIGHT,//��ʼ��y��λ��
																				BUTTON_WIDTH,//�ؼ����
																				BUTTON_HEIGHT,//�ؼ��߶�
																				LCD_COLOR_WHITE,//����ɫ
																				LCD_COLOR_BLACK,//������ɫ
																				{
																				WIDGET_HAS_FOCUS|WIDGET_ENABLED,//��ʼ������
																				WIDGET_STYLE_BORDER,//�߿���
																				1,1,//�ּ�࣬�м��
																				},
																			},
																			&_cancel_button_handler,
																			GET_WSTR(STR_CANCEL),																			
																			};


static void btn_cancel_on_key(struct widget* p_b,struct key_event* p_key_event)
{
	if(p_key_event->key_action==KEY_RELEASE)
	{
		switch(p_key_event->modify_val)
		{
			case BUTTON_ENTER:
				widget_destroy((struct widget*)&_form_edit);
				create_main_form();
			break;
		}
	}
}
/*end of �˵�*/


/* edit Form */
static void form_edit_on_key(struct widget* p_f,struct key_event* p_keyEvent);
static void form_edit_on_tick(struct widget* p_f,int32u ticks);
static void form_edit_on_event(struct widget* p_f,event_id_t event_id,void *p_event);
static void form_edit_on_init(struct widget* p_f,struct widget *p_parent);
static void form_edit_on_destroy(struct widget* p_f);
static void form_edit_on_show(struct widget* p_f);
//struct widget* form_edit_on_focus(struct widget* p_f);
static const struct widget_handler _form_edit_handler={form_edit_on_key,
																												form_edit_on_tick,
																												form_edit_on_event,
																												form_edit_on_init,
																												form_edit_on_destroy,
																												form_edit_on_show,
																												NULL};
static struct widget_form _form_edit={
																			{
																				NULL,//��widget
																				(struct widget *)&_edit_freq0,//��һ����widget
																				NULL,//last widgetͬһ��ε�widget�����һ��˫������
																				NULL,//next widget
																				&form_handler,//button widget�����¼�
																				NULL,//����
																				&font1212,
																				0,//��ʼ��x��λ��
																				0,//��ʼ��y��λ��
																				FORM_WIDTH,//�ؼ����
																				FORM_HEIGHT,//�ؼ��߶�
																				LCD_COLOR_WHITE,//����ɫ
																				LCD_COLOR_BLACK,//������ɫ
																				{
																				WIDGET_ENABLED,//��ʼ������
																				WIDGET_STYLE_NULL,
																				1,1,//�ּ�࣬�м��
																				},
																			},
																			&_form_edit_handler,
																			4,
																			1};

static void form_edit_on_key(struct widget* p_f,struct key_event* p_keyEvent)
{
}
static void form_edit_on_tick(struct widget* p_f,int32u ticks)
{
}
static void form_edit_on_event(struct widget* p_f,event_id_t event_id,void *p_event)
{
}
extern struct lcd mono_mem_lcd;
static void form_edit_on_init(struct widget* p_f,struct widget *p_parent)
{

}
static void form_edit_on_destroy(struct widget* p_f)
{
}
static void form_edit_on_show(struct widget* p_f)
{
	wchar buf[4];
	to_wchar("Hz",buf);
	lcd_draw_line_string(&mono_mem_lcd,0,0,1,&font1212,0x00,0xff,GET_WSTR(STR_SPEED_STEP1));
	lcd_draw_line_string(&mono_mem_lcd,48,0,1,&font1212,0x00,0xff,buf);
	lcd_draw_line_string(&mono_mem_lcd,66,0,1,&font1212,0x00,0xff,GET_WSTR(STR_TIME));
	lcd_draw_line_string(&mono_mem_lcd,0,1,1,&font1212,0x00,0xff,GET_WSTR(STR_SPEED_STEP2));
	lcd_draw_line_string(&mono_mem_lcd,48,1,1,&font1212,0x00,0xff,buf);
	lcd_draw_line_string(&mono_mem_lcd,66,1,1,&font1212,0x00,0xff,GET_WSTR(STR_TIME));
	lcd_draw_line_string(&mono_mem_lcd,0,2,1,&font1212,0x00,0xff,GET_WSTR(STR_SPEED_STEP3));
	lcd_draw_line_string(&mono_mem_lcd,48,2,1,&font1212,0x00,0xff,buf);
	lcd_draw_line_string(&mono_mem_lcd,66,2,1,&font1212,0x00,0xff,GET_WSTR(STR_TIME));
	lcd_draw_line_string(&mono_mem_lcd,0,3,1,&font1212,0x00,0xff,GET_WSTR(STR_DOOR_OPEN));
	lcd_draw_line_string(&mono_mem_lcd,48,3,1,&font1212,0x00,0xff,buf);
	lcd_draw_line_string(&mono_mem_lcd,66,3,1,&font1212,0x00,0xff,GET_WSTR(STR_TIME));
}



//////////////////
extern struct lcd mono_mem_lcd;
void create_edit_form(void)
{
	widget_set_lcd(&mono_mem_lcd);
	widget_create((struct widget*)&_form_edit,NULL);
}

