/**
  ******************************************************************************
  * @file    usb_desc.c
  * @author  MCD Application Team
  * @version V4.0.0
  * @date    21-January-2013
  * @brief   Descriptors for Joystick Mouse Demo
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; COPYRIGHT 2013 STMicroelectronics</center></h2>
  *
  * Licensed under MCD-ST Liberty SW License Agreement V2, (the "License");
  * You may not use this file except in compliance with the License.
  * You may obtain a copy of the License at:
  *
  *        http://www.st.com/software_license_agreement_liberty_v2
  *
  * Unless required by applicable law or agreed to in writing, software 
  * distributed under the License is distributed on an "AS IS" BASIS, 
  * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
  * See the License for the specific language governing permissions and
  * limitations under the License.
  *
  ******************************************************************************
  */


/* Includes ------------------------------------------------------------------*/
#include "usb_lib.h"
#include "usb_desc.h"

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Extern variables ----------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

/* USB Standard Device Descriptor */
//const uint8_t Joystick_DeviceDescriptor[JOYSTICK_SIZ_DEVICE_DESC] =
//  {
//    0x12,                       /*bLength */
//    USB_DEVICE_DESCRIPTOR_TYPE, /*bDescriptorType*/
//    0x00,                       /*bcdUSB */
//    0x02,
//    0x00,                       /*bDeviceClass*/
//    0x00,                       /*bDeviceSubClass*/
//    0x00,                       /*bDeviceProtocol*/
//    0x40,                       /*bMaxPacketSize 64*/
//    0x83,                       /*厂商标志idVendor (0x0483)*/
//    0x04,
//    0x10,                       /*产品标志idProduct = 0x5710*/
//    0x57,
//    0x00,                       /*bcdDevice rel. 2.00*/
//    0x02,
//    1,                          /*Index of string descriptor describing
//                                                  manufacturer */
//    2,                          /*Index of string descriptor describing
//                                                 product*/
//    3,                          /*Index of string descriptor describing the
//                                                 device serial number */
//    0x01                        /*bNumConfigurations*/
//  }
//  ; /* Joystick_DeviceDescriptor */


///* USB Configuration Descriptor */
///*   All Descriptors (Configuration, Interface, Endpoint, Class, Vendor */
//const uint8_t Joystick_ConfigDescriptor[JOYSTICK_SIZ_CONFIG_DESC] =
//  {
//    0x09, /* bLength: Configuration Descriptor size */
//    USB_CONFIGURATION_DESCRIPTOR_TYPE, /* bDescriptorType: Configuration */
//    JOYSTICK_SIZ_CONFIG_DESC,
//    /* wTotalLength: Bytes returned */
//    0x00,
//    0x01,         /*bNumInterfaces: 1 interface*/
//    0x01,         /*bConfigurationValue: Configuration value*/
//    0x00,         /*iConfiguration: Index of string descriptor describing
//                                     the configuration*/
//    0xE0,         /*bmAttributes: Self powered */
//    0x32,         /*MaxPower 100 mA: this current is used for detecting Vbus*/

//    /************** Descriptor of Joystick Mouse interface ****************/
//    /* 09 */
//    0x09,         /*bLength: Interface Descriptor size*/
//    USB_INTERFACE_DESCRIPTOR_TYPE,/*bDescriptorType: Interface descriptor type*/
//    0x00,         /*bInterfaceNumber: Number of Interface*/
//    0x00,         /*bAlternateSetting: Alternate setting*/
//    HID_ENDPOINT_NUMBER,         /*bNumEndpoints*/
//    0x03,         /*bInterfaceClass: HID*/
//    0,/*0x01,         bInterfaceSubClass : 1=BOOT, 0=no boot*/
//    0,/*0x01,         nInterfaceProtocol : 0=none, 1=keyboard, 2=mouse*/
//    0,            /*iInterface: Index of string descriptor*/
//    /******************** Descriptor of Joystick Mouse HID ********************/
//    /* 18 */
//    0x09,         /*bLength: HID Descriptor size*/
//    HID_DESCRIPTOR_TYPE, /*bDescriptorType: HID*/
//    0x10,         /*0x00,bcdHID: HID Class Spec release number*/
//    0x01,
//    0x00,         /*bCountryCode: Hardware target country*/
//    0x01,         /*bNumDescriptors: Number of HID class descriptors to follow*/
//    0x22,         /*bDescriptorType*/
//    JOYSTICK_SIZ_REPORT_DESC,/*wItemLength: Total length of Report descriptor*/
//    0x00,
//    /******************** Descriptor of Joystick Mouse endpoint ********************/
//    /* 27 */
//    0x07,          /*bLength: Endpoint Descriptor size*/
//    USB_ENDPOINT_DESCRIPTOR_TYPE, /*bDescriptorType:*/

