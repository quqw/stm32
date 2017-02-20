#include "fonts.h"
/*˵��
����ʲô��С���ֿ⣬���ʱ���վ��շ�ʽ�Ų�����Ӧ����0��Ϊ������󣬷���ᱻ��Ϊ�ǿ������
*/


/*���ֽڻ�ȡ�ַ�����,ascii����Զ���ȡascii�����ݣ����ַ��Զ���ȡ�û��Զ������ֵ���*/
//indexΪ�ֽڵ�����
const int8u* font_get_dots(struct font* p_this,wchar wch,int8u index)
{
	int i;
	char ch;
	if(wch<=0xff)
	{
		ch=(char)wch;
		if(!isprint(ch) || index>p_this->width * p_this->height/16)//��߶��ǰ��տ��ַ�������ģ���asciiʵ��ֻռ������ʳ���16
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
	int width=0,index=0;//indexΪchar������
	
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

