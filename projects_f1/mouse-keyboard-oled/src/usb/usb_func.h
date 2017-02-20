#ifndef USB_FUNC_H
#define USB_FUNC_H

BOOL hid_send_keyboard_char(int8u  asic_byte);
BOOL hid_send_mouse(int8u key,int8s x,int8s y);
BOOL hid_send_keyboard(int8u* keys);
#endif

