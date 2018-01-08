#include "types.h"
#include "board/board.h"
#include "comm_types.h"
#include "comm_transfer.h"
#include "comm_app.h"
#include "platform/delay.h"
#include "cmd.h"
#include "comm_app.h"
void comm_get_data_callback(struct communicate* comm,void* data,int len)
{
	if(!comm->trans->push_data||!comm->trans->push_data(comm->trans,data,len))
		return;
	if(!comm->protocol_parse(comm))
		return;
	board_led1_toggle();
	if(!comm->cmd_parse(comm))
		return;
	comm->trans->send(comm->trans,(int8u*)&comm->packet,comm->data_length);
}

BOOL comm_protocol_parse(struct communicate* comm)
{
	int len=comm->trans->copy_out_data(comm->trans,&comm->packet);
	int data_index=0,src_addr_len=0,dest_addr_len=0;
	int8u dest_addr[8],src_addr[8],temp;
	if(len<=0)
		return FALSE;
	comm->data_length=len;
	comm->state=COMM_STATE_RECEIVE_END;
	
	if((comm->packet.head.address_flag &FCF_LONGDST)==FCF_LONGDST)
	{
//		if(memcmp(comm->packet.data,LOCAL_LONG_ADDRESS,8)!=0)
//			return FALSE;
		memcpy(dest_addr,comm->packet.data,8);
		data_index=8;
		dest_addr_len=8;
	}
	else if((comm->packet.head.address_flag &FCF_SHORTDST)==FCF_SHORTDST)
	{
//		if(memcmp(comm->packet.data,LOCAL_SHORT_ADDRESS,4)!=0)
//			return FALSE;
		memcpy(dest_addr,comm->packet.data,4);
		data_index=4;
		dest_addr_len=4;
	}
	if((comm->packet.head.address_flag &FCF_LONGSRC)==FCF_LONGSRC)
	{
		memcpy(src_addr,comm->packet.data+data_index,8);
		data_index+=8;
		src_addr_len=8;
	}
	else if((comm->packet.head.address_flag &FCF_SHORTSRC)==FCF_SHORTSRC)
	{
		memcpy(src_addr,comm->packet.data+data_index,4);
		data_index+=4;
		src_addr_len=4;
	}

	//½»»»µØÖ·
	if(src_addr_len)
		memcpy(comm->packet.data,src_addr,src_addr_len);
	if(dest_addr_len)
		memcpy(comm->packet.data+src_addr_len,dest_addr,dest_addr_len);
	temp=comm->packet.head.address_flag&0x0f;
	comm->packet.head.address_flag>>=4;
	comm->packet.head.address_flag|=(temp<<4);
	
	comm->cmd=(struct cmd_packet*)(comm->packet.data+data_index);
	return TRUE;
	
}

BOOL comm_cmd_run(struct communicate* comm)
{
	BOOL need_response=FALSE;
	if(comm->state!=COMM_STATE_RECEIVE_END)
		return FALSE;
	comm->state=COMM_STATE_EXCUDING;
	switch(comm->cmd->cla)
	{
		case CLA_MOUSE_KEYBOARD_CTL:
			need_response= run_mkc_cmd(comm->cmd);
			break;
		default:
			comm->cmd->flag|=CMD_ERROR_FLAG;
			need_response=TRUE;
			break;
	}
	if(comm->cmd->flag&CMD_MUTE_MUTE_FLAG)
		return FALSE;
	comm->state=COMM_STATE_RESPONSE;
	comm->cmd->ins |=INS_RESPONSE_FLAG;
	comm->cmd->flag |= CMD_MUTE_MUTE_FLAG;
	comm->packet.head.length=comm->cmd->len+CMD_PACKET_HEAD_LENGTH+COMM_PACKET_HEAD_LENGTH;
	comm->data_length=comm->packet.head.length;
	return need_response;
}
