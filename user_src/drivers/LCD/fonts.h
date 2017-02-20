#ifndef FONTS_H001
#define FONTS_H001
#include "types.h"

#define CH_TO_WCH(x) 

struct font
{
    int16u width;
    int16u height;
		int32u size;//字库数量
		const int8u* ascii_lib;//ascii点阵表
    const int8u* wchar_lib;//宽字符点阵表
		const int16u* wchar_index_table;//宽字符索引表宽字符按字节获取字符点阵,对于不完整的字库，需要使用索引表,索引表内存放的是对应索引文字的编码值
    const int8u* (*get_dots)(struct font* p_this,wchar ch,int8u i);
};

extern struct font font1212;
int16u font_get_length(struct font* p_font,const wchar* str);
void to_wchar(char* str,wchar* wstr);
const int8u* font_get_dots(struct font* p_this,wchar wch,int8u index);
#endif

