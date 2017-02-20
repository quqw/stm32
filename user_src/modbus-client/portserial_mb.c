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
 * File: $Id: portserial.c,v 1.1 2007/09/12 10:15:56 wolti Exp $
 */

/* ----------------------- System includes ----------------------------------*/
#include <stdlib.h>
#include <assert.h>

/* ----------------------- Platform includes --------------------------------*/
#include "includes.h"
#include "platform/uart.h"
/* ----------------------- Modbus includes ----------------------------------*/
#include "mb.h"
#include "mbport.h"

/* ----------------------- Defines ------------------------------------------*/


/* ----------------------- Type definitions ---------------------------------*/




/* ----------------------- Static variables ---------------------------------*/


/* ----------------------- Static functions ---------------------------------*/


/* ----------------------- Start implementation -----------------------------*/


/*uart1*/
DECLARE_UART(1,0x09,0x0a);
#define MODBUS_SERIAL	uart_1
void
vMBPortSerialEnable( BOOL xRxEnable, BOOL xTxEnable )
{
    ENTER_CRITICAL_SECTION(  );
    if( xRxEnable )
    {
        uart_start_rx(&MODBUS_SERIAL);
    }
    else
    {
       uart_stop_rx(&MODBUS_SERIAL);
    }
    if( xTxEnable )
    {
       uart_start_tx(&MODBUS_SERIAL);
    }
    else
    {
       uart_stop_tx(&MODBUS_SERIAL);
    }
    EXIT_CRITICAL_SECTION(  );
}

BOOL
xMBPortSerialInit( ULONG ulBaudRate, UCHAR ucDataBits, eMBParity eParity )
{
	USART_InitTypeDef init;
	init.USART_BaudRate = ulBaudRate;
	init.USART_WordLength = USART_WordLength_8b;
	init.USART_StopBits = USART_StopBits_1;
	init.USART_Parity = USART_Parity_No;
	init.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	init.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
	ENTER_CRITICAL_SECTION(  );
	uart_init(&MODBUS_SERIAL,&init);
	EXIT_CRITICAL_SECTION(  );
	return TRUE;
}

BOOL xMBPortSerialPutByte( CHAR ucByte )
{
	uart_send_byte(&MODBUS_SERIAL,ucByte);
    return TRUE;
}

BOOL xMBPortSerialGetByte( CHAR * pucByte )
{
  if(uart_read_byte(&MODBUS_SERIAL,(int8u*)pucByte)==1)
    return TRUE;
  else return FALSE;
}


void USART1_IRQHandler(void)
{
	if(USART1->SR & (USART_FLAG_RXNE|USART_FLAG_ORE))
		pxMBFrameCBByteReceived();
	else if(USART1->SR & USART_FLAG_TXE)
		pxMBFrameCBTransmitterEmpty();
}
