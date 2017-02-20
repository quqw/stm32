#ifndef __TYPES_H
#define __TYPES_H
#include    <stdio.h>
#include    <string.h>
#include    <ctype.h>
#include    <stdlib.h>
#include    <stdarg.h>
#include <stdint.h>
//#include "stm32f10x.h"
typedef const char* string;
typedef const char* String;
typedef char* StringBuffer;
typedef unsigned short wchar;
typedef const unsigned short* wstring;

typedef unsigned char BYTE;
typedef unsigned char byte;
typedef unsigned short WORD;
typedef unsigned long DWORD;
//typedef unsigned char UInt8;
//typedef unsigned short UInt16;
//typedef unsigned long UInt32;
typedef unsigned char int8u;
typedef unsigned short int16u;
typedef unsigned int int32u;
typedef signed char int8s;
typedef short int16s;
typedef int int32s;

typedef volatile int32u  vint32u;
typedef volatile int16u vint16u;
typedef volatile int8u  vint8u;

typedef unsigned char UCHAR;
typedef char    CHAR;

typedef unsigned short USHORT;
typedef short   SHORT;

typedef unsigned long ULONG;
typedef long    LONG;

typedef struct _DATETIME{
	int8u year;
	int8u month;
	int8u day;
	int8u hour;
	int8u minute;
	int8u second;
} DATETIME;

//typedef enum
//{
//	false=0,
//	true=1
//}bool;
#define bool int8u
#define BOOL int8u 
#define TRUE 1
#define FALSE 0



/////////////////////驱动 types///////////////////////////////////////////////////////////
typedef struct flash
{
	int (*read)(struct flash* this,uint32_t addr,void* out,uint32_t len);
	int (*write)(struct flash* this,uint32_t addr,void* dat,uint32_t len);
	BOOL (*erase_sector)(struct flash* this,uint32_t addr,uint32_t sector_count);
	uint32_t (*check_black)(struct flash* this,uint32_t addr,uint32_t len);
	BOOL (*check_data)(struct flash* this,uint32_t addr,void* chk_dat,uint32_t len);
}flash_t;

typedef struct spi
{
	void (*trans)(void *this,void*tx_data,uint16_t tx_size,void*rx_data,uint16_t rx_size);
}spi_t;


#define	SPI_SLAVE_MAX_SIZE	256
typedef struct spi_slave
{
	uint8_t (*rw_byte)(void* this,uint8_t b);
	uint32_t (*trans)(void *this,void*rx_data,void*tx_data,uint16_t tx_size,uint32_t timeout_us);
	BOOL (*is_trans_end)(void);
	void (*reset)(void *this);//slave模式,由于控制权在主机,可能导致从机状态异常,需要有初始化机制
	void (*enable)(void *this);
	void (*disable)(void *this);
}spi_slave_t;


typedef struct byte_queue
{
	volatile uint16_t sof;
	volatile uint16_t eof;
	volatile uint16_t length;//当前长度
	volatile uint16_t size;//总大小
	int8u* buff;
}byte_queue_t;

typedef enum
{
  TRIGGER_RISING = 0x08,
  TRIGGER_FALLING = 0x0C,  
  TRIGGER_BOTH = 0x10
}exti_rigger_t;

typedef struct gpio
{
	void* port_addr;
	uint32_t pin_num;//0~15
	void (*set)(struct gpio* this);
	void (*reset)(struct gpio* this);
	void (*toggle)(struct gpio* this);
	uint16_t (*read_input)(struct gpio* this);
	uint16_t (*read_output)(struct gpio* this);
	void (*set_isr_trigger)(struct gpio* this,exti_rigger_t trigger);
	void (*enable_isr)(struct gpio* this);
	void (*disable_isr)(struct gpio* this);
}gpio_t;

typedef struct dac
{
	uint16_t mv_ref;
	uint16_t max_val;
	void (*enable)(void* this);
	void (*disable)(void* this);
	BOOL (*set_val)(void* this,uint16_t val);
	BOOL (*set_voltage)(void* this,uint16_t mV);
	uint16_t (*val_to_voltage)(void* this,uint16_t val);
}dac_t;

typedef struct adc
{
	uint16_t mv_ref;
	uint16_t max_val;
	void (*enable)(void* this);
	void (*disable)(void* this);
	BOOL (*get_val)(void* this,uint16_t *val);
	BOOL (*get_voltage)(void* this,uint16_t *mV);
	uint16_t (*val_to_voltage)(void* this,uint16_t val);
	BOOL (*get_all_conv_val)(void* this,uint16_t *val,uint8_t* count);
}adc_t;