//    0x81,          /*bEndpointAddress: Endpoint Address (IN)*/
//    0x03,          /*bmAttributes: Interrupt endpoint*/
//    0x20,          /*wMaxPacketSize: 64 Byte max */
//    0x00,
//    0x20,          /*bInterval: Polling Interval (32 ms)*/
//    /* 34 */
//		
//		0x07,	/* bLength: Endpoint Descriptor size */
//    USB_ENDPOINT_DESCRIPTOR_TYPE,	/* bDescriptorType: */
//			/*	Endpoint descriptor type */
//    0x01,	/* bEndpointAddress: */
//			/*	Endpoint Address (OUT) */
//    0x03,	/* bmAttributes: Interrupt endpoint */
//    0x20,	/* wMaxPacketSize: 64 Bytes max  */
//    0x00,
//    0x20,	/* bInterval: Polling Interval (20 ms) */
//		/* 41 */
//  }
//  ; /* MOUSE_ConfigDescriptor */
//	
//const uint8_t Joystick_ReportDescriptor[JOYSTICK_SIZ_REPORT_DESC] =
//  {
//    0x05,0x01,        /*  Usage Page(Generic Desktop)通用桌面设备*/
//    0x09,0x02,          /*Usage(Mouse)*/
//    0xA1,0x01,          /*Collection(Logical)*/ 
//		 0x85, 0x01,        /* REPORT_ID (1)   */
//			0x09,0x01,          /*Usage(Pointer)*/
//			0x05,0x09,          /*Usage Page(Buttons)*/
//			0x19,0x01,          /*Usage Minimum(1)最小值-最大值,鼠标3个按钮所以1-3*/
//			0x29,0x03,          /*Usage Maximum(3)*/
//			0x15, 0x00,         /*Logical Minimum(0)-bit 0*/
//			0x25,0x01,          /*Logical Maximum(1)-bit 1*/
//			0x95,0x03,          /*Report Count(3) -数量3*/
//			0x75,0x01,          /*Report Size(1)大小1bit*/ /*以上表示3个bit表示鼠标3个按钮情况，bit0：左键，bit1右键，bit2中间键*/
//			0x81,0x02,          /*Input(Variable)输入，绝对值*/
//			0x95,0x01,          /*Report Count(1)*/
//			0x75,0x05,          /*Report Size(5)填充5bit，补足1字节*/
//			0x81,0x01,          /*Input(Constant,Array)*/
//			0x05,0x01,          /*Usage Page(Generic Desktop)*/
//			0x09,0x30,          /*Usage(X axis)*/
//			0x09,0x31,         /*Usage(Y axis)*/
//			0x09,0x38,          /*Usage(Wheel)*/
//			0x15,0x81,          /*Logical Minimum(-127)*/
//			0x25,0x7F,          /*Logical Maximum(127)*/
//			0x75,0x08,          /*Report Size(8)，每个1字节*/
//			0x95,0x03,          /*Report Count(3)，总共3字节*/
//			0x81,0x06,          /*Input(Variable, Relative)*/
//			0xC0,          /*End Collection*/
//		/*51*/
//		 /************************USB键盘部分报告描述符61**********************/
//	0x05, 0x01, // USAGE_PAGE (Generic Desktop)
//	0x09, 0x06, // USAGE (Keyboard)
//	0xa1, 0x01, // COLLECTION (Application)
//	0x85, 0x02,               /*  REPORT_ID (2)		     */
//	0x05, 0x07, //     USAGE_PAGE (Keyboard/Keypad)
//	0x19, 0xe0, //     USAGE_MINIMUM (Keyboard LeftControl)
//	0x29, 0xe7, //     USAGE_MAXIMUM (Keyboard Right GUI)
//	0x15, 0x00, //     LOGICAL_MINIMUM (0)
//	0x25, 0x01, //     LOGICAL_MAXIMUM (1)   
//	0x95, 0x08, //     REPORT_COUNT (8)
//	0x75, 0x01, //     REPORT_SIZE (1) 8 bits total
//	0x81, 0x02, //     INPUT (Data,Var,Abs)
//	/*byte 0,每一个bit表示一个功能键*/
//	
//	0x95, 0x01, //     REPORT_COUNT (1)
//	0x75, 0x08, //     REPORT_SIZE (8) 
//	0x81, 0x03, //     INPUT (Cnst,Var,Abs)
//	/*byte 1，常量必须是0*/
//	
//	0x95, 0x06, //   REPORT_COUNT (6)
//	0x75, 0x08, //   REPORT_SIZE (8)
//	0x15, 0x00,	//	最小值0
//	0x25, 0xFF, //   LOGICAL_MAXIMUM (255)
//	0x19, 0x00, //   USAGE_MINIMUM (Reserved (no event indicated))   
//	0x29, 0x65, //   USAGE_MAXIMUM (Keyboard Application)
//	0x81, 0x00, //     INPUT (Data,Ary,Abs)
//	/*byte 2-7，每个按键为8bits，总共6个按键*/
//	
//	0x95, 0x02, //   REPORT_COUNT (2)
//	0x75, 0x01, //   REPORT_SIZE (1)
//	0x05, 0x08, //   USAGE_PAGE (LEDs)
//	0x19, 0x01, //   USAGE_MINIMUM (Num Lock)
//	0x29, 0x02, //   USAGE_MAXIMUM (Caps Lock)
//	0x91, 0x02, //   OUTPUT (Data,Var,Abs)
//	0x95, 0x01, //   REPORT_COUNT (1)
//	0x75, 0x06, //   REPORT_SIZE (6)
//	0x91, 0x03, //   OUTPUT (Cnst,Var,Abs)
//	/*led灯输出，2个灯+6bit补足*/
//  0xc0      // END_COLLECTION    
//}; /* Joystick_ReportDescriptor */

