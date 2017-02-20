/*
 * FreeModbus Libary: ARM7/AT91SAM7X Port
 * Copyright (C) 2006 Christian Walter <wolti@sil.at>
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
 *
 * File: $Id: portother.c,v 1.1 2007/09/12 10:15:56 wolti Exp $
 */

/* ----------------------- System includes ----------------------------------*/
#include <stdlib.h>

/* ----------------------- Platform includes --------------------------------*/
#include "stm32f10x_conf.h"
/* ----------------------- Modbus includes ----------------------------------*/
#include "port_mb.h"

/* ----------------------- Defines ------------------------------------------*/

/* ----------------------- Type definitions ---------------------------------*/

/* ----------------------- Function prototypes ------------------------------*/
void            vMBPortSerialClose( void );
void            vMBPortEventClose( void );
void            vMBPortTimerClose( void );

/* ----------------------- Static variables ---------------------------------*/

/* ----------------------- Static functions ---------------------------------*/

/* ----------------------- Start implementation -----------------------------*/

void
vMBPInit( void )
{

}

void
__aeabi_assert( const char *pcFile, const char *pcAssertion, int iLine )
{
	assert_param(1);
}


void
vMBPPortEnterCritical( void )
{

}

void
vMBPPortExitCritical( void )
{

}

void
vMBPortClose( void )
{
//    vMBPortSerialClose(  );
    vMBPortEventClose(  );
    vMBPortTimerClose(  );
}
