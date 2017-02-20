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
 * File: $Id: port.h,v 1.1 2007/09/12 10:15:56 wolti Exp $
 */

#ifndef _PORT_H
#define _PORT_H

//#include <stdint.h>
#include <assert.h>

#ifdef _cplusplus
extern          "C"
{
#endif

/* ----------------------- Defines ------------------------------------------*/

#define INLINE                         inline
#define STATIC                         static

#define PR_BEGIN_EXTERN_C              extern "C" {
#define	PR_END_EXTERN_C                }

#define MB_PORT_HAS_CLOSE              1
#define ENTER_CRITICAL_SECTION( )      vMBPPortEnterCritical( )
#define EXIT_CRITICAL_SECTION( )       vMBPPortExitCritical( )


/* ----------------------- Type definitions ---------------------------------*/
#include "types.h"

/* ----------------------- Function prototypes ------------------------------*/
    BOOL            bMBPIsWithinException( void );
    void            vMBPInit( void );
    void            vMBPPortEnterCritical( void );
    void            vMBPPortExitCritical( void );

#ifdef _cplusplus
}
#endif

#endif
