#include "includes.h"
#include "st_uart.h"
#include "assist/bytequeue.h"
#define PTR_ST_COMM 		((st_uart_t*)(this))
#define PTR_COMM 			((comm_port_t*)(this))
#define PTR_ST_UART_PORT	((USART_TypeDef*)(PTR_COMM->port_addr))
#define PTR_ISR_UART_PORT(x)	((USART_TypeDef*)(comm_uart[x]->port_addr))
#define st_uart_MAX	5
static comm_port_t* comm_uart[5];
void st_uart_init(st_uart_t *this,USART_InitTypeDef *init)
{
	switch((int32u)(PTR_COMM->port_addr))
	{
		
		case (int32u)USART1:
		{
			RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1, ENABLE);
			comm_uart[0]=PTR_COMM;
			break;
		}
		case (int32u)USART2:
		{
			RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2, ENABLE);
			comm_uart[1]=PTR_COMM;
			break;
		}
		case (int32u)USART3:
		{
			RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART3, ENABLE);
			comm_uart[2]=PTR_COMM;
			break;
		}
		case (int32u)UART4:		
		{
			RCC_APB1PeriphClockCmd(RCC_APB1Periph_UART4, ENABLE);
			comm_uart[3]=PTR_COMM;
			break;
		}
		case (int32u)UART5:		
		{
			RCC_APB1PeriphClockCmd(RCC_APB1Periph_UART5, ENABLE);
			comm_uart[4]=PTR_COMM;
			break;
		}
	}
	PTR_COMM->send_byte=st_uart_send_byte;
	PTR_COMM->send_byte_sync=st_uart_send_byte_sync;
	PTR_COMM->send_bytes=st_uart_send_bytes;
	PTR_COMM->send_bytes_sync=st_uart_send_bytes_sync;
	PTR_COMM->send_string_sync=st_uart_send_string_sync;
	PTR_COMM->send_string=st_uart_send_string;
	PTR_COMM->read_byte=st_uart_read_byte;
	PTR_COMM->read_bytes=st_uart_read_bytes;
	PTR_COMM->start_rx=st_uart_start_rx;
	PTR_COMM->start_tx=st_uart_start_tx;
	PTR_COMM->stop_rx=st_uart_stop_rx;
	PTR_COMM->stop_tx=st_uart_stop_tx;
	PTR_COMM->clear_rx=st_uart_clear;
	USART_Init(PTR_ST_UART_PORT, init);
	USART_Cmd(PTR_ST_UART_PORT, ENABLE);
}

void st_uart_set_nvic(st_uart_t* this,uint8_t pre_priority,uint8_t sub_priority)
{
	NVIC_InitTypeDef   nvic_init;
	nvic_init.NVIC_IRQChannelPreemptionPriority = pre_priority;
	nvic_init.NVIC_IRQChannelSubPriority        = sub_priority;
	nvic_init.NVIC_IRQChannelCmd                = ENABLE;
	switch((int32u)(PTR_COMM->port_addr))
	{
		case (int32u)USART1:
			nvic_init.NVIC_IRQChannel                   = USART1_IRQn;
			break;
		case (int32u)USART2:
			nvic_init.NVIC_IRQChannel                   = USART2_IRQn;
			break;
		case (int32u)USART3:
			nvic_init.NVIC_IRQChannel                   = USART3_IRQn;
			break;
	#ifdef STM32F10X_HD
		case (int32u)UART4:		
			nvic_init.NVIC_IRQChannel                   = UART4_IRQn;
			break;
		case (int32u)UART5:		
			nvic_init.NVIC_IRQChannel                   = UART5_IRQn;
			break;
	#endif
	}
	NVIC_Init(&nvic_init); 
}

int st_uart_send_byte_sync(comm_port_t *this,int8u b)
{
	while((PTR_ST_UART_PORT->SR & USART_FLAG_TXE)==0);
	PTR_ST_UART_PORT->DR=b;
	//多读一下SR，为了让TC清零。不然第一个字节发不出去,因为TC初始值为1。
	while(((PTR_ST_UART_PORT->SR & USART_FLAG_TXE)==0) || ((PTR_ST_UART_PORT->SR & USART_FLAG_TC) == 0));
	return 1;
}

