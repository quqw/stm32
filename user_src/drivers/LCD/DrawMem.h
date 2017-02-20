#ifndef _DWAR_MEM_H001
#define _DWAR_MEM_H001
#include "lcd.h"
#include "fonts.h"
void draw_mem_pixel(struct lcd* lcd, int16u x_pos, int16u y_pos, int16u color);
void draw_mem_horizon(struct lcd* lcd,int16u x_pos, int16u y_pos, int16u len, int16u color);
void draw_mem_vertical(struct lcd* lcd,int16u x_pos, int16u y_pos, int16u len,int16u color);
//void draw_mem_char(struct lcd* lcd,int16u x0, int16u y0,struct font* p_font, int16u TextColor , int16u BackColor, const char ch);
void draw_mem_char(struct lcd* lcd,int16u x0, int16u y0,struct font* p_font, int16u t_color , int16u b_color, wchar wch);
void draw_mem_bit_map(struct lcd* lcd,int16u x, int16u y,int16u wigth, int16u hight,void* p_map);
//void draw_mem_refresh_creen(struct lcd* lcd);
void draw_mem_clear_screan(struct lcd* lcd,int16u color);
void draw_mem_block(struct lcd* lcd,int16u x, int16u y,int16u width, int16u height,int16u color);

#endif


