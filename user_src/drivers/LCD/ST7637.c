#include "includes.h"
#include "st7637.h"
#include "platform/io8080.h"

/* Private define ------------------------------------------------------------*/
#define V9_MADCTRVAL                0x90     /*!< Left orientation value.     */
#define V12_MADCTRVAL               0x30     /*!< Up orientation value.       */
#define V3_MADCTRVAL                0x50     /*!< Right orientation value.    */
#define V6_MADCTRVAL                0xF0     /*!< Bottom orientation value.   */
#define BACKLIGHT_DIVIDER           500      /*!< LCD handler step.           */


int OrientationOffsetX [] = { 0 /* V12*/,0 /* V3*/,+4 /* V6*/,+4 /* V9*/ };
int OrientationOffsetY [] = { +4 /* V12*/,0 /* V3*/,0 /* V6*/,+4 /* V9*/ };

void st7637_write_data(struct lcd_hw *st7637,int16u value)
{
    st7637->io_bus->write(st7637->io_bus,1,(int8u)value);
//*((volatile int16u*)(0x60020000))=(int16u)(value<<4);
}

int16u st7637_read_data(struct lcd_hw *st7637)
{
    return (int8u)(st7637->io_bus->read(st7637->io_bus,1));
}

void st7637_write_reg(struct lcd_hw *st7637,int16u value)
{
    st7637->io_bus->write(st7637->io_bus,0,(int8u)value);
}


void st7637_reset(struct lcd_hw *st7637)
{
  /***************************************
   **                                   **
   **  -------\______________/-------   **
   **         | <---Tres---> |          **
   **                                   **
   **  Tres: Min.1ms                    **
   ***************************************/
    
  	gpio_set(st7637->gpio_rst);
    delay_ms(10);
		gpio_reset(st7637->gpio_rst);
    delay_ms(50);
  	gpio_set(st7637->gpio_rst);
    delay_ms(50);
}

void st7637_init(struct lcd_hw *st7637)
{

    gpio_init(st7637->gpio_rst,GPIO_Mode_Out_PP);
    st7637_reset(st7637);
	
   st7637_write_reg(st7637, ST7637_SWRESET );

   //-----------disable autoread + Manual read once ----------------------------
   st7637_write_reg(st7637, ST7637_AUTOLOADSET );  // Auto Load Set 0xD7
   st7637_write_data(st7637, 0xBF );               // Auto Load Disable

   st7637_write_reg(st7637, ST7637_EPCTIN );       // EE Read/write mode 0xE0
   st7637_write_data(st7637, 0x00 );               // Set read mode

   st7637_write_reg(st7637, ST7637_EPMRD );        // Read active 0xE3
   st7637_write_reg(st7637, ST7637_EPCTOUT );      // Cancel control 0xE1

   //---------------------------------- Sleep OUT ------------------------------
   st7637_write_reg(st7637, ST7637_DISPOFF );      // display off 0x28
   st7637_write_reg(st7637, ST7637_SLPOUT );       // Sleep Out 0x11

   //--------------------------------Vop setting--------------------------------
   st7637_write_reg(st7637, ST7637_VOPSET );       // Set Vop by initial Module 0xC0
   st7637_write_data(st7637, 0xFB );               // Vop = 13.64
   st7637_write_data(st7637, 0x00 );               // base on Module

   //----------------------------Set Register-----------------------------------
   st7637_write_reg(st7637, ST7637_BIASSEL );      // Bias select 0xC3
   st7637_write_data(st7637, 0x00 );               // 1/12 Bias, base on Module

   st7637_write_reg(st7637, ST7637_BSTBMPXSEL );   // Setting Booster times 0xC4
   st7637_write_data(st7637, 0x05 );               // Booster X 8

   st7637_write_reg(st7637, ST7637_BSTEFFSEL );    // Booster eff 0xC5
   st7637_write_data(st7637, 0x11 );               // BE = 0x01 (Level 2)

   st7637_write_reg(st7637, ST7637_VGSORCSEL );    // Vg with booster x2 control 0xcb
   st7637_write_data(st7637, 0x01 );               // Vg from Vdd2

   st7637_write_reg(st7637, ST7637_ID1SET );       // ID1 = 00 0xcc
   st7637_write_data(st7637, 0x00 );               //

   st7637_write_reg(st7637, ST7637_ID3SET );       // ID3 = 00 0xce
   st7637_write_data(st7637, 0x00 );               //

   st7637_write_reg(st7637, 0xB7 );                // Glass direction
   st7637_write_data(st7637, 0xC0 );               //

   st7637_write_reg(st7637, ST7637_ANASET );       // Analog circuit setting 0xd0
   st7637_write_data(st7637, 0x1D );               //

   st7637_write_reg(st7637, 0xB4 );                // PTL mode set
   st7637_write_data(st7637, 0x18 );               // power normal mode
   st7637_write_reg(st7637, ST7637_INVOFF );       // Display Inversion OFF 0x20

   st7637_write_reg(st7637, 0x2A );                // column range
   st7637_write_data(st7637, 0x04 );               //
   st7637_write_data(st7637, 0x83 );               //

   st7637_write_reg(st7637, 0x2B );                // raw range
   st7637_write_data(st7637, 0x04 );               //
   st7637_write_data(st7637, 0x83 );               //


   st7637_write_reg(st7637, ST7637_COLMOD );       // Color mode = 65k 0x3A
   st7637_write_data(st7637, 0x05 );               //

   st7637_write_reg(st7637, ST7637_MADCTR );       // Memory Access Control 0x36
   st7637_write_data(st7637, V9_MADCTRVAL );

   st7637_write_reg(st7637, ST7637_DUTYSET );      // Duty = 132 duty 0xb0
   st7637_write_data(st7637, 0x7F );

   st7637_write_reg(st7637, 0x29 );                // Display ON
   st7637_write_reg(st7637, 0xF9 );                // Gamma
   st7637_write_data(st7637, 0x00 );               //
   st7637_write_data(st7637, 0x03 );               //
   st7637_write_data(st7637, 0x05 );               //
   st7637_write_data(st7637, 0x07 );               //
   st7637_write_data(st7637, 0x09 );               //
   st7637_write_data(st7637, 0x0B );               //
   st7637_write_data(st7637, 0x0D );               //
   st7637_write_data(st7637, 0x0F );               //
   st7637_write_data(st7637, 0x11 );               //
   st7637_write_data(st7637, 0x13 );               //
   st7637_write_data(st7637, 0x15 );               //
   st7637_write_data(st7637, 0x17 );               //
   st7637_write_data(st7637, 0x19 );               //
   st7637_write_data(st7637, 0x1B );               //
   st7637_write_data(st7637, 0x1D );               //
   st7637_write_data(st7637, 0x1F );               //
	 st7637_set_rotate(  st7637,V12 );
}

