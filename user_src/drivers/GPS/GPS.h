#ifndef __GPS_H_0001
#define __GPS_H_0001
#include "platform/uart.h"
#define GPSBUFF_MAXCOUNT 256
///////接收状态定义////////
#define  __90WEIDU       90*60*1000L
#define  __180JINGDU    180*60*1000L
#define  __360JINGDU    360*60*1000L

#define    GPS_GET_TIME     0x80	//获取时间
#define    MODULE_VALID      0x40	//模块有效
#define    POSITION_VALID     0x20	//位置有效

#define DELCARE_GPS(name,uart) struct gps gps_##name={&uart}
typedef struct gps_info{	  
    
	DATETIME    UTCTime;		//以数字形式保存
  int8u State;
	int8u StarsUsed;			
	int8u StarsInView;
	int  Lo;    //东经为正，西经为负
	int  La;    //北纬为正，南纬为负
} gps_info_t;

typedef struct gps
{
	struct uart *port;
	gps_info_t	info;
	int8u   gpsbuff[GPSBUFF_MAXCOUNT];	//通讯缓冲区
	int8u   receive_count;
}gps_t;

void gps_init(gps_t* gps);
bool gps_parse_gpgga(String buf,gps_info_t* info);
void gps_tick(gps_t *gps);
#endif

