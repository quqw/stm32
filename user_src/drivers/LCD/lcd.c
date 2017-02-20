#include "lcd.h"

//lcd逻辑层
//lcd需要兼容内存刷新的单色模式和直接写lcd寄存器的彩色模式
//这一层的功能就是调用这两种模式的公共接口完成一系列图形操作

void lcd_fefresh(struct lcd* lcd)
{
    if(lcd==NULL)
        return;
	lcd->refresh_screen();
}

void lcd_clear(struct lcd* lcd,u16 color)
{
    lcd->draw_block(lcd,0,0,lcd->width,lcd->height,color);
}

void lcd_draw_pixel(struct lcd* lcd,int16s x_pos, int16s y_pos, u16 color)
{
	if(x_pos<0 || y_pos<0 ||lcd==NULL)
		return;
    lcd->draw_pixel(lcd,x_pos,y_pos, color);
}

void lcd_draw_horizon(struct lcd* lcd,int16s x_pos, int16s y_pos, int16u len, u16 color)
{
	if(lcd==NULL || y_pos<0)
		return;
    if(x_pos<0)
        x_pos=0;
	lcd->draw_horizon( lcd,x_pos, y_pos, len,color);
}

void lcd_draw_vertical(struct lcd* lcd,int16s x_pos, int16s y_pos, int16u len, u16 color)
{
	if(lcd==NULL || x_pos<0)
		return;
    if(y_pos<0)
        y_pos=0;
	lcd->draw_vertical( lcd,x_pos, y_pos, len,color);
}
//把宽字符和普通assic同时处理
void lcd_draw_char(struct lcd* lcd,int16s x_pos, int16s y_pos,struct font* font, int16u TextColor , int16u BackColor, wchar ch)
{
	if(x_pos<0 || y_pos<0 ||lcd==NULL)
		return;
	lcd->draw_char(lcd,x_pos,y_pos,font,TextColor,BackColor,ch);
}
//...需要修改加入字间距
//把宽字符和普通assic同时处理
void lcd_draw_string(struct lcd* lcd,int16s x_pos, int16s y_pos,
	struct font* p_font, uint16_t TextColor , uint16_t BackColor,const wchar* p_string)
{
	
	int i=0;
	if(!p_string)
		return;
	while(p_string[i])	{
		lcd_draw_char(lcd,x_pos,y_pos,p_font,TextColor,BackColor,p_string[i]);
		if(p_string[i]<0xff)
			x_pos+=p_font->width/2;
		else
			x_pos+=p_font->width;
		i++;
	}
}
void lcd_draw_center_string(struct lcd* lcd,int16s x_pos, int16s y_pos,int16u width, int16u height,
	struct font* p_font, uint16_t TextColor , uint16_t BackColor,const wchar* p_string)
{
	if(!p_string)
		return;
	x_pos=POS_CENTER(x_pos,x_pos+width-1,font_get_length(p_font,p_string));
	y_pos=POS_CENTER(y_pos,y_pos+height-1,p_font->height);
	lcd_draw_string(lcd,x_pos,y_pos,p_font,TextColor,BackColor,p_string);
}

//从右边开始往左写字
void lcd_draw_right_string(struct lcd* lcd,int16s x_pos, int16s y_pos,int16u width, int16u height,
	struct font* p_font, uint16_t TextColor , uint16_t BackColor,const wchar* p_string)
{
	if(!p_string)
		return;
	x_pos=x_pos+width-1-font_get_length(p_font,p_string);
	lcd_draw_string(lcd,x_pos,y_pos,p_font,TextColor,BackColor,p_string);
}

//按行写文字
void lcd_draw_line_string(struct lcd* lcd,int16s x_pos, int16u line,int16u line_space,
	struct font* p_font, uint16_t TextColor , uint16_t BackColor,const wchar* p_string)
	{
		lcd_draw_string(lcd,x_pos,line*(p_font->height+line_space),
			p_font,TextColor , BackColor,p_string);
	}



//画居中字体中的某一个字
void lcd_draw_center_string_char(struct lcd* lcd,int16s index,int16s x_pos, int16s y_pos,int16u width, int16u height,
	struct font* p_font, uint16_t TextColor , uint16_t BackColor,const wchar* p_string)
{
	if(!p_string)
		return;
	x_pos=POS_CENTER(x_pos,x_pos+width-1,font_get_length(p_font,p_string))+index*p_font->width/2;
	y_pos=POS_CENTER(y_pos,y_pos+height-1,p_font->height);
	lcd_draw_block(lcd,x_pos-1, y_pos,p_font->width/2+2, height,BackColor);//往左边多画一格
	lcd_draw_char(lcd,x_pos,y_pos,p_font,TextColor,BackColor,p_string[index]);
}

