/* 
 * MODBUS Library: AT91SAM7X/FreeRTOS port
 * Copyright (c) 2007 Christian Walter <wolti@sil.at>
 * All rights reserved.
 *
 * $Id: porttimer.c,v 1.1 2007/09/12 10:15:56 wolti Exp $
 */

/* ----------------------- System includes ----------------------------------*/
#include <stdlib.h>


/* ----------------------- Platform includes --------------------------------*/
#include "includes.h"
#include "port_mb.h"
#include "platform/gpio.h"
#include "platform/timer.h"

/* ----------------------- Modbus includes ----------------------------------*/
#include "mb.h"
#include "mbport.h"

/* ----------------------- Defines ------------------------------------------*/
#define TIMER_TIMEOUT_INVALID	( -1 )

/* ----------------------- Type definitions ---------------------------------*/


/* ----------------------- Static variables ---------------------------------*/
STATIC int32s _50us_timer=0,_50us_timer_init_val;
STATIC BOOL     bIsInitalized = FALSE;

/* ----------------------- Static functions ---------------------------------*/

/* ----------------------- Start implementation -----------------------------*/

/*

*/
DECLARE_TIMER(modbus,2);

//Ã¿50usÒ»¸ötick~20kHz
BOOL
xMBPortTimersInit( USHORT usTim1Timerout50us )
{
  ENTER_CRITICAL_SECTION(  );
	_50us_timer_init_val=usTim1Timerout50us;
	timer_init(&timer_modbus);
  timer_init_interrupt(&timer_modbus,0,0);
	timer_enable_interrupt(&timer_modbus,TIM_IT_Update);
	timer_set_prescaler(&timer_modbus,SystemCoreClock/1000000-1);//1MHz
	timer_set_auto_reload(&timer_modbus,50-1);//50us
	bIsInitalized=TRUE;
  EXIT_CRITICAL_SECTION(  );
  return TRUE;
}

void
vMBPortTimerClose( void )
{
    ENTER_CRITICAL_SECTION(  );
    if( bIsInitalized )
    {
			timer_stop(&timer_modbus);
        _50us_timer = TIMER_TIMEOUT_INVALID;
        bIsInitalized = FALSE;
			
    }
    EXIT_CRITICAL_SECTION(  );
}

void
vMBPortTimersEnable(  )
{
    assert( bIsInitalized );
    ENTER_CRITICAL_SECTION(  );
    _50us_timer=_50us_timer_init_val;
	timer_start(&timer_modbus);
    EXIT_CRITICAL_SECTION(  );
}

void
vMBPortTimersDisable(  )
{
    assert( bIsInitalized );
    ENTER_CRITICAL_SECTION(  );
	timer_stop(&timer_modbus);
    _50us_timer=0;
    EXIT_CRITICAL_SECTION(  );
}
extern BOOL
xMBRTUTimerT35Expired( void );
void TIM2_IRQHandler(void)
{
	timer_clear_interrupt_flag(&timer_modbus);
	if(_50us_timer)
	{
		_50us_timer--;
		if(_50us_timer==0)
			xMBRTUTimerT35Expired();
	}
}

