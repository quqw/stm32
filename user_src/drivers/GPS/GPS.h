#ifndef __GPS_H_0001
#define __GPS_H_0001
#include "platform/uart.h"
#define GPSBUFF_MAXCOUNT 256
///////����״̬����////////
#define  __90WEIDU       90*60*1000L
#define  __180JINGDU    180*60*1000L
#define  __360JINGDU    360*60*1000L

#define    GPS_GET_TIME     0x80	//��ȡʱ��
#define    MODULE_VALID      0x40	//ģ����Ч
#define    POSITION_VALID     0x20	//λ����Ч

#define DELCARE_GPS(name,uart) struct gps gps_##name={&uart}
typedef struct gps_info{	  
    
	DATETIME    UTCTime;		//��������ʽ����
  int8u State;
	int8u StarsUsed;			
	int8u StarsInView;
	int  Lo;    //����Ϊ��������Ϊ��
	int  La;    //��γΪ������γΪ��
} gps_info_t;

typedef struct gps
{
	struct uart *port;
	gps_info_t	info;
	int8u   gpsbuff[GPSBUFF_MAXCOUNT];	//ͨѶ������
	int8u   receive_count;
}gps_t;

void gps_init(gps_t* gps);
bool gps_parse_gpgga(String buf,gps_info_t* info);
void gps_tick(gps_t *gps);
#endif