/* USB Standard Device Descriptor */
const u8 Joystick_DeviceDescriptor[JOYSTICK_SIZ_DEVICE_DESC] =
  {
    0x12,                       /*bLength */
    USB_DEVICE_DESCRIPTOR_TYPE, /*bDescriptorType*/
    0x00,                       /*bcdUSB */
    0x02,
    0x00,                       /*bDeviceClass*/
    0x00,                       /*bDeviceSubClass*/
    0x00,                       /*bDeviceProtocol*/
    0x40,                       /*bMaxPacketSize40*/
    0x04,                       /*idVendor (0x1234)*/
    0x38,
    0x43,                       /*idProduct = 0x4321*/
    0x21,
    0x00,                       /*bcdDevice rel. 2.00*/
    0x02,
    1,                          /*Index of string descriptor describing
                                              manufacturer */
    2,                          /*Index of string descriptor describing
                                             product*/
    3,                          /*Index of string descriptor describing the
                                             device serial number */
    0x01                        /*bNumConfigurations*/
  }
  ; /* Joystick_DeviceDescriptor */


const u8 Joystick_ConfigDescriptor[JOYSTICK_SIZ_CONFIG_DESC] =
{
 /***************?????***********************/
 0x09,		//bLength??
 USB_CONFIGURATION_DESCRIPTOR_TYPE,		//bDescriptorType??
 //wTotalLength??
 JOYSTICK_SIZ_CONFIG_DESC,
 /* wTotalLength: Bytes returned */
 0x00,
 
 0x02,	//bNumInterfaces??
 0x01,	//bConfiguration??
 0x00,	//iConfigurationz??
 0x80,	//bmAttributes??
 0x32,	//bMaxPower??
 
 /*******************第一个接口描述符*************/
 0x09,	//bLength??
 0x04,	//bDescriptorType??
 0x00,	//bInterfaceNumber??
 0x00,	//bAlternateSetting??
 0x02,	//bNumEndpoints??
 0x03,	//bInterfaceClass??
 0x01,	//bInterfaceSubClass??
 0x01,	//bInterfaceProtocol??
 0x00,	//iConfiguration??
 
 /******************HID描述符************************/
 0x09,	//bLength??
 0x21,	//bDescriptorType??
 0x10,	//bcdHID??
 0x01,
 0x21,	//bCountyCode??
 0x01,	//bNumDescriptors??
 0x22,	//bDescriptorType??
 
 //下级描述符的长度，下级为键盘描述符
 sizeof(KeyboardReportDescriptor)&0xFF,
 (sizeof(KeyboardReportDescriptor)>>8)&0xFF,
 
 /**********************输入端描述符****************/
 0x07,	//bLength??
 0x05,	//bDescriptorType??
 0x81,	//bEndpointAddress??
 0x03,	//bmAttributes??
 0x10,	//wMaxPacketSize??
 0x00,
 0x0A,	//bInterval??
 
 /**********************输出端描述符***********************/
 0x07,	//bLength??
 0x05,	//bDescriptorType??
 0x01,	//bEndpointAddress??
 0x03,	//bmAttributes??
 0x10,	//wMaxPacketSize??
 0x00,
 0x0A,	//bInterval??

 /*******************第二个接口描述符*********************/
 0x09,	//bLength??
 0x04,	//bDescriptorType??
 0x01,	//bInterfaceNumber??
 0x00,	//bAlternateSetting??
 0x01,	//bNumEndpoints??
 0x03,	//bInterfaceClass??
 0x01,	//bInterfaceSubClass??
 0x02,	//bInterfaceProtocol??
 0x00,	//iConfiguration??
 
 /******************HID描述符************************/
 0x09,	//bLength??
 0x21,	//bDescriptorType??
 0x10,	//bcdHID??
 0x01,
 0x21,	//bCountyCode??
 0x01,	//bNumDescriptors??
 0x22,	//bDescriptorType??
 sizeof(MouseReportDescriptor)&0xFF,		//bDescriptorLength??
 (sizeof(MouseReportDescriptor)>>8)&0xFF,
 
 /**********************输入端点描述符***********************/
 0x07,	//bLength??
 0x05,	//bDescriptorType??
 0x82,	//bEndpointAddress??
 0x03,	//bmAttributes Interrupt endpoint
 0x10,	//wMaxPacketSize??
 0x00,
 0x0A 	//bInterval??
};

