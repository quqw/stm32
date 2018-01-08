#ifndef EXCEPTION_H
#define EXCEPTION_H
#include "types.h"
#define	EXCEPTION_STORAGE	0
#define EXCEPTION_MEM_ALLOCATE	1
#define EXCEPTION_MOTOR_CONTROL	2
#define EXCEPTION_MEM_ERROR	3

#define throw_exception(exception) throw_general_exception(exception,(int8u *)__FILE__, __LINE__)

void throw_general_exception(int32u exception,int8u* file,int32u line);

#endif

