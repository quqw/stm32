/*
说明：共享内存分配机制，采用循环空间，双向链表结构。分配的块大小为 struct MEM_HEAD的整数倍并且保证4字节对齐。
900字节存42us，取39us
*/

#include    <stdio.h>
#include    <string.h>
#include    <stdlib.h>
#include    <stdarg.h>
#include "stm32f10x.h"
#include "types.h"
#include "assist.h"
#include "sharememory.h"

#define	MEM_HEAD_SIZE	8

//必须4字节对齐
typedef struct _MEM_HEAD{
	int32u len;//数据+头的总长度，不算字节对齐
  u16 last;
	u16 next;
}MEM_HEAD;



static int8u _queue_buf[MEMORY_SIZE];
static int32u _lastHead=0;//指向最后一个数据的地址
static int _memBlockCount=0;
static int _alloced_size=0;
#define P_HEAD(x) ((MEM_HEAD*)(_queue_buf+x))
#define FIRST_HEAD	(P_HEAD(_lastHead)->next)
/*
函数说明：寻找可以分配的内存地址
参数：alloc_len：需要分配的内存大小
返回参数：寻找到的内存首地址
需要改进：现在的分配机制为顺序分配，释放规则为随意释放，这样容易产生内存碎片，可考虑查找适合的最小连续空间进行分配
*/
static int32u _MemoryFind(int32s data_len)
{
	//int32u curr_head=_lastHead+(P_HEAD(_lastHead)->len+MEM_HEAD_SIZE-1)/MEM_HEAD_SIZE*MEM_HEAD_SIZE;
	int32u curr_head=_lastHead+((P_HEAD(_lastHead)->len+MEM_HEAD_SIZE-1)&(~((int32u)(MEM_HEAD_SIZE-1))));//字节对齐的快速计算方式，计算当前分配的头位置
	curr_head=curr_head>=MEMORY_SIZE?curr_head-MEMORY_SIZE:curr_head;//当前分配的地址	
	return curr_head;	
}

/*
函数说明：循环分配内存
参数：	data_len：需要分配的内存大小
		pSrc：原始数据
返回参数：寻找到的内存首地址
需要改进：
*/

mem_address_t mem_alloc(int32s data_len,void* pSrc)
{
	mem_address_t alloc_address,next_address,next_next_address;
	int32s copy_len,alloced_len;
	//ATOMIC_RUN(
	__disable_irq();
	//alloc_len=(data_len+MEM_HEAD_SIZE-1)/MEM_HEAD_SIZE*MEM_HEAD_SIZE;//data_len可以不字节对齐，但分配的内存必须保证分配的内存是MEM_HEAD的整数倍。
		//alloc_len=(data_len+MEM_HEAD_SIZE-1)&(~((int32u)(MEM_HEAD_SIZE-1)));//不算头长度，字节对齐的快速计算方式
		next_address=_MemoryFind(data_len);//分配到的地址
		alloc_address=next_address;
		alloced_len=MEM_HEAD_SIZE+((data_len+MEM_HEAD_SIZE-1)&(~((int32u)(MEM_HEAD_SIZE-1))));
		next_next_address=next_address+alloced_len;
		if(FIRST_HEAD<=next_address)
		{
			if(next_next_address-FIRST_HEAD>MEMORY_SIZE)
			{
				__enable_irq();
				return INVALID_ADDRESS;
			}
		}else{
			if(next_next_address>FIRST_HEAD)
			{
				__enable_irq();
				return INVALID_ADDRESS;
			}
		}

		//以下开始分配
		P_HEAD(next_address)->len=data_len+MEM_HEAD_SIZE;//
		P_HEAD(next_address)->last=_lastHead;
		P_HEAD(next_address)->next=P_HEAD(_lastHead)->next;//自己的下一块指向第一块
		P_HEAD(_lastHead)->next=next_address;
		P_HEAD(P_HEAD(next_address)->next)->last=next_address;//第一块的上一块指向自己，保证环指针正确
//			if(_memBlockCount==1)//原先仅有1块的时候，头尾指针都是指向自己的，需要更改last指向位置,形成环
//				P_HEAD(_lastHead)->last=next_address;
		
		_lastHead=next_address;
		next_address+=MEM_HEAD_SIZE;//next_address最大指向MEMORY_SIZE
		copy_len=MEMORY_SIZE>data_len+next_address?data_len:MEMORY_SIZE-next_address;
		memcpy(_queue_buf+next_address, pSrc,copy_len);
		if(copy_len<data_len)
		{
			pSrc=(int8u*)pSrc+copy_len;
			copy_len=data_len-copy_len;
			memcpy(_queue_buf,pSrc,copy_len);
		}
		_alloced_size+=alloced_len;
		_memBlockCount++;
		__enable_irq();
	//)
	return alloc_address;
}