#define COMM_BUF_MAX_SIZE	256 //通信端口接收缓冲区大小
typedef struct comm_port
{
	void* port_addr;
	//uint32_t port_num;
	int (*read_byte)(struct comm_port*this,void* buf_read);//读取
	int (*read_bytes)(struct comm_port*this,void* buf_read,uint32_t length);//读取
	int (*send_byte)(struct comm_port*this,uint8_t by);
	int (*send_byte_sync)(struct comm_port*this,uint8_t by);
	int (*send_bytes_sync)(struct comm_port*this,const void* buf_send,uint32_t length);//同步发送
	int (*send_bytes)(struct comm_port*this,const void* buf_send,uint32_t length);//异步发送
	int (*send_string_sync)(struct comm_port*this,string buf_send);//同步发送
	int (*send_string)(struct comm_port*this,string buf_send);//异步发送
	//int (*prepare_async_tx_data)(void*this,int length,void* buf_send);//异步发送准备发送数据
	void (*start_rx)(struct comm_port*this);//开始接收
	void (*start_tx)(struct comm_port*this);//开始发送
	void (*stop_rx)(struct comm_port*this);//开始接收
	void (*stop_tx)(struct comm_port*this);//开始发送
	void (*clear_rx)(struct comm_port* this);//清空接收区缓存
	void (*pre_rx)(struct comm_port*this);//接收前准备,主要用于485类似通信
	void (*pre_tx)(struct comm_port*this);//发送前准备
	byte_queue_t *rx_queue;//记住volatile陷阱,指针一定要是volatile,才能保证内容是volatile
	uint16_t tx_size;
	uint16_t tx_index; 
	uint8_t tx_queue[COMM_BUF_MAX_SIZE];
}comm_port_t;






/////////////////////////////////STM32 Register Map////////////////////////////
#define PTR_BITOFFSET(ptr)          (((u32)(ptr))&0xFFFFF)
//#define PTR_BITADDR(ptr,bitIndex)   ((vu32*)(SRAM_BB_BASE+PTR_BITOFFSET(ptr)*32+(bitIndex)*4))

#define DISABLE_ISR(IRQ)    NVIC->ICER[IRQ >> 0x05] =(u32)0x01 << (IRQ & 0x1F);
#define ENABLE_ISR(IRQ)     NVIC->ISER[IRQ >> 0x05] =(u32)0x01 << (IRQ & 0x1F);

//具体实现思想,参考<<CM3权威指南>>第五章(87页~92页).M4同M3类似,只是寄存器地址变了.
//IO口操作宏定义
#define BITBAND(addr, bitnum) (((addr) & 0xF0000000)+0x2000000+(((addr) &0xFFFFF)<<5)+((bitnum)<<2)) 
#define MEM_VALUE(addr)  			*((vu32*)(addr)) 
#define BIT_VALUE(addr, bitnum)   	MEM_VALUE(BITBAND((addr), (bitnum))) 
#define BIT_ADDR(addr, bitnum) 		((vu32*)BITBAND((addr), (bitnum)))
#define PTR_BITADDR(addr, bitnum)	BIT_ADDR(addr, bitnum) 	


#define EXTI_IMR_ADDR	(EXTI_BASE+0x00)
#define EXTI_RTSR_ADDR	(EXTI_BASE+0x08)
#define EXTI_FTSR_ADDR	(EXTI_BASE+0x0c)
#define EXTI_IMR_BIT(n)		BIT_VALUE(EXTI_IMR_ADDR,n)
#define EXTI_RTSR_BIT(n)	BIT_VALUE(EXTI_RTSR_ADDR,n)
#define EXTI_FTSR_BIT(n)	BIT_VALUE(EXTI_FTSR_ADDR,n)

	
/////////////UART/////////////////
#define BIT_RXNE_OFFSET		5
#define BIT_TC_OFFSET		6	
#define BIT_TXE_OFFSET		7
#define BIT_RXORE_OFFSET	3

#define BIT_TE_OFFSET		3	
#define BIT_RE_OFFSET		2
#define BIT_RXNEIE_OFFSET	5
#define BIT_TCIE_OFFSET		6
#define BIT_TXEIE_OFFSET	7

#define UART_SR_BIT(usart,n)		BIT_VALUE((uint32_t)(usart)+0x00,n)
#define UART_CR1_BIT(usart,n)		BIT_VALUE((uint32_t)(usart)+0x0c,n)
#define UART_CR2_BIT(usart,n)		BIT_VALUE((uint32_t)(usart)+0x10,n)
#define UART_CR3_BIT(usart,n)		BIT_VALUE((uint32_t)(usart)+0x14,n)

