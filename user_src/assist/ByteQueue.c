#include "string.h"
#include "bytequeue.h"
//byte queue有个bug未解决,变量定义只能在同文件内,否则sof和 size不同步
DECLARE_BYTE_QUEUE(uart_main,256);
DECLARE_BYTE_QUEUE(uart_485,256);
DECLARE_BYTE_QUEUE(uart_gprs,256);
int byte_queue_receive(byte_queue_t *queue, int8u* out)
{
	int count=0;
	__disable_irq();
	if(queue->length>0)
	{
		*out=queue->buff[queue->sof % queue->size];
		if(queue->eof!=(uint16_t)(queue->length+queue->sof))
			__nop();
		queue->length--;
		queue->sof++;
		count= 1;
	}
	else
	{
		count= 0;
	}
	__enable_irq();
	return count;
	
}

int byte_queue_put(byte_queue_t *queue, int8u data)
{
	int count=0;
	__disable_irq();
	if(queue->length<queue->size)
	{
		queue->buff[queue->eof % queue->size]=data;
		queue->eof++;
		queue->length++;
		count= 1;
	}
	else
	{
		count= 0;
	}
	__enable_irq();
	return count;
}


void byte_queue_clear(byte_queue_t *queue)
{
	__disable_irq();
	queue->length=0;
	queue->sof=0;
	queue->eof=0;
	__enable_irq();
}
void byte_queue_clear_ex(byte_queue_t *queue)
{
	__disable_irq();
	queue->length=0;
	queue->sof=0;
	queue->eof=0;
	memset(queue->buff,0,queue->length);
	__enable_irq();
}
int byte_queue_put_bytes(byte_queue_t *queue, int8u* data,int length)
{
	int i;
	for(i=0;i<length;i++)
	{
		if(!byte_queue_put(queue,data[i]))
			return i;
	}
	return i;
}

int byte_queue_receive_bytes(byte_queue_t *queue, int8u* out,int32u length)
{
	int count=0;
	while(length--)
	{
		if(0==byte_queue_receive(queue,out++))
			return count;
		count++;
	}
	return count;
}

//int8u queue_buff[256];
//struct byte_queue test_queue={0,0,0,256,queue_buff};
//extern byte_queue_t byte_queue_uart_main;
//extern byte_queue_t byte_queue_test;
////DECLARE_BYTE_QUEUE(test,256);
//byte_queue_t *p_q=&test_queue;
////测试正常
////int byte_queue_receive(byte_queue_t *queue, int8u* out)
////{
////	int count=0;
////	__disable_irq();
////	if(test_queue.length>0)
////	{
////		*out=test_queue.buff[test_queue.sof % test_queue.size];
////		if(test_queue.eof!=(uint16_t)(test_queue.length+test_queue.sof))
////			__nop();
////		test_queue.length--;
////		test_queue.sof++;
////		count= 1;
////	}
////	else
////	{
////		count= 0;
////	}
////	__enable_irq();
////	return count;
////	
////}

////int byte_queue_put(byte_queue_t *queue, int8u data)
////{
////	int count=0;
////	__disable_irq();
////	if(test_queue.length<test_queue.size)
////	{
////		test_queue.buff[test_queue.eof % test_queue.size]=data;
////		test_queue.eof++;
////		test_queue.length++;
////		count= 1;
////	}
////	else
////	{
////		count= 0;
////	}
////	__enable_irq();
////	return count;
////}

////int byte_queue_put_bytes(byte_queue_t *queue, int8u* data,int length)
////{
////	int i;
////	for(i=0;i<length;i++)
////	{
////		if(!byte_queue_put(queue,data[i]))
////			return i;
////	}
////	return i;
////}

////void byte_queue_clear(byte_queue_t *queue)
////{
////	__disable_irq();
////	test_queue.length=0;
////	test_queue.sof=0;
////	test_queue.eof=0;
////	__enable_irq();
////}
////void byte_queue_clear_ex(byte_queue_t *queue)
////{
////	__disable_irq();
////	test_queue.length=0;
////	test_queue.sof=0;
////	test_queue.eof=0;
////	memset(queue->buff,0,queue->size);
////	__enable_irq();
////}


////int byte_queue_receive_bytes(byte_queue_t *queue, int8u* out,int32u length)
////{
////	int count=0;
////	while(length--)
////	{
////		if(0==byte_queue_receive(queue,out++))
////			return count;
////		count++;
////	}
////	return count;
////}

//////-----也是正常的--------------------------------------------------------------------
//int byte_queue_receive(byte_queue_t *queue, int8u* out)
//{
////	struct byte_queue volatile *p_q=queue;
//	int count=0;
//	__disable_irq();
//	if(p_q->length>0)
//	{
//		*out=p_q->buff[p_q->sof %p_q->size];
//		if(p_q->eof!=(uint16_t)(p_q->length+p_q->sof))
//			__nop();
//		p_q->length--;
//		p_q->sof++;
//		count= 1;
//	}
//	else
//	{
//		count= 0;
//	}
//	__enable_irq();
//	return count;
//	
//}

//int byte_queue_put(byte_queue_t *queue, int8u data)
//{
//	//struct byte_queue volatile *p_q=queue;
//	int count=0;
//	__disable_irq();
//	if(p_q->length<p_q->size)
//	{
//		p_q->buff[p_q->eof % p_q->size]=data;
//		p_q->eof++;
//		p_q->length++;
//		count= 1;
//	}
//	else
//	{
//		count= 0;
//	}
//	__enable_irq();
//	return count;
//}

//int byte_queue_put_bytes(byte_queue_t *queue, int8u* data,int length)
//{
//	int i;
//	for(i=0;i<length;i++)
//	{
//		if(!byte_queue_put(queue,data[i]))
//			return i;
//	}
//	return i;
//}

//void byte_queue_clear(byte_queue_t *queue)
//{
//	//struct byte_queue volatile *p_q=queue;
//	__disable_irq();
//	p_q->length=0;
//	p_q->sof=0;
//	p_q->eof=0;
//	__enable_irq();
//}


//int byte_queue_receive_bytes(byte_queue_t *queue, int8u* out,int32u length)
//{
//	int count=0;
//	while(length--)
//	{
//		if(0==byte_queue_receive(queue,out++))
//			return count;
//		count++;
//	}
//	return count;
//}

