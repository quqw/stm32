#ifndef ASSIST_H
#define ASSIST_H
#define CRC8_PRESET	0xff
#include "common_types.h"
int16u crc16_ccitt(int8u *pBuff, int32u len) ;
int16u crc16_ccitt_l(int8u *pBuff, int32u len) ;
int16u crc16_typea(int8u* pBuff, int32u dwLen);
void bin_to_hex(int8u* pBuff,int32u nCount,char* pTagNo);
int8u hex_to_byte(const char* pCh);
int32u hex_to_bin(const char* pTagNo,int8u* pBuff,int32u nMaxCount);
int8u reverse_bits(int8u bytIn);
int8u crc8_ccitt(int8u preset,void * pBuff,int32u nCount);
uint32_t get_cpu_id32(void);
uint16_t get_cpu_id16(void);
void get_cpu_id_bytes8(uint8_t* id);
#endif