//GPIO口地址映射
#define GPIOA_ODR_Addr    (GPIOA_BASE+20) //0x40020014
#define GPIOB_ODR_Addr    (GPIOB_BASE+20) //0x40020414 
#define GPIOC_ODR_Addr    (GPIOC_BASE+20) //0x40020814 
#define GPIOD_ODR_Addr    (GPIOD_BASE+20) //0x40020C14 
#define GPIOE_ODR_Addr    (GPIOE_BASE+20) //0x40021014 
#define GPIOF_ODR_Addr    (GPIOF_BASE+20) //0x40021414    
#define GPIOG_ODR_Addr    (GPIOG_BASE+20) //0x40021814   
#define GPIOH_ODR_Addr    (GPIOH_BASE+20) //0x40021C14    
#define GPIOI_ODR_Addr    (GPIOI_BASE+20) //0x40022014     

#define GPIOA_IDR_Addr    (GPIOA_BASE+16) //0x40020010 
#define GPIOB_IDR_Addr    (GPIOB_BASE+16) //0x40020410 
#define GPIOC_IDR_Addr    (GPIOC_BASE+16) //0x40020810 
#define GPIOD_IDR_Addr    (GPIOD_BASE+16) //0x40020C10 
#define GPIOE_IDR_Addr    (GPIOE_BASE+16) //0x40021010 
#define GPIOF_IDR_Addr    (GPIOF_BASE+16) //0x40021410 
#define GPIOG_IDR_Addr    (GPIOG_BASE+16) //0x40021810 
#define GPIOH_IDR_Addr    (GPIOH_BASE+16) //0x40021C10 
#define GPIOI_IDR_Addr    (GPIOI_BASE+16) //0x40022010 

#define GPIO_ODR(GPIO_BASE,n)				BIT_VALUE((uint32_t)(GPIO_BASE)+20,n)
#define GPIO_IDR(GPIO_BASE,n)				BIT_VALUE((uint32_t)(GPIO_BASE)+16,n)
#define GPIO_ODR_ADDR(GPIO_BASE,n)			BIT_ADDR((uint32_t)(GPIO_BASE)+20,n)
#define GPIO_IDR_ADDR(GPIO_BASE,n)			BIT_ADDR((uint32_t)(GPIO_BASE)+16,n)

#define GPIOA_ODR(n)	BIT_VALUE(GPIOA_ODR_Addr,n)
#define GPIOA_IDR(n)	BIT_VALUE(GPIOA_IDR_Addr,n)
#define GPIOB_ODR(n)	BIT_VALUE(GPIOB_ODR_Addr,n)
#define GPIOB_IDR(n)	BIT_VALUE(GPIOB_IDR_Addr,n)
#define GPIOC_ODR(n)	BIT_VALUE(GPIOC_ODR_Addr,n)
#define GPIOC_IDR(n)	BIT_VALUE(GPIOC_IDR_Addr,n)
#define GPIOD_ODR(n)	BIT_VALUE(GPIOD_ODR_Addr,n)
#define GPIOD_IDR(n)	BIT_VALUE(GPIOD_IDR_Addr,n)
#define GPIOE_ODR(n)	BIT_VALUE(GPIOE_ODR_Addr,n)
#define GPIOE_IDR(n)	BIT_VALUE(GPIOE_IDR_Addr,n)
#define GPIOF_ODR(n)	BIT_VALUE(GPIOF_ODR_Addr,n)
#define GPIOF_IDR(n)	BIT_VALUE(GPIOF_IDR_Addr,n)


///////////DMA///////////////
#define DMA_TCIF7_BIT	27
#define DMA_HTIF7_BIT	26
#define DMA_TEIF7_BIT	25
#define DMA_DMEIF7_BIT	24
#define DMA_FEIF7_BIT	22

#define DMA_TCIF6_BIT	21
#define DMA_HTIF6_BIT	20
#define DMA_TEIF6_BIT	19
#define DMA_DMEIF6_BIT	18
#define DMA_FEIF6_BIT	16

#define DMA_TCIF5_BIT	11
#define DMA_HTIF5_BIT	10
#define DMA_TEIF5_BIT	9
#define DMA_DMEIF5_BIT	8
#define DMA_FEIF5_BIT	6

#define DMA_TCIF4_BIT	5
#define DMA_HTIF4_BIT	4
#define DMA_TEIF4_BIT	3
#define DMA_DMEIF4_BIT	2
#define DMA_FEIF4_BIT	0

#define DMA_TCIF3_BIT	27
#define DMA_HTIF3_BIT	26
#define DMA_TEIF3_BIT	25
#define DMA_DMEIF3_BIT	24
#define DMA_FEIF3_BIT	22

#define DMA_TCIF2_BIT	21
#define DMA_HTIF2_BIT	20
#define DMA_TEIF2_BIT	19
#define DMA_DMEIF2_BIT	18
#define DMA_FEIF2_BIT	16

#define DMA_TCIF1_BIT	11
#define DMA_HTIF1_BIT	10
#define DMA_TEIF1_BIT	9
#define DMA_DMEIF1_BIT	8
#define DMA_FEIF1_BIT	6