//void drvst7637_SetOrientation()
//{

//}

void st7637_display_ctrl(struct lcd_hw *st7637,BOOL on)
{
	if(on)
		st7637_write_reg(st7637,ST7637_DISPON);
	else
    st7637_write_reg(st7637,ST7637_DISPOFF);
}

void st7637_write_ram_prepare( struct lcd_hw *st7637 )
{
    st7637_write_reg(st7637,ST7637_RAMWR);
}

void st7637_write_ram( struct lcd_hw *st7637,int16u dat )
{
   st7637_write_data(st7637, dat);  
   st7637_write_data(st7637, dat >> 8 );
}

//设置显示方向
void st7637_set_rotate(  struct lcd_hw *st7637,rotate_h12_v_t H12 )
   {
   // Memory Access Control 0x36
   st7637_write_reg(st7637,( ST7637_MADCTR ));

   switch( H12 )
      {
      case V3  :
         st7637_write_data(st7637, V3_MADCTRVAL );
				st7637->rotate=V3;
         break;

      case V6  :
         st7637_write_data(st7637, V6_MADCTRVAL );
				st7637->rotate=V6;
         break;

      case V9  :
         st7637_write_data(st7637, V9_MADCTRVAL );
				st7637->rotate=V9;
         break;

      case V12 :
      default  :
				st7637->rotate=V12;
         st7637_write_data(st7637, V12_MADCTRVAL );
         break;
      }
   }
	 
void st7637_set_window( struct lcd_hw *st7637,int16u x, int16u y, int16u width, int16u height )
{
   st7637_write_reg(st7637,  ST7637_RASET  );
   st7637_write_data(st7637, y+OrientationOffsetY[st7637->rotate ]);
   st7637_write_data(st7637, y+OrientationOffsetY[st7637->rotate ] + height - 1 );
   st7637_write_reg(st7637,ST7637_CASET    );
   st7637_write_data(st7637, x+OrientationOffsetX[st7637->rotate]);
   st7637_write_data(st7637, x+OrientationOffsetX[st7637->rotate]+ width - 1  );
}

void  st7637_flush16(struct lcd_hw *st7637,int16u* pBuff)
{
	int i,j;
	st7637->set_window(st7637,0,0,st7637->width,st7637->height);
	st7637->write_ram_prepare(st7637);
	for(i=0;i<st7637->width;i++)
		for(j=0;j<st7637->height;j++)
			st7637->write_ram(st7637,*(pBuff++));
}
	
void  st7637_flush8(struct lcd_hw *st7637,int8u* pBuff)
{
	int i,j;
	st7637->set_window(st7637,0,0,st7637->width,st7637->height);
	st7637->write_ram_prepare(st7637);
	for(i=0;i<st7637->width;i++)
		for(j=0;j<st7637->height;j++)
			st7637->write_ram(st7637,*(pBuff++));
}

void  st7637_flush_bitmap(struct lcd_hw *st7637,void* pBuff)
{
	int i,j;
	vu32* data_ptr=PTR_BITADDR(pBuff,0);
	st7637->set_window(st7637,0,0,st7637->width,st7637->height);
	st7637->write_ram_prepare(st7637);
	for(i=0;i<st7637->width;i++)
		for(j=0;j<st7637->height;j++)
			st7637->write_ram(st7637,(*(data_ptr++)?LCD_COLOR_WHITE:LCD_COLOR_BLACK));
            //st7637->write_ram(st7637,0);
}


