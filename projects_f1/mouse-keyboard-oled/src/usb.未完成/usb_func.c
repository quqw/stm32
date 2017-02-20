#include "usb_lib.h"
#include "usb_desc.h"
#include "usb_pwr.h"
#include "types.h"
#include "usb_func.h"
#include "platform/delay.h"

#define  KEYBD_REPORT_ID    0x02   
#define  KEYBD_LEFT_CTRL    0x01   
#define  KEYBD_LEFT_SHIFT   0x02   
#define  KEYBD_LEFT_ALT     0x04  
#define  KEYBD_LEFT_GUI     0x08
#define  KEYBD_RIGHT_CTRL   0x10   
#define  KEYBD_RIGHT_SHIFT  0x20   
#define  KEYBD_RIGHT_ALT    0x40  
#define  KEYBD_RIGHT_GUI    0x80

extern __IO uint8_t Endp2_Complete,Endp3_Complete;


/*
傍苧��
恷互了1燕幣俶勣梓廖shift
*/
//0x20~0x7F
// const int8u KEYCODE_MAP[]={
// 0x2C,0x9E,0x34+0x80,0x20+0x80,0x21+0x80,0x22+0x80,0x24+0x80,0x34,0x26+0x80,0x27+0x80,0x25+0x80,0x2E+0x80,0x36,0x2D,0x37,
// 0x38,0x27,0x1E,0x1F,0x20,0x21,0x22,0x23,0x24,
// 0x25,0x26,0x33+0x80,0x33,0x36,0x2E,0x37,0x38+0x80,0x1F,0x04,0x05,0x06,0x07,0x08,0x09,0x0A,0x0B,0x0C,0x0D,0x0E,0x0F,0x10,0x11,0x12,
// 0x13,0x14,0x15,0x16,0x17,0x18,0x19,0x1A,0x1B,0x1C,0x1D,0x2F,0x31,0x30,0x23,0x2D+0x80,0x35,
// 0x04+0x80,0x05+0x80,0x06+0x80,0x07+0x80,0x08+0x80,0x09+0x80,0x0A+0x80,
// 0x0B+0x80,0x0C+0x80,0x0D+0x80,0x0E+0x80,0x0F+0x80,0x10+0x80,
// 0x11+0x80,0x12+0x80,0x13+0x80,0x14+0x80,0x15+0x80,0x16+0x80,0x17+0x80,0x18+0x80,0x19+0x80,0x1A+0x80,0x1B+0x80,0x1C+0x80,0x1D+0x80,
// 0x2F+0x80,0x31+0x80,0x30+0x80,0x35+0x80,0
// };
const int8u FUNCKEY_MAP[]={
	0x3a,0x3b,0x3c,0x3d,0x3e,0x3f,0x40,0x41,0x42,0,43,0x44,0x45
};

//3A	Keyboard	F1		
//3B	Keyboard	F2		
//3C	Keyboard	F3		
//3D	Keyboard	F4		
//3E	Keyboard	F5		
//3F	Keyboard	F6		
//40	Keyboard	F7		
//41	Keyboard	F8		
//42	Keyboard	F9		
//43	Keyboard	F10		
//44	Keyboard	F11		
//45	Keyboard	F12