//USB键盘
const u8 KeyboardReportDescriptor[KP_ReportDescriptor_Size]=
{
	0x05, 0x01, // USAGE_PAGE (Generic Desktop)
	0x09, 0x06, // USAGE (Keyboard)
	0xa1, 0x01, // COLLECTION (Application)
	0x05, 0x07, //     USAGE_PAGE (Keyboard/Keypad)
	0x19, 0xe0, //     USAGE_MINIMUM (Keyboard LeftControl)
	0x29, 0xe7, //     USAGE_MAXIMUM (Keyboard Right GUI)
	0x15, 0x00, //     LOGICAL_MINIMUM (0)
	0x25, 0x01, //     LOGICAL_MAXIMUM (1)   
	0x95, 0x08, //     REPORT_COUNT (8)
	0x75, 0x01, //     REPORT_SIZE (1) 8 bits total
	0x81, 0x02, //     INPUT (Data,Var,Abs)
	/*byte 0,每一个bit表示一个功能键*/
	
	0x95, 0x01, //     REPORT_COUNT (1)
	0x75, 0x08, //     REPORT_SIZE (8) 
	0x81, 0x03, //     INPUT (Cnst,Var,Abs)
	/*byte 1，常量必须是0*/
	
	0x95, 0x06, //   REPORT_COUNT (6)
	0x75, 0x08, //   REPORT_SIZE (8)
	0x15, 0x00,	//	最小值0
	0x25, 0xFF, //   LOGICAL_MAXIMUM (255)
	0x19, 0x00, //   USAGE_MINIMUM (Reserved (no event indicated))   
	0x29, 0x65, //   USAGE_MAXIMUM (Keyboard Application)
	0x81, 0x00, //     INPUT (Data,Ary,Abs)
	/*byte 2-7，每个按键为8bits，总共6个按键*/
	
	0x95, 0x02, //   REPORT_COUNT (2)
	0x75, 0x01, //   REPORT_SIZE (1)
	0x05, 0x08, //   USAGE_PAGE (LEDs)
	0x19, 0x01, //   USAGE_MINIMUM (Num Lock)
	0x29, 0x02, //   USAGE_MAXIMUM (Caps Lock)
	0x91, 0x02, //   OUTPUT (Data,Var,Abs)
	0x95, 0x01, //   REPORT_COUNT (1)
	0x75, 0x06, //   REPORT_SIZE (6)
	0x91, 0x03, //   OUTPUT (Cnst,Var,Abs)
	/*led灯输出，2个灯+6bit补足*/
  0xc0      // END_COLLECTION    
}; /* Joystick_ReportDescriptor */


