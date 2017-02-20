#ifndef __DRV_LM6029_H
#define __DRV_LM6029_H

#define LCD_X_MAX 127
#define LCD_Y_MAX 63
#define LCD_Y_MAXPAGE (LCD_Y_MAX/8)
#define LM6029_SCREEN_HEIGHT	(LCD_Y_MAX+1)
#define LM6029_SCREEN_WIDTH	(LCD_X_MAX+1)


#include "platform/gpio.h"
#include "platform/io8080.h"
#include "drivers/lcd/lcd.h"

#define LCD_X_ADJUST 0
#define LCD_Y_ADJUST 4

//light£ºled struct
//reset_x £ºreset½ÅµÄ±àºÅ
//io£ºstruct io_bus
#define DECLARE_LM6029(name,light,reset_x,io)\
										DECLARE_GPIO(lm6029_##name##_reset,reset_x);\
										struct lcd_hw lcd_hw_lm6029_##name={lm6029_init,lm6029_reset,0,\
																								lm6029_write_cmd_default,lm6029_write_data_default,0,0,0,0,\
																								&gpio_lm6029_##name##_reset,&io,\
																								LM6029_SCREEN_WIDTH,LM6029_SCREEN_HEIGHT}
																								
//#define DECLARE_LM6029_NO_LIGHT(name,reset_x,io)\
//										DECLARE_GPIO(lm6029_##name##_reset,reset_x);\
//										struct lcd_hw lcd_hw_lm6029_##name={lm6029_init,lm6029_reset,0,\
//																								lm6029_write_cmd_default,lm6029_write_data_default,0,0,0,\
//																								&gpio_lm6029_##name##_reset,&io,\
//																								LM6029_SCREEN_WIDTH,LM6029_SCREEN_HEIGHT}
void lm6029_write_cmd_default(struct lcd_hw *lm,int16u cmd);
void lm6029_write_data_default(struct lcd_hw *lm,int16u data);
void lm6029_reset(struct lcd_hw *lm);
void lm6029_set_column(struct lcd_hw *lm,int8u column);
void lm6029_clear(struct lcd_hw *lm,int8u Mask);
void lm6029_init(struct lcd_hw *lm);
void  lm6029_flush(struct lcd_hw *lm,void* pBuff);
#endif

