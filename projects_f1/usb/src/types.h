#ifndef __TYPES_H
#include    <stdio.h>
#include    <string.h>
#include    <ctype.h>
#include    <stdlib.h>
#include    <stdarg.h>
#include "stm32f10x.h"
typedef const char* string;
typedef const char* String;
typedef char* StringBuffer;

typedef unsigned short wchar;
typedef const unsigned short* wstring;

typedef unsigned char BYTE;
typedef unsigned char byte;
typedef unsigned short WORD;
typedef unsigned long DWORD;
//typedef unsigned char UInt8;
//typedef unsigned short UInt16;
//typedef unsigned long UInt32;
typedef unsigned char int8u;
typedef unsigned short int16u;
typedef unsigned long int32u;
typedef signed char int8s;
typedef short int16s;
typedef int int32s;

typedef volatile int32u  vint32u;
typedef volatile int16u vint16u;
typedef volatile int8u  vint8u;

typedef unsigned char UCHAR;
typedef char    CHAR;

typedef unsigned short USHORT;
typedef short   SHORT;

typedef unsigned long ULONG;
typedef long    LONG;
//typedef enum
//{
//	false=0,
//	true=1
//}bool;
#define bool int8u
#define BOOL int8u 
#define TRUE 1
#define FALSE 0

typedef uint16_t event_id_t;


#endif  //__TYPES_H