int st_uart_send_byte(comm_port_t *this,int8u b)
{
	st_uart_send_bytes(this,&b,1);
	return 1;
}

int st_uart_send_bytes(comm_port_t *this,const void* buf,int32u length)
{
	if(length>COMM_BUF_MAX_SIZE)
		length= COMM_BUF_MAX_SIZE;
	while((PTR_ST_UART_PORT->SR & USART_FLAG_TXE)==0|| (PTR_ST_UART_PORT->SR & USART_FLAG_TC) == 0);
	memcpy(PTR_COMM->tx_queue,buf,length);
	PTR_COMM->tx_index=0;
	PTR_COMM->tx_size=length;
	st_uart_start_tx(this);
	return length;
}
int st_uart_send_bytes_sync(comm_port_t *this,const void* buf,int32u length)
{
	int i;
	const int8u* p=buf;
	if(PTR_COMM->pre_tx)
		PTR_COMM->pre_tx(this);
	for(i=0;i<length;i++)
	{
		while((PTR_ST_UART_PORT->SR & USART_FLAG_TXE)==0);
		st_uart_send_byte(this,*p);
		p++;
	}
	while((PTR_ST_UART_PORT->SR & USART_FLAG_TXE)==0 || (PTR_ST_UART_PORT->SR & USART_FLAG_TC) == 0);//多读一下SR，为了让TC清零
	if(PTR_COMM->pre_rx)
		PTR_COMM->pre_rx(this);
	return length;
}

int st_uart_send_string(comm_port_t *this,string s)
{
	int length=strlen(s);
	return st_uart_send_bytes(this,s,length);
}

int st_uart_send_string_sync(comm_port_t *this,string s)
{
	int len=strlen(s);
	return st_uart_send_bytes_sync(this,s,len);
}

int st_uart_read_byte(comm_port_t *this,void* out)
{
	return byte_queue_receive((byte_queue_t*)PTR_COMM->rx_queue,(uint8_t *)out);
}

int st_uart_read_bytes(comm_port_t *this,void* out,uint32_t length)
{
	return byte_queue_receive_bytes((byte_queue_t*)PTR_COMM->rx_queue,(uint8_t*)out,length);
}

void st_uart_start_rx(comm_port_t *this)
{
	//清空缓冲区,开始接收
	USART_ITConfig(PTR_ST_UART_PORT,USART_IT_RXNE,DISABLE);
	st_uart_clear(this);
	if(PTR_COMM->pre_rx)
		PTR_COMM->pre_rx(this);
	USART_ITConfig(PTR_ST_UART_PORT,USART_IT_RXNE,ENABLE);
}
void st_uart_stop_rx(comm_port_t *this)
{
	USART_ITConfig(PTR_ST_UART_PORT,USART_IT_RXNE,DISABLE);
	st_uart_clear(this);
}

