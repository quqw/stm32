#ifndef COMM_APPS_H
#define COMM_APPS_H
#include "comm_types.h"

void comm_get_data_callback(struct communicate* comm,void* data,int len);
BOOL comm_protocol_parse(struct communicate* comm);
BOOL comm_cmd_run(struct communicate* comm);
void comm_set_mute(struct communicate* comm);
void comm_set_active(struct communicate* comm);


#endif

