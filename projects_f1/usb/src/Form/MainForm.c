#include "includes.h"
#include "gui/widget_button.h"
#include "gui/widget_form.h"
#include "mainform.h"
#include "string.h"
#include "apps/motor-control.h"
#include "editform.h"
#define MODE_MOVE   1
#define MODE_NAVI   2


#define FORM_WIDTH	128
#define FORM_HEIGHT	64


#define MENUFORM_X POS_CENTER(0,SCREEN_WIDTH-1,MENUFORM_WIDTH)
#define MENUFORM_Y POS_CENTER(0,SCREEN_HEIGHT-1-12,MENUFORM_HEIGHT)
static struct widget_form _form_main;//������

/* main Form */
void form_main_on_key(struct widget* p_f,struct key_event* p_keyEvent);
void form_main_on_tick(struct widget* p_f,int32u ticks);
void form_main_on_event(struct widget* p_f,event_id_t event_id,void *p_event);
void form_main_on_init(struct widget* p_f,struct widget *p_parent);
void form_main_on_destroy(struct widget* p_f);
void form_main_on_show(struct widget* p_f);
//struct widget* form_main_on_focus(struct widget* p_f);
static const struct widget_handler _form_main_handler={form_main_on_key,
																												form_main_on_tick,
																												NULL,
																												form_main_on_init,
																												form_main_on_destroy,
																												form_main_on_show,
																												NULL};
static struct widget_form _form_main={
																			{
																				NULL,//��widget
																				NULL,//��һ����widget
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
																			&_form_main_handler,
																			4,
																			1};

void form_main_on_key(struct widget* p_w,struct key_event* p_key_event)
{
	if(p_key_event->key_action==KEY_RELEASE)
	{
		switch(p_key_event->modify_val)
		{
			case BUTTON_ENTER:
				if(p_key_event->key_value==(BUTTON_ESC))
				{
					widget_destroy((struct widget*)&_form_main);
					create_edit_form();
				}
			break;
		}
	}
}
static int _ticks=0;
void form_main_on_tick(struct widget* p_f,int32u ticks)
{
	_ticks+=ticks;
	if(_ticks>=100)
	{
		widget_refresh(p_f);
		_ticks=0;
	}
	
}
void form_main_on_event(struct widget* p_f,event_id_t event_id,void *p_event)
{
	
}
extern struct lcd mono_mem_lcd;
void form_main_on_init(struct widget* p_f,struct widget *p_parent)
{

}
void form_main_on_destroy(struct widget* p_f)
{
}
void form_main_on_show(struct widget* p_f)
{
	wchar buf[32];
	char str[8];
	
	lcd_draw_center_string(&mono_mem_lcd,0,6,128,12,&font1212,0x00,0xff,GET_WSTR(STR_SYS_NAME));
	to_wchar(SYSTEM_CERSION,buf);
	lcd_draw_center_string(&mono_mem_lcd,0,20,128,12,&font1212,0x00,0xff,buf);
	
	//״̬������  ��ʱ��xx
	lcd_draw_line_string(&mono_mem_lcd,0,3,1,&font1212,0x00,0xff,GET_WSTR(STR_STATUS));
	to_wchar(":",buf);
	lcd_draw_line_string(&mono_mem_lcd,24,3,1,&font1212,0x00,0xff,buf);
//	if(board_door_open())
//	{
//		lcd_draw_line_string(&mono_mem_lcd,30,3,1,&font1212,0x00,0xff,GET_WSTR(STR_DOOR_OPEN));
////		sprintf(str,"%d",s_time_params.item[DOOR_FREQ_INDEX].work_freq);
////		to_wchar(str,buf);
////		lcd_draw_line_string(&mono_mem_lcd,94,0,1,&font1212,0x00,0xff,buf);
//	}
//	else
//	{
		switch(CURRENT_WORK_INDEX)
		{
			case 0:
				lcd_draw_line_string(&mono_mem_lcd,30,3,1,&font1212,0x00,0xff,GET_WSTR(STR_SPEED_STEP1));
				break;
			case 1:
				lcd_draw_line_string(&mono_mem_lcd,30,3,1,&font1212,0x00,0xff,GET_WSTR(STR_SPEED_STEP2));
				break;
			case 2:
				lcd_draw_line_string(&mono_mem_lcd,30,3,1,&font1212,0x00,0xff,GET_WSTR(STR_SPEED_STEP3));
				break;
			case DOOR_FREQ_INDEX:
				lcd_draw_line_string(&mono_mem_lcd,30,3,1,&font1212,0x00,0xff,GET_WSTR(STR_DOOR_OPEN));
				break;
			case IDLE_FREQ_INDEX:
				lcd_draw_line_string(&mono_mem_lcd,30,3,1,&font1212,0x00,0xff,GET_WSTR(STR_IDLE));
				break;
			case -1:
				lcd_draw_line_string(&mono_mem_lcd,30,3,1,&font1212,0x00,0xff,GET_WSTR(STR_STOP));	
		}
	//}
	lcd_draw_line_string(&mono_mem_lcd,72,3,1,&font1212,0x00,0xff,GET_WSTR(STR_TIME_GONE));
	if(CURRENT_WORK_INDEX==IDLE_FREQ_INDEX||CURRENT_WORK_INDEX==-1)
	{
		sprintf(str,":--");//0.1s
	}
	else
	{
		sprintf(str,":%d",s_work_state.period/10+1);//0.1s
	}
	to_wchar(str,buf);
	lcd_draw_line_string(&mono_mem_lcd,96,3,1,&font1212,0x00,0xff,buf);	
	

	
	//��ʱ��xx Ƶ�ʣ�123
	lcd_draw_line_string(&mono_mem_lcd,0,4,1,&font1212,0x00,0xff,GET_WSTR(STR_TIME_TOTAL));
	if(CURRENT_WORK_INDEX==IDLE_FREQ_INDEX||CURRENT_WORK_INDEX==-1)
	{
		sprintf(str,":--");//0.1s
	}
	else
	{
		sprintf(str,":%d",CURRENT_PERIOD_TIME/10);//0.1s
	}
	to_wchar(str,buf);
	lcd_draw_line_string(&mono_mem_lcd,24,4,1,&font1212,0x00,0xff,buf);

	lcd_draw_line_string(&mono_mem_lcd,72,4,1,&font1212,0x00,0xff,GET_WSTR(STR_FREQ));
	if(CURRENT_WORK_INDEX==IDLE_FREQ_INDEX)
	{
		sprintf(str,":%d",s_work_params.idle_freq/100);//0.01hz
	}
	else if(CURRENT_WORK_INDEX==-1)
		sprintf(str,"0");
	else
	{
		sprintf(str,":%d",CURRENT_FREQ/100);//0.01Hz
	}
	to_wchar(str,buf);
	lcd_draw_line_string(&mono_mem_lcd,96,4,1,&font1212,0x00,0xff,buf);
	
	
	#ifdef DEBUG_GUI
	lcd_fefresh(&mono_mem_lcd);
	#endif
}



//////////////////
extern struct lcd mono_mem_lcd;
void create_main_form(void)
{
	widget_create((struct widget*)&_form_main,NULL);
	//form_main_on_show((struct widget*)&_form_main);
}

