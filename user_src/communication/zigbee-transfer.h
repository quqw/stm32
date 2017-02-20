#ifndef __FUNC_ZIGBEE_H
#define __FUNC_ZIGBEE_H
#include "types.h"
#include "comm_types.h"
#define MY_NODE_ID      0x1234
#define SRC_PAN_ID      0x1235  
#define DEST_PAN_ID     0x1234
#define BROADCAST_PAN_ID        0xffff
#define BROADCAST_SHORT_ADDR        0xffff
#define DEST_NODE_ID    0
#define ZIGBEE_CHANNEL ST_MIN_802_15_4_CHANNEL_NUMBER
#define ENDDEVICE_SEARCH        0x01



#define PACKKET_PROCESS_END           0
#define PACKKET_BEING_RECEIVE         1
#define PACKKET_BEING_PROCESS           2
#define PACKKET_RECEIVED_NOT_PROCESSED           3

typedef struct {
  int8u packetBeingProcessed;
  int8u packet[128];
  bool ackFramePendingSet;
  int32u time;
  int16u errors;
  int8s rssi;
  int8u lqi;
} RECEIVE_DATA;


void zigbee_tick(struct transfer* trans,int16u ticks);
int zigbee_copy_out(struct transfer* trans,void* dest_buff);
BOOL zigbee_send(struct transfer* trans,int8u *data,int8u length);
BOOL zigbee_push_bytes(struct transfer* trans,int8u *data,int8u length);
void zigbee_reset(struct transfer* trans);
int zigbee_read_rx(int8u *data_out,int8u len);
#endif

