#ifndef ST7637_H001
#define ST7637_H001
#include "platform/gpio.h"
#include "drivers/lcd/lcd.h"
#define     ST7637_SCREEN_HEIGHT           128
#define     ST7637_SCREEN_WIDTH            128


	
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

#define ST7637_NOP               0x00
#define ST7637_SWRESET           0x01
#define ST7637_RDDID             0x04
#define ST7637_RDDST             0x09
#define ST7637_RDDPM             0x0A
#define ST7637_RDDMADCTR         0x0B
#define ST7637_RDDCOLMOD         0x0C
#define ST7637_RDDIM             0x0D
#define ST7637_RDDSM             0x0E
#define ST7637_RDDSDR            0x0F

#define ST7637_SLPIN             0x10
#define ST7637_SLPOUT            0x11
#define ST7637_PTLON             0x12
#define ST7637_NORON             0x13

#define ST7637_INVOFF            0x20
#define ST7637_INVON             0x21
#define ST7637_APOFF             0x22
#define ST7637_APON              0x23
#define ST7637_WRCNTR            0x25
#define ST7637_DISPOFF           0x28
#define ST7637_DISPON            0x29
#define ST7637_CASET             0x2A
#define ST7637_RASET             0x2B
#define ST7637_RAMWR             0x2C
#define ST7637_RGBSET            0x2D
#define ST7637_RAMRD             0x2E

#define ST7637_PTLAR             0x30
#define ST7637_SCRLAR            0x33
#define ST7637_TEOFF             0x34
#define ST7637_TEON              0x35
#define ST7637_MADCTR            0x36
#define ST7637_VSCSAD            0x37
#define ST7637_IDMOFF            0x38
#define ST7637_IDMON             0x39
#define ST7637_COLMOD            0x3A

#define ST7637_RDID1             0xDA
#define ST7637_RDID2             0xDB
#define ST7637_RDID3             0xDC

#define ST7637_DUTYSET           0xB0
#define ST7637_FIRSTCOM          0xB1
#define ST7637_OSCDIV            0xB3
#define ST7637_PTLMOD            0xB4
#define ST7637_NLINVSET          0xB5
#define ST7637_COMSCANDIR        0xB7
#define ST7637_RMWIN             0xB8
#define ST7637_RMWOUT            0xB9

#define ST7637_VOPSET            0xC0
#define ST7637_VOPOFSETINC       0xC1
#define ST7637_VOPOFSETDEC       0xC2
#define ST7637_BIASSEL           0xC3
#define ST7637_BSTBMPXSEL        0xC4
#define ST7637_BSTEFFSEL         0xC5
#define ST7637_VOPOFFSET         0xC7
#define ST7637_VGSORCSEL         0xCB

#define ST7637_ID1SET            0xCC
#define ST7637_ID2SET            0xCD
#define ST7637_ID3SET            0xCE

#define ST7637_ANASET            0xD0
#define ST7637_AUTOLOADSET       0xD7
#define ST7637_RDTSTSTATUS       0xDE

#define ST7637_EPCTIN            0xE0
#define ST7637_EPCTOUT           0xE1
#define ST7637_EPMWR             0xE2
#define ST7637_EPMRD             0xE3
#define ST7637_MTPSEL            0xE4
#define ST7637_ROMSET            0xE5
#define ST7637_HPMSET            0xEB

#define ST7637_FRMSEL            0xF0
#define ST7637_FRM8SEL           0xF1
#define ST7637_TMPRNG            0xF2
#define ST7637_TMPHYS            0xF3
#define ST7637_TEMPSEL           0xF4
#define ST7637_THYS              0xF7
#define ST7637_FRAMESET          0xF9
                                 
#define ST7637_MAXCOL            0x83
#define ST7637_MAXPAG            0x83



/* Private variables ---------------------------------------------------------*/

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


//lcd硬件层抽象
//typedef struct lcd_hw
//{
//	void (*init)(struct lcd_hw *lcd_hw);
//	void (*reset)(struct lcd_hw *lcd_hw);
//	void (*set_window)(struct lcd_hw *lcd_hw,int16u x, int16u y, int16u width, int16u height );
//	void (*write_cmd)(struct lcd_hw *lcd_hw,int16u cmd);
//	void (*write_data)(struct lcd_hw *lcd_hw,int16u data);
//	void (*led_ctrl)(BOOL on);
//	struct led* light;
//	struct gpio* rst;
//	struct io8080_bus* io;
//}lcd_hw_t;


#define DECLARE_ST7637(name,reset_x,io)\
										DECLARE_GPIO(st7637_##name##_reset,reset_x);\
										struct lcd_hw lcd_hw_st7637_##name={st7637_init,st7637_reset,st7637_set_window,\
																								st7637_write_reg,st7637_write_data,st7637_write_ram_prepare,\
																								st7637_write_ram,st7637_display_ctrl,st7637_set_rotate,\
																								&gpio_st7637_##name##_reset,&io,\
																								ST7637_SCREEN_WIDTH,ST7637_SCREEN_HEIGHT}

void st7637_write_data(struct lcd_hw *st7637,int16u value);
int16u st7637_read_data(struct lcd_hw *st7637);
void st7637_write_reg(struct lcd_hw *st7637,int16u value);																							
void st7637_init(struct lcd_hw *st7637);
void st7637_reset(struct lcd_hw *st7637);
void st7637_set_rotate(  struct lcd_hw *st7637,rotate_h12_v_t H12 );
void st7637_set_window( struct lcd_hw *st7637,int16u x, int16u y, int16u width, int16u height );
void st7637_write_ram_prepare( struct lcd_hw *st7637 );
void st7637_write_ram( struct lcd_hw *st7637,int16u );
void st7637_display_ctrl(struct lcd_hw *st7637,BOOL on);
void  st7637_flush_bitmap(struct lcd_hw *st7637,void* pBuff);
void  st7637_flush8(struct lcd_hw *st7637,int8u* pBuff);
void  st7637_flush16(struct lcd_hw *st7637,int16u* pBuff);
#endif