////USB鼠标
////USB鼠标报告描述符的定义
//const u8 MouseReportDescriptor[Mouse_ReportDescriptor_Size]=
//{
// 0x05, 0x01, // USAGE_PAGE (Generic Desktop)
// 0x09, 0x02, // USAGE (Mouse)
// 0xa1, 0x01, // COLLECTION (Application)
// 0x09, 0x01, //   USAGE (Pointer)
// 0xa1, 0x00, //   COLLECTION (Physical)
// 0x05, 0x09, //     USAGE_PAGE (Button)
// 0x19, 0x01, //     USAGE_MINIMUM (Button 1)
// 0x29, 0x03, //     USAGE_MAXIMUM (Button 3)
// 0x15, 0x00, //     LOGICAL_MINIMUM (0)
// 0x25, 0x01, //     LOGICAL_MAXIMUM (1)
// 0x95, 0x03, //     REPORT_COUNT (3)
// 0x75, 0x01, //     REPORT_SIZE (1)
// 0x81, 0x02, //     INPUT (Data,Var,Abs)
// 0x95, 0x01, //     REPORT_COUNT (1)
// 0x75, 0x05, //     REPORT_SIZE (5)
// 0x81, 0x03, //     INPUT (Cnst,Var,Abs)
// 0x05, 0x01, //     USAGE_PAGE (Generic Desktop)
// 0x09, 0x30, //     USAGE (X)
// 0x09, 0x31, //     USAGE (Y)
// 0x09, 0x38, //     USAGE (Wheel)
// 0x15, 0x81, //     LOGICAL_MINIMUM (-127)
// 0x25, 0x7f, //     LOGICAL_MAXIMUM (127)
// 0x75, 0x08, //     REPORT_SIZE (8)
// 0x95, 0x03, //     REPORT_COUNT (3)
// 0x81, 0x06, //     INPUT (Data,Var,Rel)
// 0xc0,       //   END_COLLECTION
// 0xc0        // END_COLLECTION
//};