//此函数无法检测传入的地址是否为有效地址，危险。
static void mem_free(mem_address_t mem_address)
{
	int alloced_len;
	if(_memBlockCount==0 || mem_address>=MEMORY_SIZE)
		return;
	alloced_len=(P_HEAD(mem_address)->len+MEM_HEAD_SIZE-1)&(~((int32u)(MEM_HEAD_SIZE-1)));
	_alloced_size-=alloced_len;
	_memBlockCount--;
	P_HEAD(mem_address)->len=0;
	if(_memBlockCount==0)
	{
		_lastHead=0;
		P_HEAD(0)->last=P_HEAD(0)->next=0;
		return;
	}
	P_HEAD(P_HEAD(mem_address)->last)->next=P_HEAD(mem_address)->next;
	P_HEAD(P_HEAD(mem_address)->next)->last=P_HEAD(mem_address)->last;
	if(mem_address==_lastHead)//将最后一个数据释放掉了，所以要让lasthead指向这个数据的前一个数据
		_lastHead=P_HEAD(mem_address)->last;

}

int32s mem_peek(mem_address_t mem_address,void* pDest)
{
	mem_address_t copy_len,next_address;
	if(pDest==NULL)
		return 0;
	if(P_HEAD(mem_address)->len<MEM_HEAD_SIZE)
		return -1;
	copy_len=P_HEAD(mem_address)->len-MEM_HEAD_SIZE;
	next_address=mem_address+MEM_HEAD_SIZE;//计算真正数据所在地址
	copy_len=next_address+copy_len> MEMORY_SIZE ? MEMORY_SIZE-next_address : copy_len;
	memcpy(pDest,_queue_buf+next_address,copy_len);
	if(copy_len<P_HEAD(mem_address)->len-MEM_HEAD_SIZE)//数据正好在一头一尾，没有拷贝结束，继续拷贝
	{
		pDest=(int8u*)pDest+copy_len;
		copy_len=P_HEAD(mem_address)->len-MEM_HEAD_SIZE-copy_len;
		memcpy(pDest,_queue_buf,copy_len);
	}
	return P_HEAD(mem_address)->len-MEM_HEAD_SIZE;
}
extern void delay_ms(int32u us);
int32s mem_fetch(mem_address_t mem_address,void* pDest)
{
	int32s len;
	__disable_irq();
	len=mem_peek(mem_address,pDest);
	if(len>=0)
		mem_free(mem_address);
	__enable_irq();
	return len; 
}
#include "includes.h"
void mem_test(void *p)
{
	mem_address_t addr,addr1,addr2;
	int i,len;
	int8u src[16],dest[16];
	for(i=0;i<16;i++)
		src[i]=i+0xb0;
	while(1)
	{
		addr=mem_alloc(16,src);//+0
		if(addr==INVALID_ADDRESS)
			continue;
		addr1=mem_alloc(16,src);//+1
		if(addr1==INVALID_ADDRESS)
			continue;
		addr2=mem_alloc(16,src);//+2
		if(addr2==INVALID_ADDRESS)
			continue;
		len=mem_fetch(addr1,dest); //-1
		if(len!=16)
			continue;
		vTaskDelay(5);
		len=mem_fetch(addr,dest);//-0
		if(len!=16)
			continue;
		addr=mem_alloc(16,src);//+0
		if(addr==INVALID_ADDRESS)
			continue;
		addr1=mem_alloc(16,src);//+1
		if(addr1==INVALID_ADDRESS)
			continue;		
		vTaskDelay(5);
		len=mem_fetch(addr2,dest);//-2
		if(len!=16)
			continue;
		len=mem_fetch(addr,dest);//-0
		if(len!=16)
			continue;
		len=mem_fetch(addr1,dest);//-1
		if(len!=16)
			continue;
		
		addr=mem_alloc(16,src);//+0
		if(addr==INVALID_ADDRESS)
			continue;
		addr1=mem_alloc(16,src);//+1
		if(addr1==INVALID_ADDRESS)
			continue;
		len=mem_fetch(addr1,dest); //-1
		if(len!=16)
			continue;
		addr2=mem_alloc(16,src);//+2
		if(addr2==INVALID_ADDRESS)
			continue;
		len=mem_fetch(addr,dest);//-0
		if(len!=16)
			continue;
		addr=mem_alloc(16,src);//+0
		if(addr==INVALID_ADDRESS)
			continue;
		vTaskDelay(5);
		addr1=mem_alloc(16,src);//+1
		if(addr1==INVALID_ADDRESS)
			continue;		
		len=mem_fetch(addr2,dest);//-2
		if(len!=16)
			continue;
		len=mem_fetch(addr1,dest);//-1
		if(len!=16)
			continue;
		addr2=mem_alloc(16,src);//+2
		if(addr2==INVALID_ADDRESS)
			continue;
		len=mem_fetch(addr,dest);//-0
		if(len!=16)
			continue;
		len=mem_fetch(addr2,dest);//-2
		if(len!=16)
			continue;
	}
}