#define DMA_TCIF0_BIT	5
#define DMA_HTIF0_BIT	4
#define DMA_TEIF0_BIT	3
#define DMA_DMEIF0_BIT	2
#define DMA_FEIF0_BIT	0

#define DMA_LISR(dma,n)	BIT_VALUE((uint32_t)(dma)+0x00,n)
#define DMA_HISR(dma,n)	BIT_VALUE((uint32_t)(dma)+0x04,n)

#define DMA_CTCIF7_BIT	27
#define DMA_CHTIF7_BIT	26
#define DMA_CTEIF7_BIT	25
#define DMA_CDMEIF7_BIT	24
#define DMA_CFEIF7_BIT	22

#define DMA_CTCIF6_BIT	21
#define DMA_CHTIF6_BIT	20
#define DMA_CTEIF6_BIT	19
#define DMA_CDMEIF6_BIT	18
#define DMA_CFEIF6_BIT	16

#define DMA_CTCIF5_BIT	11
#define DMA_CHTIF5_BIT	10
#define DMA_CTEIF5_BIT	9
#define DMA_CDMEIF5_BIT	8
#define DMA_CFEIF5_BIT	6

#define DMA_CTCIF4_BIT	5
#define DMA_CHTIF4_BIT	4
#define DMA_CTEIF4_BIT	3
#define DMA_CDMEIF4_BIT	2
#define DMA_CFEIF4_BIT	0

#define DMA_CTCIF3_BIT	27
#define DMA_CHTIF3_BIT	26
#define DMA_CTEIF3_BIT	25
#define DMA_CDMEIF3_BIT	24
#define DMA_CFEIF3_BIT	22

#define DMA_CTCIF2_BIT	21
#define DMA_CHTIF2_BIT	20
#define DMA_CTEIF2_BIT	19
#define DMA_CDMEIF2_BIT	18
#define DMA_CFEIF2_BIT	16

#define DMA_CTCIF1_BIT	11
#define DMA_CHTIF1_BIT	10
#define DMA_CTEIF1_BIT	9
#define DMA_CDMEIF1_BIT	8
#define DMA_CFEIF1_BIT	6

#define DMA_CTCIF0_BIT	5
#define DMA_CHTIF0_BIT	4
#define DMA_CTEIF0_BIT	3
#define DMA_CDMEIF0_BIT	2
#define DMA_CFEIF0_BIT	0
#define DMA_LIFCR(dma,n)	BIT_VALUE((uint32_t)(dma)+0x08,n)
#define DMA_HIFCR(dma,n)	BIT_VALUE((uint32_t)(dma)+0x0c,n)


#define DMA_TEIE_BIT	2
#define DMA_DMEIE_BIT	1
#define DMA_EN_BIT		0
#define DMA_STREAM_SXCR(stream,n)	BIT_VALUE((uint32_t)(stream),n)

//////////////  SPI  //////////
#define SPI_RXNE_BIT	0
#define SPI_TXE_BIT		1

#define SPI_BSY_BIT		7
#define SPI_SR(spix,n)		BIT_VALUE((uint32_t)(spix)+0x08,n)

//////////TIM/////////////////////
#define TIM_CEN_BIT			0
#define TIM_UDIS_BIT		1
#define TIM_URS_BIT		2
#define TIM_OPM_BIT		3
#define TIM_DIR_BIT		4

#define TIM_CR1(tim,n)		BIT_VALUE((uint32_t)(tim)+0x00,n)
#define TIM_CR2(tim,n)		BIT_VALUE((uint32_t)(tim)+0x00,n)

#define	TIM_UIE_BIT		0
#define	TIM_CC1IE_BIT	1
#define	TIM_CC2IE_BIT	2
#define	TIM_CC3IE_BIT	3
#define	TIM_CC4IE_BIT	4
#define	TIM_TIE_BIT		6
#define	TIM_UDE_BIT		8
#define TIM_DIER(tim,n)		BIT_VALUE((uint32_t)(tim)+0x0c,n)


/////////// ADC /////////
#define ADC_AWD_BIT		0
#define ADC_EOC_BIT		1
#define ADC_JEOC_BIT	2
#define ADC_JSTRT_BIT	3
#define ADC_STRT_BIT	4
#define ADC_OVR_BIT	5
#define ADC_SR(adcx,n)		BIT_VALUE((uint32_t)(adcx)+0x00,n)
#define ADC_CR1(adcx,n)		BIT_VALUE((uint32_t)(adcx)+0x04,n)
#define ADC_ADON_BIT		0
#define ADC_CONT_BIT		1
#define ADC_DMA_BIT			8
#define ADC_DDS_BIT			9
#define ADC_EOCS_BIT		10
#define ADC_ALIGN_BIT		11
#define ADC_CR2(adcx,n)		BIT_VALUE((uint32_t)(adcx)+0x08,n)


#endif  //__TYPES_H
