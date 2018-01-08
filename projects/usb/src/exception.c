#include "exception.h"
#include "types.h"
#include "drivers/lcd/lcd.h"
#include "platform/delay.h"
#include "form/string.h"
extern struct lcd mono_mem_lcd;
void throw_general_exception(int32u exception, int8u *file,int32u line)
{
	wchar buf[16];
	char str[8];
	__disable_irq();
//	lcd_clear(&mono_mem_lcd,LCD_COLOR_WHITE);
//	lcd_draw_line_string(&mono_mem_lcd,38,2,1,&font1212,0x00,LCD_COLOR_WHITE,GET_WSTR(STR_ERR));
//	sprintf(str,"%04x",(int)exception);
//	to_wchar(str,buf);
//	lcd_draw_line_string(&mono_mem_lcd,64,2,1,&font1212,0x00,LCD_COLOR_WHITE,buf);	
//	lcd_fefresh(&mono_mem_lcd);
	delay_ms(100);
	//while(1);
	__enable_irq();
	
}