void st_uart_clear(comm_port_t *this)
{
	byte_queue_clear(PTR_COMM->rx_queue);
}
void st_uart_start_tx(comm_port_t *this)
{
	if(PTR_COMM->pre_tx)
		PTR_COMM->pre_tx(this);
	USART_ITConfig(PTR_ST_UART_PORT,USART_IT_TXE,ENABLE);
}
void st_uart_stop_tx(comm_port_t *this)
{
	USART_ITConfig(PTR_ST_UART_PORT,USART_IT_TC,DISABLE);
	if(PTR_COMM->pre_rx)
		PTR_COMM->pre_rx(this);
}
//#include "communication/comm_app.h"
void st_uart_preapre_tx(comm_port_t *this)
{
	if(PTR_COMM->pre_tx)
		PTR_COMM->pre_tx(this);
}
void st_uart_preapre_rx(comm_port_t *this)
{
	if(PTR_COMM->pre_rx)
		PTR_COMM->pre_rx(this);
}
void USART1_IRQHandler(void)
{
	if(UART_CR1_BIT(PTR_ISR_UART_PORT(0),BIT_RXNEIE_OFFSET) && 
		(UART_SR_BIT(PTR_ISR_UART_PORT(0),BIT_RXNE_OFFSET) || UART_SR_BIT(PTR_ISR_UART_PORT(0),BIT_RXORE_OFFSET)))
	{
		byte_queue_put(comm_uart[0]->rx_queue, PTR_ISR_UART_PORT(2)->DR);
	}
	else if(UART_CR1_BIT(PTR_ISR_UART_PORT(0),BIT_TXEIE_OFFSET) && UART_SR_BIT(PTR_ISR_UART_PORT(0),BIT_TXE_OFFSET))
	{
		if(comm_uart[0]->tx_index>=comm_uart[0]->tx_size-1)//最后一个字节，关闭TXE中断
		{
			UART_CR1_BIT(PTR_ISR_UART_PORT(0),BIT_TXEIE_OFFSET)=0;//关闭TXIE中断
			UART_CR1_BIT(PTR_ISR_UART_PORT(0),BIT_TCIE_OFFSET)=1;//开TCIE中断，目的是对于485这种通信，要等到最后一个字节发送完毕才能操作读写控制脚
			PTR_ISR_UART_PORT(0)->DR=comm_uart[0]->tx_queue[comm_uart[0]->tx_size-1];
		}
		else
		{
			PTR_ISR_UART_PORT(0)->DR=comm_uart[0]->tx_queue[comm_uart[0]->tx_index];
			comm_uart[0]->tx_index++;
		}
	}
	else if(UART_CR1_BIT(PTR_ISR_UART_PORT(0),BIT_TCIE_OFFSET) && UART_SR_BIT(PTR_ISR_UART_PORT(0),BIT_TC_OFFSET))
	{
		st_uart_stop_tx(comm_uart[0]);
	}
}

void USART2_IRQHandler(void)
{
	if(UART_CR1_BIT(PTR_ISR_UART_PORT(1),BIT_RXNEIE_OFFSET) && 
		(UART_SR_BIT(PTR_ISR_UART_PORT(1),BIT_RXNE_OFFSET) || UART_SR_BIT(PTR_ISR_UART_PORT(1),BIT_RXORE_OFFSET)))
	{
		byte_queue_put(comm_uart[1]->rx_queue, PTR_ISR_UART_PORT(1)->DR);
	}
	else if(UART_CR1_BIT(PTR_ISR_UART_PORT(1),BIT_TXEIE_OFFSET) && UART_SR_BIT(PTR_ISR_UART_PORT(1),BIT_TXE_OFFSET))
	{
		if(comm_uart[1]->tx_index>=comm_uart[1]->tx_size-1)//最后一个字节，关闭TXE中断
		{
			UART_CR1_BIT(PTR_ISR_UART_PORT(1),BIT_TXEIE_OFFSET)=1;//关闭TXIE中断
			UART_CR1_BIT(PTR_ISR_UART_PORT(1),BIT_TCIE_OFFSET)=1;//开TCIE中断，目的是对于485这种通信，要等到最后一个字节发送完毕才能操作读写控制脚
			PTR_ISR_UART_PORT(1)->DR=comm_uart[1]->tx_queue[comm_uart[1]->tx_size-1];
		}
		else
		{
			PTR_ISR_UART_PORT(1)->DR=comm_uart[1]->tx_queue[comm_uart[1]->tx_index];
			comm_uart[1]->tx_index++;
		}
	}
	else if(UART_CR1_BIT(PTR_ISR_UART_PORT(1),BIT_TCIE_OFFSET) && UART_SR_BIT(PTR_ISR_UART_PORT(1),BIT_TC_OFFSET))
	{
		st_uart_stop_tx(comm_uart[1]);
	}
}
void USART3_IRQHandler(void)
{
	uint8_t by;
	if(UART_CR1_BIT(PTR_ISR_UART_PORT(2),BIT_RXNEIE_OFFSET) && 
		(UART_SR_BIT(PTR_ISR_UART_PORT(2),BIT_RXNE_OFFSET) || UART_SR_BIT(PTR_ISR_UART_PORT(2),BIT_RXORE_OFFSET)))
	{
		byte_queue_put(comm_uart[2]->rx_queue, PTR_ISR_UART_PORT(2)->DR);
		//byte_queue_receive(comm_uart[2]->rx_queue, &by);
	}
	else if(UART_CR1_BIT(PTR_ISR_UART_PORT(2),BIT_TXEIE_OFFSET) && UART_SR_BIT(PTR_ISR_UART_PORT(2),BIT_TXE_OFFSET))
	{
		if(comm_uart[2]->tx_index>=comm_uart[2]->tx_size-1)//最后一个字节，关闭TXE中断
		{
			UART_CR1_BIT(PTR_ISR_UART_PORT(2),BIT_TXEIE_OFFSET)=2;//关闭TXIE中断
			UART_CR1_BIT(PTR_ISR_UART_PORT(2),BIT_TCIE_OFFSET)=1;//开TCIE中断，目的是对于485这种通信，要等到最后一个字节发送完毕才能操作读写控制脚
			PTR_ISR_UART_PORT(2)->DR=comm_uart[2]->tx_queue[comm_uart[2]->tx_size-1];
		}
		else
		{
			PTR_ISR_UART_PORT(2)->DR=comm_uart[2]->tx_queue[comm_uart[2]->tx_index];
			comm_uart[2]->tx_index++;
		}
	}
	else if(UART_CR1_BIT(PTR_ISR_UART_PORT(2),BIT_TCIE_OFFSET) && UART_SR_BIT(PTR_ISR_UART_PORT(2),BIT_TC_OFFSET))
	{
		st_uart_stop_tx(comm_uart[2]);
	}
}



