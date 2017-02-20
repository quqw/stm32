#ifndef COMM_APPS_H
#define COMM_APPS_H
#include "comm_types.h"

#define COMM_STATE_RECEIVING	1
#define COMM_STATE_RECEIVE_END	2
#define COMM_STATE_EXCUDING			3
#define COMM_STATE_RESPONSE		4

void comm_get_data_callback(struct communicate* comm,void* data,int len);
BOOL comm_protocol_parse(struct communicate* comm);
BOOL comm_cmd_run(struct communicate* comm);



#endif

