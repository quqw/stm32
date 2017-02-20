#ifndef __FNS_LCD_H
#define __FNS_LCD_H
#include "fonts.h"
#include "st7732.h"

// typedef enum
// {
//    // top=MADCTR_VAL_TOP_TOP_VIEW,
//    // bottom=MADCTR_VAL_BOTTOM_TOP_VIEW,
//    // left=MADCTR_VAL_LEFT_TOP_VIEW,
//    // right=MADCTR_VAL_RIGHT_TOP_VIEW
// }ORIENTATION;
#define drvLCD_SetWindow   drvST7732_SetWindow 
#define drvLCD_WriteRamPrepare  drvST7732_WriteRamPrepare
#define drvLCD_WriteRam drvST7732_WriteRam
#define drvLCD_Reset    drvST7732_Reset
#define drvLCD_Config   drvST7732_Config
#define drvLCD_Init     drvST7732_Init
#define drvLCD_DisplayOn	drvST7732_DisplayOn
#define drvLCD_DisplayOff	drvST7732_DisplayOff
void drvLCD_DrawPixel(int16u x_pos, int16u y_pos, int16u color);
void drvLCD_DrawHorizon(int16u x_pos, int16u y_pos, int16u len, int16u color);
void drvLCD_DrawVertical(int16u x_pos, int16u y_pos, int16u len, int16u color);
void drvLCD_DrawChar(int16u x_pos, int16u y_pos,FONT *p_font, int16u TextColor , int16u BackColor,const char c);
void drvLCD_DrawBitMap(int16u x, int16u y,int16u width, int16u height,void* p_map);
void drvLCD_DrawBlock(int16u XPos, int16u YPos,int16u width, int16u height,int16u Color);
void drvLCD_Refresh(void);
void drvLCD_Clear(int16u Color);
#endif

