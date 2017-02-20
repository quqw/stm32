#ifndef SHARE_MEMORY_H001
#define SHARE_MEMORY_H001
#include "types.h"
typedef int32u mem_address_t;
#define INVALID_ADDRESS	((mem_address_t)(-1))
#ifdef __DEBUG
	#define MEMORY_SIZE 128
#else
	#define MEMORY_SIZE 1024
#endif

mem_address_t mem_alloc(int32s data_len,void* pSrc);
int32s mem_fetch(mem_address_t mem_address,void* pDest);
//void mem_free(mem_address_t mem_address);
int32s mem_peek(mem_address_t mem_address,void* pDest);
void mem_test(void *p);
#endif


