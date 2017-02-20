#include "includes.h"
#include "fonts.h"
#include "lcd.h"
#include "board/board.h"
void drvLCD_DrawPixel(int16u x_pos, int16u y_pos, int16u color)
{
	if(x_pos>=SCREEN_WIDTH || y_pos>=SCREEN_HEIGHT)
    drvLCD_SetWindow(x_pos, y_pos, 1, 1 );
    drvLCD_WriteRamPrepare();        // Send LCD RAM write command.
    drvLCD_WriteRam(color);
}

void drvLCD_DrawHorizon(int16u x_pos, int16u y_pos, int16u len, int16u color)
{
	int i;
	if(y_pos>=SCREEN_HEIGHT)
		return;
	len=x_pos+len>=SCREEN_WIDTH?SCREEN_WIDTH-x_pos:len;
	drvLCD_SetWindow(x_pos,y_pos,len,1);
	drvLCD_WriteRamPrepare();
	for(i=0;i<len;i++)
		drvLCD_WriteRam(color);
}
void drvLCD_DrawVertical(int16u x_pos, int16u y_pos, int16u len, int16u color)
{
	int i;
	if(x_pos>=SCREEN_WIDTH)
		return;
	len=y_pos+len>=SCREEN_HEIGHT?SCREEN_HEIGHT-y_pos:len;
	drvLCD_SetWindow(x_pos,y_pos,1,len);
	drvLCD_WriteRamPrepare();
	for(i=0;i<len;i++)
		drvLCD_WriteRam(color);
}
	
void drvLCD_DrawChar(int16u x_pos, int16u y_pos,FONT* p_font, uint16_t TextColor , uint16_t BackColor, const char ch)
{
    int32u i,j;
	char temp_c;
    vu32 *bitMap= PTR_BITADDR(&temp_c,0);
	if(x_pos+p_font->width>=SCREEN_WIDTH || y_pos+p_font->height>=SCREEN_HEIGHT)
		return;
    drvLCD_SetWindow(x_pos,y_pos,p_font->width,p_font->height);
    drvLCD_WriteRamPrepare();
	for(i=0;i<p_font->height*p_font->width/8;i++)
	{
        temp_c=*(p_font->getFontDots(p_font,ch,i));
	    for(j=0;j<8;j++)
	    {
	        if(bitMap[j])
	            drvLCD_WriteRam(TextColor);
	        else
	            drvLCD_WriteRam(BackColor);
	    }
	}
}

void drvLCD_DrawBitMap(int16u x, int16u y,int16u width, int16u height,void* p_map)
{
	int16u i,j;
    drvLCD_SetWindow(x,y,width,height);
    drvLCD_WriteRamPrepare();
	for(i=0;i<height;i++)
		for(j=0;j<width;j++)
		{
			drvLCD_WriteRam(((int16u**)p_map)[i][j]);
		}
	
}

void drvLCD_DrawBlock(int16u x_pos, int16u y_pos,int16u width, int16u height,int16u Color)
{
    int32u i;
	if(x_pos>=SCREEN_WIDTH || y_pos>=SCREEN_HEIGHT)
		return;
	height=y_pos+height>=SCREEN_HEIGHT?SCREEN_HEIGHT-y_pos:height;
	width=x_pos+width>=SCREEN_WIDTH?SCREEN_WIDTH-x_pos:width;
    drvLCD_SetWindow(x_pos,y_pos,width,height);
    drvLCD_WriteRamPrepare();
    for(i=0;i<width*height;i++)
    {
        drvLCD_WriteRam( Color);          // Draw pixel.
    }
}

void drvLCD_Clear(int16u Color)
{

}


void drvLCD_Refresh(void)
{
}
