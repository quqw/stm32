#include "drawmem.h"
/*����ĺ�����LCD.C���á����ܣ����ڴ��п��ٿռ���Ϊ�Դ棬ֱ�Ӳ����ڴ�ռ�*/
/*�ڴ���0��ʾ��ɫ��1��ʾ��ɫ*/
/*��ɫ����0��ʾ��ɫ��������ʾ��ɫ*/

//void draw_mem_set_color(struct lcd* lcd,int16u TextColor,int16u BackColor)
//{
//	_backColor=BackColor;
//	_textColor=TextColor;
//}
//#define MEM_LCD_COLOR(x)	((x==0)?1:0)
#define MEM_LCD_COLOR(x) (x)
//mem draw���յ��õĶ����������
void draw_mem_pixel(struct lcd* lcd,int16u x_pos, int16u y_pos, int16u color)
{
	vu32* _lcd=PTR_BITADDR(lcd->lcd_buffer,0);
	if(x_pos>=lcd->width || y_pos>=lcd->height)
		return;
	if(lcd->attribute & LCD_ATTR_ROT180)//��ת180��
	{
		_lcd[(lcd->height-y_pos-1)*lcd->width+(lcd->width-x_pos-1)]=MEM_LCD_COLOR(color);
	}
	else
		_lcd[y_pos*lcd->width+x_pos]=MEM_LCD_COLOR(color);
}

/*��ˮƽ��*/
void draw_mem_horizon(struct lcd* lcd,int16u x_pos, int16u y_pos, int16u len, int16u color)
{
	int i;
	for(i=0;i<len;i++)
		lcd->draw_pixel(lcd,x_pos+i,y_pos,color);
}

/*����ֱ��*/
void draw_mem_vertical(struct lcd* lcd,int16u x_pos, int16u y_pos, int16u len,int16u color)
{
	int i;
	for(i=0;i<len;i++)
		lcd->draw_pixel	(lcd,x_pos,y_pos+i,color);
}
	
//void draw_mem_char(struct lcd* lcd,int16u x0, int16u y0,struct font* p_font, int16u t_color , int16u b_color, const char ch)
//{
//  int32u i,j;
//	int16u x=0,y=0;
//	char temp_c;
//  vu32 *bitMap= PTR_BITADDR(&temp_c,0);
//	for(i=0;i<p_font->height*p_font->width/8;i++)//һ���ֽڴ��8����
//	{
//      temp_c=*(p_font->get_dots(p_font,ch,i));
//	    for(j=0;j<8;j++)
//	    {
//				lcd->draw_pixel(lcd,x+x0,y+y0,bitMap[j]?t_color:b_color);
//				x++;
//				if(x>=p_font->width)
//				{
//						y++;
//						x=0;
//				}
//	    }
//	}
//}

//���ַ���խ�ַ�һ����
void draw_mem_char(struct lcd* lcd,int16u x0, int16u y0,struct font* p_font, int16u t_color , int16u b_color, wchar wch)
{
  int32u i,j,width,dot_bytes;
	int16u x=0,y=0;
	char temp_c;
  vu32 *bitMap= PTR_BITADDR(&temp_c,0);
	if(wch>0xff)
	{
		width=p_font->width;
	}
	else
	{
		width=p_font->width/2;
	}
	dot_bytes=p_font->height*width/8;
	for(i=0;i<dot_bytes;i++)//
	{
      temp_c=*(p_font->get_dots(p_font,wch,i));//��ͬ���ֿⶼ���Ե������draw_mem_char����Ϊ��ȡ�������ֿ�Ĺ���get_dots
	    for(j=0;j<8;j++)//һ���ֽ�8����
	    {
				lcd->draw_pixel(lcd,x+x0,y+y0,bitMap[j]?t_color:b_color);
				x++;
				if(x>=width)//����
				{
						y++;
						x=0;
				}
	    }
	}
}
void draw_mem_bit_map(struct lcd* lcd,int16u x, int16u y,int16u width, int16u height,void* p_map)
{
//	int16u i,j;
//	for(i=0;i<height;i++)
//		for(j=0;j<width;j++)
//		{
//			drvDrawMemPixel(j+x,i+y,p_map[]);
//		}
}
void draw_mem_block(struct lcd* lcd,int16u x, int16u y,int16u width, int16u height,int16u color)
{
	int16u i,j;
	for(i=0;i<height;i++)
		for(j=0;j<width;j++)
		{
			lcd->draw_pixel(lcd,j+x,i+y,color);
		}
}
void draw_mem_clear_screan(struct lcd* lcd,int16u color)
{
	memset(lcd->lcd_buffer,MEM_LCD_COLOR(color),lcd->width *lcd->height/8);
}



