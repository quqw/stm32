#ifndef WIDGET_EDIT_H001
#define WIDGET_EDIT_H001
#include "gui/widget.h"

//����
#define	MAX_EDIT_LENGTH	16
#define 	EDIT_NUMBER 0x01//�༭����
#define 	EDIT_ALPHABET 0x02//�༭��ĸ


//״̬
#define EDIT_STATE_NULL			0
#define EDIT_STATE_EDITMODE 0x01//�༭ģʽ


typedef struct widget_edit
{
	const struct widget base;
	const struct widget_handler *p_handlers;//��ͬ��edit�����ж���
	const int32u attribute;//����
	const int8u length;
	int8s focus_index;
	int16u state;//״̬
	int8s init_text[MAX_EDIT_LENGTH];
	int8s text[MAX_EDIT_LENGTH];
}widget_edit_t;

extern const struct widget_handler edit_handler;
void edit_get_text(struct widget_edit* p_e,char* text);
int edit_get_num(struct widget_edit* p_e);
void edit_set_num(struct widget_edit* p_e,int num);
void edit_set_initnum(struct widget_edit* p_e,int num);




#endif


