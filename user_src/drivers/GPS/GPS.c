#include  "includes.h"

#include "platform/gpio.h"
#include "drivers/gps/gps.h"
#define GPS_USART USART2
#define GPS_USART_IRQ USART2_IRQn


#define GPS_POWER_GPIO	GPIOA
#define GPS_POWER_RCC	RCC_APB2Periph_GPIOA
#define GPS_POWER_PIN	GPIO_Pin_0


//bool fns_GPS_parseRMC(gps_info_t* pGPS);

#define  GPS_RECEV_IDLE   0x00 		//空闲状态
#define  GPS_RECEV_START  0x01  	//收到$后,开始接收

void gps_init(gps_t *gps)
{
	gps->receive_count=0;
}

void gps_tick(gps_t *gps)
{
	int8u ch;
	while(gps->port->read_byte(gps->port,&ch)>0)
	{
		if( ch=='$')
			gps->receive_count=0;
		else if(ch=='\r' || ch=='\n')
		{
			if(gps->receive_count>20)
			{
				gps->gpsbuff[gps->receive_count]=0;
				if(memcmp(gps->gpsbuff,"GPGGA",5)==0)
				{
					gps_parse_gpgga((String)(gps->gpsbuff),&(gps->info));
				}
				else if(memcmp(gps->gpsbuff,"GPGLL",5)==0)
				{
					
				}
				else if(memcmp(gps->gpsbuff,"GPRMC",5)==0)
				{
					//if(fns_GPS_parseRMC(&g_gps))
					//	appSendEvent(GPS_EVENT_ID,MemoryAllocate(sizeof(GPSINFO),&g_gps));
				}
				else if(memcmp(gps->gpsbuff,"GPGSA",5)==0)
				{
					
				}
				else if(memcmp(gps->gpsbuff,"GPGSV",5)==0)
				{
					
				}
				else if(memcmp(gps->gpsbuff,"GPVTG",5)==0)
				{
//					sprintf(buf,"size:%ld",s_receiveCcount);
//					drvLCD_DrawString(10, 48,&AssicFont0612,0 , 0xffff, buf);
				}
				
			}
			gps->receive_count=0;
		}
		else if(isprint(ch) && ch!=' ')
		{
				gps->gpsbuff[gps->receive_count++]=(char)toupper(ch);
		}
	}
}

void drvGPS_ParseDate(int8u* pBuff,DATETIME* pTime){
    pTime->day  =(int8u)parse_u32((String)pBuff,2);
    pTime->month=(int8u)parse_u32((String)&pBuff[2],2);
    pTime->year=(int8u)parse_u32((String)&pBuff[4],2);
}

void drvGPS_ParseTime(int8u* pBuff,DATETIME* pTime){
    pTime->hour  =(int8u)parse_u32((String)pBuff,2);
    pTime->minute=(int8u)parse_u32((String)&pBuff[2],2);
    pTime->second=(int8u)parse_u32((String)&pBuff[4],2);
}

int32u drvGPS_ParseLa(int8u* pBuff){
    int32u d,f,wf;
    d=parse_u32((String)pBuff,2);
    f=parse_u32((String)&pBuff[2],2);
    wf=parse_u32((String)&pBuff[5],4);
    
    return d*3600*1000+f*60*1000+wf*6;
}

int32u drvGPS_ParseLo(int8u* pBuff){
    int32u d,f,wf;
    d=parse_u32((String)pBuff,3);
    f=parse_u32((String)&pBuff[3],2);
    wf=parse_u32((String)&pBuff[6],4);
    
    return d*3600*1000+f*60*1000+wf*6;
}

bool gps_parse_gpgga(String buf,gps_info_t* info)
{
	int8u i=0,area=0;
	int8u checksum=0,checksum2;
	memset(info,0,sizeof(gps_info_t));
	while(buf[i]&&buf[i]!='*')
	{
		checksum^=buf[i];
		if(buf[i]==',')
		{	
					area++;  
					switch(area){
					case 1:
							if(!isdigit(buf[i+1])) break;
							info->State|=GPS_GET_TIME;
							drvGPS_ParseTime((int8u*)&buf[i+1],&info->UTCTime);
							break;
					case 2:
							if(!isdigit(buf[i+1])) break;
							info->La=drvGPS_ParseLa((int8u*)&buf[i+1]);
							break;
					case 3:
							if(buf[i+1]=='S'){
									info->La=-info->La;
							}
							break;
					case 4:
							info->Lo=drvGPS_ParseLo((int8u*)&buf[i+1]);
							break;
					case 5:
							if(buf[i+1]=='W')
							{
									info->Lo=-info->Lo;
									info->State|=MODULE_VALID;      //此处认为GPS有效
							}else if(buf[i+1]=='E'){
									info->State|=MODULE_VALID;      //此处认为GPS有效
							}
							else
								info->State&=~MODULE_VALID;
							break;
					case 7:			    
							info->StarsUsed = parse_u32((String)&buf[i+1],2);
							if(info->StarsUsed>=3){
									info->State|=POSITION_VALID;
							}
							else
								info->State&=~POSITION_VALID;
							break;
					}
		}
		i++;
	}

	if(buf[i]!='*'){
			return FALSE;
	}
	checksum2=0;
	hex2bin(&buf[i+1],&checksum2,1);
	if(checksum!=checksum2){
			return FALSE;
	}
	return TRUE;
}


//bool fns_GPS_parseRMC(GPSINFO* pGPS)
//{
//	UInt8 i=0,dot_i=0;
//	UInt8 checksum=0;
//    
//	memset(pGPS,0,sizeof(GPSINFO));
//	//解析
//    for(i=0;(i<s_receiveCcount)&&(s_gpsbuff[i]!='*');i++){
//		checksum^=s_gpsbuff[i];

//		if(s_gpsbuff[i]==','){	
//            dot_i++;  
//            switch(dot_i){
//            case 1:
//                if(!isdigit(s_gpsbuff[i+1])) break;
//                drvGPS_ParseTime(&s_gpsbuff[i+1],&pGPS->UTCTime);
//                break;
//            case 2:
//                if(s_gpsbuff[i+1]=='A'){
//                    pGPS->State|=POSITION_VALID;
//                }else{
//                    pGPS->State&=~POSITION_VALID;
//                    pGPS->StarsUsed=0;
//                    return true;
//                }
//                break;
//            case 3:
//                pGPS->La=drvGPS_ParseLa(&s_gpsbuff[i+1]);    
//                break;
//            case 4:
//                if(s_gpsbuff[i+1]=='S'){
//                    pGPS->La=-pGPS->La;
//                }
//                break;
//            case 5:
//                pGPS->Lo=drvGPS_ParseLo(&s_gpsbuff[i+1]);
//                break;
//            case 6:
//                if(s_gpsbuff[i+1]=='W'){
//                    pGPS->Lo=-pGPS->Lo;
//                    pGPS->State|=MODULE_VALID;      //此处认为GPS有效
//                }else if(s_gpsbuff[i+1]=='E'){
//                    pGPS->State|=MODULE_VALID;      //此处认为GPS有效
//                }
//				else
//					pGPS->State&=~MODULE_VALID;
//                break;
//            case 7:	//速度		         
//                break;
//            case 8://估计是转速
//                break;
//            case 9://日期
//                drvGPS_ParseDate(&s_gpsbuff[i+1],&pGPS->UTCTime);
//                break;
//            case 10://磁场
//                break;    
//            }
//		}
//	}
//	return true;
//}



