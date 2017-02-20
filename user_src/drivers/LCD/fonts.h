#ifndef FONTS_H001
#define FONTS_H001
#include "types.h"

#define CH_TO_WCH(x) 

struct font
{
    int16u width;
    int16u height;
		int32u size;//�ֿ�����
		const int8u* ascii_lib;//ascii�����
    const int8u* wchar_lib;//���ַ������
		const int16u* wchar_index_table;//���ַ���������ַ����ֽڻ�ȡ�ַ�����,���ڲ��������ֿ⣬��Ҫʹ��������,�������ڴ�ŵ��Ƕ�Ӧ�������ֵı���ֵ
    const int8u* (*get_dots)(struct font* p_this,wchar ch,int8u i);
};

extern struct font font1212;
int16u font_get_length(struct font* p_font,const wchar* str);
void to_wchar(char* str,wchar* wstr);
const int8u* font_get_dots(struct font* p_this,wchar wch,int8u index);
#endif