void lcd_draw_line_center_string(struct lcd* lcd, int16s line,int16u line_space,
	struct font* p_font, uint16_t TextColor , uint16_t BackColor,const wchar* p_string)
{
	lcd_draw_center_string(lcd,0,line*(p_font->height+line_space),lcd->width, p_font-> height,
		p_font, TextColor , BackColor,p_string);
}

void lcd_draw_line_right_string(struct lcd* lcd, int16s line,int16u line_space,
	struct font* p_font, uint16_t TextColor , uint16_t BackColor,const wchar* p_string)
{
	lcd_draw_right_string(lcd,0,line*(p_font->height+line_space),lcd->width, p_font-> height,
		p_font, TextColor , BackColor,p_string);
}
void lcd_draw_block(struct lcd* lcd,int16s x_pos, int16s y_pos,int16u width, int16u height,int16u Color)
{
    if(lcd==NULL)
        return;
	if(x_pos<0)
	{
		width=x_pos+x_pos;
		x_pos=0;
	}
	if(y_pos<0)
	{
		height+=y_pos;
		y_pos=0;
	}
	lcd->draw_block(lcd,x_pos,y_pos,width,height,Color);
}

void lcd_draw_line(struct lcd* lcd,int16s x0, int16s y0, int16s x1, int16s y1,u16 color)
{
 	int16s x,y;
 	int16s dx;// = abs(x1 - x0);
 	int16s dy;// = abs(y1 - y0);

    if(lcd==NULL)
        return;
	if(y0==y1)
	{
		if(x0<=x1)
			x=x0;
		else
		{
			x=x1;
			x1=x0;
		}
  		while(x <= x1)
  		{
   			lcd_draw_pixel(lcd,x,y0,color);
   			x++;
  		}
  		return;
	}
	else if(y0>y1)
		dy=y0-y1;
	else
		dy=y1-y0;
 	if(x0==x1)
	{
		if(y0<=y1)
			y=y0;
		else
		{
			y=y1;
			y1=y0;
		}
  		while(y <= y1)
  		{
   			lcd_draw_pixel(lcd,x0,y,color);
   			y++;
  		}
  		return;
	}
	else if(x0 > x1)
 	{
		dx=x0-x1;
  		x = x1;
  		x1 = x0;
  		y = y1;
  		y1 = y0;
 	}
 	else
 	{
		dx=x1-x0;
  		x = x0;
  		y = y0;
 	}

 	if(dx == dy)
 	{
  		while(x <= x1)
  		{
   			x++;
			if(y>y1)
				y--;
			else
   				y++;
   			lcd_draw_pixel(lcd,x,y,color);
  		}
 	}
 	else
 	{
 		lcd_draw_pixel(lcd,x, y, color);
  		if(y < y1)
  		{
   			if(dx > dy)
   			{
    			int16s p = dy * 2 - dx;
    			int16s twoDy = 2 * dy;
    			int16s twoDyMinusDx = 2 * (dy - dx);
    			while(x < x1)
    			{
     				x++;
     				if(p < 0)
      					p += twoDy;
     				else
     				{
      					y++;
      					p += twoDyMinusDx;
     				}
     				lcd_draw_pixel(lcd,x, y,color);
    			}
   			}
   			else
   			{
    			int16s p = dx * 2 - dy;
    			int16s twoDx = 2 * dx;
    			int16s twoDxMinusDy = 2 * (dx - dy);
    			while(y < y1)
    			{
     				y++;
     				if(p < 0)
      					p += twoDx;
     				else
     				{
      					x++;
      					p+= twoDxMinusDy;
     				}
     				lcd_draw_pixel(lcd,x, y, color);
    			}
   			}
  		}
  		else
  		{
   			if(dx > dy)
   			{
    			int16s p = dy * 2 - dx;
    			int16s twoDy = 2 * dy;
	    		int16s twoDyMinusDx = 2 * (dy - dx);
    			while(x < x1)
    			{
     				x++;
     				if(p < 0)
    	  				p += twoDy;
     				else
     				{
      					y--;
	      				p += twoDyMinusDx;
    	 			}
     				lcd_draw_pixel(lcd,x, y,color);
    			}
   			}
	   		else
   			{
    			int16s p = dx * 2 - dy;
    			int16s twoDx = 2 * dx;
	    		int16s twoDxMinusDy = 2 * (dx - dy);
    			while(y1 < y)
    			{
     				y--;
     				if(p < 0)
	     			{
    	  				p += twoDx;
     				}
     				else
     				{
      					x++;
	      				p+= twoDxMinusDy;
    	 			}
     				lcd_draw_pixel(lcd,x, y,color);
    			}
   			}
  		}
 	}
}


