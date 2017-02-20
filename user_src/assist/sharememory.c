/*
˵���������ڴ������ƣ�����ѭ���ռ䣬˫������ṹ������Ŀ��СΪ struct MEM_HEAD�����������ұ�֤4�ֽڶ��롣
900�ֽڴ�42us��ȡ39us
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

//����4�ֽڶ���
typedef struct _MEM_HEAD{
	int32u len;//����+ͷ���ܳ��ȣ������ֽڶ���
  u16 last;
	u16 next;
}MEM_HEAD;



static int8u _queue_buf[MEMORY_SIZE];
static int32u _lastHead=0;//ָ�����һ�����ݵĵ�ַ
static int _memBlockCount=0;
static int _alloced_size=0;
#define P_HEAD(x) ((MEM_HEAD*)(_queue_buf+x))
#define FIRST_HEAD	(P_HEAD(_lastHead)->next)
/*
����˵����Ѱ�ҿ��Է�����ڴ��ַ
������alloc_len����Ҫ������ڴ��С
���ز�����Ѱ�ҵ����ڴ��׵�ַ
��Ҫ�Ľ������ڵķ������Ϊ˳����䣬�ͷŹ���Ϊ�����ͷţ��������ײ����ڴ���Ƭ���ɿ��ǲ����ʺϵ���С�����ռ���з���
*/
static int32u _MemoryFind(int32s data_len)
{
	//int32u curr_head=_lastHead+(P_HEAD(_lastHead)->len+MEM_HEAD_SIZE-1)/MEM_HEAD_SIZE*MEM_HEAD_SIZE;
	int32u curr_head=_lastHead+((P_HEAD(_lastHead)->len+MEM_HEAD_SIZE-1)&(~((int32u)(MEM_HEAD_SIZE-1))));//�ֽڶ���Ŀ��ټ��㷽ʽ�����㵱ǰ�����ͷλ��
	curr_head=curr_head>=MEMORY_SIZE?curr_head-MEMORY_SIZE:curr_head;//��ǰ����ĵ�ַ	
	return curr_head;	
}

/*
����˵����ѭ�������ڴ�
������	data_len����Ҫ������ڴ��С
		pSrc��ԭʼ����
���ز�����Ѱ�ҵ����ڴ��׵�ַ
��Ҫ�Ľ���
*/

mem_address_t mem_alloc(int32s data_len,void* pSrc)
{
	mem_address_t alloc_address,next_address,next_next_address;
	int32s copy_len,alloced_len;
	//ATOMIC_RUN(
	__disable_irq();
	//alloc_len=(data_len+MEM_HEAD_SIZE-1)/MEM_HEAD_SIZE*MEM_HEAD_SIZE;//data_len���Բ��ֽڶ��룬��������ڴ���뱣֤������ڴ���MEM_HEAD����������
		//alloc_len=(data_len+MEM_HEAD_SIZE-1)&(~((int32u)(MEM_HEAD_SIZE-1)));//����ͷ���ȣ��ֽڶ���Ŀ��ټ��㷽ʽ
		next_address=_MemoryFind(data_len);//���䵽�ĵ�ַ
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

		//���¿�ʼ����
		P_HEAD(next_address)->len=data_len+MEM_HEAD_SIZE;//
		P_HEAD(next_address)->last=_lastHead;
		P_HEAD(next_address)->next=P_HEAD(_lastHead)->next;//�Լ�����һ��ָ���һ��
		P_HEAD(_lastHead)->next=next_address;
		P_HEAD(P_HEAD(next_address)->next)->last=next_address;//��һ�����һ��ָ���Լ�����֤��ָ����ȷ
//			if(_memBlockCount==1)//ԭ�Ƚ���1���ʱ��ͷβָ�붼��ָ���Լ��ģ���Ҫ����lastָ��λ��,�γɻ�
//				P_HEAD(_lastHead)->last=next_address;
		
		_lastHead=next_address;
		next_address+=MEM_HEAD_SIZE;//next_address���ָ��MEMORY_SIZE
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


//�˺����޷���⴫��ĵ�ַ�Ƿ�Ϊ��Ч��ַ��Σ�ա�
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
	if(mem_address==_lastHead)//�����һ�������ͷŵ��ˣ�����Ҫ��lastheadָ��������ݵ�ǰһ������
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
	next_address=mem_address+MEM_HEAD_SIZE;//���������������ڵ�ַ
	copy_len=next_address+copy_len> MEMORY_SIZE ? MEMORY_SIZE-next_address : copy_len;
	memcpy(pDest,_queue_buf+next_address,copy_len);
	if(copy_len<P_HEAD(mem_address)->len-MEM_HEAD_SIZE)//����������һͷһβ��û�п�����������������
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