const u8 MouseReportDescriptor[Mouse_ReportDescriptor_Size]=
{
    0x05,0x01,        /*  Usage Page(Generic Desktop)通用桌面设备*/
    0x09,0x02,          /*Usage(Mouse)*/
    0xA1,0x01,          /*Collection(Logical)*/ 
			0x09,0x01,          /*Usage(Pointer)*/
			0x05,0x09,          /*Usage Page(Buttons)*/
			0x19,0x01,          /*Usage Minimum(1)最小值-最大值,鼠标3个按钮所以1-3*/
			0x29,0x03,          /*Usage Maximum(3)*/
			0x15, 0x00,         /*Logical Minimum(0)-bit 0*/
			0x25,0x01,          /*Logical Maximum(1)-bit 1*/
			0x95,0x03,          /*Report Count(3) -数量3*/
			0x75,0x01,          /*Report Size(1)大小1bit*/ /*以上表示3个bit表示鼠标3个按钮情况，bit0：左键，bit1右键，bit2中间键*/
			0x81,0x02,          /*Input(Variable)输入，绝对值*/
			0x95,0x01,          /*Report Count(1)*/
			0x75,0x05,          /*Report Size(5)填充5bit，补足1字节  Byte0*/
			0x81,0x01,          /*Input(Constant,Array)*/
			0x05,0x01,          /*Usage Page(Generic Desktop)*/
			0x09,0x30,          /*Usage(X axis)*/
			0x09,0x31,         /*Usage(Y axis)*/
			0x09,0x38,          /*Usage(Wheel)*/
			0x15,0x81,          /*Logical Minimum(-127)*/
			0x25,0x7F,          /*Logical Maximum(127)*/
			0x75,0x08,          /*Report Size(8)，每个1字节*/
			0x95,0x03,          /*Report Count(3)，总共3字节  Byte1~3*/
			0x81,0x06,          /*Input(Variable, Relative)*/
			0xC0,          /*End Collection*/
};
///////////////////////////描述性报告结束////////////////////////////

/* USB String Descriptors (optional) */
const uint8_t Joystick_StringLangID[JOYSTICK_SIZ_STRING_LANGID] =
  {
    JOYSTICK_SIZ_STRING_LANGID,
    USB_STRING_DESCRIPTOR_TYPE,
    0x09,
    0x04
  }
  ; /* LangID = 0x0409: U.S. English */

const uint8_t Joystick_StringVendor[JOYSTICK_SIZ_STRING_VENDOR] =
  {
    JOYSTICK_SIZ_STRING_VENDOR, /* Size of Vendor string */
    USB_STRING_DESCRIPTOR_TYPE,  /* bDescriptorType*/
    /* Manufacturer: "STMicroelectronics" */
    'S', 0, 'T', 0, 'M', 0, 'i', 0, 'c', 0, 'r', 0, 'o', 0, 'e', 0,
    'l', 0, 'e', 0, 'c', 0, 't', 0, 'r', 0, 'o', 0, 'n', 0, 'i', 0,
    'c', 0, 's', 0
  };

const uint8_t Joystick_StringProduct[JOYSTICK_SIZ_STRING_PRODUCT] =
  {
    JOYSTICK_SIZ_STRING_PRODUCT,          /* bLength */
    USB_STRING_DESCRIPTOR_TYPE,        /* bDescriptorType */
    'S', 0, 'T', 0, 'M', 0, '3', 0, '2', 0, ' ', 0, 'J', 0,
    'o', 0, 'y', 0, 's', 0, 't', 0, 'i', 0, 'c', 0, 'k', 0
  };
uint8_t Joystick_StringSerial[JOYSTICK_SIZ_STRING_SERIAL] =
  {
    JOYSTICK_SIZ_STRING_SERIAL,           /* bLength */
    USB_STRING_DESCRIPTOR_TYPE,        /* bDescriptorType */
    'S', 0, 'T', 0, 'M', 0, '3', 0, '2', 0
  };

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/

