#include "fonts.h"
/*说明
不管什么大小的字库，存放时候按照经凑方式排布，不应该有0作为空余点阵，否则会被认为是空余输出
*/


/*按字节获取字符点阵,ascii码会自动获取ascii表数据，宽字符自动获取用户自定义文字点阵*/
//index为字节的索引
const int8u* font_get_dots(struct font* p_this,wchar wch,int8u index)
{
	int i;
	char ch;
	if(wch<=0xff)
	{
		ch=(char)wch;
		if(!isprint(ch) || index>p_this->width * p_this->height/16)//宽高都是按照宽字符集来算的，但ascii实际只占半个，故除以16
			return (p_this->ascii_lib);
		return &(p_this->ascii_lib[(ch-0x20)*p_this->width*p_this->height/16+index]); 
	}
	else if(!p_this->wchar_index_table || !p_this->ascii_lib )
	{
		return (p_this->ascii_lib);
	}
	else 
	{
		if(index>p_this->width*p_this->height/8)
        return (p_this->ascii_lib);
		i=0;
		while(p_this->wchar_index_table[i])
		{
			if(wch==p_this->wchar_index_table[i])
			{
				return &(p_this->wchar_lib[i*p_this->width*p_this->height/8+index]); 
			}
			i++;
		}
		return (p_this->ascii_lib);
	}

}


int16u font_get_length(struct font* font,const wchar* str)
{
	int width=0,index=0;//index为char的索引
	
	while(TRUE)
	{
		if(str[index]==0)
			return width;
		if(str[index]>0xff)
		{
			width+=font->width;
		}
		else
		{
			width+=font->width/2;
		}
		index++;
	}
}

void to_wchar(char* str,wchar* wstr)
{
	int index=0;
	while(str[index]!=0)
	{
		wstr[index]=str[index];
		index++;
	}
	wstr[index]=0;
}

