#ifndef ST732_H001
#define ST732_H001
#include "platform/gpio.h"
#include "drivers/lcd/lcd.h"
#define     ST7732_SCREEN_HEIGHT           160
#define     ST7732_SCREEN_WIDTH            128

/* LCD Commands */
#define DISPLAY_ON               0xAF
#define DISPLAY_OFF              0xAE
#define START_LINE               0xC0
#define START_COLUMN             0x00
#define CLOCKWISE_OUTPUT         0xA0
#define DYNAMIC_DRIVE            0xA4
#define DUTY_CYCLE               0xA9
#define READ_MODIFY_WRITE_OFF    0xEE
#define SOFTWARE_RESET           0xE2

#define ST7732_NOP               0x00
#define ST7732_SWRESET           0x01
#define ST7732_RDDID             0x04
#define ST7732_RDDST             0x09
#define ST7732_RDDPM             0x0A
#define ST7732_RDDMADCTL         0x0B	//读取颜色填充方向
#define ST7732_RDDCOLMOD         0x0C	//读取顏色模式，12位，16位，18位
#define ST7732_RDDIM             0x0D
#define ST7732_RDDSM             0x0E
#define ST7732_RDDSDR            0x0F

#define ST7732_SLPIN             0x10
#define ST7732_SLPOUT            0x11
#define ST7732_PTLON             0x12
#define ST7732_NORON             0x13

#define ST7732_INVOFF            0x20
#define ST7732_INVON             0x21
#define ST7732_GAMSET            0x26
#define ST7732_DISPOFF           0x28
#define ST7732_DISPON            0x29
#define ST7732_CASET             0x2A
#define ST7732_RASET             0x2B
#define ST7732_RAMWR             0x2C
#define ST7732_RGBSET            0x2D
#define ST7732_RAMRD             0x2E

#define ST7732_PTLAR             0x30
#define ST7732_SCRLAR            0x33
#define ST7732_TEOFF             0x34
#define ST7732_TEON              0x35
#define ST7732_MADCTR            0x36   //设置填充方向
#define ST7732_VSCSAD            0x37
#define ST7732_IDMOFF            0x38
#define ST7732_IDMON             0x39
#define ST7732_COLMOD            0x3A   //设置颜色模式

#define ST7732_RDID1             0xDA
#define ST7732_RDID2             0xDB
#define ST7732_RDID3             0xDC

#define ST7732_RGBCTR            0xB0
#define ST7732_FRMCTR1           0xB1
#define ST7732_FRMCTR2           0xB2
#define ST7732_FRMCTR3           0xB3
#define ST7732_INVCTR            0xB4
#define ST7732_RGBBPCTR          0xB5
#define ST7732_DISSET5           0xB6

#define ST7732_PWCTR1           0xC0
#define ST7732_PWCTR2            0xC1
#define ST7732_PWCTR3            0xC2
#define ST7732_PWCTR4           0xC3
#define ST7732_PWCTR5           0xC4
#define ST7732_VMCTR1           0xC5
#define ST7732_VMOFCTR           0xC7

#define ST7732_WRID2             0xD1
#define ST7732_WRID3             0xD2
#define ST7732_RDID4            0xD3
#define ST7732_NVCTR1            0xD9
#define ST7732_NVCTR2            0xDE
#define ST7732_NVCTR3            0xDF

#define ST7732_GAMCTRP1          0xE0
#define ST7732_GAMCTRN1          0xE1

#define ST7732_AUTOCTRL          0xF1
#define ST7732_OSCADJ            0xF2
#define ST7732_DISPCTRL          0xF5
#define ST7732_DEFADJ            0xF6


/*实际屏幕的上方为有按钮的一方，这里的TOP为CPU一方，所以要反向*/
/*横屏后宽高也要随之而变化*/
/*说明书80页和105页*/
/* 	MX 	MY 	MV 	ML 	RGB		0	0	0*/
/*T	1	1	0*/
/*B	0	0	0*/
/*L	1	0	1*/
/*R	0	1	1*/
#define MADCTR_VAL_TOP_TOP_VIEW		0xC0
#define MADCTR_VAL_BOTTOM_TOP_VIEW	0
#define MADCTR_VAL_LEFT_TOP_VIEW	0x60
#define MADCTR_VAL_RIGHT_TOP_VIEW	0xA0



#define DECLARE_ST7732(name,reset_x,io)\
										DECLARE_GPIO(st7732_##name##_reset,reset_x);\
										struct lcd_hw lcd_hw_st7732_##name={st7732_init,st7732_reset,st7732_set_window,\
																								st7732_write_reg,st7732_write_data,st7732_write_ram_prepare,\
																								st7732_write_ram,st7732_display_ctrl,st7732_set_rotate,\
																								&gpio_st7732_##name##_reset,&io,\
																								ST7732_SCREEN_WIDTH,ST7732_SCREEN_HEIGHT}

void st7732_write_data(struct lcd_hw *st7732,int16u value);
int16u st7732_read_data(struct lcd_hw *st7732);
void st7732_write_reg(struct lcd_hw *st7732,int16u value);																							
void st7732_init(struct lcd_hw *st7732);
void st7732_reset(struct lcd_hw *st7732);
void st7732_set_rotate(  struct lcd_hw *st7732,rotate_h12_v_t H12 );
void st7732_set_window( struct lcd_hw *st7732,int16u x, int16u y, int16u width, int16u height );
void st7732_write_ram_prepare( struct lcd_hw *st7732 );
void st7732_write_ram( struct lcd_hw *st7732,int16u );
void st7732_display_ctrl(struct lcd_hw *st7732,BOOL on);
void  st7732_flush_bitmap(struct lcd_hw *st7732,void* pBuff);
void  st7732_flush8(struct lcd_hw *st7732,int8u* pBuff);
void  st7732_flush16(struct lcd_hw *st7732,int16u* pBuff);
#endif

