#ifndef USB_TRANSFER_H
#define USB_TRANSFER_H

#include "comm_types.h"
BOOL usb_push_bytes(struct transfer* trans,int8u *data,int8u length);
int usb_copy_out(struct transfer* trans,void* dest_buff);
BOOL usb_send(struct transfer* trans,int8u *data,int8u length);





#endif

