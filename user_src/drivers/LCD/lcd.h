#ifndef LCD_H
#define LCD_H
#include "types.h"
#include "fonts.h"
#include "platform/gpio.h"
#include "drivers/led.h"
//#define DEBUG_GUI
#define POS_CENTER(start,end,len)	(((start+end+1)>>1)-(len>>1))
extern struct lcd mono_mem_lcd;

#define LCD_COLOR_BLACK 0x0000	//黑色
#define LCD_COLOR_WHITE  0xFFFF	
#define LCD_COLOR_NONE	LCD_COLOR_WHITE
#define LCD_COLOR_RESERVED(x) (x^0xFFFF)

#define LCD_ATTR_NULL	0
#define LCD_ATTR_ROT90	1
#define LCD_ATTR_ROT180	2
#define LCD_ATTR_ROT270	4

typedef enum rotate_h12_v
  {
    V12=0,
    V3=1,
    V6=2,
    V9=3
} rotate_h12_v_t;
	
//lcd硬件层抽象
typedef struct lcd_hw
{
	void (*init)(struct lcd_hw *lcd_hw);
	void (*reset)(struct lcd_hw *lcd_hw);
	void (*set_window)(struct lcd_hw *lcd_hw,int16u x, int16u y, int16u width, int16u height );
	void (*write_cmd)(struct lcd_hw *lcd_hw,int16u cmd);
	void (*write_data)(struct lcd_hw *lcd_hw,int16u data);
	void (*write_ram_prepare)(struct lcd_hw *lcd_hw);
	void (*write_ram)(struct lcd_hw *lcd_hw,int16u data);
	void (*display_ctrl)(struct lcd_hw *lcd_hw,BOOL on);
	void (*set_rotate)(struct lcd_hw *lcd_hw,enum rotate_h12_v rot);
	struct gpio* gpio_rst;
	struct io8080_bus* io_bus;
	int16u width,height;
	rotate_h12_v_t rotate;
}lcd_hw_t;



//逻辑lcd
typedef struct lcd
{
	void (*draw_pixel)(struct lcd* lcd,int16u x_pos, int16u y_pos, int16u color);
	void (*draw_horizon)(struct lcd* lcd,int16u x_pos, int16u y_pos, int16u len, int16u color);
	void (*draw_vertical)(struct lcd* lcd,int16u x_pos, int16u y_pos, int16u len, int16u color);
	void (*draw_char)(struct lcd* lcd,int16u x_pos, int16u y_pos,struct font* p_font, int16u TextColor , int16u BackColor, int16u ch);
	void (*draw_bit_map)(struct lcd* lcd,int16u x, int16u y,int16u width, int16u height,void* p_map);
	void (*draw_block)(struct lcd* lcd,int16u XPos, int16u YPos,int16u width, int16u height,int16u Color);
	void (*clear_screen)(struct lcd* lcd,int16u color);
	void (*refresh_screen)(void);
	struct lcd_hw* lcd_hw;
	int16u width;
	int16u height;
	int32u attribute;
	int8u* lcd_buffer;
}lcd_t;
#include "drivers/lcd/drawmem.h"
//单色，使用内存
#define DECLARE_MONO_MEM_LCD(lcd_hw,width,height,refresh_func) static int8u mono_mem_lcd_buffer[width*height/8];\
																			struct lcd mono_mem_lcd={draw_mem_pixel,\
																									draw_mem_horizon,\
																									draw_mem_vertical,\
																									draw_mem_char,\
																									draw_mem_bit_map,\
																									draw_mem_block,\
																									draw_mem_clear_screan,\
																									refresh_func,\
																									lcd_hw,/*对于内存刷新的设备不需要知道硬件*/\
																									width,\
																									height,\
																									0,\
																									mono_mem_lcd_buffer\
																									}
void lcd_fefresh(struct lcd* lcd);
void lcd_clear(struct lcd* lcd,u16 color);
void lcd_draw_pixel(struct lcd* lcd,int16s x_pos, int16s y_pos, u16 color);
void lcd_draw_horizon(struct lcd* lcd,int16s x_pos, int16s y_pos, int16u len, u16 color);
void lcd_draw_vertical(struct lcd* lcd,int16s x_pos, int16s y_pos, int16u len, u16 color);
void lcd_draw_char(struct lcd* lcd,int16s x_pos, int16s y_pos,struct font* font, uint16_t TextColor , uint16_t BackColor, int16u ch);
void lcd_draw_string(struct lcd* lcd,int16s x_pos, int16s y_pos,
    struct font* p_font, uint16_t TextColor , uint16_t BackColor,const wchar* p_string);
void lcd_draw_right_string(struct lcd* lcd,int16s x_pos, int16s y_pos,int16u width, int16u height,
	struct font* p_font, uint16_t TextColor , uint16_t BackColor,const wchar* p_string);

void lcd_draw_line_string(struct lcd* lcd,int16s x_pos, int16u line,int16u line_space,\
	struct font* p_font, uint16_t TextColor , uint16_t BackColor,const wchar* p_string);
void lcd_draw_center_string(struct lcd* lcd,int16s x_pos, int16s y_pos,int16u width,\
	int16u height,struct font* p_font, uint16_t TextColor , uint16_t BackColor,const wchar* p_string);
void lcd_draw_center_string_char(struct lcd* lcd,int16s index,int16s x_pos, int16s y_pos,int16u width, int16u height,
	struct font* p_font, uint16_t TextColor , uint16_t BackColor,const wchar* p_string);
void lcd_draw_line_center_string(struct lcd* lcd, int16s line,int16u line_space,
	struct font* p_font, uint16_t TextColor , uint16_t BackColor,const wchar* p_string);
void lcd_draw_line_right_string(struct lcd* lcd, int16s line,int16u line_space,
	struct font* p_font, uint16_t TextColor , uint16_t BackColor,const wchar* p_string);
void lcd_draw_block(struct lcd* lcd,int16s x_pos, int16s y_pos,int16u width, int16u height,int16u Color);																									
void lcd_draw_line(struct lcd* lcd,int16s x0, int16s y0, int16s x1, int16s y1,u16 color);
#endif
