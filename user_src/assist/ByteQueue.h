#ifndef __BYTEQUEUE_H
#define __BYTEQUEUE_H
#include "common_types.h"

//length_bit
#define DECLARE_BYTE_QUEUE(name,size)	int8u queue_##name##_buff[size];\
										byte_queue_t byte_queue_##name={0,0,0,size,queue_##name##_buff}

int byte_queue_receive(byte_queue_t *queue, int8u* out);
int byte_queue_put(byte_queue_t *queue, int8u data);
void byte_queue_clear(byte_queue_t *queue);
int byte_queue_put_bytes(byte_queue_t *queue, int8u* data,int length);//返回实际放入的字节数	
int byte_queue_receive_bytes(byte_queue_t *queue, int8u* out,int32u length);
#endif

