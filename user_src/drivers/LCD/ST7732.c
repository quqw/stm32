#include "includes.h"
#include "st7732.h"
#include "platform/io8080.h"

/* Private define ------------------------------------------------------------*/
#define V9_MADCTRVAL                0x90     /*!< Left orientation value.     */
#define V12_MADCTRVAL               0x60     /*!< Up orientation value.       */
#define V3_MADCTRVAL                0xA0     /*!< Right orientation value.    */
#define V6_MADCTRVAL                0     /*!< Bottom orientation value.   */

void st7732_write_data(struct lcd_hw *st7732,int16u value)
{
    st7732->io_bus->write(st7732->io_bus,1,(int8u)value);
//*((volatile int16u*)(0x60020000))=(int16u)(value<<4);
}

int16u st7732_read_data(struct lcd_hw *st7732)
{
    return (int8u)(st7732->io_bus->read(st7732->io_bus,1));
}

void st7732_write_reg(struct lcd_hw *st7732,int16u value)
{
    st7732->io_bus->write(st7732->io_bus,0,(int8u)value);
//*((volatile int16u*)(0x60000000))=(int16u)(value<<4);
}


void st7732_reset(struct lcd_hw *st7732)
{
  /***************************************
   **                                   **
   **  -------\______________/-------   **
   **         | <---Tres---> |          **
   **                                   **
   **  Tres: Min.1ms                    **
   ***************************************/
    
  	gpio_set(st7732->gpio_rst);
    delay_ms(10);
		gpio_reset(st7732->gpio_rst);
    delay_ms(50);
  	gpio_set(st7732->gpio_rst);
    delay_ms(50);

}

void st7732_init(struct lcd_hw *st7732)
{

    gpio_init(st7732->gpio_rst,GPIO_Mode_Out_PP);
    st7732_reset(st7732);

   st7732_write_reg(st7732,ST7732_SWRESET); //Software Reset
   delay_ms(180); //DELAY 150MS
   st7732_write_reg(st7732,ST7732_SLPIN); //sleep IN
   delay_ms(100);
   st7732_write_reg(st7732,ST7732_SLPOUT); //sleep out
   delay_ms(150);
   
   st7732_write_reg(st7732,ST7732_FRMCTR1); // frame
   st7732_write_data(st7732,0x06);
   st7732_write_data(st7732,0x03);
   st7732_write_data(st7732,0x02);
   
   st7732_write_reg(st7732,ST7732_INVCTR); // 0xb4
   st7732_write_data(st7732,0x03);
   
   st7732_write_reg(st7732,ST7732_DISSET5); // frame 0xB6
   st7732_write_data(st7732,0x02);
   st7732_write_data(st7732,0x0e);
   
   st7732_write_reg(st7732,ST7732_DISPCTRL); // 0xF5
   st7732_write_data(st7732,0x1a);
   
   st7732_write_reg(st7732,0xc0); //Power Control 1 ( gvdd vci1)
   st7732_write_data(st7732,0x02);
   st7732_write_data(st7732,0x00);
   
   st7732_write_reg(st7732,0xc1); //Power Control 2( avdd vcl vgh vgl)
   st7732_write_data(st7732,0x05);
   
   st7732_write_reg(st7732,0xc2); //Power Control 3 ( in normal)
   st7732_write_data(st7732,0x02);
   st7732_write_data(st7732,0x02);
   
   st7732_write_reg(st7732,0xc3); //Power Control 4 ( in Idle mode)
   st7732_write_data(st7732,0x01);
   st7732_write_data(st7732,0x02);
   
   st7732_write_reg(st7732,0xc4); //Power Control 5 ( in partial mode)
   st7732_write_data(st7732,0x01);
   st7732_write_data(st7732,0x02);
   
   st7732_write_reg(st7732,0xc5); //Vcom Control ( vcomh,voml)
   st7732_write_data(st7732,0x47);
   st7732_write_data(st7732,0x3a);
   
   st7732_write_reg(st7732,ST7732_OSCADJ); // 0xF2 intern OSC 80Hz
   st7732_write_data(st7732,0x02);
   
   st7732_write_reg(st7732,ST7732_DEFADJ); //0xF6
   st7732_write_data(st7732,0x4c);
   
   st7732_write_reg(st7732,0xf8); //cmd 0xf8,dat 0x06; LOAD
   st7732_write_data(st7732,0x06); //SLEEP OUT LOAD DEFAULT
   
   /****************gamma adjust **********************/
   st7732_write_reg(st7732,0xe0); //gamma
   st7732_write_data(st7732,0x06);
   st7732_write_data(st7732,0x1c);
   st7732_write_data(st7732,0x1f);
   st7732_write_data(st7732,0x1f);
   st7732_write_data(st7732,0x18);
   st7732_write_data(st7732,0x13);
   st7732_write_data(st7732,0x06);
   st7732_write_data(st7732,0x03);
   st7732_write_data(st7732,0x03);
   st7732_write_data(st7732,0x04);
   st7732_write_data(st7732,0x07);
   st7732_write_data(st7732,0x07);
   st7732_write_data(st7732,0x00);
   
   st7732_write_reg(st7732,0xe1); //gamma
   st7732_write_data(st7732,0x0a);
   st7732_write_data(st7732,0x14);
   st7732_write_data(st7732,0x1b);
   st7732_write_data(st7732,0x18);
   st7732_write_data(st7732,0x12);
   st7732_write_data(st7732,0x0e);
   st7732_write_data(st7732,0x02);
   st7732_write_data(st7732,0x01);
   st7732_write_data(st7732,0x00);
   st7732_write_data(st7732,0x01);
   st7732_write_data(st7732,0x08);
   st7732_write_data(st7732,0x07);
   st7732_write_data(st7732,0x00);
   
   st7732_write_reg(st7732,ST7732_MADCTR); // Memory Access Control 0x36
   st7732_write_data(st7732,MADCTR_VAL_TOP_TOP_VIEW);
   
   st7732_write_reg(st7732,0x2a); //Column Range
   st7732_write_data(st7732,0x00);
   st7732_write_data(st7732,0x00);
   st7732_write_data(st7732,0x00);
   st7732_write_data(st7732,0x7F);
   
   st7732_write_reg(st7732,0x2b); //page Range
   st7732_write_data(st7732,0x00);
   st7732_write_data(st7732,0x00);
   st7732_write_data(st7732,0x00);
   st7732_write_data(st7732,0x7F); // configured as 128x128
   
   st7732_write_reg(st7732,ST7732_COLMOD); // Color mode = 65k 0x3A
   st7732_write_data(st7732,0x55);
   
   st7732_write_reg(st7732,ST7732_TEON); // 0x35
   st7732_write_data(st7732,0x00);
   
   st7732_write_reg(st7732,0x29); //display on
   delay_ms(20);
}