#define KEY_ENTER	0x28
#define KEY_ESC		0x29
#define KEY_DEL		0x2A
#define KEY_BACK_SPACK  42
#define KEY_TAB		0x2B
#define KEY_CAP		39
//ESC~0x29 DEL~2A TAB~2B CAP~39
/*  ASCII忖憲燕(逸廁猟周)  辛需議
KEY Hex Char      	KEY Hex Char 			KEY Hex Char 			KEY Hex Char  
 2C 　20 腎鯉       25 　38 　8              13 　50　 P             0B 　68 　h  
 1E*　 21　 !       26 　39 　9             14 　51 　Q             0C 　69　 i  
 34*　 22　 "       33* 　3A　 :             15　 52　 R             0D　 6A　 j  
 20*　 23　 #       33 　3B　 ;             16　 53 　S             0E 　6B 　k  
 21* 　24 　$        36　 3C 　<              17 　54 　T             0F 　6C　 l  
 22*　 25　 %        2E 　3D　 =              18　 55　 U             10　 6D　 m  
 24* 　26　 &        37 　3E 　>              19 　56　 V             11 　6E 　n  
 34　 27　 '        38*　 3F 　?              1A 　57　 W             12　 6F　 o  
 26* 　28 　(        1F 　40　 @              1B 　58　 X             13　 70　 p  
 27* 　29　 )        04　 41　 A              1C　 59 　Y             14 　72　 q  
 25* 　2A　 *        05 　42 　B              1D　 5A 　Z             15　 72 　r  
 2E* 　2B　 +        06 　43　 C              2F　 5B 　[             16　 73 　s  
 36　 2C 　,        07 　44　 D              31 　5C 　\             17　 74　 t  
 2D 　2D　 -        08 　45　 E              30 　5D　 ]             18　 75 　u  
 37 　 2E　 .       09　 46　 F              23　 5E　 ^             19　 76　 v  
 38　 2F　 /        0A　 47 　G              2D*　 5F　 _             1A 　77 　w
   
 27　 30　 0        0B 　48 　H              32 　60　 `             1B　 78 　x  
 1E　 31　 1        0C　 49　 I              04　 61 　a             1C　 79　 y  
 1F　 32　 2        0D　 4A 　J              05 　62　 b             1D　 7A 　z  
 20　 33　 3        0E 　4B　 K              06 　63　 c             2F* 　7B 　{  
 21　 34　 4        0F 　4C　 L              07  64　 d             31*　 7C　 | 
 22 　35　 5        10 　4D 　M              08　65 　e             30* 　7D　 }  
 23　 36 　6        11　 4E 　N              09　66　 f             32* 　7E　 ~  
 24　 37　 7        12 　4F　 O              0A  67 　g             -- 　7F　　 
*/


//int8u hid_get_key_value(int8u  key_byte)
//{
//    if(key_byte<0x20)
//	{
//        switch(key_byte){
//        case 0x0A:
//        case 0x0D:
//            return KEY_ENTER;
//        case 0x09:
//            return KEY_TAB;
//        case 0x1B:
//            return KEY_ESC;
//        }
//    }
//	else if(key_byte<0x80)
//	{
//        return KEYCODE_MAP[key_byte-0x20];
//    }
//	else if(key_byte <0x8C)
//	{
//		return FUNCKEY_MAP[key_byte-0x80];
//	}
//	return 0;
//}


BOOL hid_send_mouse(int8u key,int8s x,int8s y)
{
	int i;
	int8u mouse_buffer[5]={0,0, 0, 0, 0};
	mouse_buffer[0]=key&0x07;
	mouse_buffer[1]=(int8u)x;
	mouse_buffer[2]=(int8u)y;
	mouse_buffer[3]=0;
	for(i=0;i<100;i++)
	{
		if(bDeviceState == CONFIGURED && Endp3_Complete==1)
			break ;
		delay_us(100);
	}
	if(bDeviceState != CONFIGURED||Endp3_Complete==0)
		return FALSE;
	Endp3_Complete=0;
	/* Copy mouse position info in ENDP1 Tx Packet Memory Area*/
	USB_SIL_Write(EP3_IN, mouse_buffer, 4);
	/* Enable endpoint for transmission */
	SetEPTxValid(ENDP3);
	return TRUE;
}


BOOL hid_send_keyboard(int8u* keys)
{
	int i;
	int8u key_buff[8];
	memcpy(key_buff,keys,8);
	key_buff[1]=0;//駅倬葎0
	for(i=0;i<100;i++)
	{
		if(bDeviceState == CONFIGURED && Endp2_Complete==1)
			break ;
		delay_us(100);
	}
	if(bDeviceState != CONFIGURED||Endp2_Complete==0)
		return FALSE;
	Endp2_Complete=0;
	/* Copy mouse position info in ENDP1 Tx Packet Memory Area*/
	USB_SIL_Write(EP2_IN, key_buff, 8);
	/* Enable endpoint for transmission */
	SetEPTxValid(ENDP2);
	return TRUE;
}



