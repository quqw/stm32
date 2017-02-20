#include "common_types.h"
#include "comm_types.h"
#include "serial_transfer.h"
#include "comm_app.h"
#include "assist/delay.h"
#include "board/board.h"
void comm_set_mute(struct communicate* comm)
{
	comm->state|=COMM_STSTE_MUTE;
}

void comm_set_active(struct communicate* comm)
{
	comm->state&=~COMM_STSTE_MUTE;
}

void comm_get_data_callback(struct communicate* comm,void* data,int len)
{
	if(len<=0)
		return;
	if(comm->trans->push_data)//协议第一层获取数据，对于usb一类的协议，这个操作已经在协议栈内完成
	{
		if(!comm->trans->push_data(comm->trans,data,len))
			return;
	}
	if(!comm->protocol_parse(comm))
		return;
	if(!comm->cmd_parse(comm))
		return;
	if(comm->trans->send)
		comm->trans->send(comm->trans,(int8u*)&comm->packet,comm->data_length);
}

//协议解析。
BOOL comm_protocol_parse(struct communicate* comm)
{
	struct comm_packet *packet;
	int8u cpu_id[8];
	int8u broadcast_addr[]={0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff};
	int data_index=0,src_addr_len=0,dest_addr_len=0;
	int8u dest_addr[8],src_addr[8],temp;
	BOOL mute=FALSE;
	packet=(struct comm_packet*)comm->packet;
	#ifdef HAVE_LOCAL_ADDRESS
	board_get_cpuid(cpu_id);
	#endif
	if(comm->trans && comm->trans->copy_out_data)
			comm->data_length=comm->trans->copy_out_data(comm->trans,&comm->packet);
	else return FALSE;
	if(comm->data_length<=0)
		return FALSE;
	comm->phase=COMM_PHASE_RECEIVE_END;
	if((packet->head.address_flag &FCF_LONGDST)==FCF_LONGDST)
	{
		if(memcmp(packet->data,broadcast_addr,8)!=0)//地址核对
		{
			#ifdef HAVE_LOCAL_ADDRESS
			if(memcmp(packet->data,cpu_id,8)!=0)//地址本机核对
				return FALSE;
			#else
			return FALSE;
			#endif
		}
		else if(comm->state&COMM_STSTE_MUTE)//MUTE状态广播指令不回应
			mute=TRUE;
		memcpy(dest_addr,packet->data,8);
		data_index=8;
		dest_addr_len=8;
	}
	else if((packet->head.address_flag &FCF_SHORTDST)==FCF_SHORTDST)
	{
		if(memcmp(packet->data,broadcast_addr,4)!=0)//地址核对
		{
			#ifdef HAVE_LOCAL_ADDRESS
			if(memcmp(packet->data,cpu_id,4)!=0)//地址本机核对
				return FALSE;
			#else
			return FALSE;
			#endif
		}
		else if(comm->state&COMM_STSTE_MUTE)//MUTE状态广播指令不回应
			mute=TRUE;
			
		memcpy(dest_addr,packet->data,4);
		data_index=4;
		dest_addr_len=4;
	}
	#ifdef HAVE_LOCAL_ADDRESS//对于有本机地址的设备，不接受无目标地址数据包
	else
	{
		return FALSE;
	}
	#endif
	
	if((packet->head.address_flag &FCF_LONGSRC)==FCF_LONGSRC)
	{
		memcpy(src_addr,packet->data+data_index,8);
		data_index+=8;
		src_addr_len=8;
	}
	else if((packet->head.address_flag &FCF_SHORTSRC)==FCF_SHORTSRC)
	{
		memcpy(src_addr,packet->data+data_index,4);
		data_index+=4;
		src_addr_len=4;
	}
	comm->cmd=packet->data+data_index;
	if((((struct cmd_packet*)(comm->cmd))->ins!=0x03 || 
		0!=((struct cmd_packet*)(comm->cmd))->cla)&& mute==TRUE)//mute状态，非唤醒指令则不需要执行
		return FALSE;
		
	//交换地址
	if(src_addr_len)
		memcpy(packet->data,src_addr,src_addr_len);
	if(dest_addr_len)
		memcpy(packet->data+src_addr_len,dest_addr,dest_addr_len);
	temp=packet->head.address_flag&0x0f;
	packet->head.address_flag>>=4;
	packet->head.address_flag|=(temp<<4);
	packet->head.length=data_index+COMM_PACKET_HEAD_LENGTH;//保存好头长度，等待回应的时候使用
	return TRUE;
}


#include "cmd.h"
BOOL comm_cmd_run(struct communicate* comm)
{
//	static int last_toggle=-1;
	struct cmd_packet* cmd=comm->cmd;
	struct comm_packet *packet=(struct comm_packet*)comm->packet;
	BOOL need_response=TRUE;
	if(comm->phase!=COMM_PHASE_RECEIVE_END)
		return FALSE;
	comm->phase=COMM_PHASE_EXCUDING;
//	if((comm->packet.head.toggle!=last_toggle))
//	{
		need_response=run_cmd(cmd);
//		last_toggle=comm->packet.head.toggle;
//	}
//	else
//	{
//		comm->cmd->flag |=CMD_ERROR_FLAG;
//		comm->cmd->len=4;
//		*(int32u*)(comm->cmd->data)=ERR_CMD_REPEAT;
//	}
	if(cmd->flag&CMD_MUTE_FLAG)
		need_response= FALSE;
	comm->phase=COMM_PHASE_RESPONSE;
	cmd->ins |=INS_RESPONSE_FLAG;
	cmd->flag |= CMD_MUTE_FLAG;
	packet->head.length+=cmd->len+CMD_PACKET_HEAD_LENGTH;
	comm->data_length=packet->head.length;
	return need_response;
}