//void drvST7732_SetOrientation()
//{

//}

void st7732_display_ctrl(struct lcd_hw *st7732,BOOL on)
{
	if(on)
		st7732_write_reg(st7732,0x29);
	else
    st7732_write_reg(st7732,0x28);
}

void st7732_write_ram_prepare( struct lcd_hw *st7732 )
{
    st7732_write_reg(st7732,ST7732_RAMWR);
}

void st7732_write_ram( struct lcd_hw *st7732,int16u dat )
{
   st7732_write_data(st7732, dat);  
   st7732_write_data(st7732, dat >> 8 );
}

//设置显示方向
void st7732_set_rotate(  struct lcd_hw *st7732,rotate_h12_v_t H12 )
   {
   // Memory Access Control 0x36
   st7732_write_reg(st7732,( ST7732_MADCTR ));

   switch( H12 )
      {
      case V3  :
         st7732_write_data(st7732, V3_MADCTRVAL );
				st7732->rotate=V3;
         break;

      case V6  :
         st7732_write_data(st7732, V6_MADCTRVAL );
				st7732->rotate=V6;
         break;

      case V9  :
         st7732_write_data(st7732, V9_MADCTRVAL );
				st7732->rotate=V9;
         break;

      case V12 :
      default  :
				st7732->rotate=V12;
         st7732_write_data(st7732, V12_MADCTRVAL );
         break;
      }
   }
void st7732_set_window( struct lcd_hw *st7732,int16u x, int16u y, int16u width, int16u height )
{
   st7732_write_reg(st7732,  ST7732_RASET  );
   st7732_write_data(st7732, 0              );
   st7732_write_data(st7732, y              );
   st7732_write_data(st7732, 0              );
   st7732_write_data(st7732, y + height - 1 );
   st7732_write_reg(st7732,ST7732_CASET    );
   st7732_write_data(st7732, 0              );
   st7732_write_data(st7732, x              );
   st7732_write_data(st7732, 0              );
   st7732_write_data(st7732, x + width - 1  );
}

void  st7732_flush16(struct lcd_hw *st7732,int16u* pBuff)
{
	int i,j;
	st7732->set_window(st7732,0,0,st7732->width,st7732->height);
	st7732->write_ram_prepare(st7732);
	for(i=0;i<st7732->width;i++)
		for(j=0;j<st7732->height;j++)
			st7732->write_ram(st7732,*(pBuff++));
}
	
void  st7732_flush8(struct lcd_hw *st7732,int8u* pBuff)
{
	int i,j;
	st7732->set_window(st7732,0,0,st7732->width,st7732->height);
	st7732->write_ram_prepare(st7732);
	for(i=0;i<st7732->width;i++)
		for(j=0;j<st7732->height;j++)
			st7732->write_ram(st7732,*(pBuff++));
}

void  st7732_flush_bitmap(struct lcd_hw *st7732,void* pBuff)
{
	int i,j;
	vu32* data_ptr=PTR_BITADDR(pBuff,0);
	st7732->set_window(st7732,0,0,st7732->width,st7732->height);
	st7732->write_ram_prepare(st7732);
	for(i=0;i<st7732->width;i++)
		for(j=0;j<st7732->height;j++)
			st7732->write_ram(st7732,*(data_ptr++)?LCD_COLOR_BLACK:LCD_COLOR_WHITE);
}