void UART5_IRQHandler(void)
{
	uint8_t by;
	if(UART_CR1_BIT(PTR_ISR_UART_PORT(4),BIT_RXNEIE_OFFSET) && 
		(UART_SR_BIT(PTR_ISR_UART_PORT(4),BIT_RXNE_OFFSET) || UART_SR_BIT(PTR_ISR_UART_PORT(4),BIT_RXORE_OFFSET)))
	{
		byte_queue_put(comm_uart[4]->rx_queue, PTR_ISR_UART_PORT(4)->DR);
		//byte_queue_receive(comm_uart[2]->rx_queue, &by);
	}
	else if(UART_CR1_BIT(PTR_ISR_UART_PORT(4),BIT_TXEIE_OFFSET) && UART_SR_BIT(PTR_ISR_UART_PORT(4),BIT_TXE_OFFSET))
	{
		if(comm_uart[4]->tx_index>=comm_uart[4]->tx_size-1)//最后一个字节，关闭TXE中断
		{
			UART_CR1_BIT(PTR_ISR_UART_PORT(4),BIT_TXEIE_OFFSET)=2;//关闭TXIE中断
			UART_CR1_BIT(PTR_ISR_UART_PORT(4),BIT_TCIE_OFFSET)=1;//开TCIE中断，目的是对于485这种通信，要等到最后一个字节发送完毕才能操作读写控制脚
			PTR_ISR_UART_PORT(4)->DR=comm_uart[4]->tx_queue[comm_uart[4]->tx_size-1];
		}
		else
		{
			PTR_ISR_UART_PORT(4)->DR=comm_uart[4]->tx_queue[comm_uart[4]->tx_index];
			comm_uart[4]->tx_index++;
		}
	}
	else if(UART_CR1_BIT(PTR_ISR_UART_PORT(4),BIT_TCIE_OFFSET) && UART_SR_BIT(PTR_ISR_UART_PORT(4),BIT_TC_OFFSET))
	{
		st_uart_stop_tx(comm_uart[4]);
	}
}


